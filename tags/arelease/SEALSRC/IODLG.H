/****************************************************************/
/*                                                              */
/*                           iodlg.h                            */
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



#ifndef __IODS_H_INCLUDED__
#define __IODS_H_INCLUDED__

#include<stdarg.h>
#include"dialogs.h"

#define IO_TEXT_LIMIT   IO_FILE_LIMIT


/* input/output standard strings */

#ifndef TXT_NEW
#define TXT_NEW       INI_TEXT("New")
#endif
#ifndef TXT_DIRECTORY
#define TXT_DIRECTORY       INI_TEXT("Directory")
#endif
#ifndef TXT_LINKTOFILE
#define TXT_LINKTOFILE       INI_TEXT("Link to file")
#endif
#ifndef TXT_LINKTODIRECTORY
#define TXT_LINKTODIRECTORY  INI_TEXT("Link to directory")
#endif
#ifndef TXT_LINKS
#define TXT_LINKS       INI_TEXT("Links")
#endif
#ifndef TXT_FILETYPE
#define TXT_FILETYPE    INI_TEXT("File type")
#endif
#ifndef TXT_CHANGENAME
#define TXT_CHANGENAME  INI_TEXT("Change name")
#endif
#ifndef TXT_ICONFORLINK
#define TXT_ICONFORLINK INI_TEXT("Icon for link")
#endif
#ifndef TXT_OPEN
#define TXT_OPEN        INI_TEXT("Open")
#endif
#ifndef TXT_SAVE
#define TXT_SAVE        INI_TEXT("Save")
#endif
#ifndef TXT_COPY
#define TXT_COPY        INI_TEXT("Copy")
#endif
#ifndef TXT_DELETE
#define TXT_DELETE      INI_TEXT("Delete")
#endif
#ifndef TXT_PASTE
#define TXT_PASTE       INI_TEXT("Paste")
#endif
#ifndef TXT_PROPERTIES
#define TXT_PROPERTIES  INI_TEXT("Properties")
#endif
#ifndef TXT_NEWDIRECTORY
#define TXT_NEWDIRECTORY  INI_TEXT("New directory")
#endif
#ifndef TXT_FILENAME
#define TXT_FILENAME    INI_TEXT("File name")
#endif
#ifndef TXT_DIRNAME
#define TXT_DIRNAME     INI_TEXT("Directory name")
#endif
#ifndef TXT_DATE
#define TXT_DATE        INI_TEXT("Date")
#endif
#ifndef TXT_TIME
#define TXT_TIME        INI_TEXT("Time")
#endif
#ifndef TXT_SIZE
#define TXT_SIZE        INI_TEXT("Size")
#endif
#ifndef TXT_PATH
#define TXT_PATH        INI_TEXT("Path")
#endif
#ifndef TXT_NOTFOUND
#define TXT_NOTFOUND    INI_TEXT("Not found")
#endif
#ifndef TXT_ATTRIB
#define TXT_ATTRIB      INI_TEXT("Attrib")
#endif
#ifndef TXT_NUMBEROFFILES
#define TXT_NUMBEROFFILES  INI_TEXT("Number of files")
#endif
#ifndef TXT_NUMBEROFDIRECTORIES
#define TXT_NUMBEROFDIRECTORIES  INI_TEXT("Number of directories")
#endif
#ifndef TXT_PROCESSSTATUS
#define TXT_PROCESSSTATUS        INI_TEXT("Process status")
#endif
#ifndef TXT_DONE
#define TXT_DONE                 INI_TEXT("Done")
#endif
#ifndef TXT_HALTED
#define TXT_HALTED               INI_TEXT("Halted")
#endif
#ifndef TXT_PLEASEWAIT
#define TXT_PLEASEWAIT           INI_TEXT("Please wait")
#endif
#ifndef TXT_COPYTOFILE
#define TXT_COPYTOFILE           INI_TEXT("Copy to file")
#endif
#ifndef TXT_COPYTODIRECTORY
#define TXT_COPYTODIRECTORY      INI_TEXT("Copy to directory")
#endif
#ifndef TXT_AREYOUSUREYOUWANTTODELETE
#define TXT_AREYOUSUREYOUWANTTODELETE  INI_TEXT("Are you sure you want to delete")
#endif
#ifndef TXT_THESEFILES
#define TXT_THESEFILES                 INI_TEXT("These files")
#endif
#ifndef TXT_AREYOUSUREYOUWANTTOCOPY
#define TXT_AREYOUSUREYOUWANTTOCOPY    INI_TEXT("Are you sure you want to copy")
#endif
#ifndef TXT_FILE
#define TXT_FILE                       INI_TEXT("File")
#endif
#ifndef TXT_INTO
#define TXT_INTO                       INI_TEXT("Into")
#endif
#ifndef TXT_COPYINGFILES
#define TXT_COPYINGFILES               INI_TEXT("Copying files")
#endif
#ifndef TXT_DELETINGFILES
#define TXT_DELETINGFILES              INI_TEXT("Deleting files")
#endif
#ifndef TXT_COPYINGFILESTO
#define TXT_COPYINGFILESTO             INI_TEXT("Copying files to")
#endif
#ifndef TXT_DELETINGFILESFROM
#define TXT_DELETINGFILESFROM          INI_TEXT("Deleting files from")
#endif
#ifndef TXT_FILECOULDNTBERENAME
#define TXT_FILECOULDNTBERENAME        INI_TEXT("File couldn't be rename")
#endif
#ifndef TXT_AREYOUSTUPID
#define TXT_AREYOUSTUPID               INI_TEXT("Are you stupid")
#endif
#ifndef TXT_FILE
#define TXT_FILE                       INI_TEXT("File")
#endif
#ifndef TXT_NOTFOUND
#define TXT_NOTFOUND                   INI_TEXT("Not found")
#endif
#ifndef TXT_CHECKTHEFILENAMEANDTRYAGAIN
#define TXT_CHECKTHEFILENAMEANDTRYAGAIN  INI_TEXT("Check the filename and try again")
#endif
#ifndef TXT_EXIST
#define TXT_EXIST                      INI_TEXT("Exist")
#endif
#ifndef TXT_OPENFILE
#define TXT_OPENFILE                   INI_TEXT("Open file")
#endif
#ifndef TXT_SAVEFILEAS
#define TXT_SAVEFILEAS                 INI_TEXT("Save file as")
#endif
#ifndef TXT_WOULDYOULIKETOOVERWRITEIT
#define TXT_WOULDYOULIKETOOVERWRITEIT  INI_TEXT("Would you like to overwrite it")
#endif
#ifndef TXT_REFERENCETODIRECTORY
#define TXT_REFERENCETODIRECTORY       INI_TEXT("Reference to directory")
#endif


