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

/* Revision History (as of 02/04/2002):
 *
 * 02/04/2002: Made compatible with window skinning (orudge)
 */

#include <seal.h>
#include <menus.h>
#include <skin.h>

p_menuview  (*menuview_init) ( p_menuview o, t_rect r, p_menu menu ) = &_menuview_init;
p_menuview  (*menuview_init_ex) ( p_menuview o, t_rect r, l_rect item_size, l_rect icon_size, p_menu menu ) = &_menuview_init_ex;
p_menuview  (*hormenu_init) ( p_menuview o, t_rect r, p_menu menu ) = &_hormenu_init;
p_menuview  (*hormenu_init_ex) ( p_menuview o, t_rect r, l_rect item_size, l_rect icon_size, p_menu menu ) = &_hormenu_init_ex;


l_font* standard_menuitem_font = NULL;

/* t_menuview */

l_bool  menuview_done ( p_object o ) {

  BITMAP *bmp = MENUVIEW(o)->safe_desktop;

  l_bool r = view_done(o);

  if ( bmp )
    destroy_bitmap(bmp);

  return r;
};


void    menuview_set_state ( p_object o, l_dword st, l_bool set )
{

  if ( set && o->owner_view(o) &&
       st & OB_SF_VISIBLE && !o->is_state(o, OB_SF_VISIBLE) ) /* go to be visible */

    MENUVIEW(o)->save_desktop(MENUVIEW(o));

  view_set_state(o, st, set);

 // if ( st & OB_SF_FOCUSED ) /* menu is focused */

 //    MENUVIEW(o)->draw_current_item(MENUVIEW(o)); /* it take a lot of time => draw_item */



};


void    menuview_translate_event ( p_object o, t_event *event )
{

  RETVIEW(o, event);

  view_translate_event(o, event); /* old translate_event function */

  if ( event->type & EV_MOUSE &&

       OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) { /* mouse events */

       l_dword msg = 0;

       clear_event(event);

       menu_clear_lastitem_called(MENUVIEW(o)->menu);

       msg = desktop->execute_view(desktop, VIEW(o));

       if ( msg != MSG_NOTHING && msg != MSG_CANCEL ) {

          set_event(event, EV_MESSAGE, msg, o);

       };

       o->put_event(o, event);

       clear_event(event);

       if ( o->owner->prefer == o )  /* only one item, so redraw it for  */

           MENUVIEW(o)->draw_current_item(MENUVIEW(o));

  };

};


l_dword menuview_execute ( p_object o )
{

   p_view vo = VIEW(o);
   p_menuview mo = MENUVIEW(o);

   l_int  action = MA_NONE;
   l_bool iamsub = false;
   l_dword message = MSG_NOTHING;

   p_menuitem current = mo->menu->current;

   if ( mo->action == MA_IAMSUBMENU ) iamsub = true;

   mo->inside = true;

   mo->draw_current_item(mo);

   do {

      action = MA_NONE;

      o->get_event(o, &event_main);

      if ( event_main.type & EV_KEYBOARD )
      {


         if ( OBJECT(keyb)->state & KB_SF_KEYDOWN ) {

         l_dword msg;

         switch ( keyb->code >> 8 ) {

           case KB_DOWN  :  current = mo->get_next_item(mo, current, true); break;
           case KB_UP    :  current = mo->get_next_item(mo, current, false); break;
           case KB_LEFT  :  if ( iamsub && mo->parent_menu->menu &&
                                           mo->parent_menu->menu->items &&
                                           mo->parent_menu->menu->items->next ) action = MA_BACKWARD; break;
           case KB_ESC   :  action = MA_ESC; break;
           case KB_RIGHT :  action = MA_FORWARD; break;
           case KB_ENTER :  action = MA_ENTER; break;

         };

         msg = menu_get_hotkey_message(mo->top_menuview(mo)->menu, keyb->code);

         if ( msg != MSG_NOTHING ) {

             message = msg;
             action = MA_END;

         } else if ( KEYCTRL(KB_F1) ) /* help info */

                    _show_info_board(vo, current?current->info_text:NULL);

         };

      } else

      if ( event_main.type & EV_MOUSE ) {

         if ( OBJECT(mouse)->state & MO_SF_MOUSEPRESS ) {

           if ( vo->is_mouse_in_view(vo) ) { /* mouse is in view */

               p_menuitem cur = mo->get_item_from_mouse(mo, mouse->where);

               if ( cur ) current = cur;

           } else {

               if ( !mo->is_mouse_in_one_of_parent_item(mo, mouse->where) &&
                     mo->is_mouse_in_one_of_parent_menu(mo, mouse->where) )

                  action = MA_OTHER;

               else

               if ( OBJECT(mouse)->state & MO_SF_MOUSEDOWN &&
                    !mo->is_mouse_in_one_of_parent_menu(mo, mouse->where) ) {

                  action = MA_END;

               };
           };

        } else

        if ( OBJECT(mouse)->state & MO_SF_MOUSEUP )  { /* */

           if ( vo->is_mouse_in_view(vo) ) { /* mouse is in view */

              action = MA_ENTER;

           };

        };

      };


      if ( current != mo->menu->current ) { /* redraw items */

         p_menuitem old_current = mo->menu->current;

         mo->menu->current = current; /* set current item */

         if ( old_current != mo->menu->current ) { /* check if not same */

           mo->draw_item(mo, old_current);
           mo->draw_item(mo, mo->menu->current);

         };

      };


      if ( current ) { /* is some item selected */

         if ( action == MA_ENTER && current->enable ) { /* action is command */

            if ( current->message || current->flags & MIF_CHECK ) { /* it's command item */

               action = MA_COMMAND;
               message = current->message;

               current->lastcall = true;

               if ( current->flags & MIF_CHECK )

                  if ( current->flags & MIF_CHECKOK ) current->flags &= ~MIF_CHECKOK;

                  else current->flags |= MIF_CHECKOK;

            } else  /* it's menu item */

               action = MA_SUBMENU;

         };

         if ( action == MA_SUBMENU || action == MA_FORWARD ) { /* make new sub-menu */

            if ( current->submenu ) { /* has some submenu */

               t_rect r = mo->get_item_box(mo, current);
               r.a.x = r.b.x - 5;

               r = rect_move(r, vo->bounds.a.x, vo->bounds.a.y);

               message = mo->run_new_menu(mo, r, &action, current->submenu);

               if ( action == MA_BACKWARD ||
                    action == MA_ESC ||
                    action == MA_OTHER )  action = MA_NONE;

            } else /* not sub-menu */

              if ( action == MA_FORWARD &&
                   mo->is_hor_menu_one_of_parents(mo) &&
                   mo->is_hor_menu_one_of_parents(mo)->menu->items->next ) /* if hor-menu is under me */

                action = MA_HMFORWARD; /* set action to hor-menu forward */
         };

      };


      if ( action > 0 ) clear_event(&event_main);

   } while ( action > 0 );

   if ( action == MA_END ) o->put_event(o, &event_main);

   mo->action = action;

   mo->inside = false;

   vo->hide(vo);

   return message;
};


