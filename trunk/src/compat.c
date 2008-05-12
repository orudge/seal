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

#include <seal.h>

/* Compatibility routines for earlier versions of SEAL */

/*
 return link to file in (*link) and icon filename (*icon) from ".lnk" file (file).
*/
void io_linkedfile_ex (l_text file, l_text *link, l_text *icon)
{
   if (link)
      (*link) = getini_fromfile(file, "definition", "link");

   if (icon)
      (*icon) = getini_fromfile(file, "definition", "icon32");
}


/*
 return link from ".lnk" file (file). It return new filename where the
 physical file is.
 example:
 l_text shfile = io_linkedfile("c:/seal/desktop/hello.lnk");
 shfile contains "c:/seal/image.exe" f.e.
*/
l_text io_linkedfile (l_text file)
{
   l_text link = NULL;
   io_linkedfile_ex(file, &link, NULL);
   return link?link:_strdup(file);
}