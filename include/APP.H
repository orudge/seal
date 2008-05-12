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

#ifndef __APP_H_INCLUDED__
#define __APP_H_INCLUDED__

#include<stdarg.h>
#include"menus.h"

/* app messages */
/* message, when about button is pressed*
#ifndef   MSG_ABOUTB
#define   MSG_ABOUTB     106
#endif*/
/* message, when maximize button is pressed */
#ifndef   MSG_MAXIMIZE
#define   MSG_MAXIMIZE   101
#endif
/* message, when minimize button is pressed */
#ifndef   MSG_MINIMIZE
#define   MSG_MINIMIZE   102
#endif
/* message, when resize button is pressed */
#ifndef   MSG_RESIZE
#define   MSG_RESIZE     103
#endif
/* message, when system menu button is pressed */
#ifndef   MSG_SYSMENU
#define   MSG_SYSMENU    105
#endif


/* flags for t_appwin objects */

/* enable window to be maximized ( maximize button ) */

/**/
#define   WF_MAXIMIZE   0x01
/* enable window to be minimized ( minimize button ) */
#define   WF_MINIMIZE   0x02
/* if window is open in minimize mode */
#define   WF_MINSIZE    0x04
/* if window is maximized */
#define   WF_MAXSIZE    0x08
/* window has a system menu */
#define   WF_SYSMENU    0x10
/* window has a About or Help button mode */
#define	  WF_ABOUT      0x20

/* buttons for message window */

/* insert "close" button into message window */
#define   MB_CLOSE      0x0001
/* insert "cancel" button into message window */
#define   MB_CANCEL     0x0002
/* insert "yes" button into message window */
#define   MB_YES        0x0004
/* insert "no" button into message window */
#define   MB_NO         0x0008
/* insert "help" button into message window */
#define   MB_HELP       0x0010
/* insert "ok" button into message window */
#define   MB_OK         0x0020
/* insert "continue" button into message window */
#define   MB_CONTINUE   0x0040
/* insert "exit" button into message window */
#define   MB_EXIT       0x0080
/* insert "yes to all" button into message window */
#define   MB_YESTOALL   0x0100
/* insert "no to all" button into message window */
#define   MB_NOTOALL    0x0200

/* captions for message window */

/* message window  */

/* message window will contain Warning icon + title "Warning" */
#define   MW_WARNING      1
/* message window will contain Error icon + title "Error" */
#define   MW_ERROR        2
/* message window will contain Error icon + title "Error" */
#define   MW_INFO         3
/* message window will contain Info icon + title "Error" */
#define   MW_QUESTION     4
/* message window will contain Unavailable icon + title "Unavailable" */
#define   MW_UNAVAILABLE  5
/* message window will contain Unavailable icon + title "Unavailable" */
#define   MW_SETUP        6

/* ......all icons will find in "app.dat" file */


/* icons for message window */

/* it's internal definition for "app.c" file */

#define   MI_ICONS        6

#define   MI_WARNING      0
#define   MI_ERROR        1
#define   MI_INFO         2
#define   MI_QUESTION     3
#define   MI_UNAVAILABLE  4
#define   MI_SETUP        5


/* message buttons names */

#ifndef   TXT_CLOSE
#define   TXT_CLOSE     INI_TEXT("Close")
#endif
#ifndef   TXT_CANCEL
#define   TXT_CANCEL    INI_TEXT("Cancel")
#endif
#ifndef   TXT_YES
#define   TXT_YES       INI_TEXT("Yes")
#endif
#ifndef   TXT_NO
#define   TXT_NO        INI_TEXT("No")
#endif
#ifndef   TXT_HELP
#define   TXT_HELP      INI_TEXT("Help")
#endif
#ifndef   TXT_OK
#define   TXT_OK        INI_TEXT("Ok")
#endif
#ifndef   TXT_CONTINUE
#define   TXT_CONTINUE  INI_TEXT("Continue")
#endif
#ifndef   TXT_EXIT
#define   TXT_EXIT      INI_TEXT("Exit")
#endif
#ifndef   TXT_YESTOALL
#define   TXT_YESTOALL  INI_TEXT("Yes to all")
#endif
#ifndef   TXT_NOTOALL
#define   TXT_NOTOALL   INI_TEXT("No to all")
#endif

/* message captions names */

#ifndef   TXT_WARNING
#define   TXT_WARNING      INI_TEXT("Warning")
#endif
#ifndef   TXT_ERROR
#define   TXT_ERROR        INI_TEXT("Error")
#endif
#ifndef   TXT_INFO
#define   TXT_INFO         INI_TEXT("Info")
#endif
#ifndef   TXT_QUESTION
#define   TXT_QUESTION     INI_TEXT("Question")
#endif
#ifndef   TXT_UNAVAILABLE
#define   TXT_UNAVAILABLE  INI_TEXT("Unavailble")
#endif
#ifndef   TXT_SETUP
#define   TXT_SETUP        INI_TEXT("Setup")
#endif


