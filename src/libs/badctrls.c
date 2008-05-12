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

/****************************************************************/
/*                                                              */
/*                          badctrls.c                          */
/*                     Bad Controls for SEAL                    */
/*                            v0.4.2                            */
/*                                                              */
/*      (c) 2000, 2001 Kostas Michalopoulos aka Bad Sector      */
/*         first version for Bad Seal | www.badseal.org         */
/*                                                              */
/* ============================================================ */
/*  Controls included:                                          */
/*    t_bad_checkbox                          Checkbox control  */
/*    t_bad_button                        t_button replacement  */
/*    t_bad_vtrackbar                        vertical trackbar  */
/*    t_bad_tabbook                                   tab book  */
/*    t_bad_iconbox                         box with icons :-)  */
/* ============================================================ */
/****************************************************************/
/* Revision History (since 29/03/2002):
 *
 * 29/03/2002: Updated for SEAL 2.00.11
 */

#include <allegro.h>
#include <seal.h>
#include <badctrls.h>

#define BadSealVersion
#define UseRegistry

#include "beditor.h"

extern void beditor_draw_line(p_beditor o, BITMAP *out, t_rect r, l_text txt);
extern void beditor_draw(p_view o);
extern l_color pal_beditor[];

SetInfoAppName("Bad Controls 0.4.2");
SetInfoDesciption("Bad Controls 0.4.2");
SetInfoCopyright("Copyright (c) Kostas Michalopoulos aka Bad Sector 2000-2001");
SetInfoManufacturer("Kostas Michalopoulos");

/* common palette for Bad Controls
0 3d face
1 3d light
2 3d shadow
3 3d dark
4 3d border
5 text color
6 flat color
7 flat border
*/

l_color pal_bad_common[] = {CO_LIGHTGRAY, CO_WHITE, CO_DARKGRAY, CO_BLACK, CO_BLACK, CO_BLACK, CO_WHITE, CO_BLACK, CO_NOCOLOR};

p_bad_checkbox (*bad_checkbox_init)(p_bad_checkbox o, t_rect r, l_text cap, l_dword msg, l_int st)=&_bad_checkbox_init;
p_bad_button (*bad_button_init)(p_bad_button o, t_rect r, l_text cap, l_int style, l_dword msg)=&_bad_button_init;
p_bad_vtrackbar (*bad_vtrackbar_init)(p_bad_vtrackbar o, t_rect r, l_dword msg)=&_bad_vtrackbar_init;
p_bad_tabbook (*bad_tabbook_init)(p_bad_tabbook o, t_rect r, l_text ftitle)=&_bad_tabbook_init;
p_bad_iconbox (*bad_iconbox_init)(p_bad_iconbox o, t_rect r, l_big flags)=&_bad_iconbox_init;

static DATAFILE *dat = NULL;

#define Get_Bitmap(n) (BITMAP*)GET_DATA(dat, n)

void draw_bevel(BITMAP *out, int x1, int y1, int x2, int y2, int c1, int c2)
{
  if (out) {
    line(out, x1, y1, x2, y1, c1);
    line(out, x1, y1, x1, y2, c1);
    line(out, x2, y1, x2, y2, c2);
    line(out, x1, y2, x2, y2, c2);
  }
}

/* t_bad_checkbox */
l_bool bad_checkbox_done(p_object o)
{
  if (!view_done(o)) return false;
  //afree(&(BAD_CHECKBOX(o)->caption));
  if(BAD_CHECKBOX(o)->caption){_free(BAD_CHECKBOX(o)->caption);BAD_CHECKBOX(o)->caption=NULL;}
  return true;
};

void bad_checkbox_set_state(p_object o, l_dword st, l_bool set)
{
  view_set_state(o, st, set);

  if ( st & OB_SF_FOCUSED ) {

    VIEW(o)->draw_view(VIEW(o));

  };
};

void bad_checkbox_translate_event(p_object o, t_event *event)
{
  RETVIEW(o, event);

  view_translate_event(o, event);

  /* check if we're clicked by the mouse */
  if (event->type & EV_MOUSE && OBJECT(mouse)->state & MO_SF_MOUSELDOWN)
  {
    l_bool in_view = false;

    if (VIEW(o)->is_mouse_in_view(VIEW(o))) {
      BAD_CHECKBOX(o)->grayed = true;
      BAD_CHECKBOX(o)->draw_state(BAD_CHECKBOX(o), in_view);
    };

    while (OBJECT(mouse)->state & MO_SF_MOUSELPRESS)
    {
      l_bool safe_in_view = in_view;
      in_view = VIEW(o)->is_mouse_in_view(VIEW(o));
      if (in_view != safe_in_view) {
        BAD_CHECKBOX(o)->grayed = in_view;
        BAD_CHECKBOX(o)->draw_state(BAD_CHECKBOX(o), in_view);
      };

      o->get_event(o, event);
    };

    if (in_view) {
      if (BAD_CHECKBOX(o)->state < 2) {
        BAD_CHECKBOX(o)->state = 1-BAD_CHECKBOX(o)->state;
        BAD_CHECKBOX(o)->grayed = false;
        BAD_CHECKBOX(o)->draw_state(BAD_CHECKBOX(o), false);
        set_event(event, EV_MESSAGE, BAD_CHECKBOX(o)->message, o);
        o->put_event(o, event);
        clear_event(event);
      } else {
        BAD_CHECKBOX(o)->grayed = false;
        BAD_CHECKBOX(o)->draw_state(BAD_CHECKBOX(o), false);
      };
    };
  };

};

