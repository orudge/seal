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

#include <seal.h>
#include <seal/tabbook.h>

p_tabbook (*tabbook_init)( p_tabbook o, t_rect r ) = &_tabbook_init;

////////////////////////////////////////////////////////////////////////////////
void tabbook_insert ( p_tabbook o, l_int tab, p_object ob ) {
  p_tab t = o->tabs->at(o->tabs,tab);
  if ( !t ) return;
  OBJECT(t)->insert(OBJECT(t),ob);
};
////////////////////////////////////////////////////////////////////////////////
void tabbook_remove ( p_tabbook o, l_int tab, p_object ob ) {
  p_tab t = o->tabs->at(o->tabs,tab);
  if ( !t ) return;
  OBJECT(t)->remove(OBJECT(t),ob);
};
////////////////////////////////////////////////////////////////////////////////
void tabbook_del_tab ( p_tabbook o, l_int tab ) {
  o->tabs->free_index(o->tabs,tab);
  if ( OBJECT(o)->owner ) VIEW(o)->draw_me(VIEW(o));
};
////////////////////////////////////////////////////////////////////////////////
void    tab_setup ( p_object o ) {
  VIEW(o)->reset_align(VIEW(o));
};
////////////////////////////////////////////////////////////////////////////////
l_int tabbook_add_tab  ( p_tabbook o, l_text caption, BITMAP *icon16 ) {
  t_rect r = VIEW(o)->get_local_extent(VIEW(o));
  p_tab tab = malloc(sizeof(t_tab));
  if ( !tab ) return -1;
  clear_type(tab,sizeof(t_tab));
  r.a.x += 2;
  r.a.y += 22;
  r.b.x -= 2;
  r.b.y -= 2;
  view_init(VIEW(tab),r);
  tab->caption = caption;
  tab->icon16 = icon16;
  VIEW(tab)->brush.color = VIEW(o)->brush.color;
  OBJECT(tab)->setup = &tab_setup;
  OBJECT(o)->insert(OBJECT(o),OBJECT(tab));
  if ( !o->sel ) o->sel = tab;
  if ( OBJECT(o)->owner ) VIEW(o)->draw_me(VIEW(o));

  return  o->tabs->insert(o->tabs,tab);
};
////////////////////////////////////////////////////////////////////////////////
void   tabbook_show_tab ( p_tabbook o, l_int tab ) {
  p_tab t = o->tabs->at(o->tabs,tab);
  if ( !t ) return;
  VIEW(t)->show(VIEW(t));
  if ( o->sel ) VIEW(o->sel)->hide(VIEW(o->sel));
  o->sel = t;
  if ( OBJECT(o)->owner ) VIEW(o)->draw_view(VIEW(o));
};
////////////////////////////////////////////////////////////////////////////////
void tabbook_draw( p_view o ) {

  t_rect r = VIEW(o)->get_local_extent(VIEW(o));
  t_point p;
  BITMAP *out = VIEW(o)->begin_paint(VIEW(o), &p, r);

  if ( out ) {
    p_tabbook tb = TABBOOK(o);

    o->background(o, out, rect_move(r, p.x, p.y));

    button3d(o,out,p.x,p.y+20,p.x+r.b.x,p.y+r.b.y, 0);

    if ( tb->tabs->last ) {
	  p_item f = tb->tabs->last->next;
	  p_item pp = f;
      l_int  x = p.x+1;
	  do {
	    p_tab tab = TAB(pp->rec);
        l_int tab_width = get_text_length(VIEW(o)->font, tab->caption, -1)+10;
        l_int y = p.y+1;
        l_int xx = 0;

        if ( tab == tb->sel ) {
          y--;
          xx = 1;
        };

        line(out, x-xx, y+1, x-xx, p.y+20-1, color_3d_light);
        line(out, x-xx+1, y, x+tab_width+xx-1, y, color_3d_light);
        line(out, x+tab_width+xx-1, y+1, x+xx+tab_width-1, p.y+20-1, color_3d_shadow );
        line(out, x+tab_width+xx, y+1, x+tab_width+xx, p.y+20-1, color_3d_dark );

        if ( xx ) {
          textout_draw_rect(out, VIEW(o)->font, tab->caption, -1, x+2, p.y, x+tab_width-2, p.y+20-2, TX_ALIGN_CENTER, color_3d_text, TX_NOCOLOR, 0);
          line(out, x-xx+1, r.a.y+p.y+20, x+tab_width+xx-1, p.y+20, color_3d_face);
        } else {
          textout_draw_rect(out, VIEW(o)->font, tab->caption, -1, x+2, p.y+1, x+tab_width-2, p.y+20-1, TX_ALIGN_CENTER, color_3d_text, TX_NOCOLOR, 0);
        };

        x += tab_width+3;
        pp = pp->next;
      } while ( pp != f );
    };

  };
  VIEW(o)->end_of_paint(VIEW(o), r);
};
////////////////////////////////////////////////////////////////////////////////
void tabbook_translate_event( p_object o, t_event *event ) {

  RETVIEW(o, event);

  view_translate_event(o, event);

  if ( event->type & EV_MOUSE && OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) {
	p_tabbook tb = TABBOOK(o);
	t_rect  r = VIEW(o)->get_local_extent(VIEW(o));
    t_point m = VIEW(o)->get_local_point(VIEW(o), mouse->where);
    if ( tb->tabs->last ) {
	  p_item f = tb->tabs->last->next;
	  p_item pp = f;
      l_int  x = 0;
	  do {
	    p_tab tab = TAB(pp->rec);
        l_int tab_width = get_text_length(VIEW(o)->font, tab->caption, -1)+10;
        if ( m.x > x+2 && m.x < x+tab_width-2 && m.y < 20 ) {
          if ( tab != tb->sel ) {
            VIEW(tab)->show(VIEW(tab));
            if ( tb->sel ) VIEW(tb->sel)->hide(VIEW(tb->sel));
            tb->sel = tab;
            VIEW(o)->draw_view(VIEW(o));
          };
        };
        x += tab_width+3;
        pp = pp->next;
      } while ( pp != f );
    };
    clear_event(event);
  };

};
////////////////////////////////////////////////////////////////////////////////
p_tabbook _tabbook_init ( p_tabbook o, t_rect r ) {

  if ( !o ) return NULL;

  clear_type(o,sizeof(t_tabbook));

  view_init(VIEW(o),r);

  o->tabs = list_init(malloc(sizeof(t_list)),0,0);
  o->add_tab = &tabbook_add_tab;
  o->insert = &tabbook_insert;
  o->remove = &tabbook_remove;
  o->del_tab = &tabbook_del_tab;

  VIEW(o)->draw = &tabbook_draw;
  VIEW(o)->brush.color = color_3d_face;
  OBJECT(o)->translate_event = &tabbook_translate_event;

  return o;
};
////////////////////////////////////////////////////////////////////////////////
p_tabbook tabbook_make ( t_rect r ) {
  return tabbook_init(malloc(sizeof(t_tabbook)),r);
};
