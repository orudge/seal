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

#ifndef _FILES_H_SEAL_
#define _FILES_H_SEAL_

#define IO_FILE_LIMIT   180
#define IO_DIR_LIMIT    280
#define IO_TEXT_LIMIT   IO_FILE_LIMIT

#define FA_LINK         64
#define FA_LINKDIR     128
#define FA_LINKDRIVE   256
#define FA_NOTPARENT   512

#define FA_ALL        (FA_RDONLY|FA_ARCH|FA_HIDDEN|FA_DIREC|FA_SYSTEM)

typedef struct t_file {

  l_text       path;      /* path to file */
  l_text       filename;  /* name of the file */
  l_int        attrib;    /* attributino of the file */

  l_word       time;  /* hours:5, minutes:6, (seconds/2):5 */
  l_word       date;  /* (year-1980):7, month:4, day:5 */
  l_dword      size;  /* size of file */

  l_char       reserved[24];

} t_file;

#define io_tfiletofile(i) io_realpath((i)->path, (i)->filename)

#define DIF_SIZE          0x00001
#define DIF_HALT          0x00002
#define DIF_DIRCALLBACK   0x00004

typedef struct t_dirinfo {

   l_dword          dirs;    /* number of directories was going trought */
   l_dword          files;   /* number of files in these directory */
   l_dword          size;    /* size of all files */
   l_int            flags;

   l_char           reserved[24];

} t_dirinfo;


typedef struct t_fileop *p_fileop;

typedef struct t_fileact *p_fileact;

typedef struct t_fileact {

  l_text   cmd;
  l_int   (*act) ( l_text filename, l_text args );
  l_text   txt;

} t_fileact;

typedef struct t_fileop {

  l_text   ext;

  BITMAP  *ico32;
  BITMAP  *ico16;
  BITMAP  *(*GetIcon)( l_text filename, l_int size, l_int *ownmem );

  l_text   description;

  p_fileact def;
  p_list    actions;


} t_fileop;

#ifdef __cplusplus
  extern "C" {
#endif

t_dirinfo     io_foreach_file_ex ( l_text path, l_int flags, l_int (*callback)(), p_object o, l_dword *ind );
#define       io_foreach_file(p,f,o,i) io_foreach_file_ex(p, f, NULL, o, i)
t_file        *new_tfile ( l_text path, l_text filename, l_int attrib, l_word time, l_word date, l_dword size );
void          free_tfile ( void *p );
void          io_cleartfile ( t_file *f );
t_file        io_filetotfile ( l_text filename );
l_int         io_foreach_file_copy ( l_text dst, l_text path, p_object o, l_dword *ind );
l_int         io_removefile ( p_object ob, t_file *f, l_dword *ind );
l_int         io_copyfile ( p_object o, t_file *dst, t_file *src, l_dword *ind );
l_int         _io_copyfile ( l_text dst, l_text src );
l_int         io_numberfile ( p_object ob, t_file *f, l_dword *ind );
l_text        io_parentdir ( l_text path );
l_text        io_uniquedir ( l_text path );
l_bool        io_isfilename ( l_text file );
l_bool        io_isext ( l_text file );
l_bool        io_issame ( l_text file1, l_text file2 );
p_fileop      GetFileopForExp ( l_text ext );
p_fileop      GetFileopForFile ( l_text file );
p_fileop      GetFileopForFileAttrib ( l_text file, l_int attrib );
p_fileop      fo_init ( p_fileop o,
                       l_text ext,
                       BITMAP *ico32,
                       BITMAP *ico16,
                       BITMAP *(*GetIcon)( l_text filename, l_int size, l_int *ownmem ),
                       l_text  description );
void          fo_add_act ( p_fileop i,
                  p_fileact o,
                  l_text   cmd,
                  l_int   (*act) ( l_text filename, l_text args),
                  l_text   txt,
                  l_bool   def );
BITMAP*       get_icon_for_file_ex ( l_text filename, l_int attrib, l_int *ownmem, l_int size );
BITMAP*       get_icon_for_file ( l_text filename, l_int attrib, l_int *ownmem );
BITMAP*       load_file_icon ( l_char *args, l_int size, l_int *ownmem );
l_int         run_file_args ( l_text file, l_text args );
l_int         run_file ( l_text file );

extern        p_list   filesop;
extern        p_fileop fo_directory ;
extern        p_fileop fo_deffile;

#ifdef __cplusplus
  }
#endif

#define FILEOP(o) ((p_fileop)(o))

#define FA_ISFILE(t)  (!((t) & FA_DIREC && (t) & FA_LINKDIR))
#define io_sealfile(x) _strdup(x) /*Needed by some programs but they free rreturn value so we have to _strdup*/
#endif
