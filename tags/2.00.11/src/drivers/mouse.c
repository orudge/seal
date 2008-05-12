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
#include <drivers.h>
#include <registry.h>
#include <mouse.h>
#include <ico.h>
#include <stimer.h>
#include <screen.h>

////////////////////////////////////////////////////////////////////////////////

t_mouse   *mouse = NULL;

t_mouse*  (*mouse_init) ( p_mouse o ) = &_mouse_init;

p_cursor mouse_cursor;

p_cursor system_cursors[17];

l_int   msblock0 = 0;

l_int   msblock1 = 0;

l_int   msblock2 = 0;

l_int   msblock3 = 0;

l_int   msblock4 = 0;

l_int   mouse_flags = 0;

extern int mouse_x_focus;

extern int mouse_y_focus;

int mouse_on=1;


l_text registry_cursor[] = { "system/cursors/arrow", "system/cursors/move",
                             "system/cursors/grow",  "system/cursors/text",
                             "system/cursors/clock", "system/cursors/pencil",
                             "system/cursors/target","system/cursors/target2",
                             "system/cursors/stop",  "system/cursors/zoom",
                             "system/cursors/scrup", "system/cursors/scddn",
                             "system/cursors/scrlf", "system/cursors/scrrg",
                             "system/cursors/text2", "system/cursors/drag",
                             "system/cursors/finger", NULL };

l_int mouse_minx=0,mouse_miny=0,mouse_maxx=0x7fffffff,mouse_maxy=0x7fffffff;
////////////////////////////////////////////////////////////////////////////////
l_bool mouse_is_visible ( void ) {

 return mouse_on;

}
////////////////////////////////////////////////////////////////////////////////
void get_mouse_focus ( int *x, int *y ) {
  *x = mouse_x_focus;
  *y = mouse_y_focus;
}
extern int _mouse_x;
extern int _mouse_y;
////////////////////////////////////////////////////////////////////////////////
#define mouse_block(b,ax,ay,bx,by) ({scare_mouse();/*_area(ax,ay,bx-ax+1,by-ay+1);*/mouse_on=0;1;})
#define mouse_unblock(x) ({unscare_mouse();mouse_on=1;})
#define mouse_is_block() (mouse_on==0)
////////////////////////////////////////////////////////////////////////////////
void get_mouse_range(int *ax,int *ay, int *bx, int *by){
   *ax=mouse_minx;*ay=mouse_miny;*bx=mouse_maxx;*by=mouse_maxy;
}
////////////////////////////////////////////////////////////////////////////////
void      mouse_set_range(t_mouse *o, t_rect r )
{
  mouse_minx=r.a.x;mouse_miny=r.a.y;mouse_maxx=r.b.x;mouse_maxy=r.b.y;
  set_mouse_range(r.a.x, r.a.y, r.b.x, r.b.y);
};
////////////////////////////////////////////////////////////////////////////////
p_cursor  mouse_get_cursor(t_mouse *o)
{
  return mouse_cursor;
};
////////////////////////////////////////////////////////////////////////////////
l_bool    mouse_set_cursor(t_mouse *o, p_cursor cursor) {
  if ( cursor && !is_same_bitmap(cursor->bmp,mouse_sprite) ) {
    set_mouse_sprite(cursor->bmp);
    o->set_focus(o, cursor->focus);
    mouse_cursor=cursor;
    return true;
  };
  return false;
};
////////////////////////////////////////////////////////////////////////////////
void write_mouse_queue ( int f )
{
    mouse_flags = f<<5;
};
static END_OF_FUNCTION(write_mouse_queue)
////////////////////////////////////////////////////////////////////////////////
int read_mouse_queue ( void )
{
  int x = mouse_flags;
  mouse_flags = 0;
  return x;
};
#ifndef __RSXNT__
static END_OF_FUNCTION(read_mouse_queue)
#endif
////////////////////////////////////////////////////////////////////////////////
t_rect    mouse_get_range(t_mouse *o)
{
  t_rect r;
  get_mouse_range((int*)(&r.a.x), (int*)(&r.a.y), (int*)(&r.b.x), (int*)(&r.b.y));
  return r;
};
////////////////////////////////////////////////////////////////////////////////
/* mouse object functions */
t_point    mouse_get_focus(t_mouse *o)
{
  t_point p;
  get_mouse_focus((int*)&p.x, (int*)&p.y);
  return p;
};
////////////////////////////////////////////////////////////////////////////////
l_bool    xx_mouse_is_visible(t_mouse *o)
{
  return (l_bool)mouse_is_visible();
};
////////////////////////////////////////////////////////////////////////////////
l_bool    mouse_show ( t_mouse *o )
{
  show_mouse(screen);
  mouse_on=1;
  if ( o->is_visible(o) ) return true;
  return false;
};
////////////////////////////////////////////////////////////////////////////////
l_bool    mouse_hide(t_mouse *o)
{
  show_mouse(NULL);
  mouse_on=0;
  if ( !o->is_visible(o) ) return true;
  return false;
};
////////////////////////////////////////////////////////////////////////////////
void      mouse_set_dclick_diff(p_mouse o, l_int mili)
{
  o->dclick_milisec = mili;
};
////////////////////////////////////////////////////////////////////////////////
void      mouse_set_speed(t_mouse *o, t_point speed )
{
  o->speed = speed;
  set_mouse_speed(speed.x, speed.y);
};
////////////////////////////////////////////////////////////////////////////////
void      mouse_set_pos(t_mouse *o, t_point where )
{
  position_mouse(where.x, where.y);
};
////////////////////////////////////////////////////////////////////////////////
l_int     xx_mouse_block(p_mouse o, t_rect r )
{
  return mouse_block(screen, r.a.x, r.a.y, r.b.x, r.b.y);
};
////////////////////////////////////////////////////////////////////////////////
void      xx_mouse_unblock(p_mouse o, l_int i )
{
  mouse_unblock(i);
};
////////////////////////////////////////////////////////////////////////////////
l_bool    xx_mouse_is_block(p_mouse o )
{
  return (l_bool)mouse_is_block();
};
////////////////////////////////////////////////////////////////////////////////
void      mouse_set_focus(t_mouse *o, t_point focus )
{
  t_point f = o->get_focus(o);
  if ( (f.x != focus.x) || (f.y != focus.y) ) {
    l_bool ib = o->is_block(o);
    if ( !ib ) show_mouse(NULL); /* hide mouse */
    set_mouse_sprite_focus(focus.x, focus.y);
    if ( !ib ) show_mouse(screen); /* show mouse */
  };
};
////////////////////////////////////////////////////////////////////////////////
int ddoldy;
int ddoldx;


