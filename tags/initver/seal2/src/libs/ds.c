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


p_history  align   = NULL;
p_history  bitmaps = NULL;
p_grfx     preview = NULL;

l_text f_wallpaper = NULL;

l_text path = "WALLPAPR/";
l_text other_path;
l_bool changed = false;

#define MSG_HISUPDATE  10003
#define MSG_OTHER_BMP  10002

BITMAP *resizebmp(BITMAP *bmp,l_int w,l_int h) {
  BITMAP *o = create_bitmap(w,h);
  stretch_sprite(o,bmp,0,0,w,h);
  destroy_bitmap(bmp);
  return o;
};

////////////////////////////////////////////////////////////////////////////////
void s_update ( void ) {
  if ( align && bitmaps && preview ) {
  l_int w = rect_sizex(VIEW(preview)->bounds);
  BITMAP *bck;

  if ( !f_wallpaper || bitmaps->current == 0 )
    bck = NULL;
  else if ( bitmaps->current == 1 )
    bck = load_image(f_wallpaper);
  else
    bck =  load_image( io_realpath(path, TEXTLINE(bitmaps)->text) );

  VIEW(preview)->brush.color = desktop->brush.color;

  if ( VIEW(preview)->brush.background ) {
    destroy_bitmap(VIEW(preview)->brush.background);
    VIEW(preview)->brush.background = NULL;
  };
  if ( bck ) {

    VIEW(preview)->brush.background = ( align->current == 1 ) ? bck : resizebmp(bck, IMAGE_WIDTH(bck)*w/screen_width,IMAGE_HEIGHT(bck)*w/screen_width);

    VIEW(preview)->brush.state   = BRUSH_SELFIMG;

    if ( align->current == 2 )
      VIEW(preview)->brush.state |= BRUSH_CENTER;
    else if ( align->current == 1 )
      VIEW(preview)->brush.state |= BRUSH_STRETCH;

  } else {

    VIEW(preview)->brush.background = NULL;
  };

  view_draw_view(VIEW(preview));
  };
};
////////////////////////////////////////////////////////////////////////////////
void load_desk_sets ( l_int style, l_text wallpaper ) {

    desktop->brush.color = color_desktop_face;

    if ( wallpaper ) {

      if ( VIEW(desktop)->brush.background ) {
        destroy_bitmap(VIEW(desktop)->brush.background);
        VIEW(desktop)->brush.background = NULL;
      };

      desktop->brush.background = load_image(wallpaper);

      desktop->brush.state       = BRUSH_SELFIMG;

        if ( style == 1 )
           desktop->brush.state |= BRUSH_STRETCH;

        else if ( style == 2 )
           desktop->brush.state |= BRUSH_CENTER;

    } else {
      desktop->brush.background = NULL;
    };

};
////////////////////////////////////////////////////////////////////////////////
void set_save( void ) {
  if ( changed ) {
    l_text ffile = NULL;
    if ( bitmaps->current == 0 ) ffile = NULL;
    else if ( bitmaps->current == 1 ) ffile = _strdup(f_wallpaper);
    else ffile = io_realpath(path, TEXTLINE(bitmaps)->text);

    set_key("current/desktop/wallpaper",ffile);
    set_key_integer("current/desktop/placement",align->current);
    load_desk_sets(align->current,ffile);
    desktop->draw_view(desktop);
    _free(ffile);
    changed = false;
  };
};
////////////////////////////////////////////////////////////////////////////////
void   preview_draw ( p_view o )
{
 t_rect  r = o->get_local_extent(o);
 t_point p;

 BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {
    /*l_int x = 128*r.b.x/screen_width;
    l_int y = 32*r.b.y/screen_height;*/

    o->background(o, out, rect_move(r, p.x, p.y));

    //rectfill ( out, p.x,p.y,p.x+x,p.y+y,COLOR(8));


  };

  o->end_of_paint(o, r);
};
////////////////////////////////////////////////////////////////////////////////
static p_list get_img_items ( void ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_filehistory_item, 0);

  if ( p ) {

     p->insert(p, new_filehistory_item("JPEG Files (*.jpg)", "*.jpg"));
     p->insert(p, new_filehistory_item("GIF Files (*.gif)", "*.gif"));
     p->insert(p, new_filehistory_item("BMP Files (*.bmp)", "*.bmp"));
     p->insert(p, new_filehistory_item("PCX Files (*.pcx)", "*.pcx"));
     p->insert(p, new_filehistory_item("All Files (*.*)", "*.*"));

  };


  return p;
};
////////////////////////////////////////////////////////////////////////////////
void set_trans_ev( p_object o, t_event *event ) {

  if ( o->process == PH_PREPROCESS && event->type & EV_MESSAGE )
    switch ( event->message ) {
      case MSG_OTHER_BMP : {
        l_text file = file_dialog(INI_TEXT("Browse"), TXT_OPEN, other_path, NULL, get_img_items() , FA_ALL, FL_OPEN, NULL);

        if ( file ) {
           f_wallpaper = file;
           bitmaps->current = 1;
           TEXTLINE(bitmaps)->set_text(TEXTLINE(bitmaps), "(other)");
           if ( other_path ) _free(other_path);
           other_path = io_path(file);
           s_update();

        };
        clear_event(event);
       }; break;
       case MSG_HISUPDATE : {
         s_update();
         changed = true;
         clear_event(event);
      }; break;
    };

};
////////////////////////////////////////////////////////////////////////////////
static p_list align_list ( void ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);

  if ( p ) {

     p->insert(p, new_history_item(TXT_PATERN,  NULL, 0, NULL));
     p->insert(p, new_history_item(TXT_STRETCH, NULL, 0, NULL));
     p->insert(p, new_history_item(TXT_CENTER,  NULL, 0, NULL));

  };

  return p;
};

