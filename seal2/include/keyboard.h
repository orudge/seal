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

#include"object.h"

#ifndef __KEYBOARD_H_INCLUDED__
#define __KEYBOARD_H_INCLUDED__

#define INI_KEYBOARD     "keyboard"

/*#ifndef NEW_ALLEGRO
#define KB_ESC               1
#define KB_1                 2 
#define KB_2                 3 
#define KB_3                 4
#define KB_4                 5
#define KB_5                 6
#define KB_6                 7
#define KB_7                 8
#define KB_8                 9
#define KB_9                 10
#define KB_0                 11
#define KB_MINUS             12
#define KB_EQUALS            13
#define KB_BACKSPACE         14
#define KB_TAB               15 
#define KB_Q                 16
#define KB_W                 17
#define KB_E                 18
#define KB_R                 19
#define KB_T                 20
#define KB_Y                 21
#define KB_U                 22
#define KB_I                 23
#define KB_O                 24
#define KB_P                 25
#define KB_OPENBRACE         26
#define KB_CLOSEBRACE        27
#define KB_ENTER             28
#define KB_CONTROL           29
#define KB_LCONTROL          29
#define KB_A                 30
#define KB_S                 31
#define KB_D                 32
#define KB_F                 33
#define KB_G                 34
#define KB_H                 35
#define KB_J                 36
#define KB_K                 37
#define KB_L                 38
#define KB_COLON             39
#define KB_QUOTE             40
#define KB_TILDE             41
#define KB_LSHIFT            42
#define KB_BACKSLASH         43
#define KB_Z                 44
#define KB_X                 45
#define KB_C                 46
#define KB_V                 47
#define KB_B                 48
#define KB_N                 49
#define KB_M                 50
#define KB_COMMA             51
#define KB_STOP              52
#define KB_SLASH             53
#define KB_RSHIFT            54
#define KB_ASTERISK          55
#define KB_ALT               56
#define KB_SPACE             57
#define KB_CAPSLOCK          58
#define KB_F1                59
#define KB_F2                60
#define KB_F3                61
#define KB_F4                62
#define KB_F5                63
#define KB_F6                64
#define KB_F7                65
#define KB_F8                66
#define KB_F9                67
#define KB_F10               68
#define KB_NUMLOCK           69
#define KB_SCRLOCK           70
#define KB_HOME              71
#define KB_UP                72
#define KB_PGUP              73
#define KB_MINUS_PAD         74
#define KB_LEFT              75
#define KB_5_PAD             76
#define KB_RIGHT             77
#define KB_PLUS_PAD          78
#define KB_END               79
#define KB_DOWN              80
#define KB_PGDN              81
#define KB_INSERT            82
#define KB_DEL               83
#define KB_PRTSCR            84
#define KB_F11               87
#define KB_F12               88
#define KB_LWIN              91
#define KB_RWIN              92
#define KB_MENU              93
#define KB_PAD               100
#define KB_RCONTROL          120
#define KB_ALTGR             121
#define KB_SLASH2            122
#define KB_PAUSE             123

#define SCANCODE_TO_KEY(c)       ( ((c)<<8) + (int)((key_ascii_table[c]>0x00ff)?0:key_ascii_table[c]) )
#define SCANCODE_TO_CAPS(c)      ( ((c)<<8) + (int)((key_capslock_table[c]>0x00ff)?KB_SHIFT_FLAG:key_capslock_table[c]) )
#define SCANCODE_TO_SHIFT(c)     ( ((c)<<8) + (int)((key_shift_table[c]>0x00ff)?KB_SHIFT_FLAG:key_shift_table[c]) )
#define SCANCODE_TO_CONTROL(c)   ( ((c)<<8) + (int)((key_control_table[c]>0x00ff)?KB_CTRL_FLAG :key_control_table[c]) )
#define SCANCODE_TO_ALTGR(c)     ( ((c)<<8) + (int)((key_altgr_table[c]>0x00ff?)'^':key_altgr_table[c]) )
#define SCANCODE_TO_ALT(c)       ( ((c)<<8) + (int)((key_ascii_table[c]>0x00ff)?KB_ALT_FLAG:0) )


#define KEY_TO(c)       ((c)>>8)

#define TO_KEY(c)       SCANCODE_TO_KEY(c)
#define TO_CHAR(c)      ((c) & 0xff)
#define TO_CAPS(c)      SCANCODE_TO_CAPS(c)

#define TO_SHIFT(c)     SCANCODE_TO_SHIFT(c)

#define TO_CTRL(c)      SCANCODE_TO_CONTROL(c)

#define TO_ALTGR(c)     SCANCODE_TO_ALTGR(c)

#define TO_ALT(c)       SCANCODE_TO_ALT(c)

#else*/
#define KB_ESC        KEY_ESC          /* keyboard scan codes  */
#define KB_1          KEY_1
#define KB_2          KEY_2
#define KB_3          KEY_3
#define KB_4          KEY_4
#define KB_5          KEY_5
#define KB_6          KEY_6
#define KB_7          KEY_7
#define KB_8          KEY_8
#define KB_9          KEY_9
#define KB_0          KEY_0
#define KB_MINUS      KEY_MINUS
#define KB_EQUALS     KEY_EQUALS
#define KB_BACKSPACE  KEY_BACKSPACE
#define KB_TAB        KEY_TAB
#define KB_Q          KEY_Q
#define KB_W          KEY_W
#define KB_E          KEY_E
#define KB_R          KEY_R
#define KB_T          KEY_T
#define KB_Y          KEY_Y
#define KB_U          KEY_U
#define KB_I          KEY_I
#define KB_O          KEY_O
#define KB_P          KEY_P
#define KB_OPENBRACE  KEY_OPENBRACE
#define KB_CLOSEBRACE KEY_CLOSEBRACE
#define KB_ENTER      KEY_ENTER
#define KB_CONTROL    KEY_CONTROL
#define KB_LCONTROL   KEY_LCONTROL
#define KB_A          KEY_A
#define KB_S          KEY_S
#define KB_D          KEY_D
#define KB_F          KEY_F
#define KB_G          KEY_G
#define KB_H          KEY_H
#define KB_J          KEY_J
#define KB_K          KEY_K
#define KB_L          KEY_L
#define KB_COLON      KEY_COLON
#define KB_QUOTE      KEY_QUOTE
#define KB_TILDE      KEY_TILDE
#define KB_LSHIFT     KEY_LSHIFT
#define KB_BACKSLASH  KEY_BACKSLASH
#define KB_Z          KEY_Z
#define KB_X          KEY_X
#define KB_C          KEY_C
#define KB_V          KEY_V
#define KB_B          KEY_B
#define KB_N          KEY_N
#define KB_M          KEY_M
#define KB_COMMA      KEY_COMMA
#define KB_STOP       KEY_STOP
#define KB_SLASH      KEY_SLASH
#define KB_RSHIFT     KEY_RSHIFT
#define KB_ASTERISK   KEY_ASTERISK
#define KB_ALT        KEY_ALT
#define KB_SPACE      KEY_SPACE
#define KB_CAPSLOCK   KEY_CAPSLOCK
#define KB_F1         KEY_F1
#define KB_F2         KEY_F2
#define KB_F3         KEY_F3
#define KB_F4         KEY_F4
#define KB_F5         KEY_F5
#define KB_F6         KEY_F6
#define KB_F7         KEY_F7
#define KB_F8         KEY_F8
#define KB_F9         KEY_F9
#define KB_F10        KEY_F10
#define KB_NUMLOCK    KEY_NUMLOCK
#define KB_SCRLOCK    KEY_SCRLOCK
#define KB_HOME       KEY_HOME
#define KB_UP         KEY_UP
#define KB_PGUP       KEY_PGUP
#define KB_MINUS_PAD  KEY_MINUS_PAD
#define KB_LEFT       KEY_LEFT
#define KB_5_PAD      KEY_5_PAD
#define KB_RIGHT      KEY_RIGHT
#define KB_PLUS_PAD   KEY_PLUS_PAD
#define KB_END        KEY_END
#define KB_DOWN       KEY_DOWN
#define KB_PGDN       KEY_PGDN
#define KB_INSERT     KEY_INSERT
#define KB_DEL        KEY_DEL
#define KB_PRTSCR     KEY_PRTSCR
#define KB_F11        KEY_F11
#define KB_F12        KEY_F12
#define KB_LWIN       KEY_LWIN
#define KB_RWIN       KEY_RWIN
#define KB_MENU       KEY_MENU
#define KB_PAD        KEY_PAD
#define KB_RCONTROL   KEY_RCONTROL
#define KB_ALTGR      KEY_ALTGR
#define KB_SLASH2     KEY_SLASH2
#define KB_PAUSE      KEY_PAUSE

