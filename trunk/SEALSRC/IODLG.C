/****************************************************************/
/*                                                              */
/*                           iodlg.c                            */
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


#include"allegro.h"
#include"seal.h"
#include"scroller.h"
#include"app.h"
#include"dialogs.h"
#include"iodlg.h"
#include<dir.h>
#include<dos.h>
#include<unistd.h>


static DATAFILE *datfile = NULL;

/*
  icons for iods
*/

#define IO_ICONS        0
#define IO_NOIMAGE      0
#define ILOADIMG(i)     GET_DATA(datfile, i)


p_filelistbox  (*filelistbox_init) ( p_filelistbox o, t_rect r, l_int cols, l_int flags, l_text path, l_text file, l_int attrib, void (*file_sel)() ) = &_filelistbox_init;
p_filehistory  (*filehistory_init) ( p_filehistory o, t_rect r, p_list list, l_int limit, l_int flags, p_filelistbox filebox ) = &_filehistory_init;
p_filetextline (*filetextline_init) ( p_filetextline o, t_rect r, l_int limit, l_int flags, p_filelistbox filebox, p_filehistory filehistory ) = &_filetextline_init;
p_dirhistory   (*dirhistory_init) ( p_dirhistory o, t_rect r, l_int limit, l_int flags, p_filelistbox filebox ) = &_dirhistory_init;

l_text  (*file_dialog) ( l_text caption, l_text bname, l_text path, l_text file, p_list hislist, l_int attrib, l_int flags, void (*file_sel)() ) = &_file_dialog;


/* t_filelistbox_item functions */


/*

  menu_copy_where

  - menu that ask for copying, where we want to copy our data

  - 1. to file
  - 2. to directory, where file is placed

*/

l_dword  menu_file_copy_where ( l_text filename )
{

   l_text of = io_filename(filename);
   l_text df = io_path(filename);
   l_text copytofile = set_format_text_nice(NULL, -39, "%s %s", TXT_COPYTOFILE, of);
   l_text copytodir  = set_format_text_nice(NULL, -39, "%s %s", TXT_COPYTODIRECTORY, df);
   l_text copytofileinfo = set_format_text_nice(NULL, -39, "%s %s", TXT_INFOCOPYTOFILE, of);
   l_text copytodirinfo  = set_format_text_nice(NULL, -39, "%s %s", TXT_INFOCOPYTODIRECTORY, df);

   p_menu p = new_menu(

                 new_menu_item_ex(copytofile, NULL, 0, MSG_COPYTOFILE, true,
                                  copytofileinfo, MIF_NONE, font_system_bi, NULL, NULL, 0,
                 new_menu_line(
                 new_menu_item_ex(copytodir, NULL, 0, MSG_COPYTODIR, true,
                                  copytodirinfo, MIF_NONE, font_system_bd, NULL, NULL, 0,
                NULL)))
             );

   _free(of);
   _free(df);
   _free(copytodir);
   _free(copytofile);
   _free(copytodirinfo);
   _free(copytofileinfo);

   if ( p ) {

         t_rect   r = rect_assign(mouse->where.x-5, mouse->where.y-5, 0, 0);

         p_object menu = OBJECT(menuview_init(
                                _malloc(sizeof(t_menuview)),
                                r,
                                p)
                         );

         return desktop->execute_view(desktop, VIEW(menu));

      };

  return MSG_CANCEL;
};



/*
  properties functions

  - linkdir
  - file
  - directory
  - these functions are part of t_filelistbox_item

  - return true if something was changed, otherwise return false
*/


/*
  - show file properties from filename = real path to file
*/
l_bool  file_properties ( l_text filename )
{

   struct  ftime t;
   l_word  attrib = 0;
   l_dword size   = 0;
   FILE   *f      = fopen(filename, "rb");


   if ( f ) { /* file exist */

      l_text file = io_getfilename(filename);
      l_text path = stridup(filename, strsize(filename, file));

      _dos_getfileattr(filename, &attrib);

      getftime(fileno(f), &t);

      size = filelength(fileno(f));

      fclose(f);

      msgboxsize(400, MW_INFO, MB_OK, "%s: %s\n\n%s: %s\n\n%s: %i (bytes) = %.2f (kbytes)\n\n%s: %i.%i.%i\n\n%s: %ih:%imin:%isecs",
                 TXT_FILENAME, file,
                 TXT_PATH, path,
                 TXT_SIZE, size, ((double)size/(double)1024),
                 TXT_DATE, t.ft_day, t.ft_month, 1980+t.ft_year,
                 TXT_TIME, t.ft_hour, t.ft_min, (double)t.ft_tsec,
                 TXT_ATTRIB, NULL);

      _free(path);

   };

   return false;

};


/*
  - show directory properties from directory = real path to dir

*/
l_bool  dir_properties ( l_text filename )
{

  l_text  dir = io_getfilename(filename);
  l_text  path = stridup(filename, strsize(filename, dir));
  l_word  attrib = 0;

  struct  ftime t;

  p_object i = show_info(0, MW_INFO, "%s...", TXT_PLEASEWAIT); /* show info "Please wait..." */

  t_dirinfo dinfo = io_foreach_file(filename, DIF_SIZE, i, NULL); /* count files */

  l_text    whatbytes = "bytes";
  double    size = (double)dinfo.size;
  l_text    status = (dinfo.flags & DIF_HALT) ? TXT_HALTED : TXT_DONE;

  hide_info(i); /* hide info "Please wait... " */

  if ( dinfo.size > (l_dword)1024*100 ) { /* Kb */

      whatbytes = "Kb";
      size = size/(double)1024;

  };

  if ( dinfo.size > (l_dword)1024*1000 ) { /* Mb */

      whatbytes = "Mb";
      size = size/1024;

  };

  msgboxsize(400, MW_INFO, MB_OK, "%s: %s\n\n%s: %s\n\n%s: %i\n\n%s: %i\n\n%s: %.2f (%s)\n\n%s: (%s)",
                  TXT_DIRNAME, dir,
                  TXT_PATH, path,
                  TXT_NUMBEROFFILES, dinfo.files,
                  TXT_NUMBEROFDIRECTORIES, dinfo.dirs,
                  TXT_SIZE, size, whatbytes,
                  TXT_PROCESSSTATUS, strlwr(status),
                  TXT_ATTRIB, NULL);

  _free(path);

  return false;

};


/*
  show linkdir properties
*/
l_bool  linkdir_properties ( l_text filename )
{
  return link_properties(filename, TXT_LINKTODIRECTORY, false);
};


/*
  show linkfile properties
*/
l_bool  linkfile_properties ( l_text filename )
{
  return link_properties(filename, TXT_LINKTOFILE, true);
};


/*
  link properties
*/
l_bool  link_properties ( l_text filename, l_text linkinfo, l_bool filelink )
{

   t_rect r;

   l_bool ret = false;

   p_appwin w;

   p_textline l;
   p_textline il;
   p_stattext s;
   p_button   ok;
   p_button   close;

   r = rect_assign(0, 0, 400, 160);

   w = appwin_init(_malloc(sizeof(t_appwin)), r, TXT_INFO, 0, 0, NULL);

   if ( w ) {

      l_text ref; /* get link to directory */
      l_text icon; /* get link to directory */

      t_rect x;

      if ( filelink )

         io_linkedfile_ex(filename, &ref, &icon);

      else

         io_linkedpath_ex(filename, &ref, &icon);


      VIEW(w)->align |= TX_ALIGN_CENTER; /* center window */

      r = VIEW(w)->size_limits(VIEW(w));

      OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(w));

      x = rect_assign(r.a.x+10, r.a.y+10, r.b.x-10, r.a.y+30);

      r = rect_assign(x.a.x, x.a.y, x.a.x+100, x.a.y+20);

      s = stattext_init( _malloc(sizeof(t_stattext)), r, TX_ALIGN_RIGHT|TX_ALIGN_BOTTOM, linkinfo);

      OBJECT(w)->insert(OBJECT(w), OBJECT(s));

      r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+30);

      s = stattext_init( _malloc(sizeof(t_stattext)), r, TX_ALIGN_RIGHT|TX_ALIGN_BOTTOM, TXT_ICONFORLINK);

      OBJECT(w)->insert(OBJECT(w), OBJECT(s));

      r = rect_assign(r.b.x+10, x.a.y, x.b.x, x.b.y);

      l = textline_init( _malloc(sizeof(t_textline)), r, IO_DIR_LIMIT, 0);

      r = rect_assign(r.a.x, r.b.y+11, r.b.x, r.b.y+31);

      il = textline_init( _malloc(sizeof(t_textline)), r, IO_DIR_LIMIT, 0);

      r = rect_assign(r.a.x, r.b.y+21, r.a.x+90, r.b.y+46);

      ok = button_init( _malloc(sizeof(t_button)), r, TXT_OK, MSG_OK, BF_DEFAULT);

      r = rect_assign(r.b.x+10, r.a.y, r.b.x+100, r.b.y);

      close = button_init( _malloc(sizeof(t_button)), r, TXT_CLOSE, MSG_CLOSE, BF_NORMAL);

      OBJECT(w)->insert(OBJECT(w), OBJECT(l));
      OBJECT(w)->insert(OBJECT(w), OBJECT(il));
      OBJECT(w)->insert(OBJECT(w), OBJECT(ok));
      OBJECT(w)->insert(OBJECT(w), OBJECT(close));

      if ( l ) l->set_text(l, ref); /* set text of textline */
      if ( il ) il->set_text(il, icon); /* set text of textline */

      if ( desktop->execute_view(desktop, VIEW(w)) == MSG_OK ) {

             l_text flink = l?l->text:NULL;
             l_text ficon = il?il->text:NULL;

             if ( filelink )

                io_set_linkedfile_ex(filename, flink, ficon); /* set link to file */

             else

                io_set_linkedpath_ex(filename, flink, ficon); /* set link to directory */

             ret = true;
      };

      _free(ref);

      dispose(OBJECT(w));

   };

   return ret;
};



