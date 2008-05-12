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
 * 08/01/2002: First created (Pablo Borobia)
 * 23/09/2002: Some changes (Julien Etelain)
 */

#include <seal.h>

t_point tips_oldpos;
l_int   tips_count;


void tips_func_callback( p_object o ) {

  t_point m = VIEW(o)->get_global_point(VIEW(o), mouse->where);

  if (m.x==tips_oldpos.x && m.y==tips_oldpos.y) {
    tips_count++; }
  else {
    tips_count = 0;
    tips_oldpos = m;
  }

  if (tips_count == 15) {
    p_view tmp = desktop->get_view_under_mouse(desktop);
    if (tmp) {
      while ( tmp->get_view_under_mouse(tmp) != NULL) {
        tmp = tmp->get_view_under_mouse(tmp);
      }
      view_show_info_board(tmp);
    }
    //count = 0;
  }
};

p_view tips_init (p_view o, t_rect r) {
   if ( !o ) return NULL;
   view_init(o, r);
   o->transparent = FALSE;

   OBJECT(o)->process_tick = 100;
   OBJECT(o)->func_callback = &tips_func_callback;
   OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
   OBJECT(o)->set_options(OBJECT(o), OB_OF_STILLPROCESS, true);
   return o;
};

void tips_system_init ( void ) {
   p_view  o = tips_init(_malloc(sizeof(t_view)),rect_assign(0,0,0,0));
   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(o));
   o->hide(o);
};
