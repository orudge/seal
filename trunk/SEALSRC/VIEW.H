/****************************************************************/
/*                                                              */
/*                           view.h                             */
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



#ifndef __VIEW_H_INCLUDED__
#define __VIEW_H_INCLUDED__

#include"object.h"
#include"alltogrx.h"
#include"mouse.h"
#include"screen.h"



/* drag mode settings in view dragging (moving/resizing) */

#define DM_DRAGMOVE           0x0000001 /* it's possible to drag and move it */
#define DM_DRAGGROW           0x0000002 /* it's possible to drag and grow it */
#define DM_DRAGCONTEXT        0x0000004 /* drag by context */


/* options settings */

#define VW_OF_VISIBILITY      0x0000020 /* it's able to visible */
#define VW_OF_IGNORELIM       0x0000040 /* view ignore owner's limitation */

/* draw mode settings */

#define DWM_TESTSUBVIEWS      0x0000001 /* clip sub views */
#define DWM_ONLYTOBUFFER      0x0000002 /* output is only to buffer */
#define DWM_ONLYTOSCREEN      0x0000004 /* output is only to screen */
#define DWM_CANTACCELMOVE     0x0000008 /* disable accel moving */
#define DWM_ICONICDRAW        0x0000010 /* draw under icon */

/* next alignments */

#define  TX_ALIGN_FIXEDX      0x1000    /* sizey is fixed to size of group */
#define  TX_ALIGN_FIXEDY      0x2000    /* sizey is fixed to size of group */


/* brush states */

#define BRUSH_STRETCH         0x0000001 /* background is stretched */
#define BRUSH_SELFIMG         0x0000002 /* image is only in this brush */
#define BRUSH_GRADIENT        0x0000004 /* color is gradient to color2 */
#define BRUSH_GRADIENT_HOR    0x0000008 /* gradient is horizontal, else is vertical */
#define BRUSH_CENTER          0x0000010 /* center image to the center */


#define OUTBUFFER(o)          (*(o))

#define RETVIEW(o,e)          if ( !(o->state & OB_SF_VISIBLE) || ( e->type == EV_MOUSE && !VIEW(o)->is_mouse_in_view(VIEW(o))) ) return

typedef struct t_brush {

  l_long     state;
  l_color    color;
  BITMAP    *background;
  l_color    color2;
  l_char     reserved[22];

} t_brush;


typedef struct t_view *p_view;




/* info board */

extern l_int   info_board_key;
/* info board foreground color = black */
extern l_color info_board_fcolor;
/* info board background color */
extern l_color info_board_b1color;
/* info board background color */
extern l_color info_board_b2color;
/* info board font = helv15 */
extern l_font *info_board_font;

void   info_board_init ( void );
void   view_ini ( void );


/* size of standard item (menu, listbox, ... )*/

extern l_int   system_item_size;


/* able to drag all context of object */
extern l_bool  drag_context_ok;
/* enable accelaration of moving */
extern l_bool  accel_moving_ok;



