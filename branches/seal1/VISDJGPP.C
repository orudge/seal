/****************************************************************/
/*                                                              */
/*                           visdjgpp.c                         */
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



#include<errno.h>
#include"allegro.h"
#include"seal.h"
#include"menus.h"
#include"app.h"
#include"button.h"
#include"dialogs.h"
#include"editor.h"
#include"iodlg.h"

#include"syntax.c"

typedef struct t_error_listbox_item *p_error_listbox_item;

typedef struct t_error_listbox_item {

   struct t_listbox_item   obclass;

   l_long   line;

} t_error_listbox_item;


#define free_error_listbox_item   free_listbox_item


#include"viscomp.c"


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


#ifndef TXT_COMPILE
#define TXT_COMPILE       INI_TEXT("Compile")
#endif
#ifndef TXT_RUNEXE
#define TXT_RUNEXE        INI_TEXT("Make and run exe")
#endif
#ifndef TXT_MAKEDLX
#define TXT_MAKEDLX       INI_TEXT("Make dlx")
#endif
#ifndef TXT_HELP
#define TXT_HELP          INI_TEXT("Help")
#endif
#ifndef TXT_ABOUT
#define TXT_ABOUT         INI_TEXT("About")
#endif
#ifndef TXT_SEALREFERENCE
#define TXT_SEALREFERENCE INI_TEXT("Seal reference")
#endif


/* messages */

#define MSG_NEW      100001
#define MSG_OPEN     100002
#define MSG_SAVE     100003
#define MSG_SAVEAS   100004

#define MSG_xRUNEXE        100010
#define MSG_xMAKEDLX       100011
#define MSG_xABOUT         100012
#define MSG_xSEALREFERENCE 100013
#define MSG_xRUNEXELIBS    100014
#define MSG_xFIND          100015


/* */
#define SIZE_FOR_ERRORS 100


/* current path to file */
static l_text   current_path = NULL;
/* current file */
static l_text   current_file = NULL;
/* current libraries for the file */
static l_text   current_libs = NULL;

static p_syntax   editor = NULL;
static p_appwin   edit = NULL;
static p_listbox  errorbox = NULL;
static p_textline findstr = NULL;
static l_long     deltafind = 0;

#define ask_close() ((l_bool)(msgbox(MW_INFO, MB_YES|MB_NO, TXT_SD0001) != MSG_NO))


static l_text get_libraries ( void )
{
   t_rect   r = rect_assign(0, 0, 400, 130);
   p_appwin x = appwin_init(_malloc(sizeof(t_appwin)), r, INI_TEXT("Add libraries"), 0, 0, NULL);
   l_text text = NULL;
   p_textline tl = NULL;

   if ( x ) {

      VIEW(x)->align |= TX_ALIGN_CENTER;

      OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(x));

      r = VIEW(x)->size_limits(VIEW(x));

      r = rect_assign(r.a.x+10, r.a.y+10, r.b.x-10, r.a.y+30);

      OBJECT(x)->insert(OBJECT(x), OBJECT(stattext_init(_malloc(sizeof(t_stattext)),
                                                      r,
                                                      TX_ALIGN_BOTTOM,
                                                      INI_TEXT("Add libraries")
                                                      )));

      r = rect_assign(r.a.x, r.b.y+5, r.b.x, r.b.y+25);

      tl = textline_init(_malloc(sizeof(t_textline)), r, IO_FILE_LIMIT, 0);

      OBJECT(x)->insert(OBJECT(x), OBJECT(tl));

      if ( tl ) tl->set_text(tl, current_libs);

      r = rect_assign(r.a.x, r.b.y+15, r.a.x+80, r.b.y+35);

      OBJECT(x)->insert(OBJECT(x), OBJECT(button_init(_malloc(sizeof(t_button)),
                                                      r,
                                                      TXT_OK,
                                                      MSG_OK,
                                                      BF_DEFAULT)));

      r = rect_assign(r.b.x+10, r.a.y, r.b.x+90, r.b.y);

      OBJECT(x)->insert(OBJECT(x), OBJECT(button_init(_malloc(sizeof(t_button)),
                                                      r,
                                                      TXT_CLOSE,
                                                      MSG_CLOSE,
                                                      BF_NORMAL)));

   };


   if ( desktop->execute_view(desktop, VIEW(x)) == MSG_OK && tl ) {

        if ( current_libs ) _free(current_libs);

        current_libs = _strdup(tl->text);
   };

   dispose(OBJECT(x));

   return current_libs;

};


