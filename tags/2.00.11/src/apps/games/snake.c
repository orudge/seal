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

/* Snake game by Bad Sector */

#include <allegro.h>
#include <seal.h>
#include <app.h>
#include <button.h>
#include <menus.h>
#include <grfx.h>

#define MSG_ABOUT 100001
#define nn 00

#define levels 3
#define startlevel 2

int levelmap[levels][20][20] = {
/* level 1 */
{{nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,02,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,02,nn},
 {nn,02,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,02,nn},
 {nn,02,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,02,nn},
 {nn,02,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,02,nn},
 {nn,02,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,02,nn},
 {nn,02,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,02,nn},
 {nn,02,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,02,nn},
 {nn,02,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,02,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn}},
/* level 2 */
{{nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,02,nn,nn,nn,nn,nn,nn,02,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,02,nn,nn,nn,nn,nn,nn,02,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,02,nn,nn,nn,nn,nn,nn,02,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,02,02,02,nn,nn,02,02,02,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,02,02,02,02,02,nn,nn,nn,nn,nn,nn,nn,nn,02,02,02,02,02,nn},
 {nn,02,nn,nn,nn,02,nn,nn,nn,nn,nn,nn,nn,nn,02,nn,nn,nn,02,nn},
 {nn,02,nn,nn,nn,02,nn,nn,nn,nn,nn,nn,nn,nn,02,nn,nn,nn,02,nn},
 {nn,02,nn,nn,nn,02,nn,nn,nn,nn,nn,nn,nn,nn,02,nn,nn,nn,02,nn},
 {nn,nn,nn,02,nn,02,nn,nn,nn,nn,nn,nn,nn,nn,02,nn,02,nn,nn,nn},
 {nn,nn,nn,02,nn,02,nn,nn,nn,nn,nn,nn,nn,nn,02,nn,02,nn,nn,nn},
 {nn,nn,nn,02,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,02,nn,nn,nn},
 {nn,nn,nn,02,02,02,02,02,02,02,02,02,02,02,02,02,02,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn}},
/* level 3 */
{{nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,02,02,02,02,02,02,02,02,02,02,02,02,02,nn,nn,nn,nn,nn,nn},
 {nn,02,nn,nn,nn,nn,nn,nn,nn,02,nn,nn,nn,02,nn,nn,nn,nn,nn,nn},
 {nn,02,nn,nn,nn,nn,nn,nn,nn,02,nn,nn,nn,02,nn,nn,nn,nn,nn,nn},
 {nn,02,nn,02,05,02,nn,nn,nn,02,nn,nn,nn,02,nn,nn,02,02,02,02},
 {nn,02,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,02,nn,nn,nn,nn,nn,nn},
 {nn,02,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,02,nn,nn,nn,nn,nn,nn},
 {nn,02,nn,nn,nn,nn,nn,nn,nn,02,nn,nn,nn,02,nn,nn,02,02,02,02},
 {nn,02,nn,nn,nn,nn,nn,nn,nn,02,nn,nn,nn,02,nn,nn,nn,nn,nn,nn},
 {nn,02,02,02,02,02,02,02,02,02,02,02,02,02,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn},
 {nn,nn,nn,nn,nn,nn,nn,02,nn,02,nn,nn,nn,nn,nn,02,02,nn,02,02},
 {nn,nn,nn,nn,nn,nn,02,nn,nn,nn,02,nn,nn,nn,nn,02,nn,nn,nn,02},
 {nn,nn,nn,nn,nn,02,nn,nn,nn,nn,nn,02,nn,nn,nn,02,nn,nn,nn,02},
 {nn,nn,nn,nn,nn,02,nn,nn,05,nn,nn,02,nn,nn,nn,02,nn,nn,nn,02},
 {nn,nn,nn,nn,nn,nn,02,02,02,02,02,nn,nn,nn,nn,02,02,02,02,02},
 {nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn}}
};

int map[20][20];

static DATAFILE *dat = NULL;

