/****************************************************************/
/*                                                              */
/*                          dialogs.c                           */
/*                                                              */
/*                Seal - free desktop environment               */
/*                                                              */
/*                    Copyright (c) 1999,2000                   */
/*                         Michal Stencl                        */
/*                     All Rights Reserved                      */
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


#include <stdio.h>
#include <stdarg.h>
#include"allegro.h"
#include"seal.h"
#include"dialogs.h"
#include<math.h>

l_color  pal_stattext[] = {CO_SYSx20, CO_BLACK, CO_NOCOLOR};
l_color  pal_textline[] = {CO_WHITE, CO_SYSx20, CO_BLACK, CO_BLUE, CO_WHITE, CO_BLUE, CO_WHITE, CO_NOCOLOR};
l_color  pal_listbox[] = {CO_WHITE, CO_SYSx20, CO_BLACK, CO_BLUE, CO_WHITE, CO_RED, CO_BLUE, CO_BLUE, CO_WHITE, CO_NOCOLOR};
l_color  pal_process[] = {CO_WHITE, CO_BLACK, CO_BLACK, CO_WHITE, CO_NOCOLOR};

p_stattext (*stattext_init_ex) ( p_stattext o, t_rect r, l_int align, l_text text, va_list argp ) = &_stattext_init_ex;
p_stattext (*stattext_init) ( p_stattext o, t_rect r, l_int align, l_text text, ... ) = &_stattext_init;

p_process   (*process_init) ( p_process o, t_rect r, l_dword size, l_dword *where ) = &_process_init;
p_history   (*history_init) ( p_history o, t_rect r, p_list list, l_int limit, l_int flags ) = &_history_init;
p_listbox   (*listbox_init) ( p_listbox o, t_rect r, p_list list, l_byte cells, l_int flags ) = &_listbox_init;
p_listbox   (*worklistbox_init) ( p_listbox o, t_rect r, p_list list, l_byte cells, l_int flags ) = &_worklistbox_init;
p_textline  (*textline_init) ( p_textline o, t_rect r, l_int limit, l_int flags ) = &_textline_init;
p_textline  (*worktextline_init) ( p_textline o, t_rect r, l_int limit ) = &_worktextline_init;


/* t_process functions */

void   process_draw ( p_view o )
{
  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    button(out, p.x+r.a.x, p.y+r.a.y, p.x+r.b.x, p.y+r.b.y, COLOR(CO_BLACK), COLOR(CO_WHITE));

    PROCESS(o)->draw_process(PROCESS(o));

  };

  o->end_of_paint(o, r);
};


t_rect process_size_limits ( p_view o )
{

   return rect_assign(1, 1, rect_sizex(o->bounds)-1, rect_sizey(o->bounds)-1);

};


void   process_draw_process ( p_process o )
{

  p_view  vo = VIEW(o);
  t_rect  r = vo->size_limits(vo);
  t_point p;

  BITMAP *out = vo->begin_paint(vo, &p, r);

  if ( out ) {

      l_color fcol2 = vo->get_color(vo, 2);
      l_color fcol3 = vo->get_color(vo, 3);
      l_int   dx = (l_int)(((double)rect_sizex(r)/(double)1000)*(double)o->promile);

      vo->background(vo, out, rect_move(r, p.x, p.y));

      if ( dx > 0 )

         fade_rect(out, r.a.x+p.x, r.a.y+p.y, r.a.x+p.x+dx, r.b.y+p.y, fcol2, fcol3, FR_VER);

  };

  vo->end_of_paint(vo, r);

};


void   process_rewrite_promile ( p_process o, l_dword where )
{
   l_int  oldp = o->promile;
   l_rect sx   = rect_sizex(VIEW(o)->size_limits(VIEW(o)));

   where = lmax(0, lmin(where, o->size));

   o->promile = max(0, min(1000, (l_int)(((double)where/(double)lmax(1, o->size))*(double)1000)));

   if ( (l_int)(((double)sx/(double)1000)*(double)oldp) !=
        (l_int)(((double)sx/(double)1000)*(double)o->promile) )

       o->draw_process(o);

};


void   process_func_callback ( p_object o )
{

  if ( PROCESS(o)->where )

      PROCESS(o)->rewrite_promile(PROCESS(o), *(PROCESS(o)->where));

};


void   process_translate_event ( p_object o, t_event *event )
{

  RETVIEW(o, event);

};


/* t_history functions */


/*
  change bounds together with history line
*/
void   history_change_bounds ( p_view o, t_rect nr )
{

  view_change_bounds(o, nr);

  if ( HISTORY(o)->button ) { /* exist button */

    p_view vb = VIEW(HISTORY(o)->button);

    t_rect r  = o->bounds; /* get history bounds */

    /* change button bounds */
    vb->change_bounds(vb, rect_assign(r.b.x+1, r.a.y, r.b.x+1+STANDARD_HISTORYBUTTON_SIZE, r.b.y));

  };

};


void   history_setup ( p_object o )
{
   p_object owner;

   view_setup(o);

   owner = o->owner_view(o);

   if ( owner ) {

     p_history his = HISTORY(o);

     if ( HISTORY(o)->list ) /* set text in text line to first item */

        TEXTLINE(o)->set_text(TEXTLINE(o), (l_text)LISTBOX_ITEM_TEXT(HISTORY(o)->list, his->current));

     if ( !his->button ) { /* make history button */

        t_rect r = VIEW(o)->bounds;

        r = rect_assign(r.b.x+1, r.a.y, r.b.x+1+STANDARD_HISTORYBUTTON_SIZE, r.b.y);

        his->button = button_init(BUTTON(_malloc(sizeof(t_button))), r, "g", MSG_HISTORY, BF_NORMAL);

        if ( his->button ) {

           OBJECT(his->button)->set_options(OBJECT(his->button), OB_OF_SELECTABLE, false);

           his->button->flags |= BF_DARROUND;

           VIEW(his->button)->font = get_font_in_size("Symbols", 8, 8);

        };

        owner->insert(owner, OBJECT(his->button));

     };

   };

};


void   history_translate_event ( p_object o, t_event *event )
{

  RETVIEW(o, event);

  if ( event->type & EV_MOUSE && !is_wable(TEXTLINE(o)) ) { /* mouse event */

    if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) { /* select */

      clear_event(event);

      if ( !o->is_state(o, OB_SF_SELECTED) ) {

         o->select(o);

      };

      HISTORY(o)->show_box(HISTORY(o));

    };

  };

  textline_translate_event(o, event);

                                  /* keyboard events */
  if ( event->type & EV_KEYBOARD && o->is_state(o, OB_SF_FOCUSED) ) {

    if ( OBJECT(keyb)->state & KB_SF_KEYDOWN )

    switch ( KEY_TO(keyb->code) ) {

      case KB_DOWN : {  /* arrow DOWN was pressed */

        clear_event(event);

        HISTORY(o)->show_box(HISTORY(o));

        clear_event(event);

      }; break;

    };
  };

  /* history button was pressed */
  if ( event->type & EV_MESSAGE && event->message == MSG_HISTORY &&
       event->obj && event->obj == OBJECT(HISTORY(o)->button) ) {

        clear_event(event);

        HISTORY(o)->show_box(HISTORY(o));

  };

};


