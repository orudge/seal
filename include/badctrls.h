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
/*                          badctrls.h                          */
/*                     Bad Controls for SEAL                    */
/*                            v0.4.2                            */
/*                                                              */
/*      (c) 2000, 2001 Kostas Michalopoulos aka Bad Sector      */
/*         first version for Bad Seal | www.badseal.org         */
/*                                                              */
/* ============================================================ */
/*  Controls included:                                          */
/*    t_bad_checkbox                          Checkbox control  */
/*    t_bad_button                        t_button replacement  */
/*    t_bad_vtrackbar                        vertical trackbar  */
/*    t_bad_tabbook                                   tab book  */
/*    t_bad_iconbox                         box with icons :-)  */
/****************************************************************/

#ifndef __BADCTRL_H_INCLUDED__
#define __BADCTRL_H_INCLUDED__

#define BCB_UNCHECKED 0
#define BCB_CHECKED 1
#define BCB_GRAYED 2

#define BBS_SEAL 0
#define BBS_BADSEAL 1
#define BBS_KDE 2
#define BBS_WIN95 3
#define BBS_WIN31 4
#define BBS_MODERN 5
#define BBS_LIQUID 6

/* makes iconbox to place icons in horizontal order (default is vertical) */
#define BIB_PLACE_HORIZONTAL  0x00000001
/* iconbox contains a background image */
#define BIB_BACKGROUND_IMAGE  0x00000002
/* place background image like desktop */
#define BIB_DESKTOP_PLACEMENT 0x00000004

typedef struct t_bad_checkbox *p_bad_checkbox;
typedef struct t_bad_button *p_bad_button;
typedef struct t_bad_vtrackbar *p_bad_vtrackbar;
typedef struct t_bad_tabbook *p_bad_tabbook;
typedef struct t_bad_iconbox *p_bad_iconbox;

/* t_bad_checkbox */
typedef struct t_bad_checkbox
{
  struct t_view obclass;

  l_int state;                  /* 0=unchecked, 1=checked, 2=disabled */
  l_bool grayed;                /* 0=no, 1=yes */
  l_text caption;               /* Checkbox's caption */

  l_dword message;              /* if checkbox checked or unchecked then
                                  this message occurs */

  void (*draw_state)(p_bad_checkbox o, l_int astate);
  void (*change_state)(p_bad_checkbox o, l_int newstate);
  void (*change_caption)(p_bad_checkbox o, l_text newcap);
} t_bad_checkbox;

#define BAD_CHECKBOX(o) ((p_bad_checkbox)(o))

/* functions for bad_checkbox */

/* destroys the control */
l_bool bad_checkbox_done(p_object o);

/* changes control's state and redraws the control */
void bad_checkbox_set_state(p_object o, l_dword st, l_bool set);

/* handles events for checkbox */
void bad_checkbox_translate_event(p_object o, t_event *event);

/* draws the checkbox */
void bad_checkbox_draw(p_view o);

/* draws the checkbox's state (checked, unchecked, disabled/grayed) */
void bad_checkbox_draw_state(p_bad_checkbox o, l_int astate);

/* changes checkbox's state (0=unchecked, 1=checked, 2=grayed) */
void bad_checkbox_change_state(p_bad_checkbox o, l_int newstate);

/* changes checkbox's caption (you don't need to allocate memory here) */
void bad_checkbox_change_caption(p_bad_checkbox o, l_text newcap);

/* creates a t_bad_checkbox object in area defined by "r", using caption "cap",
  message "msg" and state "st".

  this automatically allocates memory for the caption.
*/

p_bad_checkbox _bad_checkbox_init(p_bad_checkbox o, t_rect r, l_text cap, l_dword msg, l_int st);
extern p_bad_checkbox (*bad_checkbox_init)(p_bad_checkbox o, t_rect r, l_text cap, l_dword msg, l_int st);


/* t_bad_button */
typedef struct t_bad_button
{
  struct t_view obclass;

  l_int state;                  /* 0=normal, 1=pressed, 2=disabled */
  l_int style;                  /* Button's style (look BBS_xx values) */
  l_text caption;               /* Button's caption */
  l_bool switch_button;         /* if True then button acts like a checkbox */

  l_dword message;              /* if button is pressed then this message occurs */

  void (*draw_state)(p_bad_button o, l_int astate);
  void (*change_state)(p_bad_button o, l_int newstate);
  void (*change_style)(p_bad_button o, l_int newstyle);
  void (*change_caption)(p_bad_button o, l_text newcap);
} t_bad_button;

#define BAD_BUTTON(o) ((p_bad_button)(o))

/* functions for bad_button */

/* destroys the control */
l_bool bad_button_done(p_object o);

/* changes control's state and redraws the control */
void bad_button_set_state(p_object o, l_dword st, l_bool set);

/* handles events for the button */
void bad_button_translate_event(p_object o, t_event *event);

/* draws the button */
void bad_button_draw(p_view o);

/* draws the button's state */
void bad_button_draw_state(p_bad_button o, l_int astate);

/* changes button's state (0=normal, 1=pressed) */
void bad_button_change_state(p_bad_button o, l_int newstate);

/* changes button's style (see BBS_xx values) */
void bad_button_change_style(p_bad_button o, l_int newstyle);

/* changes button's caption (you don't need to allocate memory here) */
void bad_button_change_caption(p_bad_button o, l_text newcap);

/* creates a t_bad_button object in area defined by "r", using caption "cap",
  style "style" and message "msg".

  this automatically allocates memory for the caption.
*/

