/****************************************************************/
/*                                                              */
/*                          brffunc.c                           */
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


#include"alltogrx.h"

/* from allegttf library */

FONT* load_ttf_font_ex (const char* filename,
                         const int points_w,
                         const int points_h,
                         const int smooth);

static void *br_load_font ( char* _filename, char *xxname, int w, int h, int from, int to )
{
  if ( _filename && !access(_filename, F_OK) ) {

      GrFONT* f = grx_loadfont(_filename, w, h);

      if ( !f ) f = (void*)load_ttf_font_ex(_filename, (double)w, (double)h, font_smooth);

      return (void*)f;

  };

  return NULL;
};

static void br_unload_font ( void *f )
{
  if ( f ) destroy_font((FONT*)f);
};


static void br_change_font_size ( void *f, int w, int h )
{


};


static int br_get_text_length ( void *f, unsigned char *str, int len ) {

  if ( f && str ) {

      FONT_PROP *fp;
      int c;
      int ln;

      if (((FONT*)f)->height > 0) return strlen(str) * 8;

      fp = ((FONT*)f)->dat.dat_prop;
      ln = 0;

      while ( *str && len ) {

         c = (int)*str - ' ';
         if ((c < 0) || (c >= FONT_SIZE)) c = 0;

         if (fp->dat[c]) ln += fp->dat[c]->w;

         len--;
         str++;
      };

      return ln;

  };

  return 0;

};


static int br_get_text_height ( void *f ) {

  return f?text_height((FONT*)f):0;

};


static void br_draw_text ( BITMAP *b, void *f, unsigned char *text, int x1, int y1, GrCOLOR color )
{
  if ( !f ) return;

  textout(b, (FONT*)f, text, x1, y1, color);
};



static void brfont_install_functions ( void )
{
  textlen = &br_get_text_length;
  change_font_size=&br_change_font_size;
  textheight = &br_get_text_height;
  drawtext = &br_draw_text;
  load_font = &br_load_font;
  unload_font = &br_unload_font;
};

