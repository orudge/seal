/****************************************************************/
/*                                                              */
/*                           syntax.c                           */
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



#include <errno.h>
#include <seal.h>
#include <menus.h>
#include <app.h>
#include <button.h>
#include <dialogs.h>
#include <editor.h>
#include <iodlg.h>
#include <ctype.h>


l_text syntax_C[] = { "char", "double", "enum", "float", "int", "long", "short",
                      "signed", "struct", "typedef", "union", "unsigned", "void",
                      "auto", "const", "extern", "register", "static", "vloatile",
                      "break", "case", "continue", "default", "do", "else", "for",
                      "goto", "if", "return", "sizeof", "switch", "while", "true",
                      "false", "NULL",

                      "l_text", "l_char", "l_int", "sizeof", "app_begin", "app_end",
                      "l_ptr", "l_long", "l_byte", "l_word", "l_dword", "l_big",

                       NULL};

l_char syntax_C_operator[] = { '{', '}', '[', ']', ';', ',', '.', '=', '/', '<', '>',
                                ':', '-', '+', '*', '(', ')', '%', '&', '^', '}', '?',
                                '!', ';',

                                0};

l_text syntax_C_ignore[] = { "//", NULL};


l_text syntax_C_define[] = { "#", NULL};


typedef struct t_syntax *p_syntax;


/* t_syntax */

typedef struct t_syntax {

  struct t_editor  obclass;

  l_text *syntax;
  l_char *syntax_operator;
  l_text *syntax_ignore;
  l_text *syntax_define;

} t_syntax;


#define SYNTAX(o) ((p_syntax)(o))



l_bool is_syntax_space ( l_char chr, l_char *ops )
{
   l_text x = ops;

   if ( isspace(chr) || !chr ) return true;

   if ( x )

   while ( *x ) {

      if ( *x == chr ) return true;

      x++;

   };

   return false;
};


void  syntax_colorize ( l_text text, l_text syntax, l_char *ops, l_color *mem, l_color color, l_int size, l_bool sens )
{
   l_text str = text;

   l_bool ok = true;
   l_int  len = strlen(syntax);

   while ( true ) {

      str = sens?stristr(str, syntax):strstr(str, syntax);

      if ( !str ) return;

      ok = true;

      if ( str != text ) ok = is_syntax_space(*(str-1), ops);

      if ( ok && is_syntax_space(*(str+len), ops) ) {

          l_int i = str-text;
          l_int l = min(size, i+len);

          for ( i; i < l; i++ ) mem[i] = color;

      };

      str++;

   };
};


void syntax_char_colorize ( l_text text, l_char syntax, l_color *mem, l_color color, l_int size )
{
   l_text str = text;

   l_int i;

   while ( true ) {

      str = strchr(str, syntax);;

      if ( !str ) return;

      i = str-text;

      if ( size > i ) mem[i] = color;

      str++;

   };
};


void syntax_ignore_colorize ( l_text text, l_text syntax, l_color *mem, l_color color, l_int size )
{
   l_text str = text;

   l_int i;

   str = strstr(str, syntax);

   if ( !str ) return;

   i = str-text;

   for ( i; i < size; i++ ) mem[i] = color;

};


void syntax_define_colorize ( l_text text, l_text syntax, l_color *mem, l_color color, l_int size )
{
   l_text str = text;

   l_int i;

   str = strstr(str, syntax);

   if ( !str ) return;

   i = str-text;

   for ( i; i < size; i++ ) mem[i] = color;

};


void syntax_draw_selected_text ( l_text *syntax, l_char *syntax_operator, l_text *syntax_ignore,
                                 l_text *syntax_define,
                                 BITMAP *out, l_font *font, l_text text, l_int len,
                                 l_int sx, l_int ex,
                                 l_rect ax, l_rect ay, l_rect bx, l_rect by, l_int align,
                                 l_color fcol, l_color bcol,
                                 l_color fselcol, l_color bselcol,
                                 l_color s_col, l_color so_col, l_color si_col,
                                 l_color sd_col,
                                 l_int clip)
{

   if ( text ) {

      l_int size = len<0?strlen(text):len;

      if ( size > 0 ) {

         l_int    i = 0;

         l_color *colrs = _malloc(sizeof(l_color)*size);

         for ( i; i < size; i++ ) colrs[i] = fcol;

         while ( *syntax ) {

               syntax_colorize(text, *syntax, syntax_operator, colrs, s_col, size, true);

               syntax++;

         };

         while ( *syntax_operator ) {

               syntax_char_colorize(text, *syntax_operator, colrs, so_col, size);

               syntax_operator++;

         };


         while ( *syntax_ignore ) {

               syntax_ignore_colorize(text, *syntax_ignore, colrs, si_col, size);

               syntax_ignore++;

         };


         while ( *syntax_define ) {

               syntax_define_colorize(text, *syntax_define, colrs, sd_col, size);

               syntax_define++;

         };


         for ( i = 0; i < size; i++ ) {

            draw_char(out, font, (unsigned char)(text[i]), ax, ay, colrs[i], bcol);

            ax += get_char_length(font, (unsigned char)(text[i]));

         };

         _free(colrs);

      };

   };

};




