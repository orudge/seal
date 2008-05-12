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

/* Revision History (03/04/2002):
 *
 * 03/04/2002: Fixed transparency problem with tips caused by making
 *             stattexts transparent (orudge)
 */

#include <seal.h>
#include <alltogrx.h>
#include <view.h>
#include <screen.h>

p_view  (*view_init) ( p_view o, t_rect r ) = &_view_init;


/* info board */

l_int   info_board_key = 0;

/* info board font = helv15 */
l_font *info_board_font = NULL;

/* able to drag all context of object */
l_bool  drag_context_ok = 1;
/* enable accelaration of moving */
l_bool  accel_moving_ok = 1;

l_int   move_step = 0; // Unactivated

/* size of standard item (menu, listbox, ... )*/
l_int   system_item_size = 16;

l_bool draw_to_2nd_buffer = false;

/* init info board from main ini file */
void  info_board_init ( void )
{
  info_board_key = 68;//(l_int)getininum_fromfile( INI_MAINFILE, "speed_keys", "info_board");
  info_board_font = font_system;
};

void button3d ( p_view o, BITMAP *bmp, int x1, int y1, int x2, int y2, int down ) {
  if (!down) {
	if ( o ? o->brush.state & BRUSH_LARGE3D : 1 ) {
      hline(bmp, x1, y1, x2, color_3d_light);
      vline(bmp, x1, y1, y2, color_3d_light);
      vline(bmp, x2-1, y1+1, y2-1, color_3d_shadow);
      hline(bmp, x1+1, y2-1, x2-1, color_3d_shadow);
      vline(bmp, x2, y1, y2, color_3d_dark);
      hline(bmp, x1, y2, x2, color_3d_dark);
    } else if ( o->brush.state & BRUSH_LIGHT3D ) {
	  button(bmp,x1,y1,x2,y2,color_3d_light,color_3d_shadow);
    } else if ( o->brush.state & BRUSH_FLAT ) {
	  rect(bmp,x1,y1,x2,y2,color_flat_border);
    };
  } else {
	if ( o ? o->brush.state & BRUSH_LARGE3D : 1 ) {
      hline(bmp, x1, y1, x2, color_3d_dark);
      vline(bmp, x1, y1, y2, color_3d_dark);
      hline(bmp, x1+1, y1+1, x2-1, color_3d_shadow);
      vline(bmp, x1+1, y1+1, y2-1, color_3d_shadow);
      vline(bmp, x2-1, y1+1, y2-1, color_3d_face);
      hline(bmp, x1+1, y2-1, x2-1, color_3d_face);
      vline(bmp, x2, y1, y2, color_3d_light);
      hline(bmp, x1, y2, x2, color_3d_light);
    } else if ( o->brush.state & BRUSH_LIGHT3D ) {
	  button(bmp,x1,y1,x2,y2,color_3d_shadow,color_3d_light);
    } else if ( o->brush.state & BRUSH_FLAT ) {
	  rect(bmp,x1,y1,x2,y2,color_flat_border);
    };
  }
}


/* init views from main ini file */
void  view_ini ( void )
{
   info_board_init();

   //drag_context_ok = get_key_integer("current/seal/optical/move_window_context");
   //accel_moving_ok = get_key_integer("current/seal/optical/enable_move_acceleration");
};


void  view_reset_prefer ( p_object o )
{
  p_object p = o->last_view(o);
  p_object x = NULL;

  if ( p ) {

    x = p->find_match_view(p, OB_SF_VISIBLE, OB_OF_SELECTABLE, true);

    if ( x ) o->set_prefer(o, x);

  };

  if ( !x ) obj_reset_prefer(o);

};




l_bool    view_done ( p_object o )
{
  if ( obj_exist(o) <= 0 ) return false;

  VIEW(o)->hide(VIEW(o));

  /* background releasing */
  if ( VIEW(o)->brush.background && VIEW(o)->brush.state & BRUSH_SELFIMG ) {

      destroy_bitmap(VIEW(o)->brush.background);

      VIEW(o)->brush.background = NULL;
      VIEW(o)->brush.state &= ~BRUSH_SELFIMG;

  };

  /* each object has own palette
  #ifdef __EACH_OWN_PALETTE__
  if ( o->palette ) afree((void**)(&o->palette));
  #endif */

  afree((void**)(&(VIEW(o)->info_text)));

  return obj_done(o);
};


#include<stattext.h>
/*
  show info board with text (text)
*/
void  _show_info_board ( p_view from, l_text text ) {
  p_view  o;
  t_point where;
  t_point s;
  p_stattext st;
  t_rect  r;
  t_point p;

  if ( !text || !is_top_view_under_mouse_in_me(from) )  return;

  o = from->top_view(from);

  where = mouse->where;

  r = rect_assign(where.x, where.y, where.x, where.y);

  p = o->get_global_point(o, r.a);

  get_size_of_ftext(text, info_board_font, (int*)(&r.b.x), (int*)(&r.b.y));

  r.a.y = r.a.y - r.b.y - 4;

  r.b.x += r.a.x+4;
  r.b.y += r.a.y+4;

  s = point_assign(rect_sizex(r), rect_sizey(r));

  if ( r.b.x > SCREEN_W ) {

    r.b.x = SCREEN_W;
    r.a.x = SCREEN_W-s.x;

  };

  if ( r.a.x < 0 ) {

    r.a.x = 0;
    r.b.x = s.x;

  };

  if ( r.b.y > SCREEN_H ) {

    r.b.y = SCREEN_H;
    r.a.y = SCREEN_H-s.y;

  };

  if ( r.a.y < 0 ) {

    r.a.y = 0;
    r.b.y = s.y;

  };

  st = stattext_init(_malloc(sizeof(t_stattext)),r,TX_ALIGN_CENTER,"%s",text);
  VIEW(st)->brush.state &= ~BRUSH_LARGE3D;
  VIEW(st)->brush.state |= BRUSH_DRAWBORDER|BRUSH_FLAT;
  VIEW(st)->brush.color = color_flat_face;
  VIEW(st)->transparent = false; // Never make this transparent
  OBJECT(st)->options |= OB_OF_TOPSELECT;
  st->color_text = color_flat_text;
  OBJECT(desktop)->insert(OBJECT(desktop),OBJECT(st));

  while ( KEYCTRL(KB_F1) || !event_main.type ) {

      OBJECT(from)->get_event(OBJECT(from), &event_main);

  };

  dispose(OBJECT(st));

};


void  view_show_info_board ( p_view from )
{

   if ( from->info_text ) _show_info_board(from, from->info_text);

};


p_view  view_top_view ( p_view o )
{

  while ( OBJECT(o)->owner_view(OBJECT(o)) )

    o = VIEW(OBJECT(o)->owner_view(OBJECT(o)));

  return o;

};


void    view_set_mouse_cursor ( p_view o )
{

  if ( o->cursor >= 0 )  mouse_set_cursor_focus_id(o->cursor);

};


l_bool  view_is_mouse_in_view ( p_view o )
{
  t_point p = o->get_local_point(o, mouse->where);
  if ( p.x < 0 || p.x > rect_sizex(o->bounds) ||
       p.y < 0 || p.y > rect_sizey(o->bounds) ) return false;
  return true;
};