void bad_checkbox_draw_state(p_bad_checkbox o, l_int astate)
{
  t_rect r = VIEW(o)->get_local_extent(VIEW(o));
  t_point p;

  BITMAP *out = VIEW(o)->begin_paint(VIEW(o), &p, r);

  if ( out ) {
    rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, VIEW(o)->get_color(VIEW(o), 0));

    if ((o->state != 2) && (o->grayed == false)) rectfill(out, r.a.x+p.x, r.a.y+p.y, r.a.x+p.x+14, r.a.y+p.y+14, VIEW(o)->get_color(VIEW(o), 6));
    line(out, r.a.x+p.x, r.a.y+p.y, r.a.x+p.x, r.a.y+p.y+14, VIEW(o)->get_color(VIEW(o), 2));
    line(out, r.a.x+p.x, r.a.y+p.y, r.a.x+p.x+14, r.a.y+p.y, VIEW(o)->get_color(VIEW(o), 2));
    line(out, r.a.x+p.x+14, r.a.y+p.y, r.a.x+p.x+14, r.a.y+p.y+14, VIEW(o)->get_color(VIEW(o), 1));
    line(out, r.a.x+p.x, r.a.y+p.y+14, r.a.x+p.x+14, r.a.y+p.y+14, VIEW(o)->get_color(VIEW(o), 1));

    line(out, r.a.x+p.x+1, r.a.y+p.y+1, r.a.x+p.x+1, r.a.y+p.y+13, VIEW(o)->get_color(VIEW(o), 3));
    line(out, r.a.x+p.x+1, r.a.y+p.y+1, r.a.x+p.x+13, r.a.y+p.y+1, VIEW(o)->get_color(VIEW(o), 3));
    line(out, r.a.x+p.x+13, r.a.y+p.y+1, r.a.x+p.x+13, r.a.y+p.y+13, VIEW(o)->get_color(VIEW(o), 0));
    line(out, r.a.x+p.x+1, r.a.y+p.y+13, r.a.x+p.x+13, r.a.y+p.y+13, VIEW(o)->get_color(VIEW(o), 0));

    textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x+17, r.a.y+p.y+1,
                           r.b.x+p.x-1, r.b.y+p.y-1, TX_ALIGN_LEFT, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 0);

    if ( OBJECT(o)->is_state(OBJECT(o), OB_SF_FOCUSED) ) {
      rect(out, r.a.x+p.x+16, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, VIEW(o)->get_color(VIEW(o), 4));
    };
    if (o->state == 1) {
      line(out, r.a.x+p.x+3, r.a.y+p.y+3, r.a.x+p.x+11, r.a.y+p.y+11, COLOR(0));
      line(out, r.a.x+p.x+11, r.a.y+p.y+3, r.a.x+p.x+3, r.a.y+p.y+11, COLOR(0));
      line(out, r.a.x+p.x+4, r.a.y+p.y+3, r.a.x+p.x+11, r.a.y+p.y+10, COLOR(0));
      line(out, r.a.x+p.x+10, r.a.y+p.y+3, r.a.x+p.x+3, r.a.y+p.y+10, COLOR(0));
      line(out, r.a.x+p.x+3, r.a.y+p.y+4, r.a.x+p.x+10, r.a.y+p.y+11, COLOR(0));
      line(out, r.a.x+p.x+11, r.a.y+p.y+4, r.a.x+p.x+4, r.a.y+p.y+11, COLOR(0));
    }
  };

  VIEW(o)->end_of_paint(VIEW(o), r);
};

void bad_checkbox_change_state(p_bad_checkbox o, l_int newstate)
{
  o->state = newstate;
  VIEW(o)->draw(VIEW(o));
};

void bad_checkbox_change_caption(p_bad_checkbox o, l_text newcap)
{
  //afree(&(BAD_CHECKBOX(o)->caption));
  if(BAD_CHECKBOX(o)->caption) _free(BAD_CHECKBOX(o)->caption);
  o->caption = (l_text)_strdup((char*)newcap);
  VIEW(o)->draw(VIEW(o));
}

void bad_checkbox_draw(p_view o)
{
  BAD_CHECKBOX(o)->draw_state(BAD_CHECKBOX(o), false);
};

p_bad_checkbox _bad_checkbox_init(p_bad_checkbox o, t_rect r, l_text cap, l_dword msg, l_int st)
{
  if (!o) return NULL;

  clear_type(o, sizeof(t_bad_checkbox));

  view_init(&o->obclass, r);

  o->caption = (l_text)_strdup((char*)cap);
  o->message = msg;
  o->state = st;

  OBJECT(o)->translate_event = &bad_checkbox_translate_event;
  OBJECT(o)->set_state = &bad_checkbox_set_state;
  OBJECT(o)->done = &bad_checkbox_done;

  VIEW(o)->draw = &bad_checkbox_draw;
  VIEW(o)->set_palette(VIEW(o), pal_bad_common);

  o->draw_state = &bad_checkbox_draw_state;
  o->change_state = &bad_checkbox_change_state;
  o->change_caption = &bad_checkbox_change_caption;

  return o;
};

/* t_bad_button */
l_bool bad_button_done(p_object o)
{
  if (!view_done(o)) return false;
  //afree(&(BAD_BUTTON(o)->caption));
  if(BAD_BUTTON(o)->caption){_free(BAD_BUTTON(o)->caption);BAD_BUTTON(o)->caption=NULL;}
  return true;
};

void bad_button_set_state(p_object o, l_dword st, l_bool set)
{
  view_set_state(o, st, set);

  if ( st & OB_SF_FOCUSED ) {

    VIEW(o)->draw_view(VIEW(o));

  };
};

