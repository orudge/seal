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

/* Revision History:
 *
 * 13/04/2000 - Florian Xaver:
 *              + supports more than 1 cd drive
 *              + shows time
 *              + supports: forward/rewind ( << >> )
 *
 * 08/06/2001 - jdh added sane (==nasty text) defaults if fonts not found
 *
 * 30/03/2002 - Added About dialog (orudge)
 *
 * 04/04/2002 - Updated with new icon (orudge)
 */

#include <allegro.h>
#include "seal.h"
#include "menus.h"
#include "app.h"
#include "button.h"
#include "dialogs.h"
#include "iodlg.h"
#include "bcd.h"
#include "txts.h"

SetInfoAppName("CD Player");
SetInfoDesciption("CD Player");
SetInfoCopyright("Copyright (c) Michal Stencl 1999-2000");
SetInfoManufacturer("Michal Stencl");

/* text infos */

#define TXT_ERRORINLOADINGAUDIO INI_TEXT("Error in loading audio")
#define TXT_TITEL               INI_TEXT("CD Player - drive %c:")
#define TXT_CDPLAYER            INI_TEXT("CD Player")
#define TXT_STOP                INI_TEXT("Stops playing")
#define TXT_PLAY                INI_TEXT("Plays/resumes pause")
#define TXT_PAUSE               INI_TEXT("Pause")
#define TXT_LAST                INI_TEXT("Plays last track")
#define TXT_NEXT                INI_TEXT("Plays next track")
#define TXT_REWIND              INI_TEXT("Rewind (~4 seconds)")
#define TXT_FORWARD             INI_TEXT("Forward (~4 seconds)")
#define TXT_OPEN_CLOSE          INI_TEXT("Opens/closes CD-ROM")
#undef TXT_TIME
#define TXT_TIME                INI_TEXT("Chooses which time ( /r) or address (a)  is displayed")
#define TXT_DRIVE               INI_TEXT("Changes CD-ROM drive, if more than one are avaiable")
#define TXT_PLAYBACKGROUND      INI_TEXT("Closes CD Player without stopping")

/* messages */

#define MSG_PLAY              10001
#define MSG_PAUSE             10002
#define MSG_STOP              10003
#define MSG_RR                10004
#define MSG_FF                10005
#define MSG_OUT               10006
#define MSG_PREV              10007
#define MSG_NEXT              10008
#define MSG_RBACK             10009
#define MSG_PLAYATBACKGROUND  10010
#define MSG_TIME              10011
#define MSG_DRIVE             10012


l_int  cd_track = 1;
l_int  cd_track_max = 1;
l_int  cd_track_pause = 0;
l_int  cd_track_in_background = 0;
l_long cd_track_start = 0;

l_int  time_type = 0;

int cur_drive_no=0;


void update_titel(p_object o,l_text text)
{
      /// Redraws the titel from the window

    _free(WINDOW(o)->caption);

    WINDOW(o)->caption = set_format_text(NULL, "%s", text);

      /* title, but not redraw any objects in the title such as X  */

    TEST_SUB_VIEWS(VIEW(o), WINDOW(o)->draw_title(WINDOW(o)));
}


static l_text return_hsg(int hsg) {
  l_int hours, minutes, seconds;
  l_text time_string;
  seconds = hsg / 75;
  minutes = seconds / 60;
  seconds %= 60;
  hours = minutes / 60;
  minutes %= 60;
  time_string = set_format_text(NULL, "%d:%02d:%02d", hours, minutes, seconds);
  return time_string;
}


l_text get_song_time()
{

   l_int start;
   l_int len;
   l_text time_string;
   l_int track =  bcd_now_playing();
   l_int current_address;

   if (track == 0) return "";

   current_address = bcd_audio_position();



   bcd_get_track_address(track, &start, &len);

   switch (time_type)
    {
      case 0:
       time_string = set_format_text(NULL, "%s", return_hsg(current_address - start));
      break;
      case 1:
       time_string = set_format_text(NULL, "r%s", return_hsg(start + len - current_address));
      break;
      case 2:
      default: // stop complaint about uninitialised string
       time_string = set_format_text(NULL, "a%d", current_address);
      break;
    };

   return time_string;
}