/* input/output info strings */

#ifndef TXT_INFOLINKTOFILE
#define TXT_INFOLINKTOFILE       INI_TEXT("")
#endif
#ifndef TXT_INFOLINKTODIRECTORY
#define TXT_INFOLINKTODIRECTORY  INI_TEXT("")
#endif
#ifndef TXT_INFOCHANGENAME
#define TXT_INFOCHANGENAME  INI_TEXT("")
#endif
#ifndef TXT_INFOOPEN
#define TXT_INFOOPEN        INI_TEXT("")
#endif
#ifndef TXT_INFONEWDIRECTORY
#define TXT_INFONEWDIRECTORY  INI_TEXT("")
#endif
#ifndef TXT_INFOCOPY
#define TXT_INFOCOPY        INI_TEXT("")
#endif
#ifndef TXT_INFODEL
#define TXT_INFODEL         INI_TEXT("")
#endif
#ifndef TXT_INFOPASTE
#define TXT_INFOPASTE       INI_TEXT("")
#endif
#ifndef TXT_INFOPROPERTIES
#define TXT_INFOPROPERTIES  INI_TEXT("")
#endif
#ifndef TXT_INFOCOPYTOFILE
#define TXT_INFOCOPYTOFILE           INI_TEXT("")
#endif
#ifndef TXT_INFOCOPYTODIRECTORY
#define TXT_INFOCOPYTODIRECTORY      INI_TEXT("")
#endif


