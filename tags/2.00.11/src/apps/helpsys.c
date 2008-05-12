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

/* Revision History:
 *
 * DATE         CHANGE                                         VERSION
 * 06/11/2001   Added <FONT> tag (orudge)                      0.31
 */

#include <seal.h>
#include <iodlg.h>
#include <helpsys.h>
#include <registry.h>


GrFont *old_font;

l_color flat;
l_color text;
l_color face3d;
l_color light3d;
l_color shadow3d;
l_color dark3d;
l_color _link;

p_helpbox (*helpbox_init)(p_helpbox o, t_rect r)=&_helpbox_init;

typedef struct t_topic
{
  char name[51];
  int pos, size;
} t_topic;

typedef struct t_bitmap_info
{
  char name[13];
  int width, height, bpp;
  int pos;
} t_bitmap_info;

typedef struct t_head
{
  char ID[5];
  int version;
  int topics;
  int bitmaps;
} t_head;

static p_list get_file_items()
{
  p_list p = list_init(malloc(sizeof(t_list)), &free_filehistory_item, 0);

  if (p) {
     p->insert(p, new_filehistory_item("Help files (*.hlp)", "*.hlp"));
     p->insert(p, new_filehistory_item("All files (*.*)", "*.*"));
  };

  return p;
}

#ifndef Bad_Seal
void draw_bevel(BITMAP *out, int x1, int y1, int x2, int y2, int c1, int c2)
{
  if (out) {
    line(out, x1, y1, x2, y1, c1);
    line(out, x1, y1, x1, y2, c1);
    line(out, x2, y1, x2, y2, c2);
    line(out, x1, y2, x2, y2, c2);
  }
}
#else
void draw_bevel(BITMAP *out, int x1, int y1, int x2, int y2, int c1, int c2);
/* this function is exported from Bad Controls */
#endif

/* begin of draw_string */
#define draw_string \
  s[sc] = 0; \
  xx = x+FONT_GETSTRWIDTH(VIEW(o)->font, s, -1); \
  if (xx >= mx-5) { \
    xx = 5+FONT_GETSTRWIDTH(VIEW(o)->font, s, -1); \
    if (o->u_link) o->link[o->links].x2 = x; \
    x = 5; \
    y += FONT_GETHEIGHT(VIEW(o)->font); \
    if (o->u_link) { \
      o->links++; \
      o->link[o->links].where = strdup(o->link[o->links-1].where); \
      o->link[o->links].target = strdup(o->link[o->links-1].target); \
      o->link[o->links].x1 = 5; \
      o->link[o->links].y1 = y; \
      o->link[o->links].x2 = 6; \
      o->link[o->links].y2 = y+FONT_GETHEIGHT(VIEW(o)->font); \
    }; \
  }; \
  if (y > 0) { \
    l_int align = TX_ALIGN_LEFT; \
    l_color color = text; \
    if (o->u_link) { \
      align |= TX_UNDERLINE; \
      color = _link; \
    }; \
    textout_draw(d, VIEW(o)->font, s, -1, x, y, align, color, -2 /*was TX_NOCOLOR*/); \
    if (o->u_bold) \
      textout_draw_rect(d, VIEW(o)->font, s, -1, x+1, y, x+1, y, align, color, -2, FALSE); \
  }; \
  x = xx; if (o->u_bold) x++;\
  s[0] = 0; \
  sc = 0;
/* end of draw_string */

#define free_links(o) { \
  int i; \
  for (i=0;i<o->links;i++) { \
    if (o->link[i].where) afree(&o->link[i].where); \
    if (o->link[i].target) afree(&o->link[i].target); \
  }; \
  o->links = 0; \
}


p_helpbox base_helpbox(p_helpbox o)
{
  if (!o->f_in_split) return o;
  do {
    o = HELPBOX(OBJECT(o)->owner);
  } while (o->f_in_split);
  return o;
}

p_helpbox get_split(p_helpbox o, l_text name)
{
  __p_hbsplit p;
  p_helpbox fo = o;
  if (!o->f_in_split) return o;
  o = base_helpbox(o);
  p = o->splits;
  do {
    if (!stricmp(p->name, name)) return p->split;
    p = p->next;
  } while (p);
  return fo;
}