/*
  show link/linkdir new box
*/

l_bool  link_new ( l_text path, l_text linkinfo, l_bool filelink )
{

   t_rect r;
   l_bool ret = false;

   p_appwin w;

   p_textline fn;
   p_textline l;
   p_textline il;
   p_stattext s;
   p_button   ok;
   p_button   close;

   r = rect_assign(0, 0, 400, 180);

   w = appwin_init(_malloc(sizeof(t_appwin)), r, linkinfo, 0, 0, NULL);

   if ( w ) {

      t_rect x;

      VIEW(w)->align |= TX_ALIGN_CENTER; /* center window */

      r = VIEW(w)->size_limits(VIEW(w));

      OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(w));

      x = rect_assign(r.a.x+10, r.a.y+10, r.b.x-10, r.a.y+30);

      r = rect_assign(x.a.x, x.a.y, x.a.x+100, x.a.y+20);

      s = stattext_init( _malloc(sizeof(t_stattext)), r, TX_ALIGN_RIGHT|TX_ALIGN_BOTTOM, TXT_FILENAME);

      OBJECT(w)->insert(OBJECT(w), OBJECT(s));

      r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+30);

      s = stattext_init( _malloc(sizeof(t_stattext)), r, TX_ALIGN_RIGHT|TX_ALIGN_BOTTOM, linkinfo);

      OBJECT(w)->insert(OBJECT(w), OBJECT(s));

      r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+30);

      s = stattext_init( _malloc(sizeof(t_stattext)), r, TX_ALIGN_RIGHT|TX_ALIGN_BOTTOM, TXT_ICONFORLINK);

      OBJECT(w)->insert(OBJECT(w), OBJECT(s));

      r = rect_assign(r.b.x+10, x.a.y, x.b.x, x.b.y);

      fn = textline_init( _malloc(sizeof(t_textline)), r, IO_DIR_LIMIT, 0);

      r = rect_assign(r.a.x, r.b.y+11, r.b.x, r.b.y+31);

      l = textline_init( _malloc(sizeof(t_textline)), r, IO_DIR_LIMIT, 0);

      r = rect_assign(r.a.x, r.b.y+11, r.b.x, r.b.y+31);

      il = textline_init( _malloc(sizeof(t_textline)), r, IO_DIR_LIMIT, 0);

      r = rect_assign(r.a.x, r.b.y+21, r.a.x+90, r.b.y+46);

      ok = button_init( _malloc(sizeof(t_button)), r, TXT_OK, MSG_OK, BF_DEFAULT);

      r = rect_assign(r.b.x+10, r.a.y, r.b.x+100, r.b.y);

      close = button_init( _malloc(sizeof(t_button)), r, TXT_CLOSE, MSG_CLOSE, BF_NORMAL);

      OBJECT(w)->insert(OBJECT(w), OBJECT(fn));
      OBJECT(w)->insert(OBJECT(w), OBJECT(l));
      OBJECT(w)->insert(OBJECT(w), OBJECT(il));
      OBJECT(w)->insert(OBJECT(w), OBJECT(ok));
      OBJECT(w)->insert(OBJECT(w), OBJECT(close));

      if ( il ) {

           l_text dir_icon = set_format_text(NULL, "%%%s", icon_id[ICO_DRIVEHD]);
           l_text file_icon = set_format_text(NULL, "%%%s", icon_id[ICO_APPLICATION]);

           if ( !filelink )  /* it's directory */

              il->set_text(il, dir_icon);

           else /* it's file */

              il->set_text(il, file_icon);

           _free(dir_icon);
           _free(file_icon);

      };


      if ( desktop->execute_view(desktop, VIEW(w)) == MSG_OK ) {

         l_text fname = fn?fn->text:NULL;
         l_text flink = l?l->text:NULL;
         l_text ficon = il?il->text:NULL;
         l_text realname = fname?io_realpath(path, fname):NULL;

         if ( realname && flink ) {

             ret = true;

             if ( filelink ) {

                l_text n = set_format_text(NULL, "%s.lnk", realname);

                io_set_linkedfile_ex(n, flink, ficon); /* set link to file */

                _free(n);

             } else {

                l_text n = set_format_text(NULL, "%s.ldv", realname);

                io_set_linkedpath_ex(n, flink, ficon); /* set link to directory */

                _free(n);

             };

         };

         free(realname);

      };

      dispose(OBJECT(w));

   };

   return ret;

};



/*
  set data function for
  - linkdir
  - linkfile
  - file
  - directory
  - these functions are part of t_filelistbox_item
*/

/*
  set data for file = run file (dst) by argument (src)

  - dst    destination directory
  - src    source file/directory
  - ind    indicator of process, this value contains at the end number of files
  - o      object that control process
*/
l_int linkfile_set_data ( p_object o, t_file *dst, t_file *src, l_dword *ind )
{
  return io_copyfile(o, dst, src, ind);
};


/*
  set data for file = run file (dst) by argument (src)

  - dst    destination directory
  - src    source file/directory
  - ind    indicator of process, this value contains at the end number of files
  - o      object that control process
*/
l_int file_set_data ( p_object o, t_file *dst, t_file *src, l_dword *ind )
{
  return io_copyfile(o, dst, src, ind);
};


/*
  set data for dir = copy files from src ( file/dir ) to dst ( dir ),

  - dst    destination directory
  - src    source file/directory
  - ind    indicator of process, this value contains at the end number of files
  - o      object that control process
*/
l_int dir_set_data ( p_object o, t_file *dst, t_file *src, l_dword *ind )
{

  return io_copyfile(o, dst, src, ind);

};


/*
  set data for linkdir = copy files from src ( file/dir ) to dst->linkdir ( dir ),

  - dst    destination directory
  - src    source file/directory
  - ind    indicator of process, this value contains at the end number of files
  - o      object that control process
*/
l_int linkdir_set_data ( p_object o, t_file *dst, t_file *src, l_dword *ind )
{
   l_text file = io_tfiletofile(dst);
   l_text ldv  = io_linkedpath(file);
   t_file f    = io_filetotfile(ldv);
   l_int ret   = 0;

   if ( ldv )

       ret = io_copyfile(o, &f, src, ind);

   else

       ret = io_copyfile(o, dst, src, ind);

   io_cleartfile(&f);
   _free(file);
   _free(file);

   return ret;
};



/*

   get file items from directory, return list of p_filelistbox_item(s)

   path   - real path to directory
   files  - type of files that must be displayed ( *.bmp, *.exe, hello.*, ... )
   attrib - atributes of files/directory that must be displayed

            may contain following:

            FA_DIREC     - directory
            FA_ARCH      - archive files
            FA_HIDDEN    - hidden files/directory
            FA_RDONLY    - read-only files
            FA_LINKIDIR  - link to directory - defined in _iodir.h
            FA_LINK      - link to file - defined in _iodir.h
            FA_NOTPARENT - not include ".." directory - defined in _iodir.h
*/


