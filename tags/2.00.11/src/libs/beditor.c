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
/*                    Bad Editor for Bad Seal                   */
/*                             v0.1                             */
/*                                                              */
/*         (c) 2001 Kostas Michalopoulos aka Bad Sector         */
/*         first version for Bad Seal | www.badseal.org         */
/*                                                              */
/* ============================================================ */
/*  Controls included:                                          */
/*    t_beditor                                 editor control  */
/****************************************************************/

#include <string.h>
#include <allegro.h>
#include <seal.h>
#include <beditor.h>

/* 0 - flat background
   1 - 3d light
   2 - 3d shadow
   3 - 3d dark
   4 - 3d face
   5 - text color */

l_color pal_beditor[] = {CO_WHITE, CO_WHITE, CO_DARKGRAY, CO_BLACK, CO_LIGHTGRAY, CO_BLACK, CO_NOCOLOR};

p_beditor (*beditor_init)(p_beditor o, t_rect r, l_int flags) = &_beditor_init;

l_text get_text_line(l_text whole, l_int start)
{
  l_text r = (l_text) malloc(strlen(whole)+1);
  l_int c = start, rp = 0, wlen = strlen(whole);

  if (c < 0) c = 0;
  r[rp] = 0;
  if (c < wlen && whole[c] != '\n')
  do {
    r[rp] = whole[c];
    c++;
    rp++;
  } while (c < wlen && whole[c] != '\n');
  r[rp] = 0;

  c = strlen(r);
  r = (l_text) realloc(r, c+1);

  return r;
}

void beditor_set_text(p_beditor o, l_text text)
{
/* UNUSED
  l_int c;
*/
  l_int i;
  l_text line;
  if (o->text) free(o->text);

  o->text = strdup(text);

  o->lines = 1;
  for (i=0;i<strlen(text);i++) if (text[i] == '\n') o->lines++;
  line = get_text_line(o->text, 0);
  o->line_len = strlen(line)+1;
  free(line);
  o->line_pos = 0;
  o->page_start = 0;
  o->sel_start = 0;
  o->x = 1;
  o->y = 1;
  o->px = 0;
  o->py = 0;
  o->cursor = TRUE;
  o->modified = FALSE;

  VIEW(o)->draw(VIEW(o));
}

void beditor_draw_line(p_beditor o, BITMAP *out, t_rect r, l_text txt)
{
/* UNUSED
  l_int line_height = FONT_GETHEIGHT(VIEW(o)->font)+1;
*/
  textout_draw_rect(out, VIEW(o)->font, txt, -1, r.a.x, r.a.y, r.b.x, r.b.y, TX_ALIGN_LEFT|TX_ALIGN_TOP, VIEW(o)->get_color(VIEW(o), 5), TX_NOCOLOR, 1);
}

void beditor_draw(p_view o)
{
  t_rect  r = o->get_local_extent(o);
  t_point p;
  BITMAP *out = o->begin_paint(o, &p, r);
  if (out) {
    l_int line_height = FONT_GETHEIGHT(o->font)+1;
    l_int ch_width = FONT_GETWIDTH(VIEW(o)->font, 'W');
    l_int line_y = 2;
    t_rect lr;
    l_int pos = BEDITOR(o)->page_start;
    l_text s = get_text_line(BEDITOR(o)->text, BEDITOR(o)->line_pos);
    l_int x1;
    l_char cur[2];
    l_text status;

    rectfill(out, p.x+r.a.x, p.y+r.a.y, p.x+r.b.x, p.y+r.b.y, o->get_color(o, 0));
    status = set_format_text(NULL, "   -=[%i:%i]=-  (%i)", BEDITOR(o)->y+BEDITOR(o)->py, BEDITOR(o)->x+BEDITOR(o)->px, BEDITOR(o)->line_pos+BEDITOR(o)->x-1, BEDITOR(o)->text[BEDITOR(o)->line_pos+BEDITOR(o)->x-1]);
    rectfill(out, p.x+r.a.x+2, p.y+r.a.y+2, p.x+r.b.x-2, p.y+r.a.y+1+line_height, VIEW(o)->get_color(VIEW(o), 5));
    textout_draw_rect(out, o->font, status, -1, p.x+r.a.x+2, p.y+r.a.y+2, p.x+r.b.x-2, p.y+r.b.y-2, TX_ALIGN_LEFT|TX_ALIGN_TOP, VIEW(o)->get_color(VIEW(o), 0), TX_NOCOLOR, 1);
    free(status);

    line_y += line_height-(BEDITOR(o)->py*line_height);

    do {
      l_text txt = get_text_line(BEDITOR(o)->text, pos);

      lr = rect_assign(r.a.x+p.x+2-BEDITOR(o)->px*ch_width, line_y+r.a.y+p.y, p.x+r.b.x-2, line_y+p.y+line_height-1);
      if (line_y >= line_height)
      BEDITOR(o)->draw_line(BEDITOR(o), out, lr, txt);
      line_y += line_height;

      pos += strlen(txt)+1;

      free(txt);
    } while (pos < strlen(BEDITOR(o)->text) && line_y < r.b.y);

    x1 = strlen(s);
    s[BEDITOR(o)->x+BEDITOR(o)->px] = 0;
    cur[0] = s[strlen(s)-1];
    if (BEDITOR(o)->x+BEDITOR(o)->px > x1) cur[0] = ' ';
    cur[1] = 0;
    s[BEDITOR(o)->x+BEDITOR(o)->px-1] = 0;
    x1 = p.x+r.a.x+2+FONT_GETSTRWIDTH(o->font, s, -1);
    line_y = line_height*BEDITOR(o)->y;

    textout_draw(out, o->font, cur, -1, x1-BEDITOR(o)->px*ch_width, p.y+r.a.y+2+line_y, TX_ALIGN_LEFT, o->get_color(o, 0), o->get_color(o, 5));

    free(s);

    button(out, p.x+r.a.x, p.y+r.a.y, p.x+r.b.x, p.y+r.b.y, o->get_color(o, 2), o->get_color(o, 1));
    button(out, p.x+r.a.x+1, p.y+r.a.y+1, p.x+r.b.x-1, p.y+r.b.y-1, o->get_color(o, 3), o->get_color(o, 4));
  };

  o->end_of_paint(o, r);
}

