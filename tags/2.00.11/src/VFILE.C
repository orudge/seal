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

/* Revision History (as of 02/04/2002):
 *
 * 02/04/2002: Changed ./documents to ./docs for 8.3 compatibility (orudge)
 */

#include <sys/stat.h>
#include <seal.h>
#include "vfile.h"

l_text fileroot = "DESKTOP/";
l_text fileroot_ = "DESKTOP";

////////////////////////////////////////////////////////////////////////////////
p_list VirtualDrives = NULL; // List of VD drivers

p_vfile FatVFile = NULL; // Default VD driver

#define FatVFile_list &RealGetFile,&RealGetFileAttr,&RealFindFirst,\
                      &RealFindNext,&RealFileOpen,&RealFileClose,\
                      &RealFileDelete,&RealDirMake,&RealFileRename

#define VDRIVE(o) ((p_vdrive)(o))

////////////////////////////////////////////////////////////////////////////////
// This function resolve ".." and "." in a filename
//
// ex : ResolveFileName("/mydir/../another/./") return "/another/"
l_text ResolveFileName ( l_text SealFile ) {
if ( SealFile ) {
  l_int a = 0;
  l_int st = 0;
  l_text o = _strdup(SealFile);

  while ( o[a] ) {

    if ( o[a] == '\\' ) o[a] = '/';

    if ( o[a] == '/' ) {
      if ( a - st == 2 && o[st] == '.' && o[st+1] == '.' ) {
        l_int b = st-2;
        l_bool done = 0;
        while ( b >= 0 ) {
          if ( o[b] == '/' ) {
            delstr( o, b, a-b );
            b = 0;
            done = 1;
          };
          b--;
        };
        if ( !done && o[0] == '/' ) delstr( o, st, 3 );
      } else if ( a - st == 1 && o[st] == '.')  {
        delstr( o, st, 2 );
      };

      st = a + 1;
    };
    a++;
  };
  a = strlen(o);
  if ( o[a-3] == '/' && o[a-2] == '.' && o [a-1] == '.' ) {
    l_int b = a-4;
    l_bool done = 0;
    while ( b >= 0 ) {
      if ( o[b] == '/' ) {
        delstr( o, b+1, a-b-1 );
        b = 0;
        done = 1;
      };
      b--;
    };
    if ( !done && o[0] == '/' ) delstr( o, a-3, 2 );

  };

  if ( o[a-2] == '/' && o[a-1] == '.' ) {
    delstr( o, a-1, 1 );
  };

  return o;
} else return NULL;
};
////////////////////////////////////////////////////////////////////////////////
// Return File Attr, with dos type filename (x:\xxx\xxx)
#define ATTR_NOT_EXISTS 0
#define ATTR_FILE       1
#define ATTR_DIRECTORY  2
l_int _GetFileAttr ( l_text DosFile ) {
  l_int r = ATTR_NOT_EXISTS;
  if ( !access(DosFile, F_OK) ) r = ATTR_FILE;
  if ( !access(DosFile, D_OK) ) r = ATTR_DIRECTORY;
  return r;
};
////////////////////////////////////////////////////////////////////////////////
// Return the VD for a file, and put in pth filename on drive
p_vfile GetVFile ( l_text SealFile, l_text *pth ) {
  l_text o = ResolveFileName ( SealFile );
  *pth = NULL;
  if ( !o ) return NULL;
  /*if ( o[1] == ':' && o[2] == '/' ) {  // Dos type filename (x:/xxx) so return default driver
    (*pth) = _strdup(o);
    return FatVFile;
  } else */if ( o[0] == '/' && !o[1] ) { // Root, so return default driver with root file
    (*pth) =  _strdup(fileroot);
    _free(o);
    return FatVFile;
  } else if ( o[0] == '/' ) { //

    l_int a = 1;
    l_text drive = NULL;
    l_text path = NULL;
    l_bool ok = false;

    while ( !ok && o[a] ) { // Search position of drive name
      if ( o[a] == '/' ) {
        ok = true;
        drive = stridup ( o + 1, a - 1 ); //mid( o, 1, a - 1 );
        path = _strdup( o + a + 1 );
      };
      a++;
    };

    if ( !ok ) drive =_strdup( o + 1 ); // If drive name found then isolate it

    if ( VirtualDrives && VirtualDrives->last ) {  // If the list is loaded and if there is at least one item

      p_item x = VirtualDrives->first(VirtualDrives); // Prepare listing
      p_item f = x;

      if  ( f )
      do {  // List all drivers
         p_vdrive oo = VDRIVE(x->rec);
         if ( !strcmp(oo->drive,drive) ) { // If it's the good driver
           (*pth) = path;
           _free(o);
           _free(drive);
           return oo->drv; // Return the founded driver
         };
         x = x->next; // Next driver
      } while ( x != f );
    };
    // drive is not in list, so it's a directorie in file root, so return default driver
    (*pth) = set_format_text(NULL,"%s%s",fileroot_,o);
    _free(drive);
    //_free(path);
    _free(o);
    return FatVFile;
  };
  // Not a typic filename, return default driver
  (*pth) = o;
  return FatVFile;

};
////////////////////////////////////////////////////////////////////////////////
// Return the dos filename or temporary dos file for the SealFile
l_text GetFile ( l_text SealFile ) {
  if ( SealFile ) {
    l_text  pth = NULL;
    p_vfile o = GetVFile(SealFile,&pth);
    if ( o ) {
      l_text r = o->GetFile(o,pth);
      if ( pth )_free(pth);
      return r;
    };
  };
  return NULL;
};
////////////////////////////////////////////////////////////////////////////////
// Return the attr of SealFile (/xxxx/xxxx)
l_int GetFileAttr ( l_text SealFile ) {
  if ( SealFile ) {
    l_text  pth = NULL;
    p_vfile o = GetVFile(SealFile,&pth);
    if ( o ) {
      l_int r = o->GetFileAttr(o,pth);
      if ( pth )_free(pth);
      return r;
    };
  };
  return NULL;
};