l_dword  history_listbox_execute ( p_object o )
{

  p_scroller lb = SCROLLER(o);

  p_view hb  = VIEW(lb->horbar);
  p_view vb  = VIEW(lb->verbar);

  l_int end = 0;

  LISTBOX(lb)->rewrite_item(LISTBOX(lb), LISTBOX(lb)->current);

  while ( !end ) { /* not end command */

      if ( event_main.type & EV_KEYBOARD && KEYPRESSED(TO_KEY(KB_ESC)) )

         end = 1;

      if ( event_main.type & EV_KEYBOARD && KEYPRESSED(TO_KEY(KB_ENTER)) )

         end = 2;


      if ( event_main.type & EV_MOUSE ) {

         if ( ( hb && hb->is_mouse_in_view(hb) ) ||
              ( vb && vb->is_mouse_in_view(vb) ) ) {

           end = 0;

         } else

         if ( VIEW(lb)->is_mouse_in_view(VIEW(lb)) ) {

             if ( OBJECT(mouse)->state & MO_SF_MOUSEUP )

                end = 4;

        } else

          if ( OBJECT(mouse)->state & MO_SF_MOUSEDOWN ) end = 3;

      };

      INTMAIN(&event_main);

      if ( end ) {

        clear_event(&event_main);

        if ( end == 2 || end == 4 ) return MSG_OK;

        return MSG_CANCEL;

      };

      o->get_event(o, &event_main);

  };

  return MSG_CANCEL;

};


l_dword   history_show_box ( p_history o )
{

  l_dword msg = MSG_CANCEL;

  if ( !o->listbox && o->list ) {

    p_view vo = VIEW(o);

    l_int   by = 3;
    t_rect  r = vo->get_global_bounds(vo, vo->get_local_extent(vo));
    l_long  items = o->list->get_max_item(o->list);
    l_rect  sizey = 0;

    items = lmin((items+1), STANDARD_HISTORYBOX_ITEMS);

    sizey = (max(icon_size(o), FONT_GETHEIGHT(vo->font))+by)*items + 4;


    if ( r.b.y + sizey < desktop->bounds.b.y ) /* max y of listbox is lower then desktop max y */

      r = rect_assign(r.a.x, r.b.y, r.b.x, r.b.y + sizey);

    else /* max y of listbox is greater then desktop max y */

      r = rect_assign(r.a.x, r.a.y - sizey, r.b.x, r.a.y);


    o->listbox = worklistbox_init((p_listbox)_malloc(sizeof(t_listbox)), r, o->list, 1, LF_UNDERSEL);

    if ( o->listbox ) {

        OBJECT(o->listbox)->execute = o->listbox_execute;
        OBJECT(o->listbox)->set_options(OBJECT(o->listbox), OB_OF_TOPSELECT, true);

        VIEW(o->listbox)->font = VIEW(o)->font; /* set font of box to history font */

        o->listbox->current = o->current;

        o->listbox->between.y = by;  /* set distance between texts in box */

    };

    OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(o->listbox));

    msg = desktop->execute_view(desktop, VIEW(o->listbox));

    if ( msg == MSG_OK )

       o->rewrite_item(o, o->listbox->current); /* set history item to selected listbox item */

    o->listbox = LISTBOX(dispose(OBJECT(o->listbox))); /* free listbox */

  };

  return msg;

};


l_bool history_set_list ( p_history o, p_list p )
{

   o->list = p;

   o->current = 0;

   if ( o->list )

       TEXTLINE(o)->set_text(TEXTLINE(o), (l_text)LISTBOX_ITEM_TEXT(o->list, 0));

   return true;
};


void   history_rewrite_item ( p_history o, l_long item )
{

  if ( o->list ) {

     l_long old_current = o->current;

     item = lmax(0, lmin(o->list->get_max_item(o->list), item));

     o->current = item;

     if ( o->current != old_current )

       TEXTLINE(o)->set_text(TEXTLINE(o), (l_text)LISTBOX_ITEM_TEXT(o->list, item));

  };

};



/* listbox functions */

void   listbox_change_bounds ( p_view o, t_rect nr )
{

   l_long oldc = LISTBOX(o)->current;

   LISTBOX(o)->current = LISTBOX(o)->from = SCROLLER(o)->scrollx = SCROLLER(o)->scrolly = 0;

   scroller_change_bounds(o, nr);

   LISTBOX(o)->rewrite_item(LISTBOX(o), oldc);

};


void   listbox_translate_event ( p_object o, t_event *event )
{

  RETVIEW(o, event);

  view_translate_event(o, event); /* old translate_event function */

  if ( keyb->shifts & KB_SHIFT_FLAG ) {

     if ( !(LISTBOX(o)->shift_flag & 0x01) ) { /* now was pressed shift */

        LISTBOX(o)->shift_flag |= 0x01;

        if ( LISTBOX(o)->shift_flag & 0x02 ) { /* deselect process */

            LISTBOX(o)->shift_flag &= ~0x02;
            LISTBOX(o)->shift_flag |=  0x04;

        } else { /* select proces */

            LISTBOX(o)->shift_flag &= ~0x04;
            LISTBOX(o)->shift_flag |=  0x02;

        };

     };

  } else

    LISTBOX(o)->shift_flag &= ~0x01; /* not shift */

  if ( event->type & EV_MOUSE ) { /* mouse event */

    l_long mpos;
    l_bool oksel = ( LISTBOX(o)->flags & LF_UNDERSEL ) ? true : OBJECT(mouse)->state & MO_SF_MOUSELPRESS;

    if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) { /* select */

      if ( !o->is_state(o, OB_SF_SELECTED) ) {

         clear_event(event);

         o->select(o);

         return;

      };

    };


    if ( oksel && o->is_state(o, OB_SF_FOCUSED) ) {

       mpos = LISTBOX(o)->get_pos_from_xy(LISTBOX(o), VIEW(o)->get_local_point(VIEW(o), mouse->where));

       if ( LISTBOX(o)->flags & LF_SELECTABLE &&
            LISTBOX(o)->shift_flag & 0x01 ) { /* select item */

         p_listbox_item p = (p_listbox_item)LISTBOX_ITEM_AT(LISTBOX(o)->list, mpos);

         if ( p )

            if ( LISTBOX(o)->shift_flag & 0x04 ) p->sel = false;
            else
            if ( LISTBOX(o)->shift_flag & 0x02 ) p->sel = true;

       };

       LISTBOX(o)->rewrite_item(LISTBOX(o), mpos);

       clear_event(event);

    };

  };

  if ( o->is_state(o, OB_SF_FOCUSED) )

                                  /* keyboard events */
  if ( event->type & EV_KEYBOARD ) {

    if ( OBJECT(keyb)->state & KB_SF_KEYDOWN )

    switch ( KEY_TO(keyb->code) ) {

      case KB_DOWN : {  /* arrow DOWN was pressed */

        LISTBOX(o)->rewrite_item(LISTBOX(o), LISTBOX(o)->current+1);

        clear_event(event);

      }; break;

      case KB_UP : {  /* arrow UP was pressed */

        LISTBOX(o)->rewrite_item(LISTBOX(o), LISTBOX(o)->current-1);

        clear_event(event);

      }; break;

      case KB_RIGHT : {  /* arrow RIGHT was pressed */

        LISTBOX(o)->rewrite_item(LISTBOX(o), LISTBOX(o)->current+LISTBOX(o)->get_rows(LISTBOX(o)));

        clear_event(event);

      }; break;

      case KB_LEFT : {  /* arrow LEFT was pressed */

        LISTBOX(o)->rewrite_item(LISTBOX(o), LISTBOX(o)->current-LISTBOX(o)->get_rows(LISTBOX(o)));

        clear_event(event);

      }; break;

      case KB_HOME : {  /* key HOME was pressed */

        LISTBOX(o)->rewrite_item(LISTBOX(o), 0);

        clear_event(event);

      }; break;

      case KB_END : {  /* key END was pressed */

        LISTBOX(o)->rewrite_item(LISTBOX(o), LISTBOX(o)->get_max(LISTBOX(o)));

        clear_event(event);

      }; break;
    };
  };

};


l_bool   listbox_done ( p_object o )
{

  return scroller_done ( o );

};


void  listbox_set_state ( p_object o, l_dword st, l_bool set )
{
  view_set_state(o, st, set);

  if ( st & OB_SF_FOCUSED || st & OB_SF_SELECTED ) {

    LISTBOX(o)->draw_item(LISTBOX(o), LISTBOX(o)->current, true);

  };

};


