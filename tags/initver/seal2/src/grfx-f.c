/******************************************************************
 * SEAL 2.0                                                       *
 * Copyright (c) 1999-2002 SEAL Developers. All Rights Reserved.  *
 *                                                                *
 * Web site: http://sealsystem.sourceforge.net/                   *
 * E-mail (current maintainer): orudge@users.sourceforge.net      *
 ******************************************************************/

/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <seal.h>
#include <grfx-f.h>
#include <aastr.h>

////////////////////////////////////////////////////////////////////////////////
l_bool colcmp( l_color c1, l_color c2, l_int tol  ) {

 return ((abs(getr(c1)-getr(c2)) > tol) || (abs(getg(c1)-getg(c2)) > tol) || (abs(getb(c1)-getb(c2)) > tol));

};
////////////////////////////////////////////////////////////////////////////////
l_color col2col ( l_color c1, l_color c2, l_int nb, l_int max ) {
  l_long r = nb2nb(getr(c1),getr(c2),nb,max);
  l_long g = nb2nb(getg(c1),getg(c2),nb,max);
  l_long b = nb2nb(getb(c1),getb(c2),nb,max);
  return makecol( r,g,b );
};
////////////////////////////////////////////////////////////////////////////////
l_color colNcol ( l_color c1, l_color c2, l_int nb, l_int max ) {
  #define check( x ) (( x < 0 ) ? ( 0 ):( ( x>255 )?( 255 ):(x) ))
  l_long r = nbNnb(getr(c1),getr(c2),nb,max);
  l_long g = nbNnb(getg(c1),getg(c2),nb,max);
  l_long b = nbNnb(getb(c1),getb(c2),nb,max);
  return makecol( check(r),check(g),check(b) );
};
////////////////////////////////////////////////////////////////////////////////
l_color light_color ( l_color col, l_int fact ) { // Fact : 0->255
  return col2col(col , makecol(255,255,255) , fact, 255 );
};
////////////////////////////////////////////////////////////////////////////////
void trans_blit ( BITMAP *src, BITMAP *dst, l_int sx, l_int sy, l_int dx, l_int dy, l_int w, l_int h, l_int nb, l_int max ) {
  if ( !src || !dst ) return;
  if ( get_depth(dst) < 16 ) {
    blit (src,dst,sx,sy,dx,dy,w,h);
  } else {
    BITMAP *buffer = create_bitmap(w, h);
    blit (src,buffer,sx,sy,0,0,w,h);
    set_trans_blender(0, 0, 0, nb*255/max);
    draw_trans_sprite(dst, buffer, dx, dy);
    solid_mode();
    destroy_bitmap (buffer);
  };
};
////////////////////////////////////////////////////////////////////////////////
BITMAP *resize_to_bmp ( BITMAP *bmp, l_int w, l_int h ) {
  if ( bmp && w > 0 && h > 0 ) {
    BITMAP *o = create_bitmap(w,h);
    clear_to_color(o,o->vtable->mask_color);
    if ( !o ) return NULL;
    aa_stretch_sprite(o,bmp,0,0,w,h);
    return o;
  } else return NULL;
};
////////////////////////////////////////////////////////////////////////////////
BITMAP *resizebmp(BITMAP *bmp,l_int w,l_int h) {
  BITMAP *o = resize_to_bmp(bmp,w,h);
  if ( !o ) return bmp;
  destroy_bitmap(bmp);
  return o;
};
////////////////////////////////////////////////////////////////////////////////
// Some adapted function from allegro to get the "do_" option                 //
////////////////////////////////////////////////////////////////////////////////
void do_hline(BITMAP *bmp, int x1, int y, int x2, int color, void (*proc)(BITMAP *, int, int, int)) {
  l_int a = min(x1,x2);
  l_int b = max(x1,x2);
  while ( a <= b ) {
    proc(bmp, a, y, color);
    a++;
  };
};
////////////////////////////////////////////////////////////////////////////////
void do_vline(BITMAP *bmp, int x, int y1, int y2, int color, void (*proc)(BITMAP *, int, int, int)) {
  l_int a = min(y1,y2);
  l_int b = max(y1,y2);
  while ( a <= b ) {
    proc(bmp, x, a, color);
    a++;
  };
};
////////////////////////////////////////////////////////////////////////////////
void do_rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color, void (*proc)(BITMAP *, int, int, int)) {
   int t;

   if (y1 > y2) {
      t = y1;
      y1 = y2;
      y2 = t;
   }

   if (bmp->clip) {
      if (x1 > x2) {
	 t = x1;
	 x1 = x2;
	 x2 = t;
      }

      if (x1 < bmp->cl)
	 x1 = bmp->cl;

      if (x2 >= bmp->cr)
	 x2 = bmp->cr-1;

      if (x2 < x1)
	 return;

      if (y1 < bmp->ct)
	 y1 = bmp->ct;

      if (y2 >= bmp->cb)
	 y2 = bmp->cb-1;

      if (y2 < y1)
	 return;

      bmp->clip = FALSE;
      t = TRUE;
   }
   else
      t = FALSE;

   acquire_bitmap(bmp);

   while (y1 <= y2) {
      do_hline(bmp, x1, y1, x2, color, proc);
      y1++;
   };

   release_bitmap(bmp);

   bmp->clip = t;
}
////////////////////////////////////////////////////////////////////////////////
void do_circlefill(BITMAP *bmp, int x, int y, int radius, int color, void (*proc)(BITMAP *, int, int, int)) {
   int cx = 0;
   int cy = radius;
   int df = 1 - radius;
   int d_e = 3;
   int d_se = -2 * radius + 5;
   int clip, sx, sy, dx, dy;

   if (bmp->clip) {
      sx = x-radius-1;
      sy = y-radius-1;
      dx = x+radius+1;
      dy = y+radius+1;

      if ((sx >= bmp->cr) || (sy >= bmp->cb) || (dx < bmp->cl) || (dy < bmp->ct))
	 return;

      if ((sx >= bmp->cl) && (sy >= bmp->ct) && (dx < bmp->cr) && (dy < bmp->cb))
	 bmp->clip = FALSE;

      clip = TRUE;
   }
   else
      clip = FALSE;

   acquire_bitmap(bmp);

   do {
      do_hline(bmp, x-cy, y-cx, x+cy, color, proc);

      if (cx)
	 do_hline(bmp, x-cy, y+cx, x+cy, color, proc);

      if (df < 0)  {
	 df += d_e;
	 d_e += 2;
	 d_se += 2;
      }
      else {
	 if (cx != cy) {
	    do_hline(bmp, x-cx, y-cy, x+cx, color, proc);

	    if (cy)
	       do_hline(bmp, x-cx, y+cy, x+cx, color, proc);
	 }

	 df += d_se;
	 d_e += 2;
	 d_se += 4;
	 cy--;
      }

      cx++;

   } while (cx <= cy);

   release_bitmap(bmp);

   bmp->clip = clip;
}
////////////////////////////////////////////////////////////////////////////////
void do_rect(BITMAP *bmp, int x1, int y1, int x2, int y2, int c, void (*proc)(BITMAP *, int, int, int)) {
  if ( !bmp || !proc ) return;
  acquire_bitmap(bmp);
  do_vline(bmp, x1, y1, y2, c, proc);
  do_hline(bmp, x1, y1, x2, c, proc);
  do_vline(bmp, x2, y1, y2, c, proc);
  do_hline(bmp, x1, y2, x2, c, proc);
  release_bitmap(bmp);
};
////////////////////////////////////////////////////////////////////////////////
