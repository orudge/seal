/****************************************************************/
/*                                                              */
/*                        klondike.c                            */
/*                                                              */
/*             The "Klondike" card game for Seal                */
/*                                                              */
/*                     Copyright (c) 2002                       */
/*                      Tobias Johansson                        */
/*                     All Rights Reserved                      */
/*                                                              */
/* mail:  tobbe_snodd@hotmail.com                               */
/* web:   http://hem.passagen.se/sealfiles                      */
/*                                                              */
/* Klondike is free software; you can redistribute it and/or    */
/* modify it under the terms of the GNU General Public License  */
/* as published by the Free Software Foundation; either version */
/* 2, or (at your option) any later version.                    */
/*                                                              */
/* Klondike is distributed in the hope that it will be useful,  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty  */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  */
/* the GNU General Public License for more details.             */
/*                                                              */
/* You should have received a copy of the GNU General Public    */
/* License along with Klondike; see the file COPYING.  If not,  */
/* write to the Free Software Foundation, 675 Mass Ave,         */
/* Cambridge, MA 02139, USA.                                    */
/*                                                              */
/****************************************************************/


// 3/4/2002  - Better skin support added by Tobias Johansson



#include "allegro.h"
#include "seal.h"
#include "app.h"
#include "view.h"
#include "cards.h"
#include "menus.h"
#include "helpsys.h"

#define MSG_KLOND_NEW     100001
#define MSG_KLOND_DECK    100002
#define MSG_KLOND_HELP    100003
#define MSG_CARDS_ABOUT   100004
#define MSG_TAKE_THREE	  100005
#define MSG_KLOND_SELGAME 100006

#define TEXT_ABOUT      "Klondike 1.1\n\n(c) 2002 Tobias Johansson\n\nEmail:   tobbe_snodd@hotmail.com\nWeb:   hem.passagen.se/sealfiles\n"


p_appwin   winMain;
p_view     vGame;
p_menu     menu;
p_menuview gameMenu;

t_card deck[53];
t_card piles[12][25];    // piles[0]=stock, piles[1-7]=lower piles, 
l_int  pilesCount[12];   // piles[8-11]=suite piles

l_int  drawnCards[4];
l_int  curCard, fromCard, fromPile, toPile; 
l_int  drawThreeCards;
l_int  curGame = 0;



SetInfoAppName      ("Klondike");
SetInfoDesciption   ("Seal version of the very famous patience game.");
SetInfoCopyright    ("(c) Copyright 2002 Tobias Johansson. All rights reserved.");
SetInfoManufacturer ("Tobias Johansson");



void   init_deck        (void);
void   init_game        (void);
void   deselect_all     (void);
void   move_card        (void);
void   draw_from_stock  (void);
void   redraw_vGame     (p_view v);
void   trans_event      (p_object o, p_event event);
p_menu create_game_menu (void);



void redraw_win_title(void) {
   // Redraws the title of the window to show
   // which game is currently selected.
   if (winMain) {
      _free(WINDOW(winMain)->caption);
      WINDOW(winMain)->caption = set_format_text(NULL, "Klondike - game #%d", curGame);

      TEST_SUB_VIEWS(VIEW(winMain), WINDOW(winMain)->draw_title(WINDOW(winMain)));
  };
};



void init_deck(void) {
   // Create a new empty deck
   curCard = 52;
   fromPile = fromCard = 0;

   cards_init_deck(deck);
   curGame = cards_shuffle_deck_ex(deck);
   redraw_win_title();
   deselect_all();
};



l_int init_deck_sel_game(void) {
   // Create a new empty deck
   l_int selGame;

   selGame = cards_game_select(curGame);
   if (selGame) {
      curCard = 52;
      fromPile = fromCard = 0;
      curGame = selGame;
      cards_init_deck(deck);
      cards_shuffle_deck_select(deck, curGame);
      redraw_win_title();

      return 1;
   };
   return 0;
};