void  listbox_set_options ( p_object o, l_dword op, l_bool set )
{

  obj_set_options(o, op, set);

  if ( op & OB_OF_ENABLE ) {

    VIEW(o)->draw_view(VIEW(o));

  };

};


l_bool listbox_get_data ( p_object o, t_data *rec )
{

  return obj_get_data(o, rec);

};


l_bool listbox_set_data ( p_object o, t_data *rec )
{

  return obj_set_data(o, rec);

};


void   listbox_draw ( p_view o )
{
  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    button(out, p.x+r.a.x, p.y+r.a.y, p.x+r.b.x, p.y+r.b.y, COLOR(CO_BLACK), COLOR(CO_WHITE));
    button(out, p.x+r.a.x+1, p.y+r.a.y+1, p.x+r.b.x-1, p.y+r.b.y-1, COLOR(CO_SYSx18), COLOR(CO_SYSx24));

    LISTBOX(o)->draw_box(LISTBOX(o));

  };

  o->end_of_paint(o, r);

};


t_rect listbox_size_limits ( p_view o )
{

  return rect_assign(2, 2, rect_sizex(o->bounds)-2, rect_sizey(o->bounds)-2);

};


t_rect listbox_scroll_limits ( p_scroller o )
{

  t_rect r = VIEW(o)->size_limits(VIEW(o));

  r.b.y = r.a.y+LISTBOX(o)->get_rows(LISTBOX(o))*LISTBOX(o)->get_item_size(LISTBOX(o)).y;
  r.b.x = r.a.x+LISTBOX(o)->cells*LISTBOX(o)->get_item_size(LISTBOX(o)).x;

  return r;

};


void   listbox_scroll_size ( p_scroller o, l_long *x, l_long *y )
{
  p_listbox lo = LISTBOX(o);

  SCROLLER(o)->deltax = lo->get_item_size(lo).x;
  SCROLLER(o)->deltay = lo->get_item_size(lo).y;

  scroller_scroll_size(o, x, y);

  if ( lo->cells > 1 ) {

    l_long oldx = *x;

    *x = (l_int)ceil((double)(lo->get_max(lo)+1)/(double)lo->get_rows(lo))*lo->get_item_size(lo).x;
    *x = lmax(*x, oldx);

  } else {

    l_long oldy = *y;

    *y = (lo->get_max(lo)+1)*lo->get_item_size(lo).y;
    *y = lmax(*y, oldy);

  };

};


void   listbox_recalc_positions ( p_scroller o, l_long x, l_long y )
{

  p_listbox lb = LISTBOX(o);

  if ( x )

    lb->from += (x/lb->get_item_size(lb).x)*lb->get_rows(lb); /* set o->from */

  else
  if ( y )

    lb->from += y/lb->get_item_size(lb).y; /* set o->from */

};


p_list listbox_get_selected_items ( p_listbox o, l_bool sel )
{

  p_list list = NULL;

  if ( o->list && o->get_max(o) >= 0 ) {

      list = list_init(_malloc(sizeof(t_list)), NULL, DAT_TEXT);

      if ( list ) {

         p_item i = o->list->last;
         p_item l = i;

         if ( i )

         do {

            if ( ((p_listbox_item)i->rec)->sel == sel )

                 list->insert(list, _strdup(((p_listbox_item)i->rec)->name));

            i = i->next;

         } while ( i != l );

      };

  };

  return list;

};


l_long listbox_get_pos_from_xy ( p_listbox o, t_point p )
{

  l_long  pos = 0;
  l_long  delta = 0;
  l_int   rows = o->get_rows(o);
  t_point s   = o->get_item_size(o);

  if ( o->cells < 2 ) p.x = 0;

  delta = lmin(o->get_max_in_box(o)-1, lmax(0, (min(o->cells, p.x/s.x)*rows+min(rows-1,(p.y/s.y)))));

  pos = o->from+delta;

  return pos;

};


void   listbox_draw_item_ptr ( p_listbox o, l_long item, void *recitem, l_bool set )
{

  p_view vo = VIEW(o);
  t_point p;

  l_text    item_text = recitem?((p_listbox_item)recitem)->name:NULL;
  BITMAP   *item_icon = recitem?((p_listbox_item)recitem)->icon:NULL;
  l_bool    item_sel  = recitem?((p_listbox_item)recitem)->sel:false;

  t_rect  r = o->get_item_rect(o, item);

  t_rect  t = r;
  t_rect  safe = r;

  BITMAP *out;

  if ( !recitem || rect_check_empty(r) ) return;

  out = vo->begin_paint(vo, &p, safe);

  if ( out ) {

    l_color bcolor = vo->get_color(vo, 1);
    l_color fcolor = vo->get_color(vo, 2);

    if ( OBJECT(o)->is_options(OBJECT(o), OB_OF_ENABLE) ) {

      if ( set && !item_sel ) {

        bcolor = vo->get_color(vo, 7);
        fcolor = vo->get_color(vo, 8);

        if ( OBJECT(o)->is_state(OBJECT(o), OB_SF_SELECTED) ) {

            bcolor = vo->get_color(vo, 3);
            fcolor = vo->get_color(vo, 4);

        };

        rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, bcolor);

        if ( OBJECT(o)->is_state(OBJECT(o), OB_SF_SELECTED) )

               button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_WHITE), COLOR(CO_BLACK));

      } else

        if ( item_sel ) {

           bcolor = vo->get_color(vo, 5);
           fcolor = vo->get_color(vo, 6);

           rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, bcolor);

           if ( set )

              button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_BLACK), COLOR(CO_WHITE));

        } else

          vo->background(vo, out, rect_move(r, p.x, p.y));

    } else

      rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, bcolor);

    if ( item_icon ) {

       l_int dy = (rect_sizey(r)-icon_size(o))/2;

       stretch_sprite(out, item_icon, r.a.x+p.x+5, r.a.y+p.y+dy, icon_size(o), icon_size(o));

       t.a.x = r.a.x+rect_sizey(r)+5;

    };


    if ( item_text )

      textout_draw_rect(out, vo->font, item_text, -1, t.a.x+p.x+5, r.a.y+p.y,
                             r.b.x+p.x, r.b.y+p.y, TX_ALIGN_LEFT|TX_ALIGN_CENTERY,
                             fcolor, TX_NOCOLOR, 1);


  };

  vo->end_of_paint(vo, safe);

};


void   listbox_draw_item ( p_listbox o, l_long item, l_bool set )
{

  void *recitem = LISTBOX_ITEM_AT(o->list, item);

  o->draw_item_ptr(o, item, recitem, set);

};


void   listbox_draw_box ( p_listbox o )
{
  p_view  vo = VIEW(o);

  t_rect  r = vo->size_limits(vo);
  t_point p;
  BITMAP *out;

  out = vo->begin_paint(vo, &p, r);

  if ( out ) {

    l_long  max = o->from+o->get_max_in_box(o);
    l_long  abs_max = o->get_max(o);
    l_long  i   = o->from;
    p_item  it  = o->list?o->list->at_item(o->list, i):NULL;

    if ( OBJECT(o)->is_options(OBJECT(o), OB_OF_ENABLE) )

      vo->background(vo, out, rect_move(r, p.x, p.y));

    else

      rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, vo->get_color(vo, 1));


    if ( it )

      while ( i < max && i <= abs_max ) {

        o->draw_item_ptr(o, i, it?(l_text)it->rec:NULL, (i == o->current));

        it = it->next;

        i++;

      };

  };

  vo->end_of_paint(vo, r);

};


