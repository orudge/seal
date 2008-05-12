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
#include <txts.h>

p_list txtsdir = NULL;

typedef struct txt_info {
  l_int   id;
  l_text  name;
  l_text  ref;  // Defaul language text
  l_text  txt;  // Current language text
} t_txt_info, *p_txt_info;

#define TXTINF(o) ((p_txt_info)(o))
////////////////////////////////////////////////////////////////////////////////
l_text txts_def[] = {"Yes","No","OK","Cancel",
                     "Close","Exit","Help","About",
                     "File","New","Open","Save",
                     "Save As...","Edit","Cut","Copy",
                     "Paste","Search","Find","Find Next",
                     "Find and Replace","Programs","Settings","Documents",
                     "Shut down","Maximize","Minimize","Show",
                     "Hide","No to All","Yes to All","Warning",
                     "Error","Info","Question","Unavailable",
                     "January","February","March","April",
                     "May","June","July","August",
                     "September","October","November","December",
                     "Sunday","Monday","Thuesday","Wednesday",
                     "Thursday","Friday","Saturday",
                     "Welcome!","Directory","File Type","Rename",
                     "Delete","Properties","New Directory",
                     "Filename","Directory Name","Date",
                     "Time","Size","Path","Not Found","Attrib",
                     "Number of Files","Number of Directories",
                     "Process Status","Done","Halted","Please Wait",
                     "Copy to File","Copy to Directory",
                     "Are you sure you want to delete","These files",
                     "Are you sure you want to copy","Into",
                     "Copying files","Deleting files","Copying files to",
                     "Deleting files from","File couldn't be rename",
                     "Are you stupid","Check the filename and try again",
                     "Exist","Open file","Save file as",
                     "Would you like to overwrite it","Reference to directory",
                     "Not enough memory","Not enough memory for operation",
                     "Please store your data and exit some application",
                     "Can't run file","Convert to image...","View/Open",
                     "Views the file in the right window or open a directory",
                     "Here you can convert this file format into another file format",
                     "Convert Now","Close by Alt+F4","Image is converting into file",
					 "Are you sure to exit SEAL?","Display","Display Settings",
					 "Display Settings: Wallpaper, skins, effects, language, etc",
					 "Hardware","Hardware Settings",
					 "Hardware Settings: Sound, keyboard, screen size, etc",
					 "Control Panel",
					 "colour","change","other","(other)","(none)",
					 "System Colours", "pattern","stretch","centre",
					 "Wallpaper","Display","Desktop Wallpaper",
					 "Screen Saver","test","Wait (sec)",
					 "Graphics Effects and Skin", "Skin Selector",
					 "Transparency Level", "Preview","Computer",
					 "Screen Resolution","Screen Size",
					 "Screen Depth","Use double-buffer (better graphics)",
					 "Restart", NULL };

////////////////////////////////////////////////////////////////////////////////
l_text txts_name[] = {"TXT_YES","TXT_NO","TXT_OK","TXT_CANCEL",
                     "TXT_CLOSE","TXT_EXIT","TXT_HELP","TXT_ABOUT",
                     "TXT_FILE","TXT_NEW","TXT_OPEN","TXT_SAVE",
                     "TXT_SAVE_AS","TXT_EDIT","TXT_CUT","TXT_COPY",
                     "TXT_PASTE","TXT_SEARCH","TXT_FIND","TXT_FIND_NEXT",
                     "TXT_FIND_AND_REPLACE","TXT_PROGRAMS","TXT_SETTINGS","TXT_DOCUMENTS",
                     "TXT_SHUT_DOWN","TXT_MAXIMIZE","TXT_MINIMIZE","TXT_SHOW",
                     "TXT_HIDE","TXT_NO_TO_ALL","TXT_YES_TO_ALL","TXT_WARNING",
                     "TXT_ERROR","TXT_INFO","TXT_QUESTION","TXT_UNAVAIBLE",
                     "TXT_JANUARY","TXT_FEBRUARY","TXT_MARCH","TXT_APRIL",
                     "TXT_MAY","TXT_JUNE","TXT_JULY","TXT_AUGUST",
                     "TXT_SEPTEMBER","TXT_OCTOBER","TXT_NOVEMBER","TXT_DECEMBER",
                     "TXT_SUNDAY","TXT_MONDAY","TXT_THUESDAY","TXT_WEDNESDAY",
                     "TXT_THURSDAY","TXT_FRYDAY","TXT_SATURDAY",
                     "TXT_WELCOME", "TXT_DIRECTORY","TXT_FILETYPE",
                     "TXT_CHANGENAME","TXT_DELETE","TXT_PROPERTIES",
                     "TXT_NEWDIRECTORY","TXT_FILENAME","TXT_DIRNAME",
                     "TXT_DATE","TXT_TIME","TXT_SIZE","TXT_PATH",
                     "TXT_NOTFOUND","TXT_ATTRIB","TXT_NUMBEROFFILES",
                     "TXT_NUMBEROFDIRECTORIES","TXT_PROCESSSTATUS",
                     "TXT_DONE","TXT_HALTED","TXT_PLEASEWAIT",
                     "TXT_COPYTOFILE","TXT_COPYTODIRECTORY",
                     "TXT_AREYOUSUREYOUWANTTODELETE","TXT_THESEFILES",
                     "TXT_AREYOUSUREYOUWANTTOCOPY","TXT_INTO",
                     "TXT_COPYINGFILES","TXT_DELETINGFILES",
                     "TXT_COPYINGFILESTO","TXT_DELETINGFILESFROM",
                     "TXT_FILECOULDNTBERENAME","TXT_AREYOUSTUPID",
                     "TXT_CHECKTHEFILENAMEANDTRYAGAIN","TXT_EXIST",
                     "TXT_OPENFILE","TXT_SAVEFILEAS",
                     "TXT_WOULDYOULIKETOOVERWRITEIT","TXT_REFERENCETODIRECTORY",
                     "TXT_NOTENOUGHMEMORY","TXT_NOTENOUGHMEMEMORYFOROPERATION",
                     "TXT_PLEASESTOREYOURDATAANDEXITSOMEAPPLICATION",
                     "TXT_CANTRUNFILE","TXT_CONVERTTOIMAGE",
                     "TXT_VIEWFILE","TXT_INFOVIEWFILE","TXT_INFOCONVERTTOIMAGE",
                     "TXT_CONVERTNOW","TXT_CLOSEBYALTF4",
                     "TXT_IMAGEISCONVRTINGINTOFILE","TXT_AREYOUSURETOQUITSEAL",
                     "TXT_DISPLAY","TXT_DISPLAYSETTINGS",
					 "TXT_DISPLAYINFO","TXT_HARDWARE",
					 "TXT_HARDWARESETTINGS","TXT_HARDWAREINFO","TXT_CONTROL_PANEL",
					 "TXT_COLOR","TXT_CHANGE","TXT_OTHER","TXT__OTHER_","TXT__NONE_",
					 "TXT_SYSTEM_COLORS","TXT_PATERN","TXT_STRETCH","TXT_CENTER",
					 "TXT_WALLPAPER","TXT_WALLPAPER_ALIGN","TXT_DESKTOP_WALLPAPER",
					 "TXT_SCREEN_SAVER","TXT_TEST","TXT_WAIT__SEC_",
					 "TXT_GRAPHICS_EFFECTS_AND_SKINS", "TXT_SKIN_SELECTROR",
					 "TXT_TRANSLUCIDE_LEVEL", "TXT_PREVIEW","TXT_COMPUTER",
					 "TXT_SCREEN_DEFINITION","TXT_SCREEN_SIZE",
					 "TXT_SCREEN_DEPTH","TXT_SCREEN_DBL_BUFFER_INFO",
					 "TXT_RESTART", NULL };