/*
  t_app_info

  - used for information-transfer between application and
    desktop f.e. It's used in MSG_MINIMIZE message, when app_win
    set event to EV_INFO, MSG_MINIMIZE and obj is pointer to this
    structure
*/

typedef struct t_appinfo {

   l_text      title; /* the name of window */
   p_object    obj;   /* pointer to the window */

   l_char      reserved[48]; /* reserved for future versions */

} t_appinfo;

typedef struct t_appinfo *p_appinfo;

extern t_appinfo appinfo; /* for transfers */


/*
   palette of t_appwin object :
   --------------------------

   standard colors from Seal.ini file, from section [colors] :

   index of palette        name in [colors]

   0                       3D_background
   1                       window_active_title
   2                       window_active_title_gradient
   3                       window_pasive_title
   4                       window_pasive_title_gradient
   5                       window_active_caption
   6                       window_passive_caption

*/

extern l_color  pal_appwin[];

/* appwin TAG */

#define TAG_APPWIN            0x0000040

typedef struct t_appwin *p_appwin;

/* t_appwin */

/*
  appwin object makes inherited window object. When t_appwin.app_id is not ZERO but
  the (id) of EXE/DLX file, it automatically destroy application when window is closed.
  menu - not used now.
*/

typedef struct t_appwin {

  struct t_window  obclass;

  /* (id) of application */
  l_hdlx           app_id;
  /* system menu */
  p_menuitem       menu;
  /* window 16x16 icon - automatically used for system menu icon */
  BITMAP           *icon16;
  /* window 32x32 icon */
  BITMAP           *icon32;
  /* window's original bounds (before maximizing) */
  t_rect           orig_bounds;

  p_skin acttitle;
  p_skin pastitle;
  p_skin body;

  l_char           reserved[244];  // reserve 256 bytes for future use


  /* this function is called ( if not ZERO ), whenever is event in the
     appwin_translate_event function.
     Warning ! : it's not the same with translate_event function. It's only the easy
     way, how to control messages coming into the window. Whenever
     appwin_translate_event is called ( inherited from t_object.translate_event(...) ),
     it calls this function before all process in the window and after them. If you
     want to choose if it's before or after, use o->process : PH_PREPROCESS  - before
                                                              PH_POSTPROCESS - after

     Warning ! : Not call inherited translate_event functions. It's called in
     appwin_translate_event automatically, otherwise it overflows the stack.
  */
  void             (*app_trans_ev)(p_object o, t_event *event);

} t_appwin;


#define APPWIN(o)   ((p_appwin)(o))


/* appwin functions */

/*
  returns standard system menu items depending on window's flags.
  Example of standard_system_menu_items use (from telebook.c):

  // standard menu items (Maximize, Minimize, etc)
  win->menu = standard_system_menu_items(win,
              new_menu_line(
              new_menu_item("About TeleBOOK...", 0, 0, MSG_ABOUT, NULL,
  NULL)));

*/
p_menuitem standard_system_menu_items(p_appwin o, p_menuitem next);

/*
  an equivalent for void t_object.setup(t_object *) function. This function calls
  inherited view_setup function and insert (close/maxim/minim) buttons into the window.
  this function is called after t_object.insert(t_object *o, t_object *) function,
  ...see "object.h" for more details.
*/
void      appwin_setup ( p_object o );

/*
  an equivalent for void t_window.draw_title(t_window *) function. This function draws
  the title of the window. ...see "window.h" for more details.
*/
void      appwin_draw_title ( p_window o );

/*
  an equivalent for void t_object.translate_event ( t_object*, t_event* ) function.
  This function translate events that comes into the window. It's inherited from
  win_translate_event ( see window.h ). When event comes into a window, this function
  call t_appwin.app_trans_ev before all processes in window and after them. It sets
  t_object.process to PH_PREPROCESS whenever t_appwin.app_trans_ev is called before
  and PH_POSTPROCESS whenever t_appwin.app_trans_ev is called after. Function checks
  if there was an impuls for closing and close application if t_appwin.app_id is set to
  NON_ZERO ( to (id) of application ).
*/
void      appwin_translate_event ( p_object o, t_event *event );

void      appwin_set_state ( p_object o, l_dword st, l_bool set );
void      appwin_draw ( p_view o );
t_rect    appwin_size_limits ( p_view o );

/*
  make new (appwin) window and sets t_appwin.app_id with t_appwin.app_trans_ev to ZERO.
  flags can be combination of followings :

      enable window to be maximized ( maximize button )
      #define   WF_MAXIMIZE   0x01
      enable window to be minimized ( minimize button )
      #define   WF_MINIMIZE   0x02
      if window is open in minimize mode
      #define   WF_MINSIZE    0x04
*/
extern p_appwin  (*window_init) ( p_appwin o, t_rect r, l_text caption, l_int flags );