p_appwin main;
p_grfx scr;

t_point snake[40];
l_int size = 4;
l_int d = 0;
l_int score = 0;
l_int sctmr = 0;
l_int level = 1;
l_int lives = 3;

void handle_event(p_object o, p_event event)
{
  RETVIEW(o, event);

  if (o->phase == PH_PREPROCESS && event->type == EV_KEYBOARD && OBJECT(keyb)->state & KB_SF_KEYDOWN) {
    switch (KEY_TO(keyb->code)) {
      case KB_LEFT: if (d != 1) {
        d = 3;
      } break;
      case KB_RIGHT: if (d != 3) {
        d = 1;
      } break;
      case KB_UP: if (d != 2) {
        d = 0;
      } break;
      case KB_DOWN: if (d != 0) {
        d = 2;
      } break;
    };
  };
  if (event->type == EV_MESSAGE) {
    switch (event->message) {
      case MSG_ABOUT: {
        msgbox(MW_INFO, MB_OK, "Bad Sector's Snake version 1.00\n(C) 2001 Kostas Michalopoulos aka Bad Sector\n\ne-mail: michalopoylos@hotmail.com\nWWW: http://www.badseal.org/\n\nEnjoy Snake!");
        clear_event(event);
      } break;
    };
  };
}

void draw_block(l_int x, l_int y, l_int block, BITMAP *out, t_rect r, t_point p)
{
  draw_sprite(out, (BITMAP*)GET_DATA(dat, block+1), r.a.x+p.x+x*17, r.a.y+p.y+y*17);
}

void main_draw(p_view o)
{
  t_rect r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);
  if (out) {
    l_int x, y;
    for (y=0;y<20;y++)
    for (x=0;x<20;x++) draw_block(x, y, map[x][y], out, r, p);

    for (x=0;x<size;x++) draw_block(snake[x].x, snake[x].y, 1, out, r, p);

    textout_printf(out, o->font, -1, 1+p.x, 1+p.y, o->bounds.b.x+p.x, 20+p.y, TX_ALIGN_LEFT, COLOR(0), TX_NOCOLOR, 0, "Snakes: %i    Level: %i    Score: %i", lives, level, score);
  };
  o->end_of_paint(o, r);
}

