/****************************************************************/
/*                                                              */
/*                        _iodir.c                              */
/*                                                              */
/*             source for IO operations under Seal              */
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


#include"_iodir.h"
#include<io.h>
#include<fcntl.h>
#include<dir.h>
#include<unistd.h>
#include<limits.h>
#include<string.h>
#include<sys/stat.h>
#include<dirent.h>

/* list of drives */
p_drives drives = NULL;


#ifndef BUFFER_FILE_COPY
#define BUFFER_FILE_COPY   2048  /* greater size is higher speed */
#endif

static l_char drv_buf[2] = {0, '\0'}; // fixed florianx

static p_drives drive_cur = NULL;

static int io_testlinks ( struct t_ffblk *ffblk )
{

    l_text ext = get_extension(ffblk->info.ff_name);

    strncpy(ffblk->ff_filename, ffblk->info.ff_name, 259);

    ffblk->ff_filename[259] = '\0';

      if ( ext && !stricmp(ext, "lnk") ) {

         ffblk->info.ff_attrib |= FA_LINK;

         fnsplit(ffblk->ff_filename, NULL, NULL, ffblk->info.ff_name, NULL);

         return ffblk->ff_attrib&FA_LINK?1:0;

      } else

      if ( ext && !stricmp(ext, "ldv") ) {

         ffblk->info.ff_attrib |= FA_LINKDIR;

         fnsplit(ffblk->ff_filename, NULL, NULL, ffblk->info.ff_name, NULL);

         return ffblk->ff_attrib&FA_LINKDIR?1:0;

      };


      return 1;

};


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
l_text io_nicelink ( l_text filename )
{
   if ( filename ) {

      l_text ext = get_extension(filename);

      if ( ext && (
           !stricmp(ext, "lnk") ||
           !stricmp(ext, "ldv")) ) {

         return stridup(filename, strlen(filename)-4); /* ignore .lnk or .ldv */

      } else _strdup(filename);

   };

   return NULL;
};


/*
 allocate new memory for t_file structure and set arguments to this

 structure from function arguments.
*/
t_file *new_tfile ( l_text path, l_text filename, l_int attrib, l_word time, l_word date, l_dword size )
{

   t_file *p = (t_file *)_malloc(sizeof(t_file));

   if ( p ) {

      clear_type(p, sizeof(t_file));

      p->path = _strdup(path);
      p->filename = _strdup(filename);
      p->attrib = attrib;
      p->time = time;
      p->date = date;
      p->size = size;

   };

   return p;

};


/*
 free context of (p) and (p) too.
*/
void    free_tfile ( void *p )
{

   if ( p ) {

       _free(((t_file*)p)->filename);
       _free(((t_file*)p)->path);

       _free(p);

   };


};


/*
 find first occurence of file pathname that is defined by

 atributions "attrib". Result put into t_ffblk structure.

 Return 0, if file was found, otherwise it return non-zero.

 This function is simple to findfirst function, but it support links

 files and it keep compatility with next versions.

 Please use this function for all file searches.
*/
int io_findfirst(const char *pathname, struct t_ffblk *ffblk, int attrib)
{
   l_int done = findfirst(pathname, &ffblk->info, attrib);

   ffblk->ff_attrib = attrib;

   while ( !done && !io_testlinks(ffblk) )

     done = findnext(&ffblk->info);

   return done;
};


/*
 find next occurence of file, that you defined in previous io_findfirst function.

 Return non-zero if no next file found, or ZERO  in succesfull.

 Use this function together with io_findfirst for all file finding.

 You will keep the compatibility.
*/
int io_findnext(struct t_ffblk *ffblk)
{
   l_int done = findnext(&ffblk->info);

   while ( !done && !io_testlinks(ffblk) )

     done = findnext(&ffblk->info);

   return done;

};


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

  callback  - in each file run this callback, by argument : filename
  o         - object that control this process, if object is closed ( done )
              also process is canceled.

*/

