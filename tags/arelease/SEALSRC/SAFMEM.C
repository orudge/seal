/****************************************************************/
/*                                                              */
/*                           safmem.c                           */
/*                                                              */
/*                Seal - free desktop environment               */
/*                                                              */
/*                    Copyright (c) 1999,2000                   */
/*                         Michal Stencl                        */
/*                     All Rights Reserved                      */
/*                                                              */
/* mail : stenclpmd@ba.telecom.sk                               */
/* web  : http://www.home.sk/public/seal/                       */
/*                                                              */
/* This file is part of Seal.                                   */
/*                                                              */
/* Seal is free software; you can redistribute it and/or        */
/* modify it under the terms of the GNU General Public License  */
/* as published by the Free Software Foundation; either version */
/* 2, or (at your option) any later version.                    */
/*                                                              */
/* Seal is distributed in the hope that it will be useful, but  */
/* WITHOUT ANY WARRANTY; without even the implied warranty of   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See    */
/* the GNU General Public License for more details.             */
/*                                                              */
/* You should have received a copy of the GNU General Public    */
/* License along with Seal; see the file COPYING.  If not,      */
/* write to the Free Software Foundation, 675 Mass Ave,         */
/* Cambridge, MA 02139, USA.                                    */
/*                                                              */
/****************************************************************/


#ifdef  __DEBUG_MEMORY__
#define MSS
#include"mss.h"
#endif
#include"safmem.h"


void     tag_strcpy ( void *dst, void *src, l_int size )
{
  if ( dst ) {

     clear_type(dst, size);

     if ( src ) strncpy(dst, src, size-1);
  };
};


l_int   tag_stricmp ( void *dst, void *src )
{
  return stricmp(dst, src);
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



/* safe free */

void  sf_free ( void *rec )
{

  if ( rec ) free(rec);
  rec = 0;

};


void  sf_cfree ( void *rec )
{

  if (rec) cfree(rec);
  rec = 0;

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

  return realloc(rec, size);

};


