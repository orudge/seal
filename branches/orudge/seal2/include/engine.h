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

#ifndef _SEAL_KERNEL_INCLUDED_
#define _SEAL_KERNEL_INCLUDED_

#include <stdio.h>
#include <string.h>

#ifndef             NULL
#define NULL        ((void*)0)
#endif

#ifndef true
#define true        1
#endif
#ifndef false
#define false       0
#endif

#ifndef TRUE
#define TRUE        true
#endif
#ifndef FALSE
#define FALSE       false
#endif



#define NEW_SEAL_TYPES

typedef  int                l_bool;
typedef  long               l_long;
typedef  int                l_int;
typedef  char               l_char;
typedef  long long          l_big;
typedef  unsigned char      l_byte,  l_uchar;
typedef  unsigned int       l_word,  l_uint;
typedef  unsigned long      l_dword, l_ulong;
typedef  unsigned long long l_ubig;
typedef  char               l_tag[32];
typedef  long               l_rect;
typedef  int                l_color;
typedef  char*              l_text;
typedef  void*              l_ptr;
typedef  int                l_process;

// GrXXXXXX things  ( please don't use it, it's just for compatibility )

/*#define  ScreenContext    screen
#define  GrRGB            RGB
#define  GrCOLOR          int
#define  GrCONTEXT        BITMAP
#define  GrRLE_SPRITE     RLE_SPRITE
#define  GrIMAGE          GrCONTEXT
#define  GrFONT           void*
#define  GrFIXED          fixed
#define  GrV3D            V3D
#define  GrV3DF           V3D_f
#define  GrPALETTE        PALETTE

#define  GrRgb            GrRGB
#define  GrColor          GrCOLOR
#define  GrContext        GrCONTEXT
#define  GrRleSprite      GrRLE_SPRITE
#define  GrImage          GrContext
#define  GrFont           GrFONT
#define  GrFixed          GrFIXED
#define  GrV3d            GrV3D
#define  GrV3Df           GrV3DF
#define  GrPalette        GrPALETTE

#define  CONTEXT_WIDTH(d)  (d?d->w:0)
#define  CONTEXT_HEIGHT(d) (d?d->h:0)
#define  IMAGE_WIDTH(d)    CONTEXT_WIDTH(d)
#define  IMAGE_HEIGHT(d)   CONTEXT_HEIGHT(d)  */

#define l_font GrFont

#include"allegro.h"
#include"alltogrx.h"
#include"dlx.h"
#include"safmem.h"

#define l_tag_cpy(x,y)    tag_strcpy((l_text)x, (l_text)y, sizeof(l_tag));
#define l_tag_cmp(x,y)    (!tag_stricmp(x,y))

#define clear_type(t,s)   if (t) memset(t, 0, s)
#define clear_table(t,s)  clear_type(t,s)
#define clear_event(t)    clear_type(t,sizeof(t_event))

#ifndef strsize
#define strsize(p,v)    ((v)?((v)-(p)):strlen(p))
#endif
#ifndef min
#define min(x,y)    (((x) < (y)) ?  (x) : (y))
#endif
#ifndef max
#define max(x,y)    (((x) > (y)) ?  (x) : (y))
#endif
#ifndef imin
#define imin(x,y)   min((l_int)(x),(l_int)(y))
#endif
#ifndef imax
#define imax(x,y)   max((l_int)(x),(l_int)(y))
#endif
#ifndef umin
#define umin(x,y)   min((l_word)(x),(l_word)(y))
#endif
#ifndef umax
#define umax(x,y)   max((l_word)(x),(l_word)(y))
#endif
#ifndef lmin
#define lmin(x,y)   min((l_long)(x),(l_long)(y))
#endif
#ifndef lmax
#define lmax(x,y)   max((l_long)(x),(l_long)(y))
#endif
#ifndef dmin
#define dmin(x,y)   min((l_dword)(x),(l_dword)(y))
#endif
#ifndef dmax
#define dmax(x,y)   max((l_dword)(x),(l_dword)(y))
#endif
#ifndef abs
#define abs(x)      (((x)<0)?-(x):(x))
#endif
#ifndef labs
#define labs(x)     abs((l_long)(x))
#endif

