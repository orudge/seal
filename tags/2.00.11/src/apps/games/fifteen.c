/***********************************************'*/
/*                                               */
/*                "fifteen.c"                    */
/*   A Seal version of the famous puzzle game.   */
/*                                               */
/*      Author:     Tobias Johansson             */
/*      Contact:    tobbe_snodd@hotmail.com      */
/*                                               */
/*                                               */
/*************************************************/
/*                                               */
/*   This is FREE software. Use it and spread    */
/*   it to others if you find it to be useful!   */
/*                                               */
/*************************************************/


// 4/11/2001 - Optimized for SEAL 2.00 on by Julien Etelain (a few changes)

// 9/1/2002  - Minor changes by Tobias Johansson
//                Changed system menu icon to "fifteen.ico"
//                Made the "Help" menu item open the helpfile properly
//                Removed some unused code

// 3/4/2002  - Better skin support by Tobias Johansson


#include <seal.h>
#include <app.h>
#include <view.h>
#include <time.h>
#include <stdlib.h>
#include <menus.h>
#include <helpsys.h>


SetInfoAppName("Fifteen");
SetInfoDesciption("A Seal Game");
SetInfoCopyright("(c) Copyright 2001 Tobias Johansson. All rights reserved");
SetInfoManufacturer("Tobias Johansson");

#define TXT_15_ABOUT "\"Fifteen\"\n\nVersion 1.2\n\n\n(c)2001 Tobias Johansson\n(tobbe_snodd@hotmail.com)\n"
#define MSG_GAMERESTART 100001
#define MSG_GAMEHELP    100002
#define MSG_GAMEABOUT   100003


p_appwin mainWin;
p_view gameArea;
p_menuview gameMenu;

l_int numList[16];
t_point srcCoord[16];



void srcCoord_fill(void) {
   l_int x, y;
   for (y=0 ; y<4 ; y++) {
      for (x=1 ; x<5 ; x++) {
         srcCoord[(y * 4) + x].x = (x - 1) * 55 + 1;
         srcCoord[(y * 4) + x].y = y * 55 - 1;
      };
   };
};



void init_numList(void) {
   l_int c, rval, tmp;
   for (c=0 ; c<16 ; c++) numList[c] = c;
   numList[16] = 0;
   srandom(time(0));
   for (c=1 ; c<17 ; c++) {
      rval = rand() % 15 + 1;
      tmp = numList[c];
      numList[c] = numList[rval];
      numList[rval] = tmp;
   };
};



l_int check_if_won(void) {
   l_int c, tmp;
   c = 0;
   for (tmp=1 ; tmp<16 ; tmp++) {
      if (numList[tmp] == tmp) c++;
   };
   if (c==15) return 1;
   return 0;
};



void move_brick(l_int from, l_int to) {
   numList[to] = numList[from];
   numList[from] = 0;
};



void draw_bricks(p_view v) {
   t_point p;
   t_rect rc;
   l_int x, y;
   t_rect r = v->get_local_extent(v);
   BITMAP *out = v->begin_paint(v, &p, r);
   if (out) {
      rectfill(out, p.x + 0, p.y + 0, p.x + 220, p.y + 220, COLOR(CO_DARKGRAY));
          rect(out, p.x + 0, p.y + 0, p.x + 220, p.y + 220, COLOR(CO_BLACK));
      for (y=0 ; y<4 ; y++) {
         for (x=1 ; x<5 ; x++) {
            if (numList[(4*y)+x]) {
               rectfill(out, p.x + 1 + (x - 1) * 55,
                             (y * 55) + p.y + 1,
                             p.x - 1 + x * 55,
                             (y + 1) * 55 + p.y - 1,
                             COLOR(CO_WHITE));
                   rect(out, p.x + 1 + (x - 1) * 55,
                             y * 55 + p.y + 1,
                             p.x - 1 + x * 55,
                             (y + 1) * 55 + p.y - 1,
                             COLOR(CO_BLACK));
         textout_printf(out, v->font, -1,
                             p.x+1+(x-1)*55, (y*55)+p.y+20,
                             p.x-1+(x)*55,   ((y+1)*55)+p.y-1,
                             TX_ALIGN_CENTERX, COLOR(CO_BLACK),
                             TX_NOCOLOR, 0, "%d", numList[(4*y)+x]);            };
         };
      };
   };
   v->end_of_paint(v, r);
};