t_rect  menuview_size_limits ( p_view o )
{

  return rect_assign(2, 2, rect_sizex(o->bounds)-2, rect_sizey(o->bounds)-2);

};


void  menuview_draw_under_rect ( p_view o, t_rect r )
{

  if ( MENUVIEW(o)->safe_desktop ) {
    t_point t = point_assign(0,0);
    t_point p = o->get_global_point(o, t);
    t_rect nr;

    r = rect_move(r,-o->bounds.a.x,-o->bounds.a.y);
    nr = rect_move(r,p.x,p.y);

    BLIT(MENUVIEW(o)->safe_desktop, screen, r.a.x, r.a.y, nr.a.x, nr.a.y,
                                    rect_sizex(r),//CONTEXT_WIDTH(MENUVIEW(o)->safe_desktop),
                                    rect_sizey(r));//CONTEXT_HEIGHT(MENUVIEW(o)->safe_desktop));

    /*destroy_bitmap(MENUVIEW(o)->safe_desktop);

    MENUVIEW(o)->safe_desktop = NULL;*/

  } else

    view_draw_under_rect(o, r);

};


void  menuview_draw ( p_view o )
{
  t_rect  r = o->get_local_extent(o);
  t_point p;
  l_bool old = o->is_draw_mode(o,DWM_DONTDRAWBACK);

  BITMAP *out = NULL;

  if ( old ) o->set_draw_mode(o,DWM_DONTDRAWBACK,false);

  out = o->begin_paint(o, &p, r);

  if ( out ) {

    p_menuitem item = MENUVIEW(o)->menu?MENUVIEW(o)->menu->items:NULL;

    o->background(o, out, rect_move(r, p.x, p.y));

    //button3d(o, out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, 0);

    while ( item ) {

      MENUVIEW(o)->draw_item(MENUVIEW(o), item);
      item = item->next;

    };

  };

  o->end_of_paint(o, r);

  if ( old ) o->set_draw_mode(o,DWM_DONTDRAWBACK,true);

};


p_menuview  menuview_top_menuview ( p_menuview o )
{

  while ( o->parent_menu )

    o = o->parent_menu;

  return o;

};


l_bool      menuview_is_mouse_in_one_of_parent_item ( p_menuview o, t_point p )
{
   p_menuview m = o;

   while ( m ) {

     p_menuitem sel = m->menu?m->menu->current:NULL;

     if ( sel && m->get_item_from_mouse(m, p) == sel ) return true;

     m = m->parent_menu;

   };

   return false;

};


l_bool      menuview_is_mouse_in_one_of_parent_menu ( p_menuview o, t_point p )
{
   p_menuview m = o;

   while ( m ) {

     if ( m->get_item_from_mouse(m, p) ) return true;

     m = m->parent_menu;

   };

   return false;

};


l_bool      menuview_is_one_of_subs_state ( p_menuview o, l_dword st )
{
   p_menuview m = o;

   while ( m ) {

     if ( OBJECT(m)->is_state(OBJECT(m), st) ) return true;

     m = m->sub_menu;

   };

   return false;

};


p_menuitem  menuview_get_item_from_mouse ( p_menuview o, t_point p )
{
   p_view vo = VIEW(o);

   t_rect s  = vo->get_local_extent(vo);

   p = vo->get_local_point(vo, p);

   if ( rect_contains(s, p) ) {

      p_menuitem i = o->menu?o->menu->items:NULL;

      while ( i ) {

        t_rect r = o->get_mouseitem_box(o, i);

        if ( rect_contains(r, p) ) return i;

        i = i->next;

      };

   };

   return NULL;

};


void  menuview_save_desktop ( p_menuview o )
{

  t_rect r = VIEW(o)->get_global_bounds(VIEW(o), VIEW(o)->get_local_extent(VIEW(o)));


  if ( o->safe_desktop ) return; /* if exist, destroy it */

    //  destroy_bitmap(MENUVIEW(o)->safe_desktop);

  /* create new bitmap */

  o->safe_desktop = create_bitmap(rect_sizex(r)+1, rect_sizey(r)+1);


  if ( o->safe_desktop ) /* blit screen, if there was enough memory */

     BLIT(screen, o->safe_desktop, r.a.x, r.a.y, 0, 0, rect_sizex(r), rect_sizey(r));

};


p_menuview      menuview_is_hor_menu_one_of_parents ( p_menuview o )
{
   p_menuview m = o;

   while ( m ) {

     if ( m->flags & MF_HORMENU ) return m;

     m = m->parent_menu;

   };

   return m;
};


p_menuitem  menuview_prev_item ( p_menuview o, p_menuitem item )
{
  p_menuitem p = item;
  p_menuitem first = o->menu?o->menu->items:NULL;

  if ( p == first ) p = NULL;

  if ( item )

    do {

      item = o->next_item(o, item);

    } while ( item->next != p );

  return item;

};