/*
  return rect area of item ( item )
*/
t_rect   listbox_get_item_rect ( p_listbox o, l_long item )
{
  p_view vo = VIEW(o);
  t_rect s = vo->size_limits(vo);
  l_int  rows = o->get_rows ( o );

  l_int xpos = (o->cells>1)?(item-o->from)/rows:0;
  l_int ypos = (o->cells>1)?(item-o->from)%rows:item-o->from;
  t_point item_size = o->get_item_size ( o );

  l_rect dx = xpos*item_size.x;
  l_rect dy = ypos*item_size.y;

  t_rect  r = (o->cells>1)?rect_assign(s.a.x+dx, s.a.y+dy, s.a.x+dx+item_size.x-o->between.x, s.a.y+dy+item_size.y-1):
                           rect_assign(s.a.x+dx, s.a.y+dy, s.b.x, s.a.y+dy+item_size.y-1);

  return rect_cliped(s, r);
};


/*
  listbox rewrite item :

  it rewrite item and control all item moves.

  - item is new position of listbox item.


  It clip item, so it make max(item, 0) and min(item, get_max_item(o))

  If item is greater than [o->from - the first item that we see in current pos,
  so it's left upper item] + [get_max_in_box(o) max number of items
  in one listbox, that we can see. ], it scroll listbox to the right, if item
  is lower than o->from, it scroll listbox to the left.

*/


void   listbox_rewrite_item ( p_listbox o, l_long item )
{

  l_long dx = 0;
  l_long dy = 0;
  l_long abs_cur = 0;

  l_long old_current = o->current;
  l_long max = o->get_max(o);
  l_int  max_box = o->get_max_in_box(o);

  l_bool can_scroll = false;

  o->current = lmax(0, lmin(item, max));

  if ( o->current >= max_box+o->from ) { /* is greater than box */

    l_int  rows = o->get_rows(o);
    l_long old_from = o->from;
                                         /* current+1, because it starts from 0 and rows may be 1,2,3... not 0,1,2,3 */
    abs_cur = (l_int)ceil((double)(o->current+1)/(double)rows)*rows;

    if ( o->cells == 1 ) /* other type of scrolling in 1 cell */

       abs_cur = o->current+1;

    abs_cur -= max_box; /* set o->from */

    if ( o->cells == 1 )  /* other type of scrolling in 1 cell */

      dy = (abs_cur-old_from)*o->get_item_size(o).y;

    else

      dx = ((abs_cur-old_from)/rows)*o->get_item_size(o).x;

    can_scroll = true;

  } else

  if ( o->current < o->from ) { /* is lower than box */

    l_int  rows = o->get_rows(o);
    l_long old_from = o->from;

    abs_cur = (l_int)floor((double)(o->current)/(double)rows)*rows;

    if ( o->cells == 1 ) /* other type of scrolling in 1 cell */

       abs_cur = o->current;


    if ( o->cells == 1 )  /* other type of scrolling in 1 cell */

      dy = (abs_cur-old_from)*o->get_item_size(o).y;

    else

      dx = ((abs_cur-old_from)/rows)*o->get_item_size(o).x;

    can_scroll = true;

  };

  if ( o->current != old_current || can_scroll ) {

    o->draw_item(o, old_current, false);

    if ( !can_scroll )

      o->draw_item(o, o->current, true);

  };

  if ( can_scroll ) {

    SCROLLER(o)->scroll_place(SCROLLER(o), dx, dy, SCROLL_NOW); /* scroll place */

    o->draw_item(o, o->current, true);

  };

};


l_long  listbox_get_max ( p_listbox o )
{

  if ( o->list ) return o->list->get_max_item(o->list);

  return -1;

};


t_point listbox_get_item_size ( p_listbox o )
{

  l_rect  sx = rect_sizex(VIEW(o)->size_limits(VIEW(o)));

  t_point p = point_assign(max(1, sx/o->cells), max(1, max(FONT_GETHEIGHT(VIEW(o)->font), icon_size(o))+o->between.y));

  return p;
};


l_int  listbox_get_rows ( p_listbox o )
{

  l_int  sizey_item = o->get_item_size(o).y;
  t_rect s = VIEW(o)->size_limits(VIEW(o));

  return (rect_sizey(s)/max(1, sizey_item));

};


l_bool   listbox_set_list ( p_listbox o, p_list p )
{

   o->list = p;

   o->current = o->from = 0;

   /* redraw box */
   o->draw_box(o);

   /* redraw scroll bars */
   draw_scroller_bars(o);

   return true;

};


l_int  listbox_get_max_in_box ( p_listbox o )
{

  return (o->get_rows(o)*o->cells);

};



/* t_worklistbox */


t_rect worklistbox_size_limits ( p_view o )
{

  return rect_assign(1, 1, rect_sizex(o->bounds)-1, rect_sizey(o->bounds)-1);

};


void  worklistbox_draw ( p_view o )
{

  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    rect(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_BLACK));

    LISTBOX(o)->draw_box(LISTBOX(o));

  };

  o->end_of_paint(o, r);

};



/* textline functions */

void  textline_ins_text ( p_textline o, l_int pos, l_text text )
{
  if ( pos < o->limit && pos >= 0 )

    o->text = insstr(o->text, text, pos, min(o->limit-pos, strlen(text)));
};


void  textline_ins_char ( p_textline o, l_int pos, l_char chr )
{
  l_char txt[2] = {0, '\0'};
  txt[0] = chr;

  o->ins_text(o, pos, txt);

};


void  textline_del_text ( p_textline o, l_int pos, l_int size )
{

  if ( pos < o->limit && pos >= 0 )

    o->text = delstr(o->text, pos, min(o->limit-pos, size));

};


void  textline_del_char ( p_textline o, l_int pos )
{
  o->del_text(o, pos, 1);
};


l_bool  textline_done ( p_object o )
{

  if ( !view_done(o) ) return false;

  afree((void**)(&(TEXTLINE(o)->text)));

  return true;

};


void  textline_set_state ( p_object o, l_dword st, l_bool set )
{
  view_set_state(o, st, set);

  if ( st & OB_SF_SELECTED ) {

    if ( set || o->owner && o->owner->is_state(o->owner, OB_SF_SELECTED) ) {

       TEXTLINE(o)->sel_all(TEXTLINE(o), 0, 0);

       if ( set ) TEXTLINE(o)->sel_all(TEXTLINE(o), 0, -1);

    };

    VIEW(o)->draw_view(VIEW(o));

    TEXTLINE(o)->show_cursor(TEXTLINE(o), set);

  };

};


void  textline_set_options ( p_object o, l_dword op, l_bool set )
{

  obj_set_options(o, op, set);

  if ( op & OB_OF_ENABLE ) {

    TEXTLINE(o)->sel_all(TEXTLINE(o), 0, 0);

    VIEW(o)->draw_view(VIEW(o));

  };

};


void  textline_draw ( p_view o )
{

  t_rect  r = o->get_local_extent(o);
  t_point p = o->get_global_point(o, r.a);

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    button(out, p.x+r.a.x, p.y+r.a.y, p.x+r.b.x, p.y+r.b.y, COLOR(CO_BLACK), COLOR(CO_WHITE));
    button(out, p.x+r.a.x+1, p.y+r.a.y+1, p.x+r.b.x-1, p.y+r.b.y-1, COLOR(CO_SYSx18), COLOR(CO_SYSx24));

    TEXTLINE(o)->draw_text(TEXTLINE(o));
    TEXTLINE(o)->draw_cursor(TEXTLINE(o), -1, -1);

  };

  o->end_of_paint(o, r);

};


/*

  Get data from textline, if textline is selected from some pos1 to pos2
  it return this selected text if rec->style comes as DS_SELECTED. If pos1
  and pos2 are same = there is no selected area, it store all text to
  rec->data.

  This function return DAT_TEXT id if some of styles are support.

  Styles supported by textline :

         DS_SELECTED - rec->data is set to selected text, if some is,
                       else set all text
         DS_ALL      - set rec->data to all text
         DS_WHATEVER - set rec->data to text, which textline want to send
                       [ if some selected it return this, else return all,
                         same as DS_SELECTED in this case ]

*/

