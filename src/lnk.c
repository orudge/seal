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

/* Revision History as of 29/03/2002:
 *
 * 29/03/2002: Title section added to link file (orudge)
 * 01/04/2002: Moved title code into new function (orudge)
 */

#include <seal.h>
#include <lnk.h>

l_bool lnk_is_link ( l_text file ) {
  if ( !file ) return false;
  return !stricmp(get_extension(file), "LN");
};

l_text lnk_get_title(l_text file)
{
   l_text title;

   if (!file)
      return(NULL);

   if (lnk_is_link(file))
   {
      title = strdup(getini_fromfile(file, "definition", "title"));
//      DEBUG_printf("%s = %s\n", file, title);

      if ((title == NULL) || (title[0] == 0))
         return(get_filename(lnk_nice_link(file)));
      else
         return(title);
   }
   else
      return(get_filename(file));
}
  
l_text lnk_nice_link (l_text file)
{
  if (!file)
     return NULL;

  if (lnk_is_link(file))
     return stridup(file, strlen(file)-3);
  else
     return _strdup(file);
};

BITMAP *lnk_get_link_icon ( l_text file, l_int size, l_bool *ownmem ) {
  if ( file ) {
    l_text icon = getini_fromfile(file,"definition",(size<32)?"icon16":"icon32");
    if ( icon ) {
      if ( icon[0] == '&' ) {
        return GETBMP(icon+1);
      } else {
        BITMAP* bmp = load_image(icon);
        conv_to_skipcolor_bitmap(bmp,255,0,255);
        return bmp;
      };
    };
  };
  return NULL;
};

l_text lnk_get_link ( l_text file ) {
  if ( file ) return getini_fromfile(file,"definition","link");
  return NULL;
};

l_int lnk_run_link ( l_text file, l_text args ) {
  if ( file ) {
    l_text rfile = lnk_get_link(file);
    if ( rfile ) {
      return run_file_args ( rfile, args);
    };
  };
  return 0;
};