void refresh(p_object o)
{
  p_view vo = VIEW(main);
  l_int i;

  if (!o->is_state(o, OB_SF_FOCUSED)) return;

  switch (d) {
    case 0: {
      for (i=size;i>0;i--) snake[i] = snake[i-1];
      snake[0].y--;
      if (snake[0].y == -1) snake[0].y = 19;
      VIEW(scr)->draw(VIEW(scr));
    } break;
    case 1: {
      for (i=size;i>0;i--) snake[i] = snake[i-1];
      snake[0].x++;
      if (snake[0].x == 20) snake[0].x = 0;
      VIEW(scr)->draw(VIEW(scr));
    } break;
    case 2: {
      for (i=size;i>0;i--) snake[i] = snake[i-1];
      snake[0].y++;
      if (snake[0].y == 20) snake[0].y = 0;
      VIEW(scr)->draw(VIEW(scr));
    } break;
    case 3: {
      for (i=size;i>0;i--) snake[i] = snake[i-1];
      snake[0].x--;
      if (snake[0].x == -1) snake[0].x = 19;
      VIEW(scr)->draw(VIEW(scr));
    } break;
  };
  sctmr++;
  if (sctmr == 5) {
    sctmr = 0;
    if (score > 0) score--;
  };
  for (i=1;i<=size;i++) if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) { /* collision on body */
    l_int x, y;
    sound(3000);delay(100);nosound();
    msgbox(MW_INFO, MB_OK, "Collision on body!");
    lives--;
    snake[0].x = 10;
    snake[0].y = 10;
    snake[1].x = 10;
    snake[1].y = 11;
    snake[2].x = 10;
    snake[2].y = 12;
    snake[3].x = 10;
    snake[3].y = 13;
    d = 0;
    size = 4;
    sctmr = 0;
    if (lives == -1) {
      msgbox(MW_INFO, MB_OK, "Game over!");
      for (y=0;y<20;y++)
      for (x=0;x<20;x++) map[x][y] = 0;
      map[10][15] = 4;
      score = 0;
      lives = 3;
      level = 1;
    } else {
      msgbox(MW_INFO, MB_OK, "You have %i snakes left.", lives);
    };
    break;
  };
  if (map[snake[0].x][snake[0].y] == 5) { /* on door */
    l_int x, y;
    for (y=0;y<20;y++)
    for (x=0;x<20;x++) if (map[x][y] == 5) if (x != snake[0].x && y != snake[0].y) {
      snake[0].x = x;
      snake[0].y = y;
      break;
    };
  };
  if (map[snake[0].x][snake[0].y] == 3) { /* on heart */
    map[snake[0].x][snake[0].y] = 0;
    lives++;
    score += 20;
  };
  if (map[snake[0].x][snake[0].y] == 4) { /* on box */
    l_int x, y, stop;
    map[snake[0].x][snake[0].y] = 0;
    do {
      x = random() % 20;
      y = random() % 20;
      for (i=0;i<=size;i++) if (snake[i].x == x && snake[i].y == y) continue;
    } while (map[x][y] != 0);
    map[x][y] = 4;
    if (size > 36) {
      snake[0].x = 10;
      snake[0].y = 10;
      snake[1].x = 10;
      snake[1].y = 11;
      snake[2].x = 10;
      snake[2].y = 12;
      snake[3].x = 10;
      snake[3].y = 13;
      d = 0;
      size = 4;
      sctmr = 0;
      score += 100;
      level++;
      if (level % 5 == 0) {
        map[5][3] = 3;
      };
    } else {
      snake[size] = snake[size-1];
      size++;
      snake[size] = snake[size-1];
      size++;
      sctmr = 0;
      score += 10;
    };
  };
}

void app_init()
{
  t_rect r = rect_assign(0, 0, 344, 361);
  l_int x, y;

  for (y=0;y<20;y++)
  for (x=0;x<20;x++) map[x][y] = levelmap[startlevel][y][x];

  map[10][15] = 4;

  snake[0].x = 10;
  snake[0].y = 10;
  snake[1].x = 10;
  snake[1].y = 11;
  snake[2].x = 10;
  snake[2].y = 12;
  snake[3].x = 10;
  snake[3].y = 13;

  main = appwin_init(malloc(sizeof(t_appwin)), r, "Bad Sector's Snake", WF_MINIMIZE, ap_id, &handle_event);
  main->icon16 = (BITMAP*)GET_DATA(dat, 0);
  main->menu = standard_system_menu_items(main,
               new_menu_line(
               new_menu_item("About Bad Sector's Snake...", 0, 0, MSG_ABOUT, NULL,
  NULL)));
  VIEW(main)->align |= TX_ALIGN_CENTER;
  OBJECT(main)->process_tick = 150;
  OBJECT(main)->func_callback = &refresh;
  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(main));
  init_stillprocess(OBJECT(main), 150);

  r = rect_assign(2, 19, 341, 359);
  scr = grfx_init(malloc(sizeof(t_grfx)), r);
  VIEW(scr)->draw = &main_draw;
  OBJECT(main)->insert(OBJECT(main), OBJECT(scr));
}
SetInfoAppName("Snake");
SetInfoDesciption("The famous game for Seal!");
SetInfoCopyright("(c) Copyright 2001 Bad Sector. All rights reserved");
SetInfoManufacturer("Bad Sector");

app_begin(void)
{
  if (ap_process == AP_INIT) {
    AP_SETNUMOFCALLS(1);
    dat = conv_to_skipcolor_data(DLXGetFileData(ap_id), CO_SKIP_RED, CO_SKIP_GREEN, CO_SKIP_BLUE);
    app_init();
  };
} app_end;

