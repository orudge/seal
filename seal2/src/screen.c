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
#include <screen.h>

BITMAP  *screen_virtual = NULL;
BITMAP  *screen_double_buffer = NULL;

l_int    screen_width     = 0;
l_int    screen_height    = 0;
/*static*/ l_int    screen_viswidth  = 0;
/*static*/ l_int    screen_visheight = 0;
/*static*/ l_int    screen_card      = 0;
/*static*/ l_int    screen_depth     = 0;

/* jdh moved - needs to know about screen_depth -
NOTE you need to use load_bitmap directly if you ever intend saving the image (in some other format?)
because this (now, rightly or wrongly) converts it to whatever depth we have on screen
which can be destructive, particularly in 8bits, but still not good in 15 or 16 bits.
This added to ensure gifs particularly are converted to screen depth.
*/
/*static*/ PALETTE Gr_pal;

l_int save_image( char *filename, BITMAP *bmp, AL_CONST RGB *pal) {
  return save_bitmap(GetFile(filename),bmp,pal);
};

GrIMAGE *gr_load_bitmap ( char *filename ) {
  if( filename ) {
  l_text file = GetFile(filename);
    if ( file ) {
      GrIMAGE * bmp=load_bitmap(file, Gr_pal);
      if( bmp ) {
//        return(_fixup_loaded_bitmap((BITMAP *)bmp,Gr_pal,screen_depth));
        return bmp;
      };
      _free(file);
    };
  };
  return NULL;
};

l_bool  screen_reload ( void )
{
  GrSetMode(screen_card, screen_viswidth, screen_visheight, screen_width, screen_height, screen_depth);
  GrClearClip(screen);
  if ( screen_virtual ) {
     draw_sprite(screen, screen_virtual, 0, 0);
     set_clip(screen, -1, -1, -1, -1);
     set_clip(screen_virtual, -1, -1, -1, -1);
    if ( screen_double_buffer ) set_clip(screen_double_buffer, -1, -1, -1, -1);
  };
  return true;
};

void    screen_halt ( void )
{
  if ( screen_virtual ) {
     set_clip(screen, 0, 0, screen_width, screen_height);
     set_clip(screen_virtual, 0, 0, screen_width, screen_height);
     if ( screen_double_buffer ) set_clip(screen_double_buffer, 0, 0, screen_width, screen_height);
     show_mouse(NULL);
     blit(screen, screen_virtual, 0, 0, 0, 0, screen_width, screen_height);
     show_mouse(screen);
  };
  GrSetTextMode();
};


l_bool  screen_init ( void )
{
  long card  = 0;
  long w     = get_key_integer("system/graphics/width");
  long h     = get_key_integer("system/graphics/height");
  long dbf   = get_key_integer("system/graphics/double_buffer");
  long v_w   = 0;
  long v_h   = 0;
  long depth = get_key_integer("system/graphics/depth");
  DEBUG_printf("\n...initializing screen from registry with those parameters :\n" ); /* error message, if some occures */
  DEBUG_printf(" - depth: %i bits per pixel (%d colors)\n - width: %i\n - height: %i\n", depth, 1<<depth, w, h); /* error message, if some occures */
  if ( w == 0 || h == 0 || depth == 0) safe_mode=true; /* elementary precaution jdh */

  if ( safe_mode ) {
      DEBUG_printf("\nSEAL running in safe mode\n\n");
      w = 640;
      h = 480;
      v_w = 0;
      v_h = 0;
      depth = 16;
  };

  if ( !GrSetMode(card, w, h, v_w, v_h, depth) ) { /* not exist this resolution */
     printf("\nERROR: Selected resolution (%dx%d @ %d bpp) is not supported!\n",w,h,depth);
     DEBUG_printf("\nERROR : not supported ... try to initializing in new resolution"); /* error message, if some occures */
     DEBUG_printf(" - depth: 8 bits per pixel (%d colors)\n - width: 640\n - height: 480\n", 1<<8); /* error message, if some occures */
     w = v_w = 640;
     h = v_h = 480;
     depth = 16;
     if ( !GrSetMode(card, w, h, v_w, v_h, depth) ) { /* not found this driver */
         DEBUG_printf("\nERROR: Your card does not support the default resolution of 640x480 in %d colours\n", 1<<8); /* error message, if some occures */
         seal_error(ERR_NONREGULAR, "\nERROR: Your card does not support the default resolution of 640x480 in %d colours\n", 1<<8);
     };
  };
  GrClearClip(screen);
  DEBUG_printf("\nScreen is ok");
  screen_viswidth  =  SCREEN_W;
  screen_visheight =  SCREEN_H;
  screen_depth     =  get_depth(screen);
  screen_card      =  card;
  screen_width    = max(SCREEN_W, min(VIRTUAL_W, v_w));
  screen_height   = max(SCREEN_H, min(VIRTUAL_H, v_h));
  DEBUG_printf("\n..initializing the virtual screen");
/* stops protests when run without a proper screen using the debug allegro library */
  set_color_conversion(COLORCONV_TOTAL);
  screen_virtual = create_bitmap(screen_width, screen_height);
  DEBUG_printf("   ...initializing virtual screen\n"); /* error message, if some occures */
  if ( !screen_virtual ) {
      DEBUG_printf("     - ERROR :: virtual screen is not installed\n"); /* error message, if some occures */
      return false;
  };
  if ( dbf ) {
    DEBUG_printf("   ...initializing double buffer screen\n");
    screen_double_buffer = create_bitmap(screen_width, screen_height);
    if ( !screen_double_buffer ) DEBUG_printf("     - ERROR :: double buffer screen is not installed (not fatal error)\n");
  };

  DEBUG_printf("   screen and virtual screen were succesfully installed\n"); /* error message, if some occures */
  return true;
};


void screen_done ( void )
{
  if ( screen_virtual )
     destroy_bitmap(screen_virtual);
  screen_virtual = NULL;
  if ( screen_double_buffer )
     destroy_bitmap(screen_double_buffer);
  screen_double_buffer = NULL;
  screen_width    = 0;
  screen_height   = 0;
  GrSetTextMode();
};