////////////////////////////////////////////////////////////////////////////////
l_int txt_id ( l_text regname ) {
  l_int a = 0;
  while ( txts_name[a] ) {
    if ( !stricmp(txts_name[a], regname ) ) return a+1;
    a++;
  };
  return 0;
};
////////////////////////////////////////////////////////////////////////////////
l_text lngfile = NULL;
l_text lng     = NULL;
void load_txts ( void ) {

  if ( key_exists("system/language/lngfile") ) lngfile = get_key("system/language/lngfile");
  if ( key_exists("system/language/lng") ) lng = get_key("system/language/lng");

  if ( lngfile && lng ) {
    ini_data *d = getinidata_fromfile(lngfile, lng);
    txtsdir = list_init(malloc(sizeof(t_list)),NULL,NULL);
    if ( d ) {
      int lines = getini_linenum ( d );
      int i = 1;
      while ( i < lines ) {
        char *name = NULL;
        char *txt = getini_line ( &name, d, i );
        p_txt_info o = malloc(sizeof(t_txt_info));
        clear_type(o, sizeof(t_txt_info));
        o->id = txt_id(name);
        if ( stricmp(lng,"en") ) {
          o->ref = getini_fromfile (lngfile, "en", name );
          if ( !o->ref ) o->ref = txts_def[o->id];
          else o->ref  = str_replace("\\n","\n",o->ref,1);
        } else o->ref = txt;
        o->name = name;
        o->txt  = str_replace("\\n","\n",txt,1);
        DEBUG_printf("  - Install text n°%d ( id '%s' ) as \"%s\"\n",o->id,o->name,o->txt);
        txtsdir->insert(txtsdir,o);
       i++;
      };
    };
  };
};

////////////////////////////////////////////////////////////////////////////////
l_text GETTXTTXT ( l_text text ) {

  if ( !text ) return NULL;

  if ( !stricmp(lng,"en") ) return text;

  if ( txtsdir && txtsdir->last ) {

    p_item x = txtsdir->first(txtsdir);
    p_item f = x;

    if  ( f )
    do {
       p_txt_info o = TXTINF(x->rec);

       if ( !stricmp(o->ref, text) ) return _strdup(o->txt);


       x = x->next;

    } while ( x != f );
  };

  return text;
};
////////////////////////////////////////////////////////////////////////////////
l_text GETTXT ( l_text name ) {

  if ( !name ) return NULL;

  if ( txtsdir && txtsdir->last ) {

    p_item x = txtsdir->first(txtsdir);
    p_item f = x;

    if  ( f )
    do {
       p_txt_info o = TXTINF(x->rec);

       if ( !stricmp(o->name, name) ) return o->txt;


       x = x->next;

    } while ( x != f );
  };

  if ( !stricmp(lng,"en") ) {
    l_int id = txt_id(name);
    if ( id != 0 ) return txts_def[id];
  };

  return NULL;
};
////////////////////////////////////////////////////////////////////////////////
l_text GETTXTID ( l_int id ) {
  if ( id < 1 || id > MAX_TXTS ) return NULL;

  if ( txtsdir && txtsdir->last ) {

    p_item x = txtsdir->first(txtsdir);
    p_item f = x;

    if  ( f )
    do {
       p_txt_info o = TXTINF(x->rec);

       if ( o->id == id ) return o->txt;


       x = x->next;

    } while ( x != f );
  };

  return txts_def[id-1];
};


