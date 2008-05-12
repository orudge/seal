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
 * 02/04/2002: Made stattext appear transparent is skins enabled
 */

#include <seal.h>
#include <stattext.h>
#include <skin.h>

p_stattext (*stattext_init_ex) ( p_stattext o, t_rect r, l_int align, l_text text, va_list argp ) = &_stattext_init_ex;
p_stattext (*stattext_init) ( p_stattext o, t_rect r, l_int align, l_text format, ... ) = &_stattext_init;

void  stattext_set_text ( p_stattext o, l_text text, ... ) {
  va_list arg;

  va_start(arg, text);

  vsprintf(o->text, text, arg);

  VIEW(o)->draw_view(VIEW(o));

  va_end(arg);
};

void  stattext_draw ( p_view o ) {
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
                        STATTEXT(o)->align,
                        STATTEXT(o)->color_text, TX_NOCOLOR, 0);

      if ( t ) { /* enter was found */

            t++; /* move to next char after enter */

            r.a.y += FONT_GETHEIGHT(o->font);

      };

    };

  };

  o->end_of_paint(o, safe);
};

p_stattext _stattext_init_ex ( p_stattext o, t_rect r, l_int align, l_text text, va_list argp )
{
  static l_char buffer[1024];

  if ( !o || !text ) return NULL;

  clear_type(o, sizeof(t_stattext));

  view_init(VIEW(o), r);


  /* view's functions */

  VIEW(o)->draw = &stattext_draw;


  /* stattext's declarations */

  o->align    = align;
  o->set_text = &stattext_set_text;
  o->color_text = color_3d_text;
  vsprintf(o->text, text, argp);



  /* function calling */

  OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE, false);
  OBJECT(o)->set_options(OBJECT(o), OB_OF_ENABLE, false);

  VIEW(o)->brush.color = color_3d_face;

  if (use_skins)
     VIEW(o)->transparent = true;

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

