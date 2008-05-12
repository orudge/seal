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

/****************************************************************/
/*                                                              */
/*                          cards.c                             */
/*                                                              */
/*             For creating card games for Seal 2               */
/*                                                              */
/*                  Copyright (c) 2001,2002                     */
/*                      Tobias Johansson                        */
/*                    All Rights Reserved                       */
/*                                                              */
/* mail:  tobbe_snodd@hotmail.com                               */
/* web:   http://www.seal.private.as                            */
/*                                                              */
/* Cards is free software; you can redistribute it and/or       */
/* modify it under the terms of the GNU General Public License  */
/* as published by the Free Software Foundation; either version */
/* 2, or (at your option) any later version.                    */
/*                                                              */
/* Cards is distributed in the hope that it will be useful, but */
/* WITHOUT ANY WARRANTY; without even the implied warranty of   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See    */
/* the GNU General Public License for more details.             */
/*                                                              */
/* You should have received a copy of the GNU General Public    */
/* License along with cards; see the file COPYING.  If not,     */
/* write to the Free Software Foundation, 675 Mass Ave,         */
/* Cambridge, MA 02139, USA.                                    */
/*                                                              */
/****************************************************************/
/* Revision History (as of 02/04/2002):
 *
 * 02/04/2002: Appended CARDS.DAT to XDL instead of having seperate file (orudge)
 * 04/04/2002: Fixed visual glitch with skins in About box (orudge)
 */

#define  CARDS_INT_VERSION  "1.1"


#include "allegro.h"
#include "seal.h"
#include "app.h"
#include "button.h"
#include "view.h"
#include "registry.h"
#include "dos.h"
#include "cards.h"

p_view    vDecks;
l_int     selDeck = C_BACK0;

t_rect   selDeckCoords[10] = {{7,   22,  12+CARD_WIDTH,  27+CARD_HEIGHT},
                              {87,  22,  92+CARD_WIDTH,  27+CARD_HEIGHT},
                              {167, 22,  172+CARD_WIDTH, 27+CARD_HEIGHT},
                              {247, 22,  252+CARD_WIDTH, 27+CARD_HEIGHT},
                              {327, 22,  332+CARD_WIDTH, 27+CARD_HEIGHT},

                              {7,   127, 12+CARD_WIDTH,  132+CARD_HEIGHT},
                              {87,  127, 92+CARD_WIDTH,  132+CARD_HEIGHT},
                              {167, 127, 172+CARD_WIDTH, 132+CARD_HEIGHT},
                              {247, 127, 252+CARD_WIDTH, 132+CARD_HEIGHT},
                              {327, 127, 332+CARD_WIDTH, 132+CARD_HEIGHT}};

static DATAFILE  *datFile = NULL;
static BITMAP    *invBuf  = NULL;

SetInfoAppName("Card drawing library v1.1");
SetInfoDesciption("Card drawing library for Seal 2");
SetInfoCopyright("(c) 2001-2002 Tobias Johansson");
SetInfoManufacturer("Tobias Johansson");



l_int get_selDeck_from_reg(void) {
  if (!key_exists("software/cards")) create_key("software", "cards");
  if (key_exists(key_in_path("software/cards", "deck"))) {
    selDeck = get_key_integer(key_in_path("software/cards", "deck"));
  } else {
    create_key("software/cards", "deck");
    set_key_integer(key_in_path("software/cards", "deck"), C_BACK0);
  };   
};



static l_int load_datFile(void) {

   // Loads the card graphics in dat-file GFXFILE.

   if (!datFile) {
      datFile = conv_to_skipcolor_data(DLXGetFileData(ap_id), CO_SKIP_RED, CO_SKIP_GREEN, CO_SKIP_BLUE);
//      datFile = load_datafile(GFXFILE);
      invBuf = create_bitmap(CARD_WIDTH, CARD_HEIGHT);
      return 1;
   };
   return 0;
};



l_text cards_get_version(void) {
   return CARDS_INT_VERSION;
};