l_bool textline_get_data ( p_object o, t_data *rec )
{
  if ( rec ) {

     rec->info_obj = o;

     l_tag_cpy(rec->id, DAT_TEXT);

     switch ( rec->style ) {

       case DS_SELECTED : {

          p_textline tl = TEXTLINE(o);

          if ( is_sel(tl) )

            rec->data = (void *)stridup(&(tl->text[tl->sel_from]), tl->sel_to-tl->sel_from);

          else

            rec->data = (void *)_strdup(tl->text);

          return true;

       }; break;

       case DS_ALL : {

          rec->data = (void *)_strdup(TEXTLINE(o)->text);

          return true;

       }; break;

       case DS_WHATEVER : {

          rec->style = DS_SELECTED;

          return o->get_data(o, rec);

       }; break;

     };

     l_tag_cpy(rec->id, DAT_NONE);

  };

  return false;

};


/*
   drag_where control draging process, that's defined in t_view structure.

   - First is called drag_data, when CTRL+mouse button is pressed. This set
     new cursor and wait for unpressed button. While this operation run,
     view_drag_data function call drag_where function for redrawing objects,
     that placed right under mouse cursor.
*/

l_bool textline_drag_where ( p_view o, t_data *rec, t_point where )
{
   /*
      if mouse is under this object in drag_data function &&
      rec->id == OBJECT(o)->data_type then redraw cursor
   */
   if ( view_drag_where(o, rec, where) ) {

       l_int mpos = TEXTLINE(o)->get_pos_from_xy(TEXTLINE(o), where);

       if ( is_sel(TEXTLINE(o)) )

            TEXTLINE(o)->sel_all(TEXTLINE(o), 0, 0);

       if ( !TEXTLINE(o)->cursor_visible )

            TEXTLINE(o)->show_cursor(TEXTLINE(o), true);

       if ( mpos != TEXTLINE(o)->pos )

            TEXTLINE(o)->redraw_text(TEXTLINE(o), mpos, 0);

       return true;

   } else /* Out of place || old */

   if ( OBJECT(o)->is_state(OBJECT(o), OB_SF_FOCUSED) ) {

       if ( rec ) TEXTLINE(o)->show_cursor(TEXTLINE(o), false);

       else  /* give things to right place, I'm focused so my cursor must be showed */

          TEXTLINE(o)->show_cursor(TEXTLINE(o), true);

   } else

     TEXTLINE(o)->show_cursor(TEXTLINE(o), false);

   return false;

};


/*
   set_data

   - set data to textline in these cases :

     o   textline is writeable [ is_wable(o) ],
     o   rec->id is one of textline->data_type-s ( standard is DAT_TEXT )
     o   rec->info_obj != this object, it means data are not from this object
*/

l_bool textline_set_data ( p_object o, t_data *rec )
{
  if ( rec && is_wable(TEXTLINE(o)) &&
       l_tag_cmp(o->data_type, rec->id) && rec->info_obj != o ) {

     p_textline tl  = TEXTLINE(o);
     l_bool was_sel = false;
     l_bool ok = true;

     if ( rec->style & DS_DELETE ) { /* if data is deleted */

       if ( rec->style & DS_ALL ) {

         return tl->set_text(tl, NULL);  /* delete all text */

       } else {

         if ( is_sel(tl) ) { /* delete only selected text */

            tl->redraw_text(tl, tl->pos, TO_KEY(KB_DEL));

            ok = true;

         };

         ok = false;
       };


     } else { /* if data is inserted to */

       if ( is_sel(tl) ) {

          tl->redraw_text(tl, tl->pos, TO_KEY(KB_DEL));

       };

       tl->ins_text(tl, tl->pos, (l_text)rec->data);

     };

     sel_clear(tl);

     set_format_text(&(VIEW(tl)->info_text), "%s: %i\n%s: '%s'", TXT_SIZE, strlen(tl->text), TXT_TEXT, tl->text);

     if ( !was_sel ) {

       tl->draw_text(tl);

       tl->draw_cursor(tl, -1, -1);

     };

     return ok;
  };

  return false;
};



l_bool textline_set_text ( p_textline o, l_text text )
{

  l_bool ret = false;

  _free(o->text);

  if ( text ) {

      l_int maxlen = strlen(text);

      if ( maxlen < o->limit ) ret = true;

      maxlen = min(maxlen, o->limit);

      o->text = stridup(text, maxlen);

  } else o->text = NULL;

  sel_clear(o);

  set_format_text(&(VIEW(o)->info_text), "%s: %i\n%s: '%s'", TXT_SIZE, strlen(o->text), TXT_TEXT, o->text);

  o->draw_text(o);
  o->draw_cursor(o, -1, -1);
  o->redraw_text(o, 0, 0);

  return ret;

};


/*
  textline execute
  repeat until mouse is pressed out of textline or key ESC | ENTER is pressed
*/
l_dword  textline_execute ( p_object o )
{

  l_int end = 0;

  while ( !end ) { /* not end command */

      if ( event_main.type & EV_KEYBOARD && KEYPRESSED(TO_KEY(KB_ESC)) )

         end = 1;

      if ( event_main.type & EV_KEYBOARD && KEYPRESSED(TO_KEY(KB_ENTER)) )

         end = 2;


      if ( event_main.type & EV_MOUSE ) {

         if ( !VIEW(o)->is_mouse_in_view(VIEW(o)) )

            if ( OBJECT(mouse)->state & MO_SF_MOUSEDOWN ||
                 OBJECT(mouse)->state & MO_SF_MOUSEUP ) end = 3;

      };

      o->translate_event(o, &event_main);

      INTMAIN(&event_main);

      if ( end ) {

        if ( end <= 2 ) /* not mouse events */

            clear_event(&event_main);

        if ( end == 1 )

            return MSG_CANCEL; /* ESC */

        else

            return MSG_OK;

      };

      o->get_event(o, &event_main);

  };

  return MSG_CANCEL;

};


void  textline_translate_event ( p_object o, t_event *event )
{
  RETVIEW(o, event);


  view_translate_event(o, event); /* old translate_event function */


  if ( event->type & EV_MOUSE ) { /* mouse event */

    if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) { /* select */

      if ( !o->is_state(o, OB_SF_SELECTED) ) {

         clear_event(event);

         o->select(o);

         return;
      };

    };

    if ( o->is_state(o, OB_SF_SELECTED) &&

         OBJECT(mouse)->state & MO_SF_MOUSELAUTO ) { /* select text */

        sel_clear(TEXTLINE(o));

        TEXTLINE(o)->sel_ok = 3;

        while ( OBJECT(mouse)->state & MO_SF_MOUSELAUTO ) {

          l_int mpos = TEXTLINE(o)->get_pos_from_xy(TEXTLINE(o), VIEW(o)->get_local_point(VIEW(o), mouse->where));

          TEXTLINE(o)->redraw_text(TEXTLINE(o), mpos, 0);

          o->get_event(o, event);

        };

    };

  };


  if ( o->is_state(o, OB_SF_FOCUSED) )

                                  /* keyboard events */
  if ( event->type & EV_KEYBOARD ) {

    if ( keyb->shifts & KB_SHIFT_FLAG ) {

      if ( TEXTLINE(o)->sel_ok == 0 )

         TEXTLINE(o)->sel_ok = 1;

      else

      if ( TEXTLINE(o)->sel_ok == 4 )

         TEXTLINE(o)->sel_ok = 2;

    } else TEXTLINE(o)->sel_ok = 0;


    if ( OBJECT(keyb)->state & KB_SF_KEYDOWN )

    switch ( KEY_TO(keyb->code) ) {

      case KB_RIGHT : {  /* arrow RIGHT was pressed */

        TEXTLINE(o)->redraw_text(TEXTLINE(o), TEXTLINE(o)->pos+1, 0);

        clear_event(event);

      }; break;

      case KB_LEFT : {  /* arrow LEFT was pressed */

        TEXTLINE(o)->redraw_text(TEXTLINE(o), TEXTLINE(o)->pos-1, 0);

        clear_event(event);

      }; break;

      case KB_HOME : {  /* key HOME was pressed */

        TEXTLINE(o)->redraw_text(TEXTLINE(o), 0, 0);

        clear_event(event);

      }; break;

      case KB_END : {  /* key END was pressed */

        TEXTLINE(o)->redraw_text(TEXTLINE(o), strlen(TEXTLINE(o)->text), 0);

        clear_event(event);

      }; break;

      case KB_DEL : {  /* key DEL was pressed */

        TEXTLINE(o)->redraw_text(TEXTLINE(o), TEXTLINE(o)->pos, TO_KEY(KB_DEL));

        clear_event(event);

      }; break;

      case KB_BACKSPACE : {  /* key BACKSPACE was pressed */

        TEXTLINE(o)->redraw_text(TEXTLINE(o), TEXTLINE(o)->pos-1, TO_KEY(KB_DEL));

        clear_event(event);

      }; break;

      default :

        if ( (l_byte)TO_CHAR(keyb->code) >= 32 &&
             (l_byte)TO_CHAR(keyb->code) <= 255 ) {

             TEXTLINE(o)->redraw_text(TEXTLINE(o), TEXTLINE(o)->pos+1, keyb->code);

             clear_event(event);

        }; break;
    };

  };

};