////////////////////////////////////////////////////////////////////////////////
// Add a VD
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
                 l_int  ( *FileRename ) ( p_vfile o, l_text src, l_text dst ) ) {

  p_vdrive o = _malloc(sizeof(t_vdrive)); // Allocate memory
  clear_type(o,sizeof(t_vdrive)); // Clear allocated memory
  o->drive = drive;
   o->drv = _malloc(sizeof(t_vfile)); // Allocate memory
  clear_type(o->drv,sizeof(t_vfile)); // Clear allocated memory
  o->drv->data = data;
  o->drv->GetFile     = GetFile;
  o->drv->GetFileAttr = GetFileAttr;
  o->drv->FindFirst   = FindFirst;
  o->drv->FindNext    = FindNext;
  o->drv->FileOpen    = FileOpen;
  o->drv->FileClose   = FileClose;
  o->drv->FileDelete  = FileDelete;
  o->drv->DirMake     = DirMake;
  o->drv->FileRename  = FileRename;
  if (!VirtualDrives) VirtualDrives = list_init(_malloc(sizeof(t_list)),NULL,NULL); // If list is not loaded, load it

  VirtualDrives->insert(VirtualDrives,o); // Add drive in list
};
////////////////////////////////////////////////////////////////////////////////
// Default driver ''GetFile''
l_text RealGetFile( p_vfile o, l_text path ) {

  if ( o && path )
    return set_format_text(NULL,"%s%s",o->data,path);
  if ( o )
    return _strdup(o->data);

  return NULL;
};
////////////////////////////////////////////////////////////////////////////////
// Default driver ''GetFileAttr''
l_int RealGetFileAttr( p_vfile o, l_text path ) {

  if ( o && path ) {
    l_text file = o->GetFile(o,path);
    l_int r = _GetFileAttr( file );
    _free(file);
    return r;
  };
  if ( o )
    return ATTR_DIRECTORY;

  return NULL;
};
////////////////////////////////////////////////////////////////////////////////
//
static int io_testlinks ( struct t_ffblk *ffblk ) {
    //l_text ext = get_extension(ffblk->info.ff_name);
    clear_type(&ffblk->ff_filename,260);
    strcpy(ffblk->ff_filename, ffblk->info.ff_name);


      return 1;
};
////////////////////////////////////////////////////////////////////////////////
#define IAMOK(x) ( strcmp(x ,"..") && strcmp(x,".") )

