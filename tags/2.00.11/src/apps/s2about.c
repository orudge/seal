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

/***************************************/
/* SEAL 2.0 About application          */
/* Version 0.11                        */
/*                                     */
/* Copyright (c) Owen Rudge 2001-2002  */
/***************************************/

/* Revision History:

  06/10/2001 - Created this
  ../10/2001 - Various updates
  26/10/2001 - Small updates in text and other behaviour
  19/01/2002 - Updated copyright, etc
  03/03/2002 - Updated version info line
  29/03/2002 - Updated for SEAL 2.00.11
  02/04/2002 - Updated again for SEAL 2.00.11
  03/04/2002 - And again... (added Phil Harper for startup logo)
*/

// 8,140

#include <allegro.h>
#include <seal.h>
#include <app.h>
#include <button.h>
#include <menus.h>
#include <grfx.h>

#include "s2about.h"

SetInfoAppName("SEAL 2.0 About dialog");
SetInfoDesciption("SEAL 2.0 About dialog");
SetInfoCopyright("Copyright (c) Owen Rudge 2001-2002. All Rights Reserved.");
SetInfoManufacturer("Owen Rudge");

p_appwin main;
p_grfx scr;

BITMAP *bglogo;
BITMAP *todraw;
BITMAP *allscroll;

DATAFILE *dat;

MIDI *bgmusic;

FONT *the_font;

int current_y;

//#define SCROLL_LEN     60  // unused now, it seems
#define SCROLL_ITEMS   90 // including UNDERLINEs, etc

#define UNDERLINE  "=u"

char *scroller[] = {"SEAL 2.0",
"",
"=v", // version
"",
"Copyright \xA9 SEAL Developers 2001-2002",  /// A9 = 169 = copyright symbol
"",
"All Rights Reserved.",
"",
"PROGRAMMING",
"",
"Julien Etelain <julien@pmad.net>",
UNDERLINE,
"Pretty much everything not written by",
"someone else.",
"",
"Owen Rudge <seal@orudge.freeuk.com>",
UNDERLINE,
"SFA, SealDOS, this program, many patches",
"and loads more.",
"",
"Michal Stencl <stenclm@interactivestudio.sk>",
UNDERLINE,
"Older kernel code, etc. Started the SEAL",
"project.",
"",
"Kostas Michalopoulos <michalopoylos@hotmail.com>",
UNDERLINE,
"Help System, Bad Controls, Volume Control,",
"Snake, old registry, plus other contributions.",
"",
"Jon Heather <jheather@zoo.co.uk>",
UNDERLINE,
"Many optimisations and bug fixes.",
"",
"Florian Xaver <florianx@drdos.org>",
UNDERLINE,
"SFA plus other updates.",
"",
"Pablo Borobia <p43lok@hotmail.com>",
UNDERLINE,
"Tooltips, drag-and-drop updates, \"UNDERSEL\"",
"and splitter control.",
"",
"Callum Lawson <callum@pmad.net>",
UNDERLINE,
"Tetris game, plus other coding.",
"",
"Tobias Johansson <tobbe_snodd@hotmail.com>",
UNDERLINE,
"Cruel, Fifteen, Klondike, Solitaire, Golf and",
"Towers of Hanoi games.",
"",
"Alex Levitsky <sealru@mailru.com>",
UNDERLINE,
"Disk Imager.",
"",
"Phil Harper <phil@vmlinuz.freeserve.co.uk>",
UNDERLINE,
"Startup logo, plus Qube skin.",
"",
"If you think your name should be listed and it",
"isn't, e-mail orudge@users.sourceforge.net.",
"",
"",
"Thank you for downloading and using SEAL. If you",
"have any comments or queries, please contact us!",
"The SEAL forums are a place where you can discuss",
"and get help with SEAL - see http://sealsystem.sf.net/",
"for more information.",
"",
"",
"LICENSE INFORMATION",
UNDERLINE,
"This program is free software; you can redistribute",
"it and/or modify it under the terms of the GNU",
"General Public License as published by the Free",
"Software Foundation; either version 2 of the License,",
"or (at your option) any later version.",
"",
"This program is distributed in the hope that it will",
"be useful, but WITHOUT ANY WARRANTY; without even",
"the implied warranty of MERCHANTABILITY or FITNESS",
"FOR A PARTICULAR PURPOSE.  See the GNU General",
"Public License for more details.",
"",
"You should have received a copy of the GNU General",
"Public License along with this program; if not, write",
"to the Free Software Foundation, Inc., 675 Mass Ave,",
"Cambridge, MA 02139, USA."
};  // 51 lines


