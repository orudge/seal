/****************************************************************/
/*                                                              */
/*                          drivers.h                           */
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



#ifndef __DRIVERS_H_INCLUDE__
#define __DRIVERS_H_INCLUDE__

#include"object.h"
#include"keyboard.h"
#include"mouse.h"
#include"exedlx.h"
#include"alltogrx.h"
#include"dataini.h"

/*
   the name of the file, from where are calling sub-routines before drivers
   initializing.
*/
#define SET_DRIVERS "drivers.set"

/*
   the name of the section in file "seal.ini", where are stored information
   about fonts to be initialized.
*/
#define INI_FONTS   "fonts"

/*
   the name of the section in file "seal.ini", where are stored information
   about system icons.
*/
#define INI_ICONS   "icons"


/* names of system icons */

extern l_text  icon_id[256];


/* system icons declarations */

#define ICO_APPLICATION        0
#define ICO_BROWSER            1
#define ICO_CONTROLPANEL       2
#define ICO_RUN                3
#define ICO_NETWORK            4
#define ICO_SHUTDOWN           5
#define ICO_EDITOR             6
#define ICO_FOLDER             7
#define ICO_OPENFOLDER         8
#define ICO_FONTS              9
#define ICO_MYCOMPUTER        10
#define ICO_INTERNETDIALER    11
#define ICO_INTERNET          12
#define ICO_INTERNETSETTINGS  13
#define ICO_HELP              14
#define ICO_MAILER            15
#define ICO_PRINTERS          16
#define ICO_SETTINGS          17
#define ICO_SYSTEMFOLDER      18
#define ICO_PCSYSTEM          19
#define ICO_TEMPLATE          20
#define ICO_TRASHEMPTY        21
#define ICO_TRASHFULL         22
#define ICO_UTILITY           23
#define ICO_DRIVE3_5          24
#define ICO_DRIVE5_25         25
#define ICO_DRIVECD           26
#define ICO_DRIVEHD           27
#define ICO_FAVORITES         28
#define ICO_DOCUMENTS         29
#define ICO_SEALLOGO          30
#define ICO_AUDIO             31
#define ICO_DESKTOP           32


/* return icon by the size (s) = 16 or 32 */

#define icon_application(s)      icon_sysget(ICO_APPLICATION, s)
#define icon_browser(s)          icon_sysget(ICO_BROWSER, s)
#define icon_controlpanel(s)     icon_sysget(ICO_CONTROLPANEL, s)
#define icon_run(s)              icon_sysget(ICO_RUN, s)
#define icon_network(s)          icon_sysget(ICO_NETWORK, s)
#define icon_shutdown(s)         icon_sysget(ICO_SHUTDOWN, s)
#define icon_editor(s)           icon_sysget(ICO_EDITOR, s)
#define icon_folder(s)           icon_sysget(ICO_FOLDER, s)
#define icon_openfolder(s)       icon_sysget(ICO_OPENFOLDER, s)
#define icon_fonts(s)            icon_sysget(ICO_FONTS, s)
#define icon_mycomputer(s)       icon_sysget(ICO_MYCOMPUTER, s)
#define icon_internetdialer(s)   icon_sysget(ICO_INTENETDIALER, s)
#define icon_internet(s)         icon_sysget(ICO_INTERNET, s)
#define icon_internetsettings(s) icon_sysget(ICO_INTERNETSETTINGS, s)
#define icon_help(s)             icon_sysget(ICO_HELP, s)
#define icon_mailer(s)           icon_sysget(ICO_MAILER, s)
#define icon_printers(s)         icon_sysget(ICO_PRINTERS, s)
#define icon_settings(s)         icon_sysget(ICO_SETTINGS, s)
#define icon_systemfolder(s)     icon_sysget(ICO_SYSTEMFOLDER, s)
#define icon_pcsystem(s)         icon_sysget(ICO_PCSYSTEM, s)
#define icon_template(s)         icon_sysget(ICO_TEMPLATE, s)
#define icon_trashempty(s)       icon_sysget(ICO_TRASHEMPTY, s)
#define icon_trashfull(s)        icon_sysget(ICO_TRASHFULL, s)
#define icon_utility(s)          icon_sysget(ICO_UTILITY, s)
#define icon_drive3_5(s)         icon_sysget(ICO_DRIVE3_5, s)
#define icon_drive5_25(s)        icon_sysget(ICO_DRIVE5_25, s)
#define icon_drivecd(s)          icon_sysget(ICO_DRIVECD, s)
#define icon_drivehd(s)          icon_sysget(ICO_DRIVEHD, s)
#define icon_favorites(s)        icon_sysget(ICO_FAVORITES, s)
#define icon_documents(s)        icon_sysget(ICO_DOCUMENTS, s)
#define icon_seallogo(s)         icon_sysget(ICO_SEALLOGO, s)
#define icon_audio(s)            icon_sysget(ICO_AUDIO, s)
#define icon_desktop(s)          icon_sysget(ICO_DESKTOP, s)


