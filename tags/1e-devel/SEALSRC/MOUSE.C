/****************************************************************/
/*                                                              */
/*                           mouse.c                            */
/*                                                              */
/*                Seal - free desktop environment               */
/*                                                              */
/*                    Copyright (c) 1999,2000                   */
/*                         Michal Stencl                        */
/*                     All Rights Reserved                      */
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


#include"dataini.h"
#include"mouse.h"

t_mouse   *mouse = NULL;

t_mouse*  (*mouse_init) ( p_mouse o ) = &_mouse_init;


DATAFILE  *cursor_system = NULL;
BITMAP    *cursor_standard = NULL;

l_int   msblock0 = 0;
l_int   msblock1 = 0;
l_int   msblock2 = 0;
l_int   msblock3 = 0;
l_int   msblock4 = 0;

l_byte cursor_focus[CUR_DATAMAX*2] = {
     0,0, 2,2, 8,8, 8,8, 8,8, 2,15, 7,6, 7,7, 8,8, 6,6, 6,1, 6,15, 1,6, 15,6, 8,8,
     0,0, 3,3, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0
};

l_int   mouse_flags = 0;
l_color mouse_mode_color = 0;


void  mouse_rect ( struct BITMAP *bmp, int ax, int ay, int bx, int by ) {
  if ( bmp ) {
    l_int r[4] = {bmp->cl, bmp->ct, bmp->cr, bmp->cb};
    drawing_mode(DRAW_MODE_XOR, NULL, 0, 0);
    set_clip(bmp, 0, 0, bmp->w, bmp->h);
    rect(bmp, ax, ay, bx, by, mouse_mode_color);
    set_clip(bmp, r[0], r[1], r[2], r[3]);
    drawing_mode(0, NULL, 0, 0);
  };
};

static END_OF_FUNCTION(mouse_rect)

void _run_mouse_mode ( BITMAP *bmp, int mdx, int mdy, int erase, int show ) {
  if ( bmp )
    switch ( mouse_mode ) {
      case MO_MO_RECT: {
           int r[4] = { mouse_mode_rect_ax, mouse_mode_rect_ay,
                        mouse_mode_rect_bx, mouse_mode_rect_by };
           mouse_mode_rect_ax += mdx;
           mouse_mode_rect_ay += mdy;
           mouse_mode_rect_bx += mdx;
           mouse_mode_rect_by += mdy;
           if ( show ) mouse_rect(bmp, mouse_mode_rect_ax, mouse_mode_rect_ay,
                                      mouse_mode_rect_bx, mouse_mode_rect_by);
           if ( erase ) mouse_rect(bmp, r[0], r[1], r[2], r[3]);
      }; break;
      case MO_MO_GROW: {
           int r[4] = { mouse_mode_rect_ax, mouse_mode_rect_ay,
                        mouse_mode_rect_bx, mouse_mode_rect_by };
           mouse_mode_rect_ax;
           mouse_mode_rect_ay;
           mouse_mode_rect_bx += mdx;
           mouse_mode_rect_by += mdy;
           if ( show ) mouse_rect(bmp, mouse_mode_rect_ax, mouse_mode_rect_ay,
                                      mouse_mode_rect_bx, mouse_mode_rect_by);
           if ( erase ) mouse_rect(bmp, r[0], r[1], r[2], r[3]);
      }; break;
    };
};

static END_OF_FUNCTION(_run_mouse_mode)

void mouse_mode_init ( int _mode, int _color, int ax, int ay, int bx, int by ) {
  if ( mouse_mode && mouse_is_visible() && run_mouse_mode )
    run_mouse_mode(screen, 0, 0, true, false);
  mouse_mode_rect_ax = ax;
  mouse_mode_rect_ay = ay;
  mouse_mode_rect_bx = bx;
  mouse_mode_rect_by = by;
  mouse_mode_color = _color;
  mouse_mode = _mode;
  if ( mouse_mode && mouse_is_visible() && run_mouse_mode )
    run_mouse_mode(screen, 0, 0, false, true);
};


void write_mouse_queue ( int f )
{

    mouse_flags = f<<5;

/*  if ( !mouse_flags || (f & 42) ) mouse_flags = f<<5;*/

};

static END_OF_FUNCTION(write_mouse_queue)

int read_mouse_queue ( void )
{

  int x = mouse_flags;

  mouse_flags = 0;

  return x;

};

static END_OF_FUNCTION(read_mouse_queue)



/* mouse object funstions */

BITMAP*   mouse_get_cursor(t_mouse *o)
{
  return _mouse_sprite;
};


t_rect    mouse_get_range(t_mouse *o)
{
  t_rect r;
  get_mouse_range((int*)(&r.a.x), (int*)(&r.a.y), (int*)(&r.b.x), (int*)(&r.b.y));
  return r;
};