p_list get_listbox_item_dirfiles ( l_text path, l_text files, l_int attrib, l_int isize )
{
  p_list p = list_init(_malloc(sizeof(t_list)), &free_filelistbox_item, 0);
  p_item l = NULL;
  l_text fi = NULL;
  BITMAP *icon;

  if ( p ) {

     struct t_ffblk f;

     l_int done = 0;

     if ( drv_isdriver(path) ) attrib |= FA_NOTPARENT;

     /* directories */

     if ( attrib & FA_DIREC ) {

         attrib |= FA_LINKDIR;

         fi = io_realpath(path, "*.*"); /* insert string "path"/*.* to fi */

         done = io_findfirst(fi, &f, FA_DIREC); /* looking for directories */

         _free(fi);

         while ( !done ) { /* file found */

            l_int mem = 0;

            fi = io_realpath(path, f.ff_filename); /* insert string "path"/"found file" to fi */

            /* get icon for file, this function is defined in exedlx.h */

            icon = get_icon_for_file_ex(fi, f.info.ff_attrib, &mem, isize);

            if ( f.ff_filename && f.info.ff_attrib & FA_DIREC && stricmp(f.ff_filename, ".")
                 && *(f.ff_filename) &&
                 (stricmp(f.ff_filename, "..") || !(attrib & FA_NOTPARENT))
               )  { /* OK it found direcory */

                   p_filelistbox_item diritem;

                   /* make new t_filelistbox_item */
                   diritem = new_filelistbox_item_ex(f.info.ff_name, icon,
                                 false, mem ? LIF_MEMORY : LIF_NONE,
                                 f.info.ff_attrib, f.info.ff_fdate,
                                 f.info.ff_ftime, f.info.ff_fsize,
                                 path, f.ff_filename,
                                 &dir_properties, &dir_set_data
                             );

                   p->insert(p, diritem); /* insert t_filelistbox_item (direcotry) to the list */

            };

            _free(fi);

            done = io_findnext(&f);  /* looking for next directory */

         };

         collect_by_name(p, LISTBOX_ITEM_NAME); /* collect list by the name */

         l = p->last;

     };

     attrib &= ~FA_DIREC;


     /* directory links */

     if ( attrib & FA_LINKDIR ) {

         fi = io_realpath(path, "*.*"); /* insert string "path"/*.* to fi */

         done = io_findfirst(fi, &f, FA_LINKDIR); /* looking for files */

         _free(fi);

         while ( !done ) { /* file found */

            l_int mem = 0;

            fi = io_realpath(path, f.ff_filename); /* insert string "path"/"found file" to fi */

                /* get icon for file, this function is defined in exedlx.h */

            icon = get_icon_for_file_ex(fi, f.info.ff_attrib, &mem, isize);

            if ( f.info.ff_attrib & FA_LINKDIR ) { /* OK, it's link to dir what it found */

                  p_filelistbox_item linkdiritem;

                  /* make new filelistbox item */
                  linkdiritem = new_filelistbox_item_ex(strlwr(f.info.ff_name),
                                    icon, false, mem ? LIF_MEMORY : LIF_NONE,
                                    f.info.ff_attrib, f.info.ff_fdate,
                                    f.info.ff_ftime, f.info.ff_fsize,
                                    path, f.ff_filename,
                                    &linkdir_properties, &linkdir_set_data
                                );

                  p->insert(p, linkdiritem); /* insert linkdir item into the list */

            };

            done = io_findnext(&f); /* looking for next file */

            _free(fi);

         };

         p->collect_by_name_from(p, l, LISTBOX_ITEM_NAME); /* collect list from last directory by the name */

         l = p->last;
     };


     attrib &= ~FA_LINKDIR;

     /* files finding */

     if ( attrib ) {

         fi = io_realpath(path, files);

         done = io_findfirst(fi, &f, attrib); /* looking for files */

         _free(fi);

         while ( !done ) {

            p_filelistbox_item fileitem;

            l_int mem = 0;

            fi = io_realpath(path, f.ff_filename);  /* insert string "path"/"file found" to fi */

                /* get icon for file, this function is defined in exedlx.h */

            icon = get_icon_for_file_ex(fi, f.info.ff_attrib, &mem, isize);

            if ( f.info.ff_attrib & FA_LINK )  /* it's link file */

            /* make new filelistbox item */
            fileitem = new_filelistbox_item_ex(strlwr(f.info.ff_name), icon,
                           false, mem ? LIF_MEMORY : LIF_NONE,
                           f.info.ff_attrib, f.info.ff_fdate, f.info.ff_ftime,
                           f.info.ff_fsize,
                           path, f.ff_filename,
                           &linkfile_properties, &linkfile_set_data
                       );

            else  /* it's normal file */

            /* make new filelistbox item */
            fileitem = new_filelistbox_item_ex(strlwr(f.info.ff_name), icon,
                           false, mem ? LIF_MEMORY : LIF_NONE,
                           f.info.ff_attrib, f.info.ff_fdate, f.info.ff_ftime,
                           f.info.ff_fsize,
                           path, f.ff_filename,
                           &file_properties, &file_set_data
                       );

            p->insert(p, fileitem); /* insert file item into the list */

            done = io_findnext(&f); /* looking for next file */

            _free(fi);

         };

         p->collect_by_name_from(p, l, LISTBOX_ITEM_NAME);  /* collect list from last link directory by the name */

     };

  };

  return p;
};


/* filelistbox item functions */

/*
   make new file item,

   name  - name that will be displayed at list
   icon  - icon of item
   sel   - if item is selected
   flags - if LIF_MEMORY, than context of the icon will be free automatically

   filename - real name of file/directory
   path     - real path to file/directory
   attrib   - attrib of file/directory
   date     - date of file/directory
   time     - time of file/directory
   size     - size of file, directory = 0

   properties - function that displayed info about file/directory
   set_data   - function that set data to file/directory

*/

p_filelistbox_item   new_filelistbox_item_ex ( l_text name, BITMAP *icon, l_bool sel, l_int flags, l_int attrib, l_word date, l_word time, l_dword size, l_text path, l_text filename, l_bool (*properties)(l_text filename), l_int (*set_data)(p_object o, t_file *dst, t_file *src, l_dword *ind ) )
{

  p_filelistbox_item i = (p_filelistbox_item)_malloc(sizeof(t_filelistbox_item));

  if ( i ) {

     i->obclass.name  = _strdup(name);
     i->obclass.icon  = icon;
     i->obclass.sel   = sel;
     i->obclass.flags = flags;

     i->info.filename      = _strdup(filename);
     i->info.path          = _strdup(path);
     i->info.attrib        = attrib;

     i->info.date          = date;
     i->info.time          = time;
     i->info.size          = size;

     /* functions */
     i->properties    = properties;
     i->set_data      = set_data;

  };

  return i;

};


/* free file item */

void  free_filelistbox_item ( void *o )
{

   if ( o ) {

      _free(((p_filelistbox_item)o)->info.filename);
      _free(((p_filelistbox_item)o)->info.path);

   };

   free_listbox_item(o);

};




/* t_filelistbox functions */

void  filelistbox_setup ( p_object o )
{

  scroller_setup(o);

  FILELISTBOX(o)->load_list(FILELISTBOX(o), FILELISTBOX(o)->path, FILELISTBOX(o)->file);

};


/*

  return list of selected items.

  list is in t_ffblk file type.

*/

p_list filelistbox_get_selected_items ( p_listbox o, l_bool sel )
{

  p_list list = NULL;

  if ( o->list && o->get_max(o) >= 0 ) {

      list = list_init(_malloc(sizeof(t_list)), &free_tfile, DAT_TFILE);

      if ( list ) {

         p_item i = o->list->last;
         p_item l = i;

         if ( i )
         do {

            if ( ((p_listbox_item)i->rec)->sel == sel ) {

                 p_filelistbox_item t = ((p_filelistbox_item)i->rec);

                 if ( io_isfilename(t->info.filename) ) /* if it's ok file name */

                     list->insert(list, new_tfile(t->info.path, t->info.filename,
                                        t->info.attrib, t->info.time,
                                        t->info.date, t->info.size));

            };

            i = i->next;

         } while ( i != l );

      };

  };

  return list;

};



/*

   - get data from filelistbox, data are stored in rec->data pointer and

     this function set to this pointer list of DAT_TFILEs

     ( t_file structure ).
*/

