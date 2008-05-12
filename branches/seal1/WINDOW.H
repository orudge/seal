/****************************************************************/
/*                                                              */
/*                          window.h                            */
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


#ifndef __WINDOW_H_INCLUDED__
#define __WINDOW_H_INCLUDED__

#include<string.h>

typedef struct t_window* p_window;

/*
   palette of window object :
   --------------------------

   standard colors from Seal.ini file, from section [colors] :

   index of palette        name in [colors]

   0                       3D_background
   1                       window_active_title
   2                       window_active_caption
   3                       window_passive_title
   4                       window_passive_caption

*/
extern l_color  pal_window[];


/*

  t_window class

  - window may be moveable or growable. For this purpose we use t_view.drag_mode

  variable ( see t_view class ).

*/
typedef struct t_window {

  /*
    inherit functions and variables from t_view class. You will be able to use these
    functions/vars by VIEW(...) definition. For Example, if we have pointer (win) to
    p_window, that point to previous created window, you can use VIEW(win)->.... for addressing
    all functions and variables from the class t_view. Simply use OBJECT(win)->...
    for addressing all functions/vars from t_object variable. You can do that, becouse
    t_view contains struct of t_object.
  */
  struct t_view  obclass;

  /*
     contains cpation of the window ( title ). This point own memory, where text is
     placed. In the closing of the window, function window_done free this memory.
  */
  l_text         caption;

  /*
     flags for the window. t_window not use it.
  */
  l_int          flags;


  /*
     draws window title. If the title contains other objects you can draw it without
     redrawing these objects by the t_view.set_draw_mode(t_view, DWM_TESTSUBVIEWS, true), and
     then call draw_title, or the better one is using definition TEST_SUB_VIEWS(x).
     Example : TEST_SUB_VIEWS(draw_title(win)); The format of the definitino will find
     in view.h.
  */
  void       (*draw_title) ( p_window o );

} t_window;


/*
   convert object to t_window struct, BUT it must be allocated as t_window or
   as an inherited object from t_window.
*/
#define WINDOW(o)   ((p_window)(o))


/*
  init t_window class and return (o). (o) is previous allocated memory in size
  sizeof(t_window). (r) are bounds of the window ( where is placed ). (caption) is
  the title of the window. Please not use new memory for this title such by the
  function _strdup, becouse win_init function make it by the self. (flags) are flags of
  the window. These flags are used later ...in objects (t_appwin), what's inherited
  object from object t_window ( see app.h ).

  Example :

  t_rect   r   = rect_assign(100, 100, 300, 300);
  p_window win = win_init(_malloc(sizeof(t_window)), r, "Hello Mr. Stencl", 0);
  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(win));

  Insert new (win) to the (desktop) ...(for object (desktop) see program.h)

  ...you must use OBJECT(), becouse (insert) function is the function of the class
  t_object, and arguments are : (p_object own, p_object sub)
*/
extern p_window  (*win_init) ( p_window o, t_rect r, l_text caption, l_int flags );

/*
  _win_init is the main function. win_init is pointer to this function. You can later
  define your own function, if you want....
*/
p_window  _win_init ( p_window o, t_rect r, l_text caption, l_int flags );

/*
  this function is used as an equivalent for "l_bool t_object.done(t_object *)" see object.h.
  win_init set the t_object.done to this form : OBJECT(win)->done = &win_done;
  Calls view_done function and free memory of the t_window.caption.
*/
l_bool    win_done ( p_object o );

/*
  an equivalent for "void t_object.set_state(t_object *, l_dword, l_dword )" see object.h.
  Calls view_set_state function and redraw caption when window is selected or deselected.
*/
void      win_set_state ( p_object o, l_dword st, l_bool set );

/*
  an equivalent for "t_rect t_view.size_limits(t_view * )" see view.h.
  Only return area for subviews. NOTE: if variable t_view.options is set to flag
  VW_OF_INGORELIM, this area is not accepted. For X button, for example.
*/
t_rect    win_size_limits ( p_view o );

/*
  an equivalent for "t_rect t_view.size_minimum(t_view * )" see view.h.
  return minimal sizes for the t_window. Default values are 220, 50. Of-course
  you can change it by your own function for the object :

  static t_point my_size_minumum ( p_view o ) { return point_assign(400, 300); };

  VIEW(win)->size_minumum = &my_size_minimum;
*/
t_point   win_size_minimum ( p_view o );

/*
  an equivalent for "void t_view.draw(t_view * )" see view.h.
  main drawing function of the window. The standard form of this function will find
  in file view.h.
*/
void      win_draw ( p_view o );

/*
  an equivalent for "void t_object.translate_event ( t_object *, t_event *)" see object.h.
  this function is called, whenever some event occurs. It calls function
  view_translate_event, and then are controled events for moving, closing, or
  switching between views.
*/
void      win_translate_event ( p_object o, t_event *event );

/*
  draws window title. The default form of drawing will find in file "view.h"
*/
void      win_draw_title ( p_window o );

/*
  an equivalent for "void t_view.set_mouse_cursor ( t_view * )" see view.h.
  If mouse cursor is placed in the left,bottom corner and the t_view.drag_mode is
  set to flag DM_DRAGGROW, this set mouse cursor to CUR_GROW.
*/
void      win_set_mouse_cursor ( p_view o );

/*
   this is called only once, when Seal start. It's called from the function program_init
   and read colors for pal_window from the seal.ini file.
*/
void      window_ini_palette ( void );


/* end of file window.h */
#endif