void bad_button_translate_event(p_object o, t_event *event)
{
  RETVIEW(o, event);

  view_translate_event(o, event);

  /* check if we're clicked by the mouse */
  if (event->type & EV_MOUSE && OBJECT(mouse)->state & MO_SF_MOUSELDOWN)
  {
    l_bool in_view = false;

    if (VIEW(o)->is_mouse_in_view(VIEW(o))) {
      BAD_BUTTON(o)->draw_state(BAD_BUTTON(o), in_view);
    };

    while (OBJECT(mouse)->state & MO_SF_MOUSELPRESS)
    {
      l_bool safe_in_view = in_view;
      in_view = VIEW(o)->is_mouse_in_view(VIEW(o));
      if (in_view != safe_in_view) {
        BAD_BUTTON(o)->draw_state(BAD_BUTTON(o), in_view);
      };

      o->get_event(o, event);
    };

    if (in_view) {
      if (BAD_BUTTON(o)->state < 2) {
        if (BAD_BUTTON(o)->switch_button == true)
          BAD_BUTTON(o)->state = 1-BAD_BUTTON(o)->state;
        else
          BAD_BUTTON(o)->state = 0;
        BAD_BUTTON(o)->draw_state(BAD_BUTTON(o), false);
        set_event(event, EV_MESSAGE, BAD_BUTTON(o)->message, o);
        o->put_event(o, event);
        clear_event(event);
      } else {
        BAD_BUTTON(o)->state = 0;
        BAD_BUTTON(o)->draw_state(BAD_BUTTON(o), false);
      };
    };
  };

};

void bad_button_draw_state(p_bad_button o, l_int astate)
{
  t_rect r = VIEW(o)->get_local_extent(VIEW(o));
  t_point p;

  BITMAP *out = VIEW(o)->begin_paint(VIEW(o), &p, r);
  if ( out ) {
    rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, VIEW(o)->get_color(VIEW(o), 0));
    switch (o->style) {
      case BBS_SEAL: {
        if (astate)
          draw_bevel(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(0), COLOR(15));
        else
          draw_bevel(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(15), COLOR(0));
        textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 0);
      } break;
      case BBS_BADSEAL: {
        if (astate) {
          draw_bevel(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, VIEW(o)->get_color(VIEW(o), 2), VIEW(o)->get_color(VIEW(o), 1));
          textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 0);
        } else {
          draw_bevel(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, VIEW(o)->get_color(VIEW(o), 2), VIEW(o)->get_color(VIEW(o), 3));
          draw_bevel(out, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x-1, r.b.y+p.y-1, VIEW(o)->get_color(VIEW(o), 1), VIEW(o)->get_color(VIEW(o), 2));
          textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 0);
        };
      } break;
      case BBS_KDE: {
        if (astate) {
          draw_bevel(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, VIEW(o)->get_color(VIEW(o), 2), VIEW(o)->get_color(VIEW(o), 1));
          draw_bevel(out, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x-1, r.b.y+p.y-1, VIEW(o)->get_color(VIEW(o), 2), VIEW(o)->get_color(VIEW(o), 1));
          textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x+1, r.b.y+p.y+1, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 0);
        } else {
          draw_bevel(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, VIEW(o)->get_color(VIEW(o), 1), VIEW(o)->get_color(VIEW(o), 2));
          draw_bevel(out, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x-1, r.b.y+p.y-1, VIEW(o)->get_color(VIEW(o), 1), VIEW(o)->get_color(VIEW(o), 2));
          textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 0);
        };
      } break;
      case BBS_WIN95: {
        if (astate) {
          draw_bevel(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(0), COLOR(0));
          draw_bevel(out, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x-1, r.b.y+p.y-1, VIEW(o)->get_color(VIEW(o), 2), VIEW(o)->get_color(VIEW(o), 2));
          textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x+1, r.b.y+p.y+1, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 0);
        } else {
          draw_bevel(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, VIEW(o)->get_color(VIEW(o), 1), COLOR(0));
          draw_bevel(out, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x-1, r.b.y+p.y-1, VIEW(o)->get_color(VIEW(o), 0), VIEW(o)->get_color(VIEW(o), 2));
          textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 0);
        };
      } break;
      case BBS_WIN31: {
        line(out, r.a.x+p.x, r.a.y+p.y+1, r.a.x+p.x, r.b.y+p.y-1, VIEW(o)->get_color(VIEW(o), 4));
        line(out, r.b.x+p.x, r.a.y+p.y+1, r.b.x+p.x, r.b.y+p.y-1, VIEW(o)->get_color(VIEW(o), 4));
        line(out, r.a.x+p.x+1, r.a.y+p.y, r.b.x+p.x-1, r.a.y+p.y, VIEW(o)->get_color(VIEW(o), 4));
        line(out, r.a.x+p.x+1, r.b.y+p.y, r.b.x+p.x-1, r.b.y+p.y, VIEW(o)->get_color(VIEW(o), 4));
        if (astate) {
          draw_bevel(out, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x-1, r.b.y+p.y-1, VIEW(o)->get_color(VIEW(o), 2), VIEW(o)->get_color(VIEW(o), 0));
          textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x+1, r.b.y+p.y+1, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 0);
          textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x+2, r.a.y+p.y+1, r.b.x+p.x+2, r.b.y+p.y+1, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 0);
        } else {
          draw_bevel(out, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x-1, r.b.y+p.y-1, VIEW(o)->get_color(VIEW(o), 1), VIEW(o)->get_color(VIEW(o), 2));
          draw_bevel(out, r.a.x+p.x+2, r.a.y+p.y+2, r.b.x+p.x-2, r.b.y+p.y-2, VIEW(o)->get_color(VIEW(o), 1), VIEW(o)->get_color(VIEW(o), 2));
          textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 0);
          textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x+1, r.a.y+p.y, r.b.x+p.x+1, r.b.y+p.y, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 0);
        };
      } break;
      case BBS_MODERN: {
        if (astate) {
          draw_bevel(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, VIEW(o)->get_color(VIEW(o), 2), VIEW(o)->get_color(VIEW(o), 2));
          textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x+2, r.a.y+p.y+2, r.b.x+p.x+2, r.b.y+p.y+1, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 1), TX_NOCOLOR, 0);
          textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x+1, r.b.y+p.y+1, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 2), TX_NOCOLOR, 0);
        } else {
          draw_bevel(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, VIEW(o)->get_color(VIEW(o), 1), VIEW(o)->get_color(VIEW(o), 2));
          textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x+1, r.b.y+p.y+1, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 1), TX_NOCOLOR, 0);
          textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 2), TX_NOCOLOR, 0);
        };
      } break;
      case BBS_LIQUID: {
        if (astate) {
          stretch_sprite(out, Get_Bitmap(2), r.a.x+p.x, r.a.y+p.y, rect_sizex(r), rect_sizey(r));
        } else {
          stretch_sprite(out, Get_Bitmap(1), r.a.x+p.x, r.a.y+p.y, rect_sizex(r), rect_sizey(r));
        };
        textout_draw_rect(out, VIEW(o)->font, o->caption, -1, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTER, COLOR(0), TX_NOCOLOR, 0);
      } break;
    };
  };

  VIEW(o)->end_of_paint(VIEW(o), r);
};