#define SCANCODE_TO_KEY(c)       ( ((c)<<8) | (int)((key_ascii_table[c]>0xFF)?0:key_ascii_table[c]) )
#define SCANCODE_TO_CAPS(c)      ( ((c)<<8) | (int)((key_capslock_table[c]>0xFF)?KB_SHIFT_FLAG:key_capslock_table[c]) )
#define SCANCODE_TO_SHIFT(c)     ( ((c)<<8) | (int)((key_shift_table[c]>0xFF)?KB_SHIFT_FLAG:key_shift_table[c]) )
#define SCANCODE_TO_CONTROL(c)   ( ((c)<<8) | (int)((key_control_table[c]>0xFF)?KB_CTRL_FLAG :key_control_table[c]) )
#define SCANCODE_TO_ALTGR(c)     ( ((c)<<8) | (int)((key_altgr_table[c]>0xFF)?'^':key_altgr_table[c]) )
#define SCANCODE_TO_ALT(c)       ( ((c)<<8) | (int)((key_ascii_table[c]>0xFF)?KB_ALT_FLAG:0) )


#define KEY_TO(c)       ((c) >> 8)
#define TO_KEY(c)       SCANCODE_TO_KEY(c)
#define TO_CHAR(c)      ((c) & 0xff)
#define TO_CAPS(c)      SCANCODE_TO_CAPS(c)
#define TO_SHIFT(c)     SCANCODE_TO_SHIFT(c)
#define TO_CTRL(c)      SCANCODE_TO_CONTROL(c)
#define TO_ALTGR(c)     SCANCODE_TO_ALTGR(c)
#define TO_ALT(c)       SCANCODE_TO_ALT(c)
/*#endif*/


