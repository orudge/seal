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

/**************************************/
/* SFA Player                         */
/* Version 0.71                       */
/*                                    */
/* Copyright (c) Florian Xaver 2000   */
/* Copyright (c) Owen Rudge 2001-2002 */
/* Copyright (c) Julien Etelain 2001  */
/**************************************/

/* Revision History:
 *
 * DATE                 CHANGE                          AUTHOR          VERSION
 * February(?) 2000     Created SFA Player              Florian Xaver   0.1
 * ... 2000 ...         Improved, etc                   Florian Xaver   0.2-0.5
 * April 2001           Added support for MODs          Owen Rudge      0.6
 * September 2001       Ported to SEAL 2.0              Owen Rudge      0.7
 * September 2001       SEAL 2.0 fixes                  Julien Etelain  0.7
 * 16th September 2001  Gets file list from loaded SFA  Owen Rudge      0.7
 *                      drivers
 * 16th September 2001  Expanded size of info label     Owen Rudge      0.7
 * 16th September 2001  Attempted to add Pause button   Owen Rudge      0.7
 * 4th October 2001     Added About Driver command      Owen Rudge      0.7
 * 18th January 2002    Updated copyright, etc          Owen Rudge      0.7
 * 1st April 2002       Fixed strcpy() bug              Owen Rudge      0.71
 * 2nd April 2002       Tried to fix event clearing bug Owen Rudge      0.71
 * 2nd April 2002       Added Play, Pause and Stop btns Owen Rudge      0.71
 */

#include <seal.h>
#include <app.h>
#include <button.h>
#include <dialogs.h>
#include <iodlg.h>
#include <sound.h>
#include <grfx.h>

#define PAUSE_ATTEMPT   1

SetInfoAppName("SFA Player");
SetInfoDesciption("SFA Player");
SetInfoCopyright("Copyright (c) Florian Xaver 2000, Owen Rudge 2001-2002. All Rights Reserved.");
SetInfoManufacturer("SEAL programmers");

#ifdef PAUSE_ATTEMPT
   #define MSG_PAUSE    11100
   #define MSG_PLAY     11101
   #define MSG_STOP     11102
#endif

#define MSG_ABOUT_DRV   11001

//BITMAP *ico;

p_play pp = 0;
l_int file_is_playing = 0;  // 0 = false
l_bool stopped = FALSE;

void poll_func_callback (p_object s);
void icon_draw (p_view o);

p_list get_player_filelist (void)
{
                                                /* function for free-ing items */
   p_list p = list_init(malloc(sizeof(t_list)), &free_filehistory_item, 0);
   p_item x = sound_format_list->first(sound_format_list);
   p_item f = x;
   char buf[350];
   char ext[10];

   if (p)
   {
      do {
         sprintf(buf, "%s (*.%s)", ((p_soundformat_item)x->rec)->filedesc, ((p_soundformat_item)x->rec)->extension);
         sprintf(ext, "*.%s", ((p_soundformat_item)x->rec)->extension);

         p->insert(p, new_filehistory_item(buf, ext));
         x = x->next;
      } while (x != f);

/*      p->insert(p, new_filehistory_item("MPEG Layer 2/3 (*.mp?)", "*.mp?"));
      p->insert(p, new_filehistory_item("MIDI (*.mid)", "*.mid"));
      p->insert(p, new_filehistory_item("MOV (*.mov)", "*.mov"));
      p->insert(p, new_filehistory_item("MOD (*.mod)", "*.mod"));
      p->insert(p, new_filehistory_item("MOD - XM (*.xm)", "*.xm"));
      p->insert(p, new_filehistory_item("MOD - IT (*.it)", "*.it"));
      p->insert(p, new_filehistory_item("MOD - S3M (*.s3m)", "*.s3m"));
      p->insert(p, new_filehistory_item("MOD - JGM (*.jgm)", "*.jgm"));
      p->insert(p, new_filehistory_item("All files (*.*)", "*.*"));*/
   }

   return p;
}

void play(l_text file)
{
//     seal_error(ERR_INFO, "Want to play file [%s]", file);
   if (!pp) return;

   pp->filename = strdup(file); // OCR: was: strcpy(pp->filename, file);

   if (!pp->init_driver(pp))
      seal_error(ERR_INFO, "Unable to initialise driver for %s", file);
   else
   {
      if (!pp->init_file(pp, file, FALSE))
         seal_error(ERR_INFO, "Error opening file [%s]", file);       
      else
      {
         if (!pp->play_file(pp)) return;
         file_is_playing = 1;
      }
   }
}

