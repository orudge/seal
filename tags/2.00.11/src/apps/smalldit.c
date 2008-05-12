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
 * 04/04/2002: Integrated with Tobias's changes (orudge)
 */

#include <seal.h>
#include <menus.h>
#include <app.h>
#include <button.h>
#include <dialogs.h>
#include <editor.h>
#include <iodlg.h>

/* text declarations */

#ifndef TXT_SAVE
#define TXT_SAVE     INI_TEXT("Save")
#endif
#ifndef TXT_SAVEAS
#define TXT_SAVEAS   INI_TEXT("Save as...")
#endif
#ifndef TXT_QUIT
#define TXT_QUIT     INI_TEXT("Quit")
#endif
#ifndef TXT_NONAME
#define TXT_NONAME   INI_TEXT("Noname")
#endif
#ifndef TXT_CANNOTSAVETHEFILE
#define TXT_CANNOTSAVETHEFILE  INI_TEXT("Cannot save the file")
#endif
#define TXT_SD0001   INI_TEXT("File have been edited.\n Are you sure you want to close it ?")

/* messages */

#define MSG_NEW      100001
#define MSG_OPEN     100002
#define MSG_SAVE     100003
#define MSG_SAVEAS   100004
#define MSG_PRINT    100005

#define MSG_ED_CUT    100006
#define MSG_ED_COPY   100007
#define MSG_ED_PASTE  100008
#define MSG_ED_DELETE 100009


//#define MSG_ABOUT    900001

static l_text   current_path = NULL;
static l_text   current_file = NULL;

static p_editor editor = NULL;
static p_appwin edit = NULL;
static p_menuview menu = NULL;


static p_list get_file_items ( void ) {

                                                /* function for free-ing items */
  p_list p = list_init(malloc(sizeof(t_list)), &free_filehistory_item, 0);

  if ( p ) {

     p->insert(p, new_filehistory_item("DOS ASCII files (*.txt)", "*.txt"));
     p->insert(p, new_filehistory_item("All Files (*.*)", "*.*"));

  };


  return p;

};


static p_menu  smalldit_menu_init ( void )
{
  p_menu menu = new_menu(

                  new_sub_menu(TXT_FILE, new_menu(

                     new_menu_item(TXT_NEW, "", 0, MSG_NEW, NULL,
                     new_menu_item(TXT_OPEN, "", 0, MSG_OPEN, NULL,
                     new_menu_item(TXT_SAVE, "", 0, MSG_SAVE, NULL,
                     new_menu_item(TXT_SAVE_AS, "", 0, MSG_SAVEAS, NULL,
                     new_menu_line(
                     new_menu_item(TXT_EXIT, "ALT+F4", TO_ALT(KB_F4), MSG_CLOSE, NULL,

                     NULL))))))),

                  new_sub_menu(TXT_EDIT, new_menu(

                     new_menu_item(TXT_COPY, "", 0, MSG_ED_COPY, NULL,
                     new_menu_item(TXT_CUT, "", 0, MSG_ED_CUT, NULL,
                     new_menu_item(TXT_PASTE, "", 0, MSG_ED_PASTE, NULL,
                     new_menu_item(TXT_DELETE, "DEL", 0, MSG_ED_DELETE, NULL,
                     new_menu_line(
                     new_menu_item("Select All", "", 0, MSG_SELECT_ALL, NULL,
                     NULL))))))),

               NULL)));

   return menu;

};


void    redraw_title ( void )
{

  if ( edit ) {

      _free(WINDOW(edit)->caption);

      if ( current_file )

         WINDOW(edit)->caption = set_format_text(NULL, "SmallDit - %s", current_file);

      else

         WINDOW(edit)->caption = set_format_text(NULL, "SmallDit - %s", TXT_NONAME);

      TEST_SUB_VIEWS(VIEW(edit), WINDOW(edit)->draw_title(WINDOW(edit)));

  };

};


l_bool  ask_close ( void ) {

  if  ( editor->flags & EF_HAVEBEENEDITED ) {
	 return (l_bool)(msgbox(MW_INFO, MB_YES|MB_NO, TXT_SD0001) != MSG_NO);

  } else return true;

};