void  trackinfo_draw ( p_view o )
{

  t_rect  r = o->get_local_extent(o);
  t_point p;
  BITMAP *out ;

  out = o->begin_paint(o, &p, r);

  if ( out ) {

    l_text about;

    if (cd_track_pause == 0)
           about = set_format_text(NULL, "[song %i/%i %s]", cd_track, bcd_get_audio_info(), get_song_time());
    else
           about = set_format_text(NULL, "[song %i/%i] %s", cd_track, bcd_get_audio_info(), TXT_PAUSE); // OCR - 29/3/2002



    o->background(o, out, rect_move(r, p.x, p.y));

    button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y,
                0, makecol(255,255,255));



    textout_draw_rect(out, o->font, about, -1, r.a.x+p.x+2, r.a.y+p.y+2,
                           r.b.x+p.x-2, r.b.y+p.y-2, TX_ALIGN_CENTER, makecol(255,255,255), TX_NOCOLOR, 0);

    _free(about);

  };

  o->end_of_paint(o, r);

};


void  trackinfo_func_callback ( p_object s ) /* it's call each second */
{

    l_int p = bcd_now_playing();

    VIEW(s)->draw_view(VIEW(s));

    if ( p ) cd_track = p;

};


p_view  trackinfo_init ( p_view o, t_rect r )
{

   if ( !o ) return NULL;

   view_init(o, r);

   OBJECT(o)->process_tick = 1000; /* each second rewrite it */

   OBJECT(o)->func_callback = &trackinfo_func_callback;

   o->draw = &trackinfo_draw;
   o->font = get_font_in_size("ActivaCE", 14, 14);
   if(!o->font)o->font=font_system;

   OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
   OBJECT(o)->set_options(OBJECT(o), OB_OF_STILLPROCESS, true);

   VIEW(o)->brush.color = 0;

   return o;
};


static void bcd_play_to_end ( l_int track)
{
// ff_re, to play forward or rewind, else 0

   l_int start;
   l_int len;
   l_int t = cd_track_max = bcd_get_audio_info();

   bcd_get_track_address(track, &start, &len);

   while ( t > track ) {

      l_int l;
      l_int s;

      bcd_get_track_address(t, &s, &l);

      len += l;

      t--;

   };

   bcd_play(start, len);                         // play track.

};


// ff_re, to play forward or rewind
static void bcd_play_ff_or_re ( signed int ff_re)
{

   l_int cur_track = bcd_now_playing();
   l_int len;
   l_int start;
   l_int t = cd_track_max = bcd_get_audio_info();

   bcd_get_track_address(cur_track, &start, &len);

   while ( t > cur_track ) {

      l_int l;
      l_int s;

      bcd_get_track_address(t, &s, &l);

      len += l;

      t--;

   };

   bcd_play(bcd_audio_position() + ff_re, len);   // if forward or rewind


};

/* cd - player translate event function */

void  trans_ev ( p_object o, p_event event )
{

   if ( event->type == EV_MESSAGE ) {

       switch ( event->message ) {

           case MSG_PLAYATBACKGROUND : {

               clear_event(event);

               set_event(event, EV_MESSAGE, MSG_CLOSE, o);

               cd_track_in_background = 1;

               o->put_event(o, event);

               clear_event(event);

           }; break;

           case MSG_PLAY : {

               clear_event(event);

               if ( !cd_track_pause )

                     bcd_play_to_end(cd_track);

               else bcd_resume();

               cd_track_pause = 0;

           }; break;

           case MSG_PAUSE : {

               clear_event(event);



               if (cd_track_pause == 0)
               {
               bcd_pause();
               cd_track_pause = 1;
               }

               else

               {
               bcd_resume();
               cd_track_pause = 0;
               }

           }; break;

           case MSG_STOP : {

               clear_event(event);

               bcd_stop();

               cd_track_pause = 0;

           }; break;

           case MSG_RR : {

                 clear_event(event);

                 bcd_play_ff_or_re( -400);

           }; break;

           case MSG_FF : {

                 clear_event(event);

                 bcd_play_ff_or_re( 200);

           }; break;

           case MSG_OUT : {

               clear_event(event);

               if ( bcd_device_status() & BCD_DOOR_OPEN ) {

                     bcd_close_door();

                     cd_track_max = bcd_get_audio_info();

               } else

                     bcd_open_door();

               cd_track_pause = 0;
               cd_track = 1;


           }; break;

           case MSG_PREV : {

               clear_event(event);

               cd_track = min(cd_track_max, max(1, cd_track-1));

               bcd_play_to_end(cd_track);

               cd_track_pause = 0;

           }; break;

           case MSG_NEXT : {

               clear_event(event);

               cd_track = min(cd_track_max, max(1, cd_track+1));

               bcd_play_to_end(cd_track);

               cd_track_pause = 0;

           }; break;

           case MSG_RBACK : {

               clear_event(event);

               cd_track_in_background = 1;

           }; break;

           case MSG_TIME : {

               clear_event(event);

               time_type ++;
               if (time_type > 2) time_type = 0;

           }; break;

           case MSG_DRIVE : {

               l_int drive;

               clear_event(event);

               bcd_stop();

               drive = bcd_get_num_drives();

               if (cur_drive_no > drive)
                   bcd_set_drive( cur_drive_no + 1);
               else bcd_set_drive(0);

               cd_track_max = bcd_get_audio_info();

               cd_track_pause = 0;
               cd_track = 1;

               update_titel(o, set_format_text(NULL, TXT_TITEL,  bcd_get_drive_letter( cur_drive_no )));

           }; break;

           case MSG_ABOUT:
              seal2_app_about("CD Player", load_image("/system/bmp/cdplayer.ico,32"));
              clear_event(event);
              break;
       };

   };

};