l_int cards_init_deck(t_card deck[]) {
   l_int c;

   // Load the card graphics if not already loaded.

   load_datFile();

   // Fills up the deck array with the cards (1-52)

   for (c=1 ; c<53 ; c++) {
      deck[c].card = c;
      deck[c].showFace = 0;
      deck[c].selected = 0;
   };

   return 1;
};



l_int cards_uninit_deck(void) {

   //Unloads card graphics if they aren't already

   if (datFile) {
//     unload_datafile(datFile);
     datFile = NULL;
     if (!datFile) return 1;
   };
   return 0;
};



void cards_shuffle_deck(t_card deck[]) {

   // Loop through the array and exchange every
   // card with another random card in the deck.

   l_int   c, xcrd;
   t_card  tcrd;
   srand(time(0));   // Randomize using current time as seed.

   for (c=1 ; c<53 ; c++) {
      xcrd = (rand() % 51) + 1;
      tcrd = deck[c];
      deck[c] = deck[xcrd];
      deck[xcrd] = tcrd;
   };
};



l_int cards_shuffle_deck_ex(t_card deck[]) {

   // Same as "cards_shuffle_deck" but returns
   // the number of the game (the seed).

   l_int   c, xcrd, selGame;
   t_card  tcrd;

   srand(time(0));            // Randomize using current time as seed
   selGame = rand() % 32000;  // Select a game between 0 and 32000
   srand(selGame);            // Randomize using the selected game

   for (c=1 ; c<53 ; c++) {
      xcrd = (rand() % 51) + 1;
      tcrd = deck[c];
      deck[c] = deck[xcrd];
      deck[xcrd] = tcrd;
   };

   return selGame;
};



void cards_shuffle_deck_select(t_card deck[], l_int selGame) {

   // Loop through the array and exchange every
   // card with another random card in the deck.
   // Uses selGame as seed when randomizing.

   l_int   c, xcrd;
   t_card  tcrd;
   srand(selGame);            // Randomize using the selected game

   for (c=1 ; c<53 ; c++) {
      xcrd = (rand() % 51) + 1;
      tcrd = deck[c];
      deck[c] = deck[xcrd];
      deck[xcrd] = tcrd;
   };
};



l_int cards_game_select(l_int curGame) {

   // Lets the user select the seed.

   l_int   selGame = -1;
   l_text  retVal;

   retVal = (l_text) inputbox("Select a game", "Enter the number of the game to play (1-32000):", set_format_text(NULL,"%d", curGame));
   if (retVal==NULL) return NULL;  // Cancel was clicked.

   selGame = atoi(retVal);
   if (selGame<1 || selGame>32000) {

      // Not a valid game (1-32000)

      msgbox(MW_INFO, MB_OK, "Not a valid game!\n\nYour selection must be between 1 and 32000.");
      return NULL;
   };
//   msgbox(MW_INFO, MB_OK, "%d", selGame);

   return selGame;
};



l_int cards_get_suite(t_card card) {
   if (card.card) return (int) (card.card - 1) / 13;
   else return 0;
};



l_int cards_get_color(t_card card) {
   l_int suite = cards_get_suite(card);
   if (suite == CLUBS || suite == SPADES) return CO_BLACK;
   else return CO_RED;
};



l_int cards_get_value(t_card card) {
   if (card.card) return ((card.card - 1) % 13) + 1;
   else return 0;
};



