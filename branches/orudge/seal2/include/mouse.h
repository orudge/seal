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
#include"alltogrx.h"

#ifndef __MOUSE_H_INCLUDED__
#define __MOUSE_H_INCLUDED__

#ifndef   TXT_MOUSENOTINSTALLED
#define   TXT_MOUSENOTINSTALLED     INI_TEXT("\n::mouse is not installed\n")
#endif


extern l_int   mouse_flags;

extern l_int   msblock0;
extern l_int   msblock1;
extern l_int   msblock2;
extern l_int   msblock3;
extern l_int   msblock4;


/* mouse modes */
/* show rectangle together with mouse cursor */
#define MO_MO_RECT      0x01
/* show growing rectangle together with mouse cursor */
#define MO_MO_GROW      0x02

#ifndef CUR_SKIP_COLORS
#define CUR_SKIP_COLORS
#define CUR_SKIP_RED    NOCOLOR_RED
#define CUR_SKIP_GREEN  NOCOLOR_GREEN
#define CUR_SKIP_BLUE   NOCOLOR_BLUE
#endif

/* mouse cursors */

#define CUR_ARROW            0
#define CUR_MOVE             1
#define CUR_GROW             2
#define CUR_TEXT             3
#define CUR_CLOCK            4
#define CUR_PENCIL           5
#define CUR_TARGET           6
#define CUR_TARGET2          7
#define CUR_STOP             8
#define CUR_ZOOM             9
#define CUR_SCRUP            10
#define CUR_SCRDN            11
#define CUR_SCRLF            12
#define CUR_SCRRG            13
#define CUR_TEXT2            14
#define CUR_DRAG             15
#define CUR_FINGER           16

/* the differenc between two clicks that's still defined as doubleclick */
#define  MOUSE_TIME_DOUBLECLICK  500

/* mouse flags */
#define  MO_SF_MOUSENOTHING      0x0000000
/* cursor was changed the position in (x) or (y) */
#define  MO_SF_MOUSEMOVE         0x0000020
/* left mouse button was pressed */
#define  MO_SF_MOUSELDOWN        0x0000040
/* left mouse button was released */
#define  MO_SF_MOUSELUP          0x0000080
/* right mouse button was pressed */
#define  MO_SF_MOUSERDOWN        0x0000100
/* right mouse button was released */
#define  MO_SF_MOUSERUP          0x0000200
/* middle mouse button was pressed */
#define  MO_SF_MOUSEMDOWN        0x0000400
/* middle mouse button was released */
#define  MO_SF_MOUSEMUP          0x0000800
/* cursor was showed */
#define  MO_SF_MOUSESHOW         0x0001000
/* cursor was hiden */
#define  MO_SF_MOUSEHIDE         0x0002000
/* left mouse button is still pressed */
#define  MO_SF_MOUSELAUTO        0x0004000
/* middle mouse button is still pressed */
#define  MO_SF_MOUSEMAUTO        0x0008000
/* right mouse button is still pressed */
#define  MO_SF_MOUSERAUTO        0x0010000
/* left mouse button was doubleclicked */
#define  MO_SF_MOUSELDOUBLE      0x0020000
/* right mouse button was doubleclicked */
#define  MO_SF_MOUSERDOUBLE      0x0040000
/* middle mouse button was doubleclicked */
#define  MO_SF_MOUSEMDOUBLE      0x0080000

#define  MO_SF_MOUSEDRAG         0x0100000

