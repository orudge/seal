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
#include <files.h>
#include <vfile.h>
#include <io.h>
#include <fcntl.h>
#include <dir.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#include <lnk.h>

#ifndef BUFFER_FILE_COPY
#define BUFFER_FILE_COPY   2048
#endif


////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
void    free_tfile ( void *p )
{

   if ( p ) {

       if ( ((t_file*)p)->filename ) _free(((t_file*)p)->filename);
       if ( ((t_file*)p)->path ) _free(((t_file*)p)->path);

   };

   _free(p);

};
////////////////////////////////////////////////////////////////////////////////
void  io_cleartfile ( t_file *f )
{

   if ( f ) {

      if ( f->path ) _free(f->path);
      if ( f->filename) _free(f->filename);

   };

};
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
l_text  io_parentdir ( l_text path )
{
/*
   l_text endpath = NULL;
   l_text path2 = _strdup(path);

   if ( path2 ) {

      l_text last_slash = strrchr(path2, '/');

      if ( last_slash && !(*(last_slash+1)) ) {

          (*last_slash) = '\0';

          last_slash = strrchr(path2, '/');

      };

      endpath = stridup(path2, strsize(path2, last_slash));

   };

   _free(path2);

   return endpath;
  */
  l_text tmp = io_realpath ( path , "../" );
  l_text parent = ResolveFileName ( tmp );
  _free(tmp);
  return parent;

};
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
l_bool io_isfilename ( l_text file ) {
   if ( file ) {
       if ( !stricmp(file, "..") ) return false;
       if ( !stricmp(file, ".") ) return false;
       return true;
   };
   return false;
};
////////////////////////////////////////////////////////////////////////////////
l_bool io_isext ( l_text file ) {
   if ( file ) {
       if ( strchr(file, '*') || strchr(file, '?') )
           return true;
   };
   return false;
};
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
p_list filesop = NULL;

