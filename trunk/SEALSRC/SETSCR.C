/****************************************************************/
/*                                                              */
/*                          setscr.c                            */
/*                                                              */
/*                Seal - free desktop environment               */
/*                                                              */
/*                    Copyright (c) 1999,2000                   */
/*                         Michal Stencl                        */
/*                     All Rights Reserved                      */
/*                                                              */
/* mail : stenclpmd@ba.telecom.sk                               */
/* web  : http://www.home.sk/public/seal/                       */
/*                                                              */
/* This file is part of Seal.                                   */
/*                                                              */
/* Seal is free software; you can redistribute it and/or        */
/* modify it under the terms of the GNU General Public License  */
/* as published by the Free Software Foundation; either version */
/* 2, or (at your option) any later version.                    */
/*                                                              */
/* Seal is distributed in the hope that it will be useful, but  */
/* WITHOUT ANY WARRANTY; without even the implied warranty of   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See    */
/* the GNU General Public License for more details.             */
/*                                                              */
/* You should have received a copy of the GNU General Public    */
/* License along with Seal; see the file COPYING.  If not,      */
/* write to the Free Software Foundation, 675 Mass Ave,         */
/* Cambridge, MA 02139, USA.                                    */
/*                                                              */
/****************************************************************/


#include"allegro.h"
#include"seal.h"
#include"menus.h"
#include"app.h"
#include"button.h"
#include"dialogs.h"
#include"load_htm.h"
#include"iodlg.h"
#include"bcd.h"


#define MSG_BROWSEDRV  100001
#define MSG_TEST       100002
#define MSG_BROWSEBGR  100003


static p_history   resolution = NULL;
static p_history   colors = NULL;
static p_history   style = NULL;
static p_textline  scrdriver = NULL;
static p_textline  backimage = NULL;


static p_list get_drv_items ( void ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_filehistory_item, 0);

  if ( p ) {

     p->insert(p, new_filehistory_item("FreeBE/AF or Scitech(tm) drivers (*.drv)", "*.drv"));
     p->insert(p, new_filehistory_item("All files (*.*)", "*.*"));

  };


  return p;
};


static p_list get_img_items ( void ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_filehistory_item, 0);

  if ( p ) {

     p->insert(p, new_filehistory_item("Jpeg files (*.jpg)", "*.jpg"));
     p->insert(p, new_filehistory_item("Gif files (*.gif)", "*.gif"));
     p->insert(p, new_filehistory_item("Bmp files (*.bmp)", "*.bmp"));
     p->insert(p, new_filehistory_item("Pcx files (*.pcx)", "*.pcx"));
     p->insert(p, new_filehistory_item("Tiff files (*.tif)", "*.tif"));
     p->insert(p, new_filehistory_item("Targa files (*.tga)", "*.tga"));
     p->insert(p, new_filehistory_item("Lbm files (*.lbm)", "*.lbm"));
     p->insert(p, new_filehistory_item("X Window Bitmap files (*.xbm)", "*.xbm"));
     p->insert(p, new_filehistory_item("Iax files (*.iax)", "*.iax"));
     p->insert(p, new_filehistory_item("Pse files (*.pse)", "*.pse"));
     p->insert(p, new_filehistory_item("Pixmap files (*.ppm)", "*.ppm"));
     p->insert(p, new_filehistory_item("Portable Greyscale-map files (*.pgm)", "*.pgm"));
     p->insert(p, new_filehistory_item("Vid files (*.vid)", "*.vid"));
     p->insert(p, new_filehistory_item("All files (*.*)", "*.*"));

  };


  return p;
};


static l_long get_current_resolution_item ( void ) {

  l_long w = (l_long)getininum_fromfile (INI_MAINFILE, INI_SCREEN, "width");

  switch ( w ) {

      case 640  : w = 0; break;
      case 800  : w = 1; break;
      case 1024 : w = 2; break;
      default   : w = 0; break;

  };


  return w;

};