void init_game(void) {
   l_int tc, tp;

   // Fill up the seven lower piles.
   for (tp=1 ; tp<=7 ;tp++) {
      for (tc=0 ; tc<=tp-1 ; tc++) {
         piles[tp][tc].showFace = false;
         piles[tp][tc].card = deck[curCard].card;
         curCard--;
      };
      piles[tp][tp-1].showFace = true;
      pilesCount[tp] = tp;
   };

   // Fill up the stock (the hand) with 
   // the rest of the cards in the deck.
   for (tc=curCard ; tc>0 ; tc--) {
      piles[0][tc].card     = deck[tc].card;
      piles[0][tc].showFace = true;
   };

   // Empty the four suite piles.
   for (tp=8 ; tp<=11 ;tp++) pilesCount[tp] = 0;

   // Remove old cards drawn from stock.
   for (tp=0 ; tp<=2 ;tp++) drawnCards[tp] = 0;

   pilesCount[0] = curCard;
};



l_int check_if_won(void) {

   // The game is won if the number of cards 
   // in the four upper piles equals 52.
   l_int p, c = 0;
   for (p=8 ; p<12 ; p++) c += pilesCount[p];
   if (c == 52) {
      return 1;
   } else {
      return 0;
   };
};



void deselect_all(void) {

   // Deselect all cards in all piles.
   l_int tp, tc;
   for (tp=0 ; tp<=11 ;tp++) {
      for (tc=0 ; tc<=pilesCount[tp] ; tc++) {
         piles[tp][tc].selected = false;
      };
   };
   fromCard = 0;
   fromPile = 0;
   toPile   = 0;
};



void move_card(void) {
   // Moves fromCard from fromPile to toPile.
   l_int c;

   // If moved from the deck or one of the suite piles.
   if (fromPile == 0 || fromPile >= 8) {
      pilesCount[toPile]++;
      piles[toPile][(pilesCount[toPile])-1] = piles[fromPile][fromCard-1];
      pilesCount[fromPile]--;
      for (c=fromCard-1 ; c<=pilesCount[fromPile] ; c++) 
         piles[fromPile][c] = piles[fromPile][c+1];

      // Remove the top one of the three turned cards.
      if (fromPile == 0) {
         for (c=0 ; c<3 ; c++) drawnCards[c] = drawnCards[c+1];
         if (drawnCards[0] == 0) {
            if (curCard < pilesCount[0]) {
               drawnCards[0] = curCard + 1;
            };
         };
      };
   } else {
      // Move the selected card (and the ones on top of it if any)
      // from one of the seven lower piles to another one.
      for (c=fromCard-1 ; c<pilesCount[fromPile] ; c++) {
         piles[toPile][(pilesCount[toPile])] = piles[fromPile][c];
         pilesCount[toPile]++;
      };
      pilesCount[fromPile] = fromCard-1;
   }; 
   deselect_all();
};



void draw_from_stock(void) {
   l_int c;

   // Go back to the topmost card if the bottom 
   // card of the stock is reached.
   if (curCard < 0) curCard = pilesCount[0];   

   // Remove old cards.
   for (c=0 ; c<=2 ; c++) drawnCards[c] = 0;

   // Get three new or one new card from the stock.
   if (drawThreeCards) {
      for (c=0 ; c<3 ; c++) {
         drawnCards[c] = curCard;
         curCard--;
         // Exit loop if reached bottom of pile.
         if (curCard < 1) break;
      };
   } else {
      drawnCards[0] = curCard;
      drawnCards[1] = 0;
      drawnCards[2] = 0;
      curCard--;
   };      
};



l_int get_clicked_card(l_int cPile, l_int curY, l_int startY) {

   // Returns the card that was clicked in one of the seven piles.
   // cPile is pile to look in, curY is current vert. position of
   // mouse pointer and startY is the Y-coord. to start looking at
   // (the top edge of the topmost card on the screen).

   // Loop through the cards in the pile and add the height of the
   // current card (CARD_LOFFSET or CARD_SOFFSET depending on if the
   // card is turned or not) to startY. If curY is between startY 
   // and startY + the cards' height then return the number of the 
   // current card.

   l_int c;
   for (c=0 ; c<pilesCount[cPile] ; c++) {
      if (piles[cPile][c].showFace) {
         if (curY >= startY && curY < (startY + CARD_LOFFSET)) {
            return c;
         };
         startY += CARD_LOFFSET;
      } else {
         if (curY >= startY && curY < (startY + CARD_SOFFSET)) {
            return 0;
         };
         startY += CARD_SOFFSET;
      };
   };
   if (curY >= startY && curY <= (startY + CARD_HEIGHT))
      return pilesCount[cPile];
   else 
      return 0;
};