t_dirinfo io_foreach_file_ex ( l_text path, l_int flags, l_int (*callback)(), p_object o, l_dword *ind )
{

  t_dirinfo all;

  clear_type(&all, sizeof(t_dirinfo));

  if ( path ) {

    struct t_ffblk f;

    l_text files = io_realpath(path, "*.*");

    l_int done = io_findfirst(files, &f, FA_ALL);

    _while ( !done && obj_exist(o) != TAG_DISPOSE ) { /* multitasking while */

      if ( f.ff_filename && f.info.ff_attrib & FA_DIREC && stricmp(f.ff_filename, "..") &&
           stricmp(f.ff_filename, ".") ) { /* found directory */

        l_text f1 = io_realpath(path, f.ff_filename); /* get real filename */

        t_dirinfo dir = io_foreach_file_ex(f1, flags, callback, o, ind);

        if ( ind ) (*ind)++;

        all.files += dir.files;
        all.dirs += dir.dirs+1;
        all.size += dir.size;
        all.flags = dir.flags;

        _free(f1);

      } else if ( !(f.info.ff_attrib & FA_DIREC) ) { /* found file */

          l_text curfile = io_realpath(path, f.ff_filename);

          all.files++;

          if ( ind ) (*ind)++;

          if ( flags & DIF_SIZE ) {  /* get also sizes of files */

             l_int fl = _open(curfile, O_RDONLY);

             if ( fl != -1 ) {

                all.size += filelength(fl);

                close(fl);

             };

          };

          if ( callback ) callback(curfile);

          _free(curfile);

      };

      done = io_findnext(&f);

    };

    if ( !done ) all.flags |= DIF_HALT;

    _free(files);

  };

  if ( path && flags & DIF_DIRCALLBACK && callback ) { /* call calback for directory */

      if ( ind ) (*ind)++;

      callback(path);

  };

  return all;

};



/*
 copy directories from directory (path) to destination directory (dst).

 (o) and (ind) have same efect as in io_foreach_file_ex function ( see above ).

 return zero, if some error occured or zero if files was copyied.
*/

l_int io_foreach_file_copy ( l_text dst, l_text path, p_object o, l_dword *ind )
{

  l_int ok = 0;

  if ( path ) {

    struct t_ffblk f;

    l_text files = io_realpath(path, "*.*");

    l_int done = io_findfirst(files, &f, FA_ALL);

    ok = _io_copyfile(dst, path);

    if ( ind ) (*ind)++;

    _while ( !done && obj_exist(o) != TAG_DISPOSE ) { /* multitasking while */

      if ( f.ff_filename && f.info.ff_attrib & FA_DIREC && stricmp(f.ff_filename, "..") &&
           stricmp(f.ff_filename, ".") ) { /* found directory */

        l_text f1 = io_realpath(path, f.ff_filename); /* get real filename */
        l_text dst1 = io_realpath(dst, f.ff_filename); /* get real filename */

        ok = io_foreach_file_copy(dst1, f1, o, ind);

        _free(f1);
        _free(dst1);

      } else if ( !(f.info.ff_attrib & FA_DIREC) ) { /* found file */

          l_text curfile = io_realpath(path, f.ff_filename);
          l_text dstfile = io_realpath(dst, f.ff_filename);

          ok = _io_copyfile(dstfile, curfile);

          if ( ind ) (*ind)++;

          _free(curfile);
          _free(dstfile);

      };

      done = io_findnext(&f);

    };

    _free(files);

  };

  return ok;

};


/*
 rename file or directory (nameold) to (newnew). It finds if  nameold is

 directory or file and then rename it to new name (namenew).

 It return true if operation was succesfull, otherwise it return false.
*/

l_int io_rename ( l_text nameold, l_text namenew )
{

   if ( nameold && namenew )
      /* not same */
      if ( stricmp(nameold, namenew) ) {

          l_int t = rename(nameold, namenew);

          return abs(t);

      }
      else return -1;

   return -2;
};


