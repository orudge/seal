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
#include <savescr.h>
#include <screen.h>
#include <app.h>


#define FIRE_HOTSPOTS   70

int hotspot[FIRE_HOTSPOTS];

unsigned char *temp;

void draw_bottom_line_of_fire()
{
   int c, c2;


   for (c=0; c<SCREEN_W; c++)
      temp[c] = 0;

   for (c=0; c<FIRE_HOTSPOTS; c++) {

      for (c2=hotspot[c]-20; c2<hotspot[c]+20; c2++)
	 if ((c2 >= 0) && (c2 < SCREEN_W))
	    temp[c2] = MIN(temp[c2] + 20-ABS(hotspot[c]-c2), 192 );


      hotspot[c] += (rand() & 7) - 3;
      if (hotspot[c] < 0)
	 hotspot[c] += SCREEN_W;
      else
	 if (hotspot[c] >= SCREEN_W)
	    hotspot[c] -= SCREEN_W;
   }


   for (c=0; c<SCREEN_W; c++)
      putpixel(screen, c, SCREEN_H-1, temp[c]);
}


unsigned long address;
int x, y;

////////////////////////////////////////////////////////////////////////////////
void app_timer ( p_object o ) {

      set_clip(screen,0,0,SCREEN_W,SCREEN_H);

      acquire_screen();

      draw_bottom_line_of_fire();

      bmp_select(screen);

      for (y=0; y<SCREEN_H-1; y++) {

	 address = bmp_read_line(screen, y+1);

	 for (x=0; x<SCREEN_W; x += sizeof(unsigned long))
	    *((unsigned long *)&temp[x]) = bmp_read32(address+x);


	 for (x=0; x<SCREEN_W; x++)
	    if (temp[x] > 0)
	       temp[x]--;


	 address = bmp_write_line(screen, y);


	 for (x=0; x<SCREEN_W; x += sizeof(unsigned long))
	    bmp_write32(address+x, *((unsigned long *)&temp[x]));
      }

      bmp_unwrite_line(screen);

      release_screen();

};
////////////////////////////////////////////////////////////////////////////////
void exit_app ( p_object o ) {

  set_palette(Gr_pal);

  screen_reload ( ); // Reload Seal Screen

  desktop->show(desktop); // Show desktop

  mouse->show(mouse);  // Show mouse

  dispose(o); // Unload Game Object

  screensaver_lauched = false;

  free(temp);

};
////////////////////////////////////////////////////////////////////////////////
void app_translate_event ( p_object o, t_event *event ) {


  obj_translate_event(o, event); // Call object event handler

  if ( event->type == EV_KEYBOARD || event->type == EV_MOUSE ) { // If a keyboard event happen

    exit_app ( o ); // Unload Game

    DLXUnload(ap_id); // Unload XDL

    clear_event(event); // Clear event

  };


};
////////////////////////////////////////////////////////////////////////////////
void init_app ( p_object o ) {

  PALETTE palette;

  int c;

  void *omc = mouse_callback;

  mouse_callback = NULL;

  o = obj_init(o); // Init App Object (see object.h)

  mouse->hide(mouse); // Hide mouse

  desktop->hide(desktop);  // Hide desktop

  screen_halt(); // Stop Seal Screen

  o->func_callback   = &app_timer;  // Set timer

  o->translate_event = &app_translate_event; // Set event handler

  o->set_options(o, OB_OF_NOTACTIVATE+OB_OF_TOPSELECT+OB_OF_STILLPROCESS, true); // Set options

  o->set_state(o, OB_SF_FOCUSED, true);// Set focus on app

  if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0) {
      if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) != 0) {

      }
  }

  set_clip(screen,0,0,SCREEN_W,SCREEN_H);

  temp = (unsigned char *)malloc(sizeof(unsigned char) * SCREEN_W);

  for (c=0; c<FIRE_HOTSPOTS; c++)
      hotspot[c] = rand() % SCREEN_W;

   for (c=0; c<64; c++) {
      palette[c].r = c;
      palette[c].g = 0;
      palette[c].b = 0;
   }
   for (c=64; c<128; c++) {
      palette[c].r = 63;
      palette[c].g = c-64;
      palette[c].b = 0;
   }
   for (c=128; c<192; c++) {
      palette[c].r = 63;
      palette[c].g = 63;
      palette[c].b = c-192;
   }
   for (c=192; c<256; c++) {
      palette[c].r = 63;
      palette[c].g = 63;
      palette[c].b = 63;
   }

   set_palette(palette);

  screensaver_lauched = true;



 textout_draw_rect( screen, get_font_in_size("ARIAL",20,20), "Screen saver `Fire'", -1,
                                        0 ,
                                        0 ,
                                        SCREEN_W,
                                        SCREEN_H,
                                        TX_ALIGN_CENTERX|TX_ALIGN_BOTTOM, makecol(255,255,255), TX_NOCOLOR, 1);

  // "Insert" app in Seal

  program.insert(&program,o);

  // Initialise app timer

  init_stillprocess(o,20);

  mouse_callback = omc;

};
////////////////////////////////////////////////////////////////////////////////
SetInfoAppName("Screen saver demo");
SetInfoDesciption("An exemple to make screen savers");
SetInfoCopyright("(c) Copyright 2001 Point Mad. All rights reserved");
SetInfoManufacturer("Point Mad");
////////////////////////////////////////////////////////////////////////////////
app_begin ( void ) {


  if ( ap_process == AP_INIT ) {

	if ( ap_args ) if ( !stricmp(ap_args,"SETTINGS") ) {

      msgbox( MW_INFO, MB_OK, "About `FIRE'\n\n   Screen saver by Point Mad based on an allegro exemple by Shawn Hargreaves.\n\n(c) Copyright 2001 Point Mad. All rights reserved.");

      DLXUnload(ap_id);

      return;

    };

      init_app( _malloc(sizeof(t_object)) );

  };

  if ( ap_process == AP_DONE ) {

  };

} app_end;