l_int cards_blit_card(BITMAP *out, l_int x, l_int y, t_card card) {

   l_int ix, iy, c;

   // Load the card graphics if not already loaded.

   load_datFile();

   // Draws card number "card" (1-52) to the 
   // bitmap "out" at the coordinates (x, y).

   if (card.card) {
     if (card.showFace) {

        // Draw the cards face showing its' value.

        if (card.selected) {

           // Draw an inverted card if selected. Draw the card
           // on bitmap invBuf and loop through all the pixels
           // inverting their colors.

           draw_sprite(invBuf, datFile[card.card-1].dat, 0, 0);
           for (iy=0 ; iy<CARD_HEIGHT ; iy++) {
              for (ix=0 ; ix<CARD_WIDTH ; ix++) {
                 c = getpixel(invBuf, ix, iy);
                 if (c != out->vtable->mask_color) 
                 putpixel(invBuf, ix, iy, makecol(255-getr(c), 
                                                  255-getg(c), 
                                                  255-getb(c)));
              };
           };

           // Blit the inverted card invBuf to "out". 

           draw_sprite(out, invBuf, x, y);
        } else {

           // Draw the unmodified card if not selected.

           draw_sprite(out, datFile[card.card-1].dat, x, y);
        };
     } else {

        // Just draw the back of the card.

        draw_sprite(out, datFile[selDeck].dat, x, y);
     };
   };
   return 1;
};



l_int cards_blit_symbol(BITMAP *out, l_int x, l_int y, l_int symbol) {

   // Draws bitmap number "symbol" to the bitmap "out" at the 
   // coordinates (x, y). "symbol" should be one of the defined
   // C_CROSS, C_CIRC or C_EMPTY. Use "cards_blit_back" instead 
   // for drawing the back of a card.

   // Load the card graphics if not already loaded.
   load_datFile();

   draw_sprite(out, datFile[symbol].dat, x, y);
   return 1;
};



l_int cards_blit_back(BITMAP *out, l_int x, l_int y) {

   // Draws the back of a card to "out" at coordinate (x,y).


   // Load the card graphics if not already loaded.
   load_datFile();   

   draw_sprite(out, datFile[selDeck].dat, x, y);
   return 1;
};



static void redraw_vDecks(p_view v) {
   t_point p;
   t_rect  r = v->size_limits(v); 
   BITMAP  *out = v->begin_paint(v, &p, r);
   if (out) {
      rectfill(out, p.x+5, p.y+20, p.x+410, p.y+235, color_3d_face);

      rectfill(out, p.x+selDeckCoords[selDeck-52].a.x, 
                    p.y+selDeckCoords[selDeck-52].a.y,
                    p.x+selDeckCoords[selDeck-52].b.x,
                    p.y+selDeckCoords[selDeck-52].b.y,
                    color_3d_text);

      draw_sprite(out, datFile[C_BACK0].dat, p.x+10,  p.y+25);
      draw_sprite(out, datFile[C_BACK1].dat, p.x+90,  p.y+25);
      draw_sprite(out, datFile[C_BACK2].dat, p.x+170, p.y+25);
      draw_sprite(out, datFile[C_BACK3].dat, p.x+250, p.y+25);
      draw_sprite(out, datFile[C_BACK4].dat, p.x+330, p.y+25);

      draw_sprite(out, datFile[C_BACK5].dat, p.x+10,  p.y+130);
      draw_sprite(out, datFile[C_BACK6].dat, p.x+90,  p.y+130);
      draw_sprite(out, datFile[C_BACK7].dat, p.x+170, p.y+130);
      draw_sprite(out, datFile[C_BACK8].dat, p.x+250, p.y+130);
      draw_sprite(out, datFile[C_BACK9].dat, p.x+330, p.y+130);
   };
   v->end_of_paint(v, r);
};



static void winSelect_trans_event(p_object o, p_event event) {
   t_point p;
   RETVIEW(o, event);
   if (event->type & EV_MOUSE) {
      if (OBJECT(mouse)->state & MO_SF_MOUSELUP) {
         p = VIEW(o)->get_local_point(VIEW(o), mouse->where);
         selDeck = (int) (p.x - 10) / (CARD_WIDTH + 10) + 52;
         if (p.y > +127) selDeck += 5;
         set_key_integer(key_in_path("software/cards", "deck"), selDeck);
         redraw_vDecks(vDecks);
      };
   };
};