t_point    mouse_get_focus(t_mouse *o)
{
  t_point p;
  get_mouse_focus((int*)&p.x, (int*)&p.y);
  return p;
};


l_bool    xx_mouse_is_visible(t_mouse *o)
{
  return (l_bool)mouse_is_visible();
};


l_bool    mouse_show ( t_mouse *o )
{
  show_mouse(screen);
  if ( o->is_visible(o) ) return true;
  return false;
};


l_bool    mouse_hide(t_mouse *o)
{
  show_mouse(NULL);
  if ( !o->is_visible(o) ) return true;
  return false;
};


l_bool    mouse_set_cursor(t_mouse *o, BITMAP *cursor)
{
  if ( cursor && cursor != o->get_cursor(o) ) {
    set_mouse_sprite(cursor);
    return true;
  };
  return false;
};


l_bool    mouse_set_cursor_focus(t_mouse *o, BITMAP *cursor, t_point p )
{
  if ( o->set_cursor(o, cursor) ) {
    o->set_focus(o, p);
    return true;
  };
  return false;
};


l_bool    mouse_set_mode(t_mouse *o, l_int mode, l_color col, t_rect r )
{
  mouse_mode_init(mode, col, r.a.x, r.a.y, r.b.x, r.b.y);
  return true;
};


void      mouse_set_range(t_mouse *o, t_rect r )
{
  set_mouse_range(r.a.x, r.a.y, r.b.x, r.b.y);
};


void      mouse_set_dclick_diff(p_mouse o, l_int mili)
{

  o->dclick_milisec = mili;

};


void      mouse_set_speed(t_mouse *o, t_point speed )
{
  o->speed = speed;
  set_mouse_speed(speed.x, speed.y);
};


void      mouse_set_pos(t_mouse *o, t_point where )
{
  position_mouse(where.x, where.y);
};


l_int     xx_mouse_block(p_mouse o, t_rect r )
{
  return mouse_block(screen, r.a.x, r.a.y, r.b.x, r.b.y);
};


void      xx_mouse_unblock(p_mouse o, l_int i )
{
  mouse_unblock(i);
};


l_bool    xx_mouse_is_block(p_mouse o )
{
  return (l_bool)mouse_is_block();
};


void      mouse_set_focus(t_mouse *o, t_point focus )
{
  t_point f = o->get_focus(o);
  if ( f.x != focus.x || f.y != focus.y ) {
    l_bool ib = o->is_block(o);
    if ( !ib ) show_mouse(NULL); /* hide mouse */
    set_mouse_sprite_focus(focus.x, focus.y);
    if ( !ib ) show_mouse(screen); /* show mouse */
  };
};


  /* from object */

void      mouse_translate_event ( t_object *o, t_event *event )
{
  l_big diftime = time_diff_mili(((p_mouse)o)->dclick_old_time);

  o->state = read_mouse_queue();

  ((p_mouse)o)->where = point_assign(mouse_x, mouse_y);

  if ( !(o->state & MO_SF_MOUSEUP) ) {

     if ( ((p_mouse)o)->state_last & MO_SF_MOUSELPRESS ) o->state |= MO_SF_MOUSELAUTO;
     if ( ((p_mouse)o)->state_last & MO_SF_MOUSEMPRESS ) o->state |= MO_SF_MOUSEMAUTO;
     if ( ((p_mouse)o)->state_last & MO_SF_MOUSERPRESS ) o->state |= MO_SF_MOUSERAUTO;

  };

  if ( diftime > ((p_mouse)o)->dclick_milisec )

    ((p_mouse)o)->dclick_old_time = 0;


  if ( o->state >= MO_SF_MOUSEMOVE ) {

    if ( o->state & MO_SF_MOUSEDOWN )
      if ( diftime <= ((p_mouse)o)->dclick_milisec &&
           ((p_mouse)o)->state_last & MO_SF_MOUSEUP ) {

        if ( o->state & MO_SF_MOUSELDOWN ) o->state |= MO_SF_MOUSELDOUBLE;
        if ( o->state & MO_SF_MOUSERDOWN ) o->state |= MO_SF_MOUSERDOUBLE;
        if ( o->state & MO_SF_MOUSEMDOWN ) o->state |= MO_SF_MOUSEMDOUBLE;

        o->state &= ~MO_SF_MOUSEAUTO;
        o->state &= ~MO_SF_MOUSEDOWN;

        ((p_mouse)o)->dclick_old_time = 0;

      } else

        ((p_mouse)o)->dclick_old_time = time_get_mili();


    ((p_mouse)o)->state_last = o->state;


    set_event(event, EV_MOUSE, MSG_NOTHING, o);

  } else {

    (*event).type &= ~EV_MOUSE;

  };

};


void    clear_event_mouse ( void )
{

/*  mouse_flags = 0;

  OBJECT(mouse)->state &= ~(0xFFFFFFFF-MO_SF_MOUSEMOVE);
  mouse->state_last = OBJECT(mouse)->state;*/

};


