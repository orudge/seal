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

#ifndef __TREEVIEW_H_INCLUDED__
#define __TREEVIEW_H_INCLUDED__

#include"scroller.h"

#define MSG_UPDATE 123456

typedef struct t_treeview *p_treeview;

typedef struct t_treeitem *p_treeitem;

typedef struct t_treeitem {

   l_bool extended;
   l_text caption;
   l_text name;
   BITMAP *bmp;
   l_int level;

   void (*load_subs) ( p_treeview o, p_treeitem m );

   p_treeitem next;
   p_treeitem prev;
   p_treeitem parent;
   void * nothing; // Please don't use, please use : last->next
   p_treeitem last;

} t_treeitem;

typedef struct t_treeview {


  struct t_scroller obclass;

  p_treeitem  selected;
  p_treeitem  root;
  p_list      visiblelist;
  p_list      llist;
  l_int       top;
  l_int       working;

  l_dword     message;

  p_treeitem  (*add) ( p_treeview o, l_text parent, l_text name, l_text caption, BITMAP *bmp, l_bool extended, void(*load_subs) ( p_treeview o, p_treeitem m  ) );
  p_treeitem  (*add_ex) ( p_treeview o,p_treeitem m , l_text name, l_text caption, BITMAP *bmp, l_bool extended, void(*load_subs) ( p_treeview o, p_treeitem m  ) );
  void        (*load_from_dir) ( p_treeview o,l_text m, l_text path );
  p_treeitem  (*treeitem_init) ( p_treeview t, l_bool extended, l_text caption, l_text name, BITMAP *bmp, p_treeitem next, p_treeitem prev, p_treeitem parent, p_treeitem first, p_treeitem last,void (*load_subs) ( p_treeview o, p_treeitem m  ) );
  void        (*all_treeitem) ( p_list list, p_treeitem it, l_bool ex );
  p_treeitem  (*get) ( p_treeview o, l_text name );
  void        (*update) ( p_treeview o );

} t_treeview;

#define treeitem(o)   ((p_treeitem)(o))
#define treeview(o)   ((p_treeview)(o))

p_treeitem  add_treeitem ( p_treeview o, l_text parent, l_text name, l_text caption, BITMAP *bmp, l_bool extended,void(*load_subs) ( p_treeview o, p_treeitem m  ) );
p_treeitem  add_treeitem_ex ( p_treeview o,p_treeitem m , l_text name, l_text caption, BITMAP *bmp, l_bool extended,void(*load_subs) ( p_treeview o, p_treeitem m  ) );
void load_treeitem_from_dir_ex ( p_treeview o,p_treeitem m, l_text path );
void load_treeitem_from_dir ( p_treeview o,l_text m, l_text path );

p_treeitem treeitem_init ( p_treeview t, l_bool extended, l_text caption, l_text name, BITMAP *bmp, p_treeitem next, p_treeitem prev, p_treeitem parent, p_treeitem first, p_treeitem last,void(*load_subs) ( p_treeview o, p_treeitem m ) );
void all_treeitem ( p_list list, p_treeitem it, l_bool ex );
p_treeitem get_treeitem ( p_treeview o, l_text name );
void update_tree ( p_treeview o );
void   treeview_draw ( p_view o );
void   treeview_translate_event ( p_object o, t_event *event );

void  delete_treeitem ( p_treeview o, p_treeitem it );

l_bool  treeitem_collect_by_name_from ( p_treeitem parent, p_treeitem from );
l_bool treeview_done ( p_object o );

void load_treeitem_from_reg_ex ( p_treeview o, p_treeitem m, l_text rootkey  );
void load_treeitem_from_reg ( p_treeview o,l_text m, l_text rootkey );


void load_subs_dir ( p_treeview o, p_treeitem m  );
void load_subs_reg ( p_treeview o, p_treeitem m  );

p_treeview _treeview_init ( p_treeview o, t_rect r );
extern p_treeview (*treeview_init) ( p_treeview o, t_rect r );


#endif

