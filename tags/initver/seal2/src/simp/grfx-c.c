// 326 Lines
/*

GRFX technologies

(c) Copyright 2001 Julien Etelain. All rights reserved.

*/
#define  byebyebmp(x) if ( x ) { destroy_bitmap( x ); x = NULL; };

#include"grfx-c.h"


////////////////////////////////////////////////////////////////////////////////
l_int actualborder = 1;
l_int actualpenstyle = PEN_RECT;
l_int actualfloodlevel = 100; // (full ...)
l_int actualfloodmode = FLOOD_NO;
BITMAP *demon_bmp = NULL;
////////////////////////////////////////////////////////////////////////////////
// DO FLOOD adapted from allegro ...                                          //
////////////////////////////////////////////////////////////////////////////////
typedef struct FLOODED_LINE
{
   short flags;
   short lpos, rpos;
   short y;
   short next;
} FLOODED_LINE;


static int flood_count;

#define FLOOD_IN_USE             1
#define FLOOD_TODO_ABOVE         2
#define FLOOD_TODO_BELOW         4

#define FLOOD_LINE(c)            (((FLOODED_LINE *)_scratch_mem) + c)
////////////////////////////////////////////////////////////////////////////////
l_bool colcmp( l_color c1, l_color c2, l_int tol  ) {
  return abs(getr(c1)-getr(c2)) > tol || abs(getg(c1)-getg(c2)) > tol || abs(getb(c1)-getb(c2)) > tol;
};
////////////////////////////////////////////////////////////////////////////////
static int do_flooder(BITMAP *bmp, int x, int y, int src_color, int dest_color, void (*proc)(BITMAP *, int, int, int), int tol )
{
   FLOODED_LINE *p;
   int left = 0, right = 0;
   unsigned long addr;
   int c;

      if (colcmp ( getpixel(bmp, x, y), src_color, tol ))
	 return x+1;

      for (left=x-1; left>=bmp->cl; left--)
	 if (colcmp (getpixel(bmp, left, y), src_color, tol ))
	    break;

      for (right=x+1; right<bmp->cr; right++)
	 if (colcmp (getpixel(bmp, right, y), src_color, tol ))
	    break;


   left++;
   right--;

   do_hline(bmp, left, y, right, dest_color, proc);

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
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
void do_floodfill(BITMAP *bmp, int x, int y, int color, void (*proc)(BITMAP *, int, int, int), int tol)
{
   int src_color;
   int c, done;
   FLOODED_LINE *p;

   if ((x < bmp->cl) || (x >= bmp->cr) || (y < bmp->ct) || (y >= bmp->cb))
      return;

   acquire_bitmap(bmp);

   src_color = getpixel(bmp, x, y);
   if (src_color == color) {
      release_bitmap(bmp);
      return;
   }

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

   do_flooder(bmp, x, y, src_color, color, proc, tol);

   do {
      done = TRUE;

      for (c=0; c<flood_count; c++) {

	 p = FLOOD_LINE(c);

	 if (p->flags & FLOOD_TODO_BELOW) {
	    p->flags &= ~FLOOD_TODO_BELOW;
	    if (do_check_flood_line(bmp, p->y+1, p->lpos, p->rpos, src_color, color, proc,tol)) {
	       done = FALSE;
	       p = FLOOD_LINE(c);
	    }
	 }

	 if (p->flags & FLOOD_TODO_ABOVE) {
	    p->flags &= ~FLOOD_TODO_ABOVE;
	    if (do_check_flood_line(bmp, p->y-1, p->lpos, p->rpos, src_color, color, proc,tol)) {
	       done = FALSE;
	       if ((c < bmp->cb) && (c > 0))
		  c -= 2;
	    }
	 }
      }

   } while (!done);

   release_bitmap(bmp);
}
////////////////////////////////////////////////////////////////////////////////
// All put pixel functions                                                    //
////////////////////////////////////////////////////////////////////////////////
void putpixel4 (BITMAP *bmp, int x, int y, int color) {

if ( color == -1 )
putpixel(bmp, x, y, getpixel(demon_bmp,x,y));
else
putpixel(bmp, x, y, color);

};
////////////////////////////////////////////////////////////////////////////////
void putpixel3 (BITMAP *bmp, int x, int y, int color) {

if ( actualfloodmode != FLOOD_NO )
putpixel(bmp, x, y, col2col(getpixel(bmp,x,y),color, actualfloodlevel, 100));
else
putpixel(bmp, x, y, color);

};
////////////////////////////////////////////////////////////////////////////////
void putpixel2 (BITMAP *bmp, int x, int y, int color) {
if ( actualborder != 1 )  {

if (actualfloodmode != FLOOD_LINEAR ) {
    if ( actualpenstyle == PEN_RECT )
       do_rectfill(bmp, x - ((actualborder-1) / 2), y - ((actualborder-1) / 2) , x + ((actualborder-1) / 2), y + ((actualborder-1) / 2) ,color, putpixel3);
    else if ( actualpenstyle == PEN_CIRCLE )
       do_circlefill(bmp, x, y, ((actualborder-1) / 2), color, putpixel3);
} else {
    l_int b = (actualborder-1) / 2;
    l_int a = 0;

    while ( a <= b ) {

    if ( actualpenstyle == PEN_RECT )
       do_rectfill(bmp, x - b + a, y - b + a , x + b - a, y + b - a, color, putpixel3);
    else if ( actualpenstyle == PEN_CIRCLE )
       do_circlefill(bmp, x, y, a , color, putpixel3);

    a++;
    };
};

} else putpixel3(bmp,x,y,color);

};
////////////////////////////////////////////////////////////////////////////////
// Main function for DO FLOOD                                                 //
////////////////////////////////////////////////////////////////////////////////
void floodit ( BITMAP *bmp, l_int x, l_int y, l_color new_color, l_int tol ) {
  do_floodfill ( bmp , x , y , new_color, putpixel4, tol );
};
////////////////////////////////////////////////////////////////////////////////
// Gradient engine                                                            //
////////////////////////////////////////////////////////////////////////////////
void x_gardient ( BITMAP *xmp, l_int x1, l_int y1, l_int x2, l_int y2, l_color c1,
                  l_color c2, l_int style, l_int set1, l_int set2 ) {
  l_int w     = abs(x1-x2);
  l_int h     = abs(y1-y2);
  BITMAP *bmp = create_bitmap(w,h);
  l_int x     = w * set1 / 100;
  l_int y     = h * set2 / 100;
  l_int cx    = w / 2;
  l_int cy    = h / 2;

  if ( !bmp ) return;

  rectfill(bmp,0,0,IMAGE_WIDTH(bmp), IMAGE_HEIGHT(bmp),c2);

  if ( style == GARDIENT_LINE ) {

    // Not yet supported ...

  } else if ( style == GARDIENT_RECT ) {
   l_int t = max( max ( x , abs(w - x)  ), max ( y , abs(h - y)  ) );
   l_int v = t;

    while ( v >= 0  ) {
      rectfill( bmp, x-v, y-v, x+v, y+v, col2col( c1, c2, v, t ) );
      v--;
    };
  } else if ( style == GARDIENT_CIRCLE ) {
    l_int t = max( max ( x , abs(w - x)  ), max ( y , abs(h - y) ) );
    l_int v = t;
    while ( v >= 0  ) {
      circlefill( bmp, x, y, v, col2col( c1, c2, v, t ) );
      v--;
    };
  } else if ( style == GARDIENT_RADIAL ) {
    l_int vx = 0;
    l_int vy = 0;
    l_int t = ( w + h );
    l_int v = 0;
    vx = w;
    vy = 0;
    while ( vy <= h ) {
      line(bmp,x,y,vx,vy,col2col(c1,c2,v,t));
      v++;
      vy++;
    };
    while ( vx >= 0 ) {
      line(bmp,x,y,vx,vy,col2col(c1,c2,v,t));
      v++;
      vx--;
    };
    while ( vy >= 0 ) {
      line(bmp,x,y,vx,vy,col2col(c1,c2,v,t));
      v--;
      vy--;
    };
    while ( vx <= w ) {
      line(bmp,x,y,vx,vy,col2col(c1,c2,v,t));
      v--;
      vx++;
    };
  };
  draw_sprite( xmp , bmp , x1 , y1 );
  byebyebmp(bmp);
};

////////////////////////////////////////////////////////////////////////////////
// 'Light' a color in %                                                       //
////////////////////////////////////////////////////////////////////////////////
l_color light_color ( l_color col, l_int fact ) { // Fact : 0->100
  return col2col(col , COLOR(CO_WHITE) , fact, 100);
};
////////////////////////////////////////////////////////////////////////////////
// Lines & Formes functions                                                   //
////////////////////////////////////////////////////////////////////////////////
void go_draw_it_sub ( BITMAP *out, l_int x1, l_int y1, l_int x2, l_int y2, l_color color, l_bool fill, l_dword forme ) {
  if ( forme == FORME_LINE ) do_line(out, x1, y1, x2, y2, color, putpixel2);
  if ( fill ) {
    if (forme == FORME_RECT) rectfill(out, x1, y1, x2, y2, color);
    else if (forme == FORME_CIRCLE) circlefill(out, x1 , y1 , max(abs( x2 - x1),abs( y2 - y1)), color);
    else if (forme == FORME_OVAL) ellipsefill(out, x1, y1, abs(x2 - x1), abs(y2 - y1), color);
  } else {
    if (forme == FORME_RECT)  do_rect(out, x1, y1, x2, y2, color, putpixel2);
    else if (forme == FORME_CIRCLE) do_circle(out, x1 , y1 , max(abs( x2 - x1),abs( y2 - y1)), color,putpixel2);
    else if (forme == FORME_OVAL) do_ellipse(out, x1, y1, abs(x2 - x1), abs(y2 - y1), color,putpixel2);
  };
};
////////////////////////////////////////////////////////////////////////////////
void go_draw_it ( BITMAP *out, l_int x1, l_int y1, l_int x2, l_int y2, l_color c1, l_color c2, l_int fillmode, l_int border, l_dword forme, l_dword penstyle ) {
  // Set new modes
  actualborder     = 1;
  actualfloodmode  = FLOOD_NO;
  actualfloodlevel = 100;
  // draw the inside
  if ( fillmode == FILL_COL1 ) go_draw_it_sub(out, x1, y1, x2, y2, c1, TRUE, forme);
  else if ( fillmode == FILL_COL2 ) go_draw_it_sub(out, x1, y1, x2, y2, c2, TRUE, forme);
  // Set new mode
  actualpenstyle   = penstyle;
  actualborder     = border;
  // Draw the border
  go_draw_it_sub(out, x1, y1 , x2 , y2, c1, FALSE, forme);
};
////////////////////////////////////////////////////////////////////////////////
void go_draw_point ( BITMAP *out, l_int x, l_int y,  l_color c,  l_int border, l_dword penstyle, l_int floodmode, l_int floodlevel ) {
  // Set modes
  actualborder     = border;
  actualpenstyle   = penstyle;
  actualfloodmode  = floodmode;
  actualfloodlevel = floodlevel;
  // Draw
  putpixel2 (out, x, y, c);
};
////////////////////////////////////////////////////////////////////////////////
void go_draw_line ( BITMAP *out, l_int x1, l_int y1, l_int x2, l_int y2, l_color c1, l_int border, l_dword penstyle, l_int floodmode, l_int floodlevel ) {
  // Set new mode
  actualpenstyle   = penstyle;
  actualborder     = border;
  actualfloodmode  = floodmode;
  actualfloodlevel = floodlevel;
  // Draw the border
  go_draw_it_sub(out, x1, y1 , x2 , y2, c1, FALSE, FORME_LINE);
};
////////////////////////////////////////////////////////////////////////////////
