/****************************************************************/
/*                                                              */
/*                          cruel.c                             */
/*                                                              */
/*              The "Cruel" card game for Seal 2                */
/*                                                              */
/*                     Copyright (c) 2002                       */
/*                      Tobias Johansson                        */
/*                    All Rights Reserved                       */
/*                                                              */
/* mail:  tobbe_snodd@hotmail.com                               */
/* web:   http://hem.passagen.se/sealfiles                      */
/*                                                              */
/* Cruel is free software; you can redistribute it and/or       */
/* modify it under the terms of the GNU General Public License  */
/* as published by the Free Software Foundation; either version */
/* 2, or (at your option) any later version.                    */
/*                                                              */
/* Cruel is distributed in the hope that it will be useful, but */
/* WITHOUT ANY WARRANTY; without even the implied warranty of   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See    */
/* the GNU General Public License for more details.             */
/*                                                              */
/* You should have received a copy of the GNU General Public    */
/* License along with Cruel; see the file COPYING.  If not,     */
/* write to the Free Software Foundation, 675 Mass Ave,         */
/* Cambridge, MA 02139, USA.                                    */
/*                                                              */
/****************************************************************/


#include <seal.h>
#include <app.h>
#include <view.h>
#include <cards.h>
#include <menus.h>
#include <button.h>
#include <helpsys.h>

#define MSG_CRUEL_NEW     100001
#define MSG_CRUEL_HELP    100002
#define MSG_CARDS_ABOUT   100003
#define MSG_CRUEL_SELGAME 100004
#define MSG_DEAL_CARDS    100005


p_appwin   winMain;
p_view     vGame;
p_menuview gameMenu;

t_card deck[53];
t_card piles[16][13];     // 0-11 is the lower piles, 12-15 suite piles
l_int  piles_count[16];

l_int fromPile, curGame, wasDoubleClicked = 0;


SetInfoAppName      ("Cruel");
SetInfoDesciption   ("Simple cardgame for Seal 2.");
SetInfoCopyright    ("(c) Copyright 2001 Tobias Johansson. All rights reserved.");
SetInfoManufacturer ("Tobias Johansson");


void   redraw_win_title (void);
void   init_game        (void);
void   move_card        (l_int fPile, l_int tPile);
void   deselect_all     (void);
void   deal_cards       (void);
void   redraw_vGame     (p_view v);
void   trans_event      (p_object o, p_event event);
p_menu create_game_menu (void);



void redraw_win_title(void) {
   // Redraws the title of the window to show
   // which game is currently selected.
   if (winMain) {
      _free(WINDOW(winMain)->caption);
      WINDOW(winMain)->caption = set_format_text(NULL, "Cruel - game %d", curGame);

      TEST_SUB_VIEWS(VIEW(winMain), WINDOW(winMain)->draw_title(WINDOW(winMain)));
  };
};



void init_game(void) {
   l_int  curCard, curPile;
   l_int  curSuitePile = 12;

   fromPile = 0;
   redraw_win_title();

   for (curPile=0 ; curPile<=15 ; curPile++) piles_count[curPile] = 0;

   // Move all the aces from the deck to the suite piles
   curPile = 0;
   for (curCard=1; curCard<53 ; curCard++) {
      deck[curCard].showFace = true;
      if (cards_get_value(deck[curCard]) == 1) {
         piles[curSuitePile][0] = deck[curCard];
         piles_count[curSuitePile] = 1;
         curSuitePile++;
      } else {
         if (piles_count[curPile] == 4) curPile++;
         piles[curPile][piles_count[curPile]] = deck[curCard];
         piles_count[curPile]++;
      };
   };
};



void move_card(l_int fPile, l_int tPile) {
   piles_count[tPile]++;
   piles[tPile][piles_count[tPile]-1] = piles[fPile][piles_count[fPile]-1];
   piles_count[fPile]--;
};



void deselect_all(void) {
   l_int cp, cc;
   for (cp=0 ; cp<16 ; cp++) {
      for (cc=0 ; cc<piles_count[cp] ; cc++) {
         piles[cp][cc].selected = false;
      };
   };
   fromPile = 0;
};