p_view  view_get_view_under_mouse ( p_view o )
{

  p_view x = VIEW(OBJECT(o)->first_view(OBJECT(o)));
  p_view f = x;

  t_point where = o->get_local_point(o, mouse->where);

  if ( x )
    do {

      if ( OBJECT(x)->is_state(OBJECT(x), OB_SF_VISIBLE) ) /* is visible ? */

        if ( rect_contains(x->bounds, where) )

          return x;

      x = VIEW(OBJECT(x)->next_view(OBJECT(x)));

    } while ( x != f );

  return NULL;

};


p_view  view_get_top_view_under_mouse ( p_view o )
{

  p_view x = VIEW(OBJECT(o)->first_view(OBJECT(o)));
  p_view f = x;

  t_point where = o->get_local_point(o, mouse->where);

  if ( x )
    do {

      if ( OBJECT(x)->is_state(OBJECT(x), OB_SF_VISIBLE) ) /* is visible ? */

        if ( rect_contains(x->bounds, where) ) {

            return x->get_top_view_under_mouse(x);

        };

      x = VIEW(OBJECT(x)->next_view(OBJECT(x)));

    } while ( x != f );

  if ( rect_contains(o->get_local_extent(o), where) ) return o;

  return NULL;

};


l_bool  is_top_view_under_mouse_in_me ( p_view o )
{
    p_view tv = VIEW(o)->top_view(VIEW(o));
    p_view t  = tv->get_top_view_under_mouse(tv);

    return is_my_object(OBJECT(o), OBJECT(t));
};


void       view_play_process ( p_object o, t_event *event )
{
  if ( !o ) return;

  if ( !IS_ACTIVE_PROCESS(o) || o->is_options(o, OB_OF_NOTACTIVATE) ||
       EV_IGNORE_ACTIVE_PROCESS(event->type)
  ) {

    if ( !o->is_options(o, OB_OF_STILLPROCESS) &&
          event->type & EV_MOUSE &&
         !is_top_view_under_mouse_in_me(VIEW(o)) ) return;

    ACTIVE_PROCESS(o);

    event_stop = o;

    o->translate_event(o, event);

    PASSIVE_PROCESS(o);

  };

};



void    view_translate_event ( p_object o, t_event *event )
{

  if ( event->type & EV_MOUSE ) {

    p_view v  = VIEW(o)->get_view_under_mouse(VIEW(o));
    p_view t  = VIEW(o)->get_top_view_under_mouse(VIEW(o));

     if ( t )

        t->set_mouse_cursor(t);

     if ( v && IS_OKTOSUBPROCESS(v) ) {

       if ( !t ) t = v;

       /* drag & drop data controls... */
         if ( OBJECT(mouse)->state & MO_SF_MOUSEDRAG ) {  /* drag data */

          t->drag_data(t);

          clear_event(event);

       };

       /* ...end of drag & drop data controls */

       PLAY_PROCESS(OBJECT(v), event);

     };

  } else

    obj_translate_event(o, event);


  /*if ( OBJECT(keyb)->state & KB_SF_KEYDOWN && KEYCTRL(KB_F1) )

      VIEW(o)->show_info_board(VIEW(o));*/

};


void    view_setup ( p_object o )
{

  VIEW(o)->reset_align(VIEW(o));

  VIEW(o)->show(VIEW(o));

};


void    view_after_init ( p_object o )
{


};


void    view_put_in_front_of ( p_object o, p_object before )
{
  if ( o->owner ) {

    p_object owner = o->owner;
    owner->set_prefer(owner, o);

    if ( o != before ) {

      VIEW(o)->draw_overlays(VIEW(o));
      owner->remove(owner, o);
      owner->insert_before(owner, o, before);
      owner->reset_prefer(owner);

    };
  };
};


void    view_set_state ( p_object o, l_dword st, l_bool set )
{
  p_view own = VIEW(o->owner_view(o));

  if ( !(st & OB_SF_VISIBLE) || !set || o->is_options(o, VW_OF_VISIBILITY) )

    obj_set_state(o, st, set); /* object function */

  /* OB_SF_VISIBLE */

  if ( st & OB_SF_VISIBLE &&
      (!own || OBJECT(own)->is_state(OBJECT(own), OB_SF_VISIBLE)) )
  {

    if ( o->owner && o->is_options(o, OB_OF_SELECTABLE) )
      if ( set ) {
         if ( !o->owner->prefer || o->is_options(o, OB_OF_TOPSELECT) )
            o->owner->reset_prefer(o->owner);
      } else if ( o->is_state(o, OB_SF_SELECTED) ) o->owner->reset_prefer(o->owner);

    if ( set ) {
	  l_bool old = VIEW(o)->is_draw_mode(VIEW(o),DWM_DONTDRAWBACK);
	  VIEW(o)->set_draw_mode(VIEW(o),DWM_DONTDRAWBACK,true);
      VIEW(o)->draw_view(VIEW(o));
      VIEW(o)->set_draw_mode(VIEW(o),DWM_DONTDRAWBACK,old);
    } else
      VIEW(o)->draw_under_view(VIEW(o));
  };

  if ( st & OB_SF_VISIBLE )

    VIEW(o)->for_each_sub_view_set_state(VIEW(o), OB_SF_VISIBLE, set);

};


void  view_reset_align ( p_view o )
{
  t_rect r = o->bounds;
  p_view ov = VIEW(OBJECT(o)->owner_view(OBJECT(o)));

  if ( ov ) {

    t_rect s = ov->get_limits(ov);

    if ( o->align & TX_ALIGN_CENTERX )

      r.a.x = s.a.x+(rect_sizex(s)-rect_sizex(r))/2;

    if ( o->align & TX_ALIGN_CENTERY )

      r.a.y = s.a.y+(rect_sizey(s)-rect_sizey(r))/2;

    if ( o->align & TX_ALIGN_LEFT )

      r.a.x = s.a.x;

    if ( o->align & TX_ALIGN_TOP )

      r.a.y = s.a.y;

    r.b.x = r.a.x+rect_sizex(o->bounds);
    r.b.y = r.a.y+rect_sizey(o->bounds);

    o->set_bounds(o, r);

  };

};


l_dword   view_execute_view ( p_view o, p_view sub )
{

  l_dword    message = MSG_NOTHING;
  if ( sub ) {

    l_dword     poptions = OBJECT(sub)->options;
    l_dword     pstate   = OBJECT(sub)->state;
    p_object    owner    = OBJECT(OBJECT(sub)->owner_view(OBJECT(sub)));
    p_object    prefer   = OBJECT(o)->prefer;
    l_bool      prefmodal = false;

    OBJECT(sub)->set_options(OBJECT(sub), OB_OF_TOPSELECT, true);
    OBJECT(sub)->set_state(OBJECT(sub), OB_SF_MODAL, true);

    if ( !owner ) OBJECT(o)->insert(OBJECT(o), OBJECT(sub));

    OBJECT(o)->set_prefer(OBJECT(o), OBJECT(sub));
    message = OBJECT(sub)->execute(OBJECT(sub));
    if ( !owner ) {

      dispose(OBJECT(sub));
      sub = NULL;

    };

    if ( sub ) {

      OBJECT(sub)->options = poptions;
      OBJECT(sub)->state = pstate;

    };


    OBJECT(o)->set_prefer(OBJECT(o), prefer);

  };

  return message;
};