p_menuitem  menuview_next_item ( p_menuview o, p_menuitem item )
{

  if ( item && item->next )

    item = item->next;

  else

    item = o->menu?o->menu->items:NULL;

  return item;

};


p_menuitem  menuview_get_next_item ( p_menuview o, p_menuitem item, l_bool forward )
{

  p_menuitem old = item;

  do {

    if ( forward )

      item = o->next_item(o, item);

    else

      item = o->prev_item(o, item);

  } while ( item != old && !item->name );

  return item;
};


p_menuitem  menuview_index_of ( p_menuview o, l_int item )
{

  p_menuitem i = o->menu?o->menu->items:NULL;

  while ( i ) {

    if ( item == 0 ) return i;

    item--;

    i = i->next;

  };

  return NULL;
};



l_int  menuview_at ( p_menuview o, p_menuitem p )
{

  p_menuitem i = o->menu?o->menu->items:NULL;

  l_int item = 0;

  while ( i ) {

    if ( i == p ) return item;

    item++;

    i = i->next;

  };

  return -1;
};

void  menuview_draw_item_ex ( p_menuview o, p_menuitem i, BITMAP *out, t_point p )
{
  p_view vo = VIEW(o);
  if ( i ) {

    t_rect r = o->get_item_box(o, i);

    if ( !rect_check_empty(r) ) {

       if ( out ) {

          t_rect  t = rect_assign(r.a.x+o->between.x+2+STANDARD_MENUBOUNDSDIFF_SIZE, r.a.y, r.b.x-STANDARD_MENUBOUNDSDIFF_SIZE, r.b.y);
          t_rect  f = rect_assign(r.a.x+STANDARD_MENUBOUNDSDIFF_SIZE, r.a.y, t.a.x, r.b.y);

          l_color fcolor = color_menu_text;

          //vline(out, r.a.x+p.x-1, r.a.y+p.y, r.b.y+p.y, color_3d_light);
          //vline(out, r.b.x+p.x+1, r.a.y+p.y, r.b.y+p.y, color_3d_shadow);

          if ( !i->name ) { /* draw LINE */

            l_rect dy = (rect_sizey(r)-2/*size of line*/)/2;

            vo->background(vo, out, rect_move(r, p.x, p.y));

            button(out, r.a.x+p.x+6, r.a.y+p.y+dy+1, r.b.x+p.x-6, r.a.y+p.y+dy+2, color_3d_shadow, color_3d_light);

          } else { /* draw ITEM or SUB-MENU */


            if ( !o->is_one_of_subs_state(o, OB_SF_SELECTED) ||
                  i != o->menu->current || !o->inside )  { /* not selected item */

               vo->background(vo, out, rect_move(r, p.x, p.y));
               if ( !i->enable ) /* disabled item */
                 fcolor = color_3d_light;

            } else

            { /* selected item */

               fcolor = color_menu_selected_text;

               vo->background(vo, out, rect_move(r, p.x, p.y));
               fade_rect(out, r.a.x+p.x+1, r.a.y+p.y, r.b.x+p.x-1, r.b.y+p.y, color_menu_selected_face, color_menu_selected_face_gardient, FR_HOR);

               if ( !i->enable ) /* disabled item */
                 fcolor = color_3d_shadow;

            };

            if ( !i->message || i->submenu ) { /* it's sub-menu item */

               textout_draw_rect(out, get_font_in_size("Symbols", 10, 10), "b", -1,
                                 t.a.x+p.x, t.a.y+p.y, t.b.x+p.x-8, t.b.y+p.y,
                                 TX_ALIGN_CENTERY|TX_ALIGN_RIGHT, fcolor, TX_NOCOLOR, 0);
            };

            if ( i->flags & MIF_CHECKOK ) { /* draw ok item */

               textout_draw_rect(out, get_font_in_size("Symbols", 12, 12), "O", -1,
                                 f.a.x+p.x, f.a.y+p.y, f.b.x+p.x, f.b.y+p.y,
                                 TX_ALIGN_CENTER, fcolor, TX_NOCOLOR, 0);

            } else

            if ( i->icon ) { /* draw icon if some is occurs */

               l_rect dy = (rect_sizey(f)-icon_size(o))/2;
               l_rect dx = (rect_sizex(f)-icon_size(o))/2;

               stretch_sprite(out, i->icon, f.a.x+p.x+dx, f.a.y+p.y+dy, icon_size(o), icon_size(o));

            } else

            if ( i->font_symbol && i->char_symbol > 0 ) { /* if some symbol before text */

               l_char s[2] = {0, '\0'};
               s[0] = i->char_symbol;

               textout_draw_rect(out, i->font_symbol, s, -1,
                                 f.a.x+p.x, f.a.y+p.y, f.b.x, f.b.y+p.y,
                                 TX_ALIGN_CENTER, fcolor, TX_NOCOLOR, 0);
            };


            if ( i->param ) { /* print param of item f.e. : ALT+X ... */


               textout_draw_rect(out, i->font, i->param, -1, t.a.x+p.x, t.a.y+p.y,
                                 t.b.x+p.x-8, t.b.y+p.y, TX_ALIGN_CENTERY|TX_ALIGN_RIGHT, fcolor, TX_NOCOLOR, 0);

            };


            /* print name of item */

            textout_draw_rect(out, i->font, i->name, -1, t.a.x+p.x, t.a.y+p.y,
                              t.b.x+p.x, t.b.y+p.y, TX_ALIGN_CENTERY|TX_ALIGN_LEFT, fcolor, TX_NOCOLOR, 0);
            if ( !i->enable ) /* disable item */
              if ( !o->is_one_of_subs_state(o, OB_SF_SELECTED) ||
                  i != o->menu->current || !o->inside )
              textout_draw_rect(out, i->font, i->name, -1, t.a.x+p.x-1, t.a.y+p.y-1,
                                t.b.x+p.x-1, t.b.y+p.y-1, TX_ALIGN_CENTERY|TX_ALIGN_LEFT, color_3d_shadow, TX_NOCOLOR, 0);

          }; /* end of item or sub-menu drawing */
       };

    };

  };

};