/* input/output messages */

#ifndef MSG_SELECTEDFILELISTBOXITEM
#define MSG_SELECTEDFILELISTBOXITEM   105
#endif

/* local messages */

/* message for copying file into other one in function
   menu_file_copy_where
*/
#ifndef MSG_COPYTOFILE
#define MSG_COPYTOFILE                106
#endif
/* message for copying files into directort in function
   menu_file_copy_where
*/
#ifndef MSG_COPYTODIR
#define MSG_COPYTODIR                 107
#endif
/* message for getting properties about file / directory */
#ifndef MSG_PROPERTIES
#define MSG_PROPERTIES                108
#endif
/* message for changing name of file / directory */
#ifndef MSG_CHANGENAME
#define MSG_CHANGENAME                109
#endif
/* message for making new directory */
#ifndef MSG_NEWDIRECTORY
#define MSG_NEWDIRECTORY              110
#endif
/* message called in filelistbox_translate_event for dir opening */
#ifndef MSG_OPENDIR
#define MSG_OPENDIR                   111
#endif
/* make new link to directory */
#ifndef MSG_NEWLINKTODIRECTORY
#define MSG_NEWLINKTODIRECTORY        112
#endif
/* make new link to file */
#ifndef MSG_NEWLINKTOFILE
#define MSG_NEWLINKTOFILE             113
#endif


/* filelistbox flags */

/* this flags are used for setting up the options of the filelistbox. they
   continued from the LIF_XXXX - listbox flags, you will find in dialogs.h */

/* filelistbox can open the file */
#define FL_OPEN                       0x00010  /* listbox flags before */
/* filelistbox can save the file */
#define FL_SAVE                       0x00020
/* filelistbox can return the name of opened directory if you pressed OK */
#define FL_DIROPEN                    0x00040



/* filehistory item structure */

typedef struct t_filehistory_item *p_filehistory_item;


/*
  this structire of item is used for displaying items in filelistbox or filehistory
  objects. It continued from t_listbox_item as you can see. It's only increased by
  file info about the item ( because each item repesented one file/directory ) and
  function "properties" that's called when the properties are called for the specfic
  file. "set_data" is used whenever something is copying into the file.
*/

/* filelistbox item structure */

typedef struct t_filelistbox_item *p_filelistbox_item;

typedef struct t_filelistbox_item {

  /* inherited variables from the t_listbox_item */
  struct t_listbox_item   obclass;

  /* info about the file/dir that represents this item...see _iodir.h */
  struct t_file           info;

  /* function, calling whenever properties about the file are called */
  l_bool                  (*properties)(l_text filename);
  /* function, calling whenever something is copying into the file */
  l_int                   (*set_data) ( p_object o, t_file *dst, t_file *src, l_dword *ind );

} t_filelistbox_item;


/* get the hour from the t_filelistbox_item.info.time */
#define ff_hour(t)   (((t) >> 11) & 0x1f)
/* get the min from the t_filelistbox_item.info.time */
#define ff_min(t)    (((t) >>  5) & 0x3f)
/* get the sec from the t_filelistbox_item.info.time */
#define ff_sec(t)    (((t) & 0x1f) << 1)
/* get the day from the t_filelistbox_item.info.date */
#define ff_day(d)    ((d) & 0x1f)
/* get the month from the t_filelistbox_item.info.date */
#define ff_month(d)  (((d) >>  5) & 0x0f)
/* get the year from the t_filelistbox_item.info.date */
#define ff_year(d)   ((((d) >> 9) & 0x7f) + 1980)


/* t_filelistbox */

