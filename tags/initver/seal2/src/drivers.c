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

/* Revision History (as of 05/04/2002):
 *
 * 05/04/2002: Removed #include <blahblah.c> files and moved to makefile (orudge)
 */

#include <seal.h>
#include "screen.h"
#include "mouse.h"
#include "keyboard.h"
#include "sound.h"
#include "savescr.h"

//#include "screen.c" // Screen functions ...
//#include "drivers\savescr.c"
//#include "drivers\mouse.c" // Mouse functions ...
//#include "drivers\keyboard.c" // Keyboard functions ...
//#include "drivers\sound.c" // SFA from Florian Xaver


extern char* _LIBEXPORTTABLE[];

void init_ico ( void );
l_int maxstep = 13;
l_int step = 0;
l_int dstep = 0;
BITMAP *step_ico = NULL;
BITMAP *gstep_ico = NULL;
l_bool lowgrfx = false;
t_rect lowr;

void step_init ( ) {
  if ( lowgrfx ) {
    l_int w = ( screen_width - 10 * maxstep ) / 2;
    lowr = rect_assign(w,screen_height-15,w+10*maxstep,screen_height-5);
    rect(screen,lowr.a.x-2,lowr.a.y-2,lowr.b.x+2,lowr.b.y+2,0);
  } else {
    l_int gstep = 0;
    step_ico = load_image("bmp/step.bmp");
    gstep_ico = load_image("bmp/gstep.bmp");
    if ( gstep_ico ) {
      dstep = ( screen_width - ( IMAGE_WIDTH(gstep_ico) + 10 ) * maxstep ) / 2;

      while ( gstep < maxstep ) {
        if ( step_ico ) draw_sprite(screen, gstep_ico, dstep + ( IMAGE_WIDTH(gstep_ico) + 10 ) * gstep, screen_height-IMAGE_HEIGHT(gstep_ico) - 5 );
        gstep++;
      };
    };
  };
};


void step_icon ( ) {
  if ( lowgrfx ) {
    rectfill(screen,lowr.a.x+step*10,lowr.a.y,lowr.a.x+step*10+10,lowr.b.y, makecol(0,0,255) );
  } else
    if ( step_ico ) draw_sprite(screen, step_ico, dstep + ( IMAGE_WIDTH(step_ico) + 10 ) * step, screen_height-IMAGE_HEIGHT(step_ico) - 5 );
  step++;
};
/*l_ulong lasttime = 0;

void info_init ( l_text info ) {

  step_icon();

  rectfill(screen,0 ,screen_height - 15,screen_width,screen_height,makecol(255,255,255));

  textout_draw_rect(screen, get_font_in_size("ARIAL",10,10), info, -1,
                                        0 ,
                                        screen_height - 15 ,
                                        screen_width,
                                        screen_height,
                                        TX_ALIGN_CENTER, makecol(0,0,0), makecol(255,255,255), 1);

  if ( lasttime ) DEBUG_printf("\n  ... done in %d seconds\n",time(NULL)-lasttime);
  DEBUG_printf("\n  - %s\n",info);
  lasttime = time(NULL);
};

void info_title ( l_text info, l_int id  ) {
  rectfill(screen,0 ,(id*30) ,screen_width,(id*30)+30,makecol(255,255,255));

  textout_draw_rect(screen, get_font_in_size("ARIAL",20,20), info, -1,
                                        0 ,
                                        (id*30),
                                        screen_width,
                                        (id*30)+30,
                                        TX_ALIGN_CENTER, makecol(0,0,0), makecol(255,255,255), 1);

};*/

l_bool  drivers_done ( p_object o )
{
  DEBUG_printf("Done drivers...\n");
  if ( !obj_done((p_object)o) ) return false;
  DEBUG_printf("Done fonts...\n");
  fonts_done();
  DEBUG_printf("Done screen...\n");
  screen_done();
  DEBUG_printf("Done allegro...\n");
  allegro_exit();
  DEBUG_printf("Done drivers : FINISHED\n");
  return true;

};

void drv_done ( void ) {

  drivers_done ( OBJECT(drivers) );

};

void ini_ext_runner ( void );

t_drivers *drivers_init ( t_drivers *o ) {
  BITMAP *ld = NULL;
  if ( !o ) return NULL;

  memset(o, 0, sizeof(t_drivers));

  obj_init(&o->obclass);

  init_ico ( ); // hi hi !!

  screen_init ( );

  lowgrfx = get_depth(screen) < 16;

  clear_to_color(screen, makecol(255,255,255));

  if ( !lowgrfx ) ld = load_image("bmp/load.bmp");

  if ( lowgrfx ) font_smooth = false;

  step_init();
  if ( ld ) draw_sprite(screen, ld, (screen_width-IMAGE_WIDTH(ld))/2, (screen_height-IMAGE_HEIGHT(ld))/2 );

  step_icon ( );

  load_supported_fonts("/system/fonts/");

  step_icon();

  load_txts();

  step_icon();
  DEBUG_printf("   Drivers :: Load the libexportable.\n");
  DLXImport(_LIBEXPORTTABLE);
  step_icon();
  colors_init ( );
  step_icon();
  load_bmps ( );
  step_icon();
  install_int(&aclock, 20);
  step_icon();
  ini_sound ( );
  step_icon();
  OBJECT(o)->insert(OBJECT(o), OBJECT (key_init(_malloc(sizeof(t_keyboard))) ));
  step_icon();
  OBJECT(o)->insert(OBJECT(o), OBJECT( mouse_init(_malloc(sizeof(t_mouse))) ));
  step_icon();
  ini_ext_runner();
  step_icon();
  init_skins();
  step_icon();
  RunAllRegistryEntries("system/startup/drivers",NULL);
  step_icon();

  return o;

};

void drv_init ( void ) {

  drivers = drivers_init ( _malloc(sizeof(t_drivers)) );

};
