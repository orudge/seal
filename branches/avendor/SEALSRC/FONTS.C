/****************************************************************/
/*                                                              */
/*                           fonts.c                            */
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

#include"fonts.h"
#include<unistd.h>

t_fonts_loaded *system_fonts = NULL;


/* from allegttf library */

FONT* load_ttf_font_ex (const char* filename,
                         const int points_w,
                         const int points_h,
                         const int smooth);

void *load_ttf_to_mem ( char* _filename, char *xxname, int w, int h, int from, int to )
{

  if ( _filename && !access(_filename, F_OK) ) {

      GrFONT* f = grx_loadfont(_filename, w, h);

      if ( !f ) f = (void*)load_ttf_font_ex(_filename, (double)w, (double)h, font_smooth);

      return (void*)f;

  };

  return NULL;
};


int    add_font_to_system ( void *font, char *name, int weight )
{
  t_fonts_loaded *s = system_fonts;
  t_fonts_loaded *n;
  t_fonts_loaded *last = NULL;
  int             fh = 0;

  if ( !font || !name ) return 0;

  while ( s ) {

    last = s;

    if ( fh == s->weight && !stricmp(s->name, name) ) return 2;

    s = s->next;

  };

  n = (t_fonts_loaded *)_malloc(sizeof(t_fonts_loaded));

  if ( !n ) return 0;

  memset(n, NULL, sizeof(t_fonts_loaded));

  if ( last ) last->next = n;
  else system_fonts = n;

  n->font = font;
  n->weight = weight;
  n->name = _strdup(name);

  return 1;

};


void    load_supported_fonts ( char *ini_array_name, l_bool in_mem )
{
  ini_data *d = getinidata_fromfile(INI_MAINFILE, ini_array_name);

  DEBUG_printf("\n...initializing supported fonts from '%s' file, from structure '%s'\n", INI_MAINFILE, ini_array_name); /* error message, if some occures */

  if ( d ) {

    int   lines = getini_linenum ( d );
    int   l = 0;

    while ( lines > 0 ) {

      char *def = NULL;
      char *func = NULL;
      char *value = getini_line ( &def, d, lines );
      char **f = getini_function ( &func, def );
      char **h = getini_values ( value );
      int    fr = -1;
      int    to = -1;

      /* load font, value is name of font to be loaded, def is file of font */

      if ( f && f[0] && *(f[0]) == INI_DECANUM )
        fr = *((long*)(&(f[0][1])));

      if ( f && f[1] && *(f[1]) == INI_DECANUM )
        to = *((long*)(&(f[1][1])));

      if ( h && h[0] && *(h[0]) == INI_STRING ) {

        char **t = h;

        while ( (*t) ) {

          if ( t[0] && *(t[0]) == INI_DECANUM ) {

            void *f = NULL;

            if ( in_mem ) /* load to memory, so use ALLEGTTF */

               f = load_ttf_to_mem(&(h[0][1]), NULL, *((long*)(&(t[0][1]))), *((long*)(&(t[0][1]))), fr, to);

            else

               f = load_font(&(h[0][1]), NULL, *((long*)(&(t[0][1]))), *((long*)(&(t[0][1]))), fr, to);

            DEBUG_printf(" - try to support font '%s' in width %i & height %i \n", &(h[0][1]), *((long*)(&(t[0][1]))), *((long*)(&(t[0][1])))); /* error message, if some occures */

            if ( !add_font_to_system(f, func, *((long*)(&(t[0][1])))) ) /* error */

                 DEBUG_printf("   - ERROR :: font wasn't installed\n"); /* error message, if some occures */

            else /* all is ok */

                 DEBUG_printf("   - font was succesfull installed\n"); /* error message, if some occures */

          };

          t++;
        };

      };

      freeini_values(f);
      freeini_values(h);

      _free(func);
      _free(value);
      _free(def);

      lines--;
    };

  } else ;/* error in reading */

  _free(d);

};


void    unload_system_fonts ( void )
{
};


l_font *get_font_in_size ( char *fontname, int w, int h )
{

  t_fonts_loaded *s = system_fonts;

  while ( s ) {

    if ( s->weight == w && fontname && s->name &&

         !stricmp(fontname, s->name) ) {

      return s->font;

    };

    s = s->next;
  };

  return NULL;

};