void bad_button_change_state(p_bad_button o, l_int newstate)
{
  o->state = newstate;
  VIEW(o)->draw(VIEW(o));
};

void bad_button_change_style(p_bad_button o, l_int newstyle)
{
  o->style = newstyle;
  VIEW(o)->draw(VIEW(o));
};

void bad_button_change_caption(p_bad_button o, l_text newcap)
{
  //afree(&(BAD_BUTTON(o)->caption));
  if(o->caption) _free(o->caption);
  o->caption = (l_text)_strdup((char*)newcap);
  VIEW(o)->draw(VIEW(o));
}

void bad_button_draw(p_view o)
{
  BAD_BUTTON(o)->draw_state(BAD_BUTTON(o), false);
};

p_bad_button _bad_button_init(p_bad_button o, t_rect r, l_text cap, l_int style, l_dword msg)
{
  if (!o) return NULL;

  clear_type(o, sizeof(t_bad_button));

  view_init(&o->obclass, r);

  o->caption = (l_text)_strdup((char*)cap);
  o->message = msg;
  o->state = 0;
  o->style = style;

  OBJECT(o)->translate_event = &bad_button_translate_event;
  OBJECT(o)->set_state = &bad_button_set_state;
  OBJECT(o)->done = &bad_button_done;

  VIEW(o)->draw = &bad_button_draw;
  VIEW(o)->set_palette(VIEW(o), pal_bad_common);

  o->draw_state = &bad_button_draw_state;
  o->change_state = &bad_button_change_state;
  o->change_style = &bad_button_change_style;
  o->change_caption = &bad_button_change_caption;

  return o;
};

/* t_bad_vtrackbar */
void bad_vtrackbar_set_state(p_object o, l_dword st, l_bool set)
{
  view_set_state(o, st, set);

  if ( st & OB_SF_FOCUSED ) {

    VIEW(o)->draw_view(VIEW(o));

  };
};

void bad_vtrackbar_translate_event(p_object o, t_event *event)
{
  RETVIEW(o, event);

  view_translate_event(o, event);

  /* check if we're clicked by the mouse */
  if (event->type & EV_MOUSE && OBJECT(mouse)->state & MO_SF_MOUSELDOWN)
  {
    t_point p = VIEW(o)->get_global_point(VIEW(o), point_assign(0, 0));
    t_rect r = VIEW(o)->get_local_extent(VIEW(o));
    l_long my = (r.b.y-r.a.y-4);
    l_long first_pos = BAD_VTRACKBAR(o)->pos;
    l_bool in_view=false;

    VIEW(o)->draw(VIEW(o));

    while (OBJECT(mouse)->state & MO_SF_MOUSELPRESS)
    {
      l_bool old_in_view = in_view;
      l_long old_pos = BAD_VTRACKBAR(o)->pos;
      in_view = VIEW(o)->is_mouse_in_view(VIEW(o));

      if (in_view) BAD_VTRACKBAR(o)->pos = ((mouse->where.y)-p.y)*BAD_VTRACKBAR(o)->max/my;

      if (BAD_VTRACKBAR(o)->pos > BAD_VTRACKBAR(o)->max) BAD_VTRACKBAR(o)->pos = BAD_VTRACKBAR(o)->max;
      if (BAD_VTRACKBAR(o)->pos < BAD_VTRACKBAR(o)->min) BAD_VTRACKBAR(o)->pos = BAD_VTRACKBAR(o)->min;

      if (in_view != old_in_view)
      {
        if (in_view) BAD_VTRACKBAR(o)->pressed = true; else BAD_VTRACKBAR(o)->pressed = false;
        VIEW(o)->draw(VIEW(o));
      };

      if (BAD_VTRACKBAR(o)->pos != old_pos) VIEW(o)->draw(VIEW(o));

      o->get_event(o, event);
    };
    BAD_VTRACKBAR(o)->pressed = false;
    VIEW(o)->draw(VIEW(o));

    if (BAD_VTRACKBAR(o)->pos != first_pos)
    {
      VIEW(o)->draw(VIEW(o));

      set_event(event, EV_MESSAGE, BAD_VTRACKBAR(o)->message, o);
      o->put_event(o, event);
      clear_event(event);
    };
  };
};

void bad_vtrackbar_change_pos(p_bad_vtrackbar o, l_long pos)
{
  o->pos = pos;
  VIEW(o)->draw(VIEW(o));
};

