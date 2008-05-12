/****************************************************************/
/*                                                              */
/*                          exedlx.c                            */
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

#include<stdio.h>
#include<io.h>
#include<string.h>
#include<stdlib.h>
#include<dir.h>
#include<unistd.h>
#include<process.h>
#include"allegro.h"
#include"object.h"
#include"view.h"
#include"dataini.h"
#include"dlx.h"
#include"exedlx.h"
#include"drivers.h"
#include"list.h"
#include"_iodir.h"
#include"program.h"

extern char* _LIBEXPORTTABLE[]; /* declaration must be in main exe file */

p_ext_runners  *ext_runners = NULL;
p_ico_loaders   ico_loaders = NULL;


l_int (*read_set_file)( l_char *_filename) = &_read_set_file;
l_int (*dos_command) ( l_text newpath, l_text command ) = &_dos_command;
l_int (*dos_run) ( l_text newpath, l_text command ) = &_dos_run;


l_int  _read_set_file ( l_char *_filename )
{

  l_int   x = 0;
  l_text  buffer = file_gettext(_filename);
  l_text  ptr = buffer;

  if ( !buffer ) return 0;

  DLXImport(_LIBEXPORTTABLE);

  while ( ptr ) {

      if ( (ptr = strchr(ptr, '<')) ) {

        l_text sptr = strchr(ptr, '>');

        if ( sptr ) {

          l_text filedlx = (l_text)_malloc(sptr-ptr);

          if ( filedlx ) {

            memcpy(filedlx, ptr+1, (sptr-ptr)-1);

            filedlx[(sptr-ptr)-1] = '\0';

            run_file(filedlx); /* run file */

            _free(filedlx);

          };

          ptr = sptr;

        };

      };

  };

  _free(buffer);

  return 1;
};



void   free_runners ( void )
{
  if ( ext_runners ) {

    p_ext_runners *v = ext_runners;

    while ( *v ) {

      p_ext_runners n = *v;
      v++;
      if ( n ) {
        _free(n->app);
        _free(n->ext);
      };
      _free(n);

    };

    _free(ext_runners);
    ext_runners = NULL;
  };
};


void   put_into_runners ( l_char *ext, l_char *app )
{
  l_int n = 0;

  if ( !app || !ext ) return;

  if ( ext_runners ) {

    p_ext_runners *v = ext_runners;

    while ( *v ) { /* get max number of runners */
      n++;
      v++;
    };

  };

  ext_runners = (p_ext_runners*)_realloc(ext_runners, (n+2)*sizeof(p_ext_runners));

  if ( ext_runners ) {

    p_ext_runners x = (p_ext_runners)_malloc(sizeof(t_ext_runners));

    if ( x ) {

      memset(x, 0, sizeof(t_ext_runners));
      x->ext = _strdup(ext);
      x->app = _strdup(app);

    };

    ext_runners[n] = x;
    ext_runners[n+1] = NULL;

  };

};


l_char*  get_from_runners ( l_char *ext ) {

  if ( ext_runners ) {

    p_ext_runners *v = ext_runners;

    while ( *v ) {

      if ( (*v)->ext && ext && !stricmp((*v)->ext, ext) ) /* extensions are same */

         return (*v)->app;

      v++;
    };

  };

  return NULL;

};


/* icon loaders */

BITMAP*  _load_exe_icon ( l_char *args, l_int size, l_int *ownmem )
{

   return NULL;
};


BITMAP*  _load_ldv_icon ( l_char *args, l_int size, l_int *ownmem )
{

   BITMAP *ic = NULL;

   l_text def32 = getini_fromfile(args, "definition", "icon32");
   l_text def16 = getini_fromfile(args, "definition", "icon16");

   if ( ownmem ) *(ownmem) = 0;

   if ( !def32 ) def32 = def16;
   if ( !def16 ) def16 = def32;

   if ( size > 16 && def32 ) { /* it's 32 icon */
   /* 32 icon */

     if ( *def32 == '%' ) /* system icon */

         ic = icon_sysget(icon_getid(def32+1), size);

   } else
   if ( size <= 16 && def16 ) { /* it's 16 icon */
   /* 16 icon */

     if ( *def16 == '%' ) { /* system icon */

         ic = icon_sysget(icon_getid(def16+1), size);
     };

   };

   return ic;

   _free(def16);
   _free(def32);

};