/*
  - redraw line (line) in syntax
*/

void  syntax_draw_line ( p_editor o, l_long line )
{
  p_view      v = VIEW(o);
  p_scroller  sc = SCROLLER(o);
  t_rect  r = o->line_rect(o, line);
  t_rect  t;

  t_point p;

  BITMAP *out = v->begin_paint(v, &p, r);

  if ( out ) {

     l_color fcol = v->get_color(v, 2);
     l_color fselcol = v->get_color(v, 4);
     l_color bselcol = v->get_color(v, 3);

     l_rect  sx   = -1;
     l_rect  ex   = -1;

     l_long  len;
     l_text  text  = o->lntxtlen(o, line, &len); /* get text from line "line"
                                                    and pos "pos" + len will
                                                    contains length of this
                                                    line */

     l_rect  dx = FONT_GETSTRWIDTH(v->font, text, len);;

     if ( o->flags & EF_REWRITEUNABLE ) { /* text is unrewrite able */

         fselcol = v->get_color(v, 6);
         bselcol = v->get_color(v, 5);

     };

     t = rect_move(r, p.x, p.y);

     v->background(v, out, t);

     if ( text && line >= o->sel_from.a.y && line <= o->sel_from.b.y ) {

         l_int l = len;

         sx = 0;

         if ( line == o->sel_from.a.y ) sx = lmax(0, lmin(l, o->sel_from.a.x));
         if ( line == o->sel_from.b.y ) ex = lmax(0, lmin(l, o->sel_from.b.x));

     };

     if ( len > 0 )

         syntax_draw_selected_text(SYNTAX(o)->syntax, SYNTAX(o)->syntax_operator, SYNTAX(o)->syntax_ignore,
                                   SYNTAX(o)->syntax_define,
                            out, v->font, text, len, sx, ex,
                            r.a.x+p.x-sc->scrollx, r.a.y+p.y, r.a.x+p.x+dx-sc->scrollx, r.b.y+p.y, TX_ALIGN_DEFAULT,
                            fcol, TX_NOCOLOR, fselcol, bselcol,
                            COLOR(CO_LIGHTBLUE), COLOR(CO_LIGHTBLUE),
                            COLOR(CO_DARKGRAY), COLOR(CO_GREEN),
                            0);

     else if ( text && *text ) {

         l_char c[2] = {' ', '\0'};

         draw_selected_text(out, v->font, (l_text)c, 1, sx, ex,
                            r.a.x+p.x-sc->scrollx, r.a.y+p.y, r.a.x+p.x+dx-sc->scrollx, r.b.y+p.y, TX_ALIGN_DEFAULT,
                            fcol, TX_NOCOLOR, fselcol, bselcol, 0);
     };

  };

  v->end_of_paint(v, r);

};



p_syntax  syntax_init ( p_syntax o, t_rect r, l_int flags, l_text *syntax,
                                                           l_char *syntax_operator,
                                                           l_text *syntax_ignore,
                                                           l_text *syntax_define )
{

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_syntax));

  editor_init(EDITOR(o), r, flags);


  /* syntax's declarations */

  o->syntax = syntax;
  o->syntax_operator = syntax_operator;
  o->syntax_ignore = syntax_ignore;
  o->syntax_define = syntax_define;


  /* editor's declarations */

  EDITOR(o)->draw_line = &syntax_draw_line;


  return o;

};



p_syntax  syntax_C_init ( p_syntax o, t_rect r, l_int flags )
{

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_syntax));

  syntax_init(o, r, flags, syntax_C, syntax_C_operator, syntax_C_ignore, syntax_C_define);

  return o;

};