p_data  view_drag_data ( p_view o )
{
  clear_type(&clipboard, sizeof(t_data));

  if ( l_tag_cmp(OBJECT(o)->data_type, TAG_NONE) )

     return NULL;

  clipboard.style = DS_WHATEVER;

  if ( OBJECT(o)->get_data(OBJECT(o), &clipboard) ) {

    p_view t  = NULL;

    mouse_set_cursor_focus_id(CUR_DRAG);

    while ( OBJECT(mouse)->state & MO_SF_MOUSELPRESS ) {

      p_view oldt = NULL;
      p_view tv = VIEW(o)->top_view(VIEW(o));
      t  = tv->get_top_view_under_mouse(tv);

      OBJECT(o)->get_event(OBJECT(o), &event_main);

      if ( t && OBJECT(mouse)->state & MO_SF_MOUSEMOVE ) {



          t->drag_where(t, &clipboard, t->get_local_point(t, mouse->where));

          if ( oldt ) {

              oldt->drag_where(oldt, &clipboard, oldt->get_local_point(oldt, mouse->where));

          };

      };

      oldt = t;

    };

    if ( t ) {

         t->drag_where(t, NULL, t->get_local_point(t, mouse->where));

    };

    if ( o ) {

         o->drag_where(o, NULL, o->get_local_point(o, mouse->where));

    };

    if ( t ) {

         set_event(&event_main, EV_MESSAGE, MSG_PASTE, OBJECT(t));

         OBJECT(o)->put_event(OBJECT(o), &event_main);

    };

    clear_event(&event_main);

    mouse_set_cursor_focus_id(CUR_ARROW);

    return (&clipboard);

  };

  return NULL;

};


/*
   - this function is called even, when data is droped under the object
   - where is local point to object in rectangle
          (0,0, rect_sizex(o->bounds), rect_sizey(o->bounds))
   - rec is pointer to t_data structure and contains information about
     data that comes from draging object.
   - when rec = NULL, it means it's end of draging so take things to right
     place. if mouse(where) is not under me, it means that mouse was under me
     before.

   - standard structure of function is :

   if ( view_drag_where(o, rec, where) ) {  is mouse under view

       return true;

   } else  mouse is not under view or end

   if ( OBJECT(o)->is_state(OBJECT(o), OB_SF_FOCUSED) ) {

       if ( rec )  mouse is not under view



       else  end of dragging - give things to right place


   } else {  mouse is not under view or end but in non-focused view


   };

   return false;
*/

l_bool  view_drag_where ( p_view o, p_data rec, t_point where )
{
  if ( rec && rect_contains(o->get_local_extent(o), where) ) {

     if ( l_tag_cmp(OBJECT(o)->data_type, rec->id) &&
          !is_active(OBJECT(o)) ) { /* not active process me + under me */

         mouse_set_cursor_focus_id(CUR_DRAG); /* set drag cursor */

         return true;

     } else { /* my data_type not include rec->id */

        mouse_set_cursor_focus_id(CUR_STOP); /* set stop cursor */

        return false;

     };

  } else

     return false;
};


/*

  - where is local point

  - not used, it's for compatibility
*/

l_bool  view_drop_data ( p_view o, p_data rec, t_point where )
{
   return false;
};


void   view_drag_view ( p_view o, l_word mode, t_event *event )
{
  t_rect safebounds = o->bounds;
  t_rect r = safebounds;

  if ( o->drag_mode & mode ) {

    STOP_PROCESS();

    if ( mode & DM_DRAGMOVE ) mouse_set_cursor_focus_id(CUR_MOVE);
    if ( mode & DM_DRAGGROW ) mouse_set_cursor_focus_id(CUR_GROW);

    r = o->drag(o, mode, event);

    START_PROCESS();

    o->change_bounds(o, r);
  };
};


t_rect  view_drag ( p_view o, l_word mode, t_event *event )
{
 /*t_point delta;
  t_rect  box = o->bounds;
  p_view  owner = VIEW(OBJECT(o)->owner_view(OBJECT(o)));

  if ( event->type & EV_MOUSE && owner )  {

    t_rect limits = mouse->get_range(mouse);

    t_rect  b = owner->get_global_bounds(owner, o->get_limits(o));

    t_point fc = owner->get_local_point(owner, mouse->where);

    t_point change;

    if ( _mode & DM_DRAGGROW ) _mode &= ~DM_DRAGCONTEXT;

   if ( !(_mode & DM_DRAGCONTEXT) )

      if ( !_mode & DM_DRAGMOVE )

         mouse_set_mode(mouse, MO_MO_RECT, COLOR(CO_LIGHTGRAY), o->get_global_bounds(o, o->get_local_extent(o)));

      else

      if ( _mode & DM_DRAGGROW ) {

         t_point p = o->get_global_point(o, o->size_minimum(o));

         mouse_set_mode(mouse, MO_MO_GROW, COLOR(CO_LIGHTGRAY), o->get_global_bounds(o, o->get_local_extent(o)));
      };


    mouse->set_range(mouse, b);

    if ( _mode & DM_DRAGCONTEXT )

          mouse_set_mode(mouse, 0, 0, rect_empty);

    while ( event->type & EV_MOUSE && !(OBJECT(mouse)->state & MO_SF_MOUSEUP) )
    {

      OBJECT(o)->get_event(OBJECT(o), event);

      change = owner->get_local_point(owner, mouse->where);

      if ( _mode & DM_DRAGCONTEXT && !_mode & DM_DRAGGROW) {

         change.x -= fc.x; // delta X change
         change.y -= fc.y; // delta Y change

         box = rect_assign(box.a.x+change.x, box.a.y+change.y, box.b.x+change.x, box.b.y+change.y);

         o->change_bounds(o, box);

         fc = owner->get_local_point(owner, mouse->where);

      };

    };

    change = owner->get_local_point(owner, mouse->where);

    change.x -= fc.x; // delta X change
    change.y -= fc.y; // delta Y change

    if ( _mode & DM_DRAGMOVE )

      box = rect_assign(box.a.x+change.x, box.a.y+change.y, box.b.x+change.x, box.b.y+change.y);

    else

    if ( _mode & DM_DRAGGROW )

      box = rect_assign(o->bounds.a.x, o->bounds.a.y, box.b.x+change.x, box.b.y+change.y);

    mouse_set_mode(mouse, 0, 0, rect_empty);
    mouse->set_range(mouse, limits);
  };

  return box;*/
 static t_rect box; // we pass this struct back, make sure it still there after we leave
 box.a.x= o->bounds.a.x;
 box.a.y= o->bounds.a.y;
 box.b.x= o->bounds.b.x;
 box.b.y= o->bounds.b.y;

 if ( event->type & EV_MOUSE )  {
  if (!(mode & DM_DRAGGROW)){
    l_int sx,sy,fcx=mouse_x,fcy=mouse_y;
    if ( move_step ) {
      fcx = (fcx/move_step)*move_step;
      fcy = (fcy/move_step)*move_step;
    };
    while ( !(OBJECT(mouse)->state & MO_SF_MOUSEUP) ) {
      OBJECT(o)->get_event(OBJECT(o), event);
      sx = mouse_x;sy = mouse_y;
      if ( move_step ) {
        sx = (sx/move_step)*move_step;
        sy = (sy/move_step)*move_step;
        if ( (sx-fcx) || (sy-fcy) ) {
          box.b.x+=sx-fcx;
          box.b.y+=sy-fcy;
          box.a.x+=sx-fcx;
          box.a.y+=sy-fcy;
          fcx=sx;fcy=sy;
          o->change_bounds(o, box);
        };

      } else if ( (sx-fcx) || (sy-fcy) ) {
       box.b.x+=sx-fcx;
       box.b.y+=sy-fcy;
       box.a.x+=sx-fcx;
       box.a.y+=sy-fcy;
       fcx=sx;fcy=sy;
       o->change_bounds(o, box);
      }
    }
  } else {
    l_int sx,sy,fcx=mouse_x,fcy=mouse_y; // record starting mouse position
    if ( move_step ) {
      fcx = (fcx/move_step)*move_step;
      fcy = (fcy/move_step)*move_step;
    };
    while ( !(OBJECT(mouse)->state & MO_SF_MOUSEUP) ) {
      OBJECT(o)->get_event(OBJECT(o), event);
      sx = mouse_x;sy = mouse_y; // change= current mouse pos - prior mouse pos
      if ( move_step ) {
        sx = (sx/move_step)*move_step;
        sy = (sy/move_step)*move_step;
        if ( (sx-fcx) || (sy-fcy) ) {
          box.b.x+=sx-fcx;
          box.b.y+=sy-fcy;
          fcx=sx;fcy=sy;
          o->change_bounds(o, box);
        };
      } else
      if( (sx-fcx) || (sy-fcy) ){
       box.b.x+=sx-fcx;box.b.y+=sy-fcy; // alter size
       fcx=sx;fcy=sy;
       o->change_bounds(o, box);
      }
    }
  }
 }
 return box;

};