static void beditor_translate_event(p_object o, t_event *event)
{
  RETVIEW(o, event);
  view_translate_event(o, event);

  if (event->type & EV_KEYBOARD) {
    p_beditor bo = BEDITOR(o);
    l_text s = get_text_line(bo->text, bo->line_pos);
    l_int line_height = FONT_GETHEIGHT(VIEW(o)->font)+1;
    l_int ch_width = FONT_GETWIDTH(VIEW(o)->font, 'W');

    if (OBJECT(keyb)->state & KB_SF_KEYDOWN) {
      switch (KEY_TO(keyb->code)) {
        case KB_RIGHT: if (bo->x+bo->px < bo->line_len) {
          if (bo->x < rect_sizex(VIEW(o)->bounds)/ch_width) bo->x++; else bo->px++;
          bo->sel_start++;
          VIEW(o)->draw(VIEW(o));
          clear_event(event);
        }; break;
        case KB_LEFT: if (bo->x+bo->px > 1) {
          if (bo->x > 1) bo->x--; else bo->px--;
          bo->sel_start--;
          VIEW(o)->draw(VIEW(o));
          clear_event(event);
        }; break;
        case KB_DOWN: if (bo->y+bo->py < bo->lines) {
          l_text line;

          if (bo->y+2 < rect_sizey(VIEW(o)->bounds)/line_height) bo->y++; else bo->py++;
          bo->sel_start += bo->line_len;
          bo->line_pos = bo->sel_start-(bo->x+bo->px)+1;
          line = get_text_line(bo->text, bo->line_pos);
          bo->line_len = strlen(line)+1;
          free(line);
          if (bo->x > bo->line_len) {
/*            bo->x = bo->line_len;
            if (bo->x > VIEW(o)->bounds.b.x/ch_width) bo->px = bo->x-VIEW(o)->bounds.b.x/ch_width;
            bo->x -= bo->px;*/
            bo->x = 1;
            bo->px = 0;
          };
          bo->sel_start = bo->line_pos+(bo->x+bo->px)-1;

          VIEW(o)->draw(VIEW(o));
          clear_event(event);
        }; break;
        case KB_UP: if (bo->y+bo->py > 1) {
          l_text line;
          if (bo->y > 1) bo->y--; else bo->py--;
          bo->line_pos--;
          if (bo->text[bo->line_pos-1] != '\n' && bo->line_pos > 1) {
            do {
              bo->line_pos--;
            } while (bo->text[bo->line_pos] != '\n' && bo->line_pos > 0);
            if (bo->line_pos > 0) bo->line_pos++;
          };
          line = get_text_line(bo->text, bo->line_pos);
          bo->line_len = strlen(line)+1;
          free(line);
          if (bo->x > bo->line_len) {
            bo->x = 1;
            bo->px = 0;
          };
          bo->sel_start = bo->line_pos+(bo->x+bo->px)-1;
          VIEW(o)->draw(VIEW(o));
          clear_event(event);
        }; break;
        case KB_HOME: {
          bo->x = 1;
          bo->px = 0;
          bo->sel_start = bo->line_pos;
          VIEW(o)->draw(VIEW(o));
          clear_event(event);
        }; break;
        case KB_END: {
          bo->x = bo->line_len;
          if (bo->x > rect_sizex(VIEW(o)->bounds)/ch_width) bo->px = bo->x-rect_sizex(VIEW(o)->bounds)/ch_width;
          bo->x -= bo->px;
          bo->sel_start = bo->line_pos+(bo->x+bo->px)-1;
          VIEW(o)->draw(VIEW(o));
          clear_event(event);
        }; break;
        case KB_BACKSPACE: {
          l_text line;

          if (bo->sel_start > 0) {
            bo->modified = TRUE;
            bo->sel_start--;
            if (bo->x+bo->px > 1) {
              bo->text = delstr(bo->text, bo->sel_start, 1);
              bo->line_len--;
              if (bo->x > 1) bo->x--; else bo->px--;
            } else {
              l_int old_line_pos = bo->line_pos;
              /* place cursor x */
              bo->line_pos--;
              if (bo->text[bo->line_pos-1] != '\n' && bo->line_pos > 1) {
                do {
                  bo->line_pos--;
                } while (bo->text[bo->line_pos] != '\n' && bo->line_pos > 0);
                if (bo->line_pos > 0) bo->line_pos++;
              };
              line = get_text_line(bo->text, bo->line_pos);
              bo->line_len = strlen(line)+1;
              free(line);
              bo->x = bo->line_len;
              /* delete CR char ('\n') */
              bo->text = delstr(bo->text, bo->sel_start, 1);
              bo->line_pos = old_line_pos-1;
              if (bo->text[bo->line_pos-1] != '\n' && bo->line_pos > 1) {
                do {
                  bo->line_pos--;
                } while (bo->text[bo->line_pos] != '\n' && bo->line_pos > 0);
                if (bo->line_pos > 0) bo->line_pos++;
              };
              line = get_text_line(bo->text, bo->line_pos);
              bo->line_len = strlen(line)+1;
              free(line);
              bo->sel_start = bo->line_pos+(bo->x+bo->px)-1;
              if (bo->y > 1) bo->y--; else bo->py--;
              bo->lines--;
            };
            VIEW(o)->draw(VIEW(o));
          };
          clear_event(event);
        }; break;
      };
      if ((l_byte)TO_CHAR(keyb->code) >= 32 && (l_byte)TO_CHAR(keyb->code) < 255) {
        l_text sta = (l_text) malloc(2);
        sta[0] = TO_CHAR(keyb->code);
        sta[1] = 0;
        bo->text = insstr(bo->text, sta, bo->sel_start, 1);
        free(sta);
        bo->modified = TRUE;
        bo->line_len++;
        bo->sel_start++;
        if (bo->x < rect_sizex(VIEW(o)->bounds)/ch_width) bo->x++; else bo->px++;
        VIEW(o)->draw(VIEW(o));
        clear_event(event);
      };
      if ((l_byte)TO_CHAR(keyb->code) == 13) {
        l_text line;

        bo->text = insstr(bo->text, "\n", bo->sel_start, 1);
        bo->modified = TRUE;
        bo->line_pos = bo->sel_start+1;
        line = get_text_line(bo->text, bo->line_pos);
        bo->line_len = strlen(line)+1;
        free(line);
        bo->sel_start++;
        bo->x = 1;
        bo->px = 0;
        if (bo->y+2 < rect_sizex(VIEW(o)->bounds)/line_height) bo->y++; else bo->py++;
        bo->lines++;
        VIEW(o)->draw(VIEW(o));
        clear_event(event);
      };
    };

    free(s);
  };

}