void  menuview_draw_item ( p_menuview o, p_menuitem item )
{

  p_menuitem i = item;

  if ( i ) {

    t_rect r = o->get_item_box(o, item);

    if ( !rect_check_empty(r) ) {

       p_view  vo = VIEW(o);
       t_point p;
       BITMAP *out;

       t_rect safe = rect_assign(r.a.x, r.a.y, r.b.x, r.b.y);

       out = vo->begin_paint(vo, &p, safe);

       if ( out ) {

         menuview_draw_item_ex(o,i,out,p);

       };

       vo->end_of_paint(vo, safe);

    };

  };

};


void        menuview_draw_current_item ( p_menuview o )
{

  if ( o->menu )

     o->draw_item(o, o->menu->current);
};


t_rect  menuview_get_item_box_ex ( p_menuview o, p_menuitem item, t_rect r )
{

  p_menuitem i = o->menu?o->menu->items:NULL;

  t_rect     s   = VIEW(o)->size_limits(VIEW(o));
  t_rect     sr  = r;
  l_int      hy  = 0;
  l_int      dy  = 0;

  while ( i && i != item ) {

    dy = s.a.y-sr.a.y;

    if ( i->name ) /* if item or sub-menu */

      r.a.y += o->item_size;

    else /* if line */

      r.a.y += STANDARD_MENULINE_SIZE+1;

    i = i->next;

    if ( !i ) /* last item */

       hy = sr.b.x-s.b.y;

  };

  if ( i )

    if ( i->name )

       return rect_assign(r.a.x, r.a.y+dy, r.b.x, r.a.y+dy+o->item_size+hy);

    else /* return line */

       return rect_assign(r.a.x, r.a.y+dy, r.b.x, r.a.y+dy+STANDARD_MENULINE_SIZE+hy);


  return rect_empty;

};


t_rect  menuview_get_item_box ( p_menuview o, p_menuitem item )
{

  return o->get_item_box_ex(o, item, VIEW(o)->size_limits(VIEW(o)));

};


t_rect  menuview_get_mouseitem_box ( p_menuview o, p_menuitem item )
{

  return o->get_item_box_ex(o, item, VIEW(o)->get_local_extent(VIEW(o)));

};


l_dword     menuview_run_new_menu ( p_menuview o, t_rect r, l_int *action, p_menu menu )
{

  l_dword msg = MSG_NOTHING;
  p_view  ow = VIEW(OBJECT(o)->owner_view(OBJECT(o)));

  if ( o && ow ) {

    p_object mv;

    o->sub_menu = MENUVIEW(menuview_init(MENUVIEW(_malloc(sizeof(t_menuview))),
                           r,
                           menu));

    mv = OBJECT(ow)->insert(OBJECT(ow), OBJECT(o->sub_menu));

    if ( mv ) {

       MENUVIEW(mv)->action = MA_IAMSUBMENU;
       MENUVIEW(mv)->parent_menu = o;

    };

    msg = ow->execute_view(ow, VIEW(mv));

    if ( action && mv ) *(action) = MENUVIEW(mv)->action;

    dispose(OBJECT(mv));

    o->sub_menu = NULL;

    MENUVIEW(o)->draw_current_item(MENUVIEW(o));

  };

  return msg;

};



/* horizontal menu */

l_dword     hormenu_execute ( p_object o )
{
   p_view vo = VIEW(o);
   p_menuview mo = MENUVIEW(o);

   l_int  action = MA_NONE;
   l_bool iamsub = false;
   l_bool autosub = false;
   l_dword message = MSG_NOTHING;

   p_menuitem current = mo->menu->current;

   if ( mo->action == MA_IAMSUBMENU ) iamsub = true;

   mo->inside = true;

   mo->draw_current_item(mo);

   do {

      action = MA_NONE;

      o->get_event(o, &event_main);

      if ( event_main.type & EV_KEYBOARD )
      {

         if ( OBJECT(keyb)->state & KB_SF_KEYDOWN ) {

         l_dword msg;

         switch ( keyb->code >> 8 ) {

           case KB_RIGHT :  current = mo->get_next_item(mo, current, true); break;
           case KB_LEFT  :  current = mo->get_next_item(mo, current, false); break;
           case KB_ESC   :  action = MA_ESC; break;
           case KB_ENTER :  action = MA_ENTER; break;
           case KB_DOWN  :  action = MA_SUBMENU; break;

         };

         msg = menu_get_hotkey_message(mo->top_menuview(mo)->menu, keyb->code);

         if ( msg != MSG_NOTHING ) {

             message = msg;
             action = MA_END;

         } else if ( KEYCTRL(KB_F1) ) /* help info */

                    _show_info_board(vo, current?current->info_text:NULL);

         };

      } else

      if ( event_main.type & EV_MOUSE ) {

         if ( OBJECT(mouse)->state & MO_SF_MOUSEPRESS ) {

           if ( vo->is_mouse_in_view(vo) ) { /* mouse is in view */

               p_menuitem cur = mo->get_item_from_mouse(mo, mouse->where);

               if ( cur ) current = cur;

               action = MA_SUBMENU;

           } else

           if ( OBJECT(mouse)->state & MO_SF_MOUSEDOWN )

              if ( !mo->is_mouse_in_one_of_parent_menu(mo, mouse->where) ) {

                 action = MA_END;

              };

        };

        if ( OBJECT(mouse)->state & MO_SF_MOUSEUP )  { /* */

           if ( vo->is_mouse_in_view(vo) ) /* mouse is in view */

              action = MA_ENTER;

        };

      };


      if ( current != mo->menu->current ) { /* redraw items */

         p_menuitem old_current = mo->menu->current;

         mo->menu->current = current; /* set current item */

         if ( old_current != mo->menu->current ) { /* check if not same */

           mo->draw_item(mo, old_current);
           mo->draw_item(mo, mo->menu->current);

           if ( autosub ) action = MA_SUBMENU;

         };

      };


      if ( current ) { /* is some item selected */


         if ( action == MA_ENTER && current->enable ) { /* action is command */

            if ( current->message || current->flags & MIF_CHECK ) { /* it's command item */

               action = MA_COMMAND;
               message = current->message;
               current->lastcall = true;

               if ( current->flags & MIF_CHECK )

                  if ( current->flags & MIF_CHECKOK ) current->flags &= ~MIF_CHECKOK;

                  else current->flags |= MIF_CHECKOK;

            } else  /* it's menu item */

               action = MA_SUBMENU;

         };

         if ( action == MA_SUBMENU || action == MA_FORWARD ) { /* make new sub-menu */

            if ( current->submenu ) { /* has some submenu */

               t_rect r = mo->get_item_box(mo, current);

               r.a.y = rect_sizey(vo->bounds);

               r = rect_move(r, vo->bounds.a.x, vo->bounds.a.y);

               message = mo->run_new_menu(mo, r, &action, current->submenu);

               if ( action == MA_OTHER ||      /* automatic sub-menu opening */
                    action == MA_BACKWARD ||
                    action == MA_HMFORWARD ) autosub = true; else autosub = false;

               if ( action == MA_BACKWARD ) /* key - left was pressed in sub-menu */

                  current = mo->get_next_item(mo, current, false); /* set to prev item */

               if ( action == MA_HMFORWARD ) /* key - right was pressed in sub-menu */

                  current = mo->get_next_item(mo, current, true); /* set to next item */

               if ( action == MA_ESC ) /* in sub-menu ESC so hor-menu is escaped too */

                  action = MA_END;

               if ( action == MA_HMFORWARD ||
                    action == MA_BACKWARD ||
                    action == MA_OTHER )  action = MA_NONE;

            };

         };

      };


      if ( action > 0 ) clear_event(&event_main);

   } while ( action > 0 );

   if ( action == MA_END ) o->put_event(o, &event_main);

   mo->action = action;

   mo->inside = false;

   return message;
};