p_object  get_button ( t_rect r, l_text str, l_text stralt, l_dword msg, l_int flags, l_text i_t) {

   p_button  o = button_init(_malloc(sizeof(t_button)),
                             r,
                             str,
                             msg,
                             flags);

  if ( o )
     {

       VIEW(o)->font = get_font_in_size("Symbols", 14, 14);
       if(!VIEW(o)->font){
        VIEW(o)->font=font_system;
        if(o->caption)_free(o->caption);
        o->caption=(l_text)_strdup(stralt);
       }
       VIEW(o)->info_text = strdup(i_t);
     }
  return OBJECT(o);

};


void  init_cdplayer ( void )
{

   t_rect r = rect_assign(0, 0, 225, 150);
   t_rect t = r;
   p_button button = NULL;

   p_appwin o = appwin_init(_malloc(sizeof(t_appwin)), /* make window */
                            r,
                            "",
                            0x02,
                            ap_id, /* application id */
                            &trans_ev);


   VIEW(o)->align |= TX_ALIGN_CENTER; /* set alignment */

   o->icon16 = load_image("/system/bmp/cdplayer.ico,16");

   o->menu = standard_system_menu_items(o,
                new_menu_line(
                new_menu_item("About CD Player...", 0, 0, MSG_ABOUT, NULL,
   NULL)));

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(o)); /* insert it on the desktop */

   update_titel(OBJECT(o), set_format_text(NULL, TXT_TITEL,  bcd_get_drive_letter( cur_drive_no )));

   VIEW(o)->info_text = strdup(TXT_CDPLAYER);

   r = VIEW(o)->size_limits(VIEW(o));

   r = rect_assign(r.a.x+10, r.a.y+10, r.a.x+210, r.a.y+65);

   OBJECT(o)->insert(OBJECT(o), OBJECT(trackinfo_init(_malloc(sizeof(t_view)), r)));

   //r = rect_assign(r.b.x+10, r.a.y, r.b.x+2, r.b.y);

   r = rect_assign(r.a.x+2, r.b.y+5, r.b.x, r.b.y+55);

   t = r;

   r = rect_assign(r.a.x, r.a.y, r.a.x+64, r.a.y+25); /* play button */

   OBJECT(o)->insert(OBJECT(o), get_button(r, "b","Play", MSG_PLAY, BF_NORMAL, TXT_PLAY)); /* insert button to window ...see get_button function under this function */

   r = rect_assign(r.b.x+4, r.a.y, r.b.x+34, r.a.y+25); /* play at background button */

   OBJECT(o)->insert(OBJECT(o), get_button(r, "M","Quit", MSG_PLAYATBACKGROUND, BF_NORMAL, TXT_PLAYBACKGROUND)); /* insert button to window ...see get_button function under this function */

   r = rect_assign(r.b.x+4, r.a.y, r.b.x+34, r.b.y); /* pause button */
   OBJECT(o)->insert(OBJECT(o), get_button(r, "a","||", MSG_PAUSE, BF_NORMAL, TXT_PAUSE)); /* insert button to window ...see get_button function under this function */

   r = rect_assign(r.b.x+4, r.a.y, r.b.x+34, r.b.y); /* stop button */
   OBJECT(o)->insert(OBJECT(o), get_button(r, "W","Stop", MSG_STOP, BF_NORMAL, TXT_STOP)); /* insert button to window ...see get_button function under this function */

   r = rect_assign(t.a.x, r.b.y+5, t.a.x+30, r.b.y+30); /* |<< button */
   OBJECT(o)->insert(OBJECT(o), get_button(r, "Z","|<<", MSG_PREV, BF_NORMAL, TXT_LAST)); /* insert button to window ...see get_button function under this function */

   r = rect_assign(r.b.x+4, r.a.y, r.b.x+34, r.b.y); /* << button */
   OBJECT(o)->insert(OBJECT(o), get_button(r, "X","<<", MSG_RR, BF_NORMAL, TXT_REWIND)); /* insert button to window ...see get_button function under this function */

   r = rect_assign(r.b.x+4, r.a.y, r.b.x+34, r.b.y); /* >> button */
   OBJECT(o)->insert(OBJECT(o), get_button(r, "Y",">>", MSG_FF, BF_NORMAL, TXT_FORWARD)); /* insert button to window ...see get_button function under this function */

   r = rect_assign(r.b.x+4, r.a.y, r.b.x+34, r.b.y); /* >>| button */
   OBJECT(o)->insert(OBJECT(o), get_button(r, "c",">>|", MSG_NEXT, BF_NORMAL, TXT_NEXT)); /* insert button to window ...see get_button function under this function */

   r = rect_assign(r.b.x+4, r.a.y, r.b.x+34, r.b.y); /* /\ button */
   OBJECT(o)->insert(OBJECT(o), get_button(r, "d","Eject", MSG_OUT, BF_NORMAL, TXT_OPEN_CLOSE)); /* insert button to window ...see get_button function under this function */

   r = rect_assign(r.b.x+4, r.a.y, r.b.x+34, r.b.y); /* time button */
   button = BUTTON(get_button(r, "T","T", MSG_TIME, BF_NORMAL, TXT_TIME));
   VIEW(button)->font = get_font_in_size("ActivaCE", 14, 14);
   if(!VIEW(button)->font){
    VIEW(button)->font=font_system;
    if(button->caption)_free(button->caption);
    button->caption=(l_text)_strdup("Time");
   }
   OBJECT(o)->insert(OBJECT(o), OBJECT(button)); /* insert button to window ...see get_button function under this function */

   r = rect_assign(r.a.x, r.a.y - 30, r.b.x, r.a.y - 5); /* drive button */
   button = BUTTON(get_button(r, "D","D", MSG_DRIVE, BF_NORMAL, TXT_DRIVE));
   VIEW(button)->font = get_font_in_size("ActivaCE", 14, 14);
   if(!VIEW(button)->font){
    VIEW(button)->font=font_system;
    if(button->caption)_free(button->caption);
    button->caption=(l_text)_strdup("Drive");
   }
   OBJECT(o)->insert(OBJECT(o), OBJECT(button)); /* insert button to window ...see get_button function under this function */
};


