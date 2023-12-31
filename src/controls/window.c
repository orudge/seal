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
#include <window.h>


p_window  (*win_init) ( p_window o, t_rect r, l_text caption, l_int flags ) = &_win_init;

/*
  this function is used as an equivalent for "l_bool t_object.done(t_object *)" see object.h.

  win_init set the t_object.done to this form : OBJECT(win)->done = &win_done;

  Calls view_done function and free memory of the t_window.caption.
*/
l_bool  win_done ( p_object o )
{

  if ( !view_done(o) ) return false;

  /* free memory of caption string and set the pointer to ZERO */
  afree(&(WINDOW(o)->caption));

  return true;

};


/*
  an equivalent for "void t_object.set_state(t_object *, l_dword, l_dword )" see object.h.

  Calls view_set_state function and redraw caption when window is selected or deselected.
*/
void  win_set_state ( p_object o, l_dword st, l_bool set )
{
  view_set_state(o, st, set);

  if ( st & OB_SF_SELECTED ) {

    /* draw title if windows is selected/deselected.
       also test sub-views in the title. ! not redrawn them !
    */
    TEST_SUB_VIEWS(o, WINDOW(o)->draw_title(WINDOW(o)));
    view_draw_on_rect(VIEW(o),rect_assign(2, 2, rect_sizex(VIEW(o)->bounds)-2,/*FONT_GETHEIGHT(VIEW(o)->font)*/16+3));
  };
};


/*
  an equivalent for "t_rect t_view.size_limits(t_view * )" see view.h.

  Only return area for subviews. NOTE: if variable t_view.options is set to flag

  VW_OF_INGORELIM, this area is not accepted. For X button, for example.
*/
t_rect  win_size_limits ( p_view o )
{
  /* r = { 0, 0, width of object, height of object }*/
  t_rect r = o->get_local_extent(o);

  r.a.x += 2;
  r.a.y += 16+4/*FONT_GETHEIGHT(VIEW(o)->font)+4*/; /* place for title */
  r.b.x -= 2;
  r.b.y -= 2;

  return r;
};


/*
  an equivalent for "t_rect t_view.size_minimum(t_view * )" see view.h.

  return minimal sizes for the t_window. Default values are 220, 50. Of-course

  you can change it by your own function for the object :

  static t_point my_size_minumum ( p_view o ) { return point_assign(400, 300); };

  VIEW(win)->size_minumum = &my_size_minimum;
*/
t_point   win_size_minimum ( p_view o )
{

  return point_assign(220, 50);

};


/*
  an equivalent for "void t_view.draw(t_view * )" see view.h.

  main drawing function of the window. The standard form of this function will find

  in file view.h.
*/
void  win_draw ( p_view o )
{

  /* r = { 0, 0, width of object, height of object }*/
  t_rect  r = o->get_local_extent(o);
  t_point p;

  /* start to draw in rect (r) and return delta (p.x,p.y) from 0,0 of the screen
     return output for the drawing ( default : t_view.draw_buffer - virtual_screen ).
     When last end_of_paint is called, it's drawn to t_view.draw_out ( screen )
  */
  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    o->background(o, out, rect_move(r, p.x, p.y));

    button3d(o,out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y,0);

    if ( o->drag_mode & DM_DRAGGROW ) {

	  l_uchar border = ( o->brush.state & BRUSH_LARGE3D ) ? 2 : 1;

      line(out, p.x+r.b.x-border-2, p.y+r.b.y-border-1, p.x+r.b.x-border-1, p.y+r.b.y-border-2, color_3d_light);
      line(out, p.x+r.b.x-border-3, p.y+r.b.y-border-1, p.x+r.b.x-border-1, p.y+r.b.y-border-3, color_3d_shadow);

      line(out, p.x+r.b.x-border-6, p.y+r.b.y-border-1, p.x+r.b.x-border-1, p.y+r.b.y-border-6, color_3d_light);
      line(out, p.x+r.b.x-border-7, p.y+r.b.y-border-1, p.x+r.b.x-border-1, p.y+r.b.y-border-7, color_3d_shadow);

      line(out, p.x+r.b.x-border-10, p.y+r.b.y-border-1, p.x+r.b.x-border-1, p.y+r.b.y-border-10, color_3d_light);
      line(out, p.x+r.b.x-border-11, p.y+r.b.y-border-1, p.x+r.b.x-border-1, p.y+r.b.y-border-11, color_3d_shadow);

    };

    /* draw title of the window */
    WINDOW(o)->draw_title(WINDOW(o));

  };

  /* end of drawing. if this is the last call of end_of_paint, it draws context to
     t_view.draw_out, from (out)... by default t_view.draw_buffer.
  */
  o->end_of_paint(o, r);



};


