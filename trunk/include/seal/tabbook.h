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

#ifndef _TABBOOK_SEAL_H_
#define _TABBOOK_SEAL_H_

typedef struct t_tab *p_tab;
typedef struct t_tabbook *p_tabbook;

typedef struct t_tab {
  struct t_view obclass;

  l_text  caption;
  BITMAP *icon16;

} t_tab;

#define TAB(o) ((p_tab)(o))

typedef struct t_tabbook {
  struct t_view obclass;

  p_list tabs;

  p_tab  sel;

  void   (*insert)   ( p_tabbook o, l_int tab, p_object ob );
  void   (*remove)   ( p_tabbook o, l_int tab, p_object ob );
  void   (*show_tab) ( p_tabbook o, l_int tab );
  l_int  (*add_tab)  ( p_tabbook o, l_text caption, BITMAP *icon16 );
  void   (*del_tab)  ( p_tabbook o, l_int tab );

} t_tabbook;
#define TABBOOK(o) ((p_tabbook)(o))

extern p_tabbook (*tabbook_init)( p_tabbook o, t_rect r );
p_tabbook _tabbook_init ( p_tabbook o, t_rect r );
p_tabbook tabbook_make ( t_rect r );

#endif