void deal_cards(void) {
   l_int cp, cc, numCards = 0;
   deselect_all();
   cards_init_deck(deck);

   for (cp=0 ; cp<12 ; cp++) {
      for (cc=0 ; cc<piles_count[cp] ; cc++) {
         numCards++;
         deck[numCards] = piles[cp][cc];
         deck[numCards].showFace = true;
      };
   };

   for (cp=0 ; cp<12 ; cp++) piles_count[cp] = 0;

   cp = 0;
   cc = 1;
   while (cc<=numCards) {
      piles_count[cp]++;
      piles[cp][piles_count[cp]-1] = deck[cc];
      if (piles_count[cp] == 4 && cp+1<12) cp++;
      cc++;
   };   
};



void redraw_vGame(p_view v) {
   t_point p;
   l_int   cPile;
   t_rect  r = v->size_limits(v); 
   BITMAP  *out = v->begin_paint(v, &p, r);
   if (out) {
      // Draw green background.
      rectfill(out, p.x, p.y, p.x+600, p.y+400, COLOR(CO_GREEN));

      // Draw the four suite piles
      for (cPile=0 ; cPile<4 ; cPile++) 
         cards_blit_card(out, 
                         p.x + 30 + ((CARD_WIDTH + 15) * cPile), 
                         p.y + 12, 
                         piles[12+cPile][piles_count[12+cPile]-1]);

      for (cPile=0 ; cPile<6 ; cPile++) 
         if (piles_count[cPile])
            cards_blit_card(out, 
                            p.x + 30 + ((CARD_WIDTH + 15) * cPile), 
                            p.y + CARD_HEIGHT + 40, 
                            piles[cPile][piles_count[cPile]-1]);

      for (cPile=6 ; cPile<12 ; cPile++) 
         cards_blit_card(out, 
                         p.x + 30 + ((CARD_WIDTH + 15) * (cPile - 6)), 
                         p.y + (2 * CARD_HEIGHT) + 55, 
                         piles[cPile][piles_count[cPile]-1]);

      cards_blit_symbol(out, 
                        p.x + 460, 
                        p.y + 12, 
                        C_CIRCLE);

      textout_printf(out, v->font, -1, 
                     p.x + 462, p.y + 95, 
                     p.x + 460 + CARD_WIDTH, p.y + 120, 
                     TX_ALIGN_CENTER, COLOR(CO_BLACK), 
                     TX_NOCOLOR, 0, "DEAL");
      
   };
   v->end_of_paint(v, r);
};



