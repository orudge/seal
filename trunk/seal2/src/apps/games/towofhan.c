/***********************************************'*/
/*                                               */
/*                 "towofhan.c"                  */
/* A Seal version of the "Towers of Hanoi" game. */
/*                                               */
/*       Author:     Tobias Johansson            */
/*       Contact:    tobbe_snodd@hotmail.com     */
/*                                               */
/*                                               */
/*************************************************/
/*                                               */
/*   This is FREE software. Use it and spread    */
/*   it to others if you find it to be useful!   */
/*                                               */
/*************************************************/

// 4/11/2001 - Optimized for SEAL 2.00 by Julien Etelain (a few changes)

// 7/1/2002 - Minor changes by Tobias Johansson:
//                Changed background color to color_3d_face.
//                Changed system menu icon to "towofhan.ico".
//                Made the "Help" menu item open the helpfile properly
//                Removed some unused code.

// 3/4/2002 - Better skin support by Tobias Johansson



#include <seal.h>
#include <app.h>
#include <view.h>
#include <menus.h>
#include <helpsys.h>

SetInfoAppName("Towers of Hanoi");
SetInfoDesciption("A seal game");
SetInfoCopyright("(c) Copyright 2001 Tobias Johansson. All rights reserved");
SetInfoManufacturer("Tobias Johansson");


#define  TXT_HANOI_ABOUT  "\"Towers of Hanoi\"\n\nVersion 1.1\n\n\n(c)2001 Tobias Johansson\n(tobbe_snodd@hotmail.com)\n"
#define  MSG_GAMERESTART  100001
#define  MSG_GAMEHELP     100002
#define  MSG_GAMEABOUT    100003

l_int TowRings[4][8];
l_int TowHeight[4];
l_int TowFrom  = 0;
l_int NumMoves = 0;

l_int RingOffsetY[] = {0, 105, 90, 75, 60, 45};
l_int RingOffsetX[] = {40, 28, 21, 14, 7, 0};
l_int RingColor[]   = {CO_BROWN, CO_YELLOW, CO_RED, CO_LIGHTMAGENDA, CO_GREEN, CO_BLUE};
l_int c;
t_rect r;

p_appwin mainWin;
p_view gameArea;
p_menuview gameMenu;



void draw_game(p_view v) {
   t_point p;
   t_rect rc;
   BITMAP *out;
   l_int curTow, curRing, curRingNum, OffX;
   t_rect r = v->get_local_extent(v);

   v->draw_under_view(v);

   out = v->begin_paint(v, &p, r);
   if (out) {
     // rectfill (out, p.x, p.y, p.x + r.b.x, p.y + r.b.y, color_3d_face);

      rectfill (out, p.x + 8, p.y + 120, p.x + 282, p.y + 127, COLOR(CO_BROWN));
      rect     (out, p.x + 8, p.y + 120, p.x + 282, p.y + 127, COLOR(CO_BLACK));

      if (TowFrom) {
         rectfill (out, (TowFrom - 1) * 90 + p.x + 10,  p.y + 130, 
                         TowFrom * 90 + p.x + 10 ,      p.y + 133, 
                         COLOR(CO_BLACK));
      };
      for (curTow=1 ; curTow<4 ; curTow++) {
         OffX = ((curTow - 1) * 90) + 10 + p.x;
         rectfill (out, OffX + 40,  p.y + 40, OffX + 50,  p.y + 45, COLOR(CO_BROWN));
         rect     (out, OffX + 40, p.y + 40, OffX + 50, p.y + 45, COLOR(CO_BLACK));

         for (curRing=1 ; curRing<6 ; curRing++) {
            curRingNum = TowRings[curTow][curRing];
            rectfill (out, OffX + RingOffsetX[curRingNum], 
                           p.y  + RingOffsetY[curRing], 
                           OffX + 90 - RingOffsetX[curRingNum], 
                           p.y  + RingOffsetY[curRing] + 14, 
                           COLOR(RingColor[curRingNum]));

            if (!curRingNum) {
              line(out, OffX + RingOffsetX[curRingNum], 
                        p.y  + RingOffsetY[curRing], 
                        OffX + RingOffsetX[curRingNum], 
                        p.y  + RingOffsetY[curRing] + 14, 
                        COLOR(CO_BLACK));

              line(out, OffX + 90 -RingOffsetX[curRingNum], 
                        p.y  + RingOffsetY[curRing], 
                        OffX + 90 - RingOffsetX[curRingNum], 
                        p.y  + RingOffsetY[curRing] + 14, 
                        COLOR(CO_BLACK));
            } else {
              rect(out, OffX + RingOffsetX[curRingNum], 
                        p.y  + RingOffsetY[curRing], 
                        OffX + 90 - RingOffsetX[curRingNum], 
                        p.y  + RingOffsetY[curRing] + 15, 
                        COLOR(CO_BLACK));
            };
         };
      };
   };
   v->end_of_paint(v, r);
};