l_bool  mouse_set_cursor_system_file ( l_char *cursorfile, l_int r, l_int g, l_int b )
{
  DATAFILE *p = cursor_load_file(cursorfile, r, g, b);
  if ( p ) {
    cursor_system = p;
    cursor_standard = CURSOR_GETCURSOR(p, CUR_ARROW);
    if ( MOUSE ) mouse_set_cursor_id(CUR_ARROW);
    return true;
  };
  return false;
};


l_bool  mouse_load_cursors ( void )
{
  l_char *name  = getini_fromfile(INI_MAINFILE, INI_MOUSE, "cursors_file");

  l_bool ok = mouse_set_cursor_system_file(name, CUR_SKIP_RED, CUR_SKIP_GREEN, CUR_SKIP_BLUE);

  _free(name);

  return ok;
};


static void   *mouse_callback_old = NULL;

/* stop timer of the mouse */
static void    mouse_stop_timer ( void )
{

  mouse_callback_old = mouse_callback;

  remove_mouse();

  mouse_callback = NULL;

};


/* reload timer of the mouse */
static void    mouse_reload_timer ( void )
{

  install_mouse();

  LOCK_VARIABLE(mouse_flags);
  LOCK_VARIABLE(mouse_mode_color);
  LOCK_FUNCTION(mouse_rect);
  LOCK_FUNCTION(_run_mouse_mode);
  LOCK_FUNCTION(write_mouse_queue);
  LOCK_FUNCTION(read_mouse_queue);

  mouse_callback = mouse_callback_old;

  mouse->set_speed(mouse, mouse->speed);
};


l_bool      mouse_done ( t_object *o )
{

  if ( !obj_done(o) ) return false;

  erase_safe_timer(&mouse_stop_timer);

  ((p_mouse)o)->hide((p_mouse)o);

  remove_mouse();

  mouse_callback = NULL;

  return true;

};


/* mouse init function */

t_mouse *_mouse_init ( t_mouse *o )
{
  l_int  x = install_mouse();

  l_long speed = getininum_fromfile (INI_MAINFILE, INI_MOUSE, "speed");

  DEBUG_printf("\n...initializing mouse object\n"); /* error message, if some occures */

  if ( !o ) return NULL;

  mouse = o;

  clear_type(o, sizeof(t_mouse));

  obj_init(&(o->obclass));

  o->buttons = (l_char)x;

  if ( x <= 0 || !_mouse_installed ) { /* mouse is not installed */

    o->buttons = 0;

    DEBUG_printf(" - ERROR :: mouse driver not found\n"); /* error message, if some occures */

  } else { /* mouse is installed */

     safe_timer(&mouse_stop_timer, &mouse_reload_timer);

     mouse_callback = &write_mouse_queue;
     run_mouse_mode = &_run_mouse_mode;
     LOCK_VARIABLE(mouse_flags);
     LOCK_VARIABLE(mouse_mode_color);
     LOCK_FUNCTION(mouse_rect);
     LOCK_FUNCTION(_run_mouse_mode);
     LOCK_FUNCTION(write_mouse_queue);
     LOCK_FUNCTION(read_mouse_queue);

     DEBUG_printf(" - found %i button's mouse\n", o->buttons); /* error message, if some occures */

  };

  o->speed = point_assign(speed, speed);
  o->where = point_assign(0, 0);


  /* function settings */
  o->get_cursor = &mouse_get_cursor;
  o->get_focus = &mouse_get_focus;
  o->get_range = &mouse_get_range;

  o->is_visible = &xx_mouse_is_visible;
  o->show = &mouse_show;
  o->hide = &mouse_hide;
  o->set_dclick_diff = &mouse_set_dclick_diff;
  o->set_cursor = &mouse_set_cursor;
  o->set_cursor_focus = &mouse_set_cursor_focus;
  o->set_mode = &mouse_set_mode;
  o->set_range = &mouse_set_range;
  o->set_speed = &mouse_set_speed;
  o->set_focus = &mouse_set_focus;
  o->set_pos   = &mouse_set_pos;

  o->block = &xx_mouse_block;
  o->unblock = &xx_mouse_unblock;
  o->is_block = &xx_mouse_is_block;

  o->obclass.done = &mouse_done;
  o->obclass.translate_event = &mouse_translate_event;

  o->set_dclick_diff(o, MOUSE_TIME_DOUBLECLICK);

  OBJECT(o)->set_options(OBJECT(o), OB_OF_NOTACTIVATE+OB_OF_TOPSELECT, true);

  mouse_load_cursors();

  set_mouse_speed(o->speed.x, o->speed.y);

  mouse->show(mouse); /* show mouse */

  DEBUG_printf("   mouse was succesfull installed at speed %i\n", o->speed.x); /* error message, if some occures */

  return o;

};

