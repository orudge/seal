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

#ifndef BUFFER_FORMAT_TEXT
#define BUFFER_FORMAT_TEXT   1024
#endif

l_text  set_format_text ( l_text *dest, l_text format, ... )
{
  va_list arg;
  static  l_char buffer[BUFFER_FORMAT_TEXT];
  l_text         dst = NULL;
  va_start(arg, format);
  vsprintf(buffer, format, arg);
  dst = _strdup(buffer);
  va_end(arg);

  if(strlen(buffer)>BUFFER_FORMAT_TEXT){
    DEBUG_printf("Over buffer !\nShut Down\n");
    exit(1);
  }
  if ( dest ) if ( *dest ) free(*dest);
  if ( dest ) *dest = dst;

  return dst;
};

l_text  set_format_text_nice ( l_text *dest, l_int size, l_text format, ... )
{
  va_list arg;
  static  l_char buffer[BUFFER_FORMAT_TEXT];
  l_text         dst = NULL;
  l_int          strsz;
  if ( !format ) return NULL;
  va_start(arg, format);
  strsz = vsprintf(buffer, format, arg);
  va_end(arg);

  if(strlen(buffer)>(BUFFER_FORMAT_TEXT-4)){
    DEBUG_printf("Over buffer !\nShut Down\n");
    exit(1);
  }

  if ( (strsz > abs(size)) && (size > 0) ) {
      buffer[0] = '\0';
      if ( size < 0 ) { /* before text */
         strcat(buffer, "...");
         buffer[abs(size)+3] = '\0';
      } else { /* after text */
         strcat(buffer, (&buffer)[(strsz-abs(size))-4]);
         strcat(buffer, "...");
      };
  };
  dst = _strdup(buffer);
  if ( dest ) *dest = dst;
  return dst;
}

l_text  insstr ( l_text dest, l_text str, l_long pos, l_long size )
{
  l_text n = NULL;
  l_long ttsize = 0;
  l_long sz = 0;
  if ( !str || pos < 0 || size <= 0 ) return dest;
  sz = strlen(str);
  if ( sz < 0 ) return dest;
  if ( !dest ) {
    n = (l_text)_malloc(lmin(sz, size)+1);
    n[0] = '\0';
    strcat(n, str);
    return n;
  } else {
    l_text p;
    l_text n;
    ttsize = strlen(dest)+1;
    size = lmax(0, lmin(sz, size));
    pos  = lmax(0, lmin(ttsize-1, pos));
    p = _strdup(&dest[pos]);

    n = (l_text)_realloc(dest, ttsize+size);

    if ( n && p ) {
         strcpy(&n[pos], str);
         strcpy(&n[pos+size], p);
    };
    _free(p);
    return n;
  };
};

l_text  delstr ( l_text dest, l_long pos, l_long size )
{
  if ( dest && size > 0 ) {

    l_long ttsize = strlen(dest);

    if ( pos >= ttsize ) return dest;

    size = lmax(0, lmin((ttsize-pos), size));
    pos  = lmax(0, lmin(ttsize, pos));

    if ( size > 0 ) {

      l_text p = _strdup(&dest[pos+size]);
      l_text n = (l_text)_realloc(dest, (ttsize-size)+1);

      if ( n && p ) {

        strcpy(&n[pos], p);

      };

      _free(p);

      return n;
    };

  };

  return dest;
};


l_text  stridup ( l_text str, l_long num )
{
  if ( str && num > 0 ) {

    l_text b = (l_text )_malloc(num+1);

    if ( b ) {

      strncpy(b, str, num);
      b[num] = '\0';

    };

    return b;

  };

  return NULL;
};


l_text stristr ( l_text s, l_text d )
{
  if ( s && d && *d ) {

    while ( *s ) {
      l_text olds = s;
      l_text oldd = d;
      while ( *s && *d && tolower(*s) == tolower(*d) ) {
        s++;
        d++;
      };
      if ( !(*d) ) return olds;
      if ( olds == s ) s++;
      d = oldd;
      s++;
    };

  };

  return NULL;
};

l_text str_replace ( l_text what, l_text bywhat, l_text txt, l_bool freetxt ) {
  if ( what && txt ) {
    l_text t = freetxt ? txt : _strdup(txt);
    l_uint a = 0;
    l_uint len1 = strlen(what);
    l_uint len2 = bywhat?strlen(bywhat):0;
    while ( t[a] ) {
	  if ( !strncmp( t+a, what, len1 ) ) {
	    t = delstr(t,a,len1);
	    if ( len2 ) t = insstr(t,bywhat,a,len2);
	    a+=len1-1;
      };
	  a++;
    };
	return t;
  };
};