void reset_game(void) {
   l_int tmp;
   NumMoves = 0;
   TowFrom = 0;
   for (tmp=0 ; tmp<3 ; tmp++) TowHeight[tmp] = 0;
   TowHeight[3] = 5;

   for (tmp=0 ; tmp<6 ; tmp++) {
      TowRings[0][tmp] = 0;
      TowRings[1][tmp] = 0;
      TowRings[2][tmp] = 0;
   };
   for (tmp=1 ; tmp<6 ; tmp++) TowRings[3][tmp] = 6-tmp;
   draw_game(gameArea);
};



l_int has_won(void) {
   if (TowHeight[1] == 5) {
      return 1;
   };
   return 0;
};

static void game_translate_event(p_object o, p_event event) {
   l_int SelTow;
   t_point p;
   RETVIEW(o, event);
   if (event->type & EV_MOUSE) {
      if (OBJECT(mouse)->state & MO_SF_MOUSELUP) {
         r = VIEW(o)->get_local_extent(VIEW(o));
         p = VIEW(o)->get_local_point(VIEW(o), mouse->where);
         p.x = p.x - r.a.x;
         p.y = p.y - r.a.y;

         SelTow = 0;
         if (p.x < 90) SelTow = 1;
         if (p.x > 90) SelTow = 2;
         if (p.x > 180) SelTow = 3;
         if (TowFrom == 0 && TowHeight[SelTow] > 0) {
             TowFrom = SelTow;
             clear_event(event);
         }
         else if (TowFrom != 0 && TowFrom != SelTow) {
            if (TowRings[TowFrom][TowHeight[TowFrom]] < TowRings[SelTow][TowHeight[SelTow]] || TowHeight[SelTow] == 0) {
               TowHeight[SelTow]++;
               TowRings[SelTow][TowHeight[SelTow]] = TowRings[TowFrom][TowHeight[TowFrom]];
               TowRings[TowFrom][TowHeight[TowFrom]] = 0;
               TowHeight[TowFrom]--;
               NumMoves++;
               if (has_won()) {
                  TowFrom = 0;
                  draw_game(gameArea);
                  msgbox(MW_INFO, MB_OK, "Well done! You did it in %d\nmoves.", NumMoves);
               };
            };
            TowFrom = 0;
            clear_event(event);
         };
         draw_game(gameArea);
      };
   };
};

static void translate_event(p_object o, p_event event) {
   if (event->type == EV_MESSAGE) {
      switch (event->message) {
         case MSG_GAMERESTART: {
            reset_game();
            clear_event(event);
         }; break;
         case MSG_GAMEHELP: {
            open_help("./help/towofhan.hlp");
            clear_event(event);
         }; break;
         case MSG_ABOUT: {
            msgbox(MW_INFO, MB_OK, TXT_HANOI_ABOUT);
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
   t_rect r = rect_assign(50, 50, 350, 245);
   mainWin = appwin_init(malloc(sizeof(t_appwin)), r,
                         "Towers of Hanoi", 
                         WF_MINIMIZE|WF_ABOUT,
                         ap_id, &translate_event);
   mainWin->icon16 = load_image("bmp/towofhan.ico,16");
   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(mainWin));

   r = VIEW(mainWin)->size_limits(VIEW(mainWin));
   r = rect_assign(r.a.x, r.a.y + 1, r.a.x, r.a.y + 20);
   gameMenu = hormenu_init(_malloc(sizeof(t_menuview)), 
                           r,  create_game_menu());

   OBJECT(mainWin)->insert(OBJECT(mainWin), OBJECT(gameMenu));

   r = rect_assign(5, 45, 290, 190);
   gameArea = view_init(_malloc(sizeof(t_view)), r);
   VIEW(gameArea)->draw = &draw_game;
   OBJECT(gameArea)->translate_event = &game_translate_event;
   OBJECT(gameArea)->set_options(OBJECT(gameArea), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
   OBJECT(mainWin)->insert(OBJECT(mainWin), OBJECT(gameArea));
};



app_begin(void) {
   if (ap_process == AP_INIT) {
      create_window();
      reset_game();
   };
   if (ap_process == AP_FREE) {
   };
   if (ap_process == AP_DONE) {
   };
} app_end;