/*
  object filelistbox is used for displaying and controling the box of items that
  contains information about files / directories. Each entry ( item ) of the
  t_filelistbox.obclass.list contains information about an item that contains pointer
  to one t_filelistbox_item entry. Whenever you press the item you can copy/paste it
  by the menu or by the DRAG & DROP system.
*/

typedef struct t_filelistbox *p_filelistbox;

typedef struct t_filelistbox {

   /* inherite variables / functions from t_listbox, t_view, t_object classes */
   struct t_listbox  obclass;

   /* define current path of box */
   l_text            path;
   /* define the name of the file, that's selected. If the directory, then it's
      extension until it's not changed in the t_filetextline object
   */
   l_text            file;
   /*
     attributes of files / directories that can show current box. see _iodir.h.
   */
   l_int             attrib;

   /*
     this function is called whenever is file selected. Function contains pointer to
     the function you set in the initializing. By this way it's possible to support
     quick browser accross images for example. This function is called by one argument
     that represents the file that was selected.
   */
   void            (*file_sel)();

   /*
     reload list of the items for path "path" and for extension "file". It redraw the
     box and set all to initial values.
   */
   void            (*load_list) ( p_filelistbox o, l_text path, l_text file );

   /*
     show the menu that's visible whenever the 3rd button on the mouse is pressed under
     any file / directory. It returns the message that was called. If nothing, it
     returns MSG_CANCEL.
   */
   l_dword         (*file_menu) ( p_filelistbox o );

   /*
     this function is called from the "filelistbox_translate_event" function and
     handle operations about directories or files for specfic attributes. It's called
     whenever is item pressed and opened.
   */
   l_int           (*file_handle_ex) ( p_filelistbox o, l_text opath, l_text ofile, l_int attrib, l_text filename );

   /*
     it call function file_handle_ex for current values of selected item.
   */
   l_int           (*file_handle) ( p_filelistbox o );

   /*
     function, used for the changing of the current file / directory name. returns true
     if the name was succesfull changed, otherwise it returns false.
   */
   l_bool          (*change_name) ( p_filelistbox o );

   /*
     return the real name of the file that was selected
   */
   l_text          (*get_file) ( p_filelistbox o );

   /*
     function for making new directory. If directory was successfull made, it returns
     true, otherwise it returns false.
   */
   l_bool          (*new_directory) ( p_filelistbox o );

   /*
     function for making new link to directory. If this link was successfull made,
     it returns true, otherwise it returns false.
   */
   l_bool          (*new_linkdirectory) ( p_filelistbox o );

   /*
     function for making new link to file. If this link was successfull made,
     it returns true, otherwise it returns false.
   */
   l_bool          (*new_link) ( p_filelistbox o );

} t_filelistbox;

#define FILELISTBOX(o) ((p_filelistbox)(o))

#define FILELISTBOX_ITEM_ATTRIB(o, i)((o)?((p_filelistbox_item)(o)->at((o), i))->info.attrib:0)
#define FILELISTBOX_ITEM_DATE(o, i)  ((o)?((p_filelistbox_item)(o)->at((o), i))->info.date:0)
#define FILELISTBOX_ITEM_TIME(o, i)  ((o)?((p_filelistbox_item)(o)->at((o), i))->info.time:0)
#define FILELISTBOX_ITEM_SIZE(o, i)  ((o)?((p_filelistbox_item)(o)->at((o), i))->info.size:0)
#define FILELISTBOX_ITEM_FILENAME(o, i)  ((o)?((p_filelistbox_item)(o)->at((o), i))->info.filename:NULL)
#define FILELISTBOX_ITEM_PATH(o, i)  ((o)?((p_filelistbox_item)(o)->at((o), i))->info.path:NULL)



/* t_dirhistory */

/*
  object t_dirhistory is used for showing drives that's placed in the ./DESKTOP/
  directory.
*/

typedef struct t_dirhistory *p_dirhistory;