static p_list get_resolution_items ( void ) {


  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);

  if ( p ) {

     p->insert(p, new_history_item("640x480", NULL, 0, NULL));
     p->insert(p, new_history_item("800x600", NULL, 0, NULL));
     p->insert(p, new_history_item("1024x768", NULL, 0, NULL));

  };


  return p;


};


static l_long get_current_style_item ( void ) {

  l_text s = (l_text)getini_fromfile (INI_MAINFILE, INI_DESKTOP, "style");


  if ( s ) {

     if ( !stricmp(s, "(none)") )  return 0;
     if ( !stricmp(s, "stretch") ) return 1;
     if ( !stricmp(s, "center") )  return 2;

  };

  return 0;

};


static p_list get_style_items ( void ) {


  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);

  if ( p ) {

     p->insert(p, new_history_item("pattern", NULL, 0, NULL));
     p->insert(p, new_history_item("stretch", NULL, 0, NULL));
     p->insert(p, new_history_item("center", NULL, 0, NULL));

  };


  return p;


};


static l_long get_current_color_item ( void ) {

  l_long d = (l_long)getininum_fromfile (INI_MAINFILE, INI_SCREEN, "depth");

  switch ( d ) {

      case 8    : d = 0; break;
      case 15   : d = 1; break;
      case 16   : d = 2; break;
      case 24   : d = 3; break;
      default   : d = 0; break;

  };


  return d;

};


static p_list get_colors_items ( void ) {


  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);

  if ( p ) {

     p->insert(p, new_history_item("8 bits (256 colors)", NULL, 0, NULL));
     p->insert(p, new_history_item("15 bits ( 32768 colors)", NULL, 0, NULL));
     p->insert(p, new_history_item("16 bits ( 65536 colors)", NULL, 0, NULL));
     p->insert(p, new_history_item("24 bits ( 16M colors)", NULL, 0, NULL));

  };


  return p;


};


static void trans_ev ( p_object o, p_event event )
{

  /* it's before sub-views process */
  if ( o->process == PH_PREPROCESS && event->type & EV_MESSAGE ) {

       switch ( event->message ) {

           /* set current values */
           case MSG_OK : {

                long w = 640;
                long h = 480;

                /* safe old event */
                t_event e = *event;

                /* info box */
                msgbox( MW_INFO, MB_OK, INI_TEXT("Now SEAL save your graphics settings.\n\nYou will see the change after restarting.\n\nIf some error will occur in graphics, please run SEAL in\n\nsafe mode by '-safe' argument and try to set right values."));

                /* convert resolution text to numbers */
                switch ( resolution->current ) {

                   case 0 : { w = 640; h = 480; }; break;
                   case 1 : { w = 800; h = 600; }; break;
                   case 2 : { w = 1024; h = 768; }; break;

                };

                /* set ini data to file */
                setini_tofile (INI_MAINFILE, INI_SCREEN, "width", (char*)(&w), INI_DECANUM);
                /* set ini data to file */
                setini_tofile (INI_MAINFILE, INI_SCREEN, "height", (char*)(&h), INI_DECANUM);


                /* set ini data to file */
                setini_tofile (INI_MAINFILE, INI_DESKTOP, "style", TEXTLINE(style)->text, INI_STRING);

                /* destroy current desktop wallpaper */
                if ( desktop->brush.state & BRUSH_SELFIMG )

                        destroy_bitmap(desktop->brush.background);

                desktop->brush.background = NULL;

                /* convert style text to BRUSH_xxxx styles */
                switch ( style->current ) {

                   case 0 : desktop->brush.state &= ~(BRUSH_STRETCH|BRUSH_CENTER); break;
                   case 1 : desktop->brush.state |= BRUSH_STRETCH; break;
                   case 2 : desktop->brush.state |= BRUSH_CENTER; break;

                };

                /* load new image for background */
                if ( backimage->text )

                      desktop->brush.background = load_image(backimage->text);

                desktop->brush.state |= BRUSH_SELFIMG;

                /* redraw only background of the desktop ( not redraw sub-views ) */
                desktop->draw_me(desktop);

                /* set ini data to file */
                setini_tofile (INI_MAINFILE, INI_DESKTOP, "wall_paper", backimage->text, INI_STRING);

                /* set cfg data to file */
                set_config_string(NULL, "vbeaf_driver", scrdriver->text);

                /* restore old events */
                (*event) = e;

           }; break;

           /* find screen driver by browsing over directory */
           case MSG_BROWSEDRV : {

                 /* get choosed file */
                 l_text file = file_dialog(INI_TEXT("Browse"), INI_TEXT("Open"), "c:/", NULL, get_drv_items(), FA_ALL, FL_OPEN, NULL);

                 if ( file )
                     /* set line to file */
                     scrdriver->set_text(scrdriver, file);

                 clear_event(event);

                 _free(file);

           }; break;

           /* find background image by browsing over directory */
           case MSG_BROWSEBGR : {

                 /* get choosed file */
                 l_text file = file_dialog(INI_TEXT("Browse"), INI_TEXT("Open"), "c:/", NULL, get_img_items(), FA_ALL, FL_OPEN, NULL);

                 if ( file )
                     /* set line to file */
                     backimage->set_text(backimage, file);

                 clear_event(event);

                 _free(file);

           }; break;

       };

  };

};