l_bool     filelistbox_get_data ( p_object o, t_data *rec )
{
  if ( rec && o->is_state(o, OB_SF_FOCUSED) ) { /* i'm focused */

     p_filelistbox_item i = (p_filelistbox_item)LISTBOX_ITEM_AT(LISTBOX(o)->list, LISTBOX(o)->current);

     rec->info_obj = o;
     l_tag_cpy(rec->id, DAT_LIST);
     rec->data = NULL;

     if ( i )

     switch ( rec->style ) { /* some item exist */

       case DS_SELECTED : {

          p_list p = LISTBOX(o)->get_selected_items(LISTBOX(o), true);

          if ( !p ) return false;

          if ( !p->last ) { /* not item is select */

             if ( !io_isfilename(i->info.filename) ) { /* current item is not ok file/directory = "..", ".", ... */

                 l_tag_cpy(rec->id, DAT_NONE);

                 return false;

             } else { /* current item is ok file/directory != "..", "." , ... */

                 /* make new pointer to t_file structure and add to the list */

                 p->insert(p, new_tfile(i->info.path, i->info.filename,
                                  i->info.attrib, i->info.time,
                                  i->info.date, i->info.size));

             };

          };

          rec->data = (void*)p; /* set rec->data to list of t_file structure */

          l_tag_cpy(rec->id, DAT_LIST); /* set (id) to LIST */

          return true;

       }; break;

       case DS_ALL : {

          rec->style = DS_SELECTED;

          o->select_data(o, DS_ALL, true);

          return o->get_data(o, rec);

       }; break;

       case DS_WHATEVER : {

          rec->style = DS_SELECTED;

          return o->get_data(o, rec);

       }; break;

     };

     l_tag_cpy(rec->id, DAT_NONE);

  };

  return false;

};


l_bool     filelistbox_set_data ( p_object o, t_data *rec )
{
  #define  retr(t) do { _free(path); return t; } while (0)

  l_bool   ok = false;
  l_dword  indicator = 0; /* number of files that were in process */
  l_text   path = _strdup(FILELISTBOX(o)->path);

  p_filelistbox_item i = (p_filelistbox_item)LISTBOX_ITEM_AT(LISTBOX(o)->list, LISTBOX(o)->current);

  if ( rec && l_tag_cmp(o->data_type, rec->id) && i ) {

     if ( rec->style & DS_DELETE ) {

       if ( o->is_state(o, OB_SF_FOCUSED) ) { /* delete only if focused */

           if ( rec->style & DS_ALL ) {

               retr(false);

           } else { /* delete DS_SELECTED or DS_WHATEVER, so only select item */

               p_list p = LISTBOX(o)->get_selected_items(LISTBOX(o), true);

               if ( !p ) retr(false);

               if ( !p->last ) { /* not item is select */

                  if ( !io_isfilename(i->info.filename) ) { /* current item is not ok file/directory = "..", ".", ... */

                      retr(false); /* no delete */

                  } else { /* current item is ok file/directory != "..", "." , ... */

                      /* make new pointer to t_file structure and add to the list */

                      p->insert(p, new_tfile(i->info.path, i->info.filename,
                                   i->info.attrib, i->info.time,
                                   i->info.date, i->info.size));

                  };

               };

               /* delete files in t_list (p) */

               if ( msgbox(MW_QUESTION, MB_YES|MB_NO, "%s %s ?", TXT_AREYOUSUREYOUWANTTODELETE, strlwr(TXT_THESEFILES)) == MSG_YES ) {

                  l_text nicefile = set_format_text_nice(NULL, -29, FILELISTBOX(o)->path);
                  l_text from = set_format_text(NULL, "%s %s", TXT_DELETINGFILESFROM, nicefile);

                  p_object proc; /* window that control process */

                  l_dword allfiles = 0;

                  /* get number of files in all directories */

                  p->for_each_item(p, NULL, &io_numberfile, &allfiles);

                  /* show process window */

                  proc = show_process(TXT_DELETINGFILES, from, allfiles, &indicator);

                  /* removing process */

                  p->for_each_item(p, proc, &io_removefile, &indicator);

                  /* hide process window */

                  hide_process(proc);

                  ok = true;

                  _free(from);
                  _free(nicefile);

               } else ok = false;

           };

       };

     } else /* data are pasted into the object */

     if ( l_tag_cmp(rec->id, DAT_LIST) && rec->data &&  /* rec->data is DAT_LIST = ptr to t_list structure */
         l_tag_cmp(((p_list)rec->data)->tag, DAT_TFILE) ) { /* t_list items->rec contains ptr to DAT_TFILE item = t_file structure see ( _iods.h  )*/

         l_text fn = io_realpath(i->info.path, i->info.filename);
         l_text file = NULL;
         l_text nicelink = fn; /* if file is link, it make io_nicelink */

         /* ask for where to copy */
         l_dword msg = menu_file_copy_where(nicelink);

         switch ( msg ) {

            case MSG_COPYTODIR: file = i->info.path; break; /* copy files to dir, where current file (item) is placed  */
            case MSG_COPYTOFILE: file = fn; break; /* copy files to current file (item) */

         };


         if ( !file ) {

            _free(fn);

            retr(false); /* menu was canceled */

         };

         nicelink = io_nicelink(file);

         /* copy files from t_list (p) into current item */

         if ( msgbox(MW_QUESTION, MB_YES|MB_NO, "%s %s %s %s?", TXT_AREYOUSUREYOUWANTTOCOPY, strlwr(TXT_THESEFILES), strlwr(TXT_INTO), file) == MSG_YES ) {

             l_text nicefile = set_format_text_nice(NULL, -29, file);
             l_text to = set_format_text(NULL, "%s %s", TXT_COPYINGFILESTO, nicefile);
             t_file inf;

             p_object proc; /* window that control process */

             l_dword allfiles = 0;

             /* get number of files in all directories */

             ((p_list)(rec->data))->for_each_item(((p_list)(rec->data)), NULL, &io_numberfile, &allfiles);

             /* show process window */

             proc = show_process(TXT_COPYINGFILES, to, allfiles, &indicator);

             /* copying process */

             inf = i->info; /* copy current info of file item to copying info */

             path = _strdup(inf.path);

             inf.path = io_path(file);
             inf.filename = io_filename(file);

             ((p_list)(rec->data))->for_each_item_to_item((p_list)(rec->data), proc, &inf, i->set_data, &indicator);

             _free(inf.path);
             _free(inf.filename);

             /* hide process window */

             hide_process(proc);

             ok = true;

             _free(to);
             _free(nicefile);

         } else ok = false;

         _free(nicelink);
         _free(fn);

     };

     /* reload and redraw listbox */

     if ( ok )

        /* retype path */

        RETYPE_FILES_IN_PATH(path);


  };

  retr(ok);


};



/*
   drag_where function redraw [mouse under]-item, when some t_data are under

   this object.

*/
l_bool  filelistbox_drag_where ( p_view o, p_data rec, t_point where )
{

   if ( view_drag_where(o, rec, where) ) { /* is mouse under view */

       l_dword mpos = LISTBOX(o)->get_pos_from_xy(LISTBOX(o), where);

       /*
          it must call only listbox rewrite_item, because filelistbox rewrite_item
          function make some changes
       */

       listbox_rewrite_item(LISTBOX(o), mpos);

       return true;

   } else /* mouse is not under view or end */

   if ( OBJECT(o)->is_state(OBJECT(o), OB_SF_FOCUSED) ) {

       if ( rec ) /* mouse is not under view */
          ;


       else /* end of dragging - give things to right place */
         ;

   } else { /* mouse is not under view or end but in non-focused view */


   };

   return false;
};


/*

   translate_event function

   get info about pressed item and set o->file ( t_filelistbox ) and
   o->path ( t_filelistbox ) to current file values. If file item was
   pressed ( by doubleclick ), it call MSG_OK same as in OK button.

   If file was pressed, then not clear event, but if directory or file is
   kind of extension, it clear event and reload list. ( see MSG_OK message )

   If file is pressed (by right mouse button) it call menu ( file_menu )
   for selected file. If filemenu return MSG_PROPERTIES it run properties
   function ( t_filelistbox_item ) for selected file, if MSG_COPY,
   then run get_data function, if MSG_CUT, MSG_PASTE, it run set_data
   function for selected file item.

*/

