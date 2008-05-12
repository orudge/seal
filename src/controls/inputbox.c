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
#include <app.h>
#include <dialogs.h>
#include <grfx.h>
#include <inputbox.h>

////////////////////////////////////////////////////////////////////////////////
l_text inputbox (l_text caption, l_text txt, l_text defaut) {

  l_text retvalue    = NULL;
  p_stattext lbl     = NULL;
  p_textline textbox = NULL;
  p_button   b       = NULL;
  l_dword    msg     = NULL;
  p_appwin   w       = appwin_init(_malloc(sizeof(t_appwin)),
                            rect_assign(0, 0, 400, 120),
                            caption,
                            NULL,
                            NULL,
                            NULL);


  if ( w ) VIEW(w)->align |= TX_ALIGN_CENTER;

  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(w));

  lbl = stattext_init(_malloc(sizeof(t_stattext)),
                      rect_assign(5, 20, 395, 60),
                      TX_ALIGN_BOTTOM,
                      txt);

  OBJECT(w)->insert(OBJECT(w), OBJECT(lbl));

  textbox = textline_init(_malloc(sizeof(t_textline)),
                           rect_assign(5, 65, 395, 85),
                           IO_DIR_LIMIT,
                           0);
  OBJECT(w)->insert(OBJECT(w), OBJECT(textbox));
  TEXTLINE(textbox )->set_text(TEXTLINE(textbox ), defaut);

  b = button_init(_malloc(sizeof(t_button)),
                      rect_assign(5, 90, 100, 115),
                      TXT_OK,
                      MSG_OK,
                      BF_DEFAULT);
  OBJECT(w)->insert(OBJECT(w), OBJECT(b));

  b = button_init(_malloc(sizeof(t_button)),
                      rect_assign(105, 90, 205, 115),
                      TXT_CANCEL,
                      MSG_CLOSE,
                      BF_DEFAULT);
  OBJECT(w)->insert(OBJECT(w), OBJECT(b));

  msg = desktop->execute_view(desktop, VIEW(w));

  if ( msg == MSG_OK ) retvalue = _strdup(textbox->text);

  dispose(OBJECT(w));

  return retvalue;

};
////////////////////////////////////////////////////////////////////////////////
l_long inputbox_list (l_text caption, l_text txt, p_list xlist, l_long def ) {

  l_long retvalue    = -1;
  p_stattext lbl     = NULL;
  p_history  in      = NULL;
  p_button   b       = NULL;
  l_dword    msg     = NULL;
  p_appwin   w       = appwin_init(_malloc(sizeof(t_appwin)),
                            rect_assign(0, 0, 400, 120),
                            caption,
                            NULL,
                            NULL,
                            NULL);


  if ( w ) VIEW(w)->align |= TX_ALIGN_CENTER;


  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(w));


  lbl = stattext_init(_malloc(sizeof(t_stattext)),
                      rect_assign(5, 20, 395, 60),
                      TX_ALIGN_BOTTOM,
                      txt);

  OBJECT(w)->insert(OBJECT(w), OBJECT(lbl));

  in = history_init(_malloc(sizeof(t_history)),
                            rect_assign(5, 65, 380, 85),
                            xlist ,
                            150,
                            HF_REWRITEUNABLE|LF_SELFLIST);
  in->current = def;
  OBJECT(w)->insert(OBJECT(w), OBJECT(in));

  b = button_init(_malloc(sizeof(t_button)),
                      rect_assign(5, 90, 100, 115),
                      TXT_OK,
                      MSG_OK,
                      BF_DEFAULT);
  OBJECT(w)->insert(OBJECT(w), OBJECT(b));

  b = button_init(_malloc(sizeof(t_button)),
                      rect_assign(105, 90, 205, 115),
                      TXT_CANCEL,
                      MSG_CLOSE,
                      BF_DEFAULT);
  OBJECT(w)->insert(OBJECT(w), OBJECT(b));

  msg = desktop->execute_view(desktop, VIEW(w));

  if (msg == MSG_OK) retvalue = in->current;

  dispose(OBJECT(w));

  return retvalue;

};
////////////////////////////////////////////////////////////////////////////////
BITMAP *COLSEL_IMG = NULL;

