/****************************************************************/
/*                                                              */
/*                        _iodir.h                              */
/*                                                              */
/*               Seal - free desktop environment                */
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



#ifndef ___IODIR_H_INCLUDED__
#define ___IODIR_H_INCLUDED__

#include<dir.h>

/* the section of drives. This means [drives] in seal.ini */
#define INI_DRIVES   "drives"

/* the name of desktop. Please not change. */
#define DRIVE_DESKTOP "desktop"
/* the name of pc. Please not change. */
#define DRIVE_PC      "pc"

/* the limit of chars for the name of the file. */
#define IO_FILE_LIMIT   180
/* the limit of chars for the name of the directory. */
#define IO_DIR_LIMIT    280


/*
  these definitions are used for function io_findfirst
*/
/* file is "link" to other file */
#define FA_LINK         64
/* file is "link" to directory. Seal use also link to directory */
#define FA_LINKDIR     128
/* file is "link" to drive */
#define FA_LINKDRIVE   256
#define FA_NOTPARENT   512

/* defines all attributes for file */
#define FA_ALL        (FA_RDONLY|FA_ARCH|FA_HIDDEN|FA_DIREC|FA_SYSTEM|FA_LINK|FA_LINKDIR)
/* checks if the file attribute (t) is file and not directory */
#define FA_ISFILE(t)  (!((t) & FA_DIREC && (t) & FA_LINKDIR))


/* t_dirinfo flags */

#define DIF_SIZE          0x00001 /* get size of directory */
#define DIF_HALT          0x00002 /* process was halted */
#define DIF_DIRCALLBACK   0x00004 /* callback is call for directory too */


typedef struct t_drives *p_drives;


/*
  the item of the drive
*/
typedef struct t_drives {

   l_int            id;     /* (id) of the drive */
   l_text           name;   /* name for the drive ( desktop ) */
   l_text           path;   /* path to the drive */

   l_char           reserved[24];

   struct t_drives *next;   /* pointer to the next drive in the list */

} t_drives;


/*
  used for io_findfirst and io_findnext functions.
*/
typedef struct t_ffblk {

   struct ffblk info; /* old info from DJGPP, you got by the function findfirst */

   l_int        ff_attrib;        /* reserved, for attributes use info.ff_attrib */
   l_char       ff_filename[260]; /* the real name of the file. info.ff_name contains the modification ( *.lnk not show extensions ) */

   l_char       ff_reserved[24];

} t_ffblk;


/*
  fat is used for DAT_FILE id.

  When (id) of {t_data or t_list or t_object[data_type]} is set to DAT_FILE,
  it means that some pointer will point to this structure. We will explain more
  about t_data structure and DAT_XXXX types in file "object.h".
*/

typedef struct t_file {

  l_text       path;      /* path to file */
  l_text       filename;  /* name of the file */
  l_int        attrib;    /* attributino of the file */

  l_word       time;  /* hours:5, minutes:6, (seconds/2):5 */
  l_word       date;  /* (year-1980):7, month:4, day:5 */
  l_dword      size;  /* size of file */

  l_char       reserved[24];

} t_file;


/*
  used as an output of io_for_each_file_ex function.
*/
typedef struct t_dirinfo {

   l_dword          dirs;    /* number of directories was going trought */
   l_dword          files;   /* number of files in these directory */
   l_dword          size;    /* size of all files */
   l_int            flags;

   l_char           reserved[24];

} t_dirinfo;


/* list of drives */
extern p_drives drives;


/* seal.h exportable functions */

/*
 allocate new memory for t_file structure and set arguments to this
 structure from function arguments.
*/
t_file *new_tfile ( l_text path, l_text filename, l_int attrib, l_word time, l_word date, l_dword size );

/*
 free context of (p) and (p) too.
*/
void    free_tfile ( void *p );

/*
 find first occurence of file pathname that is defined by
 atributions "attrib". Result put into t_ffblk structure.
 Return 0, if file was found, otherwise it return non-zero.
 This function is simple to findfirst function, but it support links
 files and it keep compatility with next versions.
 Please use this function for all file searches.
*/
int     io_findfirst(const char *pathname, struct t_ffblk *ffblk, int attrib);

/*
 find next occurence of file, that you defined in previous io_findfirst function.
 Return non-zero if no next file found, or ZERO  in succesfull.
 Use this function together with io_findfirst for all file finding.
 You will keep the compatibility.
*/
int     io_findnext(struct t_ffblk *ffblk);

/*
 run callback function for each file in directory ( path ) and its
 subdirectories etc,... Callback function contains 1 argument l_text realpath,
 what's in path+'/'+file type. (o) is pointer to t_object structure
 what control process, couldn't be used, but support halt of process.
 If something destroy this object, then process will be halted. (*ind) is
 pointer to l_dword variable that contains number of files for which were
 callback function called. Flags may contains combination of these constants :

 'DIF_SIZE'        - get size of directory
 'DIF_HALT'        - process was halted not use for function, it's only
                     returned argument
 'DIF_DIRCALLBACK' - callback function is also called for directories,
                     not only for files.
*/
t_dirinfo io_foreach_file_ex ( l_text path, l_int flags, l_int (*callback)(), p_object o, l_dword *ind );
#define   io_foreach_file(p,f,o,i) io_foreach_file_ex(p, f, NULL, o, i)

