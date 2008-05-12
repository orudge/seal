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

#include "object.h"
#include "alltogrx.h"

#ifndef __SCREEN_H_INCLUDED__
#define __SCREEN_H_INCLUDED__

extern BITMAP *screen_virtual;
extern BITMAP  *screen_double_buffer;

extern l_int screen_width;
extern l_int screen_height;

extern l_int screen_viswidth;
extern l_int screen_visheight;

extern l_int screen_card;
extern l_int screen_depth;

extern l_bool   screen_init ( void );
extern void     screen_done ( void );

extern l_bool  screen_reload ( void );
extern void    screen_halt ( void );

extern PALETTE Gr_pal;

#endif

