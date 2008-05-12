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
#include <app.h>
#include <dialogs.h>
#include <registry.h>
#include <iodlg.h>

#define MSG_RUN 10001
#define MSG_BROWSE 10003

p_textline tl = NULL;

static p_list get_file_items()
{
  p_list p = list_init(malloc(sizeof(t_list)), &free_filehistory_item, 0);

  if (p) {
/*     p_registry_search inf = (p_registry_search) malloc(sizeof(t_registry_search));
     if (reg_find_first("system/filetypes", inf)) do {
       l_text title = get_key(inf->name);
       l_text ext = (l_text) malloc(strlen(inf->key.name)+3);
       strcpy(ext, "*.");
       strcat(ext, inf->key.name);

       p->insert(p, new_filehistory_item(title, ext));

       free(ext);
       free(title);
     } while (reg_find_next(inf));
     free(inf);*/

     p->insert(p, new_filehistory_item("All Files (*.*)", "*.*"));
  };

  return p;
}

static void handle_event(p_object o, p_event event)
{
  if (event->type == EV_MESSAGE) {
    switch (event->message) {
      case MSG_RUN: {
        clear_event(event);
        if (tl->text && strcmp(tl->text, "")) {
          l_text runfile;
          l_bool ok;
          if (!strstr(tl->text, ".")) {
            runfile = (l_text) malloc(strlen(tl->text)+5);
            strcpy(runfile, tl->text);
            strcat(runfile, ".app");
          } else runfile = strdup(tl->text);
          ok = run_file(runfile);
          free(runfile);
          if (ok) set_event(event, EV_MESSAGE, MSG_CLOSE, o);
        };
      } break;
      case MSG_CANCEL: {
      } break;
      case MSG_BROWSE: {
        l_text file = open_dialog("/", "*.s2a", get_file_items());
        if (file) {
          if (tl->text) free(tl->text);
          tl->set_text(tl, file);
          free(file);
        };
        clear_event(event);
      } break;
    };
  };
}

void app_init()
{
  t_rect r = rect_assign(0, 0, 300, 125);
  p_appwin w;
  p_stattext t = NULL;
  p_button b = NULL;
  t_rect rr = desktop->get_local_extent(desktop);

  r = rect_move(r, 0, rect_sizey(rr)-r.b.y-28);

  w = appwin_init(malloc(sizeof(t_appwin)), r, "Run", 0, ap_id, &handle_event);
  WINDOW(w)->flags &= ~WF_SYSMENU;
  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(w));

  r = rect_assign(10, 35, 290, 55);
  t = stattext_init(malloc(sizeof(t_stattext)), r, TX_ALIGN_LEFT, "Filename:");
  OBJECT(w)->insert(OBJECT(w), OBJECT(t));

  r = rect_assign(10, 56, 290, 75);
  tl = textline_init(malloc(sizeof(t_textline)), r, 1024, 0);
  OBJECT(w)->insert(OBJECT(w), OBJECT(tl));

  r = rect_assign(10, 90, 100, 115);
  b = button_init(malloc(sizeof(t_button)), r, "Run", MSG_RUN, BF_DEFAULT);
  OBJECT(w)->insert(OBJECT(w), OBJECT(b));

  r = rect_assign(104, 90, 194, 115);
  b = button_init(malloc(sizeof(t_button)), r, "Cancel", MSG_CLOSE, BF_NORMAL);
  OBJECT(w)->insert(OBJECT(w), OBJECT(b));

  r = rect_assign(198, 90, 290, 115);
  b = button_init(malloc(sizeof(t_button)), r, "Browse...", MSG_BROWSE, BF_NORMAL);
  OBJECT(w)->insert(OBJECT(w), OBJECT(b));
}

SetInfoAppName("Run");
SetInfoDesciption("Run command program");
SetInfoCopyright("(c) Copyright 2000,2001 BadSector. All rights reserved");
SetInfoManufacturer("BadSector");

app_begin(void)
{
  if (ap_process == AP_INIT) {
    app_init();
  };
} app_end;