static void trans_ev ( p_object o, p_event event )
{

   if ( event->type == EV_MESSAGE ) {

       switch ( event->message ) {

           case MSG_CLOSE : { /* close text */

               t_event oldev = *event;

               if ( o->phase == PH_PREPROCESS );

                   if ( ask_close() )

                      *event = oldev; /* recover events */

                   else

                      clear_event(event);

           }; break;

           case MSG_NEW : { /* make new text */

                 clear_event(event);

                 if ( ask_close() ) { /* some of files was choosen and ok in warning dialog */

                     afree((void**)(&current_path));
                     afree((void**)(&current_file));

                     editor->set_text(editor, NULL); /* clear text in t_editor object */

                     redraw_title();

                 };

           }; break;

           case MSG_PRINT : { /* print */

                 clear_event(event);

           }; break;

           case MSG_OPEN : { /* open new file */

                 l_text file;

                 clear_event(event);

//                 current_path = get_key("current/seal/smalldit/last_open_dir");

                 file = open_dialog(current_path, "*.*", get_file_items());


                 if ( file && ask_close() ) { /* some of files was choosen and ok in warning dialog */

                     l_text t = file_gettext(file); /* get text from file */
                     l_text x = io_path(file); /* get only path from sf (file) */

                     _free(current_file);

                     current_file = _strdup(file);

                     _free(current_path);

                     current_path = x;

//                     set_key("current/seal/smalldit/last_open_dir", x); /* set last opened path */

                     editor->set_text(editor, t); /* set this text to t_editor object */

                      redraw_title();

                     _free(t); /* free memory ( t_editor saved it ) */

                 };

                 _free(file);

                 clear_event(event);

           }; break;

           case MSG_SAVE : { /* save current text to specific file */

                 clear_event(event);

                 if ( !current_file ) {
//                    current_path = get_key("current/seal/smalldit/last_save_dir");
                    current_file = save_dialog(current_path, "*.*", get_file_items());
                 }

                 if ( current_file )

                    if ( !file_puttext(current_file, editor->text) ) /* some error occured */

                        seal_error(ERR_INFO, "%s %s", TXT_CANNOTSAVETHEFILE, current_file);

                    else {

                        _free(current_path);

                        current_path = io_path(current_file);

//                        set_key("current/seal/smalldit/last_save_dir", current_path);

                        redraw_title();

                        RETYPE_FILES_IN_PATH(current_path);

                        set_notedited(editor);

                    };

           }; break;

           case MSG_SAVEAS : { /* save current text to another file */

                 l_text sf;

                 clear_event(event);

//                 current_path = get_key("current/seal/smalldit/last_save_dir");
                 sf = save_dialog(current_path, "*.*", get_file_items());

                 if ( sf )

                    if ( !file_puttext(sf, editor->text) ) /* some error occured */

                        seal_error(ERR_INFO, "%s %s", TXT_CANNOTSAVETHEFILE, current_file);

                    else {  /* all is ok */

                        _free(current_file);
                        _free(current_path);

                        current_file = _strdup(sf);

                        current_path = io_path(sf);

                        redraw_title();

                        RETYPE_FILES_IN_PATH(current_path);

                        set_notedited(editor);

                    };

                 //_free(sf);

           }; break;

           case MSG_ABOUT : {
              BITMAP *ico = load_image("bmp/smalldit.ico,32");
              seal2_app_about("Text Editor", ico);
              destroy_bitmap(ico);
//              msgbox(MW_INFO, MB_OK, "SmallDit 1.1 by Michal Stencl and Seal System");
              clear_event(event);
           }; break;

           case MSG_ED_CUT : {

              clipboard.style = DS_SELECTED;
              OBJECT(editor)->get_data(OBJECT(editor), &clipboard);
              editor->redraw_text(editor, editor->line, editor->line_pos, TO_KEY(KB_DEL));
              clear_event(event);

           }; break;

           case MSG_ED_COPY : {

              clipboard.style = DS_SELECTED;
              OBJECT(editor)->get_data(OBJECT(editor), &clipboard);
              clear_event(event);

           }; break;

           case MSG_ED_PASTE : {
              clipboard.style = DS_SELECTED;
              OBJECT(editor)->set_data(OBJECT(editor), &clipboard);
              clear_event(event);

           }; break;

           case MSG_ED_DELETE : {

              editor->redraw_text(editor, editor->line, editor->line_pos, TO_KEY(KB_DEL));
              clear_event(event);

           }; break;
       };
   };
};