void helpbox_parse_cmd(p_helpbox o, BITMAP *d, t_rect r, t_point p, l_text s, l_int *scp, l_text cmd)
{
  l_int i, x = o->f_x, y = o->f_y, mx = o->f_mx, my = o->f_my, sc = *scp;
  l_char par[16][256];
  l_int pars = 0;
  l_int argc = 0;
  l_bool u_str = FALSE;

  for (i=0;i<16;i++) par[i][0] = 0;
  for (i=0;i<strlen(cmd);i++) {
    switch (cmd[i]) {
      case ' ': if (!u_str) {
        if (argc > 0) {
          par[pars][argc] = 0;
          pars++;
          argc = 0;
        };
      } else {
        par[pars][argc] = cmd[i];
        argc++;
      }; break;
      case '"': {
        u_str = !u_str;
      }; break;
      default: {
        par[pars][argc] = cmd[i];
        argc++;
      }; break;
    };
  };
  if (argc > 0) par[pars][argc] = 0;

  if (!stricmp(par[0], "BR")) {
    l_int xx;
    draw_string;
    if (o->u_link) o->link[o->links].x2 = x;
    x = 5;
    y += FONT_GETHEIGHT(VIEW(o)->font);
    if (o->u_link) {
      o->links++;
      o->link[o->links].where = strdup(o->link[o->links-1].where);
      o->link[o->links].target = strdup(o->link[o->links-1].target);
      o->link[o->links].x1 = 5;
      o->link[o->links].y1 = y;
      o->link[o->links].x2 = 6; /* will changed by <LE> */
      o->link[o->links].y2 = y+FONT_GETHEIGHT(VIEW(o)->font);
    };
  };
  if (!stricmp(par[0], "L")) {
    l_int xx;
    draw_string;
    if (!o->u_link) o->links++;
    if (pars) o->link[o->links].where = strdup(par[1]);
    if (pars > 1) {
      if (o->f_in_split)
        o->link[o->links].target = strdup(par[2]);
      else
        o->link[o->links].target = NULL;
    } else o->link[o->links].target = NULL;
    o->link[o->links].x1 = x;
    o->link[o->links].y1 = y;
    o->link[o->links].x2 = x+1; /* will changed by <LE> */
    o->link[o->links].y2 = y+FONT_GETHEIGHT(VIEW(o)->font);
    o->u_link = TRUE;
  };
  if (!stricmp(par[0], "LE") && o->u_link) {
    l_int xx;
    draw_string;
    o->link[o->links].x2 = x;
    if (pars) o->link[o->links].name = strdup(par[1]); else o->link[o->links].name = NULL;
    o->u_link = FALSE;
  };
  if (!stricmp(par[0], "B")) {
    l_int xx;
    draw_string;
    o->u_bold = TRUE;
  };
  if (!stricmp(par[0], "/B")) {
    l_int xx;
    draw_string;
    o->u_bold = FALSE;
  };
  if (!stricmp(par[0], "HL")) {
    l_int xx;
    draw_string;
    if (o->u_link) o->link[o->links].x2 = x;
    x = 5;
    y += FONT_GETHEIGHT(VIEW(o)->font);
    hline(d, 4, y+4, mx-5, COLOR(0));
    hline(d, 4, y+5, mx-5, COLOR(0));
    y += 11;
    if (o->u_link) {
      o->links++;
      o->link[o->links].where = strdup(o->link[o->links-1].where);
      o->link[o->links].target = strdup(o->link[o->links-1].target);
      o->link[o->links].x1 = 5;
      o->link[o->links].y1 = y;
      o->link[o->links].x2 = 6; /* will changed by <LE> */
      o->link[o->links].y2 = y+FONT_GETHEIGHT(VIEW(o)->font);
    };
  };
  if (!stricmp(par[0], "BACKGROUND")) {
    if (o->background) free(o->background);
    o->background = strdup(par[1]);
    if (pars > 1 && !stricmp(par[2], "WATERMARK")) o->flags |= HBF_WATERMARK;
  };
  if (!stricmp(par[0], "IMAGE")) {
    l_int xx;
    BITMAP *bmp;
    draw_string;

    if (par[1]) bmp = o->get_bitmap(o, par[1]); else bmp = NULL;

    if (bmp) {
      draw_sprite(d, bmp, x, y);
      x += bmp->w;
      if (o->u_link)
        o->link[o->links].y2 = y+bmp->h;
      if (!(pars > 1 && !stricmp(par[2], "STAY"))) y += bmp->h-FONT_GETHEIGHT(VIEW(o)->font);
    };
  };
  if (!stricmp(par[0], "SPLIT")) {
    p_helpbox s1, s2, base;
    l_int splitx = atoi(par[1]);

    o->f_split = TRUE;
    if (o->f_in_split)
      s1 = helpbox_init(malloc(sizeof(t_helpbox)), rect_assign(0, 0, splitx, rect_sizey(VIEW(o)->bounds)));
    else
      s1 = helpbox_init(malloc(sizeof(t_helpbox)), rect_assign(2, 2, splitx, rect_sizey(VIEW(o)->bounds)-2));
    s1->f_in_split = TRUE;
    VIEW(s1)->align |= TX_ALIGN_BOTTOM;
    OBJECT(o)->insert(OBJECT(o), OBJECT(s1));
    if (o->f_in_split)
      s2 = helpbox_init(malloc(sizeof(t_helpbox)), rect_assign(splitx+1, 0, rect_sizex(VIEW(o)->bounds), rect_sizey(VIEW(o)->bounds)));
    else
      s2 = helpbox_init(malloc(sizeof(t_helpbox)), rect_assign(splitx+1, 2, rect_sizex(VIEW(o)->bounds)-2, rect_sizey(VIEW(o)->bounds)-2));
    s2->f_in_split = TRUE;
    VIEW(s2)->align |= TX_ALIGN_BOTTOM+TX_ALIGN_RIGHT;
    OBJECT(o)->insert(OBJECT(o), OBJECT(s2));
    s1->filename = strdup(o->filename);
    s2->filename = strdup(o->filename);
    helpbox_navigate(s1, par[2]);
    helpbox_navigate(s2, par[3]);
    OBJECT(s2)->select(OBJECT(s2));
    base = base_helpbox(o);
    base->active_split = s2;
    #define add_split(asplit, aname) { \
      p_helpbox bo = base_helpbox(o); \
      __p_hbsplit s = bo->splits; \
      if (!s) { \
        s = (__p_hbsplit) malloc(sizeof(__t_hbsplit)); \
        strcpy(s->name, aname); \
        s->split = asplit; \
        s->prev = NULL; \
        s->next = NULL; \
        bo->splits = s; \
      } else { \
        if (s->next) do { \
          s = s->next; \
        } while (s->next); \
        s->next = (__p_hbsplit) malloc(sizeof(__t_hbsplit)); \
        strcpy(s->next->name, aname); \
        s->next->split = asplit; \
        s->next->prev = s; \
        s->next->next = NULL; \
      }; \
    }
    add_split(s1, par[2]);
    add_split(s2, par[3]);
  };
  if (!stricmp(par[0], "VSPLIT")) {
    p_helpbox s1, s2, base;
    l_int splity = atoi(par[1]);
    o->f_split = TRUE;
    if (o->f_in_split)
      s1 = helpbox_init(malloc(sizeof(t_helpbox)), rect_assign(0, 0, rect_sizex(VIEW(o)->bounds), splity));
    else
      s1 = helpbox_init(malloc(sizeof(t_helpbox)), rect_assign(2, 2, rect_sizex(VIEW(o)->bounds)-2, splity));
    s1->f_in_split = TRUE;
    VIEW(s1)->align |= TX_ALIGN_RIGHT;
    OBJECT(o)->insert(OBJECT(o), OBJECT(s1));
    if (o->f_in_split)
      s2 = helpbox_init(malloc(sizeof(t_helpbox)), rect_assign(0, splity+1, rect_sizex(VIEW(o)->bounds), rect_sizey(VIEW(o)->bounds)));
    else
      s2 = helpbox_init(malloc(sizeof(t_helpbox)), rect_assign(2, splity+1, rect_sizex(VIEW(o)->bounds)-2, rect_sizey(VIEW(o)->bounds)-2));
    s2->f_in_split = TRUE;
    VIEW(s2)->align |= TX_ALIGN_BOTTOM+TX_ALIGN_RIGHT;
    OBJECT(o)->insert(OBJECT(o), OBJECT(s2));
    s1->filename = strdup(o->filename);
    s2->filename = strdup(o->filename);
    helpbox_navigate(s1, par[2]);
    helpbox_navigate(s2, par[3]);
    OBJECT(s2)->select(OBJECT(s2));
    base = base_helpbox(o);
    base->active_split = s2;
    add_split(s1, par[2]);
    add_split(s2, par[3]);
  };
  if (!stricmp(par[0], "STATIC")) {
    o->flags |= HBF_UNSELECTABLE+HBF_NOSCROLL;
  };
  if (!stricmp(par[0], "SCRIPT")) {
    if (!o->_script) {
      o->_script = (l_text) malloc(1);
      o->_script[0] = 0;
      o->flags |= HBF_RESERVED1;
    };
  };
  if (!stricmp(par[0], "BUTTON")) {
    o->request(o, HBREQ_BUTTON, par[1], NULL, NULL);
  };
  if (!stricmp(par[0], "FONT")) {
    old_font = VIEW(o)->font;
    VIEW(o)->font = get_font_in_size(par[1], atoi(par[2]), atoi(par[2]));
  }
  if (!stricmp(par[0], "/FONT")) {
    VIEW(o)->font = old_font;
  }

  o->f_x = x;
  o->f_y = y;
  *scp = sc;
}

