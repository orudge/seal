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
#include <cp.h>
#include <button.h>
#include <dialogs.h>
#include <trackbar.h>

BITMAP *icon16;
BITMAP *icon32;

p_trackbar speed = NULL;
p_trackbar dblclick = NULL;


SetInfoAppName("Mouse settings");
SetInfoDesciption("Mouse Control Panel");
SetInfoCopyright("Copyright (c) 2001 Point Mad. All rights reserved.");
SetInfoManufacturer("Point Mad");

void set_save( void ) {
  set_key_integer("system/mouse/speed",speed->value);
  set_key_integer("system/mouse/dblclick",dblclick->value*100);
  mouse->set_speed(mouse,point_assign(speed->value,speed->value));
  mouse->set_dclick_diff(mouse, dblclick->value*100);
};

void set_trans_ev(p_object o, t_event *e) {

};

l_big dclick_old_time = 0;
l_dword state_last = 0;
l_bool face = 0;

void test_trans_ev ( p_object o, t_event *event ) {

  RETVIEW(o,event);

  if ( event->type & EV_MOUSE ) {
	l_big diftime = time_diff_mili(dclick_old_time);

	if ( diftime > dblclick->value*100 ) dclick_old_time = 0;

    if ( OBJECT(mouse)->state >= MO_SF_MOUSEMOVE ) {

      if ( OBJECT(mouse)->state & MO_SF_MOUSEDOWN ) {

	    if ( diftime <= dblclick->value*100 &&
             state_last & MO_SF_MOUSEUP ) {
          face = face ? 0 : 1;
          VIEW(o)->brush.background = face ? icon16 : icon32;
          VIEW(o)->draw_me(VIEW(o));
          dclick_old_time = 0;
        } else
          dclick_old_time = time_get_mili();
      };
    };
    state_last = OBJECT(mouse)->state;
  };
};

void set_load ( p_object o ) {
   t_rect r = VIEW(o)->get_local_extent(VIEW(o));
   p_stattext st = NULL;
   p_view test;
   l_long vspeed = get_key_integer("system/mouse/speed");
   l_long vdblclick = get_key_integer("system/mouse/dblclick")/100;


   st = stattext_init(_malloc(sizeof(t_stattext)),
                      rect_assign(105, 0, r.b.x/2-16, 15), TX_ALIGN_CENTERY|TX_ALIGN_LEFT, "Fast");
   OBJECT(o)->insert(OBJECT(o), OBJECT(st));
   st = stattext_init(_malloc(sizeof(t_stattext)),
                      rect_assign(r.b.x/2-16, 0, r.b.x-35, 15), TX_ALIGN_CENTERY|TX_ALIGN_RIGHT, "Slow");
   OBJECT(o)->insert(OBJECT(o), OBJECT(st));

   st = stattext_init(_malloc(sizeof(t_stattext)),
                      rect_assign(0, 20, 100, 51), TX_ALIGN_CENTER, "Speed");
   OBJECT(o)->insert(OBJECT(o), OBJECT(st));

   speed = trackbar_init(malloc(sizeof(t_trackbar)),
                         rect_assign(105, 10, r.b.x-35, 51),0,0,10,vspeed, NULL);
   OBJECT(o)->insert(OBJECT(o), OBJECT(speed));

   st = stattext_init(_malloc(sizeof(t_stattext)),
                       rect_assign(0, 60, 100, 91), TX_ALIGN_CENTER, "Double click");
   OBJECT(o)->insert(OBJECT(o), OBJECT(st));

   dblclick = trackbar_init(malloc(sizeof(t_trackbar)),
                            rect_assign(105, 60, r.b.x-35, 91),0,1,20,vdblclick,NULL);
   OBJECT(o)->insert(OBJECT(o), OBJECT(dblclick));

   test = view_init(malloc(sizeof(t_view)),rect_assign(r.b.x-31,60,r.b.x,91));
   test->brush.background = icon32;
   OBJECT(test)->translate_event = &test_trans_ev;
   OBJECT(o)->insert(OBJECT(o), OBJECT(test));

};


app_begin (void) {
  if ( ap_process == AP_INIT ) {

     icon16 = load_image("bmp/mouse.ico,16");
     icon32 = load_image("bmp/mouse.ico,32");

     add_cp_loader("Mouse settings", NULL,cps_hardware,icon32,icon16, &set_load, &set_trans_ev, &set_save);

  };
} app_end;