t_rect  view_get_limits ( p_view o )
{
  p_view owner = VIEW(OBJECT(o)->owner_view(OBJECT(o)));

  if ( !owner ) return o->bounds;

  if ( OBJECT(o)->is_options(OBJECT(o), VW_OF_IGNORELIM) )

    return owner->get_local_extent(owner);

  return owner->size_limits(owner);
};


t_point  view_size_minimum ( p_view o )
{

  return point_assign(1, 1);

};


void    view_for_each_sub_view_set_state ( p_view o, l_dword st, l_bool set )
{

  o = VIEW(OBJECT(o)->last_view(OBJECT(o)));

  while ( o ) {

    OBJECT(o)->set_state(OBJECT(o), st, set);

    o = VIEW(OBJECT(o)->prev_view_to_first(OBJECT(o)));

  };

};


void    view_lock_drawing ( p_view o )
{

  o->draw_lock++;

};


l_bool  view_unlock_drawing ( p_view o )
{

  o->draw_lock--;

  if ( o->draw_lock <= 0 ) {

    o->draw_lock = 0;

  };

  return o->is_draw_lock(o);
};


l_bool  view_is_draw_lock ( p_view o )
{

  while ( o ) {

    if ( o->draw_lock ) return true;

    o = VIEW(OBJECT(o)->owner_view(OBJECT(o)));
  };

  return false;

};


void   view_draw_overlays ( p_view o )
{
  p_view owner = VIEW(OBJECT(o)->owner_view(OBJECT(o)));
  p_view p;
  l_long nobj;
  l_long safe;
  l_dword *states;

  if ( !owner ) return;

  p = VIEW(OBJECT(owner)->first_view(OBJECT(owner)));

  nobj = OBJECT(owner)->index_of(OBJECT(owner), OBJECT(o));
  safe = nobj;

  states = (l_dword*)_calloc(nobj, sizeof(l_dword));

  if ( !states ) return;

  while ( p != o && nobj ) {
    nobj--;
    states[nobj] = OBJECT(p)->state;
    OBJECT(p)->state &= ~OB_SF_VISIBLE;
    p = VIEW(OBJECT(p)->next_view(OBJECT(p)));
  };

  p = VIEW(OBJECT(owner)->first_view(OBJECT(owner)));

  while ( p != o ) {
    t_rect r = rect_cliped(o->bounds, p->bounds);

    if ( !rect_check_empty(r) ) {

      t_rect safeclip = o->clip;

      o->clip = rect_move(r, -o->bounds.a.x, -o->bounds.a.y);
      o->draw_me(o);
      o->clip = safeclip;

    };

    p = VIEW(OBJECT(p)->next_view(OBJECT(p)));
  };

  p = VIEW(OBJECT(owner)->first_view(OBJECT(owner)));
  nobj = safe;

  while ( p != o && nobj ) {

    t_rect r = rect_cliped(o->bounds, p->bounds);

    nobj--;

    if ( !rect_check_empty(r) ) {

      t_rect safeclip = o->clip;

      o->clip = rect_move(r, -o->bounds.a.x, -o->bounds.a.y);
      o->draw_sub_views(o, VIEW(OBJECT(o)->first_view(OBJECT(o))), VIEW(OBJECT(o)->first_view(OBJECT(o))));
      o->clip = safeclip;

    };

    OBJECT(p)->state = states[nobj];
    p = VIEW(OBJECT(p)->next_view(OBJECT(p)));
  };

  _cfree(states);
};



void  view_draw ( p_view o )
{
  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out )
    o->background(o, out, rect_move(r, p.x, p.y));

  o->end_of_paint(o, r);
};


void  view_draw_me ( p_view o ) {
  l_bool dwm = o->is_draw_mode(o, DWM_TESTSUBVIEWS);
  o->set_draw_mode(o, DWM_TESTSUBVIEWS, true);
  o->draw(o);
  o->set_draw_mode(o, DWM_TESTSUBVIEWS, dwm);
};


void  view_draw_view ( p_view o ) {
  if ( OBJECT(o)->is_state(OBJECT(o), OB_SF_VISIBLE) ) {
    o->draw(o);
    o->draw_sub_views(o, VIEW(OBJECT(o)->first_view(OBJECT(o))), VIEW(OBJECT(o)->first_view(OBJECT(o))));
  };
};


void  view_draw_sub_views ( p_view o, p_view from, p_view to )
{
   p_view s = from;
   p_view f = VIEW(OBJECT(o)->first_view(OBJECT(o)));

   if ( !f ) return;

   if ( !to ) to = from;

   if ( from && OBJECT(o)->is_state(OBJECT(o), OB_SF_VISIBLE) ) {
	 l_bool old = o->is_draw_mode(o,DWM_DRAWSUBVIEW);


	 o->set_draw_mode(o,DWM_DRAWSUBVIEW,true);


     from = s;
     do {   /* draw only owner views */

       if ( rect_overlay(o->clip, from->bounds) ) {
         t_rect old = from->clip;
         from->clip = rect_cliped(o->clip,from->bounds);
         from->clip = rect_move(from->clip,-from->bounds.a.x,-from->bounds.a.y);
         from->draw(from);
         from->clip = old;
       };

       from = VIEW(OBJECT(from)->next_view(OBJECT(from)));

     } while ( from != to && from != f );

     from = s;

     do {   /* draw only sub-views */
       if ( rect_overlay(o->clip, from->bounds) ) {
	     t_rect *old = malloc(sizeof(t_rect));
	     if ( old ) {
		   memcpy(old,&from->clip,sizeof(t_rect));
		   from->clip = rect_cliped(o->clip,from->bounds);
		   from->clip = rect_move(from->clip,-from->bounds.a.x,-from->bounds.a.y);
	     };
         from->draw_sub_views(from, VIEW(OBJECT(from)->first_view(OBJECT(from))),
                                    VIEW(OBJECT(from)->first_view(OBJECT(from))));
         if ( old ) {
	       memcpy(&from->clip,old,sizeof(t_rect));
	       free(old);
         };
        };
       from = VIEW(OBJECT(from)->next_view(OBJECT(from)));

     } while ( from != to && from != f );
     o->set_draw_mode(o,DWM_DRAWSUBVIEW,old);
   };
};