/*
  an equivalent for "void t_view.set_mouse_cursor ( t_view * )" see view.h.

  If mouse cursor is placed in the left,bottom corner and the t_view.drag_mode is

  set to flag DM_DRAGGROW, this set mouse cursor to CUR_GROW.
*/
void      win_set_mouse_cursor ( p_view o )
{
   /* make mouse->where to local bounds */
   t_point m = VIEW(o)->get_local_point ( VIEW(o), mouse->where );
   /* r = { 0, 0, width of the view, height of the view } */
   t_rect  r = VIEW(o)->get_local_extent ( VIEW(o) );

   /* possible to grow, and mouse is on the left,bottom corner */
   if (  o->drag_mode & DM_DRAGGROW && m.x > r.b.x - 10 && m.x < r.b.x &&
         m.y > r.b.y - 10 && m.y < r.b.y )

      /* set mouse cursor to CUR_GROW */
      mouse_set_sys_cursor(CUR_GROW);


   else

      /* set mouse cursor to t_view.cursor - CUR_ARROW */
      view_set_mouse_cursor(o);

};


/*
  an equivalent for "void t_object.translate_event ( t_object *, t_event *)" see object.h.

  this function is called, whenever some event occurs. It calls function

  view_translate_event, and then are controled events for moving, closing, or

  switching between views.
*/
void      win_translate_event ( p_object o, t_event *event )
{

  /* return back, if window is not visible or mouse is not in the object and
     event comes from the mouse
  */
  RETVIEW(o, event);

  /* event comes from mouse - see mouse.h */
  if ( event->type & EV_MOUSE ) { /* mouse event */

    /* left mouse button was pressed */
    if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) { /* select */

      /* select window */
      o->select(o); /* if it was selected before, make nothing */

    };

    /* left mouse button was pressed */
    if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) { /* growing */

        t_point m = VIEW(o)->get_local_point ( VIEW(o), mouse->where );
        t_rect  r = VIEW(o)->get_local_extent ( VIEW(o) );

        /* check if mouse is in left, bottom corrner */
        if (  m.x > r.b.x - 10 && m.x < r.b.x && m.y > r.b.y - 10 && m.y < r.b.y )
        /* and if isn't maximized (if FLAG 0x08 is set then window is maximized) */
        if ( !(WINDOW(o)->flags & 0x08) && VIEW(o)->drag_mode & DM_DRAGGROW ) {


             l_word dm = (VIEW(o)->is_top_view(VIEW(o)) && VIEW(o)->drag_mode & DM_DRAGCONTEXT)?DM_DRAGCONTEXT:0;

             /* ok, grow object */
             VIEW(o)->drag_view(VIEW(o), DM_DRAGGROW+dm, event);

             /* clear event, now event->type is EV_NOTHING, etc... */
             clear_event(event);

        };

    };

  };

  /* calls other objects placed in the window */
  view_translate_event(o, event); /* old translate_event function */

  if (( event->type & EV_MOUSE) && (!(WINDOW(o)->flags & 0x08))) { /* 0x08 is WF_MAXSIZE */

    /* left mouse button was pressed */
    if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) { /* dragging */

      t_point m = VIEW(o)->get_local_point ( VIEW(o), mouse->where );
      t_rect  r = VIEW(o)->get_local_extent ( VIEW(o) );
      t_rect  s = VIEW(o)->size_limits(VIEW(o));

      /* check if mouse is in the caption */
      if (  m.x > r.a.x + 2 && m.x < r.b.x -2 && m.y > r.a.y + 2 && m.y < r.a.y + s.a.y -2 )
      {

           l_word dm = (VIEW(o)->is_top_view(VIEW(o)) && VIEW(o)->drag_mode & DM_DRAGCONTEXT)?DM_DRAGCONTEXT:0;

           /* move window */
           VIEW(o)->drag_view(VIEW(o), DM_DRAGMOVE+dm, event);

           /* clear event, now event->type is EV_NOTHING, etc... */
           clear_event(event);
      };

    } /*else

    // right mouse button was pressed
      if ( OBJECT(mouse)->state & MO_SF_MOUSERDOWN ) { // dragging

      l_word dm = (VIEW(o)->is_top_view(VIEW(o)) && VIEW(o)->drag_mode & DM_DRAGCONTEXT)?DM_DRAGCONTEXT:0;

      // move window
      VIEW(o)->drag_view(VIEW(o), DM_DRAGMOVE+dm, event);

      // clear event, now event->type is EV_NOTHING, etc...

      clear_event(event);

    };*/

  };


  /* event comes from keyboard */
  if ( event->type & EV_KEYBOARD ) { /* keyboard event */

    /* ALT+TAB was pressed */
    if ( keyb->code == TO_KEY(KB_TAB) ) { /* select next view */

        p_object v = NULL;

        /* exist preferable object */
        if ( o->prefer )

           /* find next object placed in window, that's selectable and visible */
           v = o->prefer->find_match_view(o->prefer, OB_SF_VISIBLE, OB_OF_SELECTABLE, true);

        /* clear event -> we want only one to make this */
  	     clear_event(event);

        /* select previous found object */
        if ( v ) v->select(v);

    };

    /* ALT+F4 was pressed */
    if ( keyb->code == TO_ALT(KB_F4) ) {

      /* ok.... we are going to close the window */
      set_event(event, EV_MESSAGE, MSG_CLOSE, o);
      /* set program queue to event -> start in the next process */
      o->put_event(o, event);
      /* current process clear */
      clear_event(event);

    };
  };

  if ( event->type & EV_MESSAGE ) { /* message event */

    switch ( event->message ) {

      /* messages MSG_OK, or MSG_CLOSE occured */
      case MSG_OK :
      case MSG_CLOSE : {

        if ( !o->is_state(o, OB_SF_MODAL) )
           /* done (hide) the window and free memory of the window and of all
              subobjects, see "object.h"
           */
           dispose(o);

        else
        /* if object is modal = others wait for ending this one.
           see "object.h" l_dword t_object.execute(t_object *) +
               "view.h" l_dword execute_view(t_view *, t_view *)
        */
           o->end_state = event->message;

        /* clear current process */

        clear_event(event);

      }; break;

    };
  };

};


