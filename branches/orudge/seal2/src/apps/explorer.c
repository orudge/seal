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
 */

#include <seal.h>
#include <app.h>
#include <button.h>
#include <treeview.h>
#include <dialogs.h>
#include <iodlg.h>

p_treeview tree = NULL;
p_filelistbox dir = NULL;
p_appwin win = NULL;

static void trans_ev ( p_object o, p_event event ){

  if ( o->process == PH_PREPROCESS && event->type & EV_MESSAGE ) {

       switch ( event->message ) {
           case MSG_UPDATE : {
             if ( tree && tree->selected ) {
               l_text path = _strdup(tree->selected->name);

             dir->load_list(dir, path, dir->file);

               clear_event(event);
             };
           }; break;
           case MSG_ABOUT:
           {
              BITMAP *ico = load_image("bmp/dir.ico,32");
              seal2_app_about("Explorer", ico);
              destroy_bitmap(ico);

              clear_event(event);
              break;
           }
       };
    };
 };

void  dcfilelistbox_translate_event ( p_object o, p_event event )
{

   RETVIEW(o, event);

   filelistbox_translate_event(o, event);


   if ( event->type & EV_MESSAGE ) {

       switch ( event->message ) {

           case MSG_OK : {

                 l_text file = FILELISTBOX_ITEM_FILENAME(LISTBOX(o)->list, LISTBOX(o)->current);
                 l_text path = FILELISTBOX_ITEM_PATH(LISTBOX(o)->list, LISTBOX(o)->current);

                 l_text runfile = io_realpath(path, file);

                 clear_event(event);

                 if ( !run_file (runfile) )

                     seal_error(ERR_INFO, "%s\n\n%s", TXT_CANTRUNFILE, runfile);

                 _free(runfile);

           }; break;

       };

   };

};


void init_window ( void )
{
   p_treeitem it;

   t_rect   r = rect_assign(0, 0, 600, 300);

   win = appwin_init(_malloc(sizeof(t_appwin)),
                            r,
                            "Seal Explorer",
                            WF_MINIMIZE|WF_MAXIMIZE,
                            ap_id,
                            &trans_ev);


   win->menu = standard_system_menu_items(win,
                new_menu_line(
                new_menu_item("About Explorer...", 0, 0, MSG_ABOUT, NULL,
   NULL)));


   if ( win ) VIEW(win)->align |= TX_ALIGN_CENTER;

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(win));

      if ( win ) {


   tree = treeview_init(_malloc(sizeof(t_treeview)), rect_assign(5, 20, (r.b.x / 2) - 20 , r.b.y - 5));

   VIEW(tree)->align |= TX_ALIGN_BOTTOM;

   OBJECT(win)->insert(OBJECT(win), OBJECT(tree));

   dir = filelistbox_init(_malloc(sizeof(t_filelistbox)), rect_assign(r.b.x / 2, 20, r.b.x - 5 , r.b.y - 20), 2, LF_SELECTABLE|FL_OPEN, _strdup("/"), _strdup("*.*"), FA_ALL, NULL);

   VIEW(dir)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_RIGHT;

   OBJECT(win)->insert(OBJECT(win), OBJECT(dir));
   OBJECT(dir)->translate_event = &dcfilelistbox_translate_event;

   tree->add(tree,_strdup("root"),_strdup("/"),_strdup(TXT_COMPUTER),IMG_DIR16,true,NULL);

   tree->load_from_dir(tree,_strdup("/"),_strdup("/"));

      };

};

SetInfoAppName("Explorer (to finish)");
SetInfoDesciption("DC with a treeview to test treeview !");
SetInfoCopyright("(c) Copyright 2001 Seal System. All rights reserved");
SetInfoManufacturer("Seal System");

app_begin ( void ) {


  if ( ap_process == AP_INIT ) { /* When ap start */

      AP_SETNUMOFCALLS(1); /* Set MAX of Calls */

      init_window(); /* Run the init void */

  };

  if ( ap_process == AP_FREE ) {   /*   */

  };
  if ( ap_process == AP_DONE ) {  /* When ap done */

  };
} app_end;