p_bad_button _bad_button_init(p_bad_button o, t_rect r, l_text cap, l_int style, l_dword msg);
extern p_bad_button (*bad_button_init)(p_bad_button o, t_rect r, l_text cap, l_int style, l_dword msg);


/* t_bad_vtrackbar */
typedef struct t_bad_vtrackbar
{
  struct t_view obclass;

  l_long min;                   /* minimum value that the trackbar can take */
  l_long max;                   /* maximum value that... */
  l_long pos;                   /* position of the track */

  l_dword message;              /* if trackbar is changed this
                                  message occurs */

  l_bool pressed;               /* true if track is pressed */

  l_char reserved[8];

  void (*change_pos)(p_bad_vtrackbar o, l_long newpos);
} t_bad_vtrackbar;

#define BAD_VTRACKBAR(o) ((p_bad_vtrackbar)(o))

/* functions for bad_vtrackbar */

/* changes track's position */
void bad_vtrackbar_change_pos(p_bad_vtrackbar o, l_long pos);

/* changes control's state and redraws the control */
void bad_vtrackbar_set_state(p_object o, l_dword st, l_bool set);

/* handles events for trackbar */
void bad_vtrackbar_translate_event(p_object o, t_event *event);

/* draws the trackbar */
void bad_vtrackbar_draw(p_view o);

/* creates a t_bad_vtrackbar object in area defined by "r".
   default values are:

      minimum possible value = 0
      maximum possible value = 100
      position               = 0
*/

p_bad_vtrackbar _bad_vtrackbar_init(p_bad_vtrackbar o, t_rect r, l_dword msg);
extern p_bad_vtrackbar (*bad_vtrackbar_init)(p_bad_vtrackbar o, t_rect r, l_dword msg);

/* t_bad_tabbook */
typedef struct t_bad_tabbook
{
  struct t_view obclass;

  l_text title[64] ;            /* titles for tabs */
  p_view tab[64];               /* tabs */

  l_int active;                 /* active tab */
  l_int tabs;                   /* count of tabs */

  l_char reserved[24];

  void (*select_tab)(p_bad_tabbook o, l_int newtab);
  void (*add_tab)(p_bad_tabbook o, l_text title);
  void (*remove_tab)(p_bad_tabbook o, l_int tab);
  void (*on_changed)(p_bad_tabbook o, l_int newtab);   /* if not NULL, called
                                            when selected tab is changed to
                                            another tab */
} t_bad_tabbook;

#define BAD_TABBOOK(o) ((p_bad_tabbook)(o))

/* functions for bad_tabbook */

/* changes active tab */
void bad_tabbook_select_tab(p_bad_tabbook o, l_int newtab);

/* adds a new tab to tab book with title `title' note that
t_bad_tabbook can have up to 64 tabs (actually there isn't
enough room to display 64 tabs but anyway - later i'll add
scrolling buttons). To insert controls in tabs just use
tab[] array in t_bad_tabbook struct. */
void bad_tabbook_add_tab(p_bad_tabbook o, l_text title);

/* removes the tab `tab' from tab book */
void bad_tabbook_remove_tab(p_bad_tabbook o, l_int tab);

/* creates a t_bad_tabbook object in area defined by "r" with one
 tab named "untitled". Because every tab book must have at least one
 tab (else why to exist?), you must define a title for the first tab.
 Title is defined in ftitle parameter. */

/*
else why to exist?
jdh - umm let see - memory allocation problems could mean you were unable to make the first one.
*/

p_bad_tabbook _bad_tabbook_init(p_bad_tabbook o, t_rect r, l_text ftitle);
extern p_bad_tabbook (*bad_tabbook_init)(p_bad_tabbook o, t_rect r, l_text ftitle);

/* t_bad_iconbox */

typedef struct t_bad_iconbox_item *p_bad_iconbox_item;

typedef struct t_bad_iconbox_item
{
  t_point pos;
  BITMAP *icon;
  l_text caption;
  l_char reserved[24];
} t_bad_iconbox_item;

typedef struct t_bad_iconbox
{
  struct t_view obclass;

  p_list icons;

  t_point next_pos;

  t_point caption_size;
  t_point spacing;

  l_int count, index;

  l_big flags;

// now I've given you a way of extending these structs can I get rid of these reserved things please
//  l_char reserved[512]; gone;-)

  void (*add)(p_bad_iconbox o, BITMAP *icon, l_text caption);
  void (*remove)(p_bad_iconbox o, l_int item);  /* isn't working */
  void (*clear)(p_bad_iconbox o);               /* isn't working */

  void (*on_dclick)(p_bad_iconbox o, l_int item);   /* NULL by default - set
                                                    a function to this field
                                                    to know when an icon is
                                                    doubleclicked */
//  l_char reserved2[128];
} t_bad_iconbox;

#define BAD_ICONBOX(o) ((p_bad_iconbox)(o))

/* returns true if mouse is under icon "item" in icon box "o" */
l_bool bad_iconbox_mouse_under_icon_at(p_bad_iconbox o, l_int item);

/* adds icon "icon" (size 32x32) in icon box with caption "caption" */
void bad_iconbox_add(p_bad_iconbox o, BITMAP *icon, l_text caption);

/* creates a t_bad_iconbox object in area defined by "r" with "flags".
   An iconbox is a box that contains moveable icons.
*/
p_bad_iconbox _bad_iconbox_init(p_bad_iconbox o, t_rect r, l_big flags);
extern p_bad_iconbox (*bad_iconbox_init)(p_bad_iconbox o, t_rect r, l_big flags);

#endif