/*
 remove ( delete ) file/directory ( f ).

 (ob), (ind) have same effect as in io_foreach_file_ex function ( see above ).

 return zero if some error occured, else return true.

 PLEASE, CALL THIS FUNCTION FOR COPYING (files/dirs), IT CONTROLS ALL !
*/

l_int  io_removefile ( p_object ob, t_file *f, l_dword *ind )
{

    if ( f ) {

        t_dirinfo ok;

        l_text path = io_realpath(f->path, f->filename);

        if ( f->attrib & FA_DIREC )

             ok = io_foreach_file_ex(path, DIF_DIRCALLBACK, &_io_removefile, ob, ind);

        else {

            l_bool ok2 = _io_removefile(path);

            if ( ind ) (*ind)++;

            _free(path);

            return ok2;
        };

        _free(path);

        return (!(ok.flags & DIF_HALT));

    };

    return false;

};


/*
  copy file/directory (src) into destination file/directory (dst)

  - src   file for copying
  - dst   destination of file, that will be created and filled by context of src
  - ob    object ( t_view ), that's controle this process, if t_view (ob) is
          closed, than process is stoped and return false.

  return true if success, otherwise false

*/

l_int  io_copyfile ( p_object o, t_file *dst, t_file *src, l_dword *ind )
{

  if ( src && dst ) {

      l_text srcpath = io_realpath(src->path, src->filename);
      l_text dpath = io_realpath(dst->path, dst->filename);
      l_text dstpath = io_realpath(dpath, src->filename);

      l_int ret = io_foreach_file_copy(dstpath, srcpath, o, ind);

      _free(dpath);
      _free(dstpath);
      _free(srcpath);

      return ret;

  };

  return 0;

};


/*
  get number of files in directory f, or return 1 if it's only one file

  number of files is returned in *ind pointer to l_dword

  - f   file for removing
  - ob  object ( t_view ), that's controle this process, if t_view (ob) is
        closed, than process is stoped and return false.

  return true if success, otherwise false
*/

l_int  io_numberfile ( p_object ob, t_file *f, l_dword *ind )
{

    if ( f ) {

        if ( f->attrib & FA_DIREC ) {

             l_text path = io_realpath(f->path, f->filename);

             io_foreach_file_ex(path, 0, NULL, NULL, ind);

             _free(path);

        } else {

            if ( ind ) (*ind)++;

        };


        return true;

    };

    return false;

};


/*
 remove file or directory (src). It find if (src) is file or directory and then

 remove it. Return true if succesfull, else return false.

 It remove only empty directory. It's low-level deleting.

 Use io_removefile(...) you will see later for deleting.
*/
l_int   _io_removefile ( l_text file )
{

   if ( remove(file) ) /* error - it's maybe directory */

       return !rmdir(file);

   return true; /* succes removing of file */

};


/*
  copy file from (src) to (dst)

  make new file dst and copy data from src into the new ( dst ) file.

  if (src) is directory it creat directory (dst)

  return true if success, else return false

*/

l_int   _io_copyfile ( l_text dst, l_text src )
{

   if ( !dst || !src || !stricmp(dst, src) ) return false;

   if ( io_isdir(src) ) { /* (src) is directory */

      return mkdir(dst, S_IWUSR/* ignored under DOS */);

   } else { /* (src) is file */

      static l_char buffer[BUFFER_FILE_COPY];

      FILE *s = fopen(src, "rb");
      FILE *d = fopen(dst, "wb");

      l_bool ok = false;

      if ( s && d ) {

         ok = true;

         _while ( !feof(s) ) { /* multitasking while */

               l_dword size = fread(buffer, sizeof(l_char), BUFFER_FILE_COPY, s);

               fwrite(buffer, sizeof(l_char), size, d);

         };

      };

      fclose(s);
      fclose(d);

      return ok;

   };

   return false;

};


