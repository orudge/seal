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
#include <keyboard.h>

t_keyboard   *keyb = NULL;

t_keyboard*   (*key_init) ( p_keyboard o ) = &_key_init;

void   key_simulate_keypress ( p_keyboard o, l_int keycode )
{
  simulate_keypress(keycode);
};


void   key_set_ctrl_alt_del_flag ( p_keyboard o, l_bool enable )
{
  three_finger_flag = (int)enable;
};


l_bool key_get_ctrl_alt_del_flag ( p_keyboard o )
{
  return three_finger_flag;
};


void   key_clear_buffer ( p_keyboard o )
{
  clear_keybuf();
};


l_bool key_keypressed ( p_keyboard o )
{
  return keypressed();
};


l_int  key_readkey ( p_keyboard o )
{
  return readkey();
};


void  key_translate_event ( t_object *o, t_event *event )
{
  t_keyboard* kb = ((t_keyboard*)o);
  o->state &= ~KB_SF_KEYDOWN;
  kb->shifts = key_shifts;
  if ( kb->keypressed(kb) )  {

    kb->code = (kb)->readkey(kb);
    kb->keychar = (char)TO_CHAR(kb->code);
    if ( kb->code ) o->state |= KB_SF_KEYDOWN;
    kb->clear_buffer( kb );
  } else kb->code = kb->keychar;
  if ( o->state & KB_SF_KEYDOWN ) {
    set_event(event, EV_KEYBOARD, MSG_NOTHING, o);
  } else {
    (*event).type &= ~EV_KEYBOARD;
  };
  screensaver_translate_event(NULL,event);
};


static void   *keyboard_callback_old = NULL;
/* stop timer of the keyboard */
static void    key_stop_timer ( void )
{
  keyboard_callback_old = keyboard_callback;
  remove_keyboard();
  keyboard_callback = NULL;
};

/* reload timer of the keyboard */
static void    key_reload_timer ( void )
{
  install_keyboard();
  keyboard_callback = keyboard_callback_old;
};


l_bool  key_done ( t_object *o )
{
  if ( !obj_done(o) ) return false;
  remove_keyboard();
  keyboard_callback = NULL;
  erase_safe_timer(&key_stop_timer);
  return true;
};

void _pckeys_init();

t_keyboard*   _key_init ( p_keyboard o )
{
  if ( !o ) return NULL;
  memset(o, 0, sizeof(t_keyboard));
  keyb = o;
  obj_init(&(o->obclass));
  install_keyboard();
  _pckeys_init();
  /* safe timer */
  safe_timer(&key_stop_timer, &key_reload_timer);
  o->simulate_keypress = &key_simulate_keypress;
  o->set_ctrl_alt_del_flag = &key_set_ctrl_alt_del_flag;
  o->get_ctrl_alt_del_flag = &key_get_ctrl_alt_del_flag;
  o->readkey = &key_readkey;
  o->clear_buffer = &key_clear_buffer;
  o->keypressed = &key_keypressed;
  o->obclass.translate_event = &key_translate_event;
  o->obclass.done = &key_done;
  OBJECT(o)->set_options(OBJECT(o), OB_OF_NOTACTIVATE+OB_OF_TOPSELECT, true);
  return o;
};