void main_draw(p_view o)
{
   t_rect r = o->get_local_extent(o);
   t_point p;

   BITMAP *out = o->begin_paint(o, &p, r);

   if (out)
   {
      blit(bglogo, out, 0, 0, p.x, p.y, 320, 240);
      masked_blit(todraw, out, 0, 0, p.x+7, p.y+142, 305, 90);
   }

   o->end_of_paint(o, r);
}

void timer(p_object o)
{
  p_view vo = VIEW(main);
  l_int i;

 // if (!o->is_state(o, OB_SF_FOCUSED)) return;

  clear_to_color(todraw, makecol(255,0,255));
  current_y += 2;

  if (current_y > (text_height(the_font)*SCROLL_ITEMS)+90)
     current_y = -90;

  masked_blit(allscroll, todraw, 0, current_y, 0, 0, 305, 90);

  VIEW(o)->draw_me(VIEW(o));
}

void app_init()
{
   t_rect r = rect_assign(0, 0, 322, 259);
   l_int x, y;

//   main = appwin_init(malloc(sizeof(t_appwin)), r, set_format_text(NULL,"%s %s %s",TXT_ABOUT,SealApi,SealApiTxtVersion)/*"About SEAL 2.0"*/, WF_MINIMIZE, ap_id, NULL);
   main = appwin_init(malloc(sizeof(t_appwin)), r, "About SEAL 2.0", WF_MINIMIZE, ap_id, NULL);
   main->icon16 = (BITMAP*)GET_DATA(dat, WINDICON);

   VIEW(main)->align |= TX_ALIGN_CENTER;
   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(main));

   init_stillprocess(OBJECT(main), 150);

   r = rect_assign(2, 19, 320, 259); // 2, 19
   scr = grfx_init(malloc(sizeof(t_grfx)), r);
   OBJECT(scr)->func_callback = &timer;
   VIEW(scr)->draw = &main_draw;

   OBJECT(main)->insert(OBJECT(main), OBJECT(scr));
   init_stillprocess(OBJECT(scr), 150);

   play_midi(dat[BGMUSIC].dat, TRUE);
   current_y = -90;
}

app_begin(void)
{
   int i;

   if (ap_process == AP_INIT)
   {
      AP_SETNUMOFCALLS(1);
      dat = conv_to_skipcolor_data(DLXGetFileData(ap_id), CO_SKIP_RED, CO_SKIP_GREEN, CO_SKIP_BLUE);

      the_font = dat[TEXTFONT].dat; //get_font_in_size("Arial", 8, 8);

//      bglogo = load_image("s2about.bmp");
      bglogo = (BITMAP *) dat[BACKGROUND].dat;

      todraw = create_bitmap(305, 90);
      allscroll = create_bitmap(305, text_height(the_font)*SCROLL_ITEMS);
//      seal_error(ERR_INFO, "Bitmap Dimensions: %d x %d", 305, text_height(the_font)*SCROLL_LEN);

      clear_to_color(todraw, makecol(255,0,255));
      clear_to_color(allscroll, makecol(255,0,255));

      text_mode(-1);

      for (i = 0; i < SCROLL_ITEMS; i++)
      {
         if (scroller[i][0] == '=' && scroller[i][1] == 'u' && !scroller[i][2] )  // Underline previous item
         {
            line(allscroll, (305/2)-text_length(the_font, scroller[i-1])/2, i*text_height(the_font) - 1, (305/2)+text_length(the_font, scroller[i-1])/2, i*text_height(the_font) - 1,
               makecol(0,0,0));
         }
         else if (scroller[i][0] == '=' && scroller[i][1] == 'v' && !scroller[i][2] )  // Version
         {
            char ver[20];
            sprintf(ver, "Version %s", SealApiTxtVersion);
            textout_centre(allscroll, the_font, ver, 305/2, i*text_height(the_font), makecol(0,0,0));
         }
         else
            textout_centre(allscroll, the_font, scroller[i], 305/2, i*text_height(the_font), makecol(0,0,0));
      }

      app_init();
   }
   else if (ap_process == AP_DONE)
   {
      stop_midi();

//      destroy_bitmap(bglogo);
      destroy_bitmap(todraw);
      destroy_bitmap(allscroll);

//      destroy_midi(bgmusic);
//      unload_datafile(dat);
   }
} app_end;