void  filelistbox_translate_event ( p_object o, p_event event )
{

  RETVIEW(o, event);

  /* info text */

  if ( VIEW(o)->is_mouse_in_view(VIEW(o)) && LISTBOX(o)->list &&
       OBJECT(keyb)->state & KB_SF_KEYDOWN && KEYCTRL(KB_F1) )  { /* show info text */

         l_long  mpos = LISTBOX(o)->get_pos_from_xy(LISTBOX(o), VIEW(o)->get_local_point(VIEW(o), mouse->where));
         void   *rec  = LISTBOX(o)->list->at(LISTBOX(o)->list, mpos);

         l_text  item_text = rec?((p_filelistbox_item)rec)->info.filename:NULL;

         l_dword item_size = rec?((p_filelistbox_item)rec)->info.size:NULL;

         set_format_text(&(VIEW(o)->info_text), "'%s'\n %s: %i (bytes)", item_text, TXT_SIZE, item_size);

         VIEW(o)->show_info_board(VIEW(o));

         clear_event(event);

  };

  if ( event->type & EV_MOUSE &&
       OBJECT(o)->is_state(OBJECT(o), OB_SF_FOCUSED) ) { /* mouse event */

       /* test for doubleclick */
       if ( OBJECT(mouse)->state & MO_SF_MOUSELDOUBLE ) {

            clear_event(event);

            set_event(event, EV_MESSAGE, MSG_OK, o);

       };

  };


  listbox_translate_event(o, event); /* old translate event */



  /* keyboard event */

  if ( event->type & EV_KEYBOARD && OBJECT(keyb)->state & KB_SF_KEYDOWN &&

       OBJECT(o)->is_state(OBJECT(o), OB_SF_FOCUSED) ) {

       if ( keyb->code == TO_KEY(KB_ENTER) ) { /* enter was pressed */

          clear_event(event);

          set_event(event, EV_MESSAGE, MSG_OK, o); /* call MSG_OK2 */

       };

  };


  if ( event->type & EV_MOUSE ) { /* mouse event */

      if ( OBJECT(mouse)->state & MO_SF_MOUSERPRESS && o->owner &&
           o->owner->is_state(o->owner, OB_SF_FOCUSED) ) { /* right button pressed */

          if ( OBJECT(mouse)->state & MO_SF_MOUSERDOWN &&
               !o->is_state(o, OB_SF_FOCUSED) )  /* object is not focused */

               o->select(o);

          clear_event(event);


          if ( o->is_state(o, OB_SF_FOCUSED) )  /* object is focused */

              /* set current item to pointer */

              LISTBOX(o)->rewrite_item(LISTBOX(o), LISTBOX(o)->get_pos_from_xy(LISTBOX(o), VIEW(o)->get_local_point(VIEW(o), mouse->where)));

      };

      /* make menu */
      if ( o->is_state(o, OB_SF_FOCUSED) ) {

         if ( OBJECT(mouse)->state & MO_SF_MOUSERUP ) { /* right button un-pressed */

            l_dword msg = 0;

            clear_event(event);

                         /* make menu over the file/directory */
            msg = FILELISTBOX(o)->file_menu(FILELISTBOX(o));

            if ( msg ) {

               clear_event(event);

               set_event(event, EV_MESSAGE, msg, o);

            };
         };
      };

  };



  if ( event->type & EV_RETYPE ) { /* retype event */

     switch ( event->message ) {

         case MSG_RETYPEPATH : { /* this redraw filelistbox if event->info is same to o->path */

             /* ! not clear event, it must be send further !*/

             if ( event->info && FILELISTBOX(o)->path && /* if same path as retyped path */
                  io_issame(event->info, FILELISTBOX(o)->path) ) {

                    /* make new list */
                     FILELISTBOX(o)->load_list(FILELISTBOX(o), FILELISTBOX(o)->path, FILELISTBOX(o)->file);

             };

         }; break;

     };

  };


  if ( event->type & EV_MESSAGE ) { /* message event */

      switch ( event->message ) {

         case MSG_OPENDIR : {

              if ( LISTBOX(o)->flags & FL_DIROPEN )  /* open directory, end modal */

                     event->message = MSG_OK; /* end modal */

         }; break;

         case MSG_OK  : { /* open file/directory */

            if ( o->is_state(o, OB_SF_FOCUSED) ) {

                 l_int what = FILELISTBOX(o)->file_handle(FILELISTBOX(o));

                 if ( what == 2 ) { /* selected is directory */

                     clear_event(event);

                 };

            } else if ( io_isext(FILELISTBOX(o)->file) ) { /* file is extension */

                    /* reload load list */

                    FILELISTBOX(o)->load_list(FILELISTBOX(o), FILELISTBOX(o)->path, FILELISTBOX(o)->file);

                    clear_event(event);

            };

            /* file is may-be file */

            if ( event->type ) {  /* event type wasn't clear => maybe file */

                  t_event  oldev = *event;
                  l_text   file = FILELISTBOX(o)->get_file(FILELISTBOX(o));
                  l_text   rf = io_realpath(FILELISTBOX(o)->path, file);

                  if ( LISTBOX(o)->flags & FL_OPEN ) { /* i'm open dialog */

                        if ( !io_isfile(rf) ) { /* no !, this file not exist */

                              /* make message, file not found */
                              msgbox(MW_ERROR, MB_OK, "%s %s %s.\n\n%s.", TXT_FILE, file, strlwr(TXT_NOTFOUND), TXT_CHECKTHEFILENAMEANDTRYAGAIN);

                              /* try again */
                              clear_event(&oldev);
                        };

                  } else
                  if ( LISTBOX(o)->flags & FL_SAVE ) { /* i'm save dialog */

                        if ( io_isfile(rf) ) { /* oh !, this file exist */

                              /* ask for rewriting */
                              if ( msgbox(MW_WARNING, MB_YES|MB_NO, "%s %s %s.\n\n%s ?", TXT_FILE, file, strlwr(TXT_EXIST), TXT_WOULDYOULIKETOOVERWRITEIT) != MSG_YES )

                                    /* button cancel, try again */
                                    clear_event(&oldev);
                        };

                  };

                  _free(rf);

                  *event = oldev;

            };

         }; break;

         case MSG_NEWDIRECTORY : { /* make new directory */

             clear_event(event);

             FILELISTBOX(o)->new_directory(FILELISTBOX(o));

         }; break;

         case MSG_NEWLINKTODIRECTORY : { /* make new link to directory */

             clear_event(event);

             if ( link_new(FILELISTBOX(o)->path, TXT_LINKTODIRECTORY, false) )

                   /* reload list */

                   RETYPE_FILES_IN_PATH(FILELISTBOX(o)->path);

         }; break;

         case MSG_NEWLINKTOFILE : { /* make new link to file */

             clear_event(event);

             if ( link_new(FILELISTBOX(o)->path, TXT_LINKTOFILE, true) )

                   /* reload list */

                   RETYPE_FILES_IN_PATH(FILELISTBOX(o)->path);

         }; break;

         case MSG_CHANGENAME : { /* change name of file/directory */

             clear_event(event);

             FILELISTBOX(o)->change_name(FILELISTBOX(o));

         }; break;

         case MSG_PROPERTIES : { /* properties of file/directory */

            p_filelistbox_item rec = (p_filelistbox_item)LISTBOX_ITEM_AT(LISTBOX(o)->list, LISTBOX(o)->current);

            clear_event(event);

            if ( rec && rec->properties ) {

               l_text rn = io_realpath(rec->info.path, rec->info.filename); /* get real name of file = "path"/"filename" */

               if ( rec->properties(rn) ) /* something was changed */

                   /* reload list */

                   RETYPE_FILES_IN_PATH(FILELISTBOX(o)->path);


               _free(rn);

             };

         }; break;

      };

  };

};



/*
  if o->file is exension && filelistbox is focused
  then return name of selected item, else return o->file
*/
l_text filelistbox_get_file ( p_filelistbox o )
{

   if ( io_isext(FILELISTBOX(o)->file) ) /* is extension */

         return FILELISTBOX_ITEM_FILENAME(LISTBOX(o)->list, LISTBOX(o)->current);

   return FILELISTBOX(o)->file;

};


void   filelistbox_rewrite_item ( p_listbox o, l_long item )
{

   l_text   item_text = NULL;
   l_text   old_file;
   p_item   it = LISTBOX_ITEM_AT(LISTBOX(o)->list, LISTBOX(o)->current);
   t_event  e;

   listbox_rewrite_item(o, item); /* old call of listbox_rewrite_item */

   if ( !it ) return;

   old_file = (l_text)(((p_filelistbox_item)(it))->info.filename);

   it = LISTBOX_ITEM_AT(LISTBOX(o)->list, LISTBOX(o)->current);

   item_text = (l_text)(((p_filelistbox_item)(it))->info.filename);

   if ( !(o->flags & FL_DIROPEN) ) { /* not directory open */

         set_event(&e, EV_INFO, MSG_SELECTEDFILELISTBOXITEM, OBJECT(o));

         OBJECT(o)->put_event(OBJECT(o), &e);

         if ( old_file && item_text && !stricmp(old_file, item_text) ) { /* not same files */

               l_int attrib = ((p_filelistbox_item)(it))->info.attrib;

               if ( FA_ISFILE(attrib) && FILELISTBOX(o)->file_sel ) { /* new item is file && file_sel function exist */

                     l_text rf = io_realpath(FILELISTBOX(o)->path, item_text);

                     FILELISTBOX(o)->file_sel(rf); /* call file_sel function, when file is sel */

               };

         };

   };

};