void bad_vtrackbar_draw(p_view o)
{
  t_rect r = VIEW(o)->get_local_extent(VIEW(o));
  t_point p;
  l_long px, py, my;

  BITMAP *out = VIEW(o)->begin_paint(VIEW(o), &p, r);

  if ( out ) {
    px = r.a.x+p.x+((r.b.x-r.a.x)/2);
    my = (r.b.y-r.a.y-8);
    if (BAD_VTRACKBAR(o)->max == 0) py = r.a.y+p.y+4; else py = BAD_VTRACKBAR(o)->pos*my/BAD_VTRACKBAR(o)->max+r.a.y+p.y+4;
    rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, VIEW(o)->get_color(VIEW(o), 0));
    draw_bevel(out, px-1, r.a.y+p.y+4, px+1, r.b.y+p.y-4, VIEW(o)->get_color(VIEW(o), 2), VIEW(o)->get_color(VIEW(o), 1));
    vline(out, px, r.a.y+p.y+5, r.b.y+p.y-5, COLOR(0));
    rectfill(out, px-10, py-4, px+10, py+4, VIEW(o)->get_color(VIEW(o), 0));
    draw_bevel(out, px-10, py-4, px+10, py+4, VIEW(o)->get_color(VIEW(o), 1), VIEW(o)->get_color(VIEW(o), 3));
    draw_bevel(out, px-9, py-3, px+9, py+3, VIEW(o)->get_color(VIEW(o), 0), VIEW(o)->get_color(VIEW(o), 2));
    if (BAD_VTRACKBAR(o)->pressed) draw_bevel(out, px-7, py-1, px+7, py+1, VIEW(o)->get_color(VIEW(o), 2), VIEW(o)->get_color(VIEW(o), 1));
  };

  VIEW(o)->end_of_paint(VIEW(o), r);
};

p_bad_vtrackbar _bad_vtrackbar_init(p_bad_vtrackbar o, t_rect r, l_dword msg)
{
  if (!o) return NULL;

  clear_type(o, sizeof(t_bad_vtrackbar));

  view_init(&o->obclass, r);

  o->message = msg;
  o->min = 0;
  o->max = 100;
  o->pos = 0;
  o->pressed = false;

  OBJECT(o)->translate_event = &bad_vtrackbar_translate_event;
  OBJECT(o)->set_state = &bad_vtrackbar_set_state;

  VIEW(o)->draw = &bad_vtrackbar_draw;
  VIEW(o)->set_palette(VIEW(o), pal_bad_common);

  o->change_pos = &bad_vtrackbar_change_pos;

  return o;
};

/* t_bad_tabbook */
void bad_tabbook_set_state(p_object o, l_dword st, l_bool set)
{
  view_set_state(o, st, set);
  if ( st & OB_SF_FOCUSED ) {
    VIEW(o)->draw_view(VIEW(o));
  };
};

void bad_tabbook_translate_event(p_object o, t_event *event)
{
  t_rect r = VIEW(o)->get_local_extent(VIEW(o));
  t_point p = VIEW(o)->get_global_point(VIEW(o), point_assign(0, 0));
  l_int i;

// jdh moved - surely we dont want to do anything for invisible/non-selected views.
  RETVIEW(o, event);

  view_translate_event(o, event);
//  RETVIEW(o, event);

  /* check if we're clicked by the mouse */
  if (event->type & EV_MOUSE && OBJECT(mouse)->state & MO_SF_MOUSELDOWN)
  {
    l_int tab_h = 21;
    l_int x;
    l_int old_active = BAD_TABBOOK(o)->active;
    x = r.a.x+p.x+1;
    for (i=0;i<BAD_TABBOOK(o)->tabs;i++) {
      l_int tab_width = FONT_GETSTRWIDTH(VIEW(o)->font, BAD_TABBOOK(o)->title[i], -1)+10;
      l_int y1 = r.a.y+p.y+1;
      t_rect rr;
      if (i == BAD_TABBOOK(o)->active) y1--;
      rr = rect_assign(x, y1, x+tab_width, r.a.y+p.y+tab_h);
      if ((mouse->where.x >= rr.a.x) && (mouse->where.x <= rr.b.x) &&
          (mouse->where.y >= rr.a.y) && (mouse->where.y <= rr.b.y) ) {
//        BAD_TABBOOK(o)->active = i;
        if (i != old_active) {
          BAD_TABBOOK(o)->select_tab(BAD_TABBOOK(o), i);
        };
        while (OBJECT(mouse)->state & MO_SF_MOUSELPRESS)
        {
          o->get_event(o, event);
        };
        break;
      };
      x += tab_width+3;
    };
  };
  if (event->type != EV_NOTHING)
  for (i=0;i<BAD_TABBOOK(o)->tabs;i++)
    OBJECT(BAD_TABBOOK(o)->tab[i])->translate_event(OBJECT(BAD_TABBOOK(o)->tab[i]), event);
};

void bad_tabbook_tab_translate_event(p_object o, t_event *event)
{
  p_object oo, f = o->first_view(o);
// jdh added - dont even try if invisible/inactive
  RETVIEW(o, event);

  view_translate_event(o, event);

/* How does this happen? Probably o->owner->owner is not a BAD_TABBOOK at all
In which case why are we being called? Should it be o->owner rather than o->owner->owner perhaps.
*/
if( (unsigned long)(BAD_TABBOOK(o->owner->owner)->active) > 63 ) return;


//fprintf(stderr,"btb_tab_translate_event %lx\n",(unsigned long)(BAD_TABBOOK(o->owner->owner)->active));
//fprintf(stderr,"btb_tab_translate_event %lx\n",(unsigned long)BAD_TABBOOK(o->owner->owner)->tab[BAD_TABBOOK(o->owner->owner)->active]);

/* BAD_TABBOOK(o->owner->owner)->active could be an invalid pointer not an integer at this point! Sudden death. */

  if (VIEW(o) != BAD_TABBOOK(o->owner->owner)->tab[BAD_TABBOOK(o->owner->owner)->active]) return;

//fprintf(stderr,"btb_tab_translate_event 3\n");

  oo = f;
  do {
    if (oo != o) oo->translate_event(oo, event);
    oo = oo->next_view(oo);
  } while (oo != f);
};

void bad_tabbook_draw_tab(p_view o)
{
  t_rect r = VIEW(o)->get_local_extent(VIEW(o));
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {
    rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, o->get_color(o, 0));
  };

  o->end_of_paint(o, r);
};