static void game_trans_event(p_object o, p_event event) {
   t_point p;
   l_int curBrick;
   RETVIEW(o, event);
   if (event->type & EV_MOUSE) {
      if (OBJECT(mouse)->state & MO_SF_MOUSELUP) {
         p = VIEW(o)->get_local_point(VIEW(o), mouse->where);
         curBrick = 1;
         if (p.x > + 55)  curBrick ++;
         if (p.x > + 110) curBrick ++;
         if (p.x > + 165) curBrick ++;
         if (p.y > + 55)  curBrick += 4;
         if (p.y > + 110) curBrick += 4;
         if (p.y > + 165) curBrick += 4;

         if (numList[curBrick - 1] == 0 &&
                          curBrick != 1 &&
                          curBrick != 5 &&
                          curBrick != 9 &&
                          curBrick != 13) {
            move_brick(curBrick, curBrick - 1);
         } ;
         if (numList[curBrick + 1] == 0 &&
                          curBrick != 4 &&
                          curBrick != 8 &&
                          curBrick != 12 &&
                          curBrick != 16) {
            move_brick(curBrick, curBrick + 1);
         } ;
         if (numList[curBrick - 4] == 0 && (curBrick - 4) > 0) {
            move_brick(curBrick, curBrick - 4);
         } ;
         if (numList[curBrick + 4] == 0 && (curBrick + 4) < 17) {
            move_brick(curBrick, curBrick + 4);
         };
         curBrick = 0;

         clear_event(event);

         draw_bricks(gameArea);
         if (check_if_won()) {
            msgbox(MW_INFO, MB_OK, "Well done! You did it!");
            init_numList();
            clear_event(event);
            draw_bricks(gameArea);
         };
      };
   };
};



static void trans_event(p_object o, p_event event) {
   if (event->type == EV_MESSAGE) {
      switch (event->message) {
         case MSG_GAMERESTART: {
            init_numList();
            draw_bricks(gameArea);            
            clear_event(event);
         }; break;
         case MSG_GAMEHELP: {
            open_help("./help/fifteen.hlp");
            clear_event(event);
         }; break;
         case MSG_ABOUT: {
            msgbox(MW_INFO, MB_OK, TXT_15_ABOUT);
            clear_event(event);
         }; break;
      };
   };
};



static p_menu create_game_menu(void) {
   p_menu menu;
   menu = new_menu(
                 new_sub_menu("Game", new_menu(
                    new_menu_item("Restart", "", 0, MSG_GAMERESTART, NULL,
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
};



void create_window(void) {
   t_rect r, s;
   r = rect_assign(100, 100, 330, 370);
   mainWin = appwin_init(malloc(sizeof(t_appwin)), r, "Fifteen",
                         WF_MINIMIZE|WF_ABOUT, ap_id, &trans_event);
   mainWin->icon16 = load_image("./bmp/fifteen.ico,16");
   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(mainWin));

   s = VIEW(mainWin)->size_limits(VIEW(mainWin));

   r = rect_assign(r.a.x, r.a.y, r.b.x + 3, r.a.y + s.a.y + 252);
   VIEW(mainWin)->change_bounds(VIEW(mainWin), r);
   r = s;

   r = rect_assign(r.a.x, r.a.y, r.a.x, r.a.y + 20);
   gameMenu = hormenu_init(_malloc(sizeof(t_menuview)), r, create_game_menu());
   OBJECT(mainWin)->insert(OBJECT(mainWin), OBJECT(gameMenu));

   r = rect_assign(r.a.x + 4, r.b.y + 6, r.a.x + 224, r.b.y + 226);
   gameArea = view_init(_malloc(sizeof(t_view)), r);
   gameArea->draw_under_view(gameArea);
   VIEW(gameArea)->draw = &draw_bricks;
   OBJECT(gameArea)->translate_event = &game_trans_event;
   OBJECT(gameArea)->set_options(OBJECT(gameArea), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
   OBJECT(mainWin)->insert(OBJECT(mainWin), OBJECT(gameArea));
};



app_begin(void) {
   if (ap_process == AP_INIT) {
      create_window();
      srcCoord_fill();
      init_numList();
      draw_bricks(gameArea);
   };
} app_end;



