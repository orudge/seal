/****************************************************************/
/*                                                              */
/*                          exedlx.h                            */
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




#ifndef __EXEDLX_H_INCLUDED__
#define __EXEDLX_H_INCLUDED__

#include"object.h"

#define INI_EXTRUNNERS   "extension_runners"

#define EXT_LIBOSX       "%lib-seal"


typedef struct t_ico_loaders *p_ico_loaders;
typedef struct t_ext_runners *p_ext_runners;

/* ico loaders - all function that know to load icons */

typedef struct t_ico_loaders {

  l_char      *ext;
  BITMAP*    (*load_icon)(l_text filename, l_int size, l_int *ownmem);

  struct t_ico_loaders *next;

} t_ico_loaders;


/* respresents one item in the ext_runners list */

typedef struct t_ext_runners {

  l_char      *ext;
  l_char      *app;
  l_char       reserved[24];

} t_ext_runners;

/* list of extension runners, what are application that can run
   specific extension.
*/
extern p_ext_runners  *ext_runners;

/* list of loaders ( mechanism ) that can read icons from various types of
   files.
*/
extern p_ico_loaders   ico_loaders;


/*
 run all files in file "_filename".
 This file must have "*.set" structure and files to be run are defined
 between '<' and '>' charaters. (see "app.set" file)
*/
extern l_int (*read_set_file)(l_char *_filename);

/*
 this run DOS command (command), and change path to (newpath).
 return NON-ZERO if all was OK, otherwise return ZERO
*/
extern l_int (*dos_command) ( l_text newpath, l_text command );

/*
 this run DOS program (command), and change path to (newpath).
 return NON-ZERO if all was OK, otherwise return ZERO
*/
extern l_int (*dos_run) ( l_text newpath, l_text command );


/* insert new application "program" that can run extension "ext" to the list
   "ext_runners"
*/
void     put_into_runners ( l_char *ext, l_char *program );

/* return path+filename of application that can run extension "ext"
*/
l_char*  get_from_runners ( l_char *ext );

/*
  run application for specified file format defined in "args".
  These file formats and their applications are defined in "seal.ini"
  file in structure [extension_runners].

  example :

  seal.ini file :
  ...
  [extension_runners]
  "exe" = "libos-x"
  "dlx" = "libos-x"
  "bmp" = "imager.exe"
  ...

  run_file("hello.bmp");

  this run "./programs/imager.exe" and ap_args set to "hello.bmp"

  run_file("hello.exe");

  only run "hello.exe" and set ap_args to NULL
*/
l_int    run_file ( l_char *args );

/*
  run file (file) by arguments defined in (args). return 1, if file was run succesfull.
*/
l_int    run_file_args ( l_text file, l_char *args );

/*
  load icon from file "args", defined by size of icon "size". "size" may be one of
  following : 16 = ICON_16, 32 = ICON_32. (ownmen) will return information,
  if icon must be release or not. If ownmem = 1, we must free memory by
  Allegro's destroy_bitmap(), otherwise we couldn't.
*/
BITMAP*  load_file_icon ( l_char *args, l_int size, l_int *ownmem );

/*
  return BITMAP that's icon for file/directory "filename". (ownmen) will
  return information, if icon must be release or not. If ownmem = 1, we must
  free memory by Allegro's destroy_bitmap(), otherwise we couldn't.
*/
BITMAP*  get_icon_for_file ( l_text filename, l_int attrib, l_int *ownmem );

/*
  same as previous function, but we define size of icon. Size may be one of
  following : 16 = ICON_16, 32 = ICON_32;
*/
BITMAP*  get_icon_for_file_ex ( l_text filename, l_int attrib, l_int *ownmem, l_int size );

/*
  return list of arguments, from args text, same as dos-line.
  Each argument must be spaced by ' '.
*/
p_list   get_args ( l_char *args );

/*
  add new mechanism "load_icon" for icon loading from extension "ext" to
  "icon_loaders" list.
*/
void     add_to_loadfileicon ( l_char *ext, BITMAP* (*load_icon)(l_text filename, l_int size, l_int *ownmem) );


/* not for seal.h */
BITMAP*  _load_exe_icon ( l_char *args, l_int size, l_int *ownmem );
BITMAP*  _load_ldv_icon ( l_char *args, l_int size, l_int *ownmem );
BITMAP*  _load_lnk_icon ( l_char *args, l_int size, l_int *ownmem );

void     init_loadfileicon ( void );
void     free_loadfileicon ( void );

/* return true if file is executable DOS file */
l_int    is_dos_executable ( l_text file );

l_int    _dos_command ( l_text newpath, l_text command );

l_int    _dos_run ( l_text newpath, l_text command );

l_int  _read_set_file ( l_char *_filename );

#endif