static void find_trans_ev ( p_object o, p_event event )
{
   if ( event->type == EV_MESSAGE ) {

         if ( event->message == MSG_OK && findstr && findstr->text ) { /* find string */

             l_text x = NULL;

             clear_event(event);

             if ( editor && EDITOR(editor)->text &&
                 !(x=stristr(&((EDITOR(editor)->text)[deltafind]), findstr->text)) )
             {

                  deltafind = strsize(EDITOR(editor)->text, x)+1;

             } else seal_error(ERR_INFO, INI_TEXT("Text not found !"));

         };
   };
};

static void find_string ( void )
{
   t_rect   r = rect_assign(0, 0, 400, 130);
   p_appwin x = appwin_init(_malloc(sizeof(t_appwin)), r, INI_TEXT("Find text"), 0, 0, &find_trans_ev);

   if ( x ) {

      VIEW(x)->align |= TX_ALIGN_CENTER;

      OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(x));

      r = VIEW(x)->size_limits(VIEW(x));

      r = rect_assign(r.a.x+10, r.a.y+10, r.b.x-10, r.a.y+30);

      OBJECT(x)->insert(OBJECT(x), OBJECT(stattext_init(_malloc(sizeof(t_stattext)),
                                                      r,
                                                      TX_ALIGN_BOTTOM,
                                                      INI_TEXT("Find text")
                                                      )));

      r = rect_assign(r.a.x, r.b.y+5, r.b.x, r.b.y+25);

      findstr = textline_init(_malloc(sizeof(t_textline)), r, 250, 0);

      OBJECT(x)->insert(OBJECT(x), OBJECT(findstr));

      r = rect_assign(r.a.x, r.b.y+15, r.a.x+80, r.b.y+35);

      OBJECT(x)->insert(OBJECT(x), OBJECT(button_init(_malloc(sizeof(t_button)),
                                                      r,
                                                      TXT_OK,
                                                      MSG_OK,
                                                      BF_DEFAULT)));

      r = rect_assign(r.b.x+10, r.a.y, r.b.x+90, r.b.y);

      OBJECT(x)->insert(OBJECT(x), OBJECT(button_init(_malloc(sizeof(t_button)),
                                                      r,
                                                      TXT_CLOSE,
                                                      MSG_CLOSE,
                                                      BF_NORMAL)));

   };


   desktop->execute_view(desktop, VIEW(x));

   dispose(OBJECT(x));

   findstr = NULL;
   deltafind = 0;

};


/* make list of supported files */
static p_list get_file_items ( void ) {

                                                /* function for free-ing items */
  p_list p = list_init(malloc(sizeof(t_list)), &free_filehistory_item, 0);

  if ( p ) {

     p->insert(p, new_filehistory_item("C files (*.c)", "*.c"));
     p->insert(p, new_filehistory_item("Header files (*.h)", "*.h"));
     p->insert(p, new_filehistory_item("Export files (*.exp)", "*.exp"));
     p->insert(p, new_filehistory_item("C++ files (*.cc)", "*.cc"));
     p->insert(p, new_filehistory_item("C++ files (*.o)", "*.o"));
     p->insert(p, new_filehistory_item("SEAL' execution (*.exe)", "*.exe"));
     p->insert(p, new_filehistory_item("SEAL' library (*.dlx)", "*.dlx"));
     p->insert(p, new_filehistory_item("All files (*.*)", "*.*"));

  };


  return p;

};