void    hormenu_save_desktop ( p_menuview o )
{
};


t_rect  hormenu_get_item_box_ex ( p_menuview o, p_menuitem item, t_rect r )
{

  p_menuitem i = o->menu?o->menu->items:NULL;

  t_rect     s  = VIEW(o)->size_limits(VIEW(o));
  t_rect     sr  = r;
  l_int      wx = 0;
  l_int      dx = 0;

  while ( i && i != item ) {

    dx = s.a.x-sr.a.x;

    if ( i->name ) /* if item or sub-menu */

      r.a.x += FONT_GETSTRWIDTH(i->font, i->name, -1)+
               FONT_GETSTRWIDTH(i->font, i->param, -1)+
               icon_size(o)+5;

    else /* if line */

      r.a.x += STANDARD_MENULINE_WIDTH+1;

    if ( !i->next ) wx = sr.b.x-s.b.x;

    i = i->next;

  };

  if ( i )

    if ( i->name )

       return rect_assign(r.a.x+dx+8, r.a.y, r.a.x+FONT_GETSTRWIDTH(i->font, i->name, -1)+
                                                   FONT_GETSTRWIDTH(i->font, i->param, -1)+
                                                   icon_size(o)+5+dx+wx+8, r.b.y);

    else /* return line */

       return rect_assign(r.a.x+dx+8, r.a.y, r.a.x+dx+wx+8, r.b.y);


  return rect_empty;

};


void  hormenu_draw ( p_view o )
{
  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    p_menuitem item = MENUVIEW(o)->menu?MENUVIEW(o)->menu->items:NULL;

    o->background(o, out, rect_move(r, p.x, p.y));
    //rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, o->get_color(o, 0));
    //button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, color_3d_light, color_3d_shadow);
    //button(out, r.a.x+p.x+2, r.a.y+p.y+2, r.a.x+p.x+4, r.b.y+p.y-2, color_3d_light, color_3d_shadow);
    //button(out, r.a.x+p.x+5, r.a.y+p.y+2, r.a.x+p.x+7, r.b.y+p.y-2, color_3d_light, color_3d_shadow);

    while ( item ) {

      MENUVIEW(o)->draw_item(MENUVIEW(o), item);

      item = item->next;

    };

  };

  o->end_of_paint(o, r);
};