/*
 same as mkdir(dir, ...), but it keeps compatibility.
*/
l_bool  io_mkdir ( l_text dir )
{
   return (!mkdir(dir, S_IWUSR));
};


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
l_text  io_uniquedir ( l_text path )
{
   l_char dir[9] = "DIR0";
   l_int  i = 0;
   l_text rp = io_realpath(path, dir);
   l_text ret = NULL;

   while ( i < 99999 && io_isdir(rp) ) { /* if directory exist */

       itoa(i, &dir[3], 10);

       i++;

       _free(rp);

       rp = io_realpath(path, dir);

   };

   if (i < 99999) ret = _strdup(dir);

   _free(rp);

   return ret;
};



/*
  check if file1 is same as file2

  if it's, it return true, otherwise false
*/
l_bool  io_issame ( l_text file1, l_text file2 )
{
  l_bool ret = false;

  l_text f1;
  l_text f2;

  l_int  x1;
  l_int  x2;

  if ( !file1 || !file2 ) return false;

  f1 = _strdup(file1);
  f2 = _strdup(file2);

  x1 = strlen(f1)-1;
  x2 = strlen(f2)-1;

  if ( *(f1+x1) == '\\' || *(f1+x1) == '/' ) *(f1+x1) = '\0';
  if ( *(f2+x2) == '\\' || *(f2+x2) == '/' ) *(f2+x2) = '\0';

  if ( f1 && f2 && !stricmp(f1, f2) )

       ret = true;

  _free(f1);
  _free(f2);

  return ret;

};


/*
  check if file is directory or file.

  return true if it's file, else if directory
*/
l_bool  io_isfile ( l_text file )
{

   if ( access(file, D_OK) && !access(file, F_OK) ) return true;

   return false;

};


/*
 return real parent directory of the path.

 example :

 l_text  x = io_parentdir("c:/seal/");
 l_text  y = io_parentdir("c:/seal");

 x = c:/
 y = c:/
*/
l_text  io_parentdir ( l_text path )
{

   l_text endpath = NULL;
   l_text path2 = _strdup(path);

   if ( path2 ) {

      l_text last_slash = strrchr(path2, '/');

      if ( last_slash && !(*(last_slash+1)) ) { /* end of path is slash */

          (*last_slash) = '\0';

          last_slash = strrchr(path2, '/');

      };

      endpath = stridup(path2, strsize(path2, last_slash));

   };

   _free(path2);

   return endpath;

};



/*
 return true (file) is directory and it exist, else it return false.
*/
l_bool  io_isdir ( l_text file )
{

   if ( !access(file, D_OK) ) return true;

   return false;

};


/*
 return true if file or directory exist or false if not.
*/
l_bool  io_exist ( l_text file )
{

   l_bool ok = false;

   l_text odir = getcwd(NULL, PATH_MAX);

   if ( !access(file, R_OK) ) ok = true;

   if ( !chdir(file) ) ok = true;

   chdir(odir);

   return ok;

};


/*
  get real name of file (file) that we find in path (path)and return new string.

  This function control if '/' was after (path) or not. If not, it will add it.

  example :

  l_text all1path = io_realpath("c:/seal/", "seal.exe");
  l_text all2path = io_realpath("c:/seal", "seal.exe");

  all1path contains "c:/seal/seal.exe"
  all2path contains "c:/seal/seal.exe"
*/

l_text  io_realpath ( l_text path, l_text file )
{

  if ( path ) {

      l_int s = strlen(path);

      if ( s && *(path+s-1) == '/' || *(path+s-1) == '\\' ) {

            if ( file ) return set_format_text(NULL, "%s%s", path, file);
            else return strdup(path);

      } else if ( s )

         if ( file ) return set_format_text(NULL, "%s/%s", path, file);
         else return set_format_text(NULL, "%s", path);

  } else
  {

     if ( file ) return _strdup(file);
  };

  return NULL;
};


/*
 clear memory for pointer t_file, same as

 clear_type(f, sizeof(t_file)) or memset(f, 0, sizeof(t_file))
*/
void  io_cleartfile ( t_file *f )
{

   if ( f ) {

      _free(f->path);
      _free(f->filename);

   };

};


