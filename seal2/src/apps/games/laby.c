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

/* Revision History (as of 04/04/2002):
 *
 * 04/04/2002: Updated with new icon (orudge)
 */

#include <random.h>
#include "seal.h"
#include "app.h"
#include "button.h"
#include "grfx.h"
#include "dialogs.h"
#include "menus.h"


#define MSG_GAMEHELP    100002

p_grfx laby = NULL;

l_char laby_dat[20][20] = {
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
{ 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0 },
{ 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0 },
{ 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0 },
{ 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
{ 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
{ 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0 },
{ 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0 },
{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0 },
{ 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 },
{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0 },
{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0 },
{ 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0 },
{ 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0 },
{ 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 2, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

l_char  where_x = 1;
l_char  where_y = 1;

void draw_game ( p_view o ) {
  t_rect  r = o->get_local_extent(o);
  t_point pe;
  BITMAP *out = o->begin_paint(o, &pe, r);
  if ( out ) {
    l_char x = 0;
    l_char y = 0;
    rectfill(out, pe.x ,
                      pe.y ,
                      pe.x + r.b.x,
                      pe.y + r.b.y,
                      makecol(128,128,0));
    while ( x < 6 ) {
      while ( y < 6 ) {
        l_char vx = x + where_x -3;
        l_char vy = y + where_y -3;
        l_char go = 0;

        if ( vx > 0 && vx < 20 && vy > 0 && vy < 20 ) go = laby_dat[vx][vy];

        if ( go == 2 ) {

          rectfill(out, pe.x+(x * r.b.x/5)-(r.b.x/5)+5,
                    pe.y+(y * r.b.y/5)-(r.b.y/5)+5,
                    pe.x+(x * r.b.x/5),
                    pe.y+(y * r.b.y/5),
                    makecol(192,192,0));

        } else if ( !go ) {
          l_uchar c = 0;

          while ( c < 5 ) {
            rectfill(out, pe.x + (x * r.b.x/5)- r.b.x/5 + 4 - c,
                 pe.y + (y * r.b.y/5)- r.b.y/5 + 4 - c,
                 pe.x + (x * r.b.x/5) + 4 - c,
                 pe.y + (y * r.b.y/5) + 4 - c,
                 makecol(0,128+(c*128/5),0));
            c++;
          };
        };
        y++;
      };
      y = 0;
      x++;
    };

    rectfill(out, pe.x+(2 * r.b.x/5)+15,
                 pe.y+(2 * r.b.y/5)+15,
                 pe.x+(2 * r.b.x/5)+(r.b.x/5)-10,
                 pe.y+(2 * r.b.y/5)+(r.b.y/5)-10,
                 COLOR(CO_LIGHTRED));


  };
  o->end_of_paint(o, r);

};

void   draw ( void) {VIEW(laby)->draw_me(VIEW(laby)); };

static void trans_ev ( p_object o, p_event event ) {

  if ( o->is_state(o, OB_SF_FOCUSED) )

  if ( event->type & EV_KEYBOARD ) {

    if ( OBJECT(keyb)->state & KB_SF_KEYDOWN )

    switch ( KEY_TO(keyb->code) ) {

      case KB_RIGHT : {  /* arrow RIGHT was pressed */

      if (laby_dat[where_x + 1][where_y] != 0)
      where_x = where_x + 1;
      draw();

        clear_event(event);

      }; break;

      case KB_LEFT : {  /* arrow LEFT was pressed */
      if (laby_dat[where_x - 1][where_y] != 0)
      where_x = where_x - 1;
      draw();


        clear_event(event);

      }; break;

      case KB_DOWN : {  /* arrow DOWN was pressed */
      if (laby_dat[where_x][where_y + 1] != 0)
      where_y = where_y + 1;
      draw();


        clear_event(event);

      }; break;

      case KB_UP : {  /* arrow UP was pressed */
      if (laby_dat[where_x][where_y - 1] != 0)
      where_y = where_y - 1;
      draw();

        clear_event(event);
      }; break;

    };

  };
  if ( event->type == EV_MESSAGE )
    switch ( event->message ) {
      case MSG_ABOUT: {
//        msgbox(MW_INFO, MB_OK, "Laby\n  A small labyrinthe: Find the exit (lighter space)\n\n(c) Copyright 2001 Point Mad. All Rights Reserved.");
        seal2_app_about("Laby", load_image("/system/bmp/laby.ico,32"));
        clear_event(event);
      }; break;
      case MSG_GAMEHELP: {
        open_help("laby.hlp");
        clear_event(event);
      }; break;
  };
};

static p_menu create_game_menu(void) {
   p_menu menu;
   menu = new_menu(
                 new_sub_menu("Game", new_menu(
                    new_menu_item(TXT_EXIT, "ALT+F4", 0, MSG_CLOSE, NULL,
                    NULL)),
                 new_sub_menu(TXT_HELP, new_menu(
                    new_menu_item(TXT_HELP, "", 0, MSG_GAMEHELP, NULL,
                    new_menu_line(
                    new_menu_item(TXT_ABOUT, "", 0, MSG_ABOUT, NULL,
                    NULL)))),
                 NULL))
              );
   return menu;
}

void init_window ( void )
{

   t_rect   r = rect_assign(0, 0, 220, 200);

   p_appwin p = appwin_init(_malloc(sizeof(t_appwin)),
                            r,
                            "Laby",
                            WF_MINIMIZE,
                            ap_id,
                            &trans_ev);


   if ( p ) VIEW(p)->align |= TX_ALIGN_CENTER;

   p->icon16 = load_image("/system/bmp/laby.ico,16");

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(p));

      if ( p ) {

	p_menuview Menu;
    r = VIEW(p)->size_limits(VIEW(p));
    r = rect_assign(r.a.x, r.a.y + 1, r.a.x, r.a.y + 20);
    Menu = hormenu_init(_malloc(sizeof(t_menuview)), r, create_game_menu());

    OBJECT(p)->insert(OBJECT(p), OBJECT(Menu));

      r = rect_assign(35, 45, 185, 195);

      laby = grfx_init(_malloc(sizeof(t_grfx)),r );

      VIEW(laby)->draw = &draw_game;

      OBJECT(p)->insert(OBJECT(p), OBJECT(laby));

      };
};

SetInfoAppName("Laby");
SetInfoDesciption("Will you find the exit?");
SetInfoCopyright("(c) Copyright 2000,2001 Point Mad. All Rights Reserved.");
SetInfoManufacturer("Point Mad");

app_begin ( void ) {


  if ( ap_process == AP_INIT ) { /* When ap start */

      AP_SETNUMOFCALLS(1); /* Set MAX of Calls */

      init_window(); /* Run the init void */

  };

  if ( ap_process == AP_FREE ) {   /*   */

  };
  if ( ap_process == AP_DONE ) {  /* When ap done */
  };
} app_end;