void trans_event(p_object o, p_event event) {
   l_int cPile, cp, selGame;
   t_point p;
   RETVIEW(o, event);
   if (event->type & EV_MOUSE) {
      p = VIEW(vGame)->get_local_point(VIEW(vGame), mouse->where);

      if (OBJECT(mouse)->state & MO_SF_MOUSELUP) {

         if (wasDoubleClicked) {
            wasDoubleClicked = 0;
            deselect_all();
         } else

         // A "suite pile" was clicked
         if (p.x>29 && p.x<359 && p.y>12 && p.y<108) {
            cPile = (int) (p.x - 29) / (CARD_WIDTH + 15);
            cPile += 12;
            if (fromPile && (cards_get_value(piles[fromPile-1][piles_count[fromPile-1]-1]) == cards_get_value(piles[cPile][piles_count[cPile]-1]) + 1) && (cards_get_suite(piles[fromPile-1][piles_count[fromPile-1]-1]) == cards_get_suite(piles[cPile][piles_count[cPile]-1]))) {
               move_card(fromPile-1, cPile);
               deselect_all();
            } else deselect_all();
         } else

         // Another pile was clicked
         if (p.x>29 && p.x<531 && p.y>135 && p.y<343) {
            cPile = (int) (p.x - 29) / (CARD_WIDTH + 15);
            if (p.y>240) cPile+=6;

            if (fromPile && (cards_get_value(piles[fromPile-1][piles_count[fromPile-1]-1]) == cards_get_value(piles[cPile][piles_count[cPile]-1]) - 1) && (cards_get_suite(piles[fromPile-1][piles_count[fromPile-1]-1]) == cards_get_suite(piles[cPile][piles_count[cPile]-1]))) { 
               move_card(fromPile-1, cPile);
               deselect_all();
            } else if (piles_count[cPile] && !fromPile) {
               piles[cPile][piles_count[cPile]-1].selected = true;
               fromPile = cPile+1;
            } else deselect_all();
         } else

         // Deal symbol was clicked
         if (p.x>459 && p.x<531 && p.y>12 && p.y<108) {
            deal_cards();
         } else deselect_all();

         if (piles_count[12]+piles_count[13]+piles_count[14]+piles_count[15] == 52) 
            msgbox(MW_INFO, MB_OK, "Well done! You won!");

         redraw_vGame(vGame);
         clear_event(event);
      };

      if (OBJECT(mouse)->state & MO_SF_MOUSELDOUBLE) {

         // One of the twelve "play piles" was double clicked
         if (p.x>29 && p.x<531 && p.y>135 && p.y<343) {
            cPile = (int) (p.x - 29) / (CARD_WIDTH + 15);
            if (p.y>240) cPile+=6;
            for (cp=12 ; cp<16 ; cp++) {
               if (piles_count[cPile] && (cards_get_value(piles[cPile][piles_count[cPile]-1]) == cards_get_value(piles[cp][piles_count[cp]-1]) + 1) && cards_get_suite(piles[cp][piles_count[cp]-1]) == cards_get_suite(piles[cPile][piles_count[cPile]-1])) {
                  move_card(cPile, cp);
               };               
            };
         };
         wasDoubleClicked = 1;
         deselect_all();
         redraw_vGame(vGame);
         clear_event(event);
      };
   }; 

   if (event->type == EV_MESSAGE) {
      switch (event->message) {
         case MSG_DEAL_CARDS: {
            deal_cards();
            redraw_vGame(vGame);
            clear_event(event);
         }; break;
         case MSG_CLOSE: {
            cards_uninit_deck();
         }; break;
         case MSG_CRUEL_SELGAME: {
            cards_init_deck(deck);
            selGame = cards_game_select(curGame);
            if (selGame) {
               curGame = selGame;
               cards_shuffle_deck_select(deck, curGame);
               init_game();
               redraw_win_title();
            };
            redraw_vGame(vGame);
            clear_event(event);
         }; break;
         case MSG_CRUEL_NEW: {
            cards_init_deck(deck);
            curGame = cards_shuffle_deck_ex(deck);
            init_game();
            redraw_vGame(vGame);
            clear_event(event);
         }; break;
         case MSG_CRUEL_HELP: {
            open_help("./help/cruel.hlp");
            clear_event(event);
         }; break;
         case MSG_ABOUT: {
            msgbox(MW_INFO, MB_OK, "\"Cruel\" cardgame for Seal 2\n\n(c) 2002 Tobias Johansson\n\nEmail:   tobbe_snodd@hotmail.com\nWeb:   http://hem.passagen.se/sealfiles\n");
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
                    new_menu_item("Random game", "", 0, MSG_CRUEL_NEW, NULL,
                    new_menu_item("Select game", "", 0, MSG_CRUEL_SELGAME, NULL,
                    new_menu_line(
                    new_menu_item("Exit", "ALT+F4", 0, MSG_CLOSE, NULL, 
                    NULL))))),
                 new_sub_menu("Help", new_menu(
                    new_menu_item("Help", "", 0, MSG_CRUEL_HELP, NULL,
                    new_menu_line(
                    new_menu_item("About Cruel", "", 0, MSG_ABOUT, NULL, 
                    new_menu_item("About \"cards.xdl\"", "", 0, MSG_CARDS_ABOUT, NULL, 
                    NULL))))), 
                 NULL))
              );
   return menu;
};



void create_window(void) {
  t_rect r;
  winMain = appwin_init(_malloc(sizeof(t_appwin)), 
                        rect_assign(0, 0, 570, 415), 
                        "Cruel", 
                        WF_MINIMIZE|WF_ABOUT|WF_SYSMENU, 
                        ap_id, &trans_event);
  if (winMain) VIEW(winMain)->align |= TX_ALIGN_CENTER;
  winMain->icon16 = load_image("bmp/cruel.ico,16");
  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(winMain));

  r = VIEW(winMain)->size_limits(VIEW(winMain));
  gameMenu = hormenu_init(_malloc(sizeof(t_menuview)), 
                          rect_assign(r.a.x, r.a.y + 1, r.a.x, r.a.y + 20), 
                          create_game_menu());
  OBJECT(winMain)->insert(OBJECT(winMain), OBJECT(gameMenu));

  vGame = view_init(_malloc(sizeof(t_view)),
                    rect_assign(3, 45, 565, 410));
  VIEW(vGame)->draw = &redraw_vGame;
  OBJECT(winMain)->insert(OBJECT(winMain), OBJECT(vGame));

};



app_begin(void) {
   if (ap_process == AP_INIT) {
      cards_init_deck(deck);
      curGame = cards_shuffle_deck_ex(deck);
      init_game();
      create_window();
      redraw_win_title();
   };
} app_end;