BITMAP*  _load_lnk_icon ( l_char *args, l_int size, l_int *ownmem )
{

   BITMAP *ic = NULL;

   l_text def32 = getini_fromfile(args, "definition", "icon32");
   l_text def16 = getini_fromfile(args, "definition", "icon16");

   if ( ownmem ) *(ownmem) = 0;

   if ( !def32 ) def32 = def16;
   if ( !def16 ) def16 = def32;

   if ( size > 16 && def32 ) { /* it's 32 icon */
   /* 32 icon */

     if ( *def32 == '%' ) /* system icon */

         ic = icon_sysget(icon_getid(def32+1), size);

   } else
   if ( size <= 16 && def16 ) { /* it's 16 icon */
   /* 16 icon */

     if ( *def16 == '%' ) { /* system icon */

         ic = icon_sysget(icon_getid(def16+1), size);
     };

   };

   return ic;

   _free(def16);
   _free(def32);

};





BITMAP*  load_file_icon ( l_char *args, l_int size, l_int *ownmem )
{

  p_ico_loaders p = ico_loaders;

  if ( ownmem ) *(ownmem) = 0;

  if ( args )

     while ( p ) {

        l_text ext = get_extension(args);

        if ( ext && p->ext && !stricmp(ext, p->ext) )

           return p->load_icon(args, size, ownmem);

        p = p->next;

     };

  return NULL;

};


void     add_to_loadfileicon ( l_char *ext, BITMAP *(*load_icon)(l_text filename, l_int size, l_int *ownmem) )
{
  p_ico_loaders p = (p_ico_loaders)_malloc(sizeof(t_ico_loaders));

  if ( p ) {

    clear_type(p, sizeof(t_ico_loaders));

    p->ext = _strdup(ext);
    p->load_icon = load_icon;
    p->next = ico_loaders;

    ico_loaders = p;

  };

};


void     init_loadfileicon ( void )
{
  add_to_loadfileicon("exe", &_load_exe_icon);
  add_to_loadfileicon("dlx", &_load_exe_icon);
  add_to_loadfileicon("ldv", &_load_ldv_icon);
  add_to_loadfileicon("lnk", &_load_lnk_icon);
};


void     free_loadfileicon ( void )
{

   while ( ico_loaders ) {

     p_ico_loaders p = ico_loaders->next;

     _free(ico_loaders->ext);
     _free(ico_loaders);

     ico_loaders = p;

   };

   ico_loaders = NULL;

};



BITMAP*  get_icon_for_file_ex ( l_text filename, l_int attrib, l_int *ownmem, l_int size )
{

   l_text  file = io_getfilename(filename);
   BITMAP *ex   = NULL;

   if ( ownmem ) *(ownmem) = 0;

   if ( file && !stricmp(file, DRIVE_DESKTOP) ) /* desktop */

      ex = icon_desktop(size);

   else

   if ( file && !stricmp(file, DRIVE_PC) ) /* pc */

      ex = icon_mycomputer(size);

   else

   if ( attrib & FA_DIREC ) ex = icon_folder(size);

   else

      ex = load_file_icon(filename, size, ownmem);

   return ex;

};


BITMAP*  get_icon_for_file ( l_text filename, l_int attrib, l_int *ownmem )
{
   return get_icon_for_file_ex(filename, attrib, ownmem, system_item_size);
};


p_list  get_args ( l_char *args )
{

   p_list list = list_init(_malloc(sizeof(t_list)), NULL, DAT_TEXT);

   if ( list )

   while ( args ) {

      l_text arg = args;

      args = strchr(args, ' ');

      if ( args ) {

         if ( args != arg )

               list->insert(list, stridup(arg, strsize(arg, args)));

         while ( *args && *(args++) != ' ' );

      } else {

         list->insert(list, _strdup(arg));

      };

   };

   return list;

};


