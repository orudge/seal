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
/*                           helpsys.h                          */
/*                     Bad Seal help system                     */
/*                             v0.3                             */
/*                                                              */
/*        (c) 2001 Kostas Michalopoulos aka Bad Sector          */
/*                                                              */
/****************************************************************/

#ifndef __HELPSYS_H_INCLUDED__
#define __HELPSYS_H_INCLUDED__

//#include "badctrls.h"

/* basic use of Bad Seal Help System - call HELP from your programs */

/* this calls HELP.EXE and displays FILENAME. Help System will first
 look at the current directory for FILENAME and if not found, will look
 at HELP subdirectory. If again not found, will pop up an error message. */
void open_help(l_text filename);

/* like open_help, but displays a specified topic instead of "main" */
void open_help_topic(l_text filename, l_text topic);

/* advanced use of Bad Seal Help System - add a help box to your
 own programs. */

/* help viewer */
#define HELP_EXE "./s2a-bin/help.s2a"

/*** requests ***/

#define HBREQ_NOTHING   0
/* add button to Help Viewer (command <BUTTON> -unfinished-)*/
#define HBREQ_BUTTON    1

/*** flags ***/
#define HBF_UNSELECTABLE        0x0001
#define HBF_NOSCROLL            0x0002
#define HBF_WATERMARK           0x0004
#define HBF_RESERVED1           0x0008

/* __t_xlink & __t_hbgbinfo are used internally */
typedef struct __t_xlink
{
  l_int x1, y1, x2, y2;
  l_text where;
  l_text target;
  l_text name;
  l_char reserved[16];
} __t_xlink;

typedef struct __t_hbgbinfo *__p_hbgbinfo;
typedef struct __t_hbgbinfo
{
  l_text where;

  __p_hbgbinfo prev;
} __t_hbgbinfo;

typedef struct __t_hbbitmap *__p_hbbitmap;
typedef struct __t_hbbitmap
{
  l_text name;
  BITMAP *bmp;

  __p_hbbitmap next;
} __t_hbbitmap;

#define STANDARD_GET_BITMAP_SEARCH { \
  __p_hbbitmap p = o->bitmaps, m; \
  \
  if (p) do { \
    if (!stricmp(p->name, name)) return p->bmp; \
    p = p->next; \
  } while (p); \
}

#define ADD_BITMAP_TO_BITMAP_LIST(o,b,n) { \
  __p_hbbitmap m, p = o->bitmaps; \
  m = (__p_hbbitmap) malloc(sizeof(__t_hbbitmap)); \
  m->bmp = b; \
  m->name = strdup(n); \
  m->next = NULL; \
  \
  if (!p) o->bitmaps = m; else { \
    if (p->next) do { \
      p = p->next; \
    } while (p->next); \
    p->next = m; \
  }; \
}

typedef struct t_helpbox *p_helpbox;

typedef struct __t_hbsplit *__p_hbsplit;
typedef struct __t_hbsplit
{
  l_char name[51];
  p_helpbox split;

  __p_hbsplit prev;
  __p_hbsplit next;
} __t_hbsplit;

typedef struct t_helpbox
{
  t_view obclass;

  l_text text;                                  /* topic text */
  __p_hbbitmap bitmaps;                         /* bitmaps used
                                                  from current topic */

  /* to open a HLP file, assign it's name in this variable and call
    helpbox_navigate(o, topic); where TOPIC is the topic that you
    want to display */

  l_text filename;                              /* current help file */
  l_text topic;                                 /* current topic */
  l_big flags;                                  /* some flags */
  l_text background;                            /* background image */

  /* all these are for internal use - do not modify them */
  l_int f_x, f_y, f_mx, f_my, f_prg, f_maxy;
  l_bool u_link, u_bold, f_avoid_bi, f_split, f_in_split;
  p_helpbox active_split;
  __p_hbsplit splits;
  l_text _script;

  __t_xlink link[1000];
  l_int links;

  __p_hbgbinfo f_back;

  /* assign here a function for handling requests. return TRUE if request
  is handled or FALSE if not */
  l_bool (*request)(p_helpbox o, l_int request, void *data1, void *data2, void **rdata1);

  l_char reserved_for_data[1024];

  void (*navigate)(p_helpbox o, l_text where);
  void (*go_back)(p_helpbox o);
  BITMAP *(*get_bitmap)(p_helpbox o, l_text name);

  l_char reserved_for_functions[256];

} t_helpbox;

#define HELPBOX(o) ((p_helpbox)(o))

/* this navigates somewhere. where is the place where we will be navigated to.
can be:

  help:about                    shows about info
  help:open                     pops up an open dialog
  help:none                     nothing

anything else will make helpbox to try to open o->filename and read section
defined in where. */
void helpbox_navigate(p_helpbox o, l_text where);

/* this goes to previous viewed place */
void helpbox_go_back(p_helpbox o);

/* this returns the bitmap "name" */
BITMAP *helpbox_get_bitmap(p_helpbox o, l_text name);

/* this draws helpbox */
void helpbox_draw(p_view o);

/* this handles (aka translates) SEAL events for helpbox such mouse moving,
key pressing, mouse clicking, etc */
void helpbox_handle_event(p_object o, t_event *event);

/* this initialized a new helpbox in rectangular area R. */

p_helpbox _helpbox_init(p_helpbox o, t_rect r);
extern p_helpbox (*helpbox_init)(p_helpbox o, t_rect r);

#endif

