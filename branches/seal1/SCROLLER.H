/****************************************************************/
/*                                                              */
/*                          scroller.h                          */
/*                                                              */
/*                Seal - free desktop environment               */
/*                                                              */
/*                  Copyright (c) 1999,2000                     */
/*                       Michal Stencl                          */
/*                    All Rights Reserved                       */
/*                                                              */
/* mail : stenclpmd@ba.telecom.sk                               */
/* web  : http://www.home.sk/public/seal/                       */
/*                                                              */
/* This file is part of Seal.                                   */
/*                                                              */
/* Seal is free software; you can redistribute it and/or        */
/* modify it under the terms of the GNU General Public License  */
/* as published by the Free Software Foundation; either version */
/* 2, or (at your option) any later version.                    */
/*                                                              */
/* Seal is distributed in the hope that it will be useful, but  */
/* WITHOUT ANY WARRANTY; without even the implied warranty of   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See    */
/* the GNU General Public License for more details.             */
/*                                                              */
/* You should have received a copy of the GNU General Public    */
/* License along with Seal; see the file COPYING.  If not,      */
/* write to the Free Software Foundation, 675 Mass Ave,         */
/* Cambridge, MA 02139, USA.                                    */
/*                                                              */
/****************************************************************/



#ifndef __SCROLLER_H_INCLUDED__
#define __SCROLLER_H_INCLUDED__

#include <math.h>

/* scroll after move types */

#define SCROLL_NOW        false
#define SCROLL_AFTER      true


/* scrollbar sizes */

#define STANDARD_SCROLLBAR_WIDTH    15
#define MINIMUM_SCROLLBAR_WIDTH     10
#define MINIMUM_SCROLLBAR_SIZE      25
#define MINIMUM_SCROLLBOX_SIZE      25
#define STANDARD_SCROLLBUTTON_SIZE  10


/* scrollbar flags */

#define SBF_SCROLL_AFTER_MOVE       0x0001


/* scroller flags for scrollbars */

#define SF_HORSCROLLBAR   0x00001
#define SF_VERSCROLLBAR   0x00002



extern l_bool   scroll_after; /* if set, scrolling place is redraw after scrolling */

extern l_color  pal_scrollbar[];


typedef struct t_scroller *p_scroller;

typedef struct t_hscrollbar *p_hscrollbar;
typedef struct t_vscrollbar *p_vscrollbar;


/* t_hscrollbar */

typedef struct t_hscrollbar {

  struct t_view   obview;

  p_scroller      scroller;
  l_int           flags;

  void          (*draw_bar) ( p_hscrollbar o );
  l_rect        (*get_bar_size) ( p_hscrollbar o );
  l_rect        (*get_bar_pos) ( p_hscrollbar o );
  l_long        (*get_real_pos) ( p_hscrollbar o, l_rect pos );

} t_hscrollbar;

#define HSCROLLBAR(o)   ((p_hscrollbar)(o))



/* t_vscrollbar */

typedef struct t_vscrollbar {

  struct t_view   obview;

  p_scroller      scroller;
  l_int           flags;

  void          (*draw_bar) ( p_vscrollbar o );
  l_rect        (*get_bar_size) ( p_vscrollbar o );
  l_rect        (*get_bar_pos) ( p_vscrollbar o );
  l_long        (*get_real_pos) ( p_vscrollbar o, l_rect pos );

} t_vscrollbar;

#define VSCROLLBAR(o)   ((p_vscrollbar)(o))


/* t_scroller */

