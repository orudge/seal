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

#ifndef _TRACKBAR_SEAL_H_
#define _TRACKBAR_SEAL_H_

typedef struct t_trackbar {
  struct t_view obclass;

  l_uchar style;

  l_long  min;
  l_long  max;
  l_long  value;

  l_dword message;

  l_bool  pressed;

} t_trackbar, *p_trackbar;

#define TRACKBAR(o) ((p_trackbar)(o))

#define TB_HORISONTAL 0
#define TB_VERTICAL   1

extern p_trackbar (*trackbar_init) ( p_trackbar o, t_rect r, l_uchar style,l_long  min, l_long  max, l_long  value, l_dword message );
p_trackbar _trackbar_init ( p_trackbar o, t_rect r, l_uchar style,l_long  min, l_long  max, l_long  value, l_dword message );
#endif