l_color rgbcolorbox ( l_color def ) {

  l_int r = NULL;
  l_int g = NULL;
  l_int b = NULL;
  l_text d = set_format_text(NULL, "%d", getr(def));
  l_text t = inputbox ("RGB Color",INI_TEXT("Red (0-255) ?"),  d );
  _free(d);
  if ( !t ) return def;
  r = atoi(t);
  _free(t);
  if ( (r < 0 ) || (r > 255) ) {   msgbox( MW_INFO, MB_OK, INI_TEXT("Bad red value !")); return def; };

  d = set_format_text(NULL, "%d", getg(def));
  t = inputbox ("RGB Color",INI_TEXT("Green (0-255)"), d );
  _free(d);
  if ( !t ) return def;
  g = atoi(t);
  _free(t);
  if ( (g < 0 ) || (g > 255) ) {   msgbox( MW_INFO, MB_OK, INI_TEXT("Bad green value !")); return def; };

  d = set_format_text(NULL, "%d", getb(def));
  t = inputbox ("RGB Color",INI_TEXT("Blue (0-255)"), d );
  _free(d);
  if ( !t ) return def;
  b = atoi(t);
  _free(t);
  if ( (b < 0 ) || (b > 255) ) {   msgbox( MW_INFO, MB_OK, INI_TEXT("Bad blue value !")); return def; };

  return makecol(r,g,b);

};

////////////////////////////////////////////////////////////////////////////////
void   sel_draw ( p_view o ) {
  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);
  if ( out ) {
    if ( COLSEL_IMG ) stretch_sprite(out, COLSEL_IMG, p.x, p.y, r.b.x, r.b.y - 10);
    rectfill(out, p.x, p.y + r.b.y - 10, p.x + r.b.x, p.y + r.b.y, o->brush.color);
  };
  o->end_of_paint(o, r);
};
////////////////////////////////////////////////////////////////////////////////
void   sel_translate_event ( p_object o, t_event *event ) {
  RETVIEW(o, event);
  if ( event->type & EV_MOUSE ) {
    if ( OBJECT(mouse)->state & MO_SF_MOUSELUP   ) {
      p_view vo = VIEW(o);
      t_point p2 = vo->get_local_point(vo, mouse->where);
      if ( p2.x > 0 && p2.x < rect_sizex(vo->bounds) &&  p2.y > 0 && p2.y < rect_sizey(vo->bounds) - 10 ) {
        t_rect  r = vo->get_local_extent(vo);
        t_point p;
        t_point pe =  VIEW(desktop)->get_local_point(VIEW(desktop), mouse->where);
        BITMAP *out = vo->begin_paint(vo, &p, r);
        vo->brush.color = getpixel(out, pe.x, pe.y);
        vo->end_of_paint(vo, r);
        sel_draw (vo);
      } else if ( p2.x > 0 && p2.x < rect_sizex(vo->bounds) &&  p2.y > rect_sizey(vo->bounds) - 10 && p2.y < rect_sizey(vo->bounds)  )  {
        vo->brush.color = rgbcolorbox ( vo->brush.color );
        sel_draw (vo);
      };
    };
  };
};
////////////////////////////////////////////////////////////////////////////////
p_grfx sel_init( t_rect r, l_color def ) {
  p_grfx o = grfx_init(_malloc(sizeof(t_grfx)),r );
  VIEW(o)->draw = &sel_draw;
  OBJECT(o)->translate_event = &sel_translate_event;
  VIEW(o)->brush.color = def;
  return o;
};
////////////////////////////////////////////////////////////////////////////////
l_color selectcolor ( l_color def ) {
  p_grfx     xsel = NULL;
  p_button   b    = NULL;
  p_button   bx   = NULL;
  l_dword    msg  = NULL;
  p_appwin   w    = appwin_init(_malloc(sizeof(t_appwin)),
                            rect_assign(0, 0, 220, 205),
                            "Color",
                            NULL,
                            NULL,
                            NULL);

  if ( w ) VIEW(w)->align |= TX_ALIGN_CENTER;

  if ( !COLSEL_IMG ) COLSEL_IMG = load_image("bmp/colsel.bmp");

  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(w));

  xsel = sel_init ( rect_assign(13, 20, 210, 173), def );
  OBJECT(w)->insert(OBJECT(w), OBJECT(xsel));

  b = button_init(_malloc(sizeof(t_button)),
                      rect_assign(3, 180, 100, 200),
                      TXT_OK,
                      MSG_OK,
                      BF_DEFAULT);
  OBJECT(w)->insert(OBJECT(w), OBJECT(b));

  bx = button_init(_malloc(sizeof(t_button)),
                      rect_assign(103, 180, 203, 200),
                      TXT_CANCEL,
                      MSG_CLOSE,
                      BF_DEFAULT);
  OBJECT(w)->insert(OBJECT(w), OBJECT(bx));

  msg = desktop->execute_view(desktop, VIEW(w));

  if ( msg == MSG_OK ) def = VIEW(xsel)->brush.color;

  dispose(OBJECT(w));

  return def;
};
////////////////////////////////////////////////////////////////////////////////