void init_setscreen ( void )
{

   t_rect   r = rect_assign(0, 0, 380, 340);

   p_appwin p = appwin_init(_malloc(sizeof(t_appwin)),
                            r,
                            INI_TEXT("Screen settings"),
                            WF_MINIMIZE,
                            ap_id,
                            &trans_ev);

   /* center window */
   if ( p ) VIEW(p)->align |= TX_ALIGN_CENTER;

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(p));

   if ( p ) {

      t_rect   l = VIEW(p)->size_limits(VIEW(p));
      t_rect   x = rect_assign(l.a.x+10, l.a.y+10, l.a.x+250, l.a.y+30);

      p_button b = NULL;

      /* insert new text */
      p_stattext s = stattext_init(_malloc(sizeof(t_stattext)),
                                   x,
                                   TX_ALIGN_BOTTOM,
                                   INI_TEXT("Resolution"));

      /* insert stattext to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(s));

      x = rect_assign(x.a.x, x.b.y+1, x.b.x, x.b.y+21);

      /* make new history for resolution */
      resolution = history_init(_malloc(sizeof(t_history)),
                                 x,
                                 get_resolution_items(),
                                 150,
                                 HF_REWRITEUNABLE|LF_SELFLIST);

      /* insert history to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(resolution));

      /* get current mode */
      if ( resolution ) resolution->rewrite_item(resolution, get_current_resolution_item());

      x = rect_assign(x.a.x, x.b.y+10, x.b.x, x.b.y+31);

      /* insert new text */
      s = stattext_init(_malloc(sizeof(t_stattext)),
                                   x,
                                   TX_ALIGN_BOTTOM,
                                   INI_TEXT("Colors"));

      /* insert stattext to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(s));

      x = rect_assign(x.a.x, x.b.y+1, x.b.x, x.b.y+21);

      /* make new history for colors */
      colors = history_init(_malloc(sizeof(t_history)),
                            x,
                            get_colors_items(),
                            150,
                            HF_REWRITEUNABLE|LF_SELFLIST);

      /* insert history to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(colors));

      /* get current mode */
      if ( colors ) colors->rewrite_item(colors, get_current_color_item());

      x = rect_assign(x.a.x, x.b.y+10, x.b.x, x.b.y+31);

      /* insert new text */
      s = stattext_init(_malloc(sizeof(t_stattext)),
                                   x,
                                   TX_ALIGN_BOTTOM,
                                   INI_TEXT("Driver"));

      /* insert stattext to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(s));

      x = rect_assign(x.a.x, x.b.y+1, x.b.x, x.b.y+21);

      /* make new history for colors */
      scrdriver = textline_init(_malloc(sizeof(t_textline)),
                            x,
                            IO_DIR_LIMIT,
                            0);

      /* insert history to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(scrdriver));

      /* get current driver */
      if ( scrdriver ) {

            l_text m = (l_text)get_config_string(NULL, "vbeaf_driver", NULL);

            scrdriver->set_text(scrdriver, m);

      };

      r = rect_assign(x.b.x+10, x.a.y, x.b.x+90, x.a.y+20);

      /* make new button for screen driver finding */
      b = button_init(_malloc(sizeof(t_button)),
                            r,
                            INI_TEXT("Browse..."),
                            MSG_BROWSEDRV,
                            BF_NORMAL);

      /* insert history to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(b));

      x = rect_assign(x.a.x, x.b.y+10, x.b.x, x.b.y+31);

      /* insert new text */
      s = stattext_init(_malloc(sizeof(t_stattext)),
                                   x,
                                   TX_ALIGN_BOTTOM,
                                   INI_TEXT("Wallpaper"));

      /* insert history to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(s));

      x = rect_assign(x.a.x, x.b.y+1, x.b.x, x.b.y+21);

      /* make new history for colors */
      backimage = textline_init(_malloc(sizeof(t_textline)),
                            x,
                            IO_DIR_LIMIT,
                            0);

      /* insert history to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(backimage));

      /* get current wallpaper */
      if ( backimage ) {

          l_text m = getini_fromfile (INI_MAINFILE, INI_DESKTOP, "wall_paper");

          backimage->set_text(backimage, m);

          _free(m);
      };


      r = rect_assign(x.b.x+10, x.a.y, x.b.x+90, x.a.y+20);

      /* make new button for screen driver finding */
      b = button_init(_malloc(sizeof(t_button)),
                            r,
                            INI_TEXT("Browse..."),
                            MSG_BROWSEBGR,
                            BF_NORMAL);

      /* insert history to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(b));

      x = rect_assign(x.a.x, x.b.y+10, x.b.x, x.b.y+31);

      /* insert new text */
      s = stattext_init(_malloc(sizeof(t_stattext)),
                                   x,
                                   TX_ALIGN_BOTTOM,
                                   INI_TEXT("Wallpaper style"));

      /* insert history to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(s));

      x = rect_assign(x.a.x, x.b.y+1, x.b.x, x.b.y+21);

      /* make new history for colors */
      style = history_init(_malloc(sizeof(t_history)),
                            x,
                            get_style_items(),
                            50,
                            HF_REWRITEUNABLE|LF_SELFLIST);

      /* insert history to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(style));

      /* get style mode */
      if ( style ) style->rewrite_item(style, get_current_style_item());

      r = rect_assign(x.a.x, x.b.y+20, x.a.x+103, x.b.y+40);

      /* make new OK button */
      b = button_init(_malloc(sizeof(t_button)),
                            r,
                            INI_TEXT("Ok"),
                            MSG_OK,
                            BF_DEFAULT);

      /* insert history to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(b));

      r = rect_assign(r.b.x+10, r.a.y, r.b.x+113, r.b.y);

      /* make new OK button */
      b = button_init(_malloc(sizeof(t_button)),
                            r,
                            INI_TEXT("Cancel"),
                            MSG_CLOSE,
                            BF_NORMAL);

      /* insert history to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(b));

      r = rect_assign(r.b.x+10, r.a.y, r.b.x+113, r.b.y);

      /* make new TEST button */
      b = button_init(_malloc(sizeof(t_button)),
                            r,
                            INI_TEXT("Test"),
                            MSG_TEST,
                            BF_NORMAL);

      /* insert history to window */
      OBJECT(p)->insert(OBJECT(p), OBJECT(b));

   };

};


app_begin ( void ) {


  if ( ap_process == AP_INIT ) {

      AP_SETNUMOFCALLS(1); /* set max num of calls */

      init_setscreen();

  };

  if ( ap_process == AP_FREE ) {

  };

} app_end;

