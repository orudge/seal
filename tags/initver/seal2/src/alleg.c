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

#include <allegro.h>

extern unsigned short *_key_ascii_table;
extern unsigned short *_key_capslock_table;
extern unsigned short *_key_shift_table;
extern unsigned short *_key_control_table;
extern unsigned short *_key_altgr_lower_table;

unsigned short *key_ascii_table=(unsigned short *)&_key_ascii_table;
unsigned short *key_capslock_table=(unsigned short *)&_key_capslock_table;
unsigned short *key_shift_table=(unsigned short *)&_key_shift_table;
unsigned short *key_control_table=(unsigned short *)&_key_control_table;
unsigned short *key_altgr_table=(unsigned short *)&_key_altgr_lower_table;

BITMAP  *_fixup_loaded_bitmap ( BITMAP *bmp, PALETTE pal, int bpp );

extern int _textmode;
extern int _blender_alpha;
extern int _color_conv;
int _color_load_depth(int _depth, int hasalpha);

#include "alleg\alltogrx.c"

#include "alleg\fonts.c"

