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
#include <sound.h>
#include <screen.h>

FONT *arial20 = NULL;

FONT *arial10 = NULL;

//extern volatile int mouse_x;
//extern volatile int mouse_y;

// bitmaps
BITMAP *stars = NULL; // zoli etoile
BITMAP *logop = NULL; // logo pmad

BITMAP *my_mouse_sprite = NULL;

l_char map[12][9] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 12 x 9
                   { 0, 1, 1, 1, 0, 0, 0, 0, 0 },
                   { 0, 1, 0, 1, 0, 1, 1, 1, 0 },
                   { 0, 0, 0, 1, 0, 1, 1, 1, 0 },
                   { 0, 0, 1, 1, 0, 0, 0, 1, 0 },
                   { 0, 1, 1, 1, 0, 0, 0, 1, 0 },
                   { 0, 1, 1, 1, 1, 1, 1, 1, 0 },
                   { 0, 0, 1, 1, 0, 1, 1, 1, 0 },
                   { 0, 0, 1, 1, 0, 1, 1, 1, 0 },
                   { 0, 0, 0, 1, 0, 0, 1, 1, 0 },
                   { 0, 0, 0, 1, 0, 0, 1, 1, 0 },
                   { 0, 0, 0, 0, 0, 0, 0, 0, 0 }};

#define plan_x 9
#define plan_y 12

#define case_x 30
#define case_y 30

BITMAP *fond = NULL;
BITMAP *mapb = NULL;

void afficher_case ( BITMAP *out, l_int x, l_int y ) {
  set_clip(out,x*case_x,y*case_y,x*case_x+case_x,y*case_y+case_y);
  blit ( (map[y][x]) ? fond : mapb, out,x*case_x,y*case_y,x*case_x,y*case_y,case_x,case_y);
};

void afficher_plan ( BITMAP *out, l_int nx, l_int ny, l_int bx, l_int by ) {
  l_int x=nx,y=ny;
  while ( x < bx ) {
    y=ny;
    while ( y < by ) {
      afficher_case(out,x,y);
      y++;
    };
    x++;
  };
};

// fin bitmap

p_play snd = NULL;

////////////////////////////////////////////////////////////////////////////////
void game_timer ( p_object o ) {

  if ( snd ) if ( !snd->poll_file(snd) ) snd->play_file(snd) ;

};
////////////////////////////////////////////////////////////////////////////////
void exit_game ( p_object o ) {

  if ( mapb )  destroy_bitmap(mapb);
  if ( snd )   snd->stop_file(snd);

  screen_reload ( ); // Reload Seal Screen

  desktop->show(desktop); // Show desktop

  mouse->show(mouse);  // Show mouse

  dispose(o); // Unload Game Object

};
/* Evenements souris ..
#  MO_SF_MOUSEMOVE
#  MO_SF_MOUSELDOWN
#  MO_SF_MOUSELUP
#  MO_SF_MOUSERDOWN
#  MO_SF_MOUSERUP
#  MO_SF_MOUSEMDOWN
#  MO_SF_MOUSEMUP
#  MO_SF_MOUSESHOW
#  MO_SF_MOUSEHIDE
#  MO_SF_MOUSELAUTO
#  MO_SF_MOUSEMAUTO
#  MO_SF_MOUSERAUTO
#  MO_SF_MOUSELDOUBLE
#  MO_SF_MOUSERDOUBLE
#  MO_SF_MOUSEMDOUBLE
#  MO_SF_MOUSEDOWN
#  MO_SF_MOUSEUP
#  MO_SF_MOUSEAUTO
#  MO_SF_MOUSEDOUBLE

*/
////////////////////////////////////////////////////////////////////////////////
void game_translate_event ( p_object o, t_event *event ) {


  obj_translate_event(o, event); // Call object event handler

  if ( event->type == EV_KEYBOARD ) { // If a keyboard event happen

    exit_game ( o ); // Unload Game

    DLXUnload(ap_id); // Unload XDL

    clear_event(event); // Clear event

  } else if ( event->type == EV_MOUSE ) { // If a mouse event happen
    if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) { // Boutton gauche cliqué ...

    };

  };


};
////////////////////////////////////////////////////////////////////////////////
void init_game ( p_object o ) {
  l_uint a = 255;
  l_uint b = 0;

  o = obj_init(o); // Init Game Object (see object.h)

  mouse->hide(mouse); // Hide mouse

  while ( b < screen_width ) {
    vline(screen,b,0,screen_height,0);
    b++;
  };

  desktop->hide(desktop);  // Hide desktop

  screen_halt(); // Stop Seal Screen

  if ( !GrSetMode(0, 640, 480, 640, 480, 16) ) { // Try to change screen defenition
    exit_game (o);  // Quit game
    seal_error(ERR_INFO, "Your graphic card or screen don't\nsupport 640x480 @ 16 bpp.\nYou can't run this program.\n\nSorry."); // Message Error
    DLXUnload(ap_id); // Unload XDL
    return;
  };

  //clear_to_color(screen, makecol(255,255,255));

  // Search fonts

  snd = SFA_init();
  if ( !snd->init_file(snd, "my.mp3") ) { _free(snd); snd = NULL; };


  o->func_callback   = &game_timer;  // Set timer

  o->translate_event = &game_translate_event; // Set event handler

  o->set_options(o, OB_OF_NOTACTIVATE+OB_OF_TOPSELECT+OB_OF_STILLPROCESS, true); // Set options

  o->set_state(o, OB_SF_FOCUSED, true);// Set focus on Game

	// foutre l'ecran en noir !
  clear_to_color(screen, makecol(0,0,0));

	//charge le logo

  logop = load_image("s2a-bin/worms/logo.bmp");

	if ( logop ) blit(logop,screen,0,0,124,160,391,160);

  mapb = load_image("plan.bmp");

  fond = desktop->brush.background;

	// hop on laisse le logo 3 secondes
	delay(3000);

	// nike le logo
	if ( logop ) destroy_bitmap(logop);

  // Set screen RGB(0,0,0)

  clear_to_color(screen, makecol(0,0,0));

  
  arial10 = get_font_in_size("ARIAL",10,10);

  //change le sprite du curseur...
  my_mouse_sprite = load_image("s2a-bin/worms/cursor.bmp");

  show_mouse(screen);

  set_mouse_sprite (my_mouse_sprite);




  if ( !snd->play_file(snd) ) { _free(snd); snd = NULL; };

  // "Insert" game in Seal

  program.insert(&program,o);

  // Initialise Game timer

  init_stillprocess(o,20);

  afficher_plan(screen,0,0,plan_x,plan_y);
};
////////////////////////////////////////////////////////////////////////////////
SetInfoAppName("Game Demo");
SetInfoDesciption("A demo program to make full screen games.");
SetInfoCopyright("(c) Copyright 2001 Point Mad. All rights reserved");
SetInfoManufacturer("Point Mad");
////////////////////////////////////////////////////////////////////////////////
app_begin ( void ) {


  if ( ap_process == AP_INIT ) {

    init_game( _malloc(sizeof(t_object)) ); // Init game and alloc memory for game object

  };

  if ( ap_process == AP_DONE ) {

  };

} app_end;