////////////////////////////////////////////////////////////////////////////////
// Default driver FindFirst
int RealFindFirst( p_vfile drv, l_text path, struct t_ffblk *ffblk, int attrib) {
  l_int done = 0;
  if ( io_path( path ) && !stricmp(io_path( path ),fileroot) && ( attrib & FA_DIREC ) ) {
    l_text file = drv->GetFile ( drv,path );
    done = findfirst( _strdup( file ) , &ffblk->info, ffblk->ff_attrib);
    ffblk->fst = ffblk->nxt = NULL;
    ffblk->drv = drv;
    while ( !done && ( !io_testlinks(ffblk) || !IAMOK(ffblk->info.ff_name) ) )
      done = findnext(&ffblk->info);
    return done;
  } else {
    l_text file = drv->GetFile ( drv,path );
    done = findfirst( _strdup( file ) , &ffblk->info, attrib);
    ffblk->ff_attrib = attrib;
    ffblk->fst = ffblk->nxt = NULL;
    ffblk->drv = drv;
    while ( !done && !io_testlinks(ffblk) )
      done = findnext(&ffblk->info);
    return done;
  };
};
////////////////////////////////////////////////////////////////////////////////
// Default driver FindNext
int RealFindNext(struct t_ffblk *ffblk)
{
   l_int done = findnext(&ffblk->info);
    while ( !done && !io_testlinks(ffblk) )
     done = findnext(&ffblk->info);
   return done;
};
////////////////////////////////////////////////////////////////////////////////
// Default driver FileOpen
FILE* RealFileOpen ( p_vfile o, l_text SealFile, l_text mode ) {
  if ( o && SealFile && mode ) {
    l_text file = o->GetFile(o,SealFile);
    FILE *f = NULL;
    if ( file ) {
      f = _fopen(file,mode);
      _free(file);
    };
    return f;
  };
  return NULL;
};
////////////////////////////////////////////////////////////////////////////////
// Default driver FileClose
void RealFileClose ( p_vfile o, l_text path, FILE *f )
{
  _fclose(f);
};
////////////////////////////////////////////////////////////////////////////////
//
l_int RealFileDelete ( p_vfile o, l_text path ) {
   if ( !o || !path ) return 0;
   if ( remove(o->GetFile(o,path)) )
       return !rmdir(o->GetFile(o,path));
   return true;
};
////////////////////////////////////////////////////////////////////////////////
//
l_int RealDirMake  ( p_vfile o, l_text path ) {
  if ( !o || !path ) return 0;
  return (!mkdir(o->GetFile(o,path), S_IWUSR));
};
////////////////////////////////////////////////////////////////////////////////
//
l_int RealFileRename  ( p_vfile o, l_text src, l_text dst ) {
  if ( o && src && dst )
    if ( stricmp(src, dst) ) {
      l_int t = rename(o->GetFile(o,src), o->GetFile(o,dst));
      return abs(t);
    } else return -1;
  return -2;
};
////////////////////////////////////////////////////////////////////////////////
// Substitue a drive /xxxx/ to a dos path (x:\xxxx\xxx)
void Subst ( l_text drive, l_text path ) {
  if ( drive && path )
    AddVDrive( drive, path, FatVFile_list  );
};
////////////////////////////////////////////////////////////////////////////////
// Open a file (SealFile) in the submitted mode
FILE* SealFileOpen ( l_text SealFile, l_text mode ) {
  if ( SealFile ) {
    l_text  pth = NULL;
    p_vfile o = GetVFile(SealFile,&pth);
    if ( o ) {
      FILE *f = o->FileOpen(o,pth,mode);
      if ( pth ) _free(pth);
      return f;
    };
  };
  return NULL;
};
////////////////////////////////////////////////////////////////////////////////
// Close a file
void SealFileClose ( FILE *f ) {
  return _fclose ( f );
};
////////////////////////////////////////////////////////////////////////////////
// Internal function of io_removefile
l_int   _io_removefile ( l_text file ) {
  if ( file ) {
    l_text  pth  = NULL;
    p_vfile o = GetVFile(file,&pth);
    if ( pth && o ) return o->FileDelete(o,pth);
  };
  return 0;
};
////////////////////////////////////////////////////////////////////////////////
//
l_bool  io_mkdir ( l_text dir )
{
  if ( dir ) {
    l_text  pth  = NULL;
    p_vfile o = GetVFile(dir,&pth);
    if ( o && pth ) return o->DirMake(o,pth);
  };
  return 0;
};
////////////////////////////////////////////////////////////////////////////////
//
l_int io_rename ( l_text nameold, l_text namenew ) {
  if ( nameold && namenew ) {
    l_text  pth1;
    l_text  pth2;
    p_vfile o1 = GetVFile(nameold,&pth1);
    p_vfile o2 = GetVFile(namenew,&pth2);
    if ( o1 != o2 ) return -2; // (will copy and delete in the futur ...)
    return o1->FileRename(o1,pth1,pth2);
  };
  return -2;
};
// Seal 1.xx Functions Support /////////////////////////////////////////////////
// Return true if file is a directorie, return false if it's a file or not-exists
l_bool  io_isdir ( l_text file ) {
  return ( GetFileAttr(file) == ATTR_DIRECTORY);
};
////////////////////////////////////////////////////////////////////////////////
// Return true if file is a file, return false if it's a directorie or not-exists
l_bool  io_isfile ( l_text file ) {
  return ( GetFileAttr(file) == ATTR_FILE);
};
////////////////////////////////////////////////////////////////////////////////
// Retun true if file exists, return true if not-exists
l_bool  io_exist ( l_text file ) {
  return ( GetFileAttr(file) != ATTR_NOT_EXISTS);
};
////////////////////////////////////////////////////////////////////////////////
// Return the text contain by the file filename, return NULL if file not-exists
// or can't be read
l_text  file_gettext ( l_text filename ) {
  FILE *f = SealFileOpen(filename, "rt");
  if ( f ) {
      l_long size = filelength(fileno(f));
      l_text text = _malloc(size+1);
      if ( text ) {
         clear_type(text, size+1);
         fread(text, size, 1, f);
      };
      fclose(f);
      return text;
  };
  return NULL;
};
////////////////////////////////////////////////////////////////////////////////
// Write the text in the file filename, return true if written else return false
l_bool  file_puttext ( l_text filename, l_text text ) {
  FILE *f = SealFileOpen(filename, "wt");
  if ( f ) {
      if ( text ) {
         fwrite(text, strlen(text), 1, f);
      };
      fclose(f);
      return true;
  };
  return false;
};
////////////////////////////////////////////////////////////////////////////////
l_text  io_realpath ( l_text path, l_text file )
{
  if ( path ) {
      l_int s = strlen(path);
      if ( s && *(path+s-1) == '/' || *(path+s-1) == '\\' ) {
            if ( file ) return set_format_text(NULL, "%s%s", path, file);
            else return _strdup(path);
      } else if ( s )
         if ( file ) return set_format_text(NULL, "%s/%s", path, file);
         else return _strdup(path);
  } else {
     if ( file ) return _strdup(file);
  };
  return NULL;
};