void  hormenu_draw_item ( p_menuview o, p_menuitem item )
{

  p_menuitem i = item;

  if ( i ) {

    t_rect r = o->get_item_box(o, item);

    if ( !rect_check_empty(r) ) {

       p_view  vo = VIEW(o);
       t_point p;
       BITMAP *out;
       t_rect  safe;
       int dawn = 0;

       safe = rect_assign(r.a.x, r.a.y, r.b.x, r.b.y);

       if ( i == o->menu->items ) /* is the first item ? */

          safe.a.x--;

       out = vo->begin_paint(vo, &p, r);

       if ( out ) {

          t_rect  t = rect_assign(r.a.x+o->between.x+2, r.a.y, r.b.x, r.b.y);
          t_rect  f = rect_assign(r.a.x, r.a.y, t.a.x, r.b.y);

          l_color fcolor = color_menu_text;

          if ( !i->name ) { /* draw LINE */

//            vo->background(vo, out, rect_move(r, p.x, p.y));
            rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, color_menu_face);
            //hline(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, color_3d_light);
            //hline(out, r.a.x+p.x, r.b.y+p.y, r.b.x+p.x, color_3d_shadow);

          } else { /* draw ITEM or SUB-MENU */


            if ( !o->is_one_of_subs_state(o, OB_SF_SELECTED) ||
                  item != o->menu->current || !o->inside ) /* not selected item */ {

//              vo->background(vo, out, rect_move(r, p.x, p.y));
            if (!use_skins) // OCR - 02/04/2002
               rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, color_menu_face);
            //hline(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, color_3d_light);
            //hline(out, r.a.x+p.x, r.b.y+p.y, r.b.x+p.x, color_3d_shadow);

            } else { /* selected item */

               rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, color_menu_face);
               //hline(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, color_3d_light);
               //hline(out, r.a.x+p.x, r.b.y+p.y, r.b.x+p.x, color_3d_shadow);
               button(out, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x-1, r.b.y+p.y-1, color_3d_shadow, color_3d_light);
               dawn = 1;
            };

            if ( !i->enable ) /* disable item */

               fcolor = color_3d_shadow;


            if ( !i->message || i->submenu ) { /* it's sub-menu item */

//               textout_draw_rect(out, get_font_in_size("Symbols", 10, 10), "b", -1,
  //                               t.a.x+p.x, t.a.y+p.y, t.b.x+p.x-8, t.b.y+p.y,
    //                             TX_ALIGN_CENTERY|TX_ALIGN_RIGHT, fcolor, TX_NOCOLOR, 0);
            };

            if ( i->flags & MIF_CHECKOK ) { /* draw ok item */

  //             textout_draw_rect(out, get_font_in_size("Symbols", 12, 12), "O", -1,
//                                 f.a.x+p.x, f.a.y+p.y, f.b.x+p.x, f.b.y+p.y,
//                                 TX_ALIGN_CENTER, fcolor, TX_NOCOLOR, 0);

            } else

            if ( i->icon ) { /* draw icon if some is occurs */

               l_rect dy = (rect_sizey(f)-icon_size(o))/2;
               l_rect dx = (rect_sizex(f)-icon_size(o))/2;

//               stretch_sprite(out, i->icon, f.a.x+p.x+dx, f.a.y+p.y+dy, icon_size(o), icon_size(o));

            } else

            if ( i->font_symbol && i->char_symbol > 0 ) { /* if some symbol before text */

               l_char s[2] = {0, '\0'};
               s[0] = i->char_symbol;

               textout_draw_rect(out, i->font_symbol, s, -1,
                                 r.a.x+dawn, r.a.y+dawn, r.b.x+dawn, r.b.y+dawn,
                                 TX_ALIGN_CENTER, fcolor, TX_NOCOLOR, 0);
            };


            if ( i->param ) { /* print param of item f.e. : ALT+X ... */


//               textout_draw_rect(out, i->font, i->param, -1, t.a.x+p.x, t.a.y+p.y,
//                                 t.b.x+p.x-8, t.b.y+p.y, TX_ALIGN_CENTERY|TX_ALIGN_RIGHT, fcolor, TX_NOCOLOR, 0);

            };


            /* print name of item */

            textout_draw_rect(out, i->font, i->name, -1, t.a.x+p.x-12+dawn, t.a.y+p.y+dawn+5, t.b.x+p.x, t.b.y+p.y, TX_ALIGN_TOP|TX_ALIGN_LEFT, fcolor, TX_NOCOLOR, 0);

          }; /* end of item or sub-menu drawing */
       };

       vo->end_of_paint(vo, safe);

    };

  };

};


/* t_menu and t_menuitem functions */

p_menuitem  add_menu_item_to_menuitem ( p_menuitem dst, p_menuitem src )
{

  if ( dst ) {

      p_menuitem last = dst;

      while ( last->next ) last = last->next; /* move to last item */

      last->next = src;

  } else dst = src;

  return dst;

};


l_bool      add_menu_item_to_menu ( p_menu p, l_int pos, p_menuitem items )
{

   p_menuitem first = p?p->items:NULL;

   while ( first && pos > 1 ) {


     pos--;
     first = first->next;

   };

   if ( first && first->next != items ) {

     first->next = items;

     return true;

   };

   return false;
};


p_menu      new_menu ( p_menuitem item )
{
  p_menu p = (p_menu)_malloc(sizeof(t_menu));

  if ( p ) {

    clear_type(p, sizeof(t_menu));

    p->items = item;
    p->current = item;

  };

  return p;

};


p_menuitem  new_menu_line ( p_menuitem next )
{
  p_menuitem p = (p_menuitem)_malloc(sizeof(t_menuitem));

  if ( p ) {

    clear_type(p, sizeof(t_menuitem));

    p->next = next;

    return p;

  };

  return next;

};


p_menuitem  new_sub_menu_ex ( l_text name, l_bool enable, l_text info_text,
                              l_font *font, BITMAP *icon, l_font *font_symbol, l_byte chr,
                              p_menu menu, p_menuitem next )
{

  p_menuitem p;

  if ( name ) {

    p = (p_menuitem)_malloc(sizeof(t_menuitem));

    if ( !p ) return next;

    clear_type(p, sizeof(t_menuitem));

    p->next = next;
    p->name = _strdup(name);
    p->enable = enable;
    p->info_text = _strdup(info_text);
    p->font = font;
    p->submenu = menu;
    p->font_symbol = font_symbol;
    p->char_symbol = chr;

    if ( !font ) p->font = standard_menuitem_font;

    if ( !icon )

       p->icon = NULL; //icon_folder(ICON_16);

    else

       p->icon = icon;

    return p;

  };

  return next;

};


void   free_menu_item ( p_menuitem i )
{
   if ( i ) {

     _free(i->name);
     _free(i->info_text);
     _free(i->param);

     if ( i->flags & MIF_SELFICON )

         destroy_bitmap(i->icon);

   };

  _free(i);
};


void   free_menu ( p_menu i )
{
};