p_fileop fo_directory = NULL;
p_fileop fo_deffile   = NULL;
////////////////////////////////////////////////////////////////////////////////
p_fileop GetFileopForExp ( l_text ext ) {

  if ( !ext ) return NULL;

  if ( filesop && filesop->last ) {

    p_item x = filesop->first(filesop);
    p_item f = x;

    if  ( f )
    do {

       if ( !stricmp( FILEOP(x->rec)->ext , ext ) ) return FILEOP(x->rec);

       x = x->next;

    } while ( x != f );

  };

  return fo_deffile;
};
////////////////////////////////////////////////////////////////////////////////
p_fileop GetFileopForFile ( l_text file ) {
  if ( !file ) return NULL;

  if ( io_isfile(file) ) {
    return GetFileopForExp ( get_extension ( file ) );
  } else if ( io_isdir(file) ) {
    return fo_directory;
  } else {
    return NULL;
  };
};
////////////////////////////////////////////////////////////////////////////////
p_fileop GetFileopForFileAttrib ( l_text file, l_int attrib ) {
  if ( !file ) return NULL;

  if ( !( attrib & FA_DIREC ) ) {
    l_text   ext = get_extension ( file );
    p_fileop   o = GetFileopForExp ( ext );
    return o;
  } else {
    return fo_directory;
  };
};
////////////////////////////////////////////////////////////////////////////////
p_fileop fo_init ( p_fileop o,
                       l_text ext,
                       BITMAP *ico32,
                       BITMAP *ico16,
                       BITMAP *(*GetIcon)( l_text filename, l_int size, l_int *ownmem ),
                       l_text  description ) {

if ( !o ) return NULL;
o->ext = ext;
o->ico16 = ico16;
o->ico32 = ico32;
o->GetIcon = GetIcon;
o->description = description;
o->def = NULL;
o->actions = list_init(malloc(sizeof(t_list)), NULL, NULL);
return o;
};
////////////////////////////////////////////////////////////////////////////////
void fo_add_act ( p_fileop i,
                  p_fileact o,
                  l_text   cmd,
                  l_int   (*act) ( l_text filename, l_text args),
                  l_text   txt,
                  l_bool   def ) {

if ( !o || !i ) return;

o->cmd = cmd;
o->act = act;
o->txt = txt;
i->actions->insert(i->actions,o);
if ( def ) i->def = o;

};
////////////////////////////////////////////////////////////////////////////////
BITMAP*  get_icon_for_file_ex ( l_text filename, l_int attrib, l_int *ownmem, l_int size ) {
  p_fileop o = NULL; // faster because we know attrib of file
  BITMAP  *b = NULL;

  if ( !filename ) return NULL;

  o = GetFileopForFileAttrib(filename, attrib);

  if ( !o ) return NULL;

  if ( o->GetIcon ) {
    b = o->GetIcon ( filename, size, ownmem );
    if ( b ) return b;
  };

  if ( ownmem ) (*ownmem) = 0;

  if ( size < 32 )
    b = o->ico16;
  else
    b = o->ico32;

  return b;
};
////////////////////////////////////////////////////////////////////////////////
BITMAP*  get_icon_for_file ( l_text filename, l_int attrib, l_int *ownmem ) {
  return  get_icon_for_file_ex( filename, attrib, ownmem, 16 );
};
////////////////////////////////////////////////////////////////////////////////
BITMAP*  load_file_icon ( l_char *args, l_int size, l_int *ownmem ) {
  return  get_icon_for_file_ex( args, io_isdir(args)?FA_DIREC:0, ownmem, size );
};
////////////////////////////////////////////////////////////////////////////////
l_int    run_file_args ( l_text file, l_text args ) {
  p_fileop o;
  if ( !file ) return 0;

  o = GetFileopForFile(file);
  if ( !o ) return 0;

  if ( o->def ) {
    if ( o->def->act ) return o->def->act ( file, args );
    if ( o->def->cmd ) return DLXLoad(o->def->cmd , file ) ? 1 : 0 ;
  };
  return 0;
};
////////////////////////////////////////////////////////////////////////////////
l_int    run_file ( l_text file ) {
  return run_file_args ( file, NULL );
};
////////////////////////////////////////////////////////////////////////////////
l_int XDLLoad ( l_text file, l_text args ) {
   return DLXLoad(file,args);
};
////////////////////////////////////////////////////////////////////////////////
BITMAP *icon_icofile ( l_text filename, l_int size, l_bool *ownmem ) {
  if ( ownmem ) *(ownmem) = true;
  if ( filename ) {
    return load_ico(filename,size,( get_depth(screen) < 16 ) ? 16 : 256);
  };
  return NULL;
};
////////////////////////////////////////////////////////////////////////////////
void ini_ext_runner ( void ) {

  p_registry_search inf = (p_registry_search) malloc(sizeof(t_registry_search));

  p_fileop tmp = NULL;

  filesop = list_init(malloc(sizeof(t_list)), NULL, NULL);


  fo_directory = fo_init( malloc(sizeof(t_fileop)), NULL, IMG_DIR32, IMG_DIR16, NULL, TXT_DIRECTORY );

  fo_deffile = fo_init ( malloc(sizeof(t_fileop)), NULL, IMG_FILE32, IMG_FILE16, NULL, "Unknow file type" );

  tmp = fo_init ( malloc(sizeof(t_fileop)), "s2a", IMG_APP32, IMG_APP16, NULL, "Application file (executable)" );
  fo_add_act(tmp,malloc(sizeof(t_fileact)),NULL,&XDLLoad,"Run",true);
  filesop->insert(filesop,tmp);

  tmp = fo_init ( malloc(sizeof(t_fileop)), "xdl", IMG_APP32, IMG_APP16, NULL, "Dynamic file (executable)" );
  fo_add_act(tmp,malloc(sizeof(t_fileact)),NULL,&XDLLoad,"Load",true);
  filesop->insert(filesop,tmp);

 tmp = fo_init ( malloc(sizeof(t_fileop)), "sss", IMG_APP32, IMG_APP16, NULL, "Seal screen saver file (executable)" );
  fo_add_act(tmp,malloc(sizeof(t_fileact)),NULL,&XDLLoad,"Test",true);
  filesop->insert(filesop,tmp);


  tmp = fo_init ( malloc(sizeof(t_fileop)), "ln", IMG_FILE32, IMG_FILE16, &lnk_get_link_icon, "Link" );
  fo_add_act(tmp,malloc(sizeof(t_fileact)),NULL,&lnk_run_link,"Open",true);
  filesop->insert(filesop,tmp);

  tmp = fo_init ( malloc(sizeof(t_fileop)), "ico", IMG_FILE32, IMG_FILE16, &icon_icofile, "Icon file" );
  filesop->insert(filesop,tmp);

    if ( reg_find_first("system/filetypes", inf) ) do {
      l_text  key        = inf->name;
      l_text  key_loader = key_in_path(key,"loader");
      l_text  key_ico16  = key_in_path(key,"icon16");
      l_text  key_ico32  = key_in_path(key,"icon32");
      l_text  exp        = _strdup(inf->key->name);
      l_text  desc       = get_key(inf->name);
      l_text  loader     = NULL;
      l_text  ico16      = NULL;
      l_text  ico32      = NULL;
      BITMAP *icon16;
      BITMAP *icon32;

      if ( key_exists (key_loader) ) loader = get_key(key_loader);
      if ( key_exists (key_ico16) )  ico16 = get_key(key_ico16);
      if ( key_exists (key_ico32) )  ico32 = get_key(key_ico32);



      if ( stricmp(exp,"folder") )
        tmp = GetFileopForExp(exp);
      else
        tmp = fo_directory;

      if ( tmp == fo_deffile ) {

        if ( ico16 )
          icon16 = load_image(ico16);
        else
          icon16 = IMG_FILE16;
        if ( ico32 )
          icon32 = load_image(ico32);
        else
          icon32 = IMG_FILE32;

        tmp = fo_init ( malloc(sizeof(t_fileop)), exp, icon32, icon16, NULL, desc );
        filesop->insert(filesop,tmp);
      };

      if ( loader ) fo_add_act(tmp,malloc(sizeof(t_fileact)),loader,NULL,"Open",true);

      _free(key_ico16);
      _free(key_ico32);
      _free(key_loader);
      _free(ico16);
      _free(ico32);

    } while (reg_find_next(inf));

  _free(inf);
};
