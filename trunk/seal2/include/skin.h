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

#ifndef _SKIN_SEAL_H_
#define _SKIN_SEAL_H_

extern l_bool use_skins;
extern l_text skin_file;

typedef struct t_skin {
  t_rect d;
  BITMAP *bmp;
  l_color text;
} t_skin, *p_skin;

#define SKIN(o) ((p_skin)(o))

t_point get_skin_space ( p_skin o, l_int w, l_int h );
l_int   get_skin_height ( p_skin o, l_int h );
l_int   get_skin_width ( p_skin o, l_int w );
#define get_skin_fcolor(o) ((o)->text)
p_skin  get_skin ( l_text skinfile, l_text section );
void    draw_skin ( BITMAP *out, p_skin o, t_rect r, t_rect *z );

#endif
