/*

(c) Copyright 2001 Julien Etelain. All rights reserved.

*/

#ifndef __GRFX_C_H_INCLUDED__
#define __GRFX_C_H_INCLUDED__

#define FILL_NO       0
#define FILL_COL1     1
#define FILL_COL2     2

#define FORME_LINE    0
#define FORME_RECT    1
#define FORME_CIRCLE  2
#define FORME_OVAL    3

#define PEN_RECT      0
#define PEN_CIRCLE    1

#define FLOOD_NO      0
#define FLOOD_FULL    1
#define FLOOD_LINEAR  2

#define GARDIENT_LINE   0
#define GARDIENT_RECT   1
#define GARDIENT_CIRCLE 2
#define GARDIENT_RADIAL 3

void floodit ( BITMAP *bmp, l_int x, l_int y, l_color new_color, l_int tol );

void go_draw_it_sub ( BITMAP *out, l_int x1, l_int y1, l_int x2, l_int y2, l_color color, l_bool fill, l_dword forme );

void go_draw_it ( BITMAP *out, l_int x1, l_int y1, l_int x2, l_int y2, l_color c1, l_color c2, l_int fillmode, l_int border, l_dword forme, l_dword penstyle );

void go_draw_point ( BITMAP *out, l_int x, l_int y,  l_color c,  l_int border, l_dword penstyle, l_int floodmode, l_int floodlevel );

void go_draw_line ( BITMAP *out, l_int x1, l_int y1, l_int x2, l_int y2, l_color c1, l_int border, l_dword penstyle, l_int floodmode, l_int floodlevel );

void x_gardient ( BITMAP *xmp, l_int x1, l_int y1, l_int x2, l_int y2, l_color c1, l_color c2, l_int style, l_int set1, l_int set2 );

#endif

