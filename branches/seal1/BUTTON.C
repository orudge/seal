/****************************************************************/
/*                                                              */
/*                           button.c                           */
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



#include"allegro.h"
#include"seal.h"
#include"button.h"

l_color  pal_button[] = {CO_SYSx20, CO_BLACK, CO_SYSx19, CO_SYSx20, CO_NOCOLOR};


p_button    (*button_init) ( p_button o, t_rect r, l_text caption, l_dword message, l_int flags ) = &_button_init;
p_icon      (*icon_init) ( p_icon o, t_rect r, BITMAP *icon, l_dword message, l_int flags ) = &_icon_init;
p_icobutton (*icobutton_init) ( p_icobutton o, t_rect r, BITMAP *icon, l_text caption, l_dword message, l_int flags ) = &_icobutton_init;


/* icon class */

l_bool  icon_done ( p_object o )
{
  if ( obj_exist(o) <= 0 ) return false;

  if ( ICON(o)->flags & IF_SELFIMAGE ) /* image of icon is internal */

      destroy_bitmap(ICON(o)->icon);

  ICON(o)->icon = NULL;

  return view_done(o);

};


void  icon_set_state ( p_object o, l_dword st, l_bool set )
{
  view_set_state(o, st, set);

  if ( st & OB_SF_FOCUSED ) {

    VIEW(o)->draw_view(VIEW(o));

  };
};


void  icon_draw ( p_view o )
{
  t_rect  r = o->get_local_extent(o);
  t_rect  safe = r;
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, safe);

  if ( out ) {

    if ( ICON(o)->icon ) {

         if ( ICON(o)->flags & IF_STRETCH )

             stretch_sprite(out, ICON(o)->icon, p.x, p.y, rect_sizex(r)+1, rect_sizey(r)+1);

         else { /* center icon */

             p_view  own = VIEW(OBJECT(o)->owner_view(OBJECT(o)));

             l_color color = own?own->brush.color:COLOR(CO_BLACK);

             l_rect dx = (rect_sizex(r)-IMAGE_WIDTH(ICON(o)->icon))/2;
             l_rect dy = (rect_sizey(r)-IMAGE_HEIGHT(ICON(o)->icon))/2;

             rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, color);

             draw_sprite(out, ICON(o)->icon, p.x+dx, p.y+dy);

         };

    } else

      rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_BLACK));

  };

  o->end_of_paint(o, safe);

};


void  icon_translate_event ( p_object o, t_event *event )
{
  RETVIEW(o, event);


  view_translate_event(o, event); /* old translate_event function */


  /* keyboard event */
  if ( event->type & EV_KEYBOARD && OBJECT(keyb)->state & KB_SF_KEYDOWN ) {


    if ( keyb->code == TO_KEY(KB_ENTER) )

        if ( o->is_state(o, OB_SF_FOCUSED) ) {

          set_event(event, EV_MESSAGE, ICON(o)->message, o);

          o->put_event(o, event);

          clear_event(event);
        };

  };


  /* mouse event */
  if ( event->type & EV_MOUSE ) {

     if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) {

        clear_event(event);

        o->select(o);

        if ( OBJECT(mouse)->state & MO_SF_MOUSEDOUBLE ) {

          set_event(event, EV_MESSAGE, BUTTON(o)->message, o);

          o->put_event(o, event);

          clear_event(event);

        };
     };

     if ( OBJECT(mouse)->state & MO_SF_MOUSELAUTO ) { /* dragging */

        VIEW(o)->drag_view(VIEW(o), DM_DRAGMOVE, event);
        clear_event(event);

     };

  };
};





/* button class */

l_bool  button_done ( p_object o )
{

  if ( !view_done(o) ) return false;

  afree(&(BUTTON(o)->caption));

  return true;

};


void  button_set_state ( p_object o, l_dword st, l_bool set )
{
  view_set_state(o, st, set);

  if ( st & OB_SF_FOCUSED ) {

    VIEW(o)->draw_view(VIEW(o));

  };
};


void  button_set_options ( p_object o, l_dword op, l_bool set )
{
  obj_set_options(o, op, set);

  if ( op & OB_OF_ENABLE ) {

    VIEW(o)->draw_view(VIEW(o));

  };

};


l_bool button_is_default ( p_button o )
{
  p_object p = OBJECT(o)->find_match_view ( OBJECT(o), OB_SF_FOCUSED, 0, false );

  /* selected is not button */
  if ( p && !(p->tag & TAG_BUTTON) && BUTTON(o)->flags & BF_DEFAULT )

    return true;

  else if ( OBJECT(o)->is_state(OBJECT(o), OB_SF_FOCUSED) )

          return true; /* selected is me */

  return false;
};