typedef struct t_dirhistory {

   /* inherite variable / functions from t_history, t_textline, t_view, t_object classes */
   struct t_history  obclass;

   /* point to filelistbox, that's change the box of items / files / directories
      whenever the item in the history is changed
   */
   p_filelistbox     filebox;

   /* load the list of items from the "path" => standard is ./DESKTOP/ */
   void            (*load_list) ( p_dirhistory o, l_text path );

} t_dirhistory;

#define DIRHISTORY(o) ((p_dirhistory)(o))



/* t_filehistory */

/*
  object t_filehistory is used for showing extension of available files in the
  history box. The "rec" of items in the t_filehistory.obclass.list is structured
  by the format t_filehistory_item ...see above. It means, that each item contains
  the same information as in t_filelistbox_item. You are able to make an item by
  function new_filehistory_item you will see later in this section.
*/

typedef struct t_filehistory *p_filehistory;

typedef struct t_filehistory {

   /* inherite variable / functions from t_history, t_textline, t_view, t_object classes */
   struct t_history  obclass;

   /* point to filelistbox, that's change the box of items / files / directories
      whenever the item in the history is changed
   */
   p_filelistbox     filebox;

} t_filehistory;

#define FILEHISTORY(o) ((p_filehistory)(o))


/* t_filetextline */

/*
  object t_filetextline is used for manipulating with the name of files and by
  this way you can save data to other file, than exists.
*/

typedef struct t_filetextline *p_filetextline;

typedef struct t_filetextline {

   /* inherite variable / functions from t_history, t_textline, t_view, t_object classes */
   struct t_history  obclass;

   /* point to filelistbox, that's change the box of items / files / directories
      whenever the text in "obclass.obclass.text" was changed
   */
   p_filelistbox     filebox;

   /* point to filehistory, that contains information about extensions and whenever
      is extension changed it automaticaly change "obclass.obclass.text" to the
      selected extension of filehistory.
   */
   p_filehistory     filehistory;

   /*
     the old/current name of file ( extension ) if the directory was selected.
   */
   l_text            filename;

} t_filetextline;

#define FILETEXTLINE(o) ((p_filetextline)(o))


/*
  returns list, that contains items (p_item) and each item contains (rec) - pointer
  to one t_filelistbox_item. => returns list of files / directories / links defined
  in "flags" from the path "path} and exetension "files". "isize" is the standard
  size of the icon. You can use ICON_16 (default), or ICON_32. To get what's the
  default size of icon use "system_item_size" from "view.h". "flags" may be
  combination of FA_XXXX flags from _iodir.h. FA_ALL get all files, FA_DIR only
  directories.

*/
p_list  get_listbox_item_dirfiles ( l_text path, l_text files, l_int flags, l_int isize );

/*
  make new t_filelistbox_item and return pointer to this item.
  "name" is the text that will be displayed in the item. ( directory / file ).
  "icon" is the icon of item, "sel" defines if the item was automatically selected,
  "flags" defines flags for listbox, see LF_XXXX in "dialogs.h". "attrib" may contains
  combination of FA_XXXX file atributes, see "_iodir.h". "date" is the packed date
  of file / directory when was created, "time" is packed time of file / dir when was
  created. "size" is the size of file, if it's dir, the default value is ZERO for this
  value. "path" is the path of the file. "filename" is the real name of the file. For
  example, the difference between "name" and "filename" is in links, because SEAL not
  displayes extension of link. "properties" is pointer to the function that show
  the properties about the file / directory. "set_data" is pointer to the function
  that copy data from file "src" to this file / directory.
*/
p_filelistbox_item   new_filelistbox_item_ex ( l_text name, BITMAP *icon,
                                               l_bool sel, l_int flags,
                                               l_int attrib, l_word date,
                                               l_word time, l_dword size,
                                               l_text path, l_text filename,
                                               l_bool (*properties)(l_text filename),
                                               l_int (*set_data)(p_object o, t_file *dst, t_file *src, l_dword *ind ) );

