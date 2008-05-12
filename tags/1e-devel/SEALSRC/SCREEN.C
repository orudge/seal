/****************************************************************/
/*                                                              */
/*                           screen.c                           */
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



#include"allegro.h"
#include"object.h"
#include"screen.h"

#define  INI_SCREEN     "screen"

BITMAP  *screen_virtual = NULL;
l_int    screen_width     = 0;
l_int    screen_height    = 0;

static l_int    screen_viswidth  = 0;
static l_int    screen_visheight = 0;
static l_int    screen_card      = 0;
static l_int    screen_depth     = 0;


l_bool  screen_reload ( void )
{

  GrSetMode(screen_card, screen_viswidth, screen_visheight, screen_width, screen_height, screen_depth);
  GrClearClip(screen);

  if ( screen_virtual ) {

     draw_sprite(screen, screen_virtual, 0, 0);

     set_clip(screen, -1, -1, -1, -1);
     set_clip(screen_virtual, -1, -1, -1, -1);
  };

  return true;
};


void    screen_halt ( void )
{

  if ( screen_virtual ) {

     set_clip(screen, 0, 0, screen_width, screen_height);
     set_clip(screen_virtual, 0, 0, screen_width, screen_height);

     show_mouse(NULL);

     blit(screen, screen_virtual, 0, 0, 0, 0, screen_width, screen_height);

     show_mouse(screen);

  };

  GrSetTextMode();
};


l_bool  screen_init ( void )
{

  long card  = getininum_fromfile (INI_MAINFILE, INI_SCREEN, "card");
  long w     = getininum_fromfile (INI_MAINFILE, INI_SCREEN, "width");
  long h     = getininum_fromfile (INI_MAINFILE, INI_SCREEN, "height");
  long v_w   = getininum_fromfile (INI_MAINFILE, INI_SCREEN, "virtual_width");
  long v_h   = getininum_fromfile (INI_MAINFILE, INI_SCREEN, "virtual_height");
  long depth = getininum_fromfile (INI_MAINFILE, INI_SCREEN, "depth");

  DEBUG_printf("\n...initializing screen from file '%s' in sctructure '%s' by these parameters :\n", INI_MAINFILE, INI_SCREEN); /* error message, if some occures */
  DEBUG_printf(" - depth: %i bits per pixel (%d colors)\n - width: %i\n - height: %i\n", depth, 1<<depth, w, h); /* error message, if some occures */

  /* safe mode is set */
  if ( safe_mode ) {

      DEBUG_printf("\nSEAL run is safe mode\n\n");

      w = 640;
      h = 480;
      v_w = 0;
      v_h = 0;
      depth = 15;

  };

  if ( !GrSetMode(card, w, h, v_w, v_h, depth) ) { /* not exist this resolution */

     DEBUG_printf("\n...try to initializing in new resolution"); /* error message, if some occures */
     DEBUG_printf(" - depth: 15 bits per pixel (%d colors)\n - width: 640\n - height: 480\n", 1<<15); /* error message, if some occures */

     w = v_w = 640;
     h = v_h = 480;
     depth = 15;

     if ( !GrSetMode(card, w, h, v_w, v_h, depth) ) { /* not found this driver */

         DEBUG_printf("\nERROR: Your card not support 640x480 resolution in %d colors", 1<<15); /* error message, if some occures */

         seal_error(ERR_NONREGULAR, "\nERROR: Your card not support 640x480 resolution in %d colors\n", 1<<15);

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

  screen_virtual = create_bitmap(screen_width, screen_height);

  DEBUG_printf("   ...initializing virtual screen\n"); /* error message, if some occures */

  if ( !screen_virtual ) {

      DEBUG_printf("     - ERROR :: virtual screen is not installed\n"); /* error message, if some occures */
      return false;

  };

  DEBUG_printf("   screen and virtual screen were succesfull installed\n"); /* error message, if some occures */

  return true;
};


void screen_done ( void )
{
  if ( screen_virtual )

     destroy_bitmap(screen_virtual);

  screen_virtual = NULL;
  screen_width    = 0;
  screen_height   = 0;

  GrSetTextMode();
};

