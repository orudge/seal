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

/* Revision History (as of 04/04/2002):
 *
 * 04/04/2002: Fixed transparency issues with skins (orudge)
 */

#include <seal.h>
#include <app.h>
#include <button.h>
#include <stattext.h>
#include <cp.h>

p_list    cp_loaders = NULL;
p_list    cp_sections = NULL;
////////////////////////////////////////////////////////////////////////////////
l_bool      loaded     = false;
p_appwin    cp_win     = NULL;
p_stattext  cp_title   = NULL;
p_icon      cp_icon    = NULL;
p_view      cp_frame   = NULL;
p_cp_loader cp_current = NULL;
p_cp_section cps_current = NULL;

p_button    cp_bt_save = NULL;
p_button    cp_bt_canl = NULL;

p_cp_section cps_display = NULL;
p_cp_section cps_hardware = NULL;

////////////////////////////////////////////////////////////////////////////////
#define MSG_CP_SAVE    150004
#define MSG_CP_CANCEL  150005
#define MSG_CP_GO      150006
#define MSG_CP_END     150056
#define MSG_CPS_GO     150057
#define MSG_CPS_END    150107
////////////////////////////////////////////////////////////////////////////////
void add_cp_item ( t_rect r, p_object own, l_int nb, p_cp_loader cl, l_dword message ) {
  p_stattext capt;
  p_button bt;
  p_icon ico;
  capt = stattext_init( _malloc(sizeof(t_stattext)), rect_assign(40,nb*30,r.b.x-40,nb*30+25), TX_ALIGN_CENTERY, cl->caption );
  bt = button_init(_malloc(sizeof(t_button)), rect_assign(r.b.x-35,nb*30,r.b.x-5,nb*30+25),">", message,0);
  if ( cl->icon16 ) {
	ico = icon_init(_malloc(sizeof(t_icon)),rect_assign(10,nb*30,35,nb*30+25),cl->icon16,NULL,NULL);
    VIEW(ico)->drag_mode = 0;
	own->insert(own,OBJECT(ico));
  };
  own->insert(own,OBJECT(bt));
  own->insert(own,OBJECT(capt));
};
////////////////////////////////////////////////////////////////////////////////
void load_cp_panel ( p_cp_section o ) {

  t_rect r = VIEW(cp_win)->get_local_extent(VIEW(cp_win));

  if ( cp_frame ) dispose(OBJECT(cp_frame));

  cp_current = NULL;
  cps_current = o;

  r = rect_assign(105,60,r.b.x-10,r.b.y-35);

  cp_frame = view_init(_malloc(sizeof(t_view)),r);

  if (use_skins) // OCR
     cp_frame->transparent = true;
  else
     cp_frame->brush.color = color_3d_face;

  OBJECT(cp_win)->insert(OBJECT(cp_win),OBJECT(cp_frame));

  cp_icon->icon = o->icon32;
  VIEW(cp_win)->draw_me(VIEW(cp_win)); // OCR
  VIEW(cp_icon)->draw_me(VIEW(cp_icon));

  cp_title->set_text(cp_title,o->fullcaption);

  VIEW(cp_bt_save)->hide(VIEW(cp_bt_save));
  VIEW(cp_bt_canl)->hide(VIEW(cp_bt_canl));

  if ( cp_loaders && cp_loaders->last ) {
    l_int added = 0;
    l_int nb    = 0;
    p_item x = cp_loaders->first(cp_loaders);
    p_item f = x;
    do {
      p_cp_loader cl = CP_LOADER(x->rec);
      if ( cl->sec == o ) {
        add_cp_item(rect_move(r,-r.a.x,-r.a.y),OBJECT(cp_frame),added,cl,MSG_CP_GO+nb);
        added++;
      };
      nb++;
      x = x->next;
    } while ( x != f );
  };
};
////////////////////////////////////////////////////////////////////////////////
void cp_go_translate_event ( p_object o, t_event *event ) {
  RETVIEW(o, event);
  if ( cp_current->trans_ev ) {
    o->phase = PH_PREPROCESS;
    cp_current->trans_ev(o,event);
    o->phase = 0;
  };
  view_translate_event(o, event);
  if ( cp_current->trans_ev ) {
    o->phase = PH_POSTPROCESS;
    cp_current->trans_ev(o,event);
    o->phase = 0;
  };
};