void helpbox_draw_text(p_helpbox o, BITMAP *out, t_rect r, t_point p)
{
  l_int i, x = 5, y = 5-o->f_prg;
  BITMAP *d = create_bitmap(rect_sizex(r)-3, rect_sizey(r)-3);
  l_int mx = rect_sizex(r)-3, my = rect_sizey(r)-3;
  l_char s[1024];
  l_char cmd[1024];
  l_int sc = 0, cmdc = 0;
  l_bool u_tag = FALSE;
  l_bool u_str = FALSE;

  o->f_mx = mx;
  o->f_my = my;

  if (!o->background) {
    rectfill(d, 0, 0, mx, my, flat);
  } else {
    int x = 0, y, w = d->w, h = d->h;
    BITMAP *bmp = o->get_bitmap(o, o->background);

    if (o->flags & HBF_WATERMARK)
      y = 0;
    else
      y = -(o->f_prg % h);

    do {
      blit(bmp, d, 0, 0, x, y, w, h);
      x += bmp->w;
      if (x >= w) {
        x = 0;
        y += bmp->h;
      };
    } while (y < h);
  };

  s[0]=0;
  o->links = -1;
  for (i=0;i<strlen(o->text);i++) {
    switch (o->text[i]) {
      case '\n': break;
      case ' ': if (!u_tag) {
        l_int xx;
        s[sc] = 32;
        sc++;
        draw_string;
      } else {
        cmd[cmdc] = o->text[i];
        cmdc++;
      }; break;
      case '<': if (!u_str && o->text[i+1] != '<') {
        u_tag = TRUE;
        cmd[0] = 0;
        cmdc = 0;
      } else {
        if (o->text[i+1] == '<' && !u_str) i++;
        if (u_tag) {
          cmd[cmdc] = o->text[i];
          cmdc++;
        } else {
          s[sc] = o->text[i];
          sc++;
        };
      }; break;
      case '>': if (!u_str && u_tag) {
        u_tag = FALSE;
        cmd[cmdc] = 0;
        o->f_x = x;
        o->f_y = y;
        if (cmdc != 0) helpbox_parse_cmd(o, d, r, p, s, &sc, cmd);
        x = o->f_x;
        y = o->f_y;
      } else {
        if (u_tag) {
          cmd[cmdc] = o->text[i];
          cmdc++;
        } else {
          s[sc] = o->text[i];
          sc++;
        };
      }; break;
      case '"': if (u_tag) {
        u_str = !u_str;
        s[sc] = '"';
        sc++;
      } else {
        s[sc] = '"';
        sc++;
      }; break;
      default: {
        if (u_tag) {
          cmd[cmdc] = o->text[i];
          cmdc++;
        } else {
          s[sc] = o->text[i];
          sc++;
        };
      } break;
    };
    if (y > my && o->f_maxy > 0) break;
  };
  if (sc != 0) {
    l_int xx;
    draw_string;
  };
  if (o->f_maxy == 0) {
    o->f_maxy = y;
  };

  blit(d, out, 0, 0, r.a.x+p.x+2, r.a.y+p.y+2, mx, my);
  destroy_bitmap(d);
}

