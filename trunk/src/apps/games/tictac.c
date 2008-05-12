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

#include <seal.h>
#include <app.h>
#include <button.h>
#include <grfx.h>
#include <dialogs.h>
#include <menus.h>

#define MSG_GAMERESTART 100001
#define MSG_GAMEHELP    100002

p_grfx   tictactic = NULL;
l_uchar  tictactic_dat[3][3];
p_appwin p = NULL;

void draw ( void ) { VIEW(tictactic)->draw_me(VIEW(tictactic)); };

void draw_game ( p_view o ) {
  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {
    l_uchar x = 0, y = 0;

    rectfill( out, p.x , p.y , p.x + r.b.x, p.y + r.b.y, COLOR(CO_SYSx23));

    while (x < 3) {
      while (y < 3) {
        t_rect br = rect_assign(r.b.x/3*x,r.b.y/3*y,r.b.x/3*(x+1),r.b.y/3*(y+1));

        if (tictactic_dat[x][y] == 1)  rectfill(out, p.x + br.a.x + 10,
                                                   p.y + br.a.y + 10,
                                                   p.x + br.b.x - 10,
                                                   p.y + br.b.y - 10,
                                                   COLOR(CO_RED));

        if (tictactic_dat[x][y] == 2)  circlefill(out,p.x + br.a.x + r.b.x/6,p.y + br.a.y + r.b.y/6, r.b.y/6 -10 , COLOR(CO_GREEN));
        y++;
      };
      y = 0;
      x++;
    };
    x=1;
    while (x < 3) {
      line(out, p.x + r.b.x/3*x,p.y,p.x + r.b.x/3*x,p.y + r.b.y ,COLOR(CO_BLACK));
      line(out, p.x,p.y + r.b.y/3*x,p.x + r.b.x,p.y + r.b.y/3*x ,COLOR(CO_BLACK));
      x++;
    };
  };
  o->end_of_paint(o, r);

};



l_bool cht(l_long x, l_long y) {

  if (tictactic_dat[x][y] == 0) {
    tictactic_dat[x][y] = 2;
    return true;
  };

  return false;

};

l_bool computer_turn () { // Computer turn

  l_uchar x  = 0;
  l_uchar y  = 0;
  while(x < 3) {
    if (tictactic_dat[x][0] == tictactic_dat[x][1] != 0 ) if (cht(x, 2)) return 0;
    if (tictactic_dat[x][1] == tictactic_dat[x][2] != 0 ) if (cht(x, 0)) return 0;
    if (tictactic_dat[x][0] == tictactic_dat[x][2] != 0 ) if (cht(x, 1)) return 0;

    if (tictactic_dat[0][x] == tictactic_dat[1][x] != 0 ) if (cht(2, x)) return 0;
    if (tictactic_dat[1][x] == tictactic_dat[2][x] != 0 ) if (cht(0, x)) return 0;
    if (tictactic_dat[0][x] == tictactic_dat[2][x] != 0 ) if (cht(1, x)) return 0;

    x++;
  };

  if (tictactic_dat[0][0] == tictactic_dat[1][1] != 0 ) if (cht(2, 2)) return 0;
  if (tictactic_dat[1][1] == tictactic_dat[2][2] != 0 ) if (cht(0, 0)) return 0;
  if (tictactic_dat[0][2] == tictactic_dat[1][1] != 0 ) if (cht(2, 0)) return 0;
  if (tictactic_dat[1][1] == tictactic_dat[2][0] != 0 ) if (cht(0, 2)) return 0;


  x = 0;
  y = 0;
  while(x < 3) {
    while(y < 3) {
      if (cht(x,y)) return 0;
      y++;
    };
    y = 0;
    x++;
  };

  return 1; // I can't play ... It's not normal !
};

l_bool someonewin() { // Check if someone have win
  l_uchar x  = 0;
  l_uchar ok = 0;

  while(x < 3) {
    if ((tictactic_dat[x][0] == tictactic_dat[x][1])&&(tictactic_dat[x][1] == tictactic_dat[x][2])) ok = tictactic_dat[x][0];
    if ((tictactic_dat[0][x] == tictactic_dat[1][x])&&(tictactic_dat[1][x] == tictactic_dat[2][x])) ok = tictactic_dat[0][x];
    x++;
  };
  if ((tictactic_dat[0][0] == tictactic_dat[1][1])&&(tictactic_dat[1][1] == tictactic_dat[2][2])) ok = tictactic_dat[0][0];
  if ((tictactic_dat[0][2] == tictactic_dat[1][1])&&(tictactic_dat[1][1] == tictactic_dat[2][0])) ok = tictactic_dat[0][2];

  return ok;
};



