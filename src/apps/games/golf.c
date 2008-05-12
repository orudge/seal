/****************************************************************/
/*                                                              */
/*                          golf.c                              */
/*                                                              */
/*              The "Golf" card game for Seal 2                 */
/*                                                              */
/*                  Copyright (c) 2001,2002                     */
/*                      Tobias Johansson                        */
/*                    All Rights Reserved                       */
/*                                                              */
/* mail:  tobbe_snodd@hotmail.com                               */
/* web:   http://hem.passagen.se/sealfiles                      */
/*                                                              */
/* Golf is free software; you can redistribute it and/or        */
/* modify it under the terms of the GNU General Public License  */
/* as published by the Free Software Foundation; either version */
/* 2, or (at your option) any later version.                    */
/*                                                              */
/* Golf is distributed in the hope that it will be useful, but  */
/* WITHOUT ANY WARRANTY; without even the implied warranty of   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See    */
/* the GNU General Public License for more details.             */
/*                                                              */
/* You should have received a copy of the GNU General Public    */
/* License along with Golf; see the file COPYING.  If not,      */
/* write to the Free Software Foundation, 675 Mass Ave,         */
/* Cambridge, MA 02139, USA.                                    */
/*                                                              */
/****************************************************************/


#include <seal.h>
#include <app.h>
#include <view.h>
#include <cards.h>
#include <menus.h>
#include <helpsys.h>

#define MSG_GOLF_NEW   100001
#define MSG_GOLF_DECK  100002
#define MSG_GOLF_HELP  100003
#define MSG_CARDS_ABOUT 100004

p_appwin   winMain;
p_view     vGame;
p_menuview gameMenu;

t_card deck[53];

t_card piles[7][5]; 
t_card stock[20];
t_card waste[53];

l_int pilesCount[7];
l_int stockCount;
l_int wasteCount;


SetInfoAppName      ("Golf");
SetInfoDesciption   ("Simple cardgame for Seal 2.");
SetInfoCopyright    ("(c) Copyright 2001 Tobias Johansson. All rights reserved.");
SetInfoManufacturer ("Tobias Johansson");


void   init_game        (void);
void   redraw_vGame     (p_view v);
void   trans_event      (p_object o, p_event event);
p_menu create_game_menu (void);


void init_game(void) {
   l_int tc, tp;
   l_int curCard = 52;

   cards_init_deck(deck);
   cards_shuffle_deck(deck);

   // Fill up the seven lower piles.
   for (tp=0 ; tp<7 ;tp++) {
      for (tc=0 ; tc<5 ; tc++) {
         piles[tp][tc].showFace = true;
         piles[tp][tc].card = deck[curCard].card;
         curCard--;
      };
      pilesCount[tp] = 5;
   };

   // Fill up the stock.
   for (tc=0 ; tc<16 ; tc++) stock[tc] = deck[tc+1];
   stockCount = 16;

   // Add th topmost card to the waste.
   waste[0] = deck[17];
   waste[0].showFace = true;
   wasteCount = 1;
};



void redraw_vGame(p_view v) {
   t_point p;
   l_int   cCard, cPile, curY;
   t_rect  r = v->size_limits(v); 
   BITMAP  *out = v->begin_paint(v, &p, r);
   if (out) {
      // Draw green background.
      rectfill(out, p.x, p.y, p.x+600, p.y+400, COLOR(CO_GREEN));

      // Draw the seven piles.
      for (cPile=0 ; cPile<7 ; cPile++) {
         curY = 12;
         for (cCard=0 ; cCard<pilesCount[cPile] ; cCard++) {
            if (cCard-1 >= 0) curY += CARD_LOFFSET;
            cards_blit_card(out, p.x+15+(cPile*80), p.y+curY, piles[cPile][cCard]);
         };
      };

      // Draw the waste.
      for (cCard=0 ; cCard<wasteCount ; cCard++) {
         cards_blit_card(out, p.x+95+(cCard*7), p.y+230, waste[cCard]);
      };

      // Draw back of the stock.
      if (stockCount) {
         cards_blit_back(out, p.x+15, p.y+230);
      } else {
         cards_blit_symbol(out, p.x+15, p.y+230, C_EMPTY);
      };

      // Print out the number of cards in the stock.
      textout_printf(out, v->font, -1, p.x + 15, p.y + 330, 
                     p.x + 89, p.y + 345, TX_ALIGN_CENTER,
                     COLOR(CO_WHITE), TX_NOCOLOR, 0, "%d", stockCount);
   };
   v->end_of_paint(v, r);
};



