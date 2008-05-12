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

#ifndef __DRIVERS_H_INCLUDE__
#define __DRIVERS_H_INCLUDE__

#include "object.h"
#include "keyboard.h"
#include "mouse.h"
#include "alltogrx.h"
#include "dataini.h"



/* if non-zero the safe mode is set */
extern l_int   safe_mode;

/* fonts delcarations */

/* defines if the font will be smooth */
extern l_int   font_smooth;


/* return width of string (str) defined by font (f) and length (l).
   if l < 0  => strlen
*/
#define     FONT_GETSTRWIDTH(f,str,l)  ((f)?GrFontTextLength(f,str,l):0)

/* return height of font (f) */
#define     FONT_GETSTRHEIGHT(f)       ((f)?GrFontTextHeight(f):0)

/* return width of char (ch) defined by font (f) */
#define     FONT_GETWIDTH(f,ch)        ((f)?GrFontCharLength(f,ch):0)

/* return height of font */
#define     FONT_GETHEIGHT(f)          (FONT_GETSTRHEIGHT(f))


/*
   define functions for stoping the timer or reload it when it's important
*/

#include "stimer.h"


/* t_drivers */

typedef struct t_drivers *p_drivers;

typedef struct t_drivers {

  t_object  obclass;

} t_drivers;



#endif

