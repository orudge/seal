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
 * 04/04/2002: Updated with new icon and about dialog (orudge)
 */

#include <seal.h>
#include <app.h>
#include <button.h>
#include <grfx.h>
#include <dialogs.h>
#include <random.h>
#include <math.h>
#include <menus.h>
#include <stattext.h>

#define MSG_GAMERESTART 100001
#define MSG_GAMEHELP    100002

l_long HowManyMines  = 75;
p_grfx solvor = NULL;
l_uchar width = 20;
l_uchar height = 20;
l_char  solvor_dat[30][30];       // 0        1     2      3       4
                                 // Nothing, mine, Viwed, marked, Marked2
p_appwin p = NULL;
l_long fin = 0;
p_stattext st;

void cleansolvor () {
  l_uchar x = 0;
  l_uchar y = 0;
  l_long i = 0;
  while(x < width) {
    while(y < height) {
      solvor_dat[x][y] = 0;
      y++;
    };
    y = 0;
   x++;
  };

  while (i <  HowManyMines + 1) {
    x = abs(random() % (width - 2));
    y = abs(random() % (height - 2));
    x++;
    y++;
    if (solvor_dat[x][y] == 0) {
      solvor_dat[x][y] = 1;
      i++;
    };
  };

  fin = 0;

};

l_long howmany_good () {
  l_uchar x = 0;
  l_uchar y = 0;
  l_long i = 0;
  while(x < width) {
    while(y < height) {
      if (solvor_dat[x][y] == 4) i++;
      y++;
    };
    y = 0;
    x++;
  };
  return i;
};

l_long howmany_bad () {
l_uchar x = 0;
l_uchar y = 0;
l_long i = 0;
while(x < width) {
while(y < height) {
if (solvor_dat[x][y] == 3) i++;
y++;
};
y = 0;
x++;
};
return i;
};

l_long howmany (l_long x, l_long y) {
l_long i = 0;

if (solvor_dat[x + 1][y + 1] == 1) i++;
if (solvor_dat[x - 1][y - 1] == 1) i++;
if (solvor_dat[x + 1][y - 1] == 1) i++;
if (solvor_dat[x - 1][y + 1] == 1) i++;

if (solvor_dat[x][y + 1] == 1) i++;
if (solvor_dat[x][y - 1] == 1) i++;
if (solvor_dat[x + 1][y] == 1) i++;
if (solvor_dat[x - 1][y] == 1) i++;

if (solvor_dat[x + 1][y + 1] == 4) i++;
if (solvor_dat[x - 1][y - 1] == 4) i++;
if (solvor_dat[x + 1][y - 1] == 4) i++;
if (solvor_dat[x - 1][y + 1] == 4) i++;
if (solvor_dat[x][y + 1] == 4) i++;
if (solvor_dat[x][y - 1] == 4) i++;
if (solvor_dat[x + 1][y] == 4) i++;
if (solvor_dat[x - 1][y] == 4) i++;

return i;
};

void mark(l_long x, l_long y) {

if (solvor_dat[x][y] == 0) {
solvor_dat[x][y] = 3;
} else if (solvor_dat[x][y] == 1){
 solvor_dat[x][y] = 4;
} else if (solvor_dat[x][y] == 3){
solvor_dat[x][y] = 0;
} else if (solvor_dat[x][y] == 4){
solvor_dat[x][y] = 1;
};

};



l_long go(l_long x, l_long y) {

if (solvor_dat[x][y] == 1) { // It's a mine
solvor_dat[x][y] = 5;
fin = 1;
return 1;
};

if (solvor_dat[x][y] == 0) {
solvor_dat[x][y] = 2;
if (howmany(x,y) == 0) {
if ((solvor_dat[x][y + 1] == 0) && ( y != width - 1)) go(x, y + 1);
if ((solvor_dat[x][y - 1] == 0) && ( y != 0 )) go(x, y - 1);
if ((solvor_dat[x + 1][y] == 0) && ( x != width - 1)) go(x + 1, y);
if ((solvor_dat[x - 1][y] == 0) && ( y != 0 )) go(x - 1, y);
if ((solvor_dat[x + 1][y + 1] == 0)&& ( x != width - 1)&& ( y != width - 1)) go(x + 1, y + 1);
if ((solvor_dat[x - 1][y - 1] == 0)&& ( x != 0)&& ( y != 0)) go(x - 1, y - 1);
if ((solvor_dat[x + 1][y - 1] == 0)&& ( x != width - 1)&& ( y != 0)) go(x + 1, y - 1);
if ((solvor_dat[x - 1][y + 1] == 0)&& ( x != 0)&& ( y != width - 1)) go(x - 1, y + 1);
};

};
fin = 0;
return 0;
};