void trans_event(p_object o, p_event event) {
   l_int c, cc, cPile;
   t_point p;
   RETVIEW(o, event);
   if (event->type & EV_MOUSE) {
      if (OBJECT(mouse)->state & MO_SF_MOUSELUP) {
         p = VIEW(o)->get_local_point(VIEW(o), mouse->where);

         if (p.x > +15 && p.x < +91 && p.y > +276 && p.y < +(276+CARD_HEIGHT)) {

            // The stock is clicked. Move a new card to the waste. 
            if (stockCount) {
               wasteCount ++;
               waste[wasteCount-1] = stock[stockCount-1];
               waste[wasteCount-1].showFace = true;
               stockCount --;
            };
         } else

         if (p.x > +15 && p.y > +55 && p.y < +220) {

            // One of the seven piles is clicked.
            cPile = (int) (p.x - 15) / 80;   // Which pile?
            c = pilesCount[cPile]-1;
            //msgbox(MW_INFO, MB_OK, "Pile: %d\n", cPile);

            // If the value of the clicked card equals the value
            // of the topmost card in the waste plus/minus one... 
            if (cards_get_value(piles[cPile][c]) == cards_get_value(waste[wasteCount-1]) - 1 || cards_get_value(piles[cPile][c]) == cards_get_value(waste[wasteCount-1]) + 1) {

               // ...and the card in the waste isn't a king...
               if (cards_get_value(waste[wasteCount-1]) < 13) {

                  // ...move the card to the waste!
                  wasteCount ++;
                  waste[wasteCount-1] = piles[cPile][c];
                  pilesCount[cPile]--;
               };
            };
         };

         redraw_vGame(vGame);
         clear_event(event);

         // Game over?
         if (wasteCount == 52) {
            msgbox(MW_INFO, MB_OK, "Well done! You did it!\n");
         };         
      };
   }; 

   if (event->type == EV_MESSAGE) {
      switch (event->message) {
         case MSG_CLOSE: {
            cards_uninit_deck();
         }; break;
         case MSG_GOLF_NEW: {
            init_game();
            redraw_vGame(vGame);
            clear_event(event);
         }; break;
         case MSG_GOLF_DECK: {
            cards_select_deck();
            redraw_vGame(vGame);
            clear_event(event);
         }; break;
         case MSG_GOLF_HELP: {
            open_help("./help/golf.hlp");
            clear_event(event);
         }; break;
         case MSG_ABOUT: {
            msgbox(MW_INFO, MB_OK, "\"Golf\" cardgame for Seal 2\n\n(c) 2001 Tobias Johansson\n\nEmail:   tobbe_snodd@hotmail.com\nWeb:   http://hem.passagen.se/sealfiles\n");
            clear_event(event);
         }; break;
         case MSG_CARDS_ABOUT: {
            cards_about();
            clear_event(event);
         }; break;
      };
   };
};



p_menu create_game_menu(void) {
   p_menu menu;
   menu = new_menu(
                 new_sub_menu("Game", new_menu(
                    new_menu_item("New", "", 0, MSG_GOLF_NEW, NULL,
                    new_menu_line(
                    new_menu_item("Select deck", "", 0, MSG_GOLF_DECK, NULL,
                    new_menu_line(
                    new_menu_item("Exit", "ALT+F4", 0, MSG_CLOSE, NULL, 
                    NULL)))))),
                 new_sub_menu("Help", new_menu(
                    new_menu_item("Help", "", 0, MSG_GOLF_HELP, NULL,
                    new_menu_line(
                    new_menu_item("About Golf", "", 0, MSG_ABOUT, NULL, 
                    new_menu_item("About \"cards.xdl\"", "", 0, MSG_CARDS_ABOUT, NULL, 
                    NULL))))), 
                 NULL))
              );
   return menu;
};



void create_window(void) {
  t_rect r;
  winMain = appwin_init(_malloc(sizeof(t_appwin)), 
                        rect_assign(0, 0, 585, 415), 
                        "Golf", 
                        WF_MINIMIZE|WF_ABOUT|WF_SYSMENU, 
                        ap_id, &trans_event);
  if (winMain) VIEW(winMain)->align |= TX_ALIGN_CENTER;
  winMain->icon16 = load_image("bmp/golf.ico,16");
  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(winMain));

  r = VIEW(winMain)->size_limits(VIEW(winMain));
  gameMenu = hormenu_init(_malloc(sizeof(t_menuview)), 
                          rect_assign(r.a.x, r.a.y + 1, r.a.x, r.a.y + 20), 
                          create_game_menu());
  OBJECT(winMain)->insert(OBJECT(winMain), OBJECT(gameMenu));

  vGame = view_init(_malloc(sizeof(t_view)),
                    rect_assign(3, 45, 580, 410));
  VIEW(vGame)->draw = &redraw_vGame;
  OBJECT(winMain)->insert(OBJECT(winMain), OBJECT(vGame));
};



app_begin(void) {
   if (ap_process == AP_INIT) {
      init_game();
      create_window();
   };
} app_end;
