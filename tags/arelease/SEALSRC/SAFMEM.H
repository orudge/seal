/****************************************************************/
/*                                                              */
/*                           safmem.h                           */
/*                                                              */
/*                Seal - free desktop environment               */
/*                                                              */
/*                  Copyright (c) 1999,2000                     */
/*                       Michal Stencl                          */
/*                    All Rights Reserved                       */
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



#ifndef __SAFMEM_H_INCLUDED__
#define __SAFMEM_H_INCLUDED__

#include<dpmi.h>

#ifndef TXT_NOTENOUGHMEMEMORYFOROPERATION
#define TXT_NOTENOUGHMEMEMORYFOROPERATION "Not enough memory for operation"
#endif
#ifndef TXT_PLEASESTOREYOURDATAANDEXITSOMEAPPLICATION
#define TXT_PLEASESTOREYOURDATAANDEXITSOMEAPPLICATION "Please store your data and exit some application"
#endif

#define  __SAFE_MEMORY__
#ifdef   __SAFE_MEMORY__
#ifndef _malloc
#define _malloc        sf_malloc
#endif
#ifndef _free
#define _free          sf_free
#endif
#ifndef _realloc
#define _realloc       sf_realloc
#endif
#ifndef  _strdup
#define  _strdup       sf_strdup
#endif
#ifndef  _calloc
#define  _calloc       sf_calloc
#endif
#ifndef  _cfree
#define  _cfree        sf_cfree
#endif
#endif

void     tag_strcpy ( void *dst, void *src, l_int size );
l_int    tag_stricmp ( void *dst, void *src );

void     sf_alert ( void );
void    *sf_malloc ( size_t size );
void     sf_free ( void *rec );
void    *sf_realloc ( void *rec, size_t size );
void     sf_cfree ( void *rec );
void    *sf_calloc ( size_t num_elements, size_t size );
char    *sf_strdup ( const char *source );

#endif