////////////////////////////////////////////////////////////////////////////////
int io_findfirst(const char *pathname, struct t_ffblk *ffblk, int attrib)
{

  if ( !strcmp(io_path( (l_text) pathname ),"/") && ( attrib & FA_DIREC ) ) {

    ffblk->fst = VirtualDrives->first(VirtualDrives);
    ffblk->nxt = ffblk->fst;
    ffblk->pathname = _strdup(pathname);
    ffblk->ff_attrib = attrib;
    ffblk->info.ff_attrib = (FA_DIREC|FA_SYSTEM);
    strcpy(ffblk->info.ff_name,VDRIVE( ffblk->nxt->rec )->drive);
    io_testlinks(ffblk);

    return 0;

  } else {
    l_text  pth;
    p_vfile o = GetVFile((l_text)pathname,&pth);
    ffblk->fst = ffblk->nxt = NULL;
    ffblk->drv = o;

    return o->FindFirst(o,pth,ffblk,attrib);
  };
};
////////////////////////////////////////////////////////////////////////////////


int io_findnext(struct t_ffblk *ffblk) {
 if ( !ffblk->fst ) {
   return ffblk->drv->FindNext(ffblk);
 } else {
    ffblk->nxt = ffblk->nxt->next;
    if ( ffblk->fst == ffblk->nxt ) {
      l_text  pth;
      p_vfile o = GetVFile(ffblk->pathname,&pth);

      ffblk->fst = ffblk->nxt = NULL;
      ffblk->drv = o;
      return o->FindFirst(o,pth,ffblk,ffblk->ff_attrib);
    } else {
      ffblk->info.ff_attrib = (FA_DIREC|FA_SYSTEM);
      strcpy(ffblk->info.ff_name,VDRIVE( ffblk->nxt->rec )->drive);
      io_testlinks(ffblk);
      return 0;
    };
 };
};
////////////////////////////////////////////////////////////////////////////////
// Initianlise Virtual File System
void ini_vfile ( void ) {
  int a = 0;
  DEBUG_printf("\n - Start Virtual File System 1.00 ...\n   - Prepare Default V Drive\n");



  FatVFile = malloc(sizeof(t_vfile)); // Alloc memory
  clear_type(FatVFile,sizeof(t_vfile)); // Clear memory alloced
  FatVFile->data        = "";
  FatVFile->GetFile     = &RealGetFile;
  FatVFile->GetFileAttr = &RealGetFileAttr;
  FatVFile->FindFirst   = &RealFindFirst;
  FatVFile->FindNext    = &RealFindNext;
  FatVFile->FileOpen    = &RealFileOpen;
  FatVFile->FileClose   = &RealFileClose;
  FatVFile->FileDelete  = &RealFileDelete;
  FatVFile->DirMake     = &RealDirMake;
  FatVFile->FileRename  = &RealFileRename;

  DEBUG_printf("   - Create Dos drives ...\n");
  while ( a < 27) { // loop to test all dos drives
    if ( _is_remote_drive(a) != -1 ) { // If dos drive exists
      if ( a != 1 ) {
      DEBUG_printf("   - Drive %c:\\ installed as /%c/ \n",'a'+a,'a'+a);

      Subst(set_format_text(NULL,"%c",'a'+a),set_format_text(NULL,"%c:/",'a'+a));  // Add the dos drive in seal drives
      };
    };

    a++;
  };

  Subst("documents","./docs/");  // Load default drives
  Subst("system","./");               //

};