void userclick (l_uchar x , l_uchar y) { // User click somewhere
  if ( !tictactic_dat[x][y] && !someonewin() ) {
    tictactic_dat[x][y] = 1;
    if (!someonewin())
      if (computer_turn()) DEBUG_printf("TicTacTic : Computer ERR !\n");
    draw();
  };
};

void cleanttt ( void ) {
  l_uchar x = 0;
  l_uchar y = 0;
  while(x < 3) {
    while(y < 3) {
      tictactic_dat[x][y] = 0;
      y++;
    };
    y = 0;
    x++;
  };
};

static p_menu create_game_menu(void) {
   p_menu menu;
   menu = new_menu(
                 new_sub_menu("Game", new_menu(
                    new_menu_item(TXT_RESTART, "", 0, MSG_GAMERESTART, NULL,
                    new_menu_line(
                    new_menu_item(TXT_EXIT, "ALT+F4", 0, MSG_CLOSE, NULL,
                    NULL)))),
                 new_sub_menu(TXT_HELP, new_menu(
                    new_menu_item(TXT_HELP, "", 0, MSG_GAMEHELP, NULL,
                    new_menu_line(
                    new_menu_item(TXT_ABOUT, "", 0, MSG_ABOUT, NULL,
                    NULL)))),
                 NULL))
              );
   return menu;
}

static void game_trans_ev ( p_object o, p_event event ) {

  RETVIEW(o, event);

  if ( event->type & EV_MOUSE ) {

    if ( OBJECT(mouse)->state & MO_SF_MOUSEUP  ) {

      t_rect  r = VIEW(o)->get_local_extent(VIEW(o));
      t_point p =  VIEW(o)->get_local_point(VIEW(o), mouse->where);
      l_int x = p.x / (r.b.x/3);
      l_int y = p.y / (r.b.y/3);

      userclick(x, y);
      clear_event(event);
    };
  };
};
static void trans_ev ( p_object o, p_event event ) {

  if ( event->type == EV_MESSAGE )
    switch ( event->message ) {
      case MSG_GAMERESTART : {
        cleanttt();
        draw();
        clear_event(event);
      }; break;
      case MSG_ABOUT: {
        seal2_app_about("Tic-Tac-Toe", load_image("/system/bmp/tictac.ico,32"));
//        msgbox(MW_INFO, MB_OK, "Tic-Tac-Toe\n  A famous little game!\n\n(c) Copyright 2001 Point Mad. All rights reserved.");
        clear_event(event);
      }; break;
      case MSG_GAMEHELP: {
        open_help("tttc.hlp");
        clear_event(event);
      }; break;
  };
};

void init_window ( void ) {

  t_rect   r = rect_assign(0, 0, 220, 150);

  p = appwin_init(_malloc(sizeof(t_appwin)),
                            r,
                            "Tic-Tac-Toe",
                            WF_MINIMIZE,
                            ap_id,
                            &trans_ev);


  if ( p ) VIEW(p)->align |= TX_ALIGN_CENTER;

  p->icon16 = load_image("/system/bmp/tictac.ico,16");

  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(p));

  if ( p ) {
	p_menuview Menu;
    r = VIEW(p)->size_limits(VIEW(p));
    r = rect_assign(r.a.x, r.a.y + 1, r.a.x, r.a.y + 20);
    Menu = hormenu_init(_malloc(sizeof(t_menuview)), r, create_game_menu());

    OBJECT(p)->insert(OBJECT(p), OBJECT(Menu));

    r = rect_assign(60, 45, 160, 145);

    tictactic = grfx_init(_malloc(sizeof(t_grfx)),r );

    VIEW(tictactic)->draw = &draw_game;
    OBJECT(tictactic)->translate_event = &game_trans_ev;
    OBJECT(p)->insert(OBJECT(p), OBJECT(tictactic));
  };
};

SetInfoAppName("Tic-Tac-Toe");
SetInfoDesciption("Are you going to be the best?");
SetInfoCopyright("(c) Copyright 2000,2001 Point Mad. All rights reserved");
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