void poll_func_callback(p_object s) /*  called each second */
{
   if (pp)
   {
      if (stopped == FALSE)
      {
         file_is_playing = pp->poll_file(pp);

         if (file_is_playing == 0)
         {
            p_event e = malloc(sizeof(t_event));

            clear_event(e);
            set_event(e, EV_MESSAGE, MSG_CLOSE, s);

            s->put_event(s, e);
            clear_event(e);
         }
      }
   }
}

void player_translate_event (p_object o, p_event e)
{
   RETVIEW(o, e);
//   file_is_playing = pp->poll_file(pp);

   if (o->phase == PH_PREPROCESS)
   {
      if (e->type == EV_MESSAGE)
      {
         switch (e->message)
         {
            case MSG_CLOSE:
               if (pp)
               {
                  pp->stop_file(pp);
                  pp->close_file(pp);
                  pp->terminate_driver(pp);

                  _free(pp);
                  pp = NULL;
               }

               if (APPWIN(o)->icon16)
               {
                  destroy_bitmap(APPWIN(o)->icon16);
                  APPWIN(o)->icon16 = NULL;
               }

//               clear_event(e);
               break;

            case MSG_ABOUT:
               msgbox(MW_INFO, MB_OK, "SFA Player\nVersion 0.71\n\nCopyright (c) Florian Xaver 2000\nCopyright (c) Owen Rudge 2001-2002\nCopyright (c) Julien Etelain 2001\n\nAll Rights Reserved.\n\nSFA is licensed under the terms of the GNU\nGeneral Public License.");
               clear_event(e);
               break;

            case MSG_ABOUT_DRV:
               pp->display_about(pp);
               clear_event(e);
               break;

#ifdef PAUSE_ATTEMPT
            case MSG_PAUSE:
               if (pp->pause_file(pp) == TRUE)
                  stopped = TRUE;
               else
                  stopped = FALSE;

               clear_event(e);
               break;

            case MSG_PLAY:
               stopped = FALSE;

               pp->play_file(pp);
               clear_event(e);
               break;

            case MSG_STOP:
               stopped = TRUE;

               pp->stop_file(pp);
               clear_event(e);
               break;
#endif
         }
      }
   }
}

static t_point new_win_size_minimum (p_view o)
{

  return(point_assign(100, 20));
}

void stat1_func_callback (p_object s) /* it's call each second */
{
   if ( pp ) {
     l_text txt;
     l_int t = pp->get_pos(pp);
     l_int l = pp->get_len_file(pp);

     if ( l )
       txt = set_format_text(NULL, "%02d:%02d / %02d:%02d", t/60,t%60,l/60,l%60);
     else
       txt = set_format_text(NULL, "%02d:%02d", t/60,t%60);

     TEXTLINE(s)->set_text(TEXTLINE(s),txt);
     _free(txt);
  };
}

