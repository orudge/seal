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
/*                          cards.h                             */
/*                                                              */
/*          Used for creating card games for Seal 2             */
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


#define  CARDS_VERSION	"1.1"

#define  C_BACK0   52
#define  C_BACK1   53
#define  C_BACK2   54
#define  C_BACK3   55
#define  C_BACK4   56
#define  C_BACK5   57
#define  C_BACK6   58
#define  C_BACK7   59
#define  C_BACK8   60
#define  C_BACK9   61

#define  C_CIRCLE  62
#define  C_CROSS   63
#define  C_EMPTY   64


#define  GFXFILE  "./cards.dat"
#define  CARD_WIDTH   71
#define  CARD_HEIGHT  96
#define  CARD_LOFFSET 16   // Used when overlapping cards with face up.
#define  CARD_SOFFSET 8    // Used when overlapping back of cards.

#define  FACE_UP   1
#define  FACE_DOWN 0

#define  CLUBS    0
#define  DIAMONDS 1
#define  HEARTS   2
#define  SPADES   3

#define  ACE      1
#define  KING     13
#define  QUEEN    12
#define  JACK     11


typedef struct t_card {
   // This structure is used for the elements,
   // (cards) in the deck[] structure and
   // contains all the properties of a card.

   l_int   card;       // The card (1-52).

   l_bool  showFace;   // True if the face of the card is visible
                       // to the player. Otherwise the back of the
                       // card is shown.

   l_bool  selected;   // True if the card is currently selected.

} t_card;



l_text cards_get_version(void);
   // Added in v1.1
   // Returns a string containing the version of
   // the currently installed copy of "cards.xdl".


l_int cards_init_deck(t_card deck[]);
   // Fills up the array with the cards (1-52)
   // and loads the card graphics.


l_int cards_uninit_deck(void);
   // Unloads the graphics.


void cards_shuffle_deck(t_card deck[]);
   // Shuffles the deck array. This is done by looping
   // through all the integers and exchanging every 
   // number with another random number in the array.


l_int cards_shuffle_deck_ex(t_card deck[]);
   // Added in v1.1
   // Same as "cards_shuffle_deck" but also returns
   // the number of the game (the seed).


void cards_shuffle_deck_select(t_card deck[], l_int selGame);
   // Added in v1.1
   // Same as "cards_shuffle_deck_ex" but 
   // uses selGame as seed when randomizing.


l_int cards_game_select(l_int curGame);
   // Added in v1.1
   // Displays an inputbox letting the user
   // to select the seed (select a game).


l_int cards_get_color(t_card card);
   // Gets the color of "card". Returns one of Seals'
   // defined colors; CO_BLACK or CO_RED.


l_int cards_get_suite(t_card card);
   // Gets the suite of "card". Return values:
   // CLUBS, DIAMONDS, HEARTS or SPADES.


l_int cards_get_value(t_card card);
   // Gets the value of "card". Returns a number between
   // one and fourteen where 1 is the Ace and 13 is the king.


l_int cards_blit_card(BITMAP *out, l_int x, l_int y, t_card card);
   // Draws card number "card" to the bitmap "out" at 
   // the coordinates (x, y) relative to the view.


l_int cards_blit_symbol(BITMAP *out, l_int x, l_int y, l_int symbol);
   // Draws bitmap number "symbol" to the bitmap "out" at the 
   // coordinates (x, y). "Symbol" could be one of the defined
   // C_CROSS, C_CIRC, C_EMPTY or any of the card-backs.


l_int cards_blit_back(BITMAP *out, l_int x, l_int y);
   // Draws the current card-back to the bitmap "out" at (x, y).


void cards_select_deck(void);
   // Shows the dialog for selecting which deck graphics to use 
   // when drawing the back of a card.


void cards_about(void);
   // Shows the about box with copyright information etc.