/* make menu */
static p_menu  visdjgpp_menu_init ( void )
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

                  new_sub_menu(INI_TEXT("Search"), new_menu(

                     new_menu_item(INI_TEXT("Find"), "", 0, MSG_xFIND, NULL,

                     NULL)),

                  new_sub_menu(TXT_COMPILE, new_menu(

                     new_menu_item(TXT_RUNEXE, "", 0, MSG_xRUNEXE, NULL,
                     new_menu_item(INI_TEXT("Run EXE by LIBS"), "", 0, MSG_xRUNEXELIBS, NULL,
                     new_menu_item(TXT_MAKEDLX, "", 0, MSG_xMAKEDLX, NULL,

                     NULL)))),

                  new_sub_menu(TXT_HELP, new_menu(

                     new_menu_item(TXT_ABOUT, "", 0, MSG_xABOUT, NULL,
                     new_menu_item(TXT_SEALREFERENCE, "", 0, MSG_xSEALREFERENCE, NULL,

                     NULL))),

               NULL)))));

   return menu;

};


/* redraw title of window */
void    redraw_title ( void )
{

  if ( edit ) {

      _free(WINDOW(edit)->caption);

      if ( current_file )

         WINDOW(edit)->caption = set_format_text(NULL, "VisualDJGPP - %s", current_file);

      else

         WINDOW(edit)->caption = set_format_text(NULL, "VisualDJGPP - %s", TXT_NONAME);

      /* title, but not redraw any objects in the title such as X  */
      TEST_SUB_VIEWS(VIEW(edit), WINDOW(edit)->draw_title(WINDOW(edit)));

  };

};


static void make_error_box ( p_list list )
{

   t_rect r = VIEW(edit)->size_limits(VIEW(edit));

   r = rect_assign(r.a.x, r.b.y - SIZE_FOR_ERRORS, r.b.x - STANDARD_SCROLLBAR_WIDTH -1, r.b.y);

   if ( !errorbox ) {

            /* make error box if not exist yet */
            errorbox = listbox_init(_malloc(sizeof(t_listbox)), r, NULL, 1, 0);

            if ( errorbox ) {
                /* set alignments */
                VIEW(errorbox)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_RIGHT|TX_ALIGN_FIXEDY;

                /* set background */
                VIEW(errorbox)->brush.color = COLOR(CO_SYSx26);

            };
            /* insert it to window */
            OBJECT(edit)->insert(OBJECT(edit), OBJECT(errorbox));

   };

   if ( errorbox ) {
         /* dispose old list */
         dispose_list(&(errorbox->list), true);
         /* set new list */
         errorbox->set_list(errorbox, list);
   };
};