void bad_tabbook_draw(p_view o)
{
  t_rect r = VIEW(o)->get_local_extent(VIEW(o));
  t_point p;
  BITMAP *out = VIEW(o)->begin_paint(VIEW(o), &p, r);

  if ( out ) {
    l_int tab_h = 21;
    l_int i,x;

    rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.a.x+p.y+tab_h, VIEW(o)->get_color(VIEW(o), 0));
    draw_bevel(out, r.a.x+p.x, r.a.y+p.y+tab_h, r.b.x+p.x-1, r.b.y+p.y-1, VIEW(o)->get_color(VIEW(o), 1), VIEW(o)->get_color(VIEW(o), 2));
    draw_bevel(out, r.a.x+p.x, r.a.y+p.y+tab_h, r.b.x+p.x, r.b.y+p.y, VIEW(o)->get_color(VIEW(o), 1), VIEW(o)->get_color(VIEW(o), 3));

    x = r.a.x+p.x+1;
    for (i=0;i<BAD_TABBOOK(o)->tabs;i++) {
      l_int tab_width = FONT_GETSTRWIDTH(VIEW(o)->font, BAD_TABBOOK(o)->title[i], -1)+10;
      l_int y1 = r.a.y+p.y+1;
      l_int xx = 0;

      if (i == BAD_TABBOOK(o)->active) {
        y1--;
        xx = 1;
      };

      line(out, x-xx, y1+1, x-xx, r.a.y+p.y+tab_h-1, VIEW(o)->get_color(VIEW(o), 1));
      line(out, x-xx+1, y1, x+tab_width+xx-1, y1, VIEW(o)->get_color(VIEW(o), 1));
      line(out, x+tab_width+xx-1, y1+1, x+xx+tab_width-1, r.a.y+p.y+tab_h-1, VIEW(o)->get_color(VIEW(o), 2));
      line(out, x+tab_width+xx, y1+1, x+tab_width+xx, r.a.y+p.y+tab_h-1, VIEW(o)->get_color(VIEW(o), 3));

      if (i == BAD_TABBOOK(o)->active) {
        textout_draw_rect(out, VIEW(o)->font, BAD_TABBOOK(o)->title[i], -1, x+2, r.a.y+p.y, x+tab_width-2, r.a.y+p.y+tab_h-2, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 0);
        line(out, x-xx+1, r.a.y+p.y+tab_h, x+tab_width+xx-1, r.a.y+p.y+tab_h, VIEW(o)->get_color(VIEW(o), 0));
      } else {
        light_image(out, x-xx+2, y1+1, x+tab_width+xx-1, r.a.y+p.y+tab_h-1, -10, -10, -10);
        textout_draw_rect(out, VIEW(o)->font, BAD_TABBOOK(o)->title[i], -1, x+2, r.a.y+p.y+1, x+tab_width-2, r.a.y+p.y+tab_h-1, TX_ALIGN_CENTER, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 0);
      };

      x += tab_width+3;
    };
  };
  VIEW(o)->end_of_paint(VIEW(o), r);
};

void bad_tabbook_select_tab(p_bad_tabbook o, l_int newtab)
{
  if (newtab != o->active) {
    t_rect r = rect_assign(-5, -5, -4, -4);
    BAD_TABBOOK(o)->tab[o->active]->change_bounds(BAD_TABBOOK(o)->tab[o->active], r);
    o->active = newtab;
    VIEW(o)->draw(VIEW(o));
    r = VIEW(o)->bounds;
    r = rect_assign(1, 22, rect_sizex(r)-2, rect_sizey(r)-2);
    BAD_TABBOOK(o)->tab[o->active]->change_bounds(BAD_TABBOOK(o)->tab[o->active], r);
//    BAD_TABBOOK(o)->tab[newtab]->show(BAD_TABBOOK(o)->tab[newtab]);
    if (o->on_changed) o->on_changed(o, o->active);
  };
};

void bad_tabbook_add_tab(p_bad_tabbook o, l_text title)
{
 p_view temp;
  t_rect r = VIEW(o)->bounds;
  if (o->tabs == 64) return;
// jdh protection against out of memory condition (shouldn't we return some status, to say we did it)
  temp=malloc(sizeof(t_view));
  if(!temp) return;

//  r = rect_assign(1, 22, rect_sizex(r)-2, rect_sizey(r)-2);
  r = rect_assign(-5, -5, -4, -4);
  o->title[o->tabs] = strdup(title);
//  o->tab[o->tabs] = view_init(malloc(sizeof(t_view)), r);
  o->tab[o->tabs] = view_init(temp, r);

  o->tab[o->tabs]->set_palette(o->tab[o->tabs], pal_bad_common);
  o->tab[o->tabs]->draw = &bad_tabbook_draw_tab;
  OBJECT(o->tab[o->tabs])->translate_event = &bad_tabbook_tab_translate_event;
//  VIEW(o->tab[o->tabs])->hide(VIEW(o->tab[o->tabs]));
  OBJECT(o)->insert(OBJECT(o), OBJECT(o->tab[o->tabs]));
  o->tabs++;
  VIEW(o)->draw(VIEW(o));
  VIEW(o->tab[o->active])->draw(VIEW(o->tab[o->active]));
};

void bad_tabbook_remove_tab(p_bad_tabbook o, l_int tab)
{
  // do nothing
};

