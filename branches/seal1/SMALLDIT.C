/****************************************************************/
/*                                                              */
/*                           smalldit.c                         */
/*                                                              */
/*                       text editor for Seal                   */
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
#include"editor.h"
#include"iodlg.h"
#include <dos.h>
#include <time.h>

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
#define TXT_SD0001   INI_TEXT("Are you sure you want to close current file ?\nIf you haven't saved your data yet,\nplease save them, otherwise you will LOSE them")

/* messages */

#define MSG_NEW      100001
#define MSG_OPEN     100002
#define MSG_SAVE     100003
#define MSG_SAVEAS   100004
#define MSG_PRINT    100005

static l_text   current_path = NULL;
static l_text   current_file = NULL;

static p_editor editor = NULL;
static p_appwin edit = NULL;


static p_list get_file_items ( void ) {

                                                /* function for free-ing items */
  p_list p = list_init(malloc(sizeof(t_list)), &free_filehistory_item, 0);

  if ( p ) {

     p->insert(p, new_filehistory_item("DOS ASCII files (*.txt)", "*.txt"));
     p->insert(p, new_filehistory_item("All files (*.*)", "*.*"));

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
                     new_menu_item(TXT_SAVEAS, "", 0, MSG_SAVEAS, NULL,
                     new_menu_line(
                     new_menu_item(TXT_QUIT, "ALT+F4", TO_ALT(KB_F4), MSG_CLOSE, NULL,

                     NULL))))))),

               NULL));

   return menu;

};


void    redraw_title ( void )
{

  if ( edit ) {

      _free(WINDOW(edit)->caption);

      if ( current_file )

         WINDOW(edit)->caption = set_format_text(NULL, "SmallDit - %s", get_filename(current_file));

      else

         WINDOW(edit)->caption = set_format_text(NULL, "SmallDit - %s", TXT_NONAME);

      TEST_SUB_VIEWS(VIEW(edit), WINDOW(edit)->draw_title(WINDOW(edit)));

  };

};


l_bool  ask_close ( void )
{

   return (l_bool)(msgbox(MW_INFO, MB_YES|MB_NO, TXT_SD0001) != MSG_NO);

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
                 l_text sf;

                 clear_event(event);

                 file = open_dialog(current_path, "*.*", get_file_items());

                 sf = io_sealfile(file); /* if link, so transform it */

                 if ( sf && ask_close() ) { /* some of files was choosen and ok in warning dialog */

                     l_text t = file_gettext(sf); /* get text from file */
                     l_text x = io_path(sf); /* get only path from sf (file) */

                     _free(current_file);

                     current_file = _strdup(sf);

                     _free(current_path);

                     current_path = _strdup(x); /* set last opened path */

                     editor->set_text(editor, t); /* set this text to t_editor object */

                      redraw_title();

                     _free(x);
                     _free(t); /* free memory ( t_editor saved it ) */

                 };

                 _free(file);
                 _free(sf);

                 clear_event(event);

           }; break;

           case MSG_SAVE : { /* save current text to specific file */

                 clear_event(event);

                 if ( !current_file )

                    current_file = save_dialog(current_path, "*.*", get_file_items());

                 if ( current_file )

                    if ( !file_puttext(current_file, editor->text) ) /* some error occured */

                        seal_error(ERR_INFO, "%s %s", TXT_CANNOTSAVETHEFILE, current_file);

                    else {

                        _free(current_path);

                        current_path = io_path(current_file);

                        redraw_title();

                        RETYPE_FILES_IN_PATH(current_path);

                    };

           }; break;

           case MSG_SAVEAS : { /* save current text to another file */

                 l_text sf;

                 clear_event(event);

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

                    };

                 _free(sf);

           }; break;
       };
   };
};


static t_point smalldit_size_minimum ( p_view o )
{

   return point_assign(200, 150);

};

void  status_func_callback ( p_object s ) /* it's call each second */
{


     TEXTLINE(s)->set_text(TEXTLINE(s),
                               set_format_text(NULL, "[%03d:%03d] [len:%05d] %s",
                               EDITOR(editor)->line, EDITOR(editor)->line_pos,
                               strlen(EDITOR(editor)->text),
                               current_file?current_file:TXT_NONAME
                               ));

};

void  timer_func_callback ( p_object s ) /* it's call each second */
{

     time_t now = time (NULL);
     struct tm *t = localtime (&now);
     l_text buf = _malloc(100);
     if (!buf) return;
     /* Print today's date e.g. "January 31, 2001".  */
     strftime (buf, 100, "%a %b %e %H:%M:%S %Y", t);



     TEXTLINE(s)->set_text(TEXTLINE(s), buf);

};



void  init_smalldit ( void )
{
   p_textline status = NULL;
   p_textline status2 = NULL;
   t_rect r = rect_assign(0, 100, 540, 400);
   t_rect s;

   p_menuview menu = NULL;

   edit = appwin_init(_malloc(sizeof(t_appwin)),  /* make window */
                               r,
                               "SmallDit",
                               WF_MINIMIZE,
                               ap_id,
                               &trans_ev);

   VIEW(edit)->drag_mode |= DM_DRAGGROW; /* able to grow view */

   VIEW(edit)->size_minimum = &smalldit_size_minimum;

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(edit));

   s = r = VIEW(edit)->size_limits(VIEW(edit));

   r = rect_assign(r.a.x, r.a.y+1, r.a.x+20, r.a.y+20);

   menu = hormenu_init(_malloc(sizeof(t_menuview)), /* make menu bar */
                       r,
                       smalldit_menu_init());

   OBJECT(edit)->insert(OBJECT(edit), OBJECT(menu));

   r = rect_assign(r.a.x+1, r.b.y+7, s.b.x-STANDARD_SCROLLBAR_WIDTH, s.b.y-STANDARD_SCROLLBAR_WIDTH - 30);

   editor = editor_init(_malloc(sizeof(t_editor)), /* make editor */
                        r,
                        0);

   VIEW(editor)->align |= TX_ALIGN_BOTTOM+TX_ALIGN_RIGHT;

   OBJECT(edit)->insert(OBJECT(edit), OBJECT(editor));

   r = rect_assign(s.a.x + 10, s.b.y-20, s.b.x, s.b.y);

   status = dyntext_init ( _malloc(sizeof(t_textline)),
                            r,
                            300);

   OBJECT(status)->func_callback = &status_func_callback;


   OBJECT(edit)->insert(OBJECT(edit), OBJECT(status));

   status->set_text(status, "Hallo");

   init_stillprocess ( OBJECT(status), 100 );

   r = rect_assign(s.a.x + 200, s.a.y+1, s.b.x-20, s.a.y+20);

   status2 = dyntext_init ( _malloc(sizeof(t_textline)),
                            r,
                            300);

   OBJECT(status2)->func_callback = &timer_func_callback;

   status2->flags |= TX_ALIGN_RIGHT;


   OBJECT(edit)->insert(OBJECT(edit), OBJECT(status2));

   status2->set_text(status2, "Hallo");

   init_stillprocess ( OBJECT(status2), 1000 );

   

};


app_begin ( void ) {


  if ( ap_process == AP_INIT ) {

      l_text t = NULL;

      current_path = _strdup("c:/");

      init_smalldit();

      t = file_gettext(ap_args); /* get text from file */

      editor->set_text(editor, t); /* set this text to t_editor object */

      redraw_title();

      _free(t); /* free memory ( t_editor saved it ) */

  };

  if ( ap_process == AP_DONE ) {

      _free(current_path);
      _free(current_file);

  };

} app_end;

