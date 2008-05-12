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
#include <dialogs.h>
#include <editor.h>

p_editor  (*editor_init) ( p_editor o, t_rect r, l_int flags ) = &_editor_init;

l_bool editor_done ( p_object o ) {

  //afree(&EDITOR(o)->text);

  return scroller_done(o);
};

void set_edited ( p_editor o ) {
  if ( !(o->flags & EF_HAVEBEENEDITED) ) o->flags |= EF_HAVEBEENEDITED;
};
void set_notedited ( p_editor o ) {
  if ( o->flags & EF_HAVEBEENEDITED ) o->flags &= ~EF_HAVEBEENEDITED;
};

/*

  Get data from editor, if editor is selected from line1,pos1 to line2,pos2
  it return this selected text if rec->style comes as DS_SELECTED. If line1,
  pos1 and line2,pos2 are same = there is no selected area, it store all text
  to rec->data.

  This function return DAT_TEXT id if some of styles are supported.

  Styles supported by textline :

         DS_SELECTED - rec->data is set to selected text, if some is,
                       else set all text
         DS_ALL      - set rec->data to all text
         DS_WHATEVER - set rec->data to text, which textline want to send
                       [ if some selected it return this, else return all,
                         same as DS_SELECTED in this case ]

*/

l_bool editor_get_data ( p_object o, t_data *rec )
{
  if ( rec ) {

     rec->info_obj = o;
     l_tag_cpy(rec->id, DAT_TEXT);

     switch ( rec->style ) {

       case DS_SELECTED : {

          p_editor ed = EDITOR(o);

          if ( ed_is_sel(ed) ) {

            l_text startt = ed->posptr(ed, ed->sel_from.a.y, ed->sel_from.a.x);
            l_text endt =   ed->posptr(ed, ed->sel_from.b.y, ed->sel_from.b.x);

            rec->data = (void *)stridup(startt, strsize(startt, endt));

          } else

            rec->data = (void *)_strdup(EDITOR(o)->text);

          return true;

       }; break;

       case DS_ALL : {

          rec->data = (void *)_strdup(EDITOR(o)->text);

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

l_bool editor_drag_where ( p_view o, t_data *rec, t_point where )
{
   /*
      if mouse is under this object in drag_data function &&
      rec->id == OBJECT(o)->data_type then redraw cursor
   */
   if ( view_drag_where(o, rec, where) ) {

       t_point  m = EDITOR(o)->cursor_mouse_pos(EDITOR(o), where);

       if ( !EDITOR(o)->cursor_visible )

            EDITOR(o)->show_cursor(EDITOR(o), true);

       if ( m.x != EDITOR(o)->line_pos || m.y != EDITOR(o)->line )

            EDITOR(o)->redraw_text(EDITOR(o), m.y, m.x, 0);

       return true;

   } else /* Out of place || old */

   if ( OBJECT(o)->is_state(OBJECT(o), OB_SF_FOCUSED) ) {

       if ( rec ) EDITOR(o)->show_cursor(EDITOR(o), false);

       else  /* give things to right place, I'm focused so my cursor must be showed */

          EDITOR(o)->show_cursor(EDITOR(o), true);

   } else

     EDITOR(o)->show_cursor(EDITOR(o), false);

   return false;

};


/*
   set_data

   - set data to editor in these cases :

     o   editor is writeable [ ed_is_wable(o) ],
     o   rec->id is one of editor->data_type(s) ( standard is DAT_TEXT )
     o   rec->info_obj != this object, it means data are not from this object
*/

l_bool editor_set_data ( p_object o, t_data *rec )
{
  if ( rec && ed_is_wable(EDITOR(o)) &&

       l_tag_cmp(o->data_type, rec->id) ) {

     p_editor ed  = EDITOR(o);
     l_bool   was_sel = false;
     l_bool   ok = true;
     l_text   ltxt = ed->posptr(ed, ed->line, ed->line_pos);
     l_long   pos = strsize(ed->text, ltxt);

     if ( rec->style & DS_DELETE ) { /* if data is deleted */

       if ( rec->style & DS_ALL ) {

         return ed->set_text(ed, NULL);  /* delete all text */

       } else {

         if ( ed_is_sel(ed) ) { /* delete only selected text */

            ed->redraw_text(ed, ed->line, ed->line_pos, TO_KEY(KB_DEL));

            ok = true;

         };

         ok = false;
       };


     } else { /* if data is inserted to */

       if ( ed_is_sel(ed) ) { // if I'm inserted text, at the first I will delete selected one

          ed->redraw_text(ed, ed->line, ed->line_pos, TO_KEY(KB_DEL));

       };

       ok = ed->insstr(ed, pos, (l_text)rec->data);

     };

     ed_sel_clear(ed);

     if ( !was_sel || ok ) {

       ed->draw_box(ed);

       ed->draw_cursor(ed, -1, -1);

       draw_scroller_bars(o);

     };

     return ok;
  };

  return false;
};



void  editor_set_state ( p_object o, l_dword st, l_bool set )
{
  view_set_state(o, st, set);

  if ( st & OB_SF_SELECTED ) {

    EDITOR(o)->show_cursor(EDITOR(o), set);

  };

};


void  editor_set_options ( p_object o, l_dword op, l_bool set )
{

  obj_set_options(o, op, set);

  if ( op & OB_OF_ENABLE ) {

    VIEW(o)->draw_view(VIEW(o));

  };

};


t_rect editor_scroll_limits ( p_scroller o )
{

  t_rect r = VIEW(o)->size_limits(VIEW(o));

  return r;

};


void   editor_scroll_size ( p_scroller o, l_long *x, l_long *y )
{
  p_editor ed = EDITOR(o);

  l_long mx = 0;
  l_long my = 0;

  scroller_scroll_size(o, x, y);

  if ( ed->text ) {

      if ( x ) mx = *(x);
      if ( y ) my = *(y);

      o->deltax = FONT_GETWIDTH(VIEW(o)->font, 'w');
      o->deltay = ed->ysize_of_line(ed, 0, 1, NULL);

      if ( x ) *(x) = ed->line_longest + o->deltax;
      if ( y ) *(y) = ed->ysize_of_line(ed, 0, ed->line_num+2, NULL);

      if ( x ) *(x) = lmax(mx, calc_maxscrollx(o, (*x)));
      if ( y ) *(y) = lmax(my, calc_maxscrolly(o, (*y)));

  };


};


void   editor_recalc_positions ( p_scroller o, l_long x, l_long y )
{

  p_editor ed = EDITOR(o);

  l_long l;
  l_long p;

  l = ed->line_from + (y / lmax(1, o->deltay));
  l = lmax(0, lmin(ed->line_num, l));

  p = ed->line_pos_from + (x / lmax(1, o->deltax));

  ed->line_ptr = NULL;
  ed->line_ptr = ed->lnptr(ed, l); /* set new line_ptr */
  ed->line_from = l; /* set new line from where box is displayed */
  ed->line_pos_from = p;

};


/*
  insert string (str) to text-position (pos)

  return true if successfull
*/

l_bool  editor_insstr ( p_editor o, l_long pos, l_text str )
{
  l_text n = insstr(o->text, str, pos, strlen(str));

  if ( str && !n ) {

    seal_error(ERR_INFO, TXT_NOTENOUGHMEMORY); /* write message */

    return false;

  };

  o->text = n;
  o->line_ptr = NULL; /* I must set it to null 'cause lnptr work with o->line_ptr */
  o->line_num = o->lnmax(o, &(o->line_longest)); /* get number of lines */
  o->line_ptr = NULL; /* I must set it to null 'cause lnptr work with o->line_ptr */
  o->line_ptr = o->lnptr(o, o->line_from); /* get new line_ptr */

  set_edited(o);

  return true;
};


/*
  delete string from text-position (pos) and by size (size)

  return true if successfull
*/

l_bool  editor_delstr ( p_editor o, l_long pos, l_long size )
{
  l_text n = delstr(o->text, pos, size);

  o->text = n;
  o->line_ptr = NULL; /* I must set it to null 'cause lnptr work with o->line_ptr */
  o->line_num = o->lnmax(o, &(o->line_longest)); /* get number of lines */
  o->line_ptr = o->lnptr(o, o->line_from); /* get new line_ptr */

  draw_scroller_bars(SCROLLER(o)); /* redraw scroll bars */

  set_edited(o);

  return true;
};


/*
  delete char from line (line) and pos (pos)

  return deleted char
*/

l_int   editor_delchr ( p_editor o, l_long line, l_long pos )
{
  l_long size;
  l_text p = o->lntxtlen(o, line, &size);

  l_char ch = 0;

  if ( p ) {

    l_text oldtext = o->text;
    l_long where = p?(p-o->text)+lmin(size, pos):0;

    ch = o->text[where]; /* get deleted char */

    o->text = delstr(o->text, where, 1);

    o->line_num = o->lnmax(o, &(o->line_longest)); /* get number of lines */

    if ( oldtext != o->text ) { /* if insstr relloc o->text send new address */

      o->line_ptr = NULL; /* I must set it to null 'cause lnptr work with o->line_ptr */
      o->line_ptr = o->lnptr(o, o->line_from); /* get new line_ptr */

    };

    draw_scroller_bars(SCROLLER(o)); /* redraw scroll bars */

    o->draw_line(o, line);

    set_edited(o);

  };

  return (l_int)ch;
};


/*
  insert char (chr) to line (line) and pos (pos)

  return true if char was successfull inserted into the text
*/

l_bool  editor_inschr ( p_editor o, l_char chr, l_long line, l_long pos )
{

  l_long size;
  l_text p;

  if ( !chr ) return false;

  p = o->lntxtlen(o, line, &size);

  if ( p ) {

    l_rect sizex = 0;
    l_text oldtext = o->text;
    l_char v[2] = {0, 0};
    v[0] = chr;

    /* insert new character into text o->text */
    o->text = insstr(o->text, v, p?(p-o->text)+lmin(size, pos):0, 1);

    /* get size of current line */
    sizex = o->xsize_of_line(o, line, -1);

    /* inserted character is EOL */
    if ( chr == '\n' )
         /* find the longest line and number of lines */
         o->line_num = o->lnmax(o, &(o->line_longest)); /* get number of lines */

    else
    /* inserted character is not EOL and size of current line is greater then longest one */
    if ( sizex > o->line_longest  )
            /* set longest line to size of current line */
            o->line_longest = sizex;


    if ( oldtext != o->text ) { /* if insstr relloc o->text send new address */

      o->line_ptr = NULL; /* I must set it to null 'cause lnptr work with o->line_ptr */
      o->line_ptr = o->lnptr(o, o->line_from); /* get new line_ptr */

    };

    draw_scroller_bars(SCROLLER(o)); /* redraw scroll bars */

    /* redraw line */
    o->draw_line(o, line);

    set_edited(o);

    return true;
  };

  return false;
};


/*
  return first position of text from line (line)

  (*len) will contains length of this line
*/

l_text  editor_lntxtlen ( p_editor o, l_long line, l_long *len )
{
  l_text  text  = o->posptr(o, line, 0); /* from line "line" and pos "pos" */

  if ( len ) (*len) = o->sizeto(text, 1, 0);

  return text;
};


/*
     save into (longest) variable, pointer to longest line in text

   - return number of lines
*/

l_long editor_lnmax ( p_editor o, l_long *longest )
{

  l_text t = o->text;
  l_long nlines = 0L; /* number of lines */
  l_long longes = 0L; /* longest linein pixels */

  if ( t ) /* o->text exist, so inc it */

  do {

    l_text old = t;
    l_long l = 0;

    t = strchr(t, EDITOR_CHAR_ENTER); /* find first "enter" in line */

    l = FONT_GETSTRWIDTH(VIEW(o)->font, old, strsize(old, t));

    if ( l > longes ) { /* it's longer that longest for now */

        longes = l;

    };

    nlines++; /* inc max number of lines */

  } while ( t++ );

  if ( longest ) (*longest) = longes;

  return lmax(0, nlines-1);

};


/*
  find pointer to line from editor->text string

  - return pointer to line "line"

  , lines are spaced by dos-enter char = '10' ascii code
*/

l_text  editor_lnptr ( p_editor o, l_long line )
{
  l_text from = o->text;
  l_long i    = 0;

  if ( o->line_ptr && o->line_from <= line ) {

      i = o->line_from;

      from = o->line_ptr;

  };

  if ( from && i < line )

  do {

      from = strchr(from, EDITOR_CHAR_ENTER); /* find end of line = '10' ascii code */

      if ( from ) from++;

  } while ( from && ++i < line );

  return from;

};


/*
   return pointer to text, that start in line (line) and delta position

   from this line (pos). It something like (y, x) position in text.
*/

l_text  editor_posptr ( p_editor o, l_long line, l_long pos )
{
  l_text p = o->lnptr(o, line); /* get pointer of line (line) */

  if ( p ) { /* exists this line */

    l_long len = o->lnlenptr(o, p); /* get length of this line */

    return &(o->text[(p-o->text)+lmin(pos, len)]);

  };

  return NULL;

};


/*
   - return number of chars (bytes) that are from pointer (from),

   to delta line from this pointer (deltaln) and delta pos in this line

   (deltapos). Something like (y, x) from pointer from.
*/

l_long  editor_sizeto ( l_text from, l_long deltaln, l_long deltapos )
{

  if ( from ) {

    l_long size = 0;

    if ( *from )

    while ( *from && deltaln > 0 ) {

         if ( (*from) == EDITOR_CHAR_ENTER ) deltaln--;

         else size++;

         from++;

    };

    if ( !deltaln && *from && deltapos )

      while ( *from && deltapos > 0 ) {

         from++;
         size++;
         deltapos--;

      };

    return size;

  };

  return 0;
};


/*
  - return length of line line.
*/

l_long  editor_lnlen ( p_editor o, l_long line )
{
  l_long  l = 0;
  l_text  p = o->lnptr(o, line);

  if ( p )

     for (;*p && *p != EDITOR_CHAR_ENTER; p++, l++)

         ;

  return l;

};


/*
  - return length of line from ptr.
*/

l_long  editor_lnlenptr ( p_editor o, l_text ptr )
{
  l_long  l = 0;
  l_text  p = ptr;

  if ( p )

     for (;*p && *p != EDITOR_CHAR_ENTER; p++, l++)

         ;

  return l;

};


/*

   - return size of line (line) in pixels to max chars (max).

*/
l_long  editor_xsize_of_line ( p_editor o, l_long line, l_long max )
{

   if ( line ) {

         l_long  lnlen = 0;
         l_text  text = o->lntxtlen(o, line, &lnlen);
         l_long  fw = 0;

         if ( lnlen > 0 ) {

               l_long size = lnlen;

               if ( max < 0 ) max = size;

               fw = FONT_GETSTRWIDTH(VIEW(o)->font, (l_byte*)text, lmin(max, size)); /* width of line */

         } else fw = FONT_GETSTRWIDTH(VIEW(o)->font, (l_byte*)text, lmin(lnlen, (max<0?lnlen:max))); /* width of line */

         return fw;
   };

   return 0;
};


/*

   - return size of lines (from line (line)) in pixels.

   line   - pos of line
   h      - number of lines we want to get their height
   p      - pointer to integer where we want to get info about y
            position from line (line) to (delta)+(line).

*/
l_long  editor_ysize_of_line ( p_editor o, l_long line, l_long h, l_long (*p) )
{

  l_long fh = 0;

  if ( p ) (*p) = 0;

  if ( line < 0 || line > o->line_num )

      return 0;

  fh = FONT_GETSTRHEIGHT(VIEW(o)->font)+EDITOR_LINE_SIZE; /* height of line */

  if ( p ) (*p) = (line*fh);

  return fh * h;
};


t_rect editor_size_limits ( p_view o )
{

  return rect_assign(2, 2, rect_sizex(o->bounds)-2, rect_sizey(o->bounds)-2);

};


void   editor_draw ( p_view o )
{

  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  l_long  dy = EDITOR(o)->ysize_of_line(EDITOR(o), 0, 1, NULL);

  SCROLLER(o)->deltax = FONT_GETWIDTH(VIEW(o)->font, 'w');
  SCROLLER(o)->deltay = dy;

  if ( out ) {

    button3d(o,out,p.x+r.a.x, p.y+r.a.y, p.x+r.b.x, p.y+r.b.y,1);

    EDITOR(o)->draw_box(EDITOR(o));

  };

  o->end_of_paint(o, r);

};


/*
  - redraw line (line) in editor
*/

void  editor_draw_line ( p_editor o, l_long line )
{
  p_view      v = VIEW(o);
  p_scroller  sc = SCROLLER(o);
  t_rect  r = o->line_rect(o, line);
  t_rect  t;

  t_point p;

  BITMAP *out = v->begin_paint(v, &p, r);

  if ( out ) {

     l_color fcol = color_flat_text;
     l_color fselcol = color_selected_text;
     l_color bselcol = color_selected_face;

     l_rect  sx   = -1;
     l_rect  ex   = -1;

     l_long  len;
     l_text  text  = o->lntxtlen(o, line, &len); /* get text from line "line"
                                                    and pos "pos" + len will
                                                    contains length of this
                                                    line */

     l_rect  dx = FONT_GETSTRWIDTH(v->font, text, len);;

     t = rect_move(r, p.x, p.y);

     v->background(v, out, t);

     if ( text && line >= o->sel_from.a.y && line <= o->sel_from.b.y ) {

         l_int l = len;

         sx = 0;

         if ( line == o->sel_from.a.y ) sx = lmax(0, lmin(l, o->sel_from.a.x));
         if ( line == o->sel_from.b.y ) ex = lmax(0, lmin(l, o->sel_from.b.x));

     };

     if ( len > 0 )

         draw_selected_text(out, v->font, text, len, sx, ex,
                            r.a.x+p.x-sc->scrollx, r.a.y+p.y, r.a.x+p.x+dx-sc->scrollx, r.b.y+p.y, TX_ALIGN_DEFAULT,
                            fcol, TX_NOCOLOR, fselcol, bselcol, 0);

     else if ( text && *text ) {

         l_char c[2] = {' ', '\0'};

         draw_selected_text(out, v->font, (l_text)c, 1, sx, ex,
                            r.a.x+p.x-sc->scrollx, r.a.y+p.y, r.a.x+p.x+dx-sc->scrollx, r.b.y+p.y, TX_ALIGN_DEFAULT,
                            fcol, TX_NOCOLOR, fselcol, bselcol, 0);
     };

  };

  v->end_of_paint(v, r);

};


/*
  - draw box of editor, redraw background and all lines from (linefrom)

    in visible box
*/

void  editor_draw_box_ex ( p_editor o, l_long linefrom )
{
  p_view  v = VIEW(o);
  t_rect  r = v->size_limits(v);
  t_rect  x = o->line_rect(o, linefrom);
  t_point p;

  BITMAP *out;

  r = rect_assign(r.a.x, lmax(r.a.y, x.a.y), r.b.x, r.b.y);

  out = v->begin_paint(v, &p, r);

  if ( out ) {

     l_long line = linefrom;
     l_int  pgsz = o->lines_inpage(o);

     v->background(v, out, rect_move(r, p.x, p.y));

     while ( line <= o->line_num && line <= o->line_from + pgsz ) { /* draw all lines */

        if ( line >= o->line_from ) {

               x = o->line_rect(o, line);

               o->draw_line(o, line);

       };

       line++;

     };

  };

  v->end_of_paint(v, r);

  EDITOR(o)->draw_cursor(EDITOR(o), -1, -1);

};


void  editor_draw_box ( p_editor o )
{
  o->draw_box_ex(o, o->line_from);
};


void editor_timer ( p_object o ) {


  if ( !ed_is_wable(EDITOR(o)) )

     return;

  if ( o->is_state(o, OB_SF_FOCUSED) ) {
    if ( EDITOR(o)->cursor_visible ) {
      EDITOR(o)->cursor_visible = 0;
      EDITOR(o)->draw_cursor_ex ( EDITOR(o), EDITOR(o)->line, EDITOR(o)->line_pos, 0);
    } else {
      EDITOR(o)->cursor_visible = 1;
      EDITOR(o)->draw_cursor_ex ( EDITOR(o), EDITOR(o)->line, EDITOR(o)->line_pos, 1);
    };
  } else {
    if ( EDITOR(o)->cursor_visible ) {
      EDITOR(o)->cursor_visible = 0;
      EDITOR(o)->draw_cursor_ex ( EDITOR(o), EDITOR(o)->line, EDITOR(o)->line_pos, 0);
    };
  };
};


/*

  draw cursor to line (line) and pos (pos)

  if active is non-zero it show cursor, otherwise it redraw background

*/
void  editor_draw_cursor_ex ( p_editor o, l_long line, l_long pos, l_int active )
{

  p_view  vo = VIEW(o);

  t_rect  r = o->line_rect(o, line);
  t_rect  s = vo->size_limits(vo);
  t_rect  safe;

  l_long  len;
  l_text  text  = o->lntxtlen(o, line, &len); /* get text from line "line"
                                                 and pos "pos" + len will
                                                 contains length of this
                                                 line */
  l_rect rcpos = FONT_GETSTRWIDTH(vo->font, text, lmax(0, lmin(len, pos)));
  //l_rect ecpos = FONT_GETSTRWIDTH(vo->font, text, lmax(0, lmin(len, pos+1)));
  //l_color fcol = active?COLOR(CO_WHITE):color_flat_text;
  //l_color bcolor = COLOR(CO_BLACK);
  //l_color fselcol = color_selected_text;
  //l_color bselcol = color_selected_face;
  t_point p;

  BITMAP *out;

  //l_rect  sx   = -1;
  //l_rect  ex   = -1;
  l_rect  dx = FONT_GETSTRWIDTH(vo->font, text, len);

  //if ( ecpos == rcpos ) ecpos = rcpos+1+FONT_GETWIDTH(vo->font, ' ');

  r = rect_assign(r.a.x+rcpos, r.a.y, r.a.x+rcpos, r.b.y);

  r = rect_cliped(rect_move(r, -SCROLLER(o)->scrollx, 0), s);

  safe = r;

  out = vo->begin_paint(vo, &p, safe);

  if ( out ) {

    r = rect_move(r, p.x, p.y);

    if ( active ) {

      rectfill(out, r.a.x, r.a.y, r.b.x, r.b.y, color_flat_text);

         /*textout_draw_rect(out, vo->font, &(text[pos]), 1,
                           r.a.x, r.a.y, r.a.x, r.b.y+dx, TX_ALIGN_DEFAULT,
                           fcol, TX_NOCOLOR, 0);*/
    } else {

         o->draw_line ( o, line );

    };


  };

  vo->end_of_paint(vo, safe);

};


l_long editor_charsin ( p_editor o, l_long line, l_long pos, l_int plus )
{

   return o->charsin_size(o, rect_sizex(VIEW(o)->bounds)-(4+FONT_GETWIDTH(VIEW(o)->font, ' ')), line, pos, plus);

};


l_long editor_charsin_size ( p_editor o, l_rect size, l_long line, l_long pos, l_int plus )
{

  l_rect sizex = size;
  l_long chars = 0;

  l_long  tsize;
  l_text  text  = o->lntxtlen(o, line, &tsize); /* get text from line "line" */

  if ( text ) {

    pos += plus;

    while ( sizex > 0 && pos >= 0 && pos <= tsize ) {

      sizex -= FONT_GETWIDTH(VIEW(o)->font, text[pos]);

      pos += plus;

      chars++;

    };

    if ( sizex < 0 ) chars--;

    return lmax(0, chars);

  };

  return 0;

};


l_long editor_linesin ( p_editor o, l_long line, l_int plus )
{

   l_long  dy = o->ysize_of_line(o, 0, 1, NULL);

   return o->linesin_size(o, rect_sizey(VIEW(o)->bounds)-(dy+4), line, plus);

};


l_long editor_linesin_size ( p_editor o, l_rect size, l_long line, l_int plus )
{

  l_rect sizey = size;
  l_long lines = 0;

  l_long tsize = o->line_num;

  while ( sizey > 0 && line >= 0 && line < tsize ) {

      sizey -= o->ysize_of_line(o, line, 1, NULL);

      line += plus;

      lines++;

  };

  if ( sizey < 0 ) lines--;

  return lmax(0, lines);

};


t_rect  editor_get_cursor_rect ( p_editor o, l_long line, l_long pos )
{
  p_view  vo = VIEW(o);

  t_rect  r = o->line_rect(o, line);
  t_rect  safe;

  l_long  len;
  l_text  text  = o->lntxtlen(o, line, &len); /* get text from line "line"
                                                 and pos "pos" + len will
                                                 contains length of this
                                                 line */
  l_rect rcpos = FONT_GETSTRWIDTH(vo->font, text, lmax(0, lmin(len, pos)));
  l_rect ecpos = FONT_GETSTRWIDTH(vo->font, text, lmax(0, lmin(len, pos+1)));

  if ( ecpos == rcpos ) ecpos = rcpos+FONT_GETWIDTH(vo->font, ' ');

  r = rect_assign(lmax(r.a.x, r.a.x+rcpos), r.a.y, lmin(r.b.x, r.a.x+ecpos), r.b.y);

  return r;

};


/*

  draw cursor to current line and current pos and redraw all cursor

  at line (oldline) and pos (oldpos)

*/
void  editor_draw_cursor ( p_editor o, l_long oldline, l_long oldpos )
{

  l_long  len;
  l_text  text     = o->lntxtlen(o, o->line, &len); /* get text from line "line"
                                                       and pos "pos" + len will
                                                       contains length of this
                                                       line */

  l_long  oldlen;
  l_text  oldtext  = o->lntxtlen(o, oldline, &oldlen); /* get text from line "line"
                                                          and pos "pos" + len will
                                                          contains length of this
                                                          line */

  if ( !ed_is_wable(o) ) /* is not-rewrite able */

     return;

  if ( oldpos >= 0 && oldpos <= oldlen )

    o->draw_cursor_ex ( o, oldline, oldpos, 0);


  if ( o->cursor_visible )

    o->draw_cursor_ex ( o, o->line, o->line_pos, 1);

};


void  editor_show_cursor ( p_editor o, l_bool show )
{
  o->cursor_visible = show;

  o->draw_cursor(o, o->line, o->line_pos);
};


/*
  - return num of lines that may be visible in page that start from

    line (line)
*/

l_int  editor_lines_inpage ( p_editor o )
{
  l_int  ln = 0;
  t_rect r = o->line_rect(o, o->line_from);

  while ( !rect_check_empty(r) )

    r = o->line_rect(o, o->line_from+(ln++));


  return ln;
};


/*
  draw only selected lines

  dst - new selected lines
  src - old selected lines
*/

void   editor_draw_select ( p_editor o, t_rect dst, t_rect src )
{
  l_long start = o->line_from;
  l_int  ln    = o->lines_inpage(o);
  l_int  i = 0;

  while ( i < ln ) {

    l_long p = -1;

      if ( src.a.y < o->line_from+i && src.b.y > o->line_from+i &&
           dst.a.y < o->line_from+i && dst.b.y > o->line_from+i )

           ;

      else {

            if ( o->line_from+i >= src.a.y && o->line_from+i <= src.b.y )

                  o->draw_line(o, o->line_from+i);
            else
                  if ( o->line_from+i >= dst.a.y && o->line_from+i <= dst.b.y )

                       o->draw_line(o, o->line_from+i);
      };

      i++;
  };

};


/*
 - return rect of line (line)
*/

t_rect  editor_line_rect ( p_editor o, l_long line )
{
  t_rect t = rect_empty;
  p_scroller sc = SCROLLER(o);
  t_rect s = VIEW(o)->size_limits(VIEW(o));

  if ( line <= o->line_num && line >= o->line_from ) {

    l_long  f = 0;
    l_long  h = o->ysize_of_line(o, line, 1, &f);

    if ( f || h ) t = rect_assign(s.a.x, s.a.y+f-sc->scrolly, s.b.x, s.a.y+h+f-sc->scrolly-1);


  };

  return rect_cliped(t, s);

};



/*
 - set new text to editor and release memory of old one,
 - redraw editor box and scrollbars
*/

l_bool  editor_set_text ( p_editor o, l_text txt )
{

  ed_sel_clear(o);

  _free(o->text); /* release memory of old text */

  if ( txt )

     o->text = strdup(txt);

  else {

     o->text = strdup("");

  };

  o->line_num = o->lnmax(o, &(o->line_longest)); /* get number of lines */
  o->line_from = o->line = o->line_pos_from = o->line_pos = 0;
  o->line_ptr = o->text;

  reset_scroller(o);

  o->draw_box(o);

  set_notedited(o);

  draw_scroller_bars(SCROLLER(o)); /* redraw scroll bars */

  return true;

};


/*
  set line to real (line) and pos to real (pos)

  it means it bounds line and pos
*/

void  editor_realpos ( p_editor o, l_long *line, l_long *pos )
{
  l_long len;
  l_text text;

  if ( !line || !pos ) return;

  text = o->lntxtlen(o, (*line), &len);

  if ( !text || (*line) > o->line_num ) { /* this line is out of range */

     if ( *line > 0 ) {

         (*line) = o->line_num;

         o->lntxtlen(o, (*line), pos);

     } else {

         (*line) = 0;
         (*pos) = 0;

     };

     return;

  };

  if ( (*pos) < 0 && (*line) > 0 ) {

    (*line)--;

    text = o->lntxtlen(o, (*line), &len);

    (*pos) = len;

  } else {

    if ( (*line) < o->line_num && (*pos) > len ) /* posx > than this line */
    {  /* and this line is not > than LINEmax */
      if ( (*line) == o->line ) { /* line is still same, only pos is moved */

        (*line)++;
        (*pos) = 0;

      };

    };

  };

  (*line) = lmax(0, lmin(o->line_num, (*line)));
  (*pos)  = lmax(0, lmin(len, (*pos)));

};


/*
  main function for text manipulating

  - newline is position of newline that must be current now
  - newpos  is position of new char-pos in newline
  - keycode is keyboard code that contains info about type of manipulating

*/

void  editor_redraw_text ( p_editor o, l_long newline, l_long newpos, l_int keycode )
{

  p_scroller sc = SCROLLER(o);
  l_long oldpos = o->line_pos;
  l_long oldline = o->line;
  l_long len = 0;

  l_long l = newline;
  l_long p = newpos;
  l_long drawpfrom = 0;
  l_bool drawp = true;

  l_text text;

  l_bool can_draw_cursor;
  l_bool can_del;
  l_bool must_write;

  l_int  redraw_text = 0;
  t_rect sel = o->sel_from;

  can_draw_cursor = false;
  can_del = true;
  must_write = true;

  redraw_text = 0;

  if ( keycode && !ed_is_wable(o) ) return;

  if ( keycode && ed_is_sel(o) && !(sel.a.y == sel.b.y && sel.b.x == sel.a.x) )  {
    l_text pp = o->posptr(o,sel.a.y,sel.a.x);
    l_text t = o->posptr(o,sel.b.y,sel.b.x);

    o->delstr(o,(pp-o->text),(t-pp));
    if ( sel.a.y < sel.b.y ) {
      newline = sel.a.y;
      newpos  = sel.a.x;
    } else {
      newline = sel.a.y;
      newpos  = sel.a.x;
    };
    l = newline;
    p = newpos;
    o->line = newline;
    o->line_pos = newpos;
    o->show_cursor(o, true);
    VIEW(o)->draw_view(VIEW(o));
  };

  text = o->lntxtlen(o, newline, &len);

  if ( !o->sel_ok && ed_is_sel(o) ) {

        ed_sel_clear(o);

        o->draw_box_ex(o, o->line_from);

  };

  #define coddrawpage(ln)         \
    if ( !ed_is_sel(o) ) {        \
      o->draw_box_ex(o, ln);      \
      drawpfrom = ln;             \
      drawp = false;              \
    } else drawp = true

  if ( keycode ) {

    switch ( KEY_TO(keycode) ) {

      case KB_ENTER     : {  /* insert ENTER to current position */

        if ( !o->inschr(o, '\n', o->line, o->line_pos) )

             return;

        text = o->lntxtlen(o, newline, &len);

        redraw_text = 1;

        coddrawpage(o->line);

      }; break;

      case KB_TAB     : { /* insert TAB to current position */

        l_text tab = (l_text)_malloc(EDITOR_TAB_SIZE+1);
        l_text ps  = o->posptr(o, o->line, o->line_pos);

        if ( tab && ps ) {

            memset(tab, 0x20, EDITOR_TAB_SIZE);

            tab[EDITOR_TAB_SIZE] = '\0';

            if ( o->insstr(o, strsize(o->text, ps), tab) ) {

                  o->draw_line(o, o->line);

            } else {

                  _free(tab);

                  return;
            };
        };

        _free(tab);

      }; break;

      case KB_DEL     : { /* delete char from current position */

        if ( o->line >= o->line_num && o->line_pos >= len ) return; /* if I'm going to delete last text char */

        text = o->lntxtlen(o, newline, &len);

        if ( o->delchr(o, o->line, o->line_pos) == '\n' )
          coddrawpage(o->line);

        /* show cursor */
        o->show_cursor(o, true);

        return;

      }; break;

      case KB_BACKSPACE  : { /* delete char from previous position */

        if ( newline <= 0 && newpos < 0 ) return; /* if pos is lower as first text char */

        text = o->lntxtlen(o, newline, &len);

        /* set cursor to true values */
        o->realpos(o, &newline, &newpos);

        if ( !o->delchr(o, newline, newpos) ) return;

        if ( newline != l )

            coddrawpage(newline); /* if deleted first char of line */

      }; break;

      default: /* insert char to current position */

        if ( (l_byte)TO_CHAR(keycode) >= 32 && (l_byte)TO_CHAR(keycode) <= 255 ) {

           if ( !o->inschr(o, (l_char)TO_CHAR(keycode), o->line, o->line_pos) ) return;

           text = o->lntxtlen(o, newline, &len);

        }; break;
    };
  };


  if ( keycode != TO_KEY(KB_BACKSPACE) ) /* backspace set values before */

       o->realpos(o, &newline, &newpos); /* set cursor to true values */

  o->line_pos = newpos;
  o->line = newline;

  can_draw_cursor = true;


  if ( o->line_pos == oldpos && o->line == oldline ) can_draw_cursor = false;


  if ( o->line_pos != oldpos ||
       o->line != oldline    || redraw_text ) {

      /* X SCROLLING */
      if ( o->line_pos < o->line_pos_from ) { /* scroll left */

            o->draw_cursor_ex(o, oldline, oldpos, 0);

            redraw_text = 0x03;
            can_draw_cursor = false;

            sc->scroll_place(sc, o->line_pos*sc->deltax-sc->scrollx, 0, 0);

      } else /* scroll right */
      if ( o->line_pos - o->line_pos_from > o->charsin(o, o->line, o->line_pos_from, 1) ) {

            o->draw_cursor_ex(o, oldline, oldpos, 0);

            redraw_text = 0x03;
            can_draw_cursor = false;

            sc->scroll_place(sc, (o->line_pos-o->charsin(o, o->line, o->line_pos, -1))*sc->deltax-sc->scrollx, 0, 0);
      };

      /* Y SCROLLING */
      if ( o->line < o->line_from ) { /* scroll up */

            o->draw_cursor_ex(o, oldline, oldpos, 0);

            redraw_text = 0x03;
            can_draw_cursor = false;

            sc->scroll_place(sc, 0, o->line*sc->deltay-sc->scrolly, 0);

      } else /* scroll down */
      if ( o->line - o->line_from > o->linesin(o, o->line_from, 1) ) {

            o->draw_cursor_ex(o, oldline, oldpos, 0);

            redraw_text = 0x03;
            can_draw_cursor = false;

            sc->scroll_place(sc, 0, (o->line-o->linesin(o, o->line_from, 1))*sc->deltay-sc->scrolly, 0);
      };

  };


  if ( keycode ) { /* if something is write I can't shift it */

    o->sel_ok = 0;

    if ( ed_is_sel(o) ) {

        t_rect s = o->sel_from;

        ed_sel_clear(o);

        o->show_cursor(o, false); /* hide cursor */

        if ( drawp ) /* must redraw page */

           o->draw_box_ex(o, drawpfrom);

        else

           o->draw_select(o, o->sel_from, s);

        o->show_cursor(o, true); /* show cursor */
    };

  };

  if ( oldpos != o->line_pos || oldline != o->line ) {

      if ( o->sel_ok ) {

          t_rect s = o->sel_from;

          o->show_cursor(o, false); /* hide cursor */

          if ( o->sel_ok == 3 ) { /* for mouse */

               o->sel_from = rect_assign(o->line_pos, o->line, o->line_pos, o->line);
               o->sel_old = point_assign(o->sel_from.a.x, o->sel_from.a.y);

               o->sel_ok = 1;

          } else

               if ( o->sel_ok == 2 ) {


                     o->sel_from = rect_assign(lmin(oldpos, o->line_pos),
                                               lmin(oldline, o->line),
                                               lmax(oldpos, o->line_pos),
                                               lmax(oldline, o->line));

                     if ( o->sel_from.a.y != o->sel_from.b.y ) { /* not same lines */

                          o->sel_from.a.x = oldpos;
                          o->sel_from.b.x = o->line_pos;

                          o->sel_old.x = o->sel_from.a.x;

                     } else

                          o->sel_old.x = (oldpos<o->line_pos) ? o->sel_from.a.x : o->sel_from.b.x;

                     o->sel_old.y = (oldline<o->line) ? o->sel_from.a.y : o->sel_from.b.y;

                     o->sel_ok = 1;

               } else {

                     o->sel_from.b.y = lmax(o->sel_old.y, o->line);
                     o->sel_from.a.y = lmin(o->sel_old.y, o->line);

                     if ( o->sel_from.b.y > o->sel_old.y )

                           o->sel_from.b.x = o->line_pos;

                     else

                     if ( o->sel_from.b.y == o->sel_old.y )

                           if ( o->sel_from.a.y == o->sel_from.b.y )

                                 o->sel_from.b.x = lmax(o->line_pos, o->sel_old.x);

                           else

                                 o->sel_from.b.x = o->sel_old.x;

                     if ( o->sel_from.a.y < o->sel_old.y )

                            o->sel_from.a.x = o->line_pos;

                     else

                     if ( o->sel_from.a.y == o->sel_old.y )

                            if ( o->sel_from.a.y == o->sel_from.b.y )

                                 o->sel_from.a.x = lmin(o->line_pos, o->sel_old.x);

                            else

                                 o->sel_from.a.x = o->sel_old.x;

               };

      o->draw_select(o, o->sel_from, s);

      o->show_cursor(o, true); /* show cursor */

    } else o->show_cursor(o, true); /* show cursor */

  };

  if ( redraw_text ) {

    if ( redraw_text & 0x02 ) o->draw_cursor(o, -1, -1);

  };

  if ( can_draw_cursor ) o->draw_cursor(o, oldline, oldpos);

};


t_point  editor_cursor_mouse_pos ( p_editor o, t_point where )
{
  p_view v = VIEW(o);
  t_rect s = v->size_limits(v);

  l_text  ptr;
  t_point p;
  l_long  len = 0;
  l_long  ln = o->line_from;
  l_long  ps = 0;

  where.y -= s.a.y;
  where.x -= s.a.x;
  where.x += SCROLLER(o)->scrollx;

  p.y = o->ysize_of_line(o, ln, 1, (l_long*)(&p.x));

  while ( where.y > 0 && (p.y||p.x) ) {

      ln++;

      p.y = o->ysize_of_line(o, ln, 1, (l_long*)(&p.x));

      where.y -= p.y;
  };

  ln = lmax(0, lmin(o->line_num, ln-1));

  p.y = ln;

  ptr = o->lntxtlen(o, ln, &len);

  if ( ptr )

  while ( where.x > 0 && *ptr && ps < len ) {

      ps++;

      where.x -= FONT_GETWIDTH(v->font, *ptr);

      ptr++;

  };

  p.x = lmax(0, lmin(len, (where.x<=0?ps-1:ps)));
  p.y = lmax(0, lmin(o->line_num, p.y));

  return p;
};

l_bool   editor_is_in_select_rect ( p_editor o, t_point where ) {
  t_point pos = o->cursor_mouse_pos(o, where);

  if ( ed_is_sel(o) ) {

       if ( pos.y < o->sel_from.a.y ) return false;
       if ( pos.y > o->sel_from.b.y ) return false;
       if ( pos.y == o->sel_from.a.y && pos.x < o->sel_from.a.x ) return false;
       if ( pos.y == o->sel_from.b.y && pos.x >= o->sel_from.b.x ) return false;

       return true;
  };

  return false;
};

void  editor_translate_event ( p_object o, t_event *event )
{
  RETVIEW(o, event);

  view_translate_event(o, event); /* old translate_event function */

  if ( event->type & EV_MESSAGE ) {
	if ( event->message == MSG_SELECT_ALL ) {
	  l_long len=0;
	  EDITOR(o)->lntxtlen(EDITOR(o),EDITOR(o)->line_num,&len);
      EDITOR(o)->sel_from = rect_assign(0,0,len,EDITOR(o)->line_num);
      VIEW(o)->draw_me(VIEW(o));
	  clear_event(event);
    };
  };
  if ( event->type & EV_MOUSE ) { /* mouse event */
    l_bool in_sel = editor_is_in_select_rect(EDITOR(o),VIEW(o)->get_local_point(VIEW(o), mouse->where));

    if ( OBJECT(mouse)->state & MO_SF_MOUSERDOWN ) {
      show_edit_menu(o,ed_is_sel(EDITOR(o)));
      clear_event(event);
      return;
    };

    if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) { /* select */
      if ( !o->is_state(o, OB_SF_SELECTED) ) {
        clear_event(event);
        o->select(o);
        return;
      };
    };

    if ( in_sel && OBJECT(mouse)->state & MO_SF_MOUSEDOWN ) {
	  p_editor ed = EDITOR(o);
	  t_point oldp = point_assign(ed->line_pos,ed->line);
	  ed->sel_ok = 0;
      ed->cursor_visible = 0;
      while ( event->type & EV_MOUSE && OBJECT(mouse)->state & MO_SF_MOUSELPRESS ) {
        t_point np = ed->cursor_mouse_pos(ed, VIEW(o)->get_local_point(VIEW(o), mouse->where));
        if ( oldp.x != np.x || oldp.y != np.y ) {
	      ed->draw_cursor_ex ( ed, oldp.y, oldp.x, 0);
          ed->draw_cursor_ex ( ed, np.y, np.x, 1);
          ed->line = np.y;
          ed->line_pos = np.x;
        };
        oldp = np;
        o->get_event(o, event);
      };
      if ( ed->sel_from.a.y >= ed->line &&
           ( ed->sel_from.a.y == ed->line ? ed->sel_from.a.x > ed->line_pos : true ) ) {
        // before selection

        l_text pp = ed->posptr(ed,ed->sel_from.a.y,ed->sel_from.a.x);
        l_text t = ed->posptr(ed,ed->sel_from.b.y,ed->sel_from.b.x);

        l_text toinsert = stridup(pp, strsize(pp, t));

        ed->delstr(ed,(pp-ed->text), strsize(pp, t));

        if ( toinsert ) {
          l_text   ltxt = ed->posptr(ed, ed->line, ed->line_pos);
          l_long   pos = strsize(ed->text, ltxt);
          ed->insstr(ed, pos, toinsert);
          _free(toinsert);
        };

        clear_event(event);
      };
      if ( ed->sel_from.b.y <= ed->line &&
           ( ed->sel_from.b.y == ed->line ? ed->sel_from.b.x < ed->line_pos : true ) ) {
	    // after selection
        l_text pp = ed->posptr(ed,ed->sel_from.a.y,ed->sel_from.a.x);
        l_text t = ed->posptr(ed,ed->sel_from.b.y,ed->sel_from.b.x);

        l_text toinsert = stridup(pp, strsize(pp, t));

        ed->delstr(ed,(pp-ed->text), strsize(pp, t));

        if ( ed->line == ed->sel_from.b.y ) ed->line_pos -= ed->sel_from.b.x;
        ed->line -= ed->sel_from.b.y-ed->sel_from.a.y;

        if ( toinsert ) {
          l_text   ltxt = ed->posptr(ed, ed->line, ed->line_pos);
          l_long   pos = strsize(ed->text, ltxt);
          ed->insstr(ed, pos, toinsert);
          _free(toinsert);
        };

        clear_event(event);
      };

      EDITOR(o)->cursor_visible = 1;
    };

    if ( (in_sel && OBJECT(mouse)->state & MO_SF_MOUSEUP) ||
            (!in_sel && OBJECT(mouse)->state & MO_SF_MOUSELDOWN) ) {
      EDITOR(o)->sel_ok = 0;
      EDITOR(o)->redraw_text(EDITOR(o), EDITOR(o)->line, EDITOR(o)->line_pos, 0);
      clear_event(event);
    };

    if ( o->is_state(o, OB_SF_FOCUSED) ) {

        if ( OBJECT(mouse)->state & MO_SF_MOUSELAUTO && !EDITOR(o)->sel_ok )

              EDITOR(o)->sel_ok = 3;

        while ( event->type & EV_MOUSE && OBJECT(mouse)->state & MO_SF_MOUSELAUTO ) {
            t_point p = EDITOR(o)->cursor_mouse_pos(EDITOR(o),
                                   VIEW(o)->get_local_point(VIEW(o), mouse->where));

            EDITOR(o)->redraw_text(EDITOR(o), p.y, p.x, 0);

            EDITOR(o)->sel_ok = 1;

            o->get_event(o, event);

        };

        EDITOR(o)->sel_ok = 0;
        clear_event(event);
     };

  };


  if ( o->is_state(o, OB_SF_FOCUSED) )

                                  /* keyboard events */
  if ( event->type & EV_KEYBOARD ) {

    if ( !(keyb->shifts & KB_SHIFT_FLAG) ) EDITOR(o)->sel_ok = 0;

    else if ( !EDITOR(o)->sel_ok ) EDITOR(o)->sel_ok = 2;


    if ( OBJECT(keyb)->state & KB_SF_KEYDOWN )

    switch ( KEY_TO(keyb->code) ) {

      case KB_RIGHT : {  /* arrow RIGHT was pressed */

        EDITOR(o)->redraw_text(EDITOR(o), EDITOR(o)->line, EDITOR(o)->line_pos+1, 0);

        clear_event(event);

      }; break;

      case KB_LEFT : {  /* arrow LEFT was pressed */

        EDITOR(o)->redraw_text(EDITOR(o), EDITOR(o)->line, EDITOR(o)->line_pos-1, 0);

        clear_event(event);

      }; break;

      case KB_DOWN : {  /* arrow DOWN was pressed */

        EDITOR(o)->redraw_text(EDITOR(o), EDITOR(o)->line+1, EDITOR(o)->line_pos, 0);

        clear_event(event);

      }; break;

      case KB_UP : {  /* arrow UP was pressed */

        EDITOR(o)->redraw_text(EDITOR(o), EDITOR(o)->line-1, EDITOR(o)->line_pos, 0);

        clear_event(event);

      }; break;

      case KB_PGDN : {  /* PAGE DOWN was pressed */

        EDITOR(o)->redraw_text(EDITOR(o), EDITOR(o)->line+EDITOR(o)->linesin(EDITOR(o), EDITOR(o)->line, 1), EDITOR(o)->line_pos, 0);

        clear_event(event);

      }; break;

      case KB_PGUP : {  /* PAGE UP was pressed */

        EDITOR(o)->redraw_text(EDITOR(o), EDITOR(o)->line-EDITOR(o)->linesin(EDITOR(o), EDITOR(o)->line, -1), EDITOR(o)->line_pos, 0);

        clear_event(event);

      }; break;

      case KB_HOME : {  /* key HOME was pressed */

        EDITOR(o)->redraw_text(EDITOR(o), EDITOR(o)->line, 0, 0);

        clear_event(event);

      }; break;

      case KB_END : {  /* key END was pressed */

        l_long len;

        EDITOR(o)->lntxtlen(EDITOR(o), EDITOR(o)->line, &len);

        EDITOR(o)->redraw_text(EDITOR(o), EDITOR(o)->line, len, 0);

        clear_event(event);

      }; break;

      case KB_DEL : {  /* key DEL was pressed */

        EDITOR(o)->redraw_text(EDITOR(o), EDITOR(o)->line, EDITOR(o)->line_pos, TO_KEY(KB_DEL));

        clear_event(event);

      }; break;

      case KB_BACKSPACE : {  /* key BACKSPACE was pressed */

        EDITOR(o)->redraw_text(EDITOR(o), EDITOR(o)->line, EDITOR(o)->line_pos-1, TO_KEY(KB_BACKSPACE));

        clear_event(event);

      }; break;

      case KB_ENTER : { /* key ENTER was pressed */

        EDITOR(o)->redraw_text(EDITOR(o), EDITOR(o)->line+1, 0, TO_KEY(KB_ENTER));

        clear_event(event);

      }; break;

      case KB_TAB : {  /* key TAB was pressed */

        EDITOR(o)->redraw_text(EDITOR(o), EDITOR(o)->line, EDITOR(o)->line_pos+EDITOR_TAB_SIZE, TO_KEY(KB_TAB));

        clear_event(event);

      }; break;

      default :

        if ( (l_byte)TO_CHAR(keyb->code) >= 32 &&
             (l_byte)TO_CHAR(keyb->code) <= 255 ) {

             EDITOR(o)->redraw_text(EDITOR(o), EDITOR(o)->line, EDITOR(o)->line_pos+1, keyb->code);

             clear_event(event);

        }; break;
    };

  };

};

p_editor  _editor_init ( p_editor o, t_rect r, l_int flags )
{

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_editor));

  scroller_init(SCROLLER(o), r, SF_HORSCROLLBAR+SF_VERSCROLLBAR);


  /* object's declarations */
  OBJECT(o)->done = &editor_done;
  OBJECT(o)->translate_event = &editor_translate_event;
  OBJECT(o)->set_state = &editor_set_state;
  OBJECT(o)->set_options = &editor_set_options;
  OBJECT(o)->set_data = &editor_set_data;
  OBJECT(o)->get_data = &editor_get_data;

  OBJECT(o)->func_callback = &editor_timer;
  OBJECT(o)->process_tick = 500;
  OBJECT(o)->set_options(OBJECT(o), OB_OF_STILLPROCESS, true);

  l_tag_cpy(OBJECT(o)->data_type, DAT_TEXT);


  /* view's declarations */

  if ( !(flags & EF_REWRITEUNABLE) )

       VIEW(o)->cursor = CUR_TEXT;

  VIEW(o)->draw = &editor_draw;
  VIEW(o)->size_limits = &editor_size_limits;
  VIEW(o)->drag_where = &editor_drag_where;


  /* scroller's declarations */

  SCROLLER(o)->scroll_size = &editor_scroll_size;
  SCROLLER(o)->recalc_positions = &editor_recalc_positions;
  SCROLLER(o)->scroll_limits = &editor_scroll_limits;


  /* editor's declarations */

  o->flags = flags|EF_FIXEDFONT;

  o->text = strdup("");

  ed_sel_clear(o);

  o->sel_old = point_assign(o->sel_from.a.x, o->sel_from.a.y);

  o->lntxtlen = &editor_lntxtlen;
  o->lnmax = &editor_lnmax;
  o->lnptr = &editor_lnptr;
  o->posptr = &editor_posptr;
  o->sizeto = &editor_sizeto;
  o->lnlen = &editor_lnlen;
  o->lnlenptr = &editor_lnlenptr;
  o->xsize_of_line = &editor_xsize_of_line;
  o->ysize_of_line = &editor_ysize_of_line;
  o->draw_cursor = &editor_draw_cursor;
  o->draw_cursor_ex = &editor_draw_cursor_ex;
  o->show_cursor = &editor_show_cursor;
  o->get_cursor_rect = &editor_get_cursor_rect;
  o->charsin_size = &editor_charsin_size;
  o->charsin = &editor_charsin;
  o->linesin = &editor_linesin;
  o->linesin_size = &editor_linesin_size;
  o->lines_inpage = &editor_lines_inpage;
  o->draw_select = &editor_draw_select;

  o->insstr = &editor_insstr;
  o->delstr = &editor_delstr;
  o->delchr = &editor_delchr;
  o->inschr = &editor_inschr;

  o->draw_box_ex = &editor_draw_box_ex;
  o->draw_box = &editor_draw_box;
  o->draw_line = &editor_draw_line;

  o->realpos = &editor_realpos;
  o->redraw_text = &editor_redraw_text;

  o->cursor_mouse_pos = &editor_cursor_mouse_pos;

  o->set_text = &editor_set_text;
  o->line_rect = &editor_line_rect;

  VIEW(o)->brush.color = color_flat_face;

  VIEW(o)->font = get_font_in_size ("ARIAL", 10, 10);

  return o;

};