void helpbox_draw(p_view o)
{
  t_rect r = VIEW(o)->get_local_extent(VIEW(o));
  t_point p;
  BITMAP *out;

  out = VIEW(o)->begin_paint(VIEW(o), &p, r);
  if (out) {
    l_int i;

    if (!HELPBOX(o)->f_in_split) {
      draw_bevel(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, shadow3d, light3d);
      draw_bevel(out, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x-1, r.b.y+p.y-1, dark3d, face3d);
    } else {
      rect(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, flat);
      rect(out, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x-1, r.b.y+p.y-1, flat);
    };

    if (!HELPBOX(o)->f_split && HELPBOX(o)->f_in_split)
    if (OBJECT(o)->is_state(OBJECT(o), OB_SF_FOCUSED))
      rect(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, dark3d);

    if (!HELPBOX(o)->f_split) {
      l_text old_back = HELPBOX(o)->background;
      helpbox_draw_text(HELPBOX(o), out, r, p);
      if (old_back != HELPBOX(o)->background)
        helpbox_draw_text(HELPBOX(o), out, r, p);
    };
  };
  VIEW(o)->end_of_paint(VIEW(o), r);
}

void helpbox_set_state(p_object o, l_dword st, l_bool set)
{
  view_set_state(o, st, set);
  if (st & OB_SF_FOCUSED) {
    VIEW(o)->draw_view(VIEW(o));
  };
}