/*
 convert filename to t_file structure and return this structure.

 Filename is real path, it means it looks like this : path+'/'+file
*/
t_file  io_filetotfile ( l_text filename )
{
   l_text path  = io_path(filename);
   l_text file  = io_filename(filename);
   t_file f;
   clear_type(&f, sizeof(t_file));
   f.path = path;
   f.filename = file;

   return f;
};


/*
 return true if filename (file) has extension same to (ext)
*/
l_bool  io_isas ( l_text file, l_text ext )
{
    if ( file && ext ) {

         l_text ext1 = get_extension(file);

         if ( ext1 && !stricmp(ext, ext1) ) return true;

    };

    return false;
};


/*
 return link to directory in (*link) and icon filename (*icon) from ".ldv" file (file).
*/

void  io_linkedpath_ex ( l_text file, l_text *link, l_text *icon )
{
   if ( link ) (*link) = getini_fromfile(file, "definition", "link");

   if ( icon ) (*icon) = getini_fromfile(file, "definition", "icon32");
};


/*
 return link to file in (*link) and icon filename (*icon) from ".lnk" file (file).
*/

void  io_linkedfile_ex (  l_text file, l_text *link, l_text *icon )
{
   if ( link ) (*link) = getini_fromfile(file, "definition", "link");

   if ( icon ) (*icon) = getini_fromfile(file, "definition", "icon32");
};


/*
 return link from ".lnk" file (file). It return new filename where the

 physical file is.

 example:

 l_text shfile = io_linkedfile("c:/seal/desktop/hello.lnk");

 shfile contains "c:/seal/image.exe" f.e.
*/
l_text  io_linkedfile ( l_text file )
{
   l_text link = NULL;

   io_linkedfile_ex(file, &link, NULL);

   return link?link:_strdup(file);
};


/*
 return link from ".ldv" file (file).

 It returns new path from this link.

 Seal has also shortcuts to directories not only to files.

 example:

 l_text shpath = io_linkedpath("c:/seal/desktop/pc/hd-c.ldv");

 shpath contains "c:/"
*/
l_text  io_linkedpath ( l_text file )
{
   l_text path = NULL;

   io_linkedpath_ex(file, &path, NULL);

   return path?path:_strdup(file);
};


/*
  set link ( reference ) to directory ( link ) when you use file (file).
*/
void  io_set_linkedpath ( l_text file, l_text link )
{
   io_set_linkedpath_ex(file, link, NULL);
};


/*
  set link ( reference ) to file ( link ) when you use file (file).

  icon for this directory will be the name of the file (icon), where icon is placed
*/
void  io_set_linkedfile ( l_text file, l_text link )
{
   io_set_linkedfile_ex(file, link, NULL);
};


/*
  set link ( reference ) to directory ( link ) when you use file (file).

  icon for this directory will be the name of the file (icon), where icon is placed
*/
void  io_set_linkedfile_ex ( l_text file, l_text link, l_text icon )
{
   if ( link )

      setini_tofile(file, "definition", "link", link, INI_STRING);

   if ( icon )

      setini_tofile(file, "definition", "icon32", icon, INI_STRING);
};


/*
  set link ( reference ) to file ( link ) when you use file (file).

  icon for this directory will be the name of the file (icon), where icon is placed
*/
void  io_set_linkedpath_ex ( l_text file, l_text link, l_text icon )
{
   if ( link )

      setini_tofile(file, "definition", "link", link, INI_STRING);

   if ( icon )

      setini_tofile(file, "definition", "icon32", icon, INI_STRING);
};


/*
 return true if name of file is extension or false if not.

 example:

 l_bool ext = io_isext("file.*");
 l_bool file = io_isext("file.bmp");

 ext is true.
 file is false.
*/
l_bool io_isext ( l_text file )
{

   if ( file ) {

       if ( strchr(file, '*') || strchr(file, '?') )

           return true;

   };

   return false;

};


/*
 return true if file is file or directory name, not ".", ".." etc., else return false.
*/
l_bool io_isfilename ( l_text file )
{

   if ( file ) {

       if ( !stricmp(file, "..") ) return false;
       if ( !stricmp(file, ".") ) return false;

       return true;

   };

   return false;

};