l_bool  textline_select_data ( p_object o, l_int data_style, l_bool set )
{

   if ( !data_style || data_style & DS_ALL ) {

      if ( set )

          TEXTLINE(o)->sel_all(TEXTLINE(o), 0, -1); /* select */

      else

          TEXTLINE(o)->sel_all(TEXTLINE(o), 0, 0); /* unselect */

      return true;

   };

   return false;
};


t_rect textline_size_limits ( p_view o )
{

  return rect_assign(2, 2, rect_sizex(o->bounds)-2, rect_sizey(o->bounds)-2);

};


l_int  textline_get_pos_from_xy ( p_textline o, t_point p )
{

  t_rect r = VIEW(o)->size_limits(VIEW(o));
  l_int  pos = 0;

  if ( p.x > r.a.x ) {

    pos = o->charsin_size ( o, p.x-r.a.x, o->line, 1);

    return max(0, pos+o->line);

  };

  pos = o->charsin_size ( o, -(p.x), o->line, -1);

  return max(0, o->line-pos);

};


l_int textline_charsin ( p_textline o, l_int from, l_int plus )
{

   return o->charsin_size(o, rect_sizex(VIEW(o)->bounds)-(4+FONT_GETWIDTH(VIEW(o)->font, ' ')), from, plus);

};


l_int textline_charsin_size ( p_textline o, l_int size, l_int from, l_int plus )
{

  l_int sizex = size;
  l_int tsize = strlen(o->text);
  l_int chars = 0;

  if ( o->text ) {

    from += plus;

    while ( sizex > 0 && from >= 0 && from <= tsize ) {

      sizex -= FONT_GETWIDTH(VIEW(o)->font, o->text[from]);

      from += plus;

      chars++;

    };

    if ( sizex < 0 ) chars--;

    return max(0, chars);

  };

  return 0;

};


void  textline_draw_cursor_ex ( p_textline o, l_int line, l_int pos, l_int active )
{

  p_view  vo = VIEW(o);

  t_rect  r = vo->size_limits(vo);
  t_rect  safe;
  l_int rcpos = FONT_GETSTRWIDTH(vo->font, &o->text[line], pos-line);
  l_int ecpos = FONT_GETSTRWIDTH(vo->font, &o->text[line], (pos-line)+1);
  l_int fcolor = active?COLOR(CO_WHITE):vo->get_color(vo, 2);
  l_int bcolor = COLOR(CO_BLACK);
  t_point p;
  BITMAP *out;


  if ( ecpos == rcpos ) ecpos = rcpos+FONT_GETWIDTH(vo->font, ' ');

  r = rect_assign(max(r.a.x, r.a.x+rcpos), r.a.y, min(r.b.x, r.a.x+ecpos), r.b.y);

  safe = r;

  out = vo->begin_paint(vo, &p, r);

  if ( out ) {

    r = rect_move(r, p.x, p.y);

    if ( active )

      rectfill(out, r.a.x, r.a.y, r.b.x, r.b.y, bcolor);

    else

      vo->background(vo, out, r);

    textout_draw_rect(out, vo->font, &o->text[pos], 1, r.a.x, r.a.y, r.b.x, r.b.y, TX_ALIGN_LEFT, fcolor, TX_NOCOLOR, 1);

  };

  vo->end_of_paint(vo, safe);

};



void  textline_draw_cursor ( p_textline o, l_int oldline, l_int oldpos )
{

  if ( !is_wable(o) ) /* is not-rewrite able */

     return;

  if ( oldpos >= 0 && oldpos <= o->limit && oldline == o->line ) {

     o->draw_text ( o );

  };

  if ( o->cursor_visible )

    o->draw_cursor_ex ( o, o->line, o->pos, 1);

};


void    textline_sel_all ( p_textline o, l_int selfrom, l_int selto )
{

  if ( selto == -1 ) selto = strlen(o->text);

  if ( selfrom || selto )

    o->sel_ok = 4;

  else

    o->sel_ok = 0;

  o->pos = o->line = 0;

  o->sel_from = selfrom;
  o->sel_to = selto;
  o->sel_first_from = selto;
  o->sel_first_to = selto;

};


void  textline_show_cursor ( p_textline o, l_bool show )
{
  o->cursor_visible = show;

  o->draw_cursor(o, o->line, o->pos);
};


void  textline_draw_text ( p_textline o )
{
  p_view  vo = VIEW(o);

  t_rect  r = vo->size_limits(vo);
  t_rect  safe = r;
  t_point p;

  BITMAP *out = vo->begin_paint(vo, &p, r);

  if ( out ) {

    l_color fcolor = vo->get_color(vo, 2);
    l_color fscolor = vo->get_color(vo, 4);
    l_color bscolor = vo->get_color(vo, 3);

    r = rect_move(r, p.x, p.y);

    if ( OBJECT(o)->is_options(OBJECT(o), OB_OF_ENABLE) ) {

      /* is not-rewrite able */
      if ( !is_wable(o) && OBJECT(o)->is_state(OBJECT(o), OB_SF_SELECTED) ) {

        fcolor = vo->get_color(vo, 6);

        rectfill(out, r.a.x, r.a.y, r.b.x, r.b.y, vo->get_color(vo, 5));

      } else

        vo->background(vo, out, r);

    } else

       rectfill(out, r.a.x, r.a.y, r.b.x, r.b.y, vo->get_color(vo, 1));

    if ( !is_wable(o) ) /* is not-rewrite able */

       sel_clear(o);

    draw_selected_text (out, vo->font, &(o->text[o->line]), -1, max(0, o->sel_from-o->line), max(0, o->sel_to-o->line),
                        r.a.x, r.a.y, r.b.x, r.b.y, (o->flags & TF_ALIGN_RIGHT)?TX_ALIGN_RIGHT:TX_ALIGN_LEFT,
                        fcolor, TX_NOCOLOR, fscolor, bscolor, 1);


  };

  vo->end_of_paint(vo, safe);

};