/* free previous allocated item "t_filelistbox_item" */

void                 free_filelistbox_item ( void *o );

#define              new_filelistbox_item(n,i,se,f,a,d,t,s,p,fn) new_filelistbox_item_ex(n, i, se, f, a, d, t, s, p, fn, NULL, NULL)
#define              new_filehistory_item(n,f)  new_filelistbox_item(n, NULL, false, 0, 0, 0, 0, 0, NULL, f)
#define              free_filehistory_item      free_filelistbox_item


/*
  set properties for link to file if "filelink" is true, or link to directory
  when "filelink" is false. the "path" is the name of file that contains info about
  link, and "linkinfo" is the link to other file / directory.
  returns true if all was succesfull, otherwise returns false.
*/
l_bool link_properties ( l_text filename, l_text linkinfo, l_bool filelink );

/*
  make new link to file if "filelink" is true, or link to directory when "filelink"
  is false. the "path" is the new name of file that we will create and "linkinfo" is
  the link to other file / directory. returns true if all was succesfull, otherwise
  returns false.
*/
l_bool link_new ( l_text path, l_text linkinfo, l_bool filelink );

/* t_filelistbox_item functions */

/*
  get properties about file "filename" and return true if file exists, otherwise
  it returns false
*/
l_bool file_properties ( l_text filename );

/*
  get properties about directory "filename" and return true if directory exists,
  otherwise it returns false
*/
l_bool dir_properties ( l_text filename );

/*
  get properties about link to directory "filename" and return true if link exists,
  otherwise it returns false
*/
l_bool linkdir_properties ( l_text filename );

/*
  get properties about link to file "filename" and return true if link exists,
  otherwise it returns false
*/
l_bool linkfile_properties ( l_text filename );

/*
  copy data from file "t_file" to file "dst". "o" is the object that will be checked
  and process will be continued until object "o" will not be closed.
  "ind" is the current position of copying.
*/
l_int file_set_data ( p_object o, t_file *dst, t_file *src, l_dword *ind );

/*
  copy data from file/directory "t_file" to directory "dst". "o" is the object that
  will be checked and process will be continued until object "o" will not be closed.
  "ind" is the current position of copying.
*/
l_int dir_set_data ( p_object o, t_file *dst, t_file *src, l_dword *ind );

/*
  copy data from file "t_file" to directory "dst". "o" is the object that
  will be checked and process will be continued until object "o" will not be closed.
  "ind" is the current position of copying.
*/
l_int linkdir_set_data ( p_object o, t_file *dst, t_file *src, l_dword *ind );

/*
  copy data from file "t_file" to directory "dst". "o" is the object that
  will be checked and process will be continued until object "o" will not be closed.
  "ind" is the current position of copying.
*/
l_int linkfile_set_data ( p_object o, t_file *dst, t_file *src, l_dword *ind );


/* t_filelistbox functions */

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

extern p_filelistbox  (*filelistbox_init) ( p_filelistbox o, t_rect r, l_int cols, l_int flags, l_text path, l_text file, l_int attrib, void (*file_sel)() );
p_filelistbox  _filelistbox_init ( p_filelistbox o, t_rect r, l_int cols, l_int flags, l_text path, l_text file, l_int attrib, void (*file_sel)() );


void    filelistbox_setup ( p_object o );
l_bool  filelistbox_drag_where ( p_view o, p_data rec, t_point where );
void    filelistbox_rewrite_item ( p_listbox o, l_long item );
p_list  filelistbox_get_selected_items ( p_listbox o, l_bool sel );

void    filelistbox_load_list ( p_filelistbox o, l_text path, l_text file );
l_dword filelistbox_file_menu ( p_filelistbox o );
l_text  filelistbox_get_file ( p_filelistbox o );
l_int   filelistbox_file_handle_ex ( p_filelistbox o, l_text opath, l_text ofile, l_int attrib, l_text filename );
l_int   filelistbox_file_handle ( p_filelistbox o );
l_bool  filelistbox_change_name ( p_filelistbox o );
l_bool  filelistbox_new_directory ( p_filelistbox o );
l_bool  filelistbox_new_linkdirectory ( p_filelistbox o );
l_bool  filelistbox_new_link ( p_filelistbox o );