p_menuitem  new_menu_item_ex ( l_text name, l_text param, l_int hotkey, l_dword message,
                               l_bool enable, l_text info_text, l_int flags,
                               l_font *font, BITMAP *icon, l_font *font_symbol, l_byte chr,
                               p_menuitem next )
{
  p_menuitem p;

  if ( name && message != 0 ) {

    p = (p_menuitem)_malloc(sizeof(t_menuitem));

    if ( !p ) return NULL;

    clear_type(p, sizeof(t_menuitem));

    p->next = next;
    p->name = _strdup(name);
    p->message = message;
    p->enable = enable;
    p->hotkey = hotkey;
    p->info_text = _strdup(info_text);
    p->param = _strdup(param);
    p->font = font;
    p->font_symbol = font_symbol;
    p->char_symbol = chr;

    if ( !font ) p->font = standard_menuitem_font;

    p->flags = flags;
    p->icon = icon;

    return p;

  };

  return next;
};



l_int       menu_get_item_flags ( p_menu m, l_dword message )
{

   p_menuitem p = m?m->items:NULL;

   while ( p ) {

     if ( p->submenu ) {

        l_int flags = menu_get_item_flags(p->submenu, message);

        if ( flags != -1 ) return flags;

     } else

     if ( p->message == message ) return p->flags;

     p = p->next;

   };

   return -1;


};


l_long    menu_get_item_pos ( p_menu m, p_menuitem x )
{
   p_menuitem p = m?m->items:NULL;
   l_long i = 0;


   while ( p ) {

     if ( p == x ) return i;

     i++;

     p = p->next;

   };

   return -1;

};


p_menuitem    menu_get_lastitem_called ( p_menu m )
{

   p_menuitem p = m?m->items:NULL;

   while ( p ) {

     if ( p->submenu ) {

        p_menuitem last = menu_get_lastitem_called(p->submenu);

        if ( last ) {

            last->lastcall = false;

            return last;

        };

     } else

     if ( p->lastcall ) {  // Fixed 09/09/2001

       p->lastcall = false;

       return p;

     };

     p = p->next;

   };

   return NULL;

};


l_dword     menu_get_hotkey_message ( p_menu m, l_int keycode )
{

   p_menuitem p = m?m->items:NULL;

   while ( p ) {

     if ( p->submenu ) {

        l_dword msg = menu_get_hotkey_message(p->submenu, keycode);

        if ( msg ) return msg;

     } else

     if ( p->hotkey == keycode ) return p->message;

     p = p->next;

   };

   return MSG_NOTHING;

};


l_int  menu_get_max_item ( p_menu m )
{

  if ( m && m->items ) {

     p_menuitem p = m->items;
     l_int i = 0;

     while ( p ) {

       i++;

       p = p->next;

     };

     return i;

  } else

  return 0;


};


t_point  menu_get_ver_size ( p_menu menu, l_int h )
{

  p_menuitem item = menu?menu->items:NULL;

  l_rect     w = 0;
  l_rect     maxw = 0;
  l_rect     maxh = 0;

  while ( item ) {

     if ( item->name ) {

        w   = FONT_GETSTRWIDTH(item->font, item->name, -1);
        w  += FONT_GETSTRWIDTH(item->font, item->param, -1);
        w  += STANDARD_MENUBOUNDSDIFF_SIZE*2;

        maxw = max(w, maxw);

        maxh += h;

     } else

         maxh += STANDARD_MENULINE_SIZE+1;

     item = item->next;

  };

  return point_assign(maxw+60, maxh+4);

};


t_point  menu_get_hor_size ( p_menu menu, l_int h, l_int icon_size )
{

  p_menuitem item = menu?menu->items:NULL;

  l_rect     w = 0;
  l_rect     maxw = 0;
  l_rect     maxh = 0;

  while ( item ) {

     if ( item->name ) {

        maxw  += icon_size;
        maxw  += FONT_GETSTRWIDTH(item->font, item->name, -1);
        maxw  += FONT_GETSTRWIDTH(item->font, item->param, -1)+STANDARD_MENUDIFF_SIZE;

     } else

         maxw += STANDARD_MENULINE_WIDTH+1;

     item = item->next;

  };

  return point_assign(maxw+4, h+4);

};


/* t_hormenu */


void    hormenu_translate_event ( p_object o, t_event *event )
{
  t_rect r = VIEW(o)->get_local_extent(VIEW(o));
  t_point p = VIEW(o)->get_global_point(VIEW(o), point_assign(0, 0));

  RETVIEW(o, event);

  view_translate_event(o, event); /* old translate_event function */

  if ( event->type & EV_MOUSE &&

       OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) { /* mouse events */
         l_dword msg = 0;

       clear_event(event);

       if (mouse->where.x > r.a.x+p.x && mouse->where.x <= r.a.x+p.x+8) {
         while ( OBJECT(mouse)->state & MO_SF_MOUSELPRESS ) {

            o->get_event(o, event);
         };

         return;
       };

       menu_clear_lastitem_called(MENUVIEW(o)->menu);

       msg = desktop->execute_view(desktop, VIEW(o));

       if ( msg != MSG_NOTHING && msg != MSG_CANCEL ) {

          set_event(event, EV_MESSAGE, msg, o);

       };

       o->put_event(o, event);

       clear_event(event);

       if ( o->owner->prefer == o )  /* only one item, so redraw it for  */

           MENUVIEW(o)->draw_current_item(MENUVIEW(o));

  };

};

