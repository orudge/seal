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
 * 02/04/2002: Made icon control work properly with skins (orudge)
 */

#include <seal.h>
#include <button.h>


p_button    (*button_init) ( p_button o, t_rect r, l_text caption, l_dword message, l_int flags ) = &_button_init;
p_icon      (*icon_init) ( p_icon o, t_rect r, BITMAP *icon, l_dword message, l_int flags ) = &_icon_init;
p_icobutton (*icobutton_init) ( p_icobutton o, t_rect r, BITMAP *icon, l_text caption, l_dword message, l_int flags ) = &_icobutton_init;

p_skin skin_btnotov = NULL;
p_skin skin_btup   = NULL;
p_skin skin_btdown = NULL;



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


void icon_draw (p_view o)
{
   t_rect  r = o->get_local_extent(o);
   t_rect  safe = r;
   t_point p;

   BITMAP *out = o->begin_paint(o, &p, safe);

   if (out)
   {
      if (ICON(o)->icon)
      {
         if (ICON(o)->flags & IF_STRETCH)
            stretch_sprite(out, ICON(o)->icon, p.x, p.y, rect_sizex(r)+1, rect_sizey(r)+1);
         else
         { /* center icon */
            p_view  own = VIEW(OBJECT(o)->owner_view(OBJECT(o)));
            l_rect dx = (rect_sizex(r)-IMAGE_WIDTH(ICON(o)->icon))/2;
            l_rect dy = (rect_sizey(r)-IMAGE_HEIGHT(ICON(o)->icon))/2;

            if (!use_skins)
            {
               l_color color = own?own->brush.color:color_3d_face;

               rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, color);
            }

            draw_sprite(out, ICON(o)->icon, p.x+dx, p.y+dy);
         }
      }
      else
      {
         if (!use_skins)
            rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, color_3d_face);
      }
   }

   o->end_of_paint(o, safe);
}


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
  //p43
  if ( VIEW(o)->is_mouse_in_view(VIEW(o)) && (BUTTON(o)->flags & BF_UNDERSEL) && !(BUTTON(o)->flags & BF_NOSEL)) {
    if ( desktop->get_top_view_under_mouse(desktop) == VIEW(o)) {
      if ( !(BUTTON(o)->flags & BF_OVER) && event->type & EV_MOUSE ) {
        BUTTON(o)->flags |= BF_OVER;
        BUTTON(o)->draw_state(BUTTON(o), 0);
      };
    } else if (BUTTON(o)->flags & BF_OVER) {
      BUTTON(o)->flags &= ~BF_OVER;
      BUTTON(o)->draw_state(BUTTON(o), 0);
    };
  } else {
    if (BUTTON(o)->flags & BF_OVER) {
      BUTTON(o)->flags &= ~BF_OVER;
      BUTTON(o)->draw_state(BUTTON(o), 0);
    }
  }

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

//      o->select(o);

      while ( OBJECT(mouse)->state & MO_SF_MOUSELPRESS ) {

        l_bool safe_in_view = in_view;

        in_view = VIEW(o)->is_mouse_in_view(VIEW(o));

        if ( in_view != safe_in_view )

          BUTTON(o)->draw_state(BUTTON(o), in_view);

        o->get_event(o, event);

      };

      if ( in_view ) {

        if (BUTTON(o)->flags & BF_OVER) BUTTON(o)->flags &= ~BF_OVER;

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
    if ( o->skin_btdown && o->skin_btup ) {
      p_skin act = press ? o->skin_btdown : o->skin_btup;

      if ( o->skin_btnotov && !press && o->flags & BF_UNDERSEL && !(o->flags & BF_OVER) ) act = o->skin_btnotov;

      draw_skin(out,act,rect_move(r,p.x,p.y),&r);

      textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x, r.a.y,
                             r.b.x, r.b.y, TX_ALIGN_CENTER, act->text, TX_NOCOLOR, 0);

    } else {
	  l_bool idwn = (press == -1 && o->flags & BF_UNDERSEL) || press == 1;
      l_color fcolor = color_3d_text;

      if ( !OBJECT(o)->is_options(OBJECT(o), OB_OF_ENABLE) ) /* disable button */
        fcolor = color_3d_shadow;

      VIEW(o)->background(VIEW(o), out, rect_move(r, p.x, p.y));

      if ( OBJECT(o)->is_state(OBJECT(o), OB_SF_FOCUSED) ) {

        rect(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, color_3d_border);

        r = rect_assign(r.a.x+1, r.a.y+1, r.b.x-1, r.b.y-1);

      };

      textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x+2+(idwn?1:0), r.a.y+p.y+2+(idwn?1:0),
                             r.b.x+p.x-2+(idwn?1:0), r.b.y+p.y-2+(idwn?1:0), TX_ALIGN_CENTER, fcolor, TX_NOCOLOR, 0);

      if ( !press ) {

        if ( o->flags & BF_DARROUND )
          button3d(VIEW(o), out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, 0);

        else if ( !(o->flags & BF_PUSH) )
          if ( !(o->flags & BF_UNDERSEL) || o->flags & BF_OVER )
              button3d(VIEW(o), out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, 0);

      } else if ( idwn )
          button3d(VIEW(o), out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, 1);
    };
  };

  VIEW(o)->end_of_paint(VIEW(o), safe);

};