void redraw_vGame(p_view v) {
   t_point p;
   l_int   cCard, cPile, curY;
   t_rect  r = v->size_limits(v); 
   BITMAP  *out = v->begin_paint(v, &p, r);
   if (out) {
      // Draw green background.
      rectfill(out, p.x, p.y, p.x+600, p.y+400, COLOR(CO_GREEN));

      // Draw the seven lower piles.
      for (cPile=1 ; cPile<8 ; cPile++) {
         curY = 120;
         for (cCard=0 ; cCard<pilesCount[cPile] ; cCard++) {
            if (cCard-1 >= 0) {
               if (piles[cPile][cCard-1].showFace) 
                  curY += CARD_LOFFSET;
               else
                  curY += CARD_SOFFSET;
            };
            cards_blit_card(out, p.x+15+((cPile-1)*80), p.y+curY, piles[cPile][cCard]);
         };
      };

      // Draw the four upper suite piles.
      for (cPile=8 ; cPile<12 ; cPile++) {
         if (!pilesCount[cPile]) {
            cards_blit_symbol(out, p.x+255+((cPile-8)*80), p.y+12, C_EMPTY);
         } else {
            cards_blit_card(out, p.x+255+((cPile-8)*80), p.y+12, piles[cPile][(pilesCount[cPile]-1)]);
         };
      };

      // Draw back of the stock (the hand).
      if (pilesCount[0] && curCard) {
         cards_blit_back(out, p.x+15, p.y+12);
      } else {
         cards_blit_symbol(out, p.x+15, p.y+12, C_CIRCLE);
      };

      // Draw the current card(s) drawn from the stock.
      if (curCard <= pilesCount[0]) {
         for (cCard=0 ; cCard<3 ; cCard++) {
           cards_blit_card(out, p.x+95+(cCard*13), p.y+12,  piles[0][(drawnCards[cCard])]); 
         };
      };
   };
   v->end_of_paint(v, r);
};



