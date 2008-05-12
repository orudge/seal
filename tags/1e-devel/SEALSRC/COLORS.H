/****************************************************************/
/*                                                              */
/*                          colors.h                            */
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



#include"alltogrx.h"
#include"object.h"

#ifndef __COLORS_H_INCLUDED__
#define __COLORS_H_INCLUDED__

/* defines color section in "seal.ini" ...[colors] */
#define  INI_COLORS             "colors"

/*
   skip colors for RED, GREEN, BLUE. The Combination of these values will make skip
   color for images.
*/
#define  CO_SKIP_RED             255
#define  CO_SKIP_GREEN             0
#define  CO_SKIP_BLUE            255

#define  CO_ISCOLOR(x)           ((x) != CO_NOCOLOR)

/*
  main color' indexes. use by definiton COLOR(x) to get real color.
*/
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

/* gets real color from system color palette */
#define COLOR(n) ((n)<colors_standard[0]?colors_standard[n+1]:0)

/* contains system colors */
extern l_color *colors_standard;

/* contains system palette */
extern l_byte palette_standard[];

/*
  get color name_of_color for the file "seal.ini" from section [colors]

  example:

  l_color x = color_get_from_ini("3D_background");
*/
l_color  color_get_from_ini ( char *name_of_color );


/*
  ..internal seal functions
*/
l_color *color_trans_palette ( l_byte *rgb );

void colors_init ( void );
void colors_done ( void );

#endif