static void trans_ev ( p_object o, p_event event )
{

   if ( event->type == EV_MESSAGE ) {

       switch ( event->message ) {

           /* section Find */

           case MSG_xFIND : { /* find string for the text */

                 clear_event(event);

                 find_string();

           }; break;

           /* section Compile */

           case MSG_xRUNEXE : { /* make the application */

                 clear_event(event);

                 if ( !current_file ) /* file not saved yet */

                       message(o, EV_MESSAGE, MSG_SAVE, o);

                 if ( current_file ) { /* file was saved ! */

                        l_text exefile = NULL;
                        p_list list = NULL;

                        /* save file */
                        file_puttext(current_file, EDITOR(editor)->text);

                        /* compile */
                        exefile = compile(current_file, current_libs, &list);

                        /* make error box */
                        make_error_box(list);

                        if ( exefile ) { /* everything is OK */

                            if ( !run_file(exefile) ) /* error in EXE calling */

                                seal_error(ERR_INFO, "Can't run %s file", exefile);

                            _free(exefile);

                        } else { /* errors */
                        };

                 } else seal_error(ERR_INFO, "Please save your file before compiling");

           }; break;

           case MSG_xRUNEXELIBS : { /* make the application together with the libs */

                 clear_event(event);

                 if ( !current_file ) /* file not saved yet */

                       message(o, EV_MESSAGE, MSG_SAVE, o);

                 if ( current_file ) { /* file was saved ! */

                        l_text exefile = NULL;
                        p_list list = NULL;

                        /* save file */
                        file_puttext(current_file, EDITOR(editor)->text);

                        /* get libraries names */
                        get_libraries();

                        /* compile */
                        exefile = compile(current_file, current_libs, &list);

                        /* make error box */
                        make_error_box(list);

                        if ( exefile ) { /* everything is OK */

                            if ( !run_file(exefile) ) /* error in EXE calling */

                                seal_error(ERR_INFO, "Can't run %s file", exefile);

                            _free(exefile);

                        } else { /* errors */
                        };

                 } else seal_error(ERR_INFO, "Please save your file before compiling");

           }; break;

           /* section File */

           case MSG_CLOSE : { /* close text */

               t_event oldev = *event;

               clear_event(event);

               if ( o->phase == PH_PREPROCESS );

                   if ( ask_close() )

                      *event = oldev; /* recover events */


           }; break;

           case MSG_NEW : { /* make new text */

                 clear_event(event);

                 if ( ask_close() ) { /* some of files was choosen and ok in warning dialog */

                     l_text txt = file_gettext ( "app.wiz" );

                     afree((void**)(&current_file));

                     EDITOR(editor)->set_text(EDITOR(editor), txt); /* clear text in t_editor object */

                     afree((void**)(&txt));

                     redraw_title();

                 };

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

                     current_file = _strdup(sf); /* set current name of file */

                     _free(current_path);

                     current_path = _strdup(x); /* set last opened path */

                     EDITOR(editor)->set_text(EDITOR(editor), t); /* set this text to t_editor object */

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

                    if ( !file_puttext(current_file, EDITOR(editor)->text) ) /* some error occured */

                        seal_error(ERR_INFO, "%s %s", TXT_CANNOTSAVETHEFILE, current_file);

                    else {

                        _free(current_path);

                        current_path = io_path(current_file);

                        redraw_title();

                        /* redraw all file-boxes that have same path */
                        RETYPE_FILES_IN_PATH(current_path);

                    };

           }; break;

           case MSG_SAVEAS : { /* save current text to another file */

                 l_text sf;

                 clear_event(event);

                 sf = save_dialog(current_path, "*.*", get_file_items());

                 if ( sf )

                    if ( !file_puttext(sf, EDITOR(editor)->text) ) /* some error occured */

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


static t_point  visdjgpp_size_minimum ( p_view o )
{

    return point_assign(300, 300);

};


void  init_visdjgpp ( void )
{

   t_rect r = rect_assign(0, 50, 630, 480);
   t_rect s;

   p_menuview menu = NULL;

   edit = appwin_init(_malloc(sizeof(t_appwin)),  /* make window */
                               r,
                               "Visual DJGPP",
                               WF_MINIMIZE,
                               ap_id,
                               &trans_ev);

   VIEW(edit)->drag_mode |= DM_DRAGGROW; /* able to grow view */

   VIEW(edit)->size_minimum = &visdjgpp_size_minimum;

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(edit));

   s = r = VIEW(edit)->size_limits(VIEW(edit));

   r = rect_assign(r.a.x, r.a.y+1, r.a.x, r.a.y+20);

   menu = hormenu_init(_malloc(sizeof(t_menuview)), /* make menu bar */
                       r,
                       visdjgpp_menu_init());

   OBJECT(edit)->insert(OBJECT(edit), OBJECT(menu));

   r = rect_assign(r.a.x+1, r.b.y+7, s.b.x-STANDARD_SCROLLBAR_WIDTH, s.b.y-STANDARD_SCROLLBAR_WIDTH - SIZE_FOR_ERRORS);

   editor = syntax_C_init(_malloc(sizeof(t_syntax)), /* make editor */
                        r,
                        0);

   if ( editor ) {

        /* set background */
        VIEW(editor)->brush.color = COLOR(CO_WHITE);

        VIEW(editor)->align |= TX_ALIGN_BOTTOM+TX_ALIGN_RIGHT;

   };

   OBJECT(edit)->insert(OBJECT(edit), OBJECT(editor));

   make_error_box(NULL);

};


app_begin ( void ) {

  if ( ap_process == AP_INIT ) {

      l_text t = NULL;

      current_path = _strdup("c:/");

      init_visdjgpp();

      t = file_gettext(ap_args); /* get text from file */

      EDITOR(editor)->set_text(EDITOR(editor), t); /* set this text to t_editor object */

      redraw_title();

      _free(t); /* free memory ( t_editor saved it ) */

  };

  if ( ap_process == AP_DONE ) {

      _free(current_path);
      _free(current_file);
      _free(current_libs);

  };

} app_end;