void cards_select_deck(void) {
   p_appwin  winSelect;
   p_button  btnClose;
   l_dword   msg;

   // Load the card graphics if not already loaded.
   load_datFile();

   winSelect = appwin_init(_malloc(sizeof(t_appwin)),
                           rect_assign(0, 0, 410, 275),
                           "Select deck", 
                           NULL, ap_id,
                           &winSelect_trans_event);
   if (winSelect) VIEW(winSelect)->align |= TX_ALIGN_CENTER;
   winSelect->icon16 = load_image("bmp/SelDeck.ico,16");
   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(winSelect));

   btnClose = button_init(_malloc(sizeof(t_button)),
                          rect_assign(316, 240, 401, 265),
                          TXT_OK, MSG_OK, BF_NORMAL);
   OBJECT(winSelect)->insert(OBJECT(winSelect), OBJECT(btnClose));

   vDecks = view_init(_malloc(sizeof(t_view)),
                      rect_assign(0, 0, 410, 235));
   VIEW(vDecks)->draw = &redraw_vDecks;
   OBJECT(winSelect)->insert(OBJECT(winSelect), OBJECT(vDecks));

   msg = desktop->execute_view(desktop, VIEW(winSelect));
   dispose(OBJECT(winSelect));
};



static void redraw_vInfo(p_view v) {
   l_int x;
   t_point p;
   t_rect  r = v->size_limits(v); 
   BITMAP  *out = v->begin_paint(v, &p, r);
   if (out) {
      if (!use_skins)  // OCR - 04/04/2002
         rectfill(out, p.x, p.y, p.x+310, p.y+115, color_3d_face);

      for (x=10 ; x<22 ; x+=2) {
         draw_sprite(out, datFile[51].dat, p.x+x, p.y+x+15);
      };      
      textout_printf(out, v->font, -1, 
                     p.x+100,  p.y+30, p.x+310,  p.y+115, 
                     TX_ALIGN_LEFT, color_3d_text, TX_NOCOLOR, 0, 
                     "CARDS.XDL  v%s\nA free card drawing library for Seal2.\n\nCopyright (c) 2002 Tobias Johansson. \nAll rights reserved.\n\nEmail:   tobbe_snodd@hotmail.com\nWeb:   hem.passagen.se/sealfiles", CARDS_INT_VERSION);
   };
   v->end_of_paint(v, r);
};



void cards_about(void) {
   p_appwin  winAbout;
   p_button  btnClose;
   p_view    vInfo;
   l_dword   msg = MSG_NOTHING;

   // Load the card graphics if not already loaded.
   load_datFile();

   winAbout = appwin_init(_malloc(sizeof(t_appwin)),
                          rect_assign(0, 0, 310, 175),
                          "About cards.xdl", 
                          NULL, ap_id, NULL);
   if (winAbout) VIEW(winAbout)->align |= TX_ALIGN_CENTER;
   WINDOW(winAbout)->flags -= WF_SYSMENU;
   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(winAbout));

   btnClose = button_init(_malloc(sizeof(t_button)),
                          rect_assign(118, 140, 200, 165),
                          TXT_OK, MSG_OK, BF_NORMAL);
   OBJECT(winAbout)->insert(OBJECT(winAbout), OBJECT(btnClose));

   vInfo = view_init(_malloc(sizeof(t_view)),
                     rect_assign(0, 0, 310, 140));
   VIEW(vInfo)->draw = &redraw_vInfo;
   OBJECT(winAbout)->insert(OBJECT(winAbout), OBJECT(vInfo));

   msg = desktop->execute_view(desktop, VIEW(winAbout));
   dispose(OBJECT(winAbout));
};



//lib_exportable;
#include "cards.exp"


lib_begin (void) {
   if (ap_process == AP_ALLOC) {
      AP_EXPORTLIB(); 
      get_selDeck_from_reg();
   } else
   if (ap_process == AP_INIT) {
      AP_EXPORTLIB(); 
   } else
   if (ap_process == AP_FREE) {
      unload_datafile(datFile);
      destroy_bitmap(invBuf);
   };
} lib_end;