/*
  make operations for specific attributes
*/

l_int   filelistbox_file_handle_ex ( p_filelistbox o, l_text opath, l_text ofile, l_int attrib, l_text filename )
{

      if ( !opath ) return 0;

      if ( attrib & FA_LINKDRIVE ) { /* it's link to drive */

         o->load_list(o, opath, o->file); /* only reload list */

         return 2;

      };


      if ( attrib & FA_LINKDIR && filename ) { /* it's link to directory */

         l_text path = io_realpath(opath, filename);

         o->load_list(o, io_linkedpath(path), o->file);

         _free(path);

         return 2;

      };


      if ( attrib & FA_DIREC && filename ) { /* current is directory */

         if ( !strcmp(filename, "..") ) { /* parent dir !! filename must be tested if not NULL */

              l_text path = io_parentdir(opath);

              o->load_list(o, path, ofile);

              return 2;

         } else { /* sub directory */

            o->load_list(o, io_realpath(opath, filename), ofile);

            return 2;

        };


      };

  return 0;

};


l_int   filelistbox_file_handle ( p_filelistbox o )
{

  p_filelistbox_item item = LISTBOX_ITEM_AT(LISTBOX(o)->list, LISTBOX(o)->current);

  if ( item ) {

    l_int ok;

    ok = o->file_handle_ex(o, o->path, o->file, item->info.attrib, item->info.filename);

    return ok;

  };

  return 0;

};


l_dword filelistbox_file_menu ( p_filelistbox o )
{

  p_filelistbox_item item = NULL;

  item = LISTBOX_ITEM_AT(LISTBOX(o)->list, LISTBOX(o)->current);

  if ( item ) {

      l_int fatr = item->info.attrib;

      p_menu p = new_menu(

                 new_menu_item_ex(TXT_OPEN, NULL, 0, MSG_OK, true,
                                  TXT_INFOOPEN, MIF_NONE, font_system_bd, NULL, NULL, 0,
                 new_menu_line(
                 new_menu_item(TXT_CHANGENAME, NULL, 0, MSG_CHANGENAME, TXT_INFOCHANGENAME,
                 new_menu_line(
                 new_menu_item(TXT_COPY, NULL, 0, MSG_COPY, TXT_INFOCOPY,
                 new_menu_item(TXT_DELETE, NULL, 0, MSG_CUT, TXT_INFODEL,
                 new_menu_item(TXT_PASTE, NULL, 0, MSG_PASTE, TXT_INFOPASTE,
                 new_menu_line(
                 new_menu_item(TXT_PROPERTIES, NULL, 0, MSG_PROPERTIES, TXT_INFOPROPERTIES,
                 new_menu_line(
                 new_sub_menu(TXT_NEW, new_menu(

                     new_menu_item(TXT_DIRECTORY, NULL, 0, MSG_NEWDIRECTORY, TXT_INFONEWDIRECTORY,
                     new_menu_item(TXT_LINKTOFILE, NULL, 0, MSG_NEWLINKTOFILE, TXT_INFOLINKTOFILE,
                     new_menu_item(TXT_LINKTODIRECTORY, NULL, 0, MSG_NEWLINKTODIRECTORY, TXT_INFOLINKTODIRECTORY,
                     NULL)))),

                NULL)))))))))))
             );

      if ( p ) {

         t_rect   r = rect_assign(mouse->where.x-5, mouse->where.y-5, 0, 0);

         p_object menu = OBJECT(menuview_init(
                                _malloc(sizeof(t_menuview)),
                                r,
                                p)
                         );


         return desktop->execute_view(desktop, VIEW(menu));

      };
  };

  return MSG_CANCEL;
};


/*
   make new directory
   return true directory was made, else return false
*/
l_bool  filelistbox_new_directory ( p_filelistbox o )
{
   l_bool ret = false;

   l_text ud = io_uniquedir(o->path); /* get unique dir */

   if ( ud ) {

       l_text rp = io_realpath(o->path, ud);
       l_text nd = NULL;

       if ( io_mkdir(rp) ) { /* unique name is ok */

            LISTBOX(o)->list->insert(LISTBOX(o)->list, new_filelistbox_item(ud, NULL, false, 0, FA_DIREC, 0, 0, 0, o->path, ud));

            LISTBOX(o)->rewrite_item(LISTBOX(o), LISTBOX(o)->get_max(LISTBOX(o)));

            if ( !o->change_name(o) ) /* wasn't reload list */

                    /* reload list */
                    RETYPE_FILES_IN_PATH(o->path);


            ret = true;
       };

       _free(rp);
       _free(ud);

   };

   return ret;
};


/*
   change name of current item
   return true if item name was changed, else return false
*/
l_bool  filelistbox_change_name ( p_filelistbox o )
{
   l_bool ret = false;

   if ( LISTBOX(o)->list ) {

      p_filelistbox_item item = LISTBOX_ITEM_AT(LISTBOX(o)->list, LISTBOX(o)->current);

      t_rect r = LISTBOX(o)->get_item_rect(LISTBOX(o), LISTBOX(o)->current); /* get current item rect */

      p_textline t = worktextline_init((p_textline)_malloc(sizeof(t_textline)),
                                       r,
                                       IO_DIR_LIMIT);
      if ( !t ) return NULL;

      if ( item )

            t->set_text(t, item->info.filename);

      OBJECT(o)->insert(OBJECT(o), OBJECT(t));


      if ( t && VIEW(o)->execute_view(VIEW(o), VIEW(t)) != MSG_CANCEL ) /* not ESC */

         if ( t->text ) { /* set name of item to new name */

               l_text oldname = io_realpath(item->info.path, item->info.filename);
               l_text newname = io_realpath(item->info.path, t->text);
               l_int  err = 0;

               err = io_rename(oldname, newname);

               if ( err > 0 ) { /* remove errors ! warning ! not renamed, my god, what's the matter ? > I'm really happy */
                    /* draw info about */
                    msgbox(MI_ERROR, MB_OK, "%s", TXT_FILECOULDNTBERENAME);

               } else if ( err == 0 ) { /* no error, then redraw */

                    _free(item->info.filename); /* free old name */
                    item->obclass.name = _strdup(t->text); /* set to new name */

                    RETYPE_FILES_IN_PATH(o->path);


                    ret = true;
               };

               _free(oldname);

               _free(newname);

         };

      dispose(OBJECT(t));

   };

   return ret;
};


/* reload file item list and redraw this listbox */

void  filelistbox_load_list ( p_filelistbox o, l_text path, l_text file )
{

  if ( io_exist(path) ) { /* path exist */

      l_text path_ex = _strdup(path);
      l_text file_ex = _strdup(file);

      LISTBOX(o)->shift_flag = 0;

      if ( path != o->path ) _free(path);
      if ( file != o->file ) _free(file);

      _free(o->path);
      _free(o->file);

      o->path = path_ex;
      o->file = file_ex;

      if ( o->path ) {

         dispose_list(&(LISTBOX(o)->list), 1);

         LISTBOX(o)->list = get_listbox_item_dirfiles(path_ex, file_ex, o->attrib, icon_size(o));

         LISTBOX(o)->draw_box(LISTBOX(o));

         LISTBOX(o)->rewrite_item(LISTBOX(o), 0);

         draw_scroller_bars(SCROLLER(o));

      };

  } else seal_error(ERR_INFO, "%s %s %s", TXT_PATH, path, strlwr(TXT_NOTFOUND));

};



/* t_filehistory functions */

l_dword   filehistory_show_box ( p_history o )
{

  l_dword msg = history_show_box(o);

  if ( msg == MSG_OK && FILEHISTORY(o)->filebox ) { /* some item was pressed */

       l_text filename = FILELISTBOX_ITEM_FILENAME(o->list, o->current);

       FILEHISTORY(o)->filebox->load_list(FILEHISTORY(o)->filebox, FILEHISTORY(o)->filebox->path, _strdup(filename));

  };

  return msg;

};




