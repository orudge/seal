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

/*

(c) Copyright 2001 Julien Etelain. All rights reserved.

*/

#ifndef __GRFX_H_INCLUDED__
#define __GRFX_H_INCLUDED__


typedef struct t_grfx *p_grfx;

typedef struct t_grfx {

  struct t_view    obclass;

} t_grfx;

#define grfx(o)   ((p_grfx)(o))

p_grfx _grfx_init ( p_grfx o, t_rect r );
extern p_grfx (*grfx_init) ( p_grfx o, t_rect r );

#endif