/*
 copy directories from directory (path) to destination directory (dst).
 (o) and (ind) have same efect as in io_foreach_file_ex function ( see above ).
 return zero, if some error occured or zero if files was copyied.
*/
l_int   io_foreach_file_copy ( l_text dst, l_text path, p_object o, l_dword *ind );

/*
 remove ( delete ) file/directory ( f ).
 (ob), (ind) have same effect as in io_foreach_file_ex function ( see above ).
 return zero if some error occured, else return true.

 PLEASE, CALL THIS FUNCTION FOR COPYING (files/dirs), IT CONTROLS ALL !
*/
l_int   io_removefile ( p_object ob, t_file *f, l_dword *ind );

/*
 copy file/directory ( src ) from file/directory (src).
 (ob), (ind) have same effect as in io_foreach_file_ex ( see above ).
 return non-zero if all files was copied succesfull, otherwise return zero.

 PLEASE, CALL THIS FUNCTION FOR COPYING (files/dirs), IT CONTROLS ALL !
*/
l_int   io_copyfile ( p_object ob, t_file *dst, t_file *src, l_dword *ind );

/*
 get number of files from file/directory (dst).
 This number is stored to (ind).
 return non-zero if succesfull, otherwise it return false.
 (ob) is object that control process.
 Has same effect as in function io_foreach_file_ex ( see above ).
*/
l_int   io_numberfile ( p_object ob, t_file *dst, l_dword *ind );

/*
 rename file or directory (nameold) to (newnew). It finds if  nameold is
 directory or file and then rename it to new name (namenew).
 It return true if operation was succesfull, otherwise it return false.
*/
l_int   io_rename ( l_text nameold, l_text namenew );

/*
 remove file or directory (src). It find if (src) is file or directory and then
 remove it. Return true if succesfull, else return false.
 It remove only empty directory. It's low-level deleting.
 Use io_removefile(...) you will see later for deleting.
*/
l_int   _io_removefile ( l_text src );

/*
 copy file or directory from file (dst) to file (src).
 Directory is copied only empty. Use io_copyfile(...) for full copying.
 Return true if all was succesfull, return false if not.
*/
l_int   _io_copyfile ( l_text dst, l_text src );

/*
  get real name of file (file) that we find in path (path)and return new string.
  This function control if '/' was after (path) or not. If not, it will add it.

  example :

  l_text all1path = io_realpath("c:/seal/", "seal.exe");
  l_text all2path = io_realpath("c:/seal", "seal.exe");

  all1path contains "c:/seal/seal.exe"
  all2path contains "c:/seal/seal.exe"
*/
l_text  io_realpath ( l_text path, l_text file );

/*
 return link to directory in (*link) and icon filename (*icon) from ".ldv" file (file).
*/
void    io_linkedpath_ex ( l_text file, l_text *link, l_text *icon );

/*
 return link to file in (*link) and icon filename (*icon) from ".lnk" file (file).
*/
void    io_linkedfile_ex (  l_text file, l_text *link, l_text *icon );

/*
 return link from ".ldv" file (file).
 It returns new path from this link.
 Seal has also shortcuts to directories not only to files.

 example:

 l_text shpath = io_linkedpath("c:/seal/desktop/pc/hd-c.ldv");

 shpath contains "c:/"
*/
l_text  io_linkedpath ( l_text file );

/*
 return link from ".lnk" file (file). It return new filename where the
 physical file is.

 example:

 l_text shfile = io_linkedfile("c:/seal/desktop/hello.lnk");

 shfile contains "c:/seal/image.exe" f.e.
*/
l_text  io_linkedfile ( l_text file );

/*
  set link ( reference ) to directory ( link ) when you use file (file).
  icon for this directory will be the name of the file (icon), where icon is placed
*/
void    io_set_linkedfile_ex ( l_text file, l_text link, l_text icon );

/*
  set link ( reference ) to file ( link ) when you use file (file).
  icon for this directory will be the name of the file (icon), where icon is placed
*/
void    io_set_linkedpath_ex ( l_text file, l_text link, l_text icon );

/*
  set link ( reference ) to directory ( link ) when you use file (file).
*/
void    io_set_linkedpath ( l_text file, l_text link );

/*
 set link ( reference ) to file ( link ) when you use file (file).
*/
void    io_set_linkedfile ( l_text file, l_text link );


/*
 same as mkdir(dir, ...), but it keeps compatibility.
*/
l_bool  io_mkdir ( l_text dir );