////////////////////////////////////////////////////////////////////////////////
static p_list bitmaps_list ( l_text file, l_int *sel ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);

  if ( p ) {
    struct t_ffblk f;
    l_int done = 0;
    l_text fi = io_realpath(path, "*.*");
    l_int id = 2;
    p->insert(p, new_history_item(TXT__NONE_, NULL, 0, NULL));
    p->insert(p, new_history_item(TXT__OTHER_, NULL, 0, NULL));


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
  l_text    wallpaper_file = get_key( "current/desktop/wallpaper" );
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
                         TXT_WALLPAPER_ALIGN);
  o->insert(o,OBJECT(lbl));

  lbl  =    stattext_init( _malloc(sizeof(t_stattext)),
                         rect_assign( 5, a + 25, 100, a + 45 ),
                         TX_ALIGN_CENTER,
                         TXT_WALLPAPER);

  o->insert(o,OBJECT(lbl));

  b  =        button_init( _malloc(sizeof(t_button)),
                         rect_assign( r.b.x - 55, a+25 , r.b.x - 5, a + 45 ),
                         TXT_OTHER,
                         MSG_OTHER_BMP,
                         BF_DEFAULT);

  o->insert(o,OBJECT(b));


  align  = history_init( _malloc(sizeof(t_history)),
                         rect_assign( 105, a, r.b.x - 75, a + 20 ),
                         align_list() ,
                         150,
                         HF_REWRITEUNABLE|LF_SELFLIST);

  bitmaps  = history_init( _malloc(sizeof(t_history)),
                         rect_assign( 105, a + 25, r.b.x - 75, a + 45 ),
                         bitmaps_list(wallpaper_file,&sel) ,
                         150,
                         HF_REWRITEUNABLE|LF_SELFLIST);


  preview  =    grfx_init( _malloc(sizeof(t_grfx)),
                           rect_assign( c.x - xw/2, 5, c.x + xw/2, a - 5 ));
  VIEW(preview)->draw = &preview_draw;

  if ( key_exists("current/desktop/placement") ) align->current = get_key_integer("current/desktop/placement");

  bitmaps->current = 0;
  if ( wallpaper_file ) {
    l_text ppath = io_path(wallpaper_file);
    if ( ppath && !stricmp(ppath,path) ) {
      bitmaps->current = sel;
     } else if ( io_exist(wallpaper_file) ){
       bitmaps->current = 1;
       f_wallpaper = _strdup(wallpaper_file);
       if ( other_path ) _free(other_path);
       other_path = io_path(f_wallpaper);
     };
     if ( ppath ) _free(ppath);
  };

  bitmaps->message = MSG_HISUPDATE;
  align->message = MSG_HISUPDATE;

  o->insert(o,OBJECT(align));
  o->insert(o,OBJECT(bitmaps));
  o->insert(o,OBJECT(preview));

  s_update();


};


////////////////////////////////////////////////////////////////////////////////



SetInfoAppName("Desktop wallpaper Settings");
SetInfoDesciption("Sub Program of control panel");
SetInfoCopyright("(c) Copyright 2001 Point Mad. All rights reserved");
SetInfoManufacturer("Point Mad");

app_begin ( void ) {


  if ( ap_process == AP_INIT ) {
          other_path = _strdup("/");
          if ( !key_exists("current/desktop") ) create_key("current","desktop");
          if ( !key_exists("current/desktop/wallpaper") ) create_key("current/desktop","wallpaper");
          if ( !key_exists("current/desktop/placement") ) create_key("current/desktop","placement");

          add_cp_loader(TXT_DESKTOP_WALLPAPER,NULL,cps_display,load_image("bmp/wall.ico,32"),load_image("bmp/wall.ico,16"),&set_load,&set_trans_ev,&set_save);
  };

} app_end;
