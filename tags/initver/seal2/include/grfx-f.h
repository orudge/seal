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

#ifndef _GRFX_FUNCTIONS_H_
#define _GRFX_FUNCTIONS_H_

#define nb2nb(  a,  b,  nb,  max )   a + ( (b - a)  * nb / max )
#define nbNnb(  a,  b,  nb,  max )   a - ( (b - a)  * nb / max )

#define opnb2nb   nbNnb
#define opcol2col colNcol

void do_rect(BITMAP *bmp, int x1, int y1, int x2, int y2, int c, void (*proc)(BITMAP *, int, int, int));
void do_circlefill(BITMAP *bmp, int x, int y, int radius, int color, void (*proc)(BITMAP *, int, int, int));
void do_rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color, void (*proc)(BITMAP *, int, int, int));
void do_vline(BITMAP *bmp, int x, int y1, int y2, int color, void (*proc)(BITMAP *, int, int, int));
void do_hline(BITMAP *bmp, int x1, int y, int x2, int color, void (*proc)(BITMAP *, int, int, int));
BITMAP *resizebmp(BITMAP *bmp,l_int w,l_int h);
BITMAP *resize_to_bmp ( BITMAP *bmp, l_int w, l_int h );
void trans_blit ( BITMAP *src, BITMAP *dst, l_int sx, l_int sy, l_int dx, l_int dy, l_int w, l_int h, l_int nb, l_int max );
l_color light_color ( l_color col, l_int fact );
l_color colNcol ( l_color c1, l_color c2, l_int nb, l_int max );
l_color col2col ( l_color c1, l_color c2, l_int nb, l_int max );
l_bool colcmp( l_color c1, l_color c2, l_int tol  );

#define FILL_NO       0
#define FILL_COL1     1
#define FILL_COL2     2

#define FORME_LINE    0
#define FORME_RECT    1
#define FORME_CIRCLE  2
#define FORME_OVAL    3

#define PEN_RECT      0
#define PEN_CIRCLE    1

#define TRANS_NO      0
#define TRANS_FULL    1
#define TRANS_LINEAR  2

#define GARDIENT_LINE   0
#define GARDIENT_RECT   1
#define GARDIENT_CIRCLE 2
#define GARDIENT_RADIAL 3

#endif