app_begin (void)
{
   if (ap_process == AP_INIT)
   {
      t_rect r;
      p_appwin o = 0;
      p_object stat = 0;
      p_textline stat1 = 0;
      p_grfx icon = 0;
      l_text info = NULL;

#ifdef PAUSE_ATTEMPT
      p_button b = 0;
#endif
      char file[260];
      SFAINFO inf;

//    AP_SETNUMOFCALLS(1);

      if (get_sound_installed() == false);
         // dummy, this program don't start if dlxsound hasn't loaded before

      if (!ap_args) ap_args = open_dialog("/", NULL, get_player_filelist());
      if (!ap_args) return;

      strcpy(file, ap_args);

//      ico = load_image("bmp/snd.ico,32");

      pp = SFA_init();
      play(file);

      r = rect_assign(0, 0, 400, 160); // WAS ,100);

      o = appwin_init(malloc(sizeof(t_appwin)), /* make window */
                            r,
                            "SFA Player 0.71 Beta",
                            WF_MINIMIZE|WF_ABOUT,
                            ap_id, /* application id */
                            &player_translate_event);

      o->menu = standard_system_menu_items(o,
                new_menu_line(
                new_menu_item("About Driver...", 0, 0, MSG_ABOUT_DRV, NULL,
                new_menu_item("About SFA Player...", 0, 0, MSG_ABOUT, NULL,
                NULL))));

      o->icon16 = load_image("bmp/snd.ico,16");

      VIEW(o)->size_minimum = &new_win_size_minimum;   
      OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(o));

      r = rect_assign(10, 30, 380, 129); // WAS 70);

      pp->get_file_info(pp, &inf);
      
      if (inf.title[0] != NULL || inf.album[0] != NULL || inf.author[0] != NULL || inf.comment[0] != NULL || inf.genre[0] != NULL)
      {
         info = set_format_text(NULL, "Title: %s\nAuthor: %s\nAlbum: %s\nGenre: %s\nComment: %s\n\nFrequency: %dHz - %d bits Bitrate: %dkbps, %s",
            inf.title, inf.author, inf.album, inf.genre, inf.comment, inf.freq, inf.bits, inf.bitrate, inf.channels == 1 ? "Mono" : "Stereo");
      }
      else
      {
         if (inf.freq != NULL || inf.bits != NULL || inf.bitrate != NULL || inf.channels != NULL)
         {
            info = set_format_text(NULL, "Frequency: %dHz - %d bits\nBitrate: %dkbps\n%s",
               inf.freq, inf.bits, inf.bitrate, inf.channels == 1 ? "Mono" : "Stereo");
         }
         else
            info = _strdup("");
      }

      //info = pp->get_friendly_name(pp); // TODO: reimplement!!! pp->get_additional_info(pp);

      stat = stattext_init (malloc(sizeof(t_stattext)),
					 r, 
					 TX_ALIGN_LEFT, // limit
					 "Playing %s...\n%s",
                get_filename(ap_args),
                info);

      _free(info);

      OBJECT(stat)->func_callback = &poll_func_callback;

      OBJECT(o)->insert(OBJECT(o), OBJECT(stat));

      OBJECT(stat)->process_tick = 20; /* each second rewrite it */

      init_stillprocess ( OBJECT(stat), 100 );

     
      r = rect_assign(10, 130, 190, 150); // WAS 70, 190, 90

      stat1 = textline_init( _malloc(sizeof(t_textline)),
                            r,
                            255,
                            TF_REWRITEUNABLE);

      OBJECT(stat1)->func_callback = &stat1_func_callback;
      OBJECT(o)->insert(OBJECT(o), OBJECT(stat1));
 
      stat1->set_text(stat1, "Welcome!");

      init_stillprocess ( OBJECT(stat1), 1000 );

/*      icon = grfx_init(_malloc(sizeof(t_grfx)),              // OCR: An attempt at adding
                         rect_assign(363, 10, 395, 42));       // OCR: a nice icon to the window
                                                               // OCR: It didn't work. :-(
      OBJECT(icon)->func_callback = NULL;
      VIEW(icon)->draw = &icon_draw;
      OBJECT(o)->insert(OBJECT(o), OBJECT(icon));*/

#ifdef PAUSE_ATTEMPT
      r = rect_assign(200, 130, 220, 150);
      b = button_init(malloc(sizeof(t_button)), r, "b", MSG_PLAY, BF_DEFAULT);
      VIEW(b)->font = get_font_in_size("Symbols", 10, 10);

      OBJECT(o)->insert(OBJECT(o), OBJECT(b));

      r = rect_assign(230, 130, 250, 150);
      b = button_init(malloc(sizeof(t_button)), r, "a", MSG_PAUSE, BF_DEFAULT);
      VIEW(b)->font = get_font_in_size("Symbols", 10, 10);

      OBJECT(o)->insert(OBJECT(o), OBJECT(b));

      r = rect_assign(260, 130, 280, 150);
      b = button_init(malloc(sizeof(t_button)), r, "W", MSG_STOP, BF_DEFAULT);
      VIEW(b)->font = get_font_in_size("Symbols", 10, 10);

      OBJECT(o)->insert(OBJECT(o), OBJECT(b));
#endif
   }
} app_end;

/*void icon_draw (p_view o)
{
   t_rect  r = o->get_local_extent(o);
   t_point p;

   BITMAP *out = o->begin_paint(o, &p, r);

   if (out)
   {
      rectfill(out,p.x,p.y,p.x+r.b.x,p.y+r.b.y,color_3d_face);
      draw_sprite(out, ico, r.b.x, r.b.y);
   }

   o->end_of_paint(o, r);
}*/
