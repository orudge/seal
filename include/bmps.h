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

#ifndef _BMPS_SEAL_H_
#define _BMPS_SEAL_H_

#define IMG_FILE16           GETBMPID(0)
#define IMG_FILE32           GETBMPID(1)
#define IMG_DIR16            GETBMPID(2)
#define IMG_DIR32            GETBMPID(3)
#define IMG_WARNING          GETBMPID(4)
#define IMG_ERROR            GETBMPID(5)
#define IMG_INFO             GETBMPID(6)
#define IMG_QUESTION         GETBMPID(7)
#define IMG_UNAVAILABLE      GETBMPID(8)
#define IMG_SETUP            GETBMPID(9)
#define IMG_WCLOSE           GETBMPID(10)
#define IMG_WMIN             GETBMPID(11)
#define IMG_WMAX             GETBMPID(12)
#define IMG_APP16            GETBMPID(13)
#define IMG_APP32            GETBMPID(14)
#define IMG_PARDIRBT         GETBMPID(15)
#define IMG_ABOUT			 GETBMPID(16)
#define IMG_DWCLOSE          GETBMPID(17)
#define IMG_DWMIN            GETBMPID(18)
#define IMG_DWMAX            GETBMPID(19)
#define IMG_DABOUT           GETBMPID(20)

BITMAP *GETBMP ( l_text name );
BITMAP *GETBMPID ( l_int id );

// XApp GET_IMG emulation
#define GET_IMG(x) GETBMP(x)

#endif