////////////////////////////////////////////////////////////////////////////////
void cp_go_loader ( p_cp_loader cl ) {
  t_rect r = VIEW(cp_win)->get_local_extent(VIEW(cp_win));

  if ( cp_frame ) dispose(OBJECT(cp_frame));

  r = rect_assign(105,60,r.b.x-10,r.b.y-35);

  cp_frame = view_init(_malloc(sizeof(t_view)),r);

  cp_frame->brush.color = color_3d_face;

  OBJECT(cp_frame)->translate_event = &cp_go_translate_event;

  OBJECT(cp_frame)->set_options(OBJECT(cp_frame), OB_OF_TOPSELECT, true);

  OBJECT(cp_win)->insert(OBJECT(cp_win),OBJECT(cp_frame));

  cp_current = cl;
  cps_current = NULL;

  cl->load(OBJECT(cp_frame));

  if ( cl->icon32 ) {
    cp_icon->icon = cl->icon32;
    VIEW(cp_win)->draw_me(VIEW(cp_win)); // OCR
    VIEW(cp_icon)->draw_me(VIEW(cp_icon));
  };

  cp_title->set_text(cp_title,cl->caption);

  VIEW(cp_bt_save)->show(VIEW(cp_bt_save));
  VIEW(cp_bt_canl)->show(VIEW(cp_bt_canl));

};
////////////////////////////////////////////////////////////////////////////////
void trans_ev( p_object o, t_event *event ) {
  if ( o->process == PH_PREPROCESS && event->type & EV_MESSAGE ) {
    if ( event->message == MSG_CLOSE )
      loaded = false;
    else if ( event->message == MSG_OK || event->message == MSG_CP_SAVE ) {
      if ( cp_current ) {
        t_event saved;
        memcpy(&saved,event,sizeof(t_event));
        cp_current->save();
        memcpy(event,&saved,sizeof(t_event));
      };
      if ( event->message == MSG_CP_SAVE ) {
        load_cp_panel(cp_current->sec);
        clear_event(event);
      } else loaded = false;
    } else if ( event->message == MSG_CP_CANCEL ) {
      load_cp_panel(cp_current->sec);
      clear_event(event);
   /* } else if ( event->message == MSG_CP_DISPLAY ) {
      load_cp_panel(0);
      clear_event(event);
    } else if ( event->message == MSG_CP_HARWARE ) {
      load_cp_panel(1);
      clear_event(event);*/
    } else if ( event->message >= MSG_CP_GO && event->message <= MSG_CP_END ) {
      l_int nb = event->message - MSG_CP_GO;
      p_cp_loader cl = CP_LOADER(cp_loaders->at(cp_loaders,nb));
      cp_go_loader(cl);
      clear_event(event);
    } else if ( event->message >= MSG_CPS_GO && event->message <= MSG_CPS_END ) {
      l_int nb = event->message - MSG_CPS_GO;
      p_cp_section cl = CP_SECTION(cp_sections->at(cp_sections,nb));
      load_cp_panel(cl);
      clear_event(event);
    };

  };
};
////////////////////////////////////////////////////////////////////////////////
void load_cp_ex ( p_cp_loader def_cp, p_cp_section def_cps) {
  p_icobutton bt = NULL;
  p_button b = NULL;
  p_view   cp_line    = NULL;
  t_rect r = rect_assign(0,0,450,375);

  if ( loaded ) return;

  loaded = true;

  cp_win = appwin_init( _malloc(sizeof(t_appwin)),
                                 r,
                                 TXT_CONTROL_PANEL,
                                 WF_MINIMIZE,
                                 NULL,
                                 &trans_ev);

  VIEW(cp_win)->align |= TX_ALIGN_CENTER;

  OBJECT(desktop)->insert(OBJECT(desktop),OBJECT(cp_win));

  cp_title = stattext_init(_malloc(sizeof(t_stattext)),rect_assign(150,20,r.b.x-5,50),TX_ALIGN_CENTERY,"Welcome to control panel 1.0 !");

  VIEW(cp_title)->font = get_font_in_size("ARIAL",16,16);

  cp_title->color_text = CO_BLACK; //color_flat_face;

  OBJECT(cp_win)->insert(OBJECT(cp_win),OBJECT(cp_title));

  cp_icon = icon_init(_malloc(sizeof(t_icon)),rect_assign(100,20,140,50),IMG_DIR32,NULL,NULL);
  VIEW(cp_icon)->drag_mode = 0;
  OBJECT(cp_win)->insert(OBJECT(cp_win),OBJECT(cp_icon));

  cp_line = view_init(_malloc(sizeof(t_view)),rect_assign(100,55,r.b.x-5,55));
  cp_line->brush.color = color_3d_shadow;

  OBJECT(cp_win)->insert(OBJECT(cp_win),OBJECT(cp_line));

  cp_line = view_init(_malloc(sizeof(t_view)),rect_assign(100,55,100,r.b.y - 35));
  cp_line->brush.color = color_3d_shadow;

  OBJECT(cp_win)->insert(OBJECT(cp_win),OBJECT(cp_line));

  cp_frame = NULL;
  cp_current = NULL;
  if ( cp_sections ) {
	p_item p = cp_sections->first(cp_sections);
	p_item f = p;
	t_rect h = rect_assign(5,55,95,105);
	l_uchar id = 0;
	if ( p ) do {
	  p_cp_section cps = CP_SECTION(p->rec);

	  bt  = icobutton_init( _malloc(sizeof(t_icobutton)),
                        h,
                        cps->icon32,
                        cps->info,
                        MSG_CPS_GO+id,
                        BF_UNDERSEL );
      BUTTON(bt)->caption = _strdup(cps->caption);
      OBJECT(cp_win)->insert(OBJECT(cp_win),OBJECT(bt));

	  id++;

	  h = rect_move(h,0,55);
	  p = p->next;
    } while ( p != f );
  };
  b = button_init(_malloc(sizeof(t_button)), rect_assign ( 5, r.b.y - 30, 95, r.b.y - 5 ), TXT_OK, MSG_OK, 0 );
  OBJECT(cp_win)->insert(OBJECT(cp_win),OBJECT(b));

  cp_bt_save = button_init(_malloc(sizeof(t_button)), rect_assign ( 105, r.b.y - 30, 195, r.b.y - 5 ), TXT_SAVE, MSG_CP_SAVE, 0 );
  OBJECT(cp_win)->insert(OBJECT(cp_win),OBJECT(cp_bt_save));
  cp_bt_canl = button_init(_malloc(sizeof(t_button)), rect_assign ( 200, r.b.y - 30, 290, r.b.y - 5  ), TXT_CANCEL, MSG_CP_CANCEL, 0 );
  OBJECT(cp_win)->insert(OBJECT(cp_win),OBJECT(cp_bt_canl));

  if ( def_cps ) load_cp_panel(def_cps );
  if ( def_cp ) cp_go_loader(def_cp);
};

