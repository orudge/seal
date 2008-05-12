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

#ifndef _SEAL_COLORS_H_
#define _SEAL_COLORS_H_

#define  CO_SKIP_RED             255
#define  CO_SKIP_GREEN             0
#define  CO_SKIP_BLUE            255

#define  CO_ISCOLOR(x)           ((x) != CO_NOCOLOR)

#define max_basic_cols 28

extern l_color basic_colors[max_basic_cols];

#define COLOR(n) ((n)>=0&&(n)<max_basic_cols?basic_colors[(n)]:0)

#define  CO_NOCOLOR             -1
#define  CO_BLACK                0
#define  CO_BLUE                 1
#define  CO_GREEN                2
#define  CO_CYAN                 3
#define  CO_RED                  4
#define  CO_MAGENDA              5
#define  CO_BROWN                6
#define  CO_LIGHTGRAY            7
#define  CO_DARKGRAY             8
#define  CO_LIGHTBLUE            9
#define  CO_LIGHTGREEN           10
#define  CO_LIGHTCYAN            11
#define  CO_LIGHTRED             12
#define  CO_LIGHTMAGENDA         13
#define  CO_YELLOW               14
#define  CO_WHITE                15

#define  CO_SYSx0                0
#define  CO_SYSx1                1
#define  CO_SYSx2                2
#define  CO_SYSx3                3
#define  CO_SYSx4                4
#define  CO_SYSx5                5
#define  CO_SYSx6                6
#define  CO_SYSx7                7
#define  CO_SYSx8                8
#define  CO_SYSx9                9
#define  CO_SYSx10               10
#define  CO_SYSx11               11
#define  CO_SYSx12               12
#define  CO_SYSx13               13
#define  CO_SYSx14               14
#define  CO_SYSx15               15
#define  CO_SYSx16               16
#define  CO_SYSx17               17
#define  CO_SYSx18               18
#define  CO_SYSx19               19
#define  CO_SYSx20               20
#define  CO_SYSx21               21
#define  CO_SYSx22               22
#define  CO_SYSx23               23
#define  CO_SYSx24               24
#define  CO_SYSx25               25
#define  CO_SYSx26               26
#define  CO_SYSx27               27

#define max_ctrls_col 34

extern l_color ctrls_colors[max_ctrls_col];

#define color_flat_face                      ctrls_colors[0]
#define color_flat_border                    ctrls_colors[1]
#define color_flat_text                      ctrls_colors[2]
#define color_3d_face                        ctrls_colors[3]
#define color_3d_light                       ctrls_colors[4]
#define color_3d_shadow                      ctrls_colors[5]
#define color_3d_dark                        ctrls_colors[6]
#define color_3d_border                      ctrls_colors[7]
#define color_3d_text                        ctrls_colors[8]
#define color_selected_face                  ctrls_colors[9]
#define color_selected_text                  ctrls_colors[10]
#define color_desktop_face                   ctrls_colors[11]
#define color_desktop_text                   ctrls_colors[12]
#define color_menu_face                      ctrls_colors[13]
#define color_menu_face_gardient             ctrls_colors[14]
#define color_menu_text                      ctrls_colors[15]
#define color_menu_selected_face             ctrls_colors[16]
#define color_menu_selected_face_gardient    ctrls_colors[17]
#define color_menu_selected_text             ctrls_colors[18]
#define color_deskbar_face                   ctrls_colors[19]
#define color_deskbar_light                  ctrls_colors[20]
#define color_deskbar_shadow                 ctrls_colors[21]
#define color_deskbar_dark                   ctrls_colors[22]
#define color_deskbar_border                 ctrls_colors[23]
#define color_deskbar_text                   ctrls_colors[24]
#define color_deskbar_selected_face          ctrls_colors[25]
#define color_deskbar_selected_text          ctrls_colors[26]
#define color_window_act_title_text           ctrls_colors[27]
#define color_window_act_title_face           ctrls_colors[28]
#define color_window_act_title_face_gardient  ctrls_colors[29]
#define color_window_pass_title_text          ctrls_colors[30]
#define color_window_pass_title_face          ctrls_colors[31]
#define color_window_pass_title_face_gardient ctrls_colors[32]

void     set_registry_color ( l_color col, l_text name );
l_color  registry_color     ( char *name );

extern l_text  ctrls_reg[max_ctrls_col];

#endif