#define KEYPRESSED(c)   (KEYB && KEYB->obclass.state & KB_SF_KEYDOWN && KEYB->code>>8==c)
#define CHARPRESSED(c)  (KEYB && KEYB->obclass.state & KB_SF_KEYDOWN && KEYB->keychar==c)
#define KEYCTRL(c)   ( KEYB->code>>8 == c && KEYB->shifts & KB_CTRL_FLAG )
#define KEYSHIFT(c)  ( KEYB->code>>8 == c && KEYB->shifts & KB_SHIFT_FLAG )
#define KEYALT(c)    ( KEYB->code>>8 == c && KEYB->shifts & KB_ALT_FLAG )

/* state settings */

#define  KB_SF_KEYDOWN       0x0000020 /* key was pull down */

typedef struct t_keyboard *p_keyboard;

/*
  object t_keyboard is used for getting information from keyboard.
  This object is placed in t_driver object. Whenever is
  OBJECT(driver)->translate_event... called it calls also translate_event function
  from t_keyboard, t_mouse, ... structures. Whenever was keyboard pressed, the
  t_keyboard.obclass.state or OBJECT(&t_keyboard)->state is set to flag
  KB_SF_KEYDOWN...see above. it also sets t_keyboard.(code) to keycode that occurred,
  t_keyboard.(shifts) to shift state of keyboard. Available shifts are
  included in allegro.h.
*/
typedef struct t_keyboard {

  /* inherite variables / functions from t_object class */
  struct t_object obclass;

  /* the code of last pressed key */
  l_int     code;

  /* define flags, whenever is some of shifts pressed. These shifts are available
     from "allegro.h" :

     KB_SHIFT_FLAG
     KB_CTRL_FLAG
     KB_ALT_FLAG
     KB_LWIN_FLAG
     KB_RWIN_FLAG
     KB_MENU_FLAG
     KB_SCROLOCK_FLAG
     KB_NUMLOCK_FLAG
     KB_CAPSLOCK_FLAG
     KB_INALTSEQ_FLAG
     KB_ACCENT1_FLAG
     KB_ACCENT1_S_FLAG
     KB_ACCENT2_FLAG
     KB_ACCENT2_S_FLAG
   */
  l_int     shifts;

  /* ASCII character of key, that was pressed */
  l_char    keychar;

  /* Stuffs a key into the keyboard buffer, just as if the user had pressed
     it. The parameter is in the same format returned by
     t_keyboard.readkey(t_keyboard*).
  */
  void   (*simulate_keypress) ( p_keyboard o, l_int keycode );

  void   (*set_ctrl_alt_del_flag) ( p_keyboard o, l_bool enable );
  l_bool (*get_ctrl_alt_del_flag) ( p_keyboard o );

  /* Returns the next character from the keyboard buffer. If the buffer is
     empty, it waits until a key is pressed. The low byte of the return value
     contains the ASCII code of the key, and the high byte the scancode. The
     scancode remains the same whatever the state of the shift, ctrl and alt
     keys. The ASCII code is affected by shift and ctrl in the normal way
     (shift changes case, ctrl+letter gives the position of that letter in the
     alphabet, eg. ctrl+A = 1, ctrl+B = 2, etc). Pressing alt+key returns only
     the scancode, with a zero ASCII code in the low byte.
  */
  l_int  (*readkey) ( p_keyboard o );

  /* Empties the keyboard buffer. It's called after each key_translate_event call.
  */
  void   (*clear_buffer) ( p_keyboard o );

  /* Returns true if there are keypresses waiting in the input buffer. This is
     equivalent to the libc kbhit() function.
  */
  l_bool (*keypressed) ( p_keyboard o );

} t_keyboard;

/* GLOBAL keyboard class */
extern  t_keyboard   *keyb;

#define KEYB          keyb


  /* initialize keyboard */
  extern t_keyboard*   (*key_init) ( p_keyboard o );
  t_keyboard* _key_init ( p_keyboard o );

  l_int  key_readkey ( p_keyboard o );
  void   key_simulate_keypress ( p_keyboard o, l_int keycode );
  void   key_set_ctrl_alt_del_flag ( p_keyboard o, l_bool enable );
  l_bool key_get_ctrl_alt_del_flag ( p_keyboard o );
  void   key_clear_buffer ( p_keyboard o );
  l_bool key_keypressed ( p_keyboard o );

  /* from object */

  void   key_translate_event ( t_object *o, t_event *event );
  l_bool key_done ( t_object *o );


  extern unsigned short *key_ascii_table;
  extern unsigned short *key_capslock_table;
  extern unsigned short *key_shift_table;
  extern unsigned short *key_control_table;
  extern unsigned short *key_altgr_table;

#endif