void load_cp ( void ) {
  load_cp_ex(NULL,cps_display);
};

void show_cp_panel ( p_cp_loader o ) {
  if ( loaded )
	cp_go_loader(o);
  else
	load_cp_ex(o,NULL);
};
void show_cp_section ( p_cp_section o ) {
  if ( loaded )
	load_cp_panel(o);
  else
	load_cp_ex(NULL,o);
};
////////////////////////////////////////////////////////////////////////////////
p_cp_loader add_cp_loader ( l_text  caption,
                     l_text helpfile,
                     p_cp_section   sec,
                     BITMAP *icon32,
                     BITMAP *icon16,
                     void  (*load) ( p_object o ),
                     void  (*trans_ev) ( p_object o, t_event *event ),
                     void  (*save) ( void ) ) {

  p_cp_loader o = _malloc(sizeof(t_cp_loader));
  if ( !o ) return;
  o->caption = caption;
  o->helpfile = helpfile;
  o->sec = sec;
  o->icon16 = icon16;
  o->icon32 = icon32;
  o->load = load;
  o->trans_ev = trans_ev;
  o->save = save;
  cp_loaders->insert(cp_loaders,o);
  return o;

};

p_cp_section add_cp_section ( l_text  caption,
                      l_text  fullcaption,
                      l_text  info,
                      l_text  helpfile,
                      BITMAP *icon32  ) {

  p_cp_section o = _malloc(sizeof(t_cp_section));
  if ( !o ) return;
  o->caption = caption;
  o->fullcaption = fullcaption;
  o->info = info;
  o->helpfile = helpfile;
  o->icon32 = icon32;
  cp_sections->insert(cp_sections,o);
  return o;
};
////////////////////////////////////////////////////////////////////////////////
SetInfoAppName("Control Panel 1.1");
SetInfoDesciption("Global program to set Seal's settings");
SetInfoCopyright("(c) Copyright 2001 Point Mad. All rights reserved");
SetInfoManufacturer("Point Mad");
////////////////////////////////////////////////////////////////////////////////
#include"cp.exp"
lib_begin ( void ) {

  if ( ap_process == AP_ALLOC ) {

    AP_EXPORTLIB();

  };

  if ( ap_process == AP_INIT ) {

    cp_loaders = list_init(_malloc(sizeof(t_list)),NULL,0);
    cp_sections = list_init(_malloc(sizeof(t_list)),NULL,0);
    //load_cp();
    cps_display = add_cp_section(TXT_DISPLAY,TXT_DISPLAYSETTINGS,TXT_DISPLAYINFO,NULL,load_image("bmp/cp_disp.ico"));
    cps_hardware = add_cp_section(TXT_HARDWARE,TXT_HARDWARESETTINGS,TXT_HARDWAREINFO,NULL,load_image("bmp/cp_hard.ico"));

  };

} app_end;