p_menuview  _hormenu_init_ex ( p_menuview o, t_rect r, l_rect item_size,
                               l_rect icon_size, p_menu menu )
{
  t_rect   l = desktop?desktop->get_limits(desktop):r;
  t_point  s = menu_get_hor_size(menu, item_size, icon_size+5);

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_menuview));

  /* rectangle settings */

  r.b.y = r.a.y + s.y;

  if ( r.b.x < r.a.x+s.x )  r.b.x = r.a.x + s.x;

  view_init(VIEW(o), r);

  /* object's functions */

  OBJECT(o)->done = &menuview_done;
  OBJECT(o)->execute = &hormenu_execute;
  OBJECT(o)->set_state = &menuview_set_state;
  OBJECT(o)->translate_event = &hormenu_translate_event;


  /* view's functions */

  VIEW(o)->draw = &hormenu_draw;
  VIEW(o)->draw_under_rect = &menuview_draw_under_rect;
  VIEW(o)->icon_size = icon_size;

  if (use_skins)                      // OCR - 02/04/2002
     VIEW(o)->transparent = TRUE;

  /* menuview's functions and declarations */

  o->flags |= MF_HORMENU;
  o->menu = menu;
  o->between = point_assign(icon_size+5, 0);
  o->item_size = item_size;

  o->save_desktop = &hormenu_save_desktop;
  o->is_hor_menu_one_of_parents = &menuview_is_hor_menu_one_of_parents;
  o->draw_current_item = &menuview_draw_current_item;
  o->draw_item = &hormenu_draw_item;
  o->at = &menuview_at;
  o->index_of = &menuview_index_of;
  o->get_item_box_ex = &hormenu_get_item_box_ex;
  o->get_mouseitem_box = &menuview_get_mouseitem_box;
  o->get_item_box = &menuview_get_item_box;
  o->get_next_item = &menuview_get_next_item;
  o->next_item = &menuview_next_item;
  o->prev_item = &menuview_prev_item;
  o->run_new_menu = &menuview_run_new_menu;
  o->get_item_from_mouse = &menuview_get_item_from_mouse;
  o->is_mouse_in_one_of_parent_menu = &menuview_is_mouse_in_one_of_parent_menu;
  o->is_mouse_in_one_of_parent_item = &menuview_is_mouse_in_one_of_parent_item;
  o->is_one_of_subs_state = &menuview_is_one_of_subs_state;
  o->top_menuview = &menuview_top_menuview;


  /* function calls */

//  VIEW(o)->set_palette(VIEW(o), pal_hormenu);

  VIEW(o)->brush.color = color_3d_face;
 /* VIEW(o)->brush.color2 = color_menu_face_gardient;
  VIEW(o)->brush.state |= BRUSH_GRADIENT+BRUSH_GRADIENT_HOR;*/

  return o;
};


p_menuview  _hormenu_init ( p_menuview o, t_rect r, p_menu menu )
{

  return hormenu_init_ex(o, r, standard_menuitem_size, standard_menuicon_size, menu);

};



/* t_menuview */

p_menuview  _menuview_init_ex ( p_menuview o, t_rect r, l_rect item_size,
                                l_rect icon_size, p_menu menu )
{

  t_rect   l = desktop?desktop->get_limits(desktop):r;
  t_point  s = menu_get_ver_size(menu, item_size);

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_menuview));

  /* rectangle settings */

  if ( r.a.x + s.x < l.b.x )  r.b.x = r.a.x + s.x; else r.a.x = l.b.x - s.x;
  if ( r.a.y + s.y < l.b.y )  r.b.y = r.a.y + s.y; else r.a.y = l.b.y - s.y;

  r.b.x = r.a.x + s.x;
  r.b.y = r.a.y + s.y;


  view_init(VIEW(o), r);

  /* object's functions */

  OBJECT(o)->done = &menuview_done;
  OBJECT(o)->execute = &menuview_execute;
  OBJECT(o)->set_state = &menuview_set_state;
  OBJECT(o)->translate_event = &menuview_translate_event;


  /* view's functions */

  VIEW(o)->draw = &menuview_draw;
  VIEW(o)->size_limits = &menuview_size_limits;
  VIEW(o)->draw_under_rect = &menuview_draw_under_rect;
  VIEW(o)->icon_size = icon_size;

  VIEW(o)->brush.state &= ~BRUSH_LARGE3D;

  VIEW(o)->brush.state |= BRUSH_LIGHT3D|BRUSH_DRAWBORDER;

  /* menuview's functions and declarations */

  o->flags |= MF_VERMENU;
  o->menu = menu;
  o->between = point_assign(icon_size+5, 0);
  o->item_size = item_size;

  o->save_desktop = &menuview_save_desktop;
  o->is_hor_menu_one_of_parents = &menuview_is_hor_menu_one_of_parents;
  o->draw_item = &menuview_draw_item;
  o->draw_current_item = &menuview_draw_current_item;
  o->at = &menuview_at;
  o->index_of = &menuview_index_of;
  o->get_item_box_ex = &menuview_get_item_box_ex;
  o->get_mouseitem_box = &menuview_get_mouseitem_box;
  o->get_item_box = &menuview_get_item_box;
  o->get_next_item = &menuview_get_next_item;
  o->next_item = &menuview_next_item;
  o->prev_item = &menuview_prev_item;
  o->run_new_menu = &menuview_run_new_menu;
  o->get_item_from_mouse = &menuview_get_item_from_mouse;
  o->is_mouse_in_one_of_parent_menu = &menuview_is_mouse_in_one_of_parent_menu;
  o->is_mouse_in_one_of_parent_item = &menuview_is_mouse_in_one_of_parent_item;
  o->is_one_of_subs_state = &menuview_is_one_of_subs_state;

  o->top_menuview = &menuview_top_menuview;


  /* function calls */

  OBJECT(o)->options |= OB_OF_TOPSELECT;

//  VIEW(o)->set_palette(VIEW(o), pal_menu);

  switch ( translucide ) {
    case 1 : VIEW(o)->trans_effect = 235; break;
    case 2 : VIEW(o)->trans_effect = 215; break;
    case 3 : VIEW(o)->trans_effect = 195; break;
    case 4 : VIEW(o)->trans_effect = 175; break;
    case 5 : VIEW(o)->trans_effect = 155; break;
  };


  VIEW(o)->brush.color = color_menu_face;

  VIEW(o)->brush.color2 = color_menu_face_gardient;

  VIEW(o)->brush.state |= BRUSH_GRADIENT+BRUSH_GRADIENT_HOR;

  return o;
};


p_menuview  _menuview_init ( p_menuview o, t_rect r, p_menu menu )
{

  return menuview_init_ex(o, r, standard_menuitem_size, standard_menuicon_size, menu);

};