p_bad_tabbook _bad_tabbook_init(p_bad_tabbook o, t_rect r, l_text ftitle)
{
  t_rect rr;
  p_view temp;

  if (!o) return NULL;
// jdh move to protect against out of memory conditions
  temp=malloc(sizeof(t_view));
  if (!temp) return NULL;

  clear_type(o, sizeof(t_bad_tabbook));

  view_init(&o->obclass, r);

  rr = rect_assign(1, 22, rect_sizex(r)-2, rect_sizey(r)-2);

  o->title[0] = strdup(ftitle);
//  o->tab[0] = view_init(malloc(sizeof(t_view)), rr);
  o->tab[0] = view_init(temp, rr);
  o->active = 0;
  o->tabs = 1;

  o->tab[0]->set_palette(o->tab[0], pal_bad_common);
  o->tab[0]->draw = &bad_tabbook_draw_tab;
  OBJECT(o->tab[0])->translate_event = &bad_tabbook_tab_translate_event;
  OBJECT(o)->insert(OBJECT(o), OBJECT(o->tab[0]));

  OBJECT(o)->translate_event = &bad_tabbook_translate_event;
  OBJECT(o)->set_state = &bad_tabbook_set_state;

  VIEW(o)->draw = &bad_tabbook_draw;
  VIEW(o)->set_palette(VIEW(o), pal_bad_common);

  o->select_tab = &bad_tabbook_select_tab;
  o->add_tab = &bad_tabbook_add_tab;
  o->on_changed = NULL;

  return o;
};

/* t_bad_iconbox */

void bad_iconbox_draw_icon(p_bad_iconbox o, BITMAP *out, t_point p, l_int item, l_bool selected)
{
  p_bad_iconbox_item i = o->icons->at(o->icons, item);
  l_color c = COLOR(0);
  l_color b = TX_NOCOLOR;
  l_int w, h;
  if (i)
  {
    if (selected)
    {
      c = COLOR(15);
      b = COLOR(1);
    };

#ifndef NO_SCREEN
// jdh this goes bad if I run DUMMY mode without a graphics screen (and I have not the slightest idea why its wrong)
// Yow! Stretch_sprite is real terrifying stuff, it generates code on the fly to stretch the line and then executes it.
// Documented as being not terribly safe and in particular not to copy from outside source bitmap.
if ( out && i->icon)
  if((out->w >= (i->pos.x+p.x+32)) && (out->h >= (i->pos.y+p.y) )) // I think my problem is that its zero sized but
                                                                   // that test is a good idea given the warning above
     stretch_sprite(out, i->icon, i->pos.x+p.x, i->pos.y+p.y, 32, 32);
#endif
    get_size_of_ftext(i->caption, VIEW(o)->font, &w, &h);
    textout_draw_rect(out, VIEW(o)->font, i->caption, -1, i->pos.x+p.x+16-w/2, i->pos.y+p.y+38, i->pos.x+p.x+16+w/2, i->pos.y+p.y+38+h, TX_ALIGN_CENTER, c, b, 1);
  };
}

void bad_iconbox_draw(p_view o)
{
  p_bad_iconbox ib = ((p_bad_iconbox)(o));
  l_int i;
  t_rect r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(VIEW(o), &p, r);
  if (out) {
    if (ib->flags & BIB_BACKGROUND_IMAGE) {
      if (ib->flags & BIB_DESKTOP_PLACEMENT)
        stretch_sprite(out, o->brush.background, 0, 0, desktop->bounds.b.x, desktop->bounds.b.y);
      else
        stretch_sprite(out, o->brush.background, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y);
    } else
      fade_rect(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, o->brush.color, o->brush.color2, FR_VER);
    if (ib->count > 0) {
/*
 draws all icons unselected then redraws a selected one (the zeroth icon is
 automatically selected (single click) before we get any events since ib->index is initialised to 0
 I guess it should probably be initialised to -1. I cant see how ib->index could ever get to be -1 in fact
 which makes the test ib->index > -1 rather pointless.
*/
      for (i=0;i<ib->count;i++) bad_iconbox_draw_icon(ib, out, p, i, false);
      if (ib->index > -1) bad_iconbox_draw_icon(ib, out, p, ib->index, true);
    };
  };
  r.b.x += 15;
  o->end_of_paint(o, r);
};

l_bool bad_iconbox_mouse_under_icon_at(p_bad_iconbox o, l_int item)
{
  p_bad_iconbox_item i = o->icons->at(o->icons, item);
  if (i)
  {
    t_point p = VIEW(o)->get_global_point(VIEW(o), point_assign(0, 0));
    if (mouse->where.x >= i->pos.x+p.x && mouse->where.x <= i->pos.x+p.x+31 && mouse->where.y >= i->pos.y+p.y && mouse->where.y <= i->pos.y+p.y+31) return true;
  };
  return false;
}

void bad_iconbox_add(p_bad_iconbox o, BITMAP *icon, l_text caption)
{
  p_bad_iconbox_item i = (t_bad_iconbox_item *) malloc(sizeof(t_bad_iconbox_item));
  /* NB 24 bytes wasted for each of these. */
  i->pos = o->next_pos;
  i->icon = icon;
  i->caption = caption;

  o->icons->insert(o->icons, i); /* these additions appear never to be released jdh */
  o->count += 1;
  if (o->flags & BIB_PLACE_HORIZONTAL) {
    o->next_pos.x += 38+o->spacing.x;
    if (o->next_pos.x+o->spacing.x+32 > VIEW(o)->bounds.b.x) {
      o->next_pos.y += 38+o->spacing.y;
      o->next_pos.x = o->spacing.x/2;
    };
  } else {
    o->next_pos.y += 38+o->spacing.y;
    if (o->next_pos.y+o->spacing.y+32 > VIEW(o)->bounds.b.y) {
      o->next_pos.x += 38+o->spacing.x;
      o->next_pos.y = o->spacing.y/2;
    };
  };
  VIEW(o)->draw(VIEW(o));
};

void bad_iconbox_clear(p_bad_iconbox o)
{
  o->icons->free_all(o->icons);
  o->count = 0;
  VIEW(o)->draw(VIEW(o));
};

