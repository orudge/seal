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

#ifndef _PMAD_VFILE_H_
#define _PMAD_VFILE_H_

#include<dir.h>
#include"list.h"

#define io_filename(fn)    _strdup(io_getfilename(fn))

#define io_path(fn)        stridup(fn, strsize(fn, io_getfilename(fn)))

#define io_getfilename(fn) ((fn)?get_filename(fn):NULL)
typedef struct t_vfile *p_vfile;

typedef struct t_vdrive *p_vdrive;

typedef struct t_ffblk {

   struct ffblk info;

   l_int        ff_attrib;
   l_char       ff_filename[260];

   p_item fst;      // *
   p_item nxt;      // *  Private, NEVER PUT ANY INFORMATION HERE !
   l_text pathname; // *
   p_vfile drv;    // *

   l_char       ff_reserved[24];

} t_ffblk;

typedef struct t_vdrive {
  l_text  drive;
  p_vfile drv;

} t_vdrive;

typedef struct t_vfile {
  void     *data;
  l_text ( *GetFile )     ( p_vfile o, l_text path );
  l_int  ( *GetFileAttr ) ( p_vfile o, l_text path );
  l_int  ( *FindFirst )   ( p_vfile drv, l_text path, struct t_ffblk *ffblk, int attrib);
  l_int  ( *FindNext )    ( struct t_ffblk *ffblk );
  FILE  *( *FileOpen )    ( p_vfile o, l_text path, l_text mode );
  void   ( *FileClose )   ( p_vfile o, l_text path, FILE *f );
  l_int  ( *FileDelete )  ( p_vfile o, l_text path );
  l_int  ( *DirMake )     ( p_vfile o, l_text path );
  l_int  ( *FileRename )  ( p_vfile o, l_text src, l_text dst );


} t_vfile;

#ifdef __cplusplus
  extern "C" {
#endif

l_text ResolveFileName ( l_text SealFile );

int io_findnext(struct t_ffblk *ffblk);
int io_findfirst(const char *pathname, struct t_ffblk *ffblk, int attrib);
l_text GetFile ( l_text SealFile );
l_int GetFileAttr ( l_text SealFile );
l_bool  io_isdir ( l_text file );
l_bool  io_isfile ( l_text file );
l_bool  io_exist ( l_text file );
l_text  file_gettext ( l_text filename );
l_bool  file_puttext ( l_text filename, l_text text );
l_text  io_realpath ( l_text path, l_text file );
l_bool  io_mkdir ( l_text dir );
l_int io_rename ( l_text nameold, l_text namenew );
l_int   _io_removefile ( l_text file );

extern p_vfile FatVFile;
extern p_list VirtualDrives;
l_text ResolveFileName ( l_text SealFile );
p_vfile GetVFile ( l_text SealFile, l_text *pth );
void AddVDrive ( l_text drive,
                 void *data,
                 l_text ( *GetFile ) ( p_vfile o, l_text path ),
                 l_int  ( *GetFileAttr ) ( p_vfile o, l_text path ),
                 l_int  ( *FindFirst ) ( p_vfile drv, l_text path, struct t_ffblk *ffblk, int attrib),
                 l_int  ( *FindNext )( struct t_ffblk *ffblk ),
                 FILE  *( *FileOpen ) ( p_vfile o, l_text path, l_text mode ),
                 void   ( *FileClose ) ( p_vfile o, l_text path, FILE *f ),
                 l_int  ( *FileDelete ) ( p_vfile o, l_text path ),
                 l_int  ( *DirMake ) ( p_vfile o, l_text path ),
                 l_int  ( *FileRename ) ( p_vfile o, l_text src, l_text dst ) );
void Subst ( l_text drive, l_text path );

#ifdef __cplusplus
  }
#endif

#endif
