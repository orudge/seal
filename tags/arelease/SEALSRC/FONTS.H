/****************************************************************/
/*                                                              */
/*                          fonts.h                             */
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



#ifndef __FONTS_H_INCLUDED__
#define __FONTS_H_INCLUDED__

#define INI_TTFFONTS      "true_type_fonts"
#define INI_TTFFONTSMEM   "true_type_fonts_memory"

typedef struct t_fonts_loaded {

  void                     *font;
  char                     *name;
  int                       weight;
  struct t_fonts_loaded    *next;

  char                      reserved[24];

} t_fonts_loaded;


extern t_fonts_loaded *system_fonts;


int     add_font_to_system ( void *font, char *name, int weight );
void    load_supported_fonts ( char *ini_array_name, l_bool in_mem );
void    unload_system_fonts ( void );
l_font *get_font_in_size ( char *fontname, int w, int h );

#endif