void draw_game ( p_view o ) {
  t_rect  r = o->get_local_extent(o);
  t_point pe;

  BITMAP *out = o->begin_paint(o, &pe, r);

  if (out) {
    rectfill(out, pe.x ,
                  pe.y ,
                  pe.x + r.b.x,
                  pe.y + r.b.y,
                  COLOR(CO_SYSx23));

if ( !fin ) {
l_long x = 0;
l_long y = 0;
l_uchar w = r.b.x / width;
l_uchar h = r.b.y / height;
while (x < width) {
while (y < height) {

      if ((solvor_dat[x][y] == 0) || (solvor_dat[x][y] == 1)) {

      button(out,   pe.x + (x * w) + 1,
                    pe.y + (y * h) + 1,
                    pe.x + (x * w) + w,
                    pe.y + (y * h) + h,
                 COLOR(CO_WHITE), COLOR(CO_SYSx18));

      } else if (solvor_dat[x][y] == 2) {


      if (howmany(x,y) !=  0) {
l_text txt = set_format_text(NULL, "%d", howmany(x,y));
      textout_draw_rect(out, o->font, txt, -1,
                                        pe.x + (x * w) + 2,
                                        pe.y + (y * h) + 2,
                                        pe.x + (x * w) + w - 1,
                                        pe.y + (y * h) + h - 1,
                                        TX_ALIGN_CENTERY|TX_ALIGN_CENTERX, COLOR(CO_BLACK), TX_NOCOLOR, 1);
_free(txt);
};
      } else {

      line(out,   pe.x + (x * w) + 3,
                    pe.y + (y * h) + 3,
                    pe.x + (x * w) + w - 3,
                    pe.y + (y * h) + h - 3,
                    COLOR(CO_BLACK));

      line(out,   pe.x + (x * w) + 3,
                    pe.y + (y * h) + h - 3,
                    pe.x + (x * w) + w - 3,
                    pe.y + (y * h) + 3,
                    COLOR(CO_BLACK));

      button(out,   pe.x + (x * w) + 1,
                    pe.y + (y * h) + 1,
                    pe.x + (x * w) + w,
                    pe.y + (y * h) + h,
                    COLOR(CO_WHITE), COLOR(CO_SYSx18));

      };


y++;
};
y = 0;
x++;
};

} else if (fin == 1) {
      textout_draw_rect(out, o->font, "BOOM! A mine exploded!", -1,
                                        pe.x ,
                                        pe.y ,
                                        pe.x + r.b.x,
                                        pe.y + r.b.y,
                                        TX_ALIGN_CENTERY, COLOR(CO_BLACK), COLOR(CO_SYSx23), 1);
} else {
      textout_draw_rect(out, o->font, "YOU WIN :)", -1,
                                        pe.x ,
                                        pe.y ,
                                        pe.x + r.b.x,
                                        pe.y + r.b.y,
                                        TX_ALIGN_CENTERY, COLOR(CO_BLACK), COLOR(CO_SYSx23), 1);
};

};
o->end_of_paint(o, r);

};

void   draw ( void ) { VIEW(solvor)->draw_me(VIEW(solvor)); };

static void trans_ev ( p_object o, p_event event ) {
  if ( event->type == EV_MESSAGE )
    switch ( event->message ) {
      case MSG_GAMERESTART : {
        cleansolvor();
        draw();
        clear_event(event);
      }; break;
      case MSG_ABOUT: {
//        msgbox(MW_INFO, MB_OK, "Solvor aka Minesweeper\nFind the mines!\n\n(c) Copyright 2001 Point Mad. All rights reserved.");
        seal2_app_about("Solvor aka Minesweeper", load_image("/system/bmp/solvor.ico,32"));
        clear_event(event);
      }; break;
      case MSG_GAMEHELP: {
        open_help("solvor.hlp");
        clear_event(event);
      }; break;
    };
};

static void trans_ev2 ( p_object o, p_event event ) {
  if ( event->type & EV_MOUSE ) {
    if ( !fin ) {
      t_point p = VIEW(o)->get_local_point(VIEW(o), mouse->where);
      t_rect  r = VIEW(o)->get_local_extent(VIEW(o));
      l_uchar w = r.b.x / width;
      l_uchar h = r.b.y / height;
      l_uchar x = p.x/w;
      l_uchar y = p.y/h;
      if ( OBJECT(mouse)->state & MO_SF_MOUSELUP  ) {
        if ( go(x, y) == 1) fin = 1;
        draw();
      } else if ( OBJECT(mouse)->state & MO_SF_MOUSERUP ) {
        mark(x,y);
        if (howmany_good() - 1 == HowManyMines && !howmany_bad() ) { fin = 2; };
        draw();
      };
      st->set_text(st,"[ %d / %d ]",howmany_good()+howmany_bad(),HowManyMines);
      clear_event(event);
    };
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

void init_window ( void )
{

   t_rect   r = rect_assign(0, 0, 310, 370);

   p = appwin_init(_malloc(sizeof(t_appwin)),
                            r,
                            "Solvor aka Minesweeper",
                            WF_MINIMIZE,
                            ap_id,
                            &trans_ev);


   if ( p ) VIEW(p)->align |= TX_ALIGN_CENTER;

   p->icon16 = load_image("/system/bmp/solvor.ico,16");

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(p));

      if ( p ) {

	p_menuview Menu;
    r = VIEW(p)->size_limits(VIEW(p));
    r = rect_assign(r.a.x, r.a.y + 1, r.a.x, r.a.y + 20);
    Menu = hormenu_init(_malloc(sizeof(t_menuview)), r, create_game_menu());

    OBJECT(p)->insert(OBJECT(p), OBJECT(Menu));

      r = rect_assign(5, 45, 305, 65);

      st = stattext_init(_malloc(sizeof(t_stattext)),r,0,"[ 0 / %d ]",HowManyMines);
      OBJECT(p)->insert(OBJECT(p), OBJECT(st));

      cleansolvor();
      r = rect_assign(5, 65, 305, 365);

      solvor = grfx_init(_malloc(sizeof(t_grfx)),r );

      VIEW(solvor)->draw = &draw_game;
      OBJECT(solvor)->translate_event =  &trans_ev2;
      OBJECT(p)->insert(OBJECT(p), OBJECT(solvor));



      };
};

SetInfoAppName("Solvor aka Minesweeper");
SetInfoDesciption("The famous game for SEAL!");
SetInfoCopyright("(c) Copyright 2000, 2001 Point Mad. All rights reserved");
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