void  button_translate_event ( p_object o, t_event *event )
{
  RETVIEW(o, event);


  view_translate_event(o, event); /* old translate_event function */


  /* keyboard event */
  if ( event->type & EV_KEYBOARD && OBJECT(keyb)->state & KB_SF_KEYDOWN ) {


    if ( keyb->code == TO_KEY(KB_ENTER) )

        if ( BUTTON(o)->is_default(BUTTON(o)) ) {

          set_event(event, EV_MESSAGE, BUTTON(o)->message, o);

          o->put_event(o, event);

          clear_event(event);

        };

  };


  /* mouse event */

  if ( event->type & EV_MOUSE && OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) {

      l_bool in_view = false;

      while ( OBJECT(mouse)->state & MO_SF_MOUSELPRESS ) {

        l_bool safe_in_view = in_view;

        in_view = VIEW(o)->is_mouse_in_view(VIEW(o));

        if ( in_view != safe_in_view )

          BUTTON(o)->draw_state(BUTTON(o), in_view);

        o->get_event(o, event);

      };

      if ( in_view ) {

        BUTTON(o)->draw_state(BUTTON(o), false);

        set_event(event, EV_MESSAGE, BUTTON(o)->message, o);

        o->put_event(o, event);

        clear_event(event);

      };
  };
};


void  button_draw_state ( p_button o, l_int press )
{
  t_rect  r = VIEW(o)->get_local_extent(VIEW(o));
  t_rect  safe = r;
  t_point p;

  BITMAP *out = VIEW(o)->begin_paint(VIEW(o), &p, safe);

  if ( out ) {

    l_color fcolor = VIEW(o)->get_color(VIEW(o), 1);

    VIEW(o)->background(VIEW(o), out, rect_move(r, p.x, p.y));


    if ( !OBJECT(o)->is_options(OBJECT(o), OB_OF_ENABLE) ) /* disable button */

      fcolor = VIEW(o)->get_color(VIEW(o), 2);


    if ( OBJECT(o)->is_state(OBJECT(o), OB_SF_FOCUSED) ) {

      l_rect x = 0;
      l_rect y = 0;

      rect(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_BLACK));

      r = rect_assign(r.a.x+1, r.a.y+1, r.b.x-1, r.b.y-1);

      light_image ( out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, 20, 20, 20);

    };

    textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x+2, r.a.y+p.y+2,
                           r.b.x+p.x-2, r.b.y+p.y-2, TX_ALIGN_CENTER, fcolor, TX_NOCOLOR, 0);

    if ( !press ) {

      if ( o->flags & BF_DARROUND ) {

        button(out, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x-1, r.b.y+p.y-1, COLOR(CO_WHITE),
                                                            COLOR(CO_BLACK));
        button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_BLACK),
                                                                COLOR(CO_WHITE));

      } else

      if ( !(o->flags & BF_PUSH) ) {

        button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_WHITE),
                                                            COLOR(CO_BLACK));
      };

    } else

      if ( (press == -1 && o->flags & BF_UNDERSEL) || press == 1 )

            button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_BLACK),
                                                                    COLOR(CO_WHITE));


  };

  VIEW(o)->end_of_paint(VIEW(o), safe);

};


void  button_draw ( p_view o )
{
  BUTTON(o)->draw_state(BUTTON(o), false);
};




/* icobutton class */

l_bool  icobutton_done ( p_object o )
{

  if ( obj_exist(o) <= 0 ) return false;

  if ( BUTTON(o)->flags & IBF_SELFIMAGE )

      destroy_bitmap(ICOBUTTON(o)->icon);

  ICOBUTTON(o)->icon = NULL;

  return button_done(o);

};