static void new_menu_trans_event(p_object o, p_event event) {

   // This function checks to see if something is selected
   // and in that case enables the Cut and Copy menu items.
   // Also enables Paste if clipboard isn't empty.
 
   p_menuitem  curItem;

   if (event->type & EV_MOUSE) {

      curItem = menuview_index_of(menu, 1);
      curItem = curItem->submenu->items;

      if (ed_is_sel(EDITOR(editor))) {   // Enable cut?
         curItem->enable = 1;
      } else {
         curItem->enable = 0;
      };

      curItem = curItem->next;

      if (ed_is_sel(EDITOR(editor))) {   // Enable copy?
         curItem->enable = 1;
      } else {
         curItem->enable = 0;
      };

      curItem = curItem->next;

      if (clipboard.data&&l_tag_cmp(clipboard.id, DAT_TEXT)) {   // Enable paste?
         curItem->enable = 1;
      } else {
         curItem->enable = 0;
      };
   };
   menuview_translate_event(o, event);
};



static t_point smalldit_size_minimum ( p_view o )
{

   return point_assign(200, 150);

};


void  init_smalldit ( void )
{

   t_rect r = rect_assign(0, 100, 540, 400);
   t_rect s;

   edit = appwin_init(_malloc(sizeof(t_appwin)),  /* make window */
                               r,
                               _strdup("SmallDit"),
                               WF_MINIMIZE|WF_MAXIMIZE|WF_ABOUT,
                               ap_id,
                               &trans_ev);

   edit->icon16 = load_image("bmp/smalldit.ico,16");

   VIEW(edit)->drag_mode |= DM_DRAGGROW; /* able to grow view */

   VIEW(edit)->size_minimum = &smalldit_size_minimum;

   edit->menu = standard_system_menu_items(edit,
                new_menu_line(
                new_menu_item("About SmallDit...", 0, 0, MSG_ABOUT, NULL,
   NULL)));

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(edit));

   s = r = VIEW(edit)->size_limits(VIEW(edit));

   r = rect_assign(r.a.x, r.a.y+1, r.a.x, r.a.y+20);

   menu = hormenu_init(_malloc(sizeof(t_menuview)), /* make menu bar */
                       r,
                       smalldit_menu_init());

   OBJECT(edit)->insert(OBJECT(edit), OBJECT(menu));

   OBJECT(menu)->translate_event = &new_menu_trans_event;


   r = rect_assign(r.a.x+1, r.b.y+7, s.b.x-STANDARD_SCROLLBAR_WIDTH, s.b.y-STANDARD_SCROLLBAR_WIDTH);

   editor = editor_init(_malloc(sizeof(t_editor)), /* make editor */
                        r,
                        0);

   VIEW(editor)->align |= TX_ALIGN_BOTTOM+TX_ALIGN_RIGHT;

   OBJECT(edit)->insert(OBJECT(edit), OBJECT(editor));

};

SetInfoAppName("SmallDit");
SetInfoDesciption("A text editor for SEAL");
SetInfoCopyright("(c) Copyright 1999 Michal Stencl, 2001 Seal System. All rights reserved");
SetInfoManufacturer("Michal Stencl, Seal System");

app_begin ( void ) {


  if ( ap_process == AP_INIT ) {

      current_path = _strdup("/");
      current_file = NULL;

      init_smalldit();

      if ( ap_args ) {
        l_text t = file_gettext(ap_args); /* get text from file */
        if ( t ) {
          editor->set_text(editor, t); /* set this text to t_editor object */

          current_path = io_path(ap_args);
          current_file = _strdup(ap_args);

          redraw_title();

          _free(t); /* free memory ( t_editor saved it ) */
        };
      } else redraw_title();
  };

  if ( ap_process == AP_DONE ) {

      _free(current_path);
      _free(current_file);

  };

} app_end;

