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
#include "bmps.h"

////////////////////////////////////////////////////////////////////////////////
p_list bmpsdir = NULL;

typedef struct bmp_info {
  l_int   id;
  l_text  name;
  BITMAP *bmp;
} t_bmp_info, *p_bmp_info;

#define BMPINF(o) ((p_bmp_info)(o))
////////////////////////////////////////////////////////////////////////////////
l_text bmps_regname[] = { "IMG_FILE16",          "IMG_FILE32",
                          "IMG_DIR16",           "IMG_DIR32",
                          "IMG_WARNING",         "IMG_ERROR",
                          "IMG_INFO",            "IMG_QUESTION",
                          "IMG_UNAVAILABLE",     "IMG_SETUP",
                          "IMG_WCLOSE",          "IMG_WMIN",
                          "IMG_WMAX",            "IMG_APP16",
                          "IMG_APP32",           "IMG_PARDIRBT",
                          "IMG_ABOUT",           "IMG_DWCLOSE",
                          "IMG_DWMIN",           "IMG_DWMAX",
                          "IMG_DABOUT",
                          NULL };
////////////////////////////////////////////////////////////////////////////////
l_int bmp_id ( l_text regname ) {
  l_int a = 0;
  while ( bmps_regname[a] ) {
    if ( !stricmp(bmps_regname[a], regname ) ) return a;
    a++;
  };
  return -1;
};
////////////////////////////////////////////////////////////////////////////////

void load_bmps ( void ) {
  p_registry_search inf = (p_registry_search) malloc(sizeof(t_registry_search));

  bmpsdir = list_init(malloc(sizeof(t_list)),NULL,NULL);

  if ( reg_find_first("current/seal/images", inf) ) do {
    l_text file   = get_key(inf->name);
    l_text rfile  = GetFile(file);
    l_text name = _strdup(inf->key->name);

    p_bmp_info o  = _malloc(sizeof(t_bmp_info));

    clear_type(o,sizeof(t_bmp_info));

    o->id   = bmp_id(name);
    o->bmp  = load_image(rfile);
    o->name = name;

    conv_to_skipcolor_bitmap(o->bmp,255,0,255);

    bmpsdir->insert(bmpsdir,o);

    _free(file);
    _free(rfile);
  } while (reg_find_next(inf));

  _free(inf);

};

////////////////////////////////////////////////////////////////////////////////

BITMAP *GETBMP ( l_text name ) {

  if ( !name ) return NULL;

  if ( bmpsdir && bmpsdir->last ) {

    p_item x = bmpsdir->first(bmpsdir);
    p_item f = x;

    if  ( f )
    do {
       p_bmp_info o = BMPINF(x->rec);

       if ( !stricmp(o->name, name) ) return o->bmp;


       x = x->next;

    } while ( x != f );
  };

  return NULL;
};
////////////////////////////////////////////////////////////////////////////////
BITMAP *GETBMPID ( l_int id ) {

  if ( bmpsdir && bmpsdir->last ) {

    p_item x = bmpsdir->first(bmpsdir);
    p_item f = x;

    if  ( f )
    do {
       p_bmp_info o = BMPINF(x->rec);

       if ( o->id == id ) return o->bmp;


       x = x->next;

    } while ( x != f );
  };
  return NULL;
};