/*
 return real parent directory of the path.

 example :

 l_text  x = io_parentdir("c:/seal/");
 l_text  y = io_parentdir("c:/seal");

 x = c:/
 y = c:/
*/
l_text  io_parentdir ( l_text path );

/*
 get unique name of directory that not exists in path (path).
 Return the new name of this directory.

 example:

 l_text d1 = io_uniquedir("c:/seal");
 io_mkdir(io_realpath("c:/seal", d1);
 l_text d2 = io_uniquedir("c:/seal");

  d1 = "DIR0"
  d2 = "DIR1"
  ...
*/
l_text  io_uniquedir ( l_text path );

/*
 return true if file or directory exist or false if not.
*/
l_bool  io_exist ( l_text file );

/*
 PLEASE, CALL THIS FUNCTION FOR REMOVING (files/dirs), IT CONTROLS ALL !
*/
l_int   io_remove_file ( l_text file );

/*
 return true if name of file is extension or false if not.

 example:

 l_bool ext = io_isext("file.*");
 l_bool file = io_isext("file.bmp");

 ext is true.
 file is false.
*/
l_bool  io_isext ( l_text file );

/*
 return true if file1 is the same file1 as file2
*/
l_bool  io_issame ( l_text file1, l_text file2 );

/*
 return true if file (file) is file and exist, else it return false.
*/
l_bool  io_isfile ( l_text file );

/*
 return true (file) is directory and it exist, else it return false.
*/
l_bool  io_isdir ( l_text file );

/*
 return new string for filename if the extension is not ".ldv" or ".lnk".
 If it is, it truncate extension and return only new string of poor filename.

 example:

 l_text link_to_directory = io_nicelink("hello.ldv");
 l_text link_to_file = io_nicelink("ciao.lnk");
 l_text file = io_nicelink("image.bmp");

 link_to_directory = "hello"
 link_to_file = "ciao"
 file = "image.bmp"
*/
l_text  io_nicelink ( l_text filename );

/*
 clear memory for pointer t_file, same as
 clear_type(f, sizeof(t_file)) or memset(f, 0, sizeof(t_file))
*/
void    io_cleartfile ( t_file *f );

/*
 convert filename to t_file structure and return this structure.
 Filename is real path, it means it looks like this : path+'/'+file
*/
t_file  io_filetotfile ( l_text filename );

/*
 convert pointer to t_file structure to real path and return
 new allocated string.
*/
#define io_tfiletofile(i) io_realpath((i)->path, (i)->filename)

/*
 return true if file is file or directory name, not ".", ".." etc., else return false.
*/
l_bool  io_isfilename ( l_text file );

/*
 return true if filename (file) has extension same to (ext)
*/
l_bool  io_isas ( l_text file, l_text ext );

/*
 return new string for the file, without the path

 example:

 l_text onlyfile = io_filename("c:/seal.exe");

 onlyfile contains "seal.exe"
*/
#define io_filename(fn)    _strdup(io_getfilename(fn))

/*
 return new string for the path , without the file

 example:

 l_text onlypath = io_path("c:/seal.exe");

 onlypath contains "c:/"
*/
#define io_path(fn)        stridup(fn, strsize(fn, io_getfilename(fn)))

/*
  USE FOR GETTING REAL NAME OF THE FILE. When file is link, it links you to correct
  place of the file.
*/
#define io_sealfile(fn)    io_linkedfile(fn)

/*
  USE FOR GETTING REAL PATH OF THE DIRECTORY. When path is link, it links you to correct
  place of the directory.
*/
#define io_sealpath(fn)    io_linkedpath(fn)

#define io_getfilename(fn) ((fn)?get_filename(fn):NULL)

/*
 return first name of drive from drive list from SEAL.INI file.
 (drv) is pointer to t_drive structure.
 This function use as findfirst function.
*/
l_text  drv_findfirst ( p_drives *drv );

/*
 return next name of drive from drive list and insert new t_drive
 structure into drv->next = new...

 example :

 t_drives d;

 drv_findfirst(&d);

 _while ( drv_findnext(&d) ) ;

 ...now (d) contains information about all drives defined in
 "seal.ini" file, and these drives are in p_drives drives GLOBAL variable.
*/
l_text  drv_findnext ( p_drives *drv );

/*
 add new drive to list of drives
*/
void    drv_set ( l_text namedrive, l_text path, l_int id );

/*
 return path of drive - or convert name of drive to real path.
 ! do not release memory of returned text !
*/
l_text  drv_fixname ( l_text drive );

/*
 convert path to name of drive.
 ! do not release memory of returned text !
*/
l_text  drv_fixreal ( l_text path );

l_text  drv_fixid ( l_int id );
l_text  drv_isdriver ( l_text path );


void    drv_init ( void );
void    drv_done ( void );

/*
 return all text from file (filename)
*/
l_text  file_gettext ( l_text filename );

/*
 returns true if text (text) was writen into the file (filename),
 otherwise returns false
*/
l_bool  file_puttext ( l_text filename, l_text text );


/* end of _iodir.h file */
#endif