////////////////////////////////////////////////////////////////////////////////
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

    if ( o->state & MO_SF_MOUSEDOWN ) {
      if ( diftime <= (((p_mouse)o)->dclick_milisec) &&
           (((p_mouse)o)->state_last & MO_SF_MOUSEUP) ) {
        if ( o->state & MO_SF_MOUSELDOWN ) o->state |= MO_SF_MOUSELDOUBLE;
        if ( o->state & MO_SF_MOUSERDOWN ) o->state |= MO_SF_MOUSERDOUBLE;
        if ( o->state & MO_SF_MOUSEMDOWN ) o->state |= MO_SF_MOUSEMDOUBLE;
        o->state &= ~MO_SF_MOUSEAUTO;
        o->state &= ~MO_SF_MOUSEDOWN;
        ((p_mouse)o)->dclick_old_time = 0;
      } else {
        ((p_mouse)o)->dclick_old_time = time_get_mili();
         ddoldx = mouse_x;
         ddoldy = mouse_y;
      }
    }
    if ( ((p_mouse)o)->state_last & MO_SF_MOUSEDRAG && o->state & MO_SF_MOUSEUP ) {
      o->state &= ~MO_SF_MOUSEDRAG;
    }
    if ( !(((p_mouse)o)->state_last & MO_SF_MOUSEDRAG) && o->state & MO_SF_MOUSELAUTO && (abs(ddoldx - mouse_x)>2 || abs(ddoldy - mouse_y)>2) ) {
      o->state |= MO_SF_MOUSEDRAG;
    }
    ((p_mouse)o)->state_last = o->state;

    set_event(event, EV_MOUSE, MSG_NOTHING, o);
  } else {
    (*event).type &= ~EV_MOUSE;
  };
  screensaver_translate_event(NULL,event);
};
////////////////////////////////////////////////////////////////////////////////
void    clear_event_mouse ( void )
{

};
////////////////////////////////////////////////////////////////////////////////
p_cursor get_registry_cursor ( l_int id ) {
  if ( id >= 0 && id < 18 ) {
    l_text key = registry_cursor[id];
    l_text file;
    if ( !key ) return NULL;
    file = get_key(key);
    if ( !file ) return NULL;
    return load_cur ( file, 256 );
  };
  return NULL;
};
////////////////////////////////////////////////////////////////////////////////
l_bool  mouse_load_cursors ( void )
{
  l_int a = 0;
  while ( registry_cursor[a] ) {
    system_cursors[a] = get_registry_cursor(a);
    a++;
  };
  return 1;
};
////////////////////////////////////////////////////////////////////////////////
static void   *mouse_callback_old = NULL;

