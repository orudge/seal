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
#include "colors.h"

l_color basic_colors[max_basic_cols];

l_byte basic_pal[] = {
       0,   0,   0, /* -+                                             */
       0,   0, 128, /*  |                                             */
       0, 128,   0, /*  |                                             */
       0, 128, 128, /*  |                                             */
     128,   0,   0, /*  |                                             */
     128,   0, 128, /*  |                                             */
     128,  64,   0, /*  |                                             */
     192, 192, 192, /*  |=> Basic 16 Colors                           */
     128, 128, 128, /*  |                                             */
       0,   0, 255, /*  |                                             */
       0, 255,   0, /*  |                                             */
       0, 255, 255, /*  |                                             */
     255,   0,   0, /*  |                                             */
     255,   0, 255, /*  |                                             */
     255, 255,   0, /*  |                                             */
     255, 255, 255, /* -+                                             */

      90,  90,  100,
     105, 105, 115,
     120, 120, 130,
     135, 135, 145,
     150, 150, 150,
     156, 160, 178,
     170, 170, 188,
     195, 195, 195,
     210, 210, 220,
     225, 225, 235,
     240, 240, 250,
     255, 255, 255,
};



l_color ctrls_colors[max_ctrls_col];

l_text  ctrls_reg[max_ctrls_col] = {
  "flat_face",  // 0
  "flat_border",
  "flat_text",
  "3d_face",
  "3d_light", // 4
  "3d_shadow",
  "3d_dark",
  "3d_border",
  "3d_text",
  "selected_face", // 9
  "selected_text",
  "desktop_face",
  "desktop_text",
  "menu_face",
  "menu_face_gardient", // 14
  "menu_text",
  "menu_selected_face",
  "menu_selected_face_gardient",
  "menu_selected_text",
  "deskbar_face", // 19
  "deskbar_light",
  "deskbar_shadow",
  "deskbar_dark",
  "deskbar_border",
  "deskbar_text",  // 24
  "deskbar_selected_face",
  "deskbar_selected_text", // 26
  "window_act_title_text",
  "window_act_title_face",
  "window_act_title_face_gardient",
  "window_pass_title_text",
  "window_pass_title_face",
  "window_pass_title_face_gardient",
  NULL
};

l_byte ctrls_pal[max_ctrls_col*3] = {
    255, 255, 255, // 0
    0,   0,   0,
    0,   0,   0,
    192, 192, 192,
    225, 255, 255, //4
    128, 128, 128,
    64,  64,  64,
    0,   0,   0,
    0,   0,   0,
    64,  64,  255,  // 9
    255, 255, 255,
    0,   192, 192,
    0,   0,   0,
    192, 192, 192,
    220, 220, 220, // 14
    0,   0,   0,
    64,  64,  255,
    128, 128, 255,
    255, 255, 255,
    192, 192, 192, //19
    225, 255, 255,
    128, 128, 128,
    0,   0,   0,
    0,   0,   0,
    0,   0,   0,  // 24
    224, 244, 244,
    0,   0,   0,
    255, 255, 255,
    255, 120, 84,
    255, 255, 64, // 29
    0,   0,   0,
    192, 192, 192,
    128, 128, 128,
    0,0,0
};
////////////////////////////////////////////////////////////////////////////////
l_color  registry_color(char *name)
{
  l_text final_key;
  l_text value;
  l_text rgb[3];
  l_int r, g, b, i, c, ii;

  if ( !name )    return 0;
  if ( !name[0] ) return 0;

  final_key = key_in_path("current/seal/colors", name);

  value = get_key(final_key);
  free(final_key);
  if (value) {
    rgb[0] = (char *) malloc(10);
    rgb[1] = (char *) malloc(10);
    rgb[2] = (char *) malloc(10);
    c = 0;
    ii = 0;
    for (i=0;i<3;i++) strcpy(rgb[c], "");
    for (i=0;i<strlen(value);i++) {
      if (value[i] == ',' && c < 2) {
        c++;
        ii = 0;
      } else if (ii < 9) {
        if (value[i] != ' ') {
          rgb[c][ii] = value[i];
          ii++;
          rgb[c][ii] = 0;
        };
      };
    };
    r = atoi(rgb[0]);
    g = atoi(rgb[1]);
    b = atoi(rgb[2]);
    return makecol(r, g, b);
  } else {
    return COLOR(0);
  };
};
////////////////////////////////////////////////////////////////////////////////
void set_registry_color ( l_color col, l_text name ) {
l_text kkey = key_in_path("current/seal/colors",name);

if ( !key_exists( kkey ) )                create_key("current/seal/colors",name);


set_key( kkey, set_format_text(NULL,"%d,%d,%d",getr(col),getg(col),getb(col)) );

_free(kkey);

};
////////////////////////////////////////////////////////////////////////////////
void colors_init_st ( void ) {
  l_int a;

  a = 0;
  while ( a < max_basic_cols ) {
    basic_colors[a] = makecol(basic_pal[(a*3)], basic_pal[(a*3)+1],basic_pal[(a*3)+2]);
    a++;
  };

};

void colors_init ( void ) {
  l_int a;

  colors_init_st();

  a = 0;
  while ( ctrls_reg[a] ) {
    l_text key = key_in_path("current/seal/colors",ctrls_reg[a]);
    if ( key_exists( key ) )
      ctrls_colors[a] = registry_color(ctrls_reg[a]);
    else
      ctrls_colors[a] = makecol(ctrls_pal[(a*3)], ctrls_pal[(a*3)+1],ctrls_pal[(a*3)+2]);
    _free(key);
    a++;
  };

};
////////////////////////////////////////////////////////////////////////////////
void colors_done ( void ) {

};