l_int  run_file_args ( l_text file, l_char *args )
{
  l_char *ext = get_extension(file);
  l_char *app = get_from_runners(ext);
  _free(ext);

  if ( app ) {

    hdlx_t i = 0;

    if ( app && !stricmp(app, EXT_LIBOSX) ) /* if args is file known by os-x library */

      i = DLXLoad(file, args);

    else {

      l_text arg1 = set_format_text(NULL, "%s %s", file, args);

      i = DLXLoad(app, arg1);

      if ( !i ) /* try to run as DOS file */

          i = dos_run(arg1, arg1);

      _free(arg1);

     };

    return i?1:0;
  };

  return 0;
};


l_int  run_file ( l_char *args )
{
  l_char *ext = get_extension(args);
  l_char *app = get_from_runners(ext);

  if ( app ) {

    hdlx_t i = 0;

    if ( is_dos_executable(args) ) { /* can execute in MS-DOS mode ? */

       i = dos_run(NULL, args);

       return i;

    };

    if ( app && !stricmp(app, EXT_LIBOSX) ) /* if args is file known by os-x library */

      i = DLXLoad(args, NULL);

    else

      i = DLXLoad(app, args);

    return i?1:0;
  };

  return 0;
};


void  init_ext_runners ( void )
{
  ini_data *exts = getinidata_fromfile ( INI_MAINFILE, INI_EXTRUNNERS );

  DEBUG_printf("\n...initializing extension runners from file '%s' and structure '%s'\n", INI_MAINFILE, INI_EXTRUNNERS); /* error message, if some occures */

  if ( exts ) {

    l_int lines = getini_linenum(exts);

    while ( lines > 0 ) {

      l_char *app = NULL;
      l_char *ext = NULL;

      app = getini_line(&ext, exts, lines);

      if ( app || ext )

          DEBUG_printf(" - found extension runner '%s' for extension '%s'\n", app, ext); /* error message, if some occures */

      put_into_runners(ext, app);

      _free(app);
      _free(ext);

      lines--;
    };

  };

  _free(exts);

  init_loadfileicon();

};


void  done_ext_runners ( void )
{

  while ( *ext_runners ) {

    p_ext_runners *o = ext_runners+1;

    _free((*ext_runners)->app);
    _free((*ext_runners)->ext);
    _free((*ext_runners));

    ext_runners = o;

  };

  free_loadfileicon();

  ext_runners = NULL;
};


l_int  is_dos_executable ( l_text file )
{

  FILE *f = fopen(file, "rt");

  if ( f ) {

      l_bool ok   = 0;
      l_char h[2] = {'\0', '\0'};

      fread(h, 2, 1, f); /* read MZ header */

      if ( h[0] == 'M' && h[1] == 'Z' ) /* it's MZ */

           ok = 1;

      fclose(f);

      return ok;
  };

  return 0;

};


l_int  _dos_command ( l_text newpath, l_text command )
{
    l_int   err = 0;

    l_text  path   = getcwd(NULL, IO_DIR_LIMIT);
    l_int   oldsys = __system_flags;
    l_text  p = io_path(newpath);
    l_text  f = io_filename(command);

    if ( p ) chdir(p);

    if ( is_dos_executable(f) ) { /* can execute in MS-DOS mode ? */

         screen_halt();

         safe_timer_halt();

         __system_flags |= __system_call_cmdproc;

         err = system(f);

         __system_flags = oldsys;

         screen_reload();

         safe_timer_reload();

    } else {

       err = (DLXLoad(command, NULL)==0?-1:0);

    };

    if ( path && newpath ) chdir(path);

    _free(p);
    _free(f);

    return (err==-1?0:1);
};


l_int  _dos_run ( l_text newpath, l_text command )
{

    l_int   err = 0;
    l_text  path   = getcwd(NULL, IO_DIR_LIMIT);
    l_int   oldsys = __system_flags;
    l_text  p = io_path(newpath);
    l_text  f = io_filename(command);

    if ( p ) chdir(p);

    if ( !io_isas(f, "exe") || !is_dos_executable(f) ) {

         screen_halt();

         safe_timer_halt();

         err = spawnlp(P_WAIT, f, f, 0);

         if ( path && newpath ) chdir(path);

         screen_reload();

         safe_timer_reload();

    } else {

       err = DLXLoad(command, NULL)==0?-1:0;

    };

    if ( path && newpath ) chdir(path);

    _free(p);
    _free(f);

    return (err==-1?0:1);
};