typedef struct t_view {

  struct t_object   obclass;

  struct t_brush    brush;
  struct t_rect     bounds;
  struct t_rect     clip;
  struct t_rect    *safe_clip;

  BITMAP           *draw_buffer;
  BITMAP          **draw_out;
  l_font           *font;
  l_int             cursor;

  l_text            info_text;
  l_int             align;

  l_color          *palette;

  l_int             draw_mode;
  l_int             drag_mode;
  l_long            draw_lock;

  l_int             icon_size;

  l_dword (*execute_view) ( p_view o, p_view sub );

  p_view  (*get_top_view_under_mouse) ( p_view o );
  p_view  (*top_view) ( p_view o );
  void    (*show_info_board) ( p_view o );

  p_data  (*drag_data) ( p_view o );
  l_bool  (*drag_where) ( p_view o, p_data rec, t_point where );
  l_bool  (*drop_data) ( p_view o, p_data rec, t_point where );

  void    (*drag_view) ( p_view o, l_word mode, t_event *event );
  t_rect  (*drag) ( p_view o, l_word mode, t_event *event );
  t_point (*size_minimum) ( p_view o );
  t_rect  (*get_limits) ( p_view o );

  void    (*reset_align) ( p_view o );

  void    (*draw) ( p_view o );
  void    (*draw_view) ( p_view o );
  void    (*draw_sub_views) ( p_view o, p_view from, p_view to );

  void    (*set_mouse_cursor) ( p_view o );
  l_bool  (*is_mouse_in_view) ( p_view o );
  p_view  (*get_view_under_mouse) ( p_view o );
  void    (*lock_drawing) ( p_view o );
  l_bool  (*unlock_drawing) ( p_view o );
  l_bool  (*is_draw_lock) ( p_view o );

  BITMAP* (*begin_paint) ( p_view o, t_point *p, t_rect rwhere );
  void    (*end_of_paint) ( p_view o, t_rect rwhere );

  void    (*draw_overlays) ( p_view o );
  l_bool  (*draw_mini_box) ( p_view o, BITMAP *out, BITMAP *buffer, t_rect rwhere );

  void    (*draw_under_view) ( p_view o );
  void    (*draw_under_rect) ( p_view o, t_rect r );
  void    (*draw_in_rect) ( p_view o, t_rect r );
  void    (*draw_me) ( p_view o );

  l_color (*get_color) ( p_view o, l_int cpos );
  void    (*set_palette) ( p_view o, l_color *palette );

  l_bool  (*is_top_view) ( p_view o );
  void    (*move_accel) ( p_view o, t_rect from, t_rect to );
  void    (*move_view) ( p_view o, t_rect r );
  void    (*grow_view) ( p_view o, t_rect r );

  t_rect  (*get_local_extent) ( p_view o );

  t_rect  (*get_global_bounds) ( p_view o, t_rect r );
  t_rect  (*get_local_bounds) ( p_view o, t_rect r );
  t_point (*get_global_point) ( p_view o, t_point p );
  t_point (*get_local_point) ( p_view o, t_point p );

  t_rect* (*get_not_overlays) ( p_view o, t_rect rlocal, t_rect rgroup,
                                p_view p, p_view target, l_long *rnum,
                                t_rect **rout, l_bool sub, l_bool ws );

  void    (*show) ( p_view o );
  void    (*hide) ( p_view o );

  void    (*for_each_sub_view_set_state) ( p_view o, l_dword st, l_bool set );
  void    (*set_draw_mode) ( p_view o, l_int dm, l_bool set );
  l_bool  (*is_draw_mode) ( p_view o, l_int dm );

  t_rect  (*size_limits) ( p_view o );
  t_rect  (*get_cliped_extent) ( p_view o );

  void    (*set_bounds) ( p_view o, t_rect r );

  void    (*change_bounds) ( p_view o, t_rect nr );

  void    (*set_clip) ( p_view o, BITMAP *out, t_rect r );
  void    (*background) ( p_view o, BITMAP *out, t_rect r );

  void    (*set_clips) ( p_view o, t_rect r );
  void    (*reset_clips) ( p_view o );

} t_view;

  #define VIEW(o)   ((p_view)(o))


  #define icon_size(o)  (VIEW(o)->icon_size)

  #define reset_clip(o, out)  set_clip(o, out, o->clip)


/* object's functions */

  void    view_reset_prefer ( p_object o );
  l_bool  view_done ( p_object o );
  void    view_setup ( p_object o );
  void    view_set_state ( p_object o, l_dword st, l_bool set );
  void    view_put_in_front_of ( p_object o, p_object before );
  void    view_translate_event ( p_object o, t_event *event );
  void    view_play_process ( p_object o, t_event *event );