static l_int get_track ( l_text args )
{
  if ( args ) {

     l_text x = args;
     l_text s;
     l_int  i = 0;

     while ( *x && ( (*x < '0') || (*x > '9') ) ) x++;
     args = x;
     while ( *x && (*x >= '0') && (*x <= '9') ) x++;

     s = stridup(args, strsize(args, x));

     if ( s ) i = atol(s);

     _free(s);

     return i;

  };

  return 0;
};


app_begin ( void ) {


  if ( ap_process == AP_INIT ) {

    l_int err = 0;
    l_int i   = 0;

    AP_SETNUMOFCALLS(1); /* set max num of calls */

    bcd_open();

    err = bcd_audio_busy();

    if ( !err ) { /* audio is not play, ok ! */

       bcd_reset();

    } else if ( err == -1 ) /* audio error */

         seal_error(ERR_INFO, "%s !", TXT_ERRORINLOADINGAUDIO);

    else /* audio is play now */

      cd_track = bcd_now_playing();

    cd_track_max = bcd_get_audio_info();

    init_cdplayer();

    i = get_track(ap_args);

    if ( i > 0 ) {

         cd_track = min(cd_track_max, max(1, i));

         bcd_play_to_end(cd_track);

    };

  };

  if ( ap_process == AP_FREE ) {

     if ( !cd_track_in_background ) {

         bcd_stop();

         bcd_close();

     };

  };

} app_end;

