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
/*                          badctrls.h                          */
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

#ifndef __BADEDITR_H_INCLUDED__
#define __BADEDITR_H_INCLUDED__

typedef struct t_beditor *p_beditor;
typedef struct t_beditor
{
  struct t_view obclass;

  l_text text;
  l_bool modified;

  l_int line_pos;
  l_int line_len;
  l_int lines;
  l_int page_start;

  l_int sel_start;
  l_int sel_len;

  l_int x, y;
  l_int px, py;

  l_bool cursor;

  l_int flags;

  void (*set_text)(p_beditor o, l_text text);
  void (*draw_line)(p_beditor o, BITMAP *out, t_rect r, l_text txt);
} t_beditor;

#define BEDITOR(o) ((p_beditor)(o))

/* Sets editor's text */
void beditor_set_text(p_beditor o, l_text text);

p_beditor _beditor_init(p_beditor o, t_rect r, l_int flags);
extern p_beditor (*beditor_init)(p_beditor o, t_rect r, l_int flags);

#endif