p_beditor _beditor_init(p_beditor o, t_rect r, l_int flags)
{
  if (!o) return NULL;
  clear_type(o, sizeof(t_beditor));
  view_init(&o->obclass, r);

  o->set_text = &beditor_set_text;
  o->draw_line = &beditor_draw_line;

  VIEW(o)->draw = &beditor_draw;
  VIEW(o)->set_palette(VIEW(o), pal_beditor);
  VIEW(o)->brush.color = VIEW(o)->get_color(VIEW(o), 0);
  VIEW(o)->font = get_font_in_size("EditorFont", -1, -1);

  OBJECT(o)->translate_event = &beditor_translate_event;

  o->flags = flags;
//  o->text = strdup("test1\nline with test\nBad Editor (C) 2001 Bad Sector\nyeap! It's mine\n");
  o->text = strdup("");
  o->modified = FALSE;
  o->line_pos = 0;
  o->line_len = strlen(get_text_line(o->text, 0))+1;
  o->lines = 1;
  o->page_start = 0;
  o->sel_start = 0;
  o->sel_len = 0;
  o->x = 1;
  o->y = 1;
  o->px = 0;
  o->py = 0;

  return o;
}

/*
#include "beditor.exp"

lib_begin(void)
{
  if (ap_process == AP_ALLOC)
  {
    AP_EXPORTLIB();
  } else
  if (ap_process == AP_INIT)
  {
    AP_SETNUMOFCALLS(1);

    pal_beditor[0] = registry_color("flat_background");
    pal_beditor[1] = registry_color("3d_light");
    pal_beditor[2] = registry_color("3d_shadow");
    pal_beditor[3] = registry_color("3d_dark");
    pal_beditor[4] = registry_color("3d_flat");
    pal_beditor[5] = registry_color("edit_text");
  };
} lib_end;
*/