void trans_event(p_object o, p_event event) {
   l_int c, cc, cPile, i;
   t_point p;
   RETVIEW(o, event);
   if (event->type & EV_MOUSE) {

      // FOR DEBUGGING: Show value of selected card with right-click
      // if (OBJECT(mouse)->state & MO_SF_MOUSERUP) {
      //    msgbox(MW_INFO, MB_OK, "Card value:  %d", cards_get_value(piles[fromPile][fromCard-1]));
      // };

      if (OBJECT(mouse)->state & MO_SF_MOUSELUP) {
         p = VIEW(o)->get_local_point(VIEW(o), mouse->where);

         if (p.x > +15 && p.x < +91 && p.y > +55 && p.y < +151) {

            // The stock is clicked. Draw 1 or 3 new cards. 
            deselect_all();
            draw_from_stock();
         } else

         if (p.x > +95 && p.x < +200 && p.y > +55 && p.y < +151) {

            // The topmost of the three drawn cards or the 
            // single drawn card is clicked. 
            for (c=2 ; c>=0 ; c--) {
               if (drawnCards[c]) {
                  if (drawnCards[c] == fromCard - 1) {
                     deselect_all();
                     break;
                  } else {
                     piles[0][(drawnCards[c])].selected = true;
                     fromCard = drawnCards[c] + 1;
                     fromPile = 0;
                     break;
                  };
               };
            };
         } else 


         if (p.x > +255 && p.x < +570 && p.y > +55 && p.y < +151) {

            // One of the four suite piles is clicked. 
            cPile = (int) (p.x - 255) / (CARD_WIDTH + 10);   // Which pile?
            cPile += 8;
            c = pilesCount[cPile];
           
            // Move to pile if selected is an ace and pile is empty.
            if (cards_get_value(piles[fromPile][fromCard - 1]) == 1 && !piles[cPile][c-1].card) {
               if (!fromPile || fromCard == pilesCount[fromPile]) {
                  toPile = cPile;
                  move_card();
                  deselect_all();
               };
            } else

            // Move the selected card to the clicked pile if possible.
            if (piles[fromPile][fromCard - 1].card == piles[cPile][c-1].card + 1) {
               if (!fromPile || fromCard == pilesCount[fromPile]) {
                  toPile = cPile;
                  move_card();
                  deselect_all();
               };
            } else

            // Deselect all if topmost card in pile is already selected.
            if (piles[cPile][c-1].selected) {
               deselect_all();
            } else

            // If nothing else is selected, select the topmost card 
            // in the pile if it isn't empty or ace.
            if (c && cards_get_value(piles[cPile][c-1]) > 1) {
               if (!fromCard) {
                  fromCard = c;
                  fromPile = cPile;
                  piles[fromPile][fromCard - 1].selected = true;
               };
            };

         } else


         if (p.x > +15 && p.x < +570 && p.y > +160) {

            // One of the seven lower piles is clicked.
            cPile = (int) (p.x - 15) / (CARD_WIDTH + 10);   // Which pile?
            cPile ++;
            c = pilesCount[cPile];

            // Move card to empty pile if it is a King.
            if (cards_get_value(piles[fromPile][fromCard-1]) == 13 && c<1) {
               toPile = cPile;
               move_card();
               deselect_all();
            } else

            // Turn card if not already turned.
            if (!piles[cPile][c - 1].showFace) {
               piles[cPile][c - 1].showFace = true;
               deselect_all();
            } else 

            // Move card to pile if possible.
            if (fromCard && piles[cPile][c-1].showFace && (cards_get_value(piles[fromPile][fromCard-1]) == cards_get_value(piles[cPile][c-1]) - 1) && (cards_get_color(piles[fromPile][fromCard-1]) != cards_get_color(piles[cPile][c-1]))) {
               toPile = cPile;
               move_card();
               deselect_all();
            } else 

            // Select card if nothing else is selected.
            if (!fromCard && c && piles[cPile][c - 1].showFace) {
               c = get_clicked_card(cPile, p.y, 150);
               if  (c) {
                  fromCard = c;
                  fromPile = cPile;
                  for (cc=fromCard-1 ; cc<=pilesCount[fromPile] ; cc++)
                     piles[fromPile][cc].selected = true;
               };
            } else

            // Deselect if already selected.
            if (piles[cPile][c - 1].selected) {
               deselect_all();
            } else {

               deselect_all();
            };

         } else {
            // Clicked somwhere else, not on a pile.
            deselect_all();
         };

         redraw_vGame(vGame);
         clear_event(event);

         // Game over?
         if (check_if_won()) {
            msgbox(MW_INFO, MB_OK, "Well done! You did it!\n");
         };         
      };
   }; 

   if (event->type == EV_MESSAGE) {
      switch (event->message) {

         case MSG_CLOSE: {
         }; break;

         case MSG_KLOND_NEW: {
            // New game
            init_deck();
            init_game();
            deselect_all();
            redraw_vGame(vGame);
            clear_event(event);
         }; break;

         case MSG_KLOND_SELGAME: {
            // Select game
            if (init_deck_sel_game()) {
               init_game();
               deselect_all();
            };
            redraw_vGame(vGame);
            clear_event(event);
         }; break;

         case MSG_KLOND_DECK: {
            // Select deck
            cards_select_deck();
            redraw_vGame(vGame);
            clear_event(event);
         }; break;

         case MSG_TAKE_THREE: {
            // "Take three cards" cards option
            i = menu_get_item_flags(menu, MSG_TAKE_THREE);
            if (i & MIF_CHECKOK) {  
               drawThreeCards = 1;
            } else {
               drawThreeCards = 0;
            };
            clear_event(event);
         }; break;

         case MSG_KLOND_HELP: {
            // Display help file
            open_help("./help/klondike.hlp");
            clear_event(event);
         }; break;

         case MSG_ABOUT: {
            // Show "About klondike" message
            msgbox(MW_INFO, MB_OK, TEXT_ABOUT);
            clear_event(event);
         }; break;

         case MSG_CARDS_ABOUT: {
            // Show "About cards" message
            cards_about();
            clear_event(event);
         }; break;
      };
   };
};