void  textline_redraw_text ( p_textline o, l_int newpos, l_int keycode )
{

  l_int oldpos = o->pos;
  l_int oldline = o->line;

  l_bool can_draw_cursor = false;
  l_bool can_del = true;
  l_bool must_write = true;

  l_int  redraw_text = 0;

  if ( newpos >= 0 && newpos < o->limit ) {

    if ( !keycode ) o->pos = max(0, min(strlen(o->text), newpos));

    else

      o->pos = newpos;

    can_draw_cursor = true;

  } else return;

  if ( o->pos == oldpos ) can_draw_cursor = false;

  if ( keycode ) {

    if ( !is_wable(o) ) /* is not-rewrite able */

      return;

    if ( is_sel(o) ) {

      o->del_text(o, o->sel_from, o->sel_to-o->sel_from);
      o->pos = min(strlen(o->text), o->sel_from+newpos-oldpos);
      oldpos = o->sel_from;
      o->line = o->pos-o->charsin(o, o->pos, -1);

      can_del = false;

    };

    sel_clear(o);

    if ( keycode == TO_KEY(KB_DEL) ) {

      if ( strlen(o->text) > 0 || !can_del ) { /* can delete character */

        if ( can_del ) o->del_char(o, o->pos);

        redraw_text = 0x03;

        can_draw_cursor = false;

      } else {

        if ( can_del ) seal_error(ERR_NONREGULAR, "");

      };

    } else {

      if ( !can_del ) o->pos = oldpos+1;

      if ( strlen(o->text) < o->limit-1 ) { /* can put next char */

        if ( o->pos != oldpos || must_write ) {

           o->ins_char(o, oldpos, (l_char)TO_CHAR(keycode));

           redraw_text = 0x03;

           can_draw_cursor = false;

        };

      } else {

        o->pos = oldpos;

        seal_error(ERR_NONREGULAR, "");

      };
    };

  };

  if ( !o->sel_ok ) /* control selected text */

    sel_clear(o);

  else
  if ( o->pos != oldpos ) { /* control selected text */

    if ( o->sel_ok == 3 ) {  /* for mouse */

      o->sel_first_from = o->sel_from = o->pos;
      o->sel_first_to = o->sel_to = o->pos;

      o->sel_ok = 2;

    } else if ( o->sel_ok == 1 ) { /* for arrows */

      o->sel_from = min(oldpos, o->pos);
      o->sel_to = max(oldpos, o->pos);

      o->sel_first_from = oldpos;
      o->sel_first_to = oldpos;

      o->sel_ok = 2;

    } else if ( o->sel_ok == 2 ) { /* continue */

      o->sel_from = min(o->pos, o->sel_first_from);
      o->sel_to = max(o->pos, o->sel_first_to);

    };

    redraw_text = 0x01;

  };

  if ( o->pos != oldpos || redraw_text )

    if ( o->pos < o->line ) {

      o->line = o->pos;

      redraw_text = 0x03;
      can_draw_cursor = false;

    } else
    if ( o->pos - o->line >= o->charsin(o, o->line, 1) ) {

      o->line = o->pos-o->charsin(o, o->pos, -1);

      redraw_text = 0x03;
      can_draw_cursor = false;

    };


  if ( redraw_text ) {

    if ( redraw_text & 0x01 ) o->draw_text(o);
    if ( redraw_text & 0x02 ) o->draw_cursor(o, -1, -1);

  };


  if ( can_draw_cursor ) o->draw_cursor(o, oldline, oldpos);


  set_format_text(&(VIEW(o)->info_text), "%s: %i\n%s: '%s'", TXT_SIZE, strlen(o->text), TXT_TEXT, o->text);

};



/* t_workline */

t_rect worktextline_size_limits ( p_view o )
{

  return rect_assign(1, 1, rect_sizex(o->bounds)-1, rect_sizey(o->bounds)-1);

};


void  worktextline_draw ( p_view o )
{

  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    rect(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_BLACK));

    TEXTLINE(o)->draw_text(TEXTLINE(o));
    TEXTLINE(o)->draw_cursor(TEXTLINE(o), -1, -1);

  };

  o->end_of_paint(o, r);

};



/* listbox item functions */

p_listbox_item   new_listbox_item ( l_text name, BITMAP *icon, l_bool sel, l_int flags )
{

  p_listbox_item i = (p_listbox_item)_malloc(sizeof(t_listbox_item));

  if ( i ) {

     clear_type(i, sizeof(t_listbox_item));

     i->name  = _strdup(name);
     i->icon  = icon;
     i->sel   = sel;
     i->flags = flags;

  };

  return i;

};



void  free_listbox_item ( void* t )
{

  if ( t ) {

     _free(((p_listbox_item)t)->name);

     if ( ((p_listbox_item)t)->flags & LIF_MEMORY ) /* own memory */

        destroy_bitmap(((p_listbox_item)t)->icon);

     _free(t);

  };

};




/* stattext */

void  stattext_draw ( p_view o )
{
  t_rect  r = o->get_local_extent(o);
  t_point p = o->get_global_point(o, r.a);
  l_text  t = STATTEXT(o)->text;
  t_rect  safe = r;
  l_int   ln = 0;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    o->background(o, out, rect_move(r, p.x, p.y));

    while ( t ) {  /* calc how many lines are in the text */
       t = strchr(t, '\n'); /* find end-char */
       if ( t ) t++;
       ln++;
    };

    t = STATTEXT(o)->text;

    if ( STATTEXT(o)->align & TX_ALIGN_CENTERY ) /* y align is in the CENTER */

         r.a.y += (rect_sizey(r)-(ln*FONT_GETHEIGHT(o->font)))/2;

    else

    if ( STATTEXT(o)->align & TX_ALIGN_BOTTOM ) { /* y align is at the BOTTOM */

         r.a.y = r.b.y - (ln*FONT_GETHEIGHT(o->font));

    };

    while ( t && *t && r.a.y <= safe.b.y ) {

      l_text oldt = t;

      t = strchr(t, '\n'); /* find end-char */

      r.b.y  = r.a.y + FONT_GETHEIGHT(o->font);

      textout_draw_rect(out, o->font, oldt, strsize(oldt, t),
                        r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y,
                        0/*STATTEXT(o)->align*/,
                        o->get_color(o, 1), TX_NOCOLOR, 0);

      if ( t ) { /* enter was found */

            t++; /* move to next char after enter */

            r.a.y += FONT_GETHEIGHT(o->font);

      };

    };

  };

  o->end_of_paint(o, safe);
};



/* process */

p_process   _process_init ( p_process o, t_rect r, l_dword size, l_dword *where )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_process));

  view_init(VIEW(o), r);


  /* object's declarations */

  OBJECT(o)->translate_event = &process_translate_event;
  OBJECT(o)->func_callback = &process_func_callback;


  /* view's declarations */

  VIEW(o)->draw = &process_draw;
  VIEW(o)->size_limits = &process_size_limits;


  /* process's declarations */

  o->where = where;
  o->size  = size;

  o->draw_process = &process_draw_process;
  o->rewrite_promile = &process_rewrite_promile;

  OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
  OBJECT(o)->set_options(OBJECT(o), OB_OF_STILLPROCESS, true);

  VIEW(o)->set_palette(VIEW(o), pal_process);

  VIEW(o)->brush.color = VIEW(o)->get_color(VIEW(o), 0);
  VIEW(o)->brush.color2 = VIEW(o)->get_color(VIEW(o), 1);
  VIEW(o)->brush.state |= BRUSH_GRADIENT;

  return o;

};



/* history */

p_history   _history_init ( p_history o, t_rect r, p_list list, l_int limit, l_int flags )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_history));

  textline_init(TEXTLINE(o), r, limit, flags);


  /* object's declarations */

  OBJECT(o)->translate_event = &history_translate_event;
  OBJECT(o)->setup = &history_setup;


  /* view's declarations */

  VIEW(o)->change_bounds = &history_change_bounds;


  /* history's declarations */

  o->list = list;

  o->rewrite_item = &history_rewrite_item;
  o->listbox_execute = &history_listbox_execute;
  o->show_box = &history_show_box;
  o->set_list = &history_set_list;

  return o;
};




