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
 * 30/03/2002 - Added About dialog (orudge)
 * 02/04/2002 - Fixed small icon problem (changed help.bmp to help.ico,16) (orudge)
 */

#include <allegro.h>
#include <seal.h>
#include <app.h>
#include <dialogs.h>
#include <iodlg.h>
#include <helpsys.h>

#define MSG_CONTENTS    10001
#define MSG_GOBACK      10002

p_appwin main = NULL;
p_helpbox help = NULL;
l_text filename = NULL;
l_text topic = NULL;

static void trans_ev(p_object o, p_event event)
{
  if (event->type == EV_MESSAGE) {
    switch (event->message) {
      case MSG_CONTENTS: {
        help->navigate(help, "main");
        clear_event(event);
      }; break;
      case MSG_GOBACK: {
        help->go_back(help);
        clear_event(event);
      }; break;
      case MSG_ABOUT:
      {
         BITMAP *ico = load_image("bmp/help.ico,32");
         seal2_app_about("Help", ico);
         destroy_bitmap(ico);

         clear_event(event);
         break;
      }
    };
  };
}

l_bool on_request(p_helpbox o, l_int request, void *data1, void *data2, void **rdata1)
{
  switch (request) {
    case HBREQ_BUTTON: {
//      DEBUG_printf("\nAdd button %s", (l_text) data1);
      return TRUE;
    }; break;
  };

  return FALSE;
}

void app_init()
{
  p_button b = NULL;
  t_rect r = rect_assign(0, 0, 550, 430);
  BITMAP *ico16;
  main = appwin_init(malloc(sizeof(t_appwin)), r, "SEAL Help System", WF_MINIMIZE|WF_MAXIMIZE, ap_id, &trans_ev);

  main->menu = standard_system_menu_items(main,
                new_menu_line(
                new_menu_item("About Help...", 0, 0, MSG_ABOUT, NULL,
   NULL)));

  ico16 = load_image("bmp/help.ico,16");
//  #ifdef Bad_Seal
  main->icon16 = ico16;
//  #endif
  VIEW(main)->align |= TX_ALIGN_CENTER;
  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(main));

  r = rect_assign(3, 48, 547, 427);
  help = helpbox_init(malloc(sizeof(t_helpbox)), r);
  help->request = &on_request;
  VIEW(help)->align |= TX_ALIGN_BOTTOM+TX_ALIGN_RIGHT;
  OBJECT(main)->insert(OBJECT(main), OBJECT(help));

  if (filename) {
    help->filename = filename;
    helpbox_navigate(help, topic);
  };

  r = rect_assign(3, 20, 103, 45);
  b = button_init(malloc(sizeof(t_button)), r, "Contents", MSG_CONTENTS, 0);
  OBJECT(main)->insert(OBJECT(main), OBJECT(b));

  r = rect_assign(106, 20, 206, 45);
  b = button_init(malloc(sizeof(t_button)), r, "Go Back", MSG_GOBACK, 0);
  OBJECT(main)->insert(OBJECT(main), OBJECT(b));
}

SetInfoAppName("HELP Viewer");
SetInfoDesciption("SEAL 2 Help viewer");
SetInfoCopyright("(c) 2000-2001 Kostas Michalopoulos aka Bad Sector");
SetInfoManufacturer("Bad Sector");

app_begin(void)
{
  if (ap_process == AP_INIT) {
    if (ap_args) {
      filename = strdup(ap_args);
      if (!strchr(filename, ' ')) topic = strdup("main"); else {
        topic = strchr(filename, ' ')+1;
        filename[strlen(filename)-strlen(topic)-1]=0;
      };
    };
    app_init();
  };
} app_end;