void helpbox_select_me(p_helpbox o)
{
  OBJECT(o)->select(OBJECT(o));
  if (o->f_in_split)
    helpbox_select_me(HELPBOX(OBJECT(o)->owner));
}

void helpbox_handle_event(p_object o, t_event *event)
{
  view_translate_event(o, event);

  RETVIEW(o, event);

  if (!(HELPBOX(o)->flags & HBF_NOSCROLL))
  if (event->type & EV_KEYBOARD && o->is_state(o, OB_SF_FOCUSED)) {
    if (OBJECT(keyb)->state & KB_SF_KEYDOWN ) {
      switch (KEY_TO(keyb->code)) {
        case KB_DOWN: if (HELPBOX(o)->f_prg < (HELPBOX(o)->f_maxy-rect_sizey(VIEW(o)->bounds)+35)) {
          HELPBOX(o)->f_prg += 30;
          VIEW(o)->draw_view(VIEW(o));
          clear_event(event);
        }; break;
        case KB_UP: if (HELPBOX(o)->f_prg > 0) {
          HELPBOX(o)->f_prg -= 30;
          VIEW(o)->draw_view(VIEW(o));
          clear_event(event);
        }; break;
      };
    };
  };

  if (!(HELPBOX(o)->flags & HBF_UNSELECTABLE))
  if ((event->type & EV_MOUSE) && OBJECT(mouse)->state & MO_SF_MOUSELPRESS) {
    p_helpbox base = base_helpbox(HELPBOX(o));
    helpbox_select_me(HELPBOX(o));
    base->active_split = HELPBOX(o);
  };

  if ((event->type & EV_MOUSE) && (OBJECT(mouse)->state & MO_SF_MOUSEMOVE || OBJECT(mouse)->state & MO_SF_MOUSELPRESS)) {
    l_int i;
    t_point p = VIEW(o)->get_global_point(VIEW(o), point_assign(0, 0));
    t_rect r = VIEW(o)->get_local_extent(VIEW(o));
    p_helpbox ho = HELPBOX(o);
    l_int cur = CUR_ARROW;
    l_int lnk = -1;
    l_bool mouse_pressed = FALSE;

    for (i=0;i<=ho->links;i++)
      if (mouse->where.x >= ho->link[i].x1+r.a.x+p.x && mouse->where.y >= ho->link[i].y1+r.a.y+p.y &&
          mouse->where.x <= ho->link[i].x2+r.a.x+p.x && mouse->where.y <= ho->link[i].y2+r.a.y+p.y) {
        cur = CUR_FINGER;
        lnk = i;

        break;
      };

    if (VIEW(o)->cursor != cur) {
      VIEW(o)->cursor = cur;
      VIEW(o)->set_mouse_cursor(VIEW(o));
    };

    mouse_pressed = (OBJECT(mouse)->state & MO_SF_MOUSELPRESS);

    while (OBJECT(mouse)->state & MO_SF_MOUSELPRESS) {
      o->get_event(o, event);
    };
    clear_event(event);

    if (cur == CUR_FINGER && mouse_pressed) {
      p_helpbox target = HELPBOX(o);
      if (HELPBOX(o)->f_in_split && HELPBOX(o)->link[lnk].target)
        target = get_split(HELPBOX(o), HELPBOX(o)->link[lnk].target);
      target->navigate(target, HELPBOX(o)->link[lnk].where);
    };
  };
}