/* any of mouse buttons was doubleclicked */
#define  MO_SF_MOUSEDOUBLE       (MO_SF_MOUSELDOUBLE|MO_SF_MOUSERDOUBLE|MO_SF_MOUSEMDOUBLE)
/* any of mouse buttons is still pressed */
#define  MO_SF_MOUSEAUTO         (MO_SF_MOUSELAUTO|MO_SF_MOUSERAUTO|MO_SF_MOUSEMAUTO)
/* any of mouse buttons was pressed */
#define  MO_SF_MOUSEDOWN         (MO_SF_MOUSELDOWN|MO_SF_MOUSERDOWN|MO_SF_MOUSEMDOWN)
/* any of mouse buttons was released */
#define  MO_SF_MOUSEUP           (MO_SF_MOUSELUP|MO_SF_MOUSERUP|MO_SF_MOUSEMUP)
/* any of mouse buttons is down */
#define  MO_SF_MOUSEPRESS        (MO_SF_MOUSEAUTO|MO_SF_MOUSEDOWN)
/* left mouse button is down */
#define  MO_SF_MOUSELPRESS       (MO_SF_MOUSELAUTO|MO_SF_MOUSELDOWN)
/* middle mouse button is down */
#define  MO_SF_MOUSEMPRESS       (MO_SF_MOUSEMAUTO|MO_SF_MOUSEMDOWN)
/* right mouse button is down */
#define  MO_SF_MOUSERPRESS       (MO_SF_MOUSERAUTO|MO_SF_MOUSERDOWN)


typedef struct t_mouse *p_mouse;

typedef struct t_cursor *p_cursor;

typedef struct t_cursor {

  BITMAP *bmp;

  struct t_point focus;

} t_cursor;

typedef struct t_mouse {

  struct t_object  obclass;

  struct t_point   speed;

  struct t_point   where;

  l_char           buttons;

  l_dword          state_last;
  l_int            dclick_milisec;
  l_big            dclick_old_time;

  p_cursor   (*get_cursor)(p_mouse o);

  t_point   (*get_focus)(p_mouse o);

  t_rect    (*get_range)(p_mouse o);

  l_bool    (*is_visible)(p_mouse o);

  void      (*set_dclick_diff)(p_mouse o, l_int mili);

  l_bool    (*show)(p_mouse o);

  l_bool    (*hide)(p_mouse o);

  l_int     (*block)(p_mouse o, t_rect r );

  void      (*unblock)(p_mouse o, l_int i );

  l_bool    (*is_block)(p_mouse o );

  void      (*set_pos)( p_mouse o, t_point where );

  l_bool    (*set_cursor)(p_mouse o, p_cursor cursor);

  void      (*set_range)(p_mouse o, t_rect r );

  void      (*set_speed)(p_mouse o, t_point speed );

  void      (*set_focus)(p_mouse o, t_point focus );

} t_mouse;

extern   t_mouse *mouse;
#define  MOUSE    mouse

extern p_cursor system_cursors[17];

#define  mouse_set_sys_cursor(id)           MOUSE->set_cursor(MOUSE, ( id < 17 )? system_cursors[id] : system_cursors[0])

#define  mouse_set_cursor_focus_id          mouse_set_sys_cursor

#define  mouse_rect_block(ctx,i,r)          msblock##i = mouse->block(mouse, ctx, r)

#define  mouse_rect_unblock(i)              mouse->unblock(mouse, i)

/* mouse functions */

  /* init mouse */
  extern t_mouse*  (*mouse_init) ( p_mouse o );
  t_mouse*  _mouse_init ( p_mouse o );

  p_cursor  mouse_get_cursor(p_mouse o);
  t_point   mouse_get_focus(p_mouse o);
  t_rect    mouse_get_range(p_mouse o);
  l_bool    xx_mouse_is_visible(p_mouse o);
  void      mouse_set_dclick_diff(p_mouse o, l_int mili);
  l_bool    mouse_show(p_mouse o);
  l_bool    mouse_hide(p_mouse o);
  void      mouse_set_pos(p_mouse o, t_point where );
  l_bool    mouse_set_cursor(p_mouse o, p_cursor cursor);
  void      mouse_set_range(p_mouse o, t_rect r );
  void      mouse_set_speed(p_mouse o, t_point speed );
  void      mouse_set_focus(p_mouse o, t_point focus );

  l_int     xx_mouse_block(p_mouse o, t_rect r );
  void      xx_mouse_unblock(p_mouse o, l_int i );
  l_bool    xx_mouse_is_block(p_mouse o );

  /* from object */

  l_bool    mouse_done ( t_object *o );
  void      mouse_translate_event ( t_object *o, t_event *event );

  void      mouse_load_cursor ( l_char *filename );
  l_bool    mouse_set_cursor_system_file ( l_char *cursorfile, l_int r, l_int g, l_int b );


  void    clear_event_mouse ( void );

#endif