void    view_draw_under_rect ( p_view o, t_rect r )
{
  p_view own = VIEW(OBJECT(o)->owner_view(OBJECT(o)));
  if ( own && OBJECT(own)->is_state(OBJECT(own), OB_SF_VISIBLE) ) {

    p_view f = VIEW(OBJECT(o)->next_view(OBJECT(o)));

    l_dword dwm = own->is_draw_mode(own, DWM_TESTSUBVIEWS);
    t_rect  olc = own->clip;

    own->clip = r;
    own->set_draw_mode(own, DWM_TESTSUBVIEWS, true);
    own->draw(own);
    own->set_draw_mode(own, DWM_TESTSUBVIEWS, dwm);

    if ( f != VIEW(OBJECT(own)->first_view(OBJECT(own))) ) {
      own->draw_sub_views(own, o->is_draw_mode(o,OB_SF_DRAWBACK) ? VIEW(OBJECT(own)->first_view(OBJECT(own))) : f, VIEW(OBJECT(own)->first_view(OBJECT(own))));
    };


    //own->begin_paint(own, NULL, r);

    //own->end_of_paint(own, r);


    own->clip = olc;

  };
};

void   view_draw_overlays_rect ( p_view o, t_rect on ) {
  if ( OBJECT(o)->state & OB_SF_VISIBLE && !o->is_draw_mode(o,DWM_DONTDRAWBACK) ) {
    t_point p = o->get_global_point(o, point_assign(0, 0));
    l_bool xdraw = screen_double_buffer && !draw_to_2nd_buffer;
    OBJECT(o)->state &= ~OB_SF_VISIBLE;
    if ( xdraw ) {
      set_clip(screen_double_buffer,p.x+on.a.x,p.y+on.a.y,p.x+on.b.x,p.y+on.b.y);
      draw_to_2nd_buffer++;
    };
    view_draw_under_rect(o,rect_move(on,o->bounds.a.x,o->bounds.a.y));
    if ( xdraw ) {
      draw_to_2nd_buffer--;
      if ( draw_to_2nd_buffer < 0 ) draw_to_2nd_buffer=0;
      set_clip(o->draw_buffer,p.x+on.a.x,p.y+on.a.y,p.x+on.b.x,p.y+on.b.y);
      blit(screen_double_buffer,o->draw_buffer,p.x+on.a.x,p.y+on.a.y,p.x+on.a.x,p.y+on.a.y, rect_sizex(on)+1,rect_sizey(on)+1);
      set_clip(screen_double_buffer,0,0,screen_double_buffer->w-1,screen_double_buffer->h-1);
    } else {
      set_clip(o->draw_buffer,p.x+on.a.x,p.y+on.a.y,p.x+on.b.x,p.y+on.b.y);
      blit(draw_to_2nd_buffer?screen_double_buffer:OUTBUFFER(o->draw_out),o->draw_buffer,
            p.x+on.a.x,p.y+on.a.y,p.x+on.a.x,p.y+on.a.y, rect_sizex(on)+1,rect_sizey(on)+1);
    };
    OBJECT(o)->state |= OB_SF_VISIBLE;
  };
};

void    view_draw_on_rect ( p_view o, t_rect r ) {
  if ( o && OBJECT(o)->is_state(OBJECT(o), OB_SF_VISIBLE) ) {
    t_rect  olc = o->clip;
    o->clip = r;
    o->draw_sub_views(o, VIEW(OBJECT(o)->first_view(OBJECT(o))), VIEW(OBJECT(o)->first_view(OBJECT(o))));
    o->clip = olc;
  };

};

void    view_draw_under_view ( p_view o )
{
  o->draw_under_rect(o, o->bounds);
};


void    view_draw_in_rect ( p_view o, t_rect r )
{

  o->clip = r;
  o->draw_view(o);
  o->clip = o->get_local_extent(o);

};

l_bool  view_draw_mini_box ( p_view o, BITMAP *out, BITMAP *buffer, t_rect rwhere )
{
  t_view *p = o;
  l_long  rnum = 1;
  t_rect* r = NULL;
  t_rect* rout = NULL;
  t_rect  clip = o->get_cliped_extent(o);
  l_bool  isscr = is_same_bitmap(out, screen);
  if ( !rect_check_empty(clip) && OBJECT(o)->is_state(OBJECT(o), OB_SF_VISIBLE) && out ) { /* object is visible and output bitmap is set */

    t_rect* r = o->get_not_overlays ( o, rwhere, rect_move(rwhere, o->bounds.a.x, o->bounds.a.y),
                                      o, p, &rnum, &rout, o->is_draw_mode(o, DWM_TESTSUBVIEWS), false);


    if ( r ) { /* rect exist */

      t_point globe = o->get_global_point(o, point_assign(0, 0));

      while ( !rect_check_empty(*r) ) {

        t_rect d = rect_move(*r, globe.x, globe.y);
        l_int  i = 0;

        o->set_clips(o, *r);

        if ( isscr ) i = mouse->block(mouse, d);

        if ( o->trans_effect == 255 )
          blit(buffer, out, d.a.x, d.a.y, d.a.x, d.a.y, rect_sizex(d)+1, rect_sizey(d)+1);
        else {
	      if ( isscr && screen_double_buffer ) {
		    trans_blit(buffer, screen_double_buffer, d.a.x, d.a.y, d.a.x, d.a.y, rect_sizex(d)+1, rect_sizey(d)+1,o->trans_effect,255);
		    blit(screen_double_buffer,out, d.a.x, d.a.y, d.a.x, d.a.y, rect_sizex(d)+1, rect_sizey(d)+1);
	      } else
            trans_blit(buffer, out, d.a.x, d.a.y, d.a.x, d.a.y, rect_sizex(d)+1, rect_sizey(d)+1,o->trans_effect,255);
        };

        if ( isscr ) mouse->unblock(mouse, i);

        o->reset_clips(o);

        r++;
      };


      sf_free(rout);
      return true;

    };

  };

  return false;
};


l_color view_get_color ( p_view o, l_int cpos )
{
  l_color  *color = o->palette;
  if ( color ) return color[cpos];
  return 0;
};


l_bool view_is_top_view ( p_view o )
{
  p_view p = o;
  p_view t = NULL;

  while ( p ) {

    t = VIEW(OBJECT(p)->owner_view(OBJECT(p)));

    if ( t && OBJECT(t)->first_view(OBJECT(t)) != OBJECT(p) ) return false;

    p = VIEW(OBJECT(p)->owner_view(OBJECT(p)));
  };

  return true;
};

void  view_move_accel ( p_view o, t_rect from, t_rect to )
{
  p_view p = VIEW(OBJECT(o)->owner_view(OBJECT(o)));
  t_rect clip;

  t_rect xfrom;
  t_rect xto;
  l_int  x;
  l_int  y;
  l_int  nx;
  l_int  ny;

  l_int  i = 0;

  t_rect  d;
  t_point pt;

  if ( !p ) return;

  clip = rect_cliped(p->get_cliped_extent(p), o->get_limits(o));

  xfrom = rect_cliped(from, clip);
  xto = rect_cliped(to, clip);
  x  = (from.a.x - to.a.x)+1;
  y  = (from.a.y - to.a.y)+1;
  nx = (from.a.x > to.a.x)?1:0;
  ny = (from.a.y > to.a.y)?1:0;

  xfrom = rect_move(xfrom, ((xfrom.a.x > xto.a.x) ? -x : (to.b.x - from.b.x)-nx),
                           ((xfrom.a.y > xto.a.y) ? -y : (to.b.y - from.b.y)-ny));

  p->set_clips(p, clip);

  pt = p->get_global_point(p, point_assign(0, 0));

  /* blit function */
  d = rect_assign(pt.x+to.a.x, pt.y+to.a.y, pt.x+to.b.x, pt.y+to.b.y);

  i = mouse->block(mouse, d);

  blit(OUTBUFFER(o->draw_out), OUTBUFFER(o->draw_out), pt.x+from.a.x, pt.y+from.a.y, pt.x+to.a.x, pt.y+to.a.y, rect_sizex(from)+1, rect_sizey(from)+1);

  mouse->unblock(mouse, i);

  p->reset_clips(p);

  rect_double_overlay(&xfrom, &xto);

  if ( !rect_check_empty(xfrom) ) {

    xfrom = rect_move(xfrom, -o->bounds.a.x, -o->bounds.a.y);

    o->draw_in_rect(o, xfrom);

  };

  if ( !rect_check_empty(xto) ) {

    xto = rect_move(xto, -o->bounds.a.x, -o->bounds.a.y);

    o->draw_in_rect(o, xto);

  };

};