void helpbox_navigate(p_helpbox o, l_text where)
{
  __p_hbgbinfo tmp = o->f_back;
  if (!where) return;

  #define clear_bitmaps \
    if (o->bitmaps) do { \
      __p_hbbitmap next = o->bitmaps->next; \
      destroy_bitmap(o->bitmaps->bmp); \
      free(o->bitmaps->name); \
      free(o->bitmaps); \
      o->bitmaps = next; \
    } while (o->bitmaps)


  if (!stricmp(where, "HELP:ABOUT")) {
    if (o->text) afree(&o->text);
    if (o->topic) afree(&o->topic);
    if (o->background) afree(&o->background);
    if (o->flags & HBF_WATERMARK) o->flags &=~HBF_WATERMARK;
    clear_bitmaps;
    free_links(o);
    o->topic = strdup("help:about");

    o->text = strdup("<b>Bad Seal Help System version 0.2</b><br><br>
  HELP (Bad Seal Help System), was made at 20.05.2001 by Kostas Michalopoulos
 aka Bad Sector, mostly for use under Bad Seal GUI. But you can find HELP
 also for SEAL 1.0E and other compatible versions and distributions (like
 StarSeal).<br><br>
  For more information on how to write your own HELP (*.HLP) files, see
 HELP's documentation.<br><br>
  You can contact me at:<br>
    e-mail:<br>
      badsector@badseal.org<br>
    www:<br>
      (SEAL/Bad Seal resources page)<br>
      http://www.badseal.org/<br>
<br>
 Last Update: " __DATE__" " __TIME__ "
");
  } else if (!stricmp(where, "HELP:NONE")) {
    if (o->text) afree(&o->text);
    if (o->topic) afree(&o->topic);
    if (o->background) afree(&o->background);
    if (o->flags & HBF_WATERMARK) o->flags &=~HBF_WATERMARK;
    clear_bitmaps;
    free_links(o);
    o->topic = strdup("help:none");
    o->text = strdup("\n");
  } else if (!stricmp(where, "HELP:OPEN")) {
    l_text file;
    file = open_dialog("/", "*.hlp", get_file_items());
    if (file) {
      if (o->filename) free(o->filename);
      o->filename = file;
      o->navigate(o, "main");
      return;
    };
  } else if (!stricmp(where, "HELP:BACK")) {
    if (!o->f_back || !o->f_back->prev) {
      msgbox(MW_INFO, MB_OK, "Cannot go back");
    } else o->go_back(o);
    return;
  } else if (!o->filename) {
    if (o->text) afree(&o->text);
    if (o->topic) afree(&o->topic);
    if (o->background) afree(&o->background);
    if (o->flags & HBF_WATERMARK) o->flags &=~HBF_WATERMARK;
    clear_bitmaps;
    free_links(o);
    o->text = strdup("<b>Error: topic without file specified");
  } else {
    FILE *f;
    t_topic rec;
    t_head head;
    l_int i, pos;
    l_text s;

    if (o->text) afree(&o->text);
    if (o->topic) afree(&o->topic);
    if (o->background) afree(&o->background);
    if (o->flags & HBF_WATERMARK) o->flags &=~HBF_WATERMARK;
    clear_bitmaps;
    if (!strcmp(o->filename, "")) {
      f = NULL;
    } else {
      f = fopen(o->filename, "rb");
    };

    if (!f) {
      free_links(o);
      o->text = strdup("<b>Error: file not found, cannot open file or file not specified");
    } else {
      l_bool ok = TRUE;
      fread(&head, sizeof(t_head), 1, f);
      if (head.ID[0] != 'B' || head.ID[1] != 'S' || head.ID[2] != 'H' ||
          head.ID[3] != 'L' || head.ID[4] != 'P') {
        free_links(o);
        o->text = strdup("<b>Error: invalid file header or file is damaged");
        ok = FALSE;
      };
      if (head.version > 1) {
        free_links(o);
        o->text = strdup("<b>Error: unknown file version");
        ok = FALSE;
      };
      if (head.topics == 0) {
        free_links(o);
        o->text = strdup("<b>Error: file without topics");
        ok = FALSE;
      };
      if (ok) {
        pos = 0;
        for (i=0;i<head.topics;i++) {
          fread(&rec, sizeof(t_topic), 1, f);
          if (!stricmp(rec.name, where)) {
            pos = rec.pos;
            break;
          };
        };
        if (pos == 0) {
          o->text = malloc(1024);
          strcpy(o->text, "<b>Error: topic not found - '</b>");
          strcat(o->text, where);
          strcat(o->text, "<b>'.</b>");
          ok = FALSE;
        };
      };
      if (ok) {
        fpos_t ps = pos;
        fsetpos(f, &ps);
        s = (l_text) malloc(rec.size+1);
        fread(s, rec.size, 1, f);
        s[rec.size] = 0;
        o->text = s;
        o->topic = strdup(where);
        free_links(o);
/*        o->text = (l_text) malloc(rec.size+1);
        memcpy(o->text, s, rec.size);
        o->text[rec.size]=0;
        free(s);*/
      };
      fclose(f);
    };
  };

  if (!o->text) o->text = strdup("");

  o->f_maxy = 0;
  o->f_prg = 0;

  VIEW(o)->draw_view(VIEW(o));

  if (!o->f_avoid_bi) {
    o->f_back = (__p_hbgbinfo) malloc(sizeof(__t_hbgbinfo));
    o->f_back->where = strdup(where);
    o->f_back->prev = tmp;
  } else {
    o->f_avoid_bi = FALSE;
  };
}

l_bool helpbox_request(p_helpbox o, l_int request, void *data1, void *data2, void **rdata1)
{
  return TRUE;
}

void helpbox_go_back(p_helpbox o)
{
  __p_hbgbinfo tmp = o->f_back;

  if (o->f_split && o->active_split) {
    o->active_split->go_back(o->active_split);
    return;
  };

  if (!o->f_back || !o->f_back->prev) {
    return;
  };

  if (o->f_back->prev->where) {
    o->f_avoid_bi = TRUE;
    o->navigate(o, o->f_back->prev->where);
  };

  o->f_back = o->f_back->prev;
  free(tmp);
}

BITMAP *helpbox_get_bitmap(p_helpbox o, l_text name)
{
  FILE *f;
  t_bitmap_info rec;
  t_head head;
  int i, ii;
  BITMAP *bmp;
  int pal[256];

  __p_hbbitmap p = o->bitmaps, m;

  STANDARD_GET_BITMAP_SEARCH;

  if (!o->filename) return NULL;

  f = fopen(o->filename, "rb");
  fread(&head, sizeof(t_head), 1, f);
  if (head.bitmaps == 0 || head.version == 0) return NULL;
  for (i=0;i<head.bitmaps;i++) {
    fseek(f, sizeof(head)+head.topics*sizeof(t_topic)+i*sizeof(t_bitmap_info), SEEK_SET);
    fread(&rec, sizeof(rec), 1, f);
    if (!stricmp(rec.name, name)) {
      bmp = create_bitmap(rec.width, rec.height);

      fseek(f, rec.pos, SEEK_SET);
      for (ii=0;ii<256;ii++) {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        fread(&r, 1, 1, f);
        fread(&g, 1, 1, f);
        fread(&b, 1, 1, f);
        pal[ii] = makecol(r, g, b);
      };
      for (ii=0;ii<rec.height;ii++) {
        int x;
        unsigned char col;

        for (x=0;x<rec.width;x++) {
          fread(&col, 1, 1, f);
          putpixel(bmp, x, ii, pal[(int) col]);
        };
      };

      break;
    };
  };
  fclose(f);

  ADD_BITMAP_TO_BITMAP_LIST(o, bmp, name);

  return bmp;
}

l_bool helpbox_done(p_object o)
{
  p_helpbox ho = HELPBOX(o);
  if (!view_done(o)) return false;

  if (ho->text) free(ho->text);
  if (ho->topic) free(ho->topic);
  if (ho->filename) free(ho->filename);
  if (ho->bitmaps) do {
    __p_hbbitmap next = ho->bitmaps->next;
    destroy_bitmap(ho->bitmaps->bmp);
    free(ho->bitmaps->name);
    free(ho->bitmaps);
    ho->bitmaps = next;
  } while (ho->bitmaps);
  if (ho->f_back) do {
    __p_hbgbinfo prev = ho->f_back->prev;
    if (ho->f_back->where) free(ho->f_back->where);
    free(ho->f_back);
    ho->f_back = prev;
  } while (ho->f_back);
  if (ho->background) free(ho->background);
  free_links(ho);

  return true;
}

p_helpbox _helpbox_init(p_helpbox o, t_rect r)
{
  l_int i;
  if (!o) return NULL;
  clear_type(o, sizeof(t_helpbox));
  view_init(&o->obclass, r);

  OBJECT(o)->translate_event = &helpbox_handle_event;
  OBJECT(o)->set_state = &helpbox_set_state;
  OBJECT(o)->done = &helpbox_done;

  VIEW(o)->draw = &helpbox_draw;

  o->navigate = &helpbox_navigate;
  o->go_back = &helpbox_go_back;
  o->get_bitmap = &helpbox_get_bitmap;

  o->request = &helpbox_request;

  o->text = strdup("Bad Seal help system<br>
No file loaded<br><br><br>
Click <l help:open>here<le> to open a help file.<br><br>
Click <l help:about popup>here<le> for more info about Help System.
");
  o->bitmaps = NULL;
  o->filename = NULL;
  o->topic = NULL;
  o->background = NULL;

  o->f_prg = 0;
  o->f_maxy = 0;
  o->f_back = NULL;
  o->f_avoid_bi = FALSE;
  o->f_split = FALSE;
  o->f_in_split = FALSE;
  o->active_split = NULL;
  o->splits = NULL;

  return o;
}

void open_help(l_text filename)
{
  /* i can also run HELP.EXE with only filename specified, but this
   will also check if filename is in ./help if not in current directory. */
  open_help_topic(filename, "main");
}

void open_help_topic(l_text filename, l_text topic)
{
  FILE *f = fopen(filename, "rb");
  l_text fn = (l_text) malloc(strlen(filename)+16);
  l_text args = (l_text) malloc(strlen(filename)+strlen(topic)+2);

  if (!f) {
    strcpy(fn, "./help/");
    strcat(fn, filename);
    f = fopen(fn, "rb");
    if (f) filename = fn; else {
      free(fn);
      free(args);
      msgbox(MW_ERROR, MB_OK, "Cannot find help file - %s", filename);
      return;
    };
  };
  fclose(f);

  strcpy(args, filename);
  strcat(args, " ");
  strcat(args, topic);
  if (!run_file_args(HELP_EXE, args)) {
    msgbox(MW_ERROR, MB_OK, "Help System error: cannot find help viewer");
  };

  free(args);
  free(fn);
}

#include "helpsys.exp"

#ifdef SEAL_2
SetInfoAppName("HELP");
SetInfoDesciption("SEAL2 Help System");
SetInfoCopyright("(c) 2000-2001 Kostas Michalopoulos aka Bad Sector");
SetInfoManufacturer("Bad Sector");
#endif

lib_begin(void)
{
  if (ap_process == AP_ALLOC)
  {
    AP_EXPORTLIB();
  } else
  if (ap_process == AP_INIT)
  {
    AP_SETNUMOFCALLS(1);

    #ifdef Bad_Seal
    flat = registry_color("flat_background");
    text = registry_color("edit_text");
    face3d = registry_color("3d_face");
    light3d = registry_color("3d_light");
    shadow3d = registry_color("3d_shadow");
    dark3d = registry_color("3d_dark");
    if (key_exists("current/seal/colors/link")) _link = registry_color("link"); else _link = COLOR(9);
    #else
    #ifdef SEAL_2  // means that SEAL2 is used
    flat = color_flat_face;
    text = color_3d_text;
    face3d = color_3d_face;
    light3d = color_3d_light;
    shadow3d = color_3d_shadow;
    dark3d = color_3d_dark;
    _link = COLOR(9);
    #else
    flat = color_get_from_ini("text_input_background");
    text = color_get_from_ini("text");
    face3d = color_get_from_ini("3D_background");
    light3d = COLOR(15);
    shadow3d = COLOR(0);
    dark3d = COLOR(0);
    _link = COLOR(9);
    #endif
    #endif
  };
} lib_end;