/* t_dirhistory functions */

l_dword   dirhistory_show_box ( p_history o )
{

  l_dword msg;

  if ( DIRHISTORY(o)->filebox ) {

      DIRHISTORY(o)->load_list(DIRHISTORY(o), DIRHISTORY(o)->filebox->path);

  };

  msg = history_show_box(o);

  if ( msg == MSG_OK && DIRHISTORY(o)->filebox ) {

       l_int  attrib = FILELISTBOX_ITEM_ATTRIB(HISTORY(o)->list, HISTORY(o)->current);
       l_text file   = FILELISTBOX_ITEM_FILENAME(HISTORY(o)->list, HISTORY(o)->current);
       l_text path   = FILELISTBOX_ITEM_PATH(HISTORY(o)->list, HISTORY(o)->current);

       DIRHISTORY(o)->filebox->file_handle_ex(DIRHISTORY(o)->filebox, path, FILEHISTORY(o)->filebox->file, attrib, file);

  };

  return msg;

};


void    dirhistory_load_list ( p_dirhistory o, l_text path )
{

   if ( path ) {

       p_list   p;
       p_drives d;
       l_text   drvname;

       if ( HISTORY(o)->list )

             dispose_list ( &(HISTORY(o)->list), true );

       p = HISTORY(o)->list = list_init(_malloc(sizeof(t_list)), &free_filelistbox_item, 0);

       drvname = drv_findfirst(&d);

       while ( d ) {

           if ( drvname ) { /* if drive exist */

               p_list v;

               p->insert(p, new_filelistbox_item(drvname, get_icon_for_file_ex(drvname, FA_DIREC, NULL, icon_size(o)), false, 0, FA_LINKDRIVE, 0, 0, 0, d->path, d->path));

               v = get_listbox_item_dirfiles(d->path, "*.*", FA_LINKDIR+FA_DIREC+FA_NOTPARENT, icon_size(o));

               if ( v ) {

                  p->copy_ctx(p, v);

                  dispose_list(&v, false);

               };


           };

           drvname = drv_findnext(&d);
       };

   };

};


/* t_filetextline functions */

void  filetextline_translate_event ( p_object o, p_event event )
{

  RETVIEW(o, event);


  textline_translate_event(o, event); /* old translate_event function */


  /* keyboard event */

  if ( event->type & EV_KEYBOARD && OBJECT(keyb)->state & KB_SF_KEYDOWN &&

       OBJECT(o)->is_state(OBJECT(o), OB_SF_FOCUSED) ) {

       if ( keyb->code == TO_KEY(KB_ENTER) ) { /* enter was pressed */

          clear_event(event);

          set_event(event, EV_MESSAGE, MSG_OK, o); /* call MSG_OK2 */

       };

  };

  if ( event->type & EV_MESSAGE ) /* event message */

     if ( event->message == MSG_OK && o->is_state(o, OB_SF_FOCUSED) ) {

         if ( TEXTLINE(o)->text && FILETEXTLINE(o)->filebox ) {  /*  */

               FILETEXTLINE(o)->filebox->file = TEXTLINE(o)->text;

               if ( io_isext(FILETEXTLINE(o)->filebox->file) ) {

                     clear_event(event);

                     FILETEXTLINE(o)->filebox->load_list(FILETEXTLINE(o)->filebox,
                                                         FILETEXTLINE(o)->filebox->path,
                                                         FILETEXTLINE(o)->filebox->file);

               };

         } else

               clear_event(event);

     };

  if ( event->type & EV_INFO ) { /* info event */

      /* filelistbox was reselect */
     if ( event->message == MSG_SELECTEDFILELISTBOXITEM &&
          event->obj && event->obj == OBJECT(FILEHISTORY(o)->filebox) ) {

          p_filelistbox_item rec = (p_filelistbox_item)LISTBOX_ITEM_AT(LISTBOX(event->obj)->list, LISTBOX(event->obj)->current);

          if ( rec ) { /* filelistbox current item exist */

             if ( rec->info.attrib & FA_DIREC ||
                  rec->info.attrib & FA_LINKDIR ) /* current item is directory or link to dir */

                 TEXTLINE(o)->set_text(TEXTLINE(o), FILEHISTORY(o)->filebox->file);

             else {

                 TEXTLINE(o)->set_text(TEXTLINE(o), rec->info.filename);

                 FILETEXTLINE(o)->filename = _strdup(rec->info.filename);

             };

          };

          clear_event(event);

     };

  };

};





/* t_dirhistory */

p_dirhistory  _dirhistory_init ( p_dirhistory o, t_rect r, l_int limit, l_int flags, p_filelistbox filebox )
{

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_dirhistory));

  history_init(HISTORY(o), r, NULL, limit, flags);

  o->filebox = filebox;


  /* object's functions */


  /* view's functions */


  /* history's functions */

  HISTORY(o)->show_box = &dirhistory_show_box;


  /* dirhistory's functions */

  o->load_list = &dirhistory_load_list;

  /* function calling */


  return o;

};



/* t_filehistory */

p_filehistory  _filehistory_init ( p_filehistory o, t_rect r, p_list list, l_int limit, l_int flags, p_filelistbox filebox )
{

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_filehistory));

  history_init(HISTORY(o), r, list, limit, flags);

  o->filebox = filebox;


  /* object's functions */


  /* view's functions */


  /* history's functions */

  HISTORY(o)->show_box = &filehistory_show_box;


  /* filehistory's functions */


  /* function calling */


  return o;

};



/* t_filetextline */

p_filetextline  _filetextline_init ( p_filetextline o, t_rect r, l_int limit, l_int flags, p_filelistbox filebox, p_filehistory filehistory )
{

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_filetextline));

  textline_init(TEXTLINE(o), r, limit, flags);

  o->filebox = filebox;
  o->filehistory = filehistory;


  /* object's functions */

  OBJECT(o)->translate_event = &filetextline_translate_event;


  /* view's functions */


  /* function calling */


  return o;

};



/* t_filelistbox */

/*
   filelistbox - is used for work with files/directories in listbox

   - cols      number of columns
   - flags     flags of listbox + flags of filelistbox

                ...listbox flags

               LF_NONE
               LF_UNDERSEL    select item whenever is mouse under it
               LF_SELECTABLE  select more item for copying

                ...filistbox flags

               FL_SAVE        main button works as save button
               FL_OPEN        main button works as open button
               FL_DIROPEN     main button works as diropen button

   - path      path to directory, from where is the first output of files
   - file      name of file, that is as the first in file textline

   - atrib     file atributions, atrrib of files that may be displayed by
               filelistbox. Definitions are defined in _iodir.h and <dir.h>

               `FA_RDONLY'    Include read-only files in the search
               `FA_HIDDEN'    Include hidden files in the search
               `FA_SYSTEM'    Include system files in the search
               `FA_LABEL'     Include the volume label in the search
               `FA_DIREC'     Include subdirectories in the search
               `FA_ARCH'      Include modified files in the search
               ...in _iodir.h
               `FA_LINK'      link to file
               `FA_LINKDIR'   link to dir
               `FA_LINKDRIVE' link to drive
               `FA_NOTPARENT' not show parent directory ".."

  - file_sel
               function that is called for all files, when file is focused,
               argument is [path to file/filename] of current file.

*/
p_filelistbox  _filelistbox_init ( p_filelistbox o, t_rect r, l_int cols, l_int flags, l_text path, l_text file, l_int attrib, void (*file_sel)() )
{

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_filelistbox));

  listbox_init(LISTBOX(o), r, NULL, cols, flags);

  o->file = _strdup(file);
  o->path = _strdup(path);
  o->attrib = attrib;
  o->file_sel = file_sel;


  /* object's functions */

  l_tag_cpy(OBJECT(o)->data_type, DAT_LIST);

  OBJECT(o)->setup = &filelistbox_setup;
  OBJECT(o)->set_data = &filelistbox_set_data;
  OBJECT(o)->get_data = &filelistbox_get_data;
  OBJECT(o)->translate_event = &filelistbox_translate_event;


  /* view's functions */

  VIEW(o)->drag_where = &filelistbox_drag_where;


  /* listbox's functions */

  LISTBOX(o)->rewrite_item = &filelistbox_rewrite_item;
  LISTBOX(o)->get_selected_items = &filelistbox_get_selected_items;
  LISTBOX(o)->flags = flags;


  /* filelistbox's functions */

  o->load_list = &filelistbox_load_list;
  o->file_menu = &filelistbox_file_menu;
  o->get_file = &filelistbox_get_file;
  o->file_handle_ex = &filelistbox_file_handle_ex;
  o->file_handle = &filelistbox_file_handle;
  o->change_name = &filelistbox_change_name;
  o->new_directory = &filelistbox_new_directory;


  /* function calling */


  return o;

};