void  button_draw ( p_view o ) {
  BUTTON(o)->draw_state(BUTTON(o), false);
};

/* icobutton class */

l_bool  icobutton_done ( p_object o ) {
  if ( obj_exist(o) <= 0 ) return false;

  if ( BUTTON(o)->flags & IBF_SELFIMAGE )

      destroy_bitmap(ICOBUTTON(o)->icon);

  ICOBUTTON(o)->icon = NULL;
  return button_done(o);

};

void  icobutton_draw_state ( p_button o, l_int press ) {
  p_icobutton ib = ICOBUTTON(o);

  t_rect  r = VIEW(o)->get_local_extent(VIEW(o));
  t_rect  safe = r;
  t_point p;

  BITMAP *out = VIEW(o)->begin_paint(VIEW(o), &p, safe);

  if ( out ) {

    l_color fcolor = color_3d_light;

    VIEW(o)->background(VIEW(o), out, rect_move(r, p.x, p.y));


    if ( !OBJECT(o)->is_options(OBJECT(o), OB_OF_ENABLE) ) /* disable button */

      fcolor = color_3d_shadow;


    if ( ib->icon ) { /* center icon if exist */
        BITMAP *icon = ( ib->dwnicon && press != 0 ) ? ib->dwnicon : ib->icon;
        l_rect dx = (rect_sizex(r)-IMAGE_WIDTH(icon))/2;
        l_rect dy = (rect_sizey(r)-IMAGE_HEIGHT(icon)-((o->caption)?get_text_height(VIEW(o)->font):0))/2;
        l_bool idwn = (press == -1 && o->flags & BF_UNDERSEL) || ( press == 1 && !( o->flags & BF_NOSEL ) );

        if ( idwn ) {
	      dx++;
		  dy++;
	    };

        draw_sprite(out, icon, p.x+dx, p.y+dy);

        if ( o->caption ) textout_draw_rect(out,VIEW(o)->font,o->caption,-1,p.x+(idwn?1:0),p.y+dy+icon->h,p.x+r.b.x+(idwn?1:0),p.y+dy+icon->h+get_text_height(VIEW(o)->font),TX_ALIGN_CENTER,color_3d_text,TX_NOCOLOR,1);

    };

      if ( !press ) {

        if ( o->flags & BF_DARROUND ) {
          button3d(VIEW(o), out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, 0);

        } else
        if ( !(o->flags & BF_PUSH) ) {
          if (!(o->flags & BF_UNDERSEL) || o->flags & BF_OVER)
            //if ( !(o->flags & BF_DARROUND) ) {
             // button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, color_3d_light, color_3d_dark);
            //} else {
              button3d(VIEW(o), out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, 0);
            //}
        };

      } else

        if ( (press == -1 && o->flags & BF_UNDERSEL) || ( press == 1 && !( o->flags & BF_NOSEL ) ) )
           //if ( o->flags & BF_DARROUND )
              button3d(VIEW(o), out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, 1);
           //else
           //   button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y,  color_3d_dark ,color_3d_light);
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

  VIEW(o)->brush.state &= ~BRUSH_LARGE3D;
  /* function calls */
  if ( flags & BF_DARROUND || !(flags & BF_UNDERSEL) )
    VIEW(o)->brush.state |= BRUSH_LARGE3D;
  else
    VIEW(o)->brush.state |= BRUSH_LIGHT3D;

//  VIEW(o)->set_palette(VIEW(o), pal_button);
  VIEW(o)->brush.color = color_3d_face;
//  VIEW(o)->brush.skin = skin_button;
  if ( !skin_btup && !skin_btdown && !skin_btnotov && use_skins ) {
    skin_btup = get_skin(skin_file,"btup");
    skin_btdown = get_skin(skin_file,"btdown");
    skin_btnotov = get_skin(skin_file,"btnotover");
  };

  if ( use_skins ) {
    VIEW(o)->transparent = true;
    o->skin_btup = skin_btup;
    o->skin_btdown = skin_btdown;
    o->skin_btnotov = skin_btnotov;
    if ( skin_btnotov && !(o->flags & BF_UNDERSEL)) o->flags |= BF_UNDERSEL;
  };
  return o;
};


p_icobutton  _icobutton_init ( p_icobutton o, t_rect r, BITMAP *icon, l_text caption, l_dword message, l_int flags )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_icobutton));

  button_init(&o->obclass, r, NULL, message, flags);


  /* icobutton's declarations */

  o->icon = icon;
  o->dwnicon = NULL;

  /* object's functions */

  OBJECT(o)->done = &icobutton_done;


  /* button's functions */

  BUTTON(o)->draw_state = &icobutton_draw_state;


  VIEW(o)->info_text = _strdup(caption);

  return o;
};

