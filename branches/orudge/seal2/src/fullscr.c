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

////////////////////////////////////////////////////////////////////////////////
void app_timer ( p_object o ) {


};
////////////////////////////////////////////////////////////////////////////////
void exit_app ( p_object o ) {
  screen_reload ( ); // Reload Seal Screen

  desktop->show(desktop); // Show desktop

  mouse->show(mouse);  // Show mouse

  dispose(o); // Unload Game Object

};
////////////////////////////////////////////////////////////////////////////////
void app_translate_event ( p_object o, t_event *event ) {


  obj_translate_event(o, event); // Call object event handler

  if ( event->type == EV_KEYBOARD ) { // If a keyboard event happen

    exit_app ( o ); // Unload Game

    DLXUnload(ap_id); // Unload XDL

    clear_event(event); // Clear event

  };


};
////////////////////////////////////////////////////////////////////////////////
void init_app ( p_object o ) {

  o = obj_init(o); // Init App Object (see object.h)

  mouse->hide(mouse); // Hide mouse

  desktop->hide(desktop);  // Hide desktop

  screen_halt(); // Stop Seal Screen

  if ( !GrSetMode(0, 640, 480, 640, 480, 16) ) { // Try to change screen defenition
    exit_app (o);  // Quit app
    seal_error(ERR_INFO, "Your graphic card or screen don't\nsupport 640x480 @ 16 bpp.\nYou can't run this program.\n\nSorry."); // Message Error
    DLXUnload(ap_id); // Unload XDL
    return;
  };

  o->func_callback   = &app_timer;  // Set timer

  o->translate_event = &app_translate_event; // Set event handler

  o->set_options(o, OB_OF_NOTACTIVATE+OB_OF_TOPSELECT+OB_OF_STILLPROCESS, true); // Set options

  o->set_state(o, OB_SF_FOCUSED, true);// Set focus on app

  clear_to_color(screen, makecol(0,0,0));

  // "Insert" app in Seal

  program.insert(&program,o);

  // Initialise app timer

  init_stillprocess(o,20);


};
////////////////////////////////////////////////////////////////////////////////
SetInfoAppName("Full screen app Demo");
SetInfoDesciption("An exemple to make full screen apps.");
SetInfoCopyright("(c) Copyright 2001 Point Mad. All rights reserved");
SetInfoManufacturer("Point Mad");
////////////////////////////////////////////////////////////////////////////////
app_begin ( void ) {


  if ( ap_process == AP_INIT ) {

    init_app( _malloc(sizeof(t_object)) ); // Init game and alloc memory for game object

  };

  if ( ap_process == AP_DONE ) {

  };

} app_end;