/* view's functions */

  extern p_view  (*view_init) ( p_view o, t_rect r );
  p_view  _view_init ( p_view o, t_rect r );

  l_dword view_execute_view ( p_view o, p_view sub );

  void    view_reset_align ( p_view o );

  p_view  view_get_top_view_under_mouse ( p_view o );
  p_view  view_top_view ( p_view o );
  void    view_show_info_board ( p_view o );
  void    view_set_mouse_cursor ( p_view o );
  l_bool  view_is_mouse_in_view ( p_view o );

  p_data  view_drag_data ( p_view o );
  l_bool  view_drag_where ( p_view o, p_data rec, t_point where );
  l_bool  view_drop_data ( p_view o, p_data rec, t_point where );

  void    view_drag_view ( p_view o, l_word mode, t_event *event );
  t_rect  view_drag ( p_view o, l_word _mode, t_event *event );
  t_point view_size_minimum ( p_view o );
  t_rect  view_get_limits ( p_view o );


  void    view_draw ( p_view o );
  void    view_draw_view ( p_view o );
  void    view_draw_sub_views ( p_view o, p_view from, p_view to );
  void    view_draw_me ( p_view o );

  l_color view_get_color ( p_view o, l_int cpos );
  void    view_set_palette ( p_view o, l_int *palette );

  l_bool  view_is_top_view ( p_view o );
  void    view_move_accel ( p_view o, t_rect from, t_rect to );
  void    view_move_view ( p_view o, t_rect r );
  void    view_grow_view ( p_view o, t_rect r );

  p_view  view_get_view_under_mouse ( p_view o );
  void    view_lock_drawing ( p_view o );
  l_bool  view_unlock_drawing ( p_view o );
  l_bool  view_is_draw_lock ( p_view o );

  BITMAP* view_begin_paint ( p_view o, t_point *p, t_rect rwhere );
  void    view_end_of_paint ( p_view o, t_rect rwhere );

  void    view_draw_overlays ( p_view o );
  l_bool  view_draw_mini_box ( p_view o, BITMAP *out, BITMAP *buffer, t_rect rwhere );

  void    view_draw_under_rect ( p_view o, t_rect r );
  void    view_draw_under_view ( p_view o );
  void    view_draw_in_rect ( p_view o, t_rect r );

  t_rect  view_get_local_extent ( p_view o );

  t_rect  view_get_global_bounds ( p_view o, t_rect r );
  t_rect  view_get_local_bounds ( p_view o, t_rect r );
  t_point view_get_global_point ( p_view o, t_point p );
  t_point view_get_local_point ( p_view o, t_point p );

  t_rect* view_get_not_overlays ( p_view o, t_rect rlocal, t_rect rgroup,
                                  p_view p, p_view target, l_long *rnum,
                                  t_rect **rout, l_bool sub, l_bool ws );

  void    view_show ( p_view o );
  void    view_hide ( p_view o );

  void    view_for_each_sub_view_set_state ( p_view o, l_dword st, l_bool set );
  void    view_set_draw_mode ( p_view o, l_int dm, l_bool set );
  l_bool  view_is_draw_mode ( p_view o, l_int dm );

  t_rect  view_size_limits ( p_view o );
  t_rect  view_get_cliped_extent ( p_view o );

  void    view_set_bounds ( p_view o, t_rect r );

  void    view_change_bounds ( p_view o, t_rect nr );

  void    view_set_clip ( p_view o, BITMAP *out, t_rect r );
  void    view_background ( p_view o, BITMAP *out, t_rect r );

  void    view_set_clips ( p_view o, t_rect r );
  void    view_reset_clips ( p_view o );



void   _show_info_board ( p_view from, l_text text );
l_bool is_top_view_under_mouse_in_me ( p_view o );


#define BLIT(src,dst,sx,sy,dx,dy,w,h)                                    \
do {                                                                     \
    l_bool iv = mouse->is_visible(mouse);                                \
    t_rect sc;                                                           \
    t_rect dc;                                                           \
    get_clip(src, (int*)(&sc.a.x), (int*)(&sc.a.y), (int*)(&sc.b.x), (int*)(&sc.b.y)); \
    get_clip(dst, (int*)(&dc.a.x), (int*)(&dc.a.y), (int*)(&dc.b.x), (int*)(&dc.b.y)); \
    set_clip(dst, dx, dy, dx+w, dy+h);                                   \
    set_clip(src, sx, sy, sx+w, sy+h);                                   \
    mouse->hide(mouse);                                                  \
    blit(src, dst, sx, sy, dx, dy, w+1, h+1);                            \
    if ( iv ) mouse->show(mouse);                                        \
    set_clip(dst, dc.a.x, dc.a.y, dc.b.x, dc.b.y);                       \
    set_clip(src, sc.a.x, sc.a.y, sc.b.x, sc.b.y);                       \
} while (0)

#define TEST_SUB_VIEWS(o, x)                                            \
do {                                                                    \
    l_bool  tsv = VIEW(o)->is_draw_mode(VIEW(o), DWM_TESTSUBVIEWS);     \
    VIEW(o)->set_draw_mode(VIEW(o), DWM_TESTSUBVIEWS, true);            \
    ##x;                                                                \
    VIEW(o)->set_draw_mode(VIEW(o), DWM_TESTSUBVIEWS, tsv);             \
} while (0)

#define NOT_ACCEL_MOVE(o, x)                                            \
do {                                                                    \
    l_bool  tsv = VIEW(o)->is_draw_mode(VIEW(o), DWM_CANTACCELMOVE);    \
    VIEW(o)->set_draw_mode(VIEW(o), DWM_CANTACCELMOVE, true);           \
    ##x;                                                                \
    VIEW(o)->set_draw_mode(VIEW(o), DWM_CANTACCELMOVE, tsv);            \
} while (0)


#define VIEW_CLIPED(o, r) rect_cliped(VIEW(o)->get_cliped_extent(VIEW(o)), r)

#endif

