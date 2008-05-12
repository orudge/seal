/****************************************************************/
/*                                                              */
/*                          desktop.h                           */
/*                                                              */
/*                Seal - free desktop environment               */
/*                                                              */
/*                  Copyright (c) 1999,2000                     */
/*                       Michal Stencl                          */
/*                    All Rights Reserved                       */
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



#ifndef __DESKTOP_H_INCLUDED__
#define __DESKTOP_H_INCLUDED__


/* desktop menu texts */

#ifndef TXT_CANTRUNFILE
#define TXT_CANTRUNFILE        INI_TEXT("Can't run file")
#endif
#ifndef TXT_PROGRAMS
#define TXT_PROGRAMS           INI_TEXT("Programs")
#endif
#ifndef TXT_SETTINGS
#define TXT_SETTINGS           INI_TEXT("Settings")
#endif
#ifndef TXT_CLOSEWIN
#define TXT_CLOSEWIN           INI_TEXT("Close window")
#endif
#ifndef TXT_SHUTDOWN
#define TXT_SHUTDOWN           INI_TEXT("Shut down")
#endif
#ifndef TXT_REFRESHDESKTOP
#define TXT_REFRESHDESKTOP     INI_TEXT("Refresh desktop")
#endif
#ifndef TXT_RESTART
#define TXT_RESTART            INI_TEXT("Restart")
#endif
#ifndef TXT_AREYOUSUREYOUWANTTOQUITSEAL
#define TXT_AREYOUSUREYOUWANTTOQUITSEAL INI_TEXT("Are you sure you want to quit Seal")
#endif


/*
 t_desktopline object is used for inserting any objects to the desktopline you can
 see on the top of the Seal desktop.
*/

/* t_desktopline */
typedef struct t_desktopline *p_desktopline;

typedef struct t_desktopline {

   /* inherited functions / variabes from t_view and t_object objects */
   struct t_view     obclass;

   /* this function is called, whenever is new object inserted or removed into/from
      the desktopline object and resize desktopline to actual width and height.
   */
   void            (*refresh_place) ( p_desktopline o );

} t_desktopline;


#define DESKTOPLINE(o) ((p_desktopline)(o))


/* indicates if it can be drawed under desktop line. 1 = it is, 0 = it isn't  */
extern l_int            draw_under_line;

/* desktop line. it's used for inserting to desktopline of the Seal. Use this value
   for inserting to an object.
*/
extern p_desktopline  desktopline;


/* t_desktop */

/*
 init standard desktop line. it's used for internal desktop.dlx using.
*/
p_desktopline   _desktopline_init ( p_desktopline o );

extern p_desktopline  (*desktopline_init) ( p_desktopline o );


/*
 redefine desktop size_limits when the draw_under_line is set to NON-ZERO.
*/
t_rect    desktop_size_limits ( p_view o ); /* !!! redefine desktop size_limits */

/*
 an equivalent of "l_dword t_object.valid ( p_object o, l_dword msg )" function.
 it's called, whenever the message MSG_QUIT is called. control quiting of the Seal.
*/
l_dword   program_valid ( p_object o, l_dword msg );

/*
 genrates new icon, that's placed in the desktop.dat file, which is attached into
 the end of desktop.dlx file. Used for internal using of desktop.dlx
*/
p_object  desktopline_genbutton ( l_int ico, l_text info, l_dword msg );

/*
 an equivalent of "t_rect t_view.size_limits ( p_view o )" function. set limits of
 desktop line, where can be sub-objects inserted.
*/
t_rect    desktopline_size_limits ( p_view o );

/*
 an equivalent of "void t_object.remove ( p_object o, p_object sub )" function.
 it calls "obj_remove" function together with t_desktopline.refresh_place function.
 this function removes sub-object from the desktopline and set width and height of the
 desktopline to actual values.
*/
void      desktopline_remove ( p_object o, p_object sub );

/*
 an equivalent of "t_object* t_object.insert ( p_object o, p_object sub )" function.
 it calls "obj_insert" function together with t_desktopline.refresh_place function.
 this function inserts sub-object to the desktopline and set width and height of the
 desktopline to actual values.
*/
t_object* desktopline_insert ( p_object o, p_object sub );

/*
 an equivalent of "void t_view.draw ( p_view o )" function.
 draws desktopline.
*/
void      desktopline_draw ( p_view o );

/*
 this function is called, whenever is new object inserted or removed into/from
 the desktopline object and resize desktopline to actual width and height.
*/
void      desktopline_refresh_place ( p_desktopline o );

/*
 load menu from the path. make new array of t_menuitem(s)
*/
p_menuitem  menu_load_from_dir ( l_text path, l_text filename );

/*
 ...used for internal desktop.dlx. translate event, whenever an item from
 "menu_load_from_dir" is pressed.
*/
void        filemenu_trans ( p_object o, p_event event );

#endif