void  view_move_view ( p_view o, t_rect r ) {
  t_rect safebounds = o->bounds;

  if ( !rect_equals(r, safebounds) &&
        rect_sizex(r) == rect_sizex(safebounds) &&
        rect_sizey(r) == rect_sizey(safebounds) ) {

      o->set_bounds(o, r);

      if ( !o->is_top_view(o) || !OBJECT(o)->owner_view(OBJECT(o)) ||
            o->is_draw_mode(o, DWM_CANTACCELMOVE) || o->transparent )

        o->draw_view(o);

      else

        o->move_accel(o, safebounds, r);

        o->draw_under_rect(o, safebounds);

    };
};


void  view_grow_view ( p_view o, t_rect r )
{
  p_object p = OBJECT(o)->first_view(OBJECT(o));
  p_object f = p;
  p_view own = VIEW(OBJECT(o)->owner);
  t_point pt;
  t_rect sb = o->bounds;
  t_rect ff;

  if ( screen_double_buffer ) {
    if ( !draw_to_2nd_buffer ) {
      l_int i = 0;
      pt = own->get_global_point(own, point_assign(0, 0));
      ff = rect_assign(pt.x+min(r.a.x,sb.a.x),pt.y+min(r.a.y,sb.a.y),pt.x+max(r.b.x,sb.b.x),pt.y+max(r.b.y,sb.b.y));
      set_clip(screen_double_buffer,ff.a.x,ff.a.y,ff.b.x,ff.b.y);
      i = mouse->block(mouse, ff);
      blit(screen,screen_double_buffer,ff.a.x,ff.a.y,ff.a.x,ff.a.y, rect_sizex(ff)+1,rect_sizey(ff)+1);
      mouse->unblock(mouse, i);
    };
    draw_to_2nd_buffer++;
  };

  o->set_bounds(o, r);
  o->draw(o);

  if ( p )

    do {

         p_view v = VIEW(p);
         t_rect r = v->bounds;

         l_rect dx = rect_sizex(o->bounds) - rect_sizex(sb);
         l_rect dy = rect_sizey(o->bounds) - rect_sizey(sb);

         if ( v->align & TX_ALIGN_RIGHT ) {

               if ( v->align & TX_ALIGN_FIXEDX )  r.a.x += dx;

               r.b.x += dx;

         };

         if ( v->align & TX_ALIGN_BOTTOM )  {

               if ( v->align & TX_ALIGN_FIXEDY )  r.a.y += dy;

               r.b.y += dy;
         };

         if ( !rect_equals(r, v->bounds) )

               NOT_ACCEL_MOVE(v, v->change_bounds(v, r));

         else

               v->draw_view(v);

         p = p->next_view_to_last(p);

    } while ( p );

  o->draw_under_rect(o, sb);

  if ( screen_double_buffer ) {
    draw_to_2nd_buffer--;
    if ( draw_to_2nd_buffer <= 0 ) draw_to_2nd_buffer=0;
    if ( !draw_to_2nd_buffer ) {
      l_int i = 0;
      set_clip(screen,ff.a.x,ff.a.y,ff.b.x,ff.b.y);
      i = mouse->block(mouse, ff);
      blit(screen_double_buffer,screen,ff.a.x,ff.a.y,ff.a.x,ff.a.y, rect_sizex(ff)+1,rect_sizey(ff)+1);
      mouse->unblock(mouse, i);
    };
  };
};


void    view_set_palette ( p_view o, l_int *palette )
{
  /* each object has own palette */
  #ifdef __EACH_OWN_PALETTE__
  l_int i = 1;

  if ( o->palette ) sf_free(o->palette);

  if ( palette ) {

      /* get num of colors */
      while ( CO_ISCOLOR(*pallette) ) i++;

      /* allocate mem and copy palette to destination */
      o->palette = copy_type(palette, i*sizeof(l_int));

  } else o->palette = NULL;
  #else
  /* each object has global palette */
      o->palette = palette;
  #endif
};

BITMAP* view_begin_paint ( p_view o, t_point *p, t_rect r )
{
  t_point t = point_assign(0, 0);
  l_bool  is_screen_out = o->is_draw_mode(o, DWM_ONLYTOSCREEN);
  p_view  own = VIEW(OBJECT(o)->owner_view(OBJECT(o)));

  if ( p ) *p = o->get_global_point(o, t);
  if ( OBJECT(o)->state & OB_SF_DRAWBACK ) return NULL;
  if ( !o->draw_lock &&  own && !own->is_draw_mode(own,DWM_DRAWSUBVIEW)
       && ( o->transparent || o->trans_effect != 255 ) ) {
    OBJECT(o)->state |= OB_SF_DRAWBACK;
    view_draw_overlays_rect(o,r);
    OBJECT(o)->state &= ~OB_SF_DRAWBACK;
  };

  /* fifo safe_clip => ptr to each clip */
  o->safe_clip = fifo_add_rect(o->safe_clip, &(o->clip), o->draw_lock);

  o->clip = rect_cliped(o->clip, rect_cliped(o->get_local_extent(o), r));

  o->set_clips(o, r);

  o->lock_drawing(o);

  if ( is_screen_out )

     return rect_check_empty(o->get_cliped_extent(o))?NULL:OUTBUFFER(o->draw_out);

  else
         /* when we don't want to write into buffer => only for speed */
    return rect_check_empty(o->get_cliped_extent(o))?NULL:o->draw_buffer;
};


void    view_end_of_paint ( p_view o, t_rect rwhere )
{

  t_rect *t = NULL;

  if ( OBJECT(o)->state & OB_SF_DRAWBACK ) return;

  o->unlock_drawing(o);

  if ( !o->draw_lock ) { /* not locking, so i'm going to draw */

    if ( !o->is_draw_mode(o, DWM_ONLYTOBUFFER) &&
         !o->is_draw_mode(o, DWM_ONLYTOSCREEN) ) {

      o->draw_mini_box(o, (draw_to_2nd_buffer)? screen_double_buffer : OUTBUFFER(o->draw_out), o->draw_buffer, rwhere);

    };
  };

  /* get old clip */
  t = fifo_get_rect(&(o->safe_clip), o->draw_lock);

  if ( t ) {

      o->clip = *t;

      o->set_clips(o, o->clip);

  };

  if ( !o->draw_lock ) /* */

      o->reset_clips(o); /* it's not important to use, but may-be...*/

};