void  icobutton_draw_state ( p_button o, l_int press )
{
  p_icobutton ib = ICOBUTTON(o);

  t_rect  r = VIEW(o)->get_local_extent(VIEW(o));
  t_rect  safe = r;
  t_point p;

  BITMAP *out = VIEW(o)->begin_paint(VIEW(o), &p, safe);

  if ( out ) {

    l_color fcolor = VIEW(o)->get_color(VIEW(o), 1);

    VIEW(o)->background(VIEW(o), out, rect_move(r, p.x, p.y));


    if ( !OBJECT(o)->is_options(OBJECT(o), OB_OF_ENABLE) ) /* disable button */

      fcolor = VIEW(o)->get_color(VIEW(o), 2);


    if ( ib->icon ) { /* center icon if exist */

        l_rect dx = (rect_sizex(r)-IMAGE_WIDTH(ib->icon))/2;
        l_rect dy = (rect_sizey(r)-IMAGE_HEIGHT(ib->icon))/2;

        draw_sprite(out, ib->icon, p.x+dx, p.y+dy);

    };

    if ( OBJECT(o)->is_state(OBJECT(o), OB_SF_FOCUSED) ) {

         l_rect x = 0;
         l_rect y = 0;

         rect(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_BLACK));

         r = rect_assign(r.a.x+1, r.a.y+1, r.b.x-1, r.b.y-1);

         light_image ( out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, 20, 20, 20);

    };

    if ( !press ) {

      if ( o->flags & BF_DARROUND ) {

        button(out, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x-1, r.b.y+p.y-1, COLOR(CO_WHITE),
                                                            COLOR(CO_BLACK));
        button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_BLACK),
                                                                COLOR(CO_WHITE));

      } else

      if ( !(o->flags & BF_PUSH) ) {

        button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_WHITE),
                                                            COLOR(CO_BLACK));
      };

    } else

      if ( (press == -1 && o->flags & BF_UNDERSEL) || press == 1 )

            button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_BLACK),
                                                                    COLOR(CO_WHITE));


  };

  VIEW(o)->end_of_paint(VIEW(o), safe);

};



p_icon  _icon_init ( p_icon o, t_rect r, BITMAP *icon, l_dword message, l_int flags )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_icon));

  view_init(VIEW(o), r);


  /* object's functions */

  OBJECT(o)->translate_event = &icon_translate_event;
  OBJECT(o)->set_state = &icon_set_state;
  OBJECT(o)->done = &icon_done;


  /* view's declarations */

  VIEW(o)->drag_mode |= DM_DRAGMOVE;
  VIEW(o)->draw = &icon_draw;


  /* icon's declarations */

  o->flags = flags;
  o->message = message;
  o->icon = icon;

  VIEW(o)->set_draw_mode(VIEW(o), DWM_ICONICDRAW, true);

  return o;
};



p_button  _button_init ( p_button o, t_rect r, l_text caption, l_dword message, l_int flags )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_button));

  view_init(&o->obclass, r);

  /* object's declarations */

  OBJECT(o)->tag |= TAG_BUTTON;


  /* button's declarations */

  o->caption = (l_text)_strdup((char*)caption);
  o->message = message;
  o->flags = flags;


  /* object's functions */

  OBJECT(o)->translate_event = &button_translate_event;
  OBJECT(o)->set_state = &button_set_state;
  OBJECT(o)->set_options = &button_set_options;
  OBJECT(o)->done = &button_done;


  /* view's functions */

  VIEW(o)->draw = &button_draw;


  /* button's functions */

  o->draw_state = &button_draw_state;
  o->is_default = &button_is_default;


  /* function calls */

  VIEW(o)->set_palette(VIEW(o), pal_button);

  if ( flags & BF_PUSH )
      VIEW(o)->brush.color = VIEW(o)->get_color(VIEW(o), 3);
  else
      VIEW(o)->brush.color = VIEW(o)->get_color(VIEW(o), 0);

  return o;
};


p_icobutton  _icobutton_init ( p_icobutton o, t_rect r, BITMAP *icon, l_text caption, l_dword message, l_int flags )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_icobutton));

  button_init(&o->obclass, r, NULL, message, flags);


  /* icobutton's declarations */

  o->icon = icon;


  /* object's functions */

  OBJECT(o)->done = &icobutton_done;


  /* button's functions */

  BUTTON(o)->draw_state = &icobutton_draw_state;


  VIEW(o)->info_text = _strdup(caption);


  return o;
};


#include"button.exp"

lib_begin ( void ) {

  if ( ap_process == AP_ALLOC ) {

          AP_EXPORTLIB(); /* export table in app.exp */

  } else

  if ( ap_process == AP_INIT ) {

          AP_SETNUMOFCALLS(1); /* set max num of calls */

          pal_button[0] = color_get_from_ini("3D_button");
          pal_button[1] = color_get_from_ini("button_active_caption");
          pal_button[2] = color_get_from_ini("button_pasive_caption");
          pal_button[3] = color_get_from_ini("3D_background");

  }

  if ( ap_process == AP_FREE ) {

  };


} lib_end;