typedef struct t_scroller {

  struct t_view   obclass;

  l_long          scrollx;
  l_long          scrolly;
  l_int           deltax;
  l_int           deltay;

  l_long          aftmovex;
  l_long          aftmovey;

  l_int           flags;
  p_hscrollbar    horbar;
  p_vscrollbar    verbar;

  l_char          scroll_auto;

  t_rect (*scroll_limits) ( p_scroller o );
  void   (*scroll_size) ( p_scroller o, l_long *x, l_long *y );
  void   (*scroll_place) ( p_scroller o, l_long dx, l_long dy, l_bool aft_move );
  void   (*recalc_positions) ( p_scroller o, l_long dx, l_long dy );
  void   (*max_moves) ( p_scroller o, l_long *dx, l_long *dy );

} t_scroller;

#define SCROLLER(o)   ((p_scroller)(o))

#define draw_scroller_bars(o) do {                                      \
  if ( SCROLLER(o)->horbar ) VIEW(SCROLLER(o)->horbar)->draw_view(VIEW(SCROLLER(o)->horbar));     \
  if ( SCROLLER(o)->verbar ) VIEW(SCROLLER(o)->verbar)->draw_view(VIEW(SCROLLER(o)->verbar));     \
} while (0)



#define calc_maxscrollx(o,x) (ceil((double)(x) / (double)lmax(1, SCROLLER(o)->deltax))*(double)SCROLLER(o)->deltax)
#define calc_maxscrolly(o,y) (ceil((double)(y) / (double)lmax(1, SCROLLER(o)->deltay))*(double)SCROLLER(o)->deltay)


#define reset_scroller(o) do {                                      \
   SCROLLER(o)->scrollx = 0;                                        \
   SCROLLER(o)->scrolly = 0;                                        \
} while (0)


/* hscrollbar functions */

void    hscrollbar_translate_event ( p_object o, t_event *event );
t_rect  hscrollbar_size_limits ( p_view o );
t_point hscrollbar_size_minimum ( p_view o );
void    hscrollbar_draw ( p_view o );

void   hscrollbar_draw_bar ( p_hscrollbar o );
l_rect hscrollbar_get_bar_size ( p_hscrollbar o );
l_rect hscrollbar_get_bar_pos ( p_hscrollbar o );
l_long hscrollbar_get_real_pos ( p_hscrollbar o, l_rect pos );

extern p_hscrollbar   (*hscrollbar_init) ( p_hscrollbar o, t_rect r, p_scroller f );

p_hscrollbar   _hscrollbar_init ( p_hscrollbar o, t_rect r, p_scroller f );



/* vscrollbar functions */

void    vscrollbar_translate_event ( p_object o, t_event *event );
t_rect  vscrollbar_size_limits ( p_view o );
t_point vscrollbar_size_minimum ( p_view o );
void    vscrollbar_draw ( p_view o );

void   vscrollbar_draw_bar ( p_vscrollbar o );
l_rect vscrollbar_get_bar_size ( p_vscrollbar o );
l_rect vscrollbar_get_bar_pos ( p_vscrollbar o );
l_long vscrollbar_get_real_pos ( p_vscrollbar o, l_rect pos );

extern p_vscrollbar   (*vscrollbar_init) ( p_vscrollbar o, t_rect r, p_scroller f );

p_vscrollbar   _vscrollbar_init ( p_vscrollbar o, t_rect r, p_scroller f );



/* scroller functions */

l_bool scroller_done ( p_object o );
void   scroller_setup ( p_object o );
void   scroller_change_bounds ( p_view o, t_rect nr );
void   scroller_background ( p_view o, BITMAP *out, t_rect r );

t_rect scroller_scroll_limits ( p_scroller o );
void   scroller_recalc_positions ( p_scroller o, l_long x, l_long y );
void   scroller_scroll_size ( p_scroller o, l_long *x, l_long *y );
void   scroller_scroll_place ( p_scroller o, l_long dx, l_long dy, l_bool aft_move );
void   scroller_max_moves ( p_scroller o, l_long *dx, l_long *dy );

extern p_scroller   (*scroller_init) ( p_scroller o, t_rect r, l_int flags );

p_scroller   _scroller_init ( p_scroller o, t_rect r, l_int flags );

#endif

