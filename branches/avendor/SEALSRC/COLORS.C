/****************************************************************/
/*                                                              */
/*                          colors.c                            */
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


#include"colors.h"
#include"dataini.h"

l_color *colors_standard = NULL;


l_byte palette_standard[]
 = {
       28,  1,   1, /* This line only set how many triples are there
                       => (25*1*1)                                    */
       0,   0,   0, /* -+                                             */
       0,   0, 128, /*  |                                             */
       0, 130,  65, /*  |                                             */
       0, 128, 128, /*  |                                             */
     128,   0,   0, /*  |                                             */
     207,  75, 177, /*  |                                             */
     175,  97,  45, /*  |                                             */
     192, 192, 192, /*  |=> Basic 16 Colors                           */
     128, 128, 128, /*  |                                             */
       0,   0, 255, /*  |                                             */
       0, 255,   0, /*  |                                             */
       0, 255, 255, /*  |                                             */
     255,   0,   0, /*  |                                             */
     226, 150, 209, /*  |                                             */
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


/*
  get color name_of_color for the file "seal.ini" from section [colors]

  example:

  l_color x = color_get_from_ini("3D_background");
*/
l_color  color_get_from_ini ( char *name_of_color )
{

  ini_rgb scolor  = getini_color (INI_MAINFILE, INI_COLORS, name_of_color);



  return makecol(scolor.r, scolor.g, scolor.b);
};


/* internal seal function, default called from drivers_init function */

l_color *color_trans_palette ( l_byte *rgb )
{

  if ( rgb ) {

    l_long    size = rgb[0]*rgb[1]*rgb[2];
    l_color  *pal = _malloc(sizeof(l_color)*(size+1));
    l_long    i   = 1;
    l_int     dpt = get_depth(screen);

    if ( pal ) {

      pal[0] = size;

      while ( i < size ) {

        if ( dpt < 15 ) {
          RGB pr;
          pr.r = rgb[i*3];
          pr.g = rgb[i*3+1];
          pr.b = rgb[i*3+2];

          set_color(i, &pr);
          pal[i] = i;

        } else
          pal[i] = makecol(rgb[i*3], rgb[i*3+1], rgb[i*3+2]);

        i++;
      };
    };

    return pal;

  };

  return NULL;
};



void colors_init ( void )
{
  colors_standard = color_trans_palette(palette_standard);
};


void colors_done ( void )
{
  _free(colors_standard);
  colors_standard = NULL;
};