/* dialogs */

/*
   main file-dialog window

   - caption   name of window
   - bname     name of main button
   - path      path to directory, from where is the first output of files
   - file      name of file, that is as the first in file textline
   - hislist   list of file extensions

       hislist making...

       p_list p = list_init(_malloc(sizeof(t_list)), 0, free_filehistory_item);

      p->insert(p, new_filehistory_item("All files", "*.*"))
      p->insert(p, new_filehistory_item("Bitmap files", "*.bmp"))
      ....

  - atrib      file atributions, atrrib of files that may be displayed by
               filelistbox. Definitions are defined in _iodir.h and <dir.h>

               `FA_RDONLY'    Include read-only files in the search
               `FA_HIDDEN'    Include hidden files in the search
               `FA_SYSTEM'    Include system files in the search
               `FA_LABEL'     Include the volume label in the search
               `FA_DIREC'     Include subdirectories in the search
               `FA_ARCH'      Include modified files in the search
               ...in _iodir.h
               `FA_LINK'      link to file
               `FA_LINKDIR'   link to dir
               `FA_LINKDRIVE' link to drive
               `FA_NOTPARENT' not show parent directory ".."

  - flags      flags of filelistbox

               FL_SAVE        main button works as save button
               FL_OPEN        main button works as open button
               FL_DIROPEN     main button works as diropen button

  - file_sel
               function that is called for all files, when file is focused,
               argument is [path to file/filename] of current file.

*/

static t_point  fapp_size_minimum ( p_view o )
{
  return point_assign(300, 200);
};

l_text  _file_dialog ( l_text caption, l_text bname, l_text path, l_text file, p_list hislist, l_int attrib, l_int flags, void (*file_sel)() )
{

   l_text endfile = NULL;

   p_appwin w;

   t_rect r = rect_assign(0, 0, 400, 320);

   if ( flags & FL_DIROPEN )  { /* it's dir dialog */

       r.b.x -= 30;
       r.b.y -= 40;

   };

   w = appwin_init(_malloc(sizeof(t_appwin)), r, caption, 0, 0, NULL);

   if ( w ) {

      p_button   b = NULL;
      p_button   c = NULL;
      p_dirhistory   h = NULL;
      p_filelistbox  box = NULL;
      p_filehistory  d = NULL;
      p_filetextline tl = NULL;
      p_stattext     st_fname = NULL;
      p_stattext     st_ftype = NULL;
      p_stattext     st_links = NULL;
      t_rect     t;
      t_rect     f;
      t_rect     s;

      r = t = VIEW(w)->size_limits(VIEW(w));

      r.a.x += 10;
      r.a.y += 10;
      r.b.x -= 10;
      r.b.y -= 10;

      t = f = r;

      VIEW(w)->drag_mode |= DM_DRAGGROW;
      VIEW(w)->size_minimum = &fapp_size_minimum;

      r = rect_assign(r.a.x, r.a.y+30, t.b.x, r.a.y+190);

      box = filelistbox_init(_malloc(sizeof(t_filelistbox)), r, 3, LF_SELECTABLE|flags, path, (l_text)FILELISTBOX_ITEM_FILENAME(hislist, 0), attrib, file_sel);

      f = rect_assign(r.a.x+70, t.a.y, t.a.x+250, t.a.y+20);

      s = rect_assign(f.a.x-60, f.a.y, f.a.x-10, f.b.y);

      st_links = stattext_init(_malloc(sizeof(t_stattext)), s, TX_ALIGN_BOTTOM, "%s :", TXT_LINKS);

      h = dirhistory_init(_malloc(sizeof(t_dirhistory)), f, IO_TEXT_LIMIT, HF_REWRITEUNABLE, box);

      r = rect_assign(r.a.x+100, r.b.y+STANDARD_SCROLLBAR_WIDTH+20, r.b.x-STANDARD_HISTORYBUTTON_SIZE-120, r.b.y+STANDARD_SCROLLBAR_WIDTH+40);

      r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+30);

      s = rect_assign(r.a.x-100, r.a.y, r.a.x-10, r.b.y);

      if ( !(flags & FL_DIROPEN) ) {  /* it's file dialog */

            st_ftype = stattext_init(_malloc(sizeof(t_stattext)), s, TX_ALIGN_BOTTOM, "%s :", TXT_FILETYPE);

            d = filehistory_init(_malloc(sizeof(t_filehistory)), r, hislist, IO_TEXT_LIMIT, HF_REWRITEUNABLE, box);

            f = rect_assign(r.a.x, r.a.y-30, r.b.x, r.a.y-10);

            s = rect_assign(f.a.x-100, f.a.y, f.a.x-10, f.b.y);

            st_fname = stattext_init(_malloc(sizeof(t_stattext)), s, TX_ALIGN_BOTTOM, "%s :", TXT_FILENAME);

            tl = filetextline_init(_malloc(sizeof(t_filetextline)), f, IO_TEXT_LIMIT, TF_NONE, box, d);

            r = rect_assign(r.b.x+STANDARD_HISTORYBUTTON_SIZE+10, r.a.y, t.b.x, r.b.y);

            b = button_init(_malloc(sizeof(t_button)), r, bname, MSG_OK, BF_DEFAULT);

            /* button alignment */
            if  ( b ) VIEW(b)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_RIGHT|TX_ALIGN_FIXEDX|TX_ALIGN_FIXEDY;

      } else { /* it's directory dialog */

            r = rect_assign(t.a.x, t.b.y-20, t.b.x, t.b.y);

            b = button_init(_malloc(sizeof(t_button)), r, bname, MSG_OPENDIR, BF_DEFAULT);

            /* button alignment */
            if  ( b ) VIEW(b)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_RIGHT|TX_ALIGN_FIXEDY;
      };

      /* window alignment */
      VIEW(w)->align = TX_ALIGN_CENTER;

      /* filebox alignment */
      if  ( box ) VIEW(box)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_RIGHT;

      /* filehistory alignment */
      if  ( d ) VIEW(d)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_RIGHT|TX_ALIGN_FIXEDY;

      /* filetextline alignment */
      if  ( tl ) VIEW(tl)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_RIGHT|TX_ALIGN_FIXEDY;

      /* filename stattext */
      if  ( st_fname ) VIEW(st_fname)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_FIXEDX|TX_ALIGN_FIXEDY;

      /* filetype stattext */
      if  ( st_ftype ) VIEW(st_ftype)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_FIXEDX|TX_ALIGN_FIXEDY;


      OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(w));

      OBJECT(w)->insert(OBJECT(w), OBJECT(h));
      OBJECT(w)->insert(OBJECT(w), OBJECT(b));
      OBJECT(w)->insert(OBJECT(w), OBJECT(box));
      OBJECT(w)->insert(OBJECT(w), OBJECT(tl));
      OBJECT(w)->insert(OBJECT(w), OBJECT(d));
      OBJECT(w)->insert(OBJECT(w), OBJECT(st_fname));
      OBJECT(w)->insert(OBJECT(w), OBJECT(st_ftype));
      OBJECT(w)->insert(OBJECT(w), OBJECT(st_links));

      if ( tl ) /* not directory, not line */

            TEXTLINE(tl)->set_text(TEXTLINE(tl), file);

      if ( desktop->execute_view(desktop, VIEW(w)) == MSG_OK ) {

         l_text file1 = box?box->file:NULL;
         l_text filename = tl?TEXTLINE(tl)->text:file1;
         l_text pathname = box?box->path:NULL;

         endfile  = io_realpath(pathname, filename);

      };

      dispose(OBJECT(w));

   };

   return endfile;

};



#include"iodlg.exp"

lib_begin ( void ) {

  if ( ap_process == AP_ALLOC ) {

          AP_EXPORTLIB(); /* export table in app.exp */

  } else

  if ( ap_process == AP_INIT ) {

          AP_SETNUMOFCALLS(1); /* set max num of calls */

          datfile = conv_to_skipcolor_data(DLXGetFileData(ap_id), CO_SKIP_RED, CO_SKIP_GREEN, CO_SKIP_BLUE);

  } else

  if ( ap_process == AP_FREE ) {

    unload_datafile(datfile);
    datfile = NULL;

  };


} lib_end;

