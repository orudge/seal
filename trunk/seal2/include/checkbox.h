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

#ifndef _NEW_DIALOGS_H_
#define _NEW_DIALOGS_H_

typedef struct t_checkbox *p_checkbox;

typedef struct t_checkbox {
  struct t_view class;

  l_dword state;
  l_text  caption;
  l_color fcolor;
  l_dword message;

  p_skin skin_checkbox_up;         // OCR - 30/03/2002
  p_skin skin_checkbox_down;
  p_skin skin_checkbox_greyed;
  p_skin skin_checkbox_greyed_checked;
  p_skin skin_checkbox_checked;

} t_checkbox;

#define CHECKBOX(o) ((p_checkbox)(o))

#define CB_CHECKED     0x01
#define CB_GREYED      0x02

extern p_checkbox (*checkbox_init) ( p_checkbox o, t_rect r, l_text caption, l_dword state, l_dword message );
p_checkbox _checkbox_init ( p_checkbox o, t_rect r, l_text caption, l_dword state, l_dword message );
#endif