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

#ifndef _STATTEXT_H_INCLUDED_
#define _STATTEXT_H_INCLUDED_
/*
  object t_stattext is used drawing formated text and placed it to the other object.
*/
typedef struct t_stattext *p_stattext;

typedef struct t_stattext {

  /* inherited functions / variabes from t_view and t_object objects */
  struct t_view  obclass;

  /* formated text */
  l_char         text[1024];

  /* alignment. use values TX_ALIGN_XXXX. */
  l_int          align;

  l_color        color_text;

  void           (*set_text) ( p_stattext o, l_text text, ... );

} t_stattext;

#define STATTEXT(o) ((p_stattext)(o))

p_stattext _stattext_init_ex ( p_stattext o, t_rect r, l_int align, l_text text, va_list argp );
p_stattext _stattext_init ( p_stattext o, t_rect r, l_int align, l_text format, ... );
extern p_stattext (*stattext_init_ex) ( p_stattext o, t_rect r, l_int align, l_text text, va_list argp );
extern p_stattext (*stattext_init) ( p_stattext o, t_rect r, l_int align, l_text format, ... );

void  stattext_draw ( p_view o );
void  stattext_set_text ( p_stattext o, l_text text, ... );

#endif