static p_drives drv_info ( l_text drive, l_text rdr, l_int id )
{
   p_drives d = drives;

   while ( d ) {

      if ( (drive && d->name && !stricmp(drive, d->name)) ||
           (rdr && d->path && !stricmp(rdr, d->path)) || id == d->id )

          return d;

      d = d->next;

   };

   return NULL;
};


l_text  drv_findfirst ( p_drives *drv )
{
   if ( drv ) *drv = drives;

   if ( drives ) return drives->name;

   return NULL;
};


l_text  drv_findnext ( p_drives *drv )
{
   if ( drv ) {

     (*drv) = (*drv)->next;

     return (*drv)?(*drv)->name:NULL;

   };

   return NULL;
};


l_text  drv_fixname ( l_text drive )
{

   p_drives d = drv_info(drive, NULL, -1);

   if ( d ) return d->path;

   return drive;

};


l_text  drv_fixreal ( l_text path )
{

   p_drives d = drv_info(NULL, path, -1);

   if ( d ) return d->name;

   return path;

};


l_text  drv_isdriver ( l_text path )
{

   p_drives d = drv_info(NULL, path, -1);

   if ( d ) return d->name;

   return NULL;

};


l_text  drv_fixid ( l_int id )
{

   p_drives d = drv_info(NULL, NULL, id);

   if ( d ) return d->name;

   drv_buf[0] = id+'a';

   return (l_text)drv_buf;

};


void   drv_set ( l_text namedrive, l_text path, l_int id )
{

   if ( namedrive && path ) {

      p_drives d = drv_info(namedrive, NULL, -1);
      l_bool   ok = false;

      if ( d ) {

         ok = true;
         _free(d->name);
         _free(d->path);

      } else d = (p_drives)_malloc(sizeof(t_drives));

      if ( d ) {

         clear_type(d, sizeof(t_drives));
         d->name = _strdup(namedrive);
         d->path = _strdup(path);
         d->id = id;

         if ( !ok ) {

            d->next = drives;

            drives = d;

         };

      };

   };

};


/*
 return all text from file (filename)
*/
l_text  file_gettext ( l_text filename )
{

  FILE *f = fopen(filename, "rt");

  if ( f ) { /* file exist */

      l_long size = filelength(fileno(f));

      l_text text = _malloc(size+1); /* allocate size for file */

      if ( text ) {

         clear_type(text, size+1); /* clear all to zero */

         fread(text, size, 1, f);

      };

      fclose(f);

      return text;

  };

  return NULL; /* file not exist */

};


/*
 returns true if text (text) was writen into the file (filename),

 otherwise returns false
*/
l_bool  file_puttext ( l_text filename, l_text text )
{

  FILE *f = fopen(filename, "wt");

  if ( f ) { /* file exist */

      if ( text ) {

         fwrite(text, strlen(text), 1, f);

      };

      fclose(f);

      return true;

  };

  return false;
};


void   drv_init ( void )
{

  ini_data *drvs = getinidata_fromfile ( INI_MAINFILE, INI_DRIVES );

  DEBUG_printf("\n...initializing drivers from file '%s' and structure '%s'\n", INI_MAINFILE, INI_DRIVES); /* error message, if some occures */

  if ( drvs ) {

    l_int lines = getini_linenum(drvs);

    while ( lines > 0 ) {

      l_char *drvname = NULL;
      l_char *path = NULL;

      path = getini_line(&drvname, drvs, lines);

      if ( path || drvname )

          DEBUG_printf(" - found drive '%s' for path '%s'\n", drvname, path); /* error message, if some occures */

      drv_set(drvname, path, 0);

      _free(path);
      _free(drvname);

      lines--;
    };

  };

  _free(drvs);

};


void   drv_done ( void )
{

  while ( drives ) {

     p_drives o = drives->next;

     _free(drives->name);
     _free(drives->path);
     _free(drives);

     drives = o;

  };

  drives = NULL;

};