void    view_set_clips ( p_view o, t_rect r ) {
  r = o->get_global_bounds(o, rect_cliped(r, o->get_cliped_extent(o)));

  if ( o->draw_buffer )
    set_clip(o->draw_buffer, r.a.x, r.a.y, r.b.x, r.b.y);

  if ( OUTBUFFER(o->draw_out) )
    set_clip(OUTBUFFER(o->draw_out), r.a.x, r.a.y, r.b.x, r.b.y);

};

void    view_reset_clips ( p_view o ) {
  if ( o->draw_buffer )
    set_clip(o->draw_buffer, rect_empty.a.x, rect_empty.a.y, rect_empty.b.x, rect_empty.b.y);

  if ( OUTBUFFER(o->draw_out) )
    set_clip(OUTBUFFER(o->draw_out), rect_empty.a.x, rect_empty.a.y, rect_empty.b.x, rect_empty.b.y);
};


t_rect  view_get_local_extent ( p_view o )
{
  t_rect r = rect_assign(0, 0, rect_sizex(o->bounds), rect_sizey(o->bounds));

  return r;
};


t_rect  view_get_global_bounds ( p_view o, t_rect r )
{
  while ( o ) {

    r = rect_move(r, o->bounds.a.x, o->bounds.a.y);

    o = (p_view)(((p_object)o)->owner_view((p_object)o));

  };
  return r;
};


t_rect  view_get_local_bounds ( p_view o, t_rect r )
{
  t_rect dest = o->get_local_extent(o);
  dest = o->get_global_bounds(o, dest);

  dest.a.x = r.a.x - dest.a.x;
  dest.a.y = r.a.y - dest.a.y;
  dest.b.x = dest.a.x + rect_sizex(r);
  dest.b.y = dest.a.y + rect_sizey(r);

  return dest;
};


t_point view_get_global_point ( p_view o, t_point p )
{
  t_rect  r = o->get_global_bounds(o, rect_assign(p.x, p.y, p.x, p.y));
  return r.a;
};


t_point view_get_local_point ( p_view o, t_point p )
{
  t_rect r = o->get_local_bounds(o, rect_assign(p.x, p.y, p.x, p.y));
  return r.a;
};


t_rect* view_get_not_overlays ( p_view o, t_rect rlocal, t_rect rgroup,
                                p_view p, p_view target, l_long *rnum,
                                t_rect **rout, l_bool sub, l_bool ws )
{
  t_rect box = rect_empty;
  l_bool b = false;
  l_bool s = false;
  t_rect start_box;
  t_rect safe;

  __1:

  if ( p ) p = VIEW(OBJECT(p)->prev_view_to_first(OBJECT(p)));

  __2:

  start_box = rgroup;
  safe = rgroup;

  while ( p ) {

    t_rect  r = p->bounds;

    l_dword state = OBJECT(p)->state;

    /* if p is subview & not ignore owner's limits */

    if ( ws && !OBJECT(p)->is_options(OBJECT(p), VW_OF_IGNORELIM) )

         r = rect_cliped(o->size_limits(o), r);

    if ( !(state & OB_SF_VISIBLE) ||/* !(state & OB_SF_EXPOSED) ||*/
         rgroup.b.x < r.a.x || rgroup.b.y < r.a.y ||
         rgroup.a.x > r.b.x || rgroup.a.y > r.b.y )

         p = VIEW(OBJECT(p)->prev_view_to_first(OBJECT(p)));

    else {

      box.a.x = max(r.a.x, rgroup.a.x);
      box.a.y = max(r.a.y, rgroup.a.y);
      box.b.x = min(r.b.x, rgroup.b.x);
      box.b.y = min(r.b.y, rgroup.b.y);

      break;

    };
  };

  if ( !rect_check_empty(box) ) {

    if ( box.a.y > rgroup.a.y && box.a.y <= rgroup.b.y ) {

      rgroup.b.y = box.a.y-1; safe.a.y = box.a.y;
      (*rout) = o->get_not_overlays(o, rgroup, rgroup, p, target, rnum, rout, sub, ws);
      rgroup = start_box;

    };

    if ( box.b.y < rgroup.b.y && box.b.y >= rgroup.a.y ) {

      rgroup.a.y = box.b.y+1; safe.b.y = box.b.y;
      (*rout) = o->get_not_overlays(o, rgroup, rgroup, p, target, rnum, rout, sub, ws);
      rgroup = start_box;

    };

    if ( box.a.x > rgroup.a.x && box.a.x <= rgroup.b.x ) {

      rgroup.a.y = safe.a.y; rgroup.b.y = safe.b.y;
      rgroup.b.x = box.a.x-1;
      (*rout) = o->get_not_overlays(o, rgroup, rgroup, p, target, rnum, rout, sub, ws);
      rgroup = start_box;

    };

    if ( box.b.x < rgroup.b.x && box.b.x >= rgroup.a.x ) {

      rgroup.a.x = box.b.x+1;
      rgroup.a.y = safe.a.y; rgroup.b.y = safe.b.y;
      (*rout) = o->get_not_overlays(o, rgroup, rgroup, p, target, rnum, rout, sub, ws);
      rgroup = start_box;

    };

  } else {

    if ( target ) {

      target = VIEW(OBJECT(target)->owner_view(OBJECT(target)));
      p = target;

      if ( p )
        rgroup = rect_move(rgroup, p->bounds.a.x, p->bounds.a.y);

      goto __1;

    } else if ( !sub  ) {

        (*rnum)++;

        if ( rout ) {

          (*rout) = (t_rect*)_realloc((*rout), (*rnum)*sizeof(t_rect));
          if (*rout) {

            (*rout)[(*rnum)-2] = ws?rgroup:o->get_local_bounds(o, rgroup);
            (*rout)[(*rnum)-1] = rect_empty;

          };

          return (*rout);

        } else return NULL;

      } else {

        p = VIEW(OBJECT(o)->last_view(OBJECT(o)));

        rgroup = o->get_local_bounds(o, rgroup);

        ws = true; /* to be able to know that p(s) are subviews */
        sub = false;

        goto __2; /* can't move p to prev position */

      };
  };

  return (*rout);
};


void    view_set_draw_mode ( p_view o, l_int dm, l_bool set )
{
  if ( set )
    o->draw_mode |= dm;
  else
    o->draw_mode &= ~dm;
};


l_bool  view_is_draw_mode ( p_view o, l_int dm ) {
  return (l_bool)(o->draw_mode & dm);
};

void    view_show ( p_view o ) {
  if ( !OBJECT(o)->is_state(OBJECT(o), OB_SF_VISIBLE) )

    OBJECT(o)->set_state(OBJECT(o), OB_SF_VISIBLE, true);
};

void    view_hide ( p_view o ) {
  if ( OBJECT(o)->is_state(OBJECT(o), OB_SF_VISIBLE) )

    OBJECT(o)->set_state(OBJECT(o), OB_SF_VISIBLE, false);
};

t_rect  view_get_cliped_extent ( p_view o ) {
  t_rect r = o->clip;
  p_view s = o;
  p_view x = NULL;

  while ( o ) {

    r = rect_cliped(r, o->get_local_extent(o));

    if ( !rect_check_empty(r) ) r = rect_cliped(r, o->clip);

    else return rect_empty;

    if ( !rect_check_empty(r) ) {

      r = rect_move(r, o->bounds.a.x, o->bounds.a.y);

      r = rect_cliped(r, o->get_limits(o));

    } else return rect_empty;

    if ( rect_check_empty(r) ) return rect_empty;

    o = VIEW(OBJECT(o)->owner_view(OBJECT(o)));

  };

  return s->get_local_bounds(s, r);
};