// l_text TXT ( l_int id );

#define ERR_NONTHING         0
#define ERR_NONREGULAR       1
#define ERR_INFO             2

#define TAG_NONE              0x0000000
#define TAG_DISPOSE           TAG_NONE
#define TAG_OBJECT            0x0000001
#define TAG_VIEW              0x0000002
#define TAG_WINDOW            0x0000004
#define TAG_BUTTON            0x0000008

/*
  data types
*/

#define  DAT_NONE      "NONE"    /* none data type */
#define  DAT_TEXT      "TEXT"    /* single text ended by zero */
#define  DAT_IMAGE     "IMAGE"   /* type of BITMAP */
#define  DAT_TFILE     "TFILE"   /* file t_file structure */
#define  DAT_LIST      "LIST"    /* list ( p_list) */
#define  DAT_ALLKNOW   "ALLKNOW" /* all data types are accepted */
#define  DAT_NUMBER    "NUMBER"  /* type is the number */
#define  DAT_FLOAT     "FLOAT"   /* type is the number with floating point */
#define  DAT_HTML      "HTML"    /* type is the HTML format */

#ifdef __cplusplus
   extern "C" {
#endif

// ---- Engine.c ---------------------------------------------------------------

l_int  DEBUG_printf ( l_text text, ...);

extern l_int  (*seal_error) ( l_int errtype, l_text str, ... );
extern l_text  SealApi;
extern l_text  SealApiCopyright;
extern l_int   SealApiVersion;
extern l_text  SealApiTxtVersion;

#include"object.h"
#include"view.h"
#include"list.h"
#include"drivers.h"

extern l_int  task_tick;

extern t_object   program;

extern t_list     stillprocess_list;

extern p_drivers  drivers;

extern p_view     desktop;

extern l_int      clock_including;

extern l_int   translucide;

int  program_put_into_stillprocess ( p_object o, p_object s );
int  program_clear_from_stillprocess ( p_object o, p_object s );
void program_put_event ( t_object *o, t_event *event );
void program_get_event ( t_object *o, t_event *event );

void   program_translate_event ( t_object *o, t_event *event );
l_bool program_done ( t_object *o );

t_object *program_init ( t_object *o );
void      program_call_each_stillprocess ( p_list o );

#define   program_shutdown()   do {                                            \
                set_event(&event_main, EV_MESSAGE, MSG_QUIT, OBJECT(&program));\
                OBJECT(&program)->put_event(OBJECT(&program), &event_main);    \
          } while (0)

#ifdef    ENABLE_MULTITASKER
l_bool    __multi_process = true;
#define   HALT_MULTITASKER() __multi_process=false
#define   while(t1)      _while((__multi_process)&&(##t1))
#endif


// ---- Text.c -----------------------------------------------------------------

l_text  set_format_text ( l_text *dest, l_text format, ... );
l_text  set_format_text_nice ( l_text *dest, l_int size, l_text format, ... );
l_text  insstr ( l_text dest, l_text str, l_long pos, l_long size );
l_text  delstr ( l_text dest, l_long pos, l_long size );
l_text  stridup ( l_text str, l_long num );
l_text  stristr ( l_text s, l_text d );
l_text str_replace ( l_text what, l_text bywhat, l_text txt, l_bool freetxt );

#define mid(txt,a,len) stridup((txt)+(a),(len))

void trans_blit ( BITMAP *src, BITMAP *dst, l_int sx, l_int sy, l_int dx, l_int dy, l_int w, l_int h, l_int nb, l_int max );

FILE* SealFileOpen ( l_text SealFile, l_text mode );
void SealFileClose ( FILE *f );
#define iffound(a,b) ({l_color c=b;if(c!=-1) a=c;})

l_int save_image( char *filename, BITMAP *bmp, AL_CONST RGB *pal);

void seal2_app_about(l_text appname, BITMAP *icon);

#ifdef __cplusplus
   }
#endif

#endif /* _SEAL_KERNEL_INCLUDED_ */
