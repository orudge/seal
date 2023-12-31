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

#ifdef  __DEBUG_MEMORY__
#define MSS
#include <mss.h>
#endif
#include <safmem.h>

void     tag_strcpy ( void *dst, void *src, l_int size )
{
  if ( dst ) {
     clear_type(dst, size);
     if ( src ) strncpy(dst, src, size-1);
  };
};

l_int   tag_stricmp ( void *dst, void *src )
{
  if ( dst && src ) return stricmp(dst, src);
  return 1;
};

/* safe malloc */
void   sf_alert ( void )
{
     seal_error(ERR_INFO, "%s.\n\n%s", TXT_NOTENOUGHMEMEMORYFOROPERATION, TXT_PLEASESTOREYOURDATAANDEXITSOMEAPPLICATION);
};

void  *sf_malloc ( size_t size )
{
void *p = malloc(size);
return p;
};
void  sf_free ( void *rec )
{
  if (rec) free(rec);
};
#ifdef __RSXNT__
#define cfree(rec) free(rec)
#endif
void  sf_cfree ( void *rec )
{
  if (rec) cfree(rec);
};
void  *sf_calloc ( size_t num_elements, size_t size )
{
   return calloc(num_elements, size);
};
char  *sf_strdup ( const char *source )
{
 return source?strdup(source):NULL;
};
void   *sf_realloc ( void *rec, size_t size )
{
 return rec?realloc(rec, size):sf_malloc(size);
};