/* listbox */

p_listbox  _listbox_init ( p_listbox o, t_rect r, p_list list, l_byte cells, l_int flags )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_listbox));

  cells = max(1, cells);

  scroller_init(SCROLLER(o), r, (cells == 1) ? SF_VERSCROLLBAR : SF_HORSCROLLBAR);


  /* object's declarations */

  OBJECT(o)->translate_event = &listbox_translate_event;
  OBJECT(o)->set_state = &listbox_set_state;
  OBJECT(o)->set_options = &listbox_set_options;
  OBJECT(o)->get_data = &listbox_get_data;
  OBJECT(o)->set_data = &listbox_set_data;
  OBJECT(o)->done = &listbox_done;


  /* view's declarations */

  VIEW(o)->draw = &listbox_draw;
  VIEW(o)->size_limits = &listbox_size_limits;
  VIEW(o)->change_bounds = &listbox_change_bounds;

  /* scroller's declarations */

  SCROLLER(o)->scroll_size = &listbox_scroll_size;
  SCROLLER(o)->scroll_limits = &listbox_scroll_limits;
  SCROLLER(o)->recalc_positions = &listbox_recalc_positions;


  /* listbox's declarations */

  o->cells = cells;
  o->flags = flags;
  o->list = list;
  o->between.y = 2;
  o->between.x = 4;

  o->get_pos_from_xy = &listbox_get_pos_from_xy;
  o->draw_item_ptr = &listbox_draw_item_ptr;
  o->draw_item = &listbox_draw_item;
  o->draw_box = &listbox_draw_box;
  o->rewrite_item = &listbox_rewrite_item;
  o->get_max_in_box = &listbox_get_max_in_box;
  o->get_item_rect = &listbox_get_item_rect;
  o->get_max = &listbox_get_max;
  o->get_rows = &listbox_get_rows;
  o->get_item_size = &listbox_get_item_size;
  o->get_selected_items = &listbox_get_selected_items;
  o->set_list = &listbox_set_list;


  VIEW(o)->set_palette(VIEW(o), pal_listbox);

  VIEW(o)->brush.color = VIEW(o)->get_color(VIEW(o), 0);

  return o;
};



/* worklistbox */

p_listbox  _worklistbox_init ( p_listbox o, t_rect r, p_list list, l_byte cells, l_int flags )
{

  if ( !o ) return NULL;

  listbox_init(o, r, list, cells, flags);

  /* retype draw */

  VIEW(o)->draw = &worklistbox_draw;
  VIEW(o)->size_limits = &worklistbox_size_limits;

  return o;
};



/* textline */

p_textline  _textline_init ( p_textline o, t_rect r, l_int limit, l_int flags )
{

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_textline));

  view_init(VIEW(o), r);


  /* object's declarations */

  OBJECT(o)->translate_event = &textline_translate_event;
  OBJECT(o)->execute = &textline_execute;

  OBJECT(o)->set_state = &textline_set_state;
  OBJECT(o)->set_options = &textline_set_options;
  OBJECT(o)->get_data = &textline_get_data;
  OBJECT(o)->set_data = &textline_set_data;
  OBJECT(o)->select_data = &textline_select_data;
  OBJECT(o)->done = &textline_done;

  l_tag_cpy(OBJECT(o)->data_type, DAT_TEXT);


  /* view's declarations */

  if ( !(flags & TF_REWRITEUNABLE) )

       VIEW(o)->cursor = CUR_TEXT;

  VIEW(o)->draw = &textline_draw;
  VIEW(o)->drag_where = &textline_drag_where;
  VIEW(o)->size_limits = &textline_size_limits;


  /* textline's declarations */

  o->limit = limit+1;
  o->flags = flags;

  o->ins_text = &textline_ins_text;
  o->ins_char = &textline_ins_char;
  o->del_text = &textline_del_text;
  o->del_char = &textline_del_char;
  o->draw_cursor_ex = &textline_draw_cursor_ex;
  o->draw_cursor = &textline_draw_cursor;
  o->show_cursor = &textline_show_cursor;
  o->draw_text = &textline_draw_text;
  o->redraw_text = &textline_redraw_text;
  o->sel_all = &textline_sel_all;
  o->charsin = &textline_charsin;
  o->set_text = &textline_set_text;
  o->charsin_size = &textline_charsin_size;
  o->get_pos_from_xy = &textline_get_pos_from_xy;

  VIEW(o)->set_palette(VIEW(o), pal_textline);

  VIEW(o)->brush.color = VIEW(o)->get_color(VIEW(o), 0);

  return o;
};




/* worktextline */

p_textline  _worktextline_init ( p_textline o, t_rect r, l_int limit )
{

  if ( !o ) return NULL;

  textline_init(o, r, limit, TF_NONE);

  /* retype draw */

  VIEW(o)->draw = &worktextline_draw;
  VIEW(o)->size_limits = &worktextline_size_limits;

  return o;
};


/* stattext */

p_stattext _stattext_init_ex ( p_stattext o, t_rect r, l_int align, l_text text, va_list argp )
{
  static l_char buffer[256];

  if ( !o || !text ) return NULL;

  clear_type(o, sizeof(t_stattext));

  view_init(VIEW(o), r);


  /* view's functions */

  VIEW(o)->draw = &stattext_draw;


  /* stattext's declarations */

  o->align = align;

  vsprintf(o->text, text, argp);

  /* function calling */

  OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE, false);
  OBJECT(o)->set_options(OBJECT(o), OB_OF_ENABLE, false);

  VIEW(o)->set_palette(VIEW(o), pal_stattext);

  VIEW(o)->brush.color = VIEW(o)->get_color(VIEW(o), 0);

  return o;
};


p_stattext _stattext_init ( p_stattext o, t_rect r, l_int align, l_text text, ... )
{

  va_list arg;
  va_start(arg, text);

  o = stattext_init_ex(o, r, align, text, arg);

  va_end(arg);

  return o;

};


#include"dialogs.exp"


lib_begin ( void ) {

  if ( ap_process == AP_ALLOC ) {

          AP_EXPORTLIB(); /* export table in app.exp */

  } else

  if ( ap_process == AP_INIT ) {

          AP_SETNUMOFCALLS(1); /* set max num of calls */

          pal_stattext[0] = color_get_from_ini("3D_background");
          pal_stattext[1] = color_get_from_ini("text");

          pal_textline[0] = color_get_from_ini("text_input_background");
          pal_textline[1] = color_get_from_ini("text_input_disable_background");
          pal_textline[2] = color_get_from_ini("text_input_foreground");
          pal_textline[3] = color_get_from_ini("text_input_background_select");
          pal_textline[4] = color_get_from_ini("text_input_foreground_select");
          pal_textline[5] = color_get_from_ini("text_input_nowrite_background_select");
          pal_textline[6] = color_get_from_ini("text_input_nowrite_foreground_select");

          pal_listbox[0] = color_get_from_ini("text_input_background");
          pal_listbox[1] = color_get_from_ini("text_input_disable_background");
          pal_listbox[2] = color_get_from_ini("text_input_foreground");
          pal_listbox[3] = color_get_from_ini("item_background_select");
          pal_listbox[4] = color_get_from_ini("item_foreground_select");
          pal_listbox[5] = color_get_from_ini("item_background_select_item");
          pal_listbox[6] = color_get_from_ini("item_foreground_select_item");
          pal_listbox[7] = color_get_from_ini("item_background_notme");
          pal_listbox[8] = color_get_from_ini("item_foreground_notme");

          pal_process[0] = color_get_from_ini("process_background");
          pal_process[1] = color_get_from_ini("process_background_gradient");
          pal_process[2] = color_get_from_ini("process_fill");
          pal_process[3] = color_get_from_ini("process_fill_gradient");

  } else

  if ( ap_process == AP_FREE ) {


  };


} lib_end;