static void    mouse_stop_timer ( void )
{
  mouse_callback_old = mouse_callback;
  remove_mouse();
  mouse_callback = NULL;
};
////////////////////////////////////////////////////////////////////////////////
static void    mouse_reload_timer ( void )
{
  install_mouse();
  LOCK_FUNCTION(write_mouse_queue);
  LOCK_FUNCTION(read_mouse_queue);

  mouse_callback = mouse_callback_old;
  mouse->set_speed(mouse, mouse->speed);
};
////////////////////////////////////////////////////////////////////////////////
l_bool      mouse_done ( t_object *o )
{
  if ( !obj_done(o) ) return false;
  erase_safe_timer(&mouse_stop_timer);
  ((p_mouse)o)->hide((p_mouse)o);
  remove_mouse();
  mouse_callback = NULL;
  return true;
};
////////////////////////////////////////////////////////////////////////////////
t_mouse *_mouse_init ( t_mouse *o )
{
  l_int  x = install_mouse();
  l_long speed = get_key_integer("system/mouse/speed");
  l_long dblclick = get_key_integer("system/mouse/dblclick");
  DEBUG_printf("\n...initializing mouse object\n"); /* error message, if some occures */
  if ( !o ) return NULL;
  mouse = o;
  clear_type(o, sizeof(t_mouse));
  obj_init(&(o->obclass));
  o->buttons = (l_char)x;
  if ( (x <= 0) /*|| !_mouse_installed*/ ) { /* mouse is not installed */
    o->buttons = 0;
    DEBUG_printf(" - ERROR :: mouse driver not found - try ctmouse.exe\n"); /* error message, if some occures */

    system ( "ctmouse.exe");
    x = install_mouse();
    if ( (x <= 0) /*|| !_mouse_installed*/ ) {
      DEBUG_printf(" - ERROR :: no mouse ??? Shut down SEAL\n");
      screen_halt();
      printf("Fatal error : No mouse found !\n");
      exit(1);
    };
    o->buttons = (l_char)x;
  } //else { /* mouse is installed */

     safe_timer(&mouse_stop_timer, &mouse_reload_timer);
     mouse_callback = &write_mouse_queue;

     LOCK_FUNCTION(write_mouse_queue);
     LOCK_FUNCTION(read_mouse_queue);
     DEBUG_printf(" - found %i button's mouse\n", o->buttons); /* error message, if some occures */
  //};
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
  o->set_range = &mouse_set_range;
  o->set_speed = &mouse_set_speed;
  o->set_focus = &mouse_set_focus;
  o->set_pos   = &mouse_set_pos;
  o->block = &xx_mouse_block;
  o->unblock = &xx_mouse_unblock;
  o->is_block = &xx_mouse_is_block;
  o->obclass.done = &mouse_done;
  o->obclass.translate_event = &mouse_translate_event;
  o->set_dclick_diff(o, dblclick);
  OBJECT(o)->set_options(OBJECT(o), OB_OF_NOTACTIVATE+OB_OF_TOPSELECT, true);
  mouse_load_cursors();
  set_mouse_speed(o->speed.x, o->speed.y);
  mouse_set_sys_cursor(0);

  mouse->hide(mouse);

  mouse_maxx=screen_width-1;
  mouse_maxy=screen_height-1;
  DEBUG_printf("   mouse was succesfull installed at speed %i\n", o->speed.x);

  return o;
};
////////////////////////////////////////////////////////////////////////////////