/*
  make new (appwin) window and sets t_appwin.app_trans_ev to "trans_ev" ( see above )
  and t_appwin.app_id to "id". "id" is the identification number of application you will
  get by global variable ap_id from "seal.h". If application is run it sets the
  ap_id of current application.

  flags can be combination of followings :

      enable window to be maximized ( maximize button )
      #define   WF_MAXIMIZE   0x01
      enable window to be minimized ( minimize button )
      #define   WF_MINIMIZE   0x02
      if window is open in minimize mode
      #define   WF_MINSIZE    0x04
*/
extern p_appwin  (*appwin_init) ( p_appwin o, t_rect r, l_text caption, l_int flags, l_hdlx id,
                                  void (*trans_ev)( p_object o, t_event *e ));

p_appwin  _window_init ( p_appwin o, t_rect r, l_text caption, l_int flags );
p_appwin  _appwin_init ( p_appwin o, t_rect r, l_text caption, l_int flags, l_hdlx id,
                         void (*trans_ev)( p_object o, t_event *e ));

#define new_application(r,c,f,func)  appwin_init(_malloc(sizeof(t_appwin)), r, c, f, ap_id, func);


/* message window */

p_appwin  _message_init_ex ( p_object owner, p_appwin o, t_rect r, l_text caption, l_int flags,
                             l_char   iflags,
                             l_dword  bflags, void (*trans_ev)( p_object o, t_event *e ),
                             l_text in_text, va_list argp );

extern p_appwin  (*message_init_ex) ( p_object owner, p_appwin o, t_rect r, l_text caption, l_int flags,
                                      l_char   iflags,
                                      l_dword  bflags, void (*trans_ev)( p_object o, t_event *e ),
                                      l_text in_text, va_list argp );


l_dword  msgbox_arg ( l_dword wflags, l_dword  bflags, l_text in_text, va_list args );

/*
  show message box, where (wflags) and (bflags) are flags defined above
  and (in_text+...) is formated text of message box.
  Return one of values defined above...MSG_CLOSE, MSG_OK, .... .
*/
l_dword  msgbox ( l_dword wflags, l_dword  bflags, l_text in_text, ... );

/*
  same as msgbox function, but it set minimal size of width to (minx),
  because msgbox set width & height to text.
*/
l_dword  msgboxsize ( l_rect minx, l_dword wflags, l_dword  bflags, l_text in_text, ... );

/*
  the same as "msgbox" but formated text is defined by arument list.
*/
l_dword  msgboxsize_arg ( l_rect minx, l_dword wflags, l_dword  bflags, l_text in_text, va_list args );

/*
  make message box by wflags = MW_INFO and return pointer to this box.
*/
p_appwin msginfo ( l_rect minx, l_dword wflags, l_text in_text, ... );

/*
  the same as "msginfo" but formated text is defined by arument list.
*/
p_appwin msginfo_arg ( l_rect minx, l_dword wflags, l_text in_text, va_list args );

/*
  make process window, where (minx) is minimal size of the window,
  process_title is name of window, (in_text) is text for process,
  (size) is size for process and (ind) is pointer to l_dword variable,
  where we want safe information about process.
  It returns pointer to this window. See also object t_process "dialogs.h".
*/
p_appwin msgprocess_ex ( l_rect minx, l_text process_title, l_text in_text,
                         l_dword size, l_dword *ind, l_int flags, l_hdlx id,
                         void (*trans_ev)(p_object, p_event) );

/*
  make process window, where (minx) is minimal size of the window,
  process_title is name of window, (in_text) is text for process,
  (size) is size for process and (ind) is pointer to l_dword variable,
  where we want safe information about process.
  It returns pointer to this window. See also object t_process "dialogs.h".
*/
p_appwin msgprocess ( l_rect minx, l_text process_title, l_text in_text, l_dword size, l_dword *ind );


/*
  make new msgprocess in minimal size (minx = 400).
  At the and of process, dispose this window by function hide_process(o)
*/
#define  show_process_ex(wt,pt,s,i,f,id,e)   (void*)msgprocess_ex(400, wt, pt, s, i, f, id, e)
#define  show_process(wt,pt,s,i)             (void*)msgprocess(400, wt, pt, s, i)

/*
  dispose message process, that was created by function show_process

  example :

  l_dword ind = 0;
  void *x = show_process("Process", "Calling 1000x", 1000, &ind);

  _for ((*ind); (*ind) < 1000; (*ind)++) delay(50);

  hide_process(x);
*/
#define  hide_process(o)           dispose(OBJECT(o))

/*
  create message info box and return pointer in (void*).
  You may later dipose this message box by function hide_info(o)
*/
#define  show_info                 (void*)msginfo

/*
  dispose message info box, that was created by function show_info
*/
#define  hide_info(o)              dispose(OBJECT(o))



/* frame object */

/*
 make frame. It's same as t_window object, but it not draw the title. You can close
 it by ALT+F4, same as window. If the title is not shown, it's also not important to
 called the window.
*/
extern p_window  (*frame_init) ( p_window o, t_rect r );
p_window  _frame_init ( p_window o, t_rect r );

/*
  an equivalent of "void t_window.draw_title ( t_window * )" function.
  It makes nothing. Not draw anything.
*/
void    frame_draw_title ( p_window o );

/*
  an equivalent of "void t_view.draw ( t_view * )" function.
  This draw only frame arround the background.
*/
void    frame_draw ( p_view o );

#endif