/* t_dirhistory functions */

/*
  create new history of drives, where (r) is the area where will be history
  placed. (limit) is the maximum number of characters of drivename. (flags) is
  combination of HF_XXXX flags, ...see dialogs.h. (filelistbox) is the pointer
  to t_filelistbox that's changed whenever is drive from history changed.
*/

extern p_dirhistory  (*dirhistory_init) ( p_dirhistory o, t_rect r, l_int limit, l_int flags, p_filelistbox filebox );
p_dirhistory  _dirhistory_init ( p_dirhistory o, t_rect r, l_int limit, l_int flags, p_filelistbox filebox );

l_dword dirhistory_show_box ( p_history o );

void    dirhistory_load_list ( p_dirhistory o, l_text path );



/* t_filehistory functions */
/*
  create new t_filehistory object, that's used for showing extension of available
  files in the history box. The "rec" of items in the t_filehistory.obclass.list
  is structured by the format t_filehistory_item ...see above. It means, that
  each item contains the same information as in t_filelistbox_item.
  You are able to make an item by function new_filehistory_item you will see later
  in this section. (r) is the area, where will be object placed. (list) is the list of
  items that contains important information stored in t_filelistbox_item, see above.
  (limit) is the maximum number of characters of filehistory text. (flags) is
  combination of HF_XXXX flags, ...see dialogs.h. (filelistbox) is the pointer
  to t_filelistbox that's changed whenever is drive from history changed.
*/
extern p_filehistory  (*filehistory_init) ( p_filehistory o, t_rect r, p_list list, l_int limit, l_int flags, p_filelistbox filebox );
p_filehistory  _filehistory_init ( p_filehistory o, t_rect r, p_list list, l_int limit, l_int flags, p_filelistbox filebox );

l_dword filehistory_show_box ( p_history o );



/* t_filetextline */

/*
  create filetextline, that's used for manipulating with the name of files and by
  this way you can save data to other file, than exists. "r" is the area where the
  line will be displayed. "limit" is the maximum number of characters of filename.
  "flags" is combination of TF_XXXX flags, ...see dialogs.h. "filebox" is the pointer
  to t_filelistbox object and "filehistory" to t_filehistory object.
*/

extern p_filetextline  (*filetextline_init) ( p_filetextline o, t_rect r, l_int limit, l_int flags, p_filelistbox filebox, p_filehistory filehistory );
p_filetextline  _filetextline_init ( p_filetextline o, t_rect r, l_int limit, l_int flags, p_filelistbox filebox, p_filehistory filehistory );

void  filetextline_translate_event ( p_object o, p_event event );



/* dialogs */

l_dword  menu_file_copy_where ( l_text filename );

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

  returns the file that was opened or saved. if the dialog-window was canceled,
  it returns NULL.
*/
extern l_text  (*file_dialog) ( l_text caption, l_text bname, l_text path, l_text file, p_list hislist, l_int attrib, l_int flags, void (*file_sel)() );

/*
  show dialog window with box of directories only
*/
#define dir_dialog(c,bn,p) file_dialog(c, bn, p, NULL, NULL, FA_DIREC, FL_DIROPEN, NULL)

#define save_dialog(p,f,l) file_dialog(TXT_SAVEFILEAS, TXT_SAVE, p, f, l, FA_ALL, FL_SAVE, NULL)
#define open_dialog(p,f,l) file_dialog(TXT_OPENFILE, TXT_OPEN, p, f, l, FA_ALL, FL_OPEN, NULL)

l_text  _file_dialog ( l_text caption, l_text bname, l_text path, l_text file, p_list hislist, l_int attrib, l_int flags, void (*file_sel)() );

#endif

