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
#include <grfx.h>
#include <button.h>
#include <dialogs.h>
#include <iodlg.h>
#include <screen.h>
#include <colors.h>
#include <savescr.h>

p_textline tme = NULL;
p_history  ss  = NULL;
p_grfx     preview = NULL;



l_text path = "./";

l_bool changed = false;

#define MSG_HISUPDATE   10003
#define MSG_SS_SETTINGS 10004
#define MSG_SS_TEST     10005

////////////////////////////////////////////////////////////////////////////////
void set_save( void ) {
  l_text ffile = NULL;
  if ( ss->current == 0 ) ffile = NULL;
  else ffile = io_realpath(path, TEXTLINE(ss)->text);

  set_key("current/seal/screensaver/app",ffile);
  set_key_integer("current/seal/screensaver/dif",atoi(tme->text));

  if ( screensaver_app ) _free(screensaver_app);

  screensaver_app = ffile;

  screensaver_dif = atoi(tme->text);

};
////////////////////////////////////////////////////////////////////////////////
void   preview_draw ( p_view o )
{
 t_rect  r = o->get_local_extent(o);
 t_point p;

 BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

	o->background(o, out, rect_move(r, p.x, p.y));

  };

  o->end_of_paint(o, r);
};
////////////////////////////////////////////////////////////////////////////////
void set_trans_ev( p_object o, t_event *event ) {

  if ( o->process == PH_PREPROCESS && event->type & EV_MESSAGE ) {
    l_dword msg = event->message;

    if ( msg == MSG_SS_SETTINGS || msg == MSG_SS_TEST ) {

	  l_text file = NULL;

      clear_event(event);

      if ( ss->current == 0 ) return;

      file = io_realpath(path, TEXTLINE(ss)->text);

      if ( file ) {
	    run_file_args ( file, ( msg == MSG_SS_SETTINGS ) ? "SETTINGS" : NULL );
        _free(file);
      };
    };

  };

};

////////////////////////////////////////////////////////////////////////////////
static p_list screensaver_list ( l_text file, l_int *sel ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);

  if ( p ) {
    struct t_ffblk f;
    l_int done = 0;
    l_text fi = io_realpath(path, "*.sss");
    l_int id = 1;

    p->insert(p, new_history_item(TXT__NONE_, NULL, 0, NULL));

    done = io_findfirst(fi, &f, FA_ARCH);
    while ( !done ) {
      l_text realfile = io_realpath(path, f.ff_filename);
      if ( io_isfilename (f.ff_filename) ) {
        p->insert(p, new_history_item(f.ff_filename, NULL, 0, NULL));
        if ( file && sel ) if(!stricmp(realfile,file))*(sel) = id;
      };
      id++;
      _free(realfile);
      done = io_findnext(&f);
    };

     _free(fi);

  };

  return p;
};
////////////////////////////////////////////////////////////////////////////////
void set_load ( p_object o ) {
  t_rect       r = VIEW(o)->get_local_extent(VIEW(o));
  l_int        a = r.b.y - (2*25);
  p_stattext lbl = NULL;
  p_button     b = NULL;
  t_point     wh = point_assign(r.b.x-r.a.x, a-r.a.y );
  t_point      c = point_assign(r.a.x+(wh.x/2), r.a.y+(wh.y/2) );
  l_int       xw = wh.y*64/48;
  l_int      sel = 0;

  lbl  =    stattext_init( _malloc(sizeof(t_stattext)),
                         rect_assign( 5, a , 100, a + 20 ),
                         TX_ALIGN_CENTER,
                         TXT_WAIT__SEC_);
  o->insert(o,OBJECT(lbl));

  lbl  =    stattext_init( _malloc(sizeof(t_stattext)),
                         rect_assign( 5, a + 25, 100, a + 45 ),
                         TX_ALIGN_CENTER,
                         TXT_SCREEN_SAVER);

  o->insert(o,OBJECT(lbl));

  b  =        button_init( _malloc(sizeof(t_button)),
                         rect_assign( r.b.x - 65, a , r.b.x - 5, a + 20 ),
                         TXT_TEST,
                         MSG_SS_TEST,
                         BF_DEFAULT);
  o->insert(o,OBJECT(b));

  b  =        button_init( _malloc(sizeof(t_button)),
                         rect_assign( r.b.x - 65, a + 25, r.b.x - 5, a + 45 ),
                         TXT_SETTINGS,
                         MSG_SS_SETTINGS,
                         BF_DEFAULT);
  o->insert(o,OBJECT(b));

  tme = textline_init(_malloc(sizeof(t_textline)),
                           rect_assign( 105, a, r.b.x - 85, a + 20 ),
                           IO_DIR_LIMIT,
                           0);

  //_free(tme->text);
  tme->text = set_format_text(NULL,"%d", screensaver_dif);


  ss  = history_init( _malloc(sizeof(t_history)),
                         rect_assign( 105, a + 25, r.b.x - 85, a + 45 ),
                         screensaver_list(screensaver_app,&sel) ,
                         150,
                         HF_REWRITEUNABLE|LF_SELFLIST);


  preview  =    grfx_init( _malloc(sizeof(t_grfx)),
                           rect_assign( c.x - xw/2, 5, c.x + xw/2, a - 5 ));
  VIEW(preview)->draw = &preview_draw;
  VIEW(preview)->brush.color = color_desktop_face;

  ss->current = sel;
  ss->message = MSG_HISUPDATE;


  o->insert(o,OBJECT(tme));
  o->insert(o,OBJECT(ss));
  o->insert(o,OBJECT(preview));


};


////////////////////////////////////////////////////////////////////////////////



SetInfoAppName("Screen saver chooser");
SetInfoDesciption("Sub Program of control panel");
SetInfoCopyright("(c) Copyright 2001 Point Mad. All rights reserved");
SetInfoManufacturer("Point Mad");

app_begin ( void ) {


  if ( ap_process == AP_INIT ) {
          if ( !key_exists("current/seal/screensaver") ) create_key("current/seal","screensaver");
          if ( !key_exists("current/seal/screensaver/dif") ) create_key("current/seal/screensaver","dif");
          if ( !key_exists("current/seal/screensaver/app") ) create_key("current/seal/screensaver","app");

          add_cp_loader(TXT_SCREEN_SAVER,NULL,cps_display,load_image("bmp/saver.ico,32"),load_image("bmp/saver.ico,16"),&set_load,&set_trans_ev,&set_save);
  };

} app_end;