void bad_iconbox_translate_event(p_object o, t_event *event)
{
  RETVIEW(o, event);
  if ( o->phase == PH_PREPROCESS ) return;
  if (VIEW(o)->is_mouse_in_view(VIEW(o)))
  if ((event->type & EV_MOUSE) && ((OBJECT(mouse)->state & MO_SF_MOUSELDOWN) || (OBJECT(mouse)->state & MO_SF_MOUSELDOUBLE)))
  {
    p_bad_iconbox ib = ((p_bad_iconbox)(o));
    l_int i, idx;
    idx = -1;
    if (ib->count > 0) {
      for (i=0;i<ib->count;i++) { // going to be painfully slow with hundreds of icons aint it? 
       if (bad_iconbox_mouse_under_icon_at(ib, i)) {
        if (OBJECT(mouse)->state & MO_SF_MOUSELDOWN) { /* single click */
          idx = i;
          ib->index = i;
          VIEW(o)->draw(VIEW(o));
        } else { /* double click */
          if (ib->on_dclick != NULL) ib->on_dclick(ib, i);
        };
        break;
       };
      }
    };
    while (OBJECT(mouse)->state & MO_SF_MOUSELPRESS) o->get_event(o, event);
    clear_event(event);
  };
};

p_bad_iconbox _bad_iconbox_init(p_bad_iconbox o, t_rect r, l_big flags)
{
  if (!o) return NULL;
  clear_type(o, sizeof(t_bad_iconbox)); /* with a monster 512+128 bytes wasted on each of these */
  view_init(&o->obclass, r);
  o->icons = list_init(malloc(sizeof(t_list)), &free, 0); /* possibly never to be released jdh */
  o->next_pos = point_assign(21, 21);
  o->caption_size = point_assign(64, 24);
  o->spacing = point_assign(43, 43);
  o->count = 0;
  o->index = 0;
  o->flags = flags;
  o->on_dclick = NULL;
  o->add = &bad_iconbox_add;
  o->clear = &bad_iconbox_clear;
  VIEW(o)->draw = &bad_iconbox_draw;
  VIEW(o)->set_palette(VIEW(o), pal_bad_common);
  VIEW(o)->brush.color = pal_bad_common[6];
  VIEW(o)->brush.color2 = pal_bad_common[6];
  VIEW(o)->brush.background = NULL;
  OBJECT(o)->translate_event = &bad_iconbox_translate_event;
  return o;
};

#include "badctrls.exp"

lib_begin(void)
{
  if (ap_process == AP_ALLOC)
  {
    AP_EXPORTLIB();
  } else
  if (ap_process == AP_INIT)
  {
    AP_SETNUMOFCALLS(1);
    {
    dat = conv_to_skipcolor_data(DLXGetFileData(ap_id), CO_SKIP_RED, CO_SKIP_GREEN, CO_SKIP_BLUE);
    #ifdef BadSealVersion
    #ifdef UseRegistry
    iffound(pal_bad_common[0] , registry_color("3d_face"));
    iffound(pal_bad_common[1] , registry_color("3d_light"));
    iffound(pal_bad_common[2] , registry_color("3d_shadow"));
    iffound(pal_bad_common[3] , registry_color("3d_dark"));
    iffound(pal_bad_common[4] , registry_color("3d_border"));
    iffound(pal_bad_common[5] , registry_color("text"));
    iffound(pal_bad_common[6] , registry_color("flat_background"));
    iffound(pal_bad_common[7] , registry_color("flat_border"));
    iffound(pal_beditor[0] , registry_color("flat_background"));
    iffound(pal_beditor[1] , registry_color("3d_light"));
    iffound(pal_beditor[2] , registry_color("3d_shadow"));
    iffound(pal_beditor[3] , registry_color("3d_dark"));
    iffound(pal_beditor[4] , registry_color("3d_flat"));
    iffound(pal_beditor[5] , registry_color("edit_text"));
    #else
    iffound(pal_bad_common[0] , color_get_from_ini("3d_face"));
    iffound(pal_bad_common[1] , color_get_from_ini("3d_light"));
    iffound(pal_bad_common[2] , color_get_from_ini("3d_shadow"));
    iffound(pal_bad_common[3] , color_get_from_ini("3d_dark"));
    iffound(pal_bad_common[4] , color_get_from_ini("3d_border"));
    iffound(pal_bad_common[5] , color_get_from_ini("text"));
    iffound(pal_bad_common[6] , color_get_from_ini("flat_background"));
    iffound(pal_bad_common[7] , color_get_from_ini("flat_border"));
    iffound(pal_beditor[0] , color_get_from_ini("flat_background"));
    iffound(pal_beditor[1] , color_get_from_ini("3d_light"));
    iffound(pal_beditor[2] , color_get_from_ini("3d_shadow"));
    iffound(pal_beditor[3] , color_get_from_ini("3d_dark"));
    iffound(pal_beditor[4] , color_get_from_ini("3d_flat"));
    iffound(pal_beditor[5] , color_get_from_ini("edit_text"));
    #endif
    #else
    iffound(pal_bad_common[0] , color_get_from_ini("3D_background"));
    pal_bad_common[1] = COLOR(15);
    pal_bad_common[2] = COLOR(0);
    pal_bad_common[3] = COLOR(0);
    pal_bad_common[4] = COLOR(0);
    iffound(pal_bad_common[5] , color_get_from_ini("text"));
    iffound(pal_bad_common[6] , color_get_from_ini("text_input_background"));
    pal_bad_common[7] = COLOR(0);
    iffound(pal_beditor[0] , color_get_from_ini("text_input_background"));
    pal_beditor[1] = COLOR(15);
    pal_beditor[2] = COLOR(0);
    pal_beditor[3] = COLOR(0);
    iffound(pal_beditor[4] , color_get_from_ini("3D_background"));
    iffound(pal_beditor[5] , color_get_from_ini("text"));
    #endif
   }
  };
} lib_end;

