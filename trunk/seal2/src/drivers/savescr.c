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

////////////////////////////////////////////////////////////////////////////////

#include <seal.h>
#include <savescr.h>

////////////////////////////////////////////////////////////////////////////////

l_big  screensaver_lastime = 0;
l_text screensaver_app = NULL;
l_bool screensaver_lauched = false;
l_uint screensaver_dif = 120; // 120 sec -> 2 min

void screensaver_timer ( p_object o ) {
  if ( !screensaver_lauched )
    if ( screensaver_dif <= time(NULL) - screensaver_lastime )
      if ( screensaver_app ) {
	    screensaver_lauched = true;
        run_file ( screensaver_app );
      };
};

void screensaver_translate_event ( p_object o , t_event *event ) {
  if ( event->type & EV_KEYBOARD || event->type & EV_MOUSE ) screensaver_lastime = time(NULL);
};

p_object screensaver_init ( p_object o ) {

  if ( !o ) return NULL;

  clear_type(o,sizeof(t_object));

  obj_init(o);

  o->func_callback = &screensaver_timer;
  o->process_tick = 1000;
  o->set_options(o, OB_OF_STILLPROCESS, true);
  o->set_options(o, OB_OF_NOTACTIVATE+OB_OF_TOPSELECT, true);

  screensaver_lastime = time(NULL);

  if ( key_exists("current/seal/screensaver/app") ) screensaver_app = get_key("current/seal/screensaver/app");
  if ( key_exists("current/seal/screensaver/dif") ) screensaver_dif = get_key_integer("current/seal/screensaver/dif");

  if ( screensaver_app ) if ( !io_exist(screensaver_app) ) screensaver_app = NULL;

  DEBUG_printf("ScreenSaver settings : %s %d\n",screensaver_app,screensaver_dif);

  return o;
};