p_menu create_game_menu(void) {
   menu = new_menu(
                 new_sub_menu("Game", new_menu(
                    new_menu_item("New game", "", 0, MSG_KLOND_NEW, NULL,
                    new_menu_item("Select game", "", 0, MSG_KLOND_SELGAME, NULL,
                    new_menu_line(
                    new_menu_item("Exit", "ALT+F4", 0, MSG_CLOSE, NULL, 
                    NULL))))),
                 new_sub_menu("Options", new_menu(
                    new_menu_check_item("Take three cards", "", drawThreeCards, 0, MSG_TAKE_THREE, NULL, 
                    new_menu_line(
                    new_menu_item("Select deck", "", 0, MSG_KLOND_DECK, NULL,
                    NULL)))), 
                 new_sub_menu("Help", new_menu(
                    new_menu_item("Help", "", 0, MSG_KLOND_HELP, NULL,
                    new_menu_line(
                    new_menu_item("About Klondike", "", 0, MSG_ABOUT, NULL, 
                    new_menu_item("About cards.dlx", "", 0, MSG_CARDS_ABOUT, NULL, 
                    NULL))))), 
                 NULL)))
              );
   return menu;
};



void create_window(void) {
   t_rect r, s;
   r = rect_assign(10, 10, 595, 445);
   winMain = appwin_init(_malloc(sizeof(t_appwin)), 
                         r, 
                         "Klondike", 
                         WF_MINIMIZE|WF_ABOUT, 
                         ap_id, &trans_event);
   winMain->icon16 = load_image("bmp/klondike.ico,16");
   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(winMain));

   s = VIEW(winMain)->size_limits(VIEW(winMain));
   r = rect_assign(r.a.x, r.a.y, r.b.x + 5, r.a.y + s.a.y + 430);
   VIEW(winMain)->change_bounds(VIEW(winMain), r);
   if (winMain) VIEW(winMain)->align |= TX_ALIGN_CENTER;
   r = s;

   r = rect_assign(r.a.x, r.a.y, r.a.x, r.a.y + 20);
   gameMenu = hormenu_init(_malloc(sizeof(t_menuview)), r, create_game_menu());
   OBJECT(winMain)->insert(OBJECT(winMain), OBJECT(gameMenu));

   r = rect_assign(r.a.x + 4, r.b.y + 6, r.a.x + 582, r.b.y + 404);  // 3, 45, 580, 430
   vGame = view_init(_malloc(sizeof(t_view)), r);
   vGame->draw_under_view(vGame);
   VIEW(vGame)->draw = &redraw_vGame;
   OBJECT(vGame)->translate_event = &trans_event;
   OBJECT(vGame)->set_options(OBJECT(vGame), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
   OBJECT(winMain)->insert(OBJECT(winMain), OBJECT(vGame));

};



app_begin(void) {
   if (ap_process == AP_INIT) {

      // Read settings from registry
      if (!key_exists("software/klondike")) create_key("software", "klondike");
      if (!key_exists("software/klondike/lastgame")) {
         create_key("software/klondike", "lastgame");
         init_deck();
      } else {
         curGame = get_key_integer("software/klondike/lastgame");
      };
      if (!key_exists("software/klondike/takethree")) {
         create_key("software/klondike", "takethree");
         drawThreeCards = 1;
      } else {
         drawThreeCards = get_key_integer("software/klondike/takethree");
      };

      // Start new game using settings
      curCard = 52;
      fromPile = fromCard = 0;
      cards_init_deck(deck);
      cards_shuffle_deck_select(deck, curGame);
      redraw_win_title();

      init_game();
      create_window();
      redraw_win_title();
   } else

   if (ap_process == AP_DONE) {
      // Store settings in registry when exiting
      set_key_integer("software/klondike/lastgame", curGame);
      set_key_integer("software/klondike/takethree", drawThreeCards);
      cards_uninit_deck();
   };
} app_end;
