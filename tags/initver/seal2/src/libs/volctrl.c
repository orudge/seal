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

/******************************************************/
/* SEAL 2.0 Volume Control control panel              */
/* Copyright (c) Owen Rudge 2001                      */
/*                                                    */
/* Based on Bad Seal Volume Controller by Bad Sector. */
/* Copyright (c) Bad Sector 2000-2001. All Rights     */
/* Reserved.                                          */
/******************************************************/

#include <seal.h>
#include <cp.h>
#include <button.h>
#include <dialogs.h>
#include <trackbar.h>

#define MSG_WAVE 100001
#define MSG_MIDI 100002

static DATAFILE *dat = NULL;

l_long  s_size   = 0;
l_long  s_depth  = 0;

BITMAP *icon16;
BITMAP *icon32;

/*p_bad_vtrackbar wave = NULL;
p_bad_vtrackbar midi = NULL;*/
p_trackbar wave = NULL;
p_trackbar midi = NULL;

p_cp_loader mypanel = NULL;

SetInfoAppName("Volume Control Panel");
SetInfoDesciption("Volume Control Panel");
SetInfoCopyright("Copyright (c) Kostas Michalopoulos 2000-2001, Owen Rudge 2001");
SetInfoManufacturer("Kostas Michalopoulos and Owen Rudge");

void set_save(void)
{
   // Nothing to do here
}

void set_trans_ev(p_object o, t_event *e)
{
   RETVIEW(o, e);

   if ((o->phase == PH_POSTPROCESS) && (e->type == EV_MESSAGE))
   {
      switch (e->message)
      {
         case MSG_WAVE:
            set_seal_volume(255-wave->value);
            clear_event(e);

            break;
         case MSG_MIDI:
            set_seal_volume_midi(255-midi->value);
            clear_event(e);

            break;
      }
   }
}

void set_load (p_object o)
{
   t_rect r = VIEW(o)->get_local_extent(VIEW(o));
   p_stattext st1 = NULL;
   p_stattext st2 = NULL;


   st1 = stattext_init(_malloc(sizeof(t_stattext)), rect_assign(0, 0, r.b.x/2, 40), TX_ALIGN_CENTER, "Digital sound volume");
   OBJECT(o)->insert(OBJECT(o), OBJECT(st1));


   /*wave = bad_vtrackbar_init(_malloc(sizeof(t_bad_vtrackbar)), r, MSG_WAVE);
   wave->max = 255;
   wave->pos = 63;
   OBJECT(o)->insert(OBJECT(o), OBJECT(wave));*/
   wave = trackbar_init(malloc(sizeof(t_trackbar)),rect_assign(0, 50, r.b.x/2, 150),1,0,255,255-get_seal_volume(),MSG_WAVE);
   OBJECT(o)->insert(OBJECT(o), OBJECT(wave));


   st2 = stattext_init(_malloc(sizeof(t_stattext)), rect_assign(r.b.x/2+5, 0, r.b.x, 40), TX_ALIGN_CENTER, "MIDI synthesis volume");
   OBJECT(o)->insert(OBJECT(o), OBJECT(st2));


   /*midi = bad_vtrackbar_init(_malloc(sizeof(t_bad_vtrackbar)), r, MSG_MIDI);
   midi->max = 255;
   midi->pos = 63;
   OBJECT(o)->insert(OBJECT(o), OBJECT(midi));*/

   midi = trackbar_init(malloc(sizeof(t_trackbar)),rect_assign(r.b.x/2+5, 50, r.b.x, 150),1,0,255,255-get_seal_volume_midi(),MSG_MIDI);
   OBJECT(o)->insert(OBJECT(o), OBJECT(midi));
}

void  myicon_draw ( p_view o ) {

 t_rect  r = o->get_local_extent(o);
 t_point p;

 BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

   rectfill(out,p.x,p.y,p.x+r.b.x,p.y+r.b.y,color_deskbar_face);

   if ( icon16 ) masked_blit(icon16,out,0,0,p.x,p.y,icon16->w,icon16->h);

  };

  o->end_of_paint(o, r);
};

void   myicon_translate_event ( p_object o, t_event *event ) {
  RETVIEW(o, event);
  view_translate_event(o, event);
  if ( event->type & EV_MOUSE  && OBJECT(mouse)->state & MO_SF_MOUSELDOUBLE  ) {
    show_cp_panel(mypanel);
    clear_event(event);
  };
};
void add_to_userbox(p_object o);

app_begin (void) {
  if ( ap_process == AP_INIT ) {
	 p_view ico;
     icon16 = load_image("bmp/volctrl.ico,16");
     icon32 = load_image("bmp/volctrl.ico,32");

     mypanel = add_cp_loader("Volume", NULL,cps_hardware,icon32,icon16, &set_load, &set_trans_ev, &set_save);

     ico = view_init(_malloc(sizeof(t_view)),rect_assign(0,0,16,16));
     ico->draw = &myicon_draw;
     OBJECT(ico)->translate_event = &myicon_translate_event;
     add_to_userbox(OBJECT(ico));
  };
} app_end;