/*
  draws window title. The default form of drawing will find in file "view.h"
*/
void  win_draw_title ( p_window o )
{

  t_rect  r = rect_assign(2, 2, rect_sizex(VIEW(o)->bounds)-2,
                          FONT_GETHEIGHT(VIEW(o)->font)+2);
  t_point p;

  /* start to draw in rect (r) and return delta (p.x,p.y) from 0,0 of the screen
     return output for the drawing ( default : t_view.draw_buffer - virtual_screen ).
     When last end_of_paint is called, it's drawn to t_view.draw_out ( screen )
  */
  BITMAP *out = VIEW(o)->begin_paint(VIEW(o), &p, r);

  if ( out ) {
//    /* get 4th color from the palette t_view.palette = window_passive_caption */
    l_color fcolor = color_window_pass_title_text;
//    /* get 2th color from the palette t_view.palette = window_passive_title */
    l_color bcolor = color_window_pass_title_face;
    l_color bcolorg = color_window_pass_title_face_gardient;
//    /* return formated text */
    //l_text  caption = set_format_text(NULL, "  %s  ", o->caption);
//
//    /* find the center of the title in (y) */
    l_int dy = (rect_sizey(r) - FONT_GETHEIGHT(VIEW(o)->font)) / 2;
//
//    /* window is selected ? */
    if ( OBJECT(o)->state & OB_SF_SELECTED ) {
//
//      /* get 2th color from the palette t_view.palette = window_active_title */
      fcolor = color_window_act_title_text;
//      /* get 1th color from the palette t_view.palette = window_active_caption */
      bcolor = color_window_act_title_face;
      bcolorg = color_window_act_title_face_gardient;
//
    };

    /* draw filled rectangle by the color (bcolor) ...see above */

    fade_rect(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, bcolor, bcolorg, FR_HOR);

    /* draw text by the font t_view.font, text (caption), length -1 = strlen, ... you know */
    textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x, r.a.y+p.y+1,
                           r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTER, fcolor, CO_NOCOLOR, 0);

    /* free memory last allocated by function set_format_text ...see above */
    //sf_free(caption);

  };

  /* end of drawing. if this is the last call of end_of_paint, it draws context to
     t_view.draw_out, from (out)... by default t_view.draw_buffer.
  */
  VIEW(o)->end_of_paint(VIEW(o), r);

};


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
p_window  _win_init ( p_window o, t_rect r, l_text caption, l_int flags )
{
  if ( !o ) return NULL;

  /* set memory of pointer to ZERO */
  clear_type(o, sizeof(t_window));

  /* call old initialization function */
  view_init(VIEW(o), r);

  /* object's declarations */

  OBJECT(o)->tag |= TAG_WINDOW;


  /* view's declarations */

  /* enable dragmove, and dragcontext only if move_window_context is set to non-zero
     in seal.ini file in section [optimalization]...see "view.h" function
     view_ini ( void )
  */
  VIEW(o)->drag_mode |=  DM_DRAGMOVE+drag_context_ok*DM_DRAGCONTEXT;
  VIEW(o)->draw_mode &= ~(DWM_CANTACCELMOVE*accel_moving_ok);


  /* window's declarations */

  /* allocate memory for (caption) and copy string from (caption) to new place */
  o->caption = (l_text)_strdup((char*)caption);

  o->flags = flags;


  /* object's functions */

  /* set old functions to new */
  OBJECT(o)->set_state = &win_set_state;
  OBJECT(o)->translate_event = &win_translate_event;
  OBJECT(o)->done = &win_done;

  /* view's functions */

  VIEW(o)->draw = &win_draw;
  VIEW(o)->size_limits = &win_size_limits;
  VIEW(o)->size_minimum = &win_size_minimum;
  VIEW(o)->set_mouse_cursor = &win_set_mouse_cursor;


  /* window functions */

  o->draw_title = &win_draw_title;


  /* function calling */

  /* object can be selected on the top */
  OBJECT(o)->set_options(OBJECT(o), OB_OF_TOPSELECT, true);

  /* set palette of the window from standard window palette */
//  VIEW(o)->set_palette(VIEW(o), pal_window);

  /* set font of "caption" to bold system font. declaration is in "driver.c" file */
  VIEW(o)->font = get_font_in_size("ARIAL",8,8);

  /* set background color of window to first palette (pal_window) color.
     this is used when function "t_view.background(t_view*, r)" is used in draw function.
  */
  VIEW(o)->brush.color = color_3d_face;

  return o;

};

