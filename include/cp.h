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

#ifndef _CONTROL_PANEL_H_
#define _CONTROL_PANEL_H_

typedef struct t_cp_section {

	l_text  caption;
	l_text  info;
	l_text  fullcaption;
	l_text  helpfile;
	BITMAP *icon32;

} t_cp_section, *p_cp_section;

#define CP_SECTION(o) ((p_cp_section)(o))

typedef struct t_cp_loader {

     l_text  caption;
     l_text  helpfile;
     BITMAP *icon16;
     BITMAP *icon32;

     void  (*load)     ( p_object o );
     void  (*trans_ev) ( p_object o, t_event *event );
     void  (*save)     ( void );

     p_cp_section sec;

} t_cp_loader, *p_cp_loader;

#define CP_LOADER(o) ((p_cp_loader)(o))

p_cp_loader add_cp_loader ( l_text  caption,
                     l_text helpfile,
                     p_cp_section   sec,
                     BITMAP *icon32,
                     BITMAP *icon16,
                     void  (*load) ( p_object o ),
                     void  (*trans_ev) ( p_object o, t_event *event ),
                     void  (*save) ( void ) );

p_cp_section add_cp_section ( l_text  caption,
                      l_text  fullcaption,
                      l_text  info,
                      l_text  helpfile,
                      BITMAP *icon32 );

void load_cp ( );

extern p_list    cp_loaders;
extern p_list    cp_sections;

extern p_cp_section cps_display;
extern p_cp_section cps_hardware;

void show_cp_section ( p_cp_section o );
void show_cp_panel ( p_cp_loader o );

#endif