#define ICON_SYSGET(id, size)   get_icon_from_library(icon_system, id, size)
#define icon_sysget(id, size)   ICON_SYSGET(id, size)
#define icon_getid(s)           get_icon_id(s)

/* icon library, get icon from this library by icon_sysget(id, size), ...see above */
extern l_iconlibrary *icon_system;


/* if non-zero the safe mode is set */
extern l_int   safe_mode;


/* fonts delcarations */

/* defines if the font will be smooth */
extern l_int   font_smooth;

/* defines if the font will 1,25* greater than normal font */
extern l_long  font_times_1_25;

/* pointer to regular system font */
extern l_font *font_system;
/* pointer to bold system font */
extern l_font *font_system_bd;
/* pointer to italic system font */
extern l_font *font_system_i;
/* pointer to bold+italic system font */
extern l_font *font_system_bi;

/* this function is called before Seal starting. You can initialize it to your own
   shot function.
*/
extern void (*screen_shot) ( void );

/* return width of string (str) defined by font (f) and length (l).
   if l < 0  => strlen
*/
#define     FONT_GETSTRWIDTH(f,str,l)  (f?GrFontTextLength(f,str,l):0)

/* return height of font (f) */
#define     FONT_GETSTRHEIGHT(f)       (f?GrFontTextHeight(f):0)

/* return width of char (ch) defined by font (f) */
#define     FONT_GETWIDTH(f,ch)        (f?GrFontCharLength(f,ch):0)

/* return height of font */
#define     FONT_GETHEIGHT(f)          FONT_GETSTRHEIGHT(f)


/*
   define functions for stoping the timer or reload it when it's important
*/

typedef struct t_timer_def *p_timer_def;

typedef struct t_timer_def {

  l_int   active; /* flag if the timer is active */

  void    (*stop) (); /* stop the timer */
  void    (*reload) (); /* reload timer */

  l_char  reserved[24];

  p_timer_def next;

} t_timer_def;

extern p_timer_def safe_timers;

/* safe timer */
void  safe_timer ( void (*stop)(), void (*reload)() );
/* erase previous saved timer */
void  erase_safe_timer ( void (*stop)() );
/* erase all saved timers */
void safe_timer_free_all ( void );
/* reload timers */
void safe_timer_reload ( void );
/* stop timers */
void safe_timer_halt ( void );


/* t_drivers */

typedef struct t_drivers *p_drivers;

typedef struct t_drivers {

  t_object  obclass;

} t_drivers;


/* initialize icons. function is called in function drivers_init */
void icons_init ( void );

/* destroy icons. function is called in function drivers_done */
void icons_done ( void );

/* initialize fonts. function is called in function drivers_init */
void fonts_init ( void );

/* destroy icons. function is called in function drivers_done  */
void fonts_done ( void );

/* return (id) of the system font from the name (n) */
l_int  get_icon_id ( l_text n );


/* init all drivers. this function is called in function program_init in "program.c".
   return pointer to drivers, where objects such as mouse, keyboard, ... are stored
*/
t_drivers *drivers_init ( t_drivers *o );

/* done all drivers. this function is called in function program_done in "program.c" */
l_bool drivers_done ( t_object *o );

/* end of file drivers.h */
#endif

