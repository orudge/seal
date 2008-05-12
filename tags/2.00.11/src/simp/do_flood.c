/*         ______   ___    ___ 
 *        /\  _  \ /\_ \  /\_ \ 
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___ 
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/  * * G R F X * *
 *
 *      The do_floodfill routine.
 *             (part of GRFX)
 *
 *      By Shawn Hargreaves.
 *
 *      See readme.txt for copyright information.
 *
 *      Adapted by Julien Etelain for do_xxx and tolerance ....
 */




typedef struct FLOODED_LINE      /* store segments which have been flooded */
{
   short flags;                  /* status of the segment */
   short lpos, rpos;             /* left and right ends of segment */
   short y;                      /* y coordinate of the segment */
   short next;                   /* linked list if several per line */
} FLOODED_LINE;


static int flood_count;          /* number of flooded segments */

#define FLOOD_IN_USE             1
#define FLOOD_TODO_ABOVE         2
#define FLOOD_TODO_BELOW         4

#define FLOOD_LINE(c)            (((FLOODED_LINE *)_scratch_mem) + c)



/* flooder:
 *  Fills a horizontal line around the specified position, and adds it
 *  to the list of drawn segments. Returns the first x coordinate after 
 *  the part of the line which it has dealt with.
 */

l_bool colcmp( l_color c1, l_color c2, l_int tol  ) { // c1 !tol c2

return ((abs(getr(c1)-getr(c2)) > tol) || (abs(getg(c1)-getg(c2)) > tol) || (abs(getb(c1)-getb(c2)) > tol));

};



static int do_flooder(BITMAP *bmp, int x, int y, int src_color, int dest_color, void (*proc)(BITMAP *, int, int, int), int tol )
{
   FLOODED_LINE *p;
   int left = 0, right = 0;
   unsigned long addr;
   int c;


      /* check start pixel */
      if (colcmp ( getpixel(bmp, x, y), src_color, tol ))
	 return x+1;

      /* work left from starting point */ 
      for (left=x-1; left>=bmp->cl; left--)
	 if (colcmp (getpixel(bmp, left, y), src_color, tol ))
	    break;

      /* work right from starting point */ 
      for (right=x+1; right<bmp->cr; right++)
	 if (colcmp (getpixel(bmp, right, y), src_color, tol ))
	    break;


   left++;
   right--;

   /* draw the line */
   do_hline(bmp, left, y, right, dest_color, proc);

   /* store it in the list of flooded segments */
   c = y;
   p = FLOOD_LINE(c);

   if (p->flags) {
      while (p->next) {
	 c = p->next;
	 p = FLOOD_LINE(c);
      }

      p->next = c = flood_count++;
      _grow_scratch_mem(sizeof(FLOODED_LINE) * flood_count);
      p = FLOOD_LINE(c);
   }

   p->flags = FLOOD_IN_USE;
   p->lpos = left;
   p->rpos = right;
   p->y = y;
   p->next = 0;

   if (y > bmp->ct)
      p->flags |= FLOOD_TODO_ABOVE;

   if (y+1 < bmp->cb)
      p->flags |= FLOOD_TODO_BELOW;

   return right+2;
}



/* check_flood_line:
 *  Checks a line segment, using the scratch buffer is to store a list of 
 *  segments which have already been drawn in order to minimise the required 
 *  number of tests.
 */
static int do_check_flood_line(BITMAP *bmp, int y, int left, int right, int src_color, int dest_color, void (*proc)(BITMAP *, int, int, int), int tol)
{
   int c;
   FLOODED_LINE *p;
   int ret = FALSE;

   while (left <= right) {
      c = y;

      for (;;) {
	 p = FLOOD_LINE(c);

	 if ((left >= p->lpos) && (left <= p->rpos)) {
	    left = p->rpos+2;
	    break;
	 }

	 c = p->next;

	 if (!c) {
	    left = do_flooder(bmp, left, y, src_color, dest_color, proc,tol);
	    ret = TRUE;
	    break; 
	 }
      }
   }

   return ret;
}



/* floodfill:
 *  Fills an enclosed area (starting at point x, y) with the specified color.
 */
void do_floodfill(BITMAP *bmp, int x, int y, int color, void (*proc)(BITMAP *, int, int, int), int tol)
{
   int src_color;
   int c, done;
   FLOODED_LINE *p;

   /* make sure we have a valid starting point */ 
   if ((x < bmp->cl) || (x >= bmp->cr) || (y < bmp->ct) || (y >= bmp->cb))
      return;

   acquire_bitmap(bmp);

   /* what color to replace? */
   src_color = getpixel(bmp, x, y);
   if (src_color == color) {
      release_bitmap(bmp);
      return;
   }

   /* set up the list of flooded segments */
  _grow_scratch_mem(sizeof(FLOODED_LINE) * bmp->cb);
   flood_count = bmp->cb;
   p = _scratch_mem;
   for (c=0; c<flood_count; c++) {
      p[c].flags = 0;
      p[c].lpos = SHRT_MAX;
      p[c].rpos = SHRT_MIN;
      p[c].y = y;
      p[c].next = 0;
   }

   /* start up the flood algorithm */
   do_flooder(bmp, x, y, src_color, color, proc, tol);

   /* continue as long as there are some segments still to test */
   do {
      done = TRUE;

      /* for each line on the screen */
      for (c=0; c<flood_count; c++) {

	 p = FLOOD_LINE(c);

	 /* check below the segment? */
	 if (p->flags & FLOOD_TODO_BELOW) {
	    p->flags &= ~FLOOD_TODO_BELOW;
	    if (do_check_flood_line(bmp, p->y+1, p->lpos, p->rpos, src_color, color, proc,tol)) {
	       done = FALSE;
	       p = FLOOD_LINE(c);
	    }
	 }

	 /* check above the segment? */
	 if (p->flags & FLOOD_TODO_ABOVE) {
	    p->flags &= ~FLOOD_TODO_ABOVE;
	    if (do_check_flood_line(bmp, p->y-1, p->lpos, p->rpos, src_color, color, proc,tol)) {
	       done = FALSE;
	       /* special case shortcut for going backwards */
	       if ((c < bmp->cb) && (c > 0))
		  c -= 2;
	    }
	 }
      }

   } while (!done);

   release_bitmap(bmp);
}

