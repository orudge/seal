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
#include <trackbar.h>
#include <grfx-f.h>

p_trackbar (*trackbar_init) ( p_trackbar o, t_rect r, l_uchar style,l_long  min, l_long  max, l_long  value, l_dword message ) = &_trackbar_init;
void trackbar_translate_event(p_object o, t_event *event ) {

  if ( event->type & EV_MOUSE && OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) {
	p_trackbar tb = TRACKBAR(o);
	l_int firstvalue = tb->value;
	t_rect r = VIEW(o)->get_local_extent(VIEW(o));
	l_bool in_view = false;
    tb->pressed = true;

    VIEW(o)->draw_me(VIEW(o));

    while (OBJECT(mouse)->state & MO_SF_MOUSELPRESS) {
      l_bool old_in_view = in_view;
      t_point m = VIEW(o)->get_local_point(VIEW(o), mouse->where);
      in_view = VIEW(o)->is_mouse_in_view(VIEW(o));
      if ( in_view ) {
	    l_long oldvalue = tb->value;
        if ( !tb->style )
          tb->value = tb->min+((m.x-5)*(tb->max-tb->min)/(r.b.x-10));
        else
          tb->value = tb->min+((m.y-5)*(tb->max-tb->min)/(r.b.y-10));
        if ( tb->value < tb->min ) tb->value = tb->min;
        if ( tb->value > tb->max ) tb->value = tb->max;
        if ( tb->value != oldvalue ) VIEW(o)->draw_me(VIEW(o));
      };
      if ( in_view != old_in_view ) {
        if ( in_view )
          tb->pressed = true;
        else
          tb->pressed = false;
        VIEW(o)->draw_me(VIEW(o));
      };
      o->get_event(o, event);
    };
    tb->pressed = false;
    VIEW(o)->draw_me(VIEW(o));
    if ( tb->value != firstvalue ) {
      set_event(event, EV_MESSAGE, tb->message, o);
      o->put_event(o, event);
      clear_event(event);
    };
  };
};


/////////////////////////////////////////////////////////////
void draw_trackbar ( p_view o ) {
  t_rect r = o->get_local_extent(o);
  t_point p;
  BITMAP *out = o->begin_paint(o,&p,r);
  if ( out ) {
	p_trackbar tb = TRACKBAR(o);
	l_color l = col2col(color_3d_face,COLOR(15),1,2);
    rectfill(out,p.x,p.y,p.x+r.b.x,p.y+r.b.y,color_3d_face);
	if ( !tb->style ) {
	  l_int pos = tb->value*(r.b.x-10)/(tb->max-tb->min);

	  button(out,p.x,p.y+(r.b.y/2)-1,p.x+r.b.x,p.y+(r.b.y/2)+1,color_3d_shadow,color_3d_light);
      hline(out,p.x+1,p.y+(r.b.y/2),p.x+r.b.x-1,color_3d_dark);

      rectfill(out,p.x+pos,p.y+(r.b.y/2)-10,p.x+pos+10,p.y+(r.b.y/2)+10,tb->pressed?l:color_3d_face);
      button(out,p.x+pos,p.y+(r.b.y/2)-10,p.x+pos+10,p.y+(r.b.y/2)+10,color_3d_light,color_3d_shadow);

    } else {
	  l_int pos = tb->value*(r.b.y-10)/(tb->max-tb->min);

	  button(out,p.x+(r.b.x/2)-1,p.y,p.x+(r.b.x/2)+1,p.y+r.b.y,color_3d_shadow,color_3d_light);
      vline(out,p.x+(r.b.x/2),p.y+1,p.y+r.b.y-1,color_3d_dark);

      rectfill(out,p.x+(r.b.x/2)-10,p.y+pos,p.x+(r.b.x/2)+10,p.y+pos+10,tb->pressed?l:color_3d_face);
      button(out,p.x+(r.b.x/2)-10,p.y+pos,p.x+(r.b.x/2)+10,p.y+pos+10,color_3d_light,color_3d_shadow);


    };
  };
  o->end_of_paint(o, r);
};
/////////////////////////////////////////////////////////////
p_trackbar _trackbar_init ( p_trackbar o, t_rect r, l_uchar style,l_long  min, l_long  max, l_long  value, l_dword message ) {
  if ( !o ) return NULL;
  clear_type(o,sizeof(t_trackbar));
  view_init(VIEW(o),r);

  o->style = style;
  o->min = min(max,min);
  o->max = max(max,min);
  o->value = value;
  o->message = message;

  VIEW(o)->draw = &draw_trackbar;
  OBJECT(o)->translate_event = &trackbar_translate_event;
  return o;
};