t_rect  view_size_limits ( p_view o ) {
  return o->get_local_extent(o);
};

void    view_set_bounds ( p_view o, t_rect r ) {
  t_point s = o->size_minimum(o);

  o->bounds = rect_assign(r.a.x, r.a.y, lmax(r.b.x, r.a.x+s.x), lmax(r.b.y, r.a.y+s.y));

  o->clip = o->get_local_extent(o);

};

void    view_change_bounds ( p_view o, t_rect nr ) {
  t_rect oldb = o->bounds;

  /* maybe mimimum size is yet and we make also mimimum, so try it */
  o->set_bounds(o, nr);
  nr = o->bounds;
  o->set_bounds(o, oldb);

  if ( !rect_equals(o->bounds, nr) ) {  /* old rect and new one are not same */

    if ( rect_sizex(nr) == rect_sizex(o->bounds) &&
         rect_sizey(nr) == rect_sizey(o->bounds) ) {

      o->move_view(o, nr);

    } else {

      o->grow_view(o, nr);

    };

  };
};

void    view_set_clip ( p_view o, BITMAP *out, t_rect r ) {

  r = o->get_global_bounds(o, rect_cliped(r, o->get_cliped_extent(o)));

  if ( out )

    set_clip(out, r.a.x, r.a.y, r.b.x, r.b.y);

};

void button3d ( p_view o, BITMAP *bmp, int x1, int y1, int x2, int y2, int down );

void    view_background ( p_view o, BITMAP *out, t_rect r ) {
  if ( o->transparent ) return;

  if ( o->brush.background ) {

    if ( o->brush.state & BRUSH_STRETCH )

      stretch_sprite(out, o->brush.background, r.a.x, r.a.y, rect_sizex(r)+1, rect_sizey(r)+1);

    else

    if ( o->brush.state & BRUSH_CENTER ) {

      l_rect dx = (rect_sizex(r)-IMAGE_WIDTH(o->brush.background))/2;
      l_rect dy = (rect_sizey(r)-IMAGE_HEIGHT(o->brush.background))/2;

      rectfill(out, r.a.x, r.a.y, r.b.x, r.b.y, o->brush.color);

      draw_sprite(out, o->brush.background, r.a.x+dx, r.a.y+dy);

    }
    else

      blit_ex(o->brush.background, out, 0, 0, r.a.x, r.a.y,
              IMAGE_WIDTH(o->brush.background), IMAGE_HEIGHT(o->brush.background),
              rect_sizex(r)+1, rect_sizey(r)+1);

  } else {

    if (  o->brush.color == o->brush.color2 ||
        !(o->brush.state & BRUSH_GRADIENT) )

       rectfill(out, r.a.x, r.a.y, r.b.x, r.b.y, o->brush.color);

    else

       if ( o->brush.state & BRUSH_GRADIENT_HOR )

          fade_rect(out, r.a.x, r.a.y, r.b.x, r.b.y, o->brush.color, o->brush.color2, FR_HOR);

       else

          fade_rect(out, r.a.x, r.a.y, r.b.x, r.b.y, o->brush.color, o->brush.color2, FR_VER);

  };

  if ( o->brush.state & BRUSH_DRAWBORDER ) {
	if ( rect_equals(r, o->get_global_bounds( o, o->get_local_extent(o))) )
      button3d( o, out, r.a.x, r.a.y, r.b.x, r.b.y, o->brush.state & BRUSH_DOWNBORDER );
  };

};



p_view  _view_init ( p_view o, t_rect r )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_view));

  obj_init(&o->obclass);


  /* object's functions */

  OBJECT(o)->reset_prefer = &view_reset_prefer;
  OBJECT(o)->setup = &view_setup;
  OBJECT(o)->after_init = &view_after_init;
  OBJECT(o)->set_state = &view_set_state;
  OBJECT(o)->put_in_front_of = &view_put_in_front_of;
  OBJECT(o)->done = &view_done;
  OBJECT(o)->translate_event = &view_translate_event;

  OBJECT(o)->play_process = &view_play_process;


  /* view's functions */

  o->draw_out = &screen;
  o->draw_buffer = screen_virtual;
  o->font = font_system;
  o->cursor = CUR_ARROW;
  o->palette = NULL;
  o->draw_mode |= DWM_CANTACCELMOVE;

  o->reset_align = &view_reset_align;

  o->execute_view = &view_execute_view;
  o->get_top_view_under_mouse = &view_get_top_view_under_mouse;
  o->top_view = &view_top_view;
  o->show_info_board = &view_show_info_board;
  o->set_mouse_cursor = &view_set_mouse_cursor;
  o->is_mouse_in_view = &view_is_mouse_in_view;

  o->drag_data = &view_drag_data;
  o->drag_where = &view_drag_where;
  o->drop_data = &view_drop_data;

  o->drag_view = &view_drag_view;
  o->drag = &view_drag;
  o->size_minimum = &view_size_minimum;
  o->get_limits = &view_get_limits;

  o->draw = &view_draw;
  o->draw_me = &view_draw_me;
  o->draw_view = &view_draw_view;
  o->draw_sub_views = &view_draw_sub_views;

  o->get_color = &view_get_color;
  o->set_palette = &view_set_palette;

  o->is_top_view = &view_is_top_view;
  o->move_accel = &view_move_accel;
  o->move_view = &view_move_view;
  o->grow_view = &view_grow_view;

  o->get_view_under_mouse = &view_get_view_under_mouse;
  o->lock_drawing = &view_lock_drawing;
  o->unlock_drawing = &view_unlock_drawing;
  o->is_draw_lock = &view_is_draw_lock;

  o->begin_paint = &view_begin_paint;
  o->end_of_paint = &view_end_of_paint;

  o->draw_overlays = &view_draw_overlays;
  o->draw_mini_box = &view_draw_mini_box;

  o->draw_under_view = &view_draw_under_view;
  o->draw_under_rect = &view_draw_under_rect;
  o->draw_in_rect = &view_draw_in_rect;

  o->get_local_extent = &view_get_local_extent;

  o->get_global_bounds = &view_get_global_bounds;
  o->get_local_bounds = &view_get_local_bounds;
  o->get_global_point = &view_get_global_point;
  o->get_local_point = &view_get_local_point;

  o->get_not_overlays = &view_get_not_overlays;

  o->show = &view_show;
  o->hide = &view_hide;

  o->for_each_sub_view_set_state = &view_for_each_sub_view_set_state;
  o->set_draw_mode = &view_set_draw_mode;
  o->is_draw_mode = &view_is_draw_mode;

  o->size_limits = &view_size_limits;
  o->get_cliped_extent = &view_get_cliped_extent;

  o->set_bounds = &view_set_bounds;

  o->change_bounds = &view_change_bounds;

  o->set_clip = &view_set_clip;
  o->background = &view_background;

  o->set_clips = &view_set_clips;
  o->reset_clips = &view_reset_clips;
  o->icon_size = system_item_size;

  o->transparent = false;
  o->trans_effect = 255; // 255 = SOLID
  /* functions running & variable's declarations */

  OBJECT(o)->set_options(OBJECT(o), VW_OF_VISIBILITY+OB_OF_SELECTABLE, true);

  o->set_bounds(o, r);
  o->clip = o->get_local_extent(o);
  o->brush.color = COLOR(CO_BLACK);
  o->brush.skin = NULL;

  o->brush.state |= BRUSH_LARGE3D;

  OBJECT(o)->tag |= TAG_VIEW;

};

