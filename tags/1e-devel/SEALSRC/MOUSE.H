/****************************************************************/
/*                                                              */
/*                          mouse.h                             */
/*                                                              */
/*                Seal - free desktop environment               */
/*                                                              */
/*                  Copyright (c) 1999,2000                     */
/*                       Michal Stencl                          */
/*                    All Rights Reserved                       */
/*                                                              */
/* mail : stenclpmd@ba.telecom.sk                               */
/* web  : http://www.home.sk/public/seal/                       */
/*                                                              */
/* This file is part of Seal.                                   */
/*                                                              */
/* Seal is free software; you can redistribute it and/or        */
/* modify it under the terms of the GNU General Public License  */
/* as published by the Free Software Foundation; either version */
/* 2, or (at your option) any later version.                    */
/*                                                              */
/* Seal is distributed in the hope that it will be useful, but  */
/* WITHOUT ANY WARRANTY; without even the implied warranty of   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See    */
/* the GNU General Public License for more details.             */
/*                                                              */
/* You should have received a copy of the GNU General Public    */
/* License along with Seal; see the file COPYING.  If not,      */
/* write to the Free Software Foundation, 675 Mass Ave,         */
/* Cambridge, MA 02139, USA.                                    */
/*                                                              */
/****************************************************************/



#include"object.h"
#include"alltogrx.h"

#ifndef __MOUSE_H_INCLUDED__
#define __MOUSE_H_INCLUDED__

#ifndef   TXT_MOUSENOTINSTALLED
#define   TXT_MOUSENOTINSTALLED     INI_TEXT("\n::mouse is not installed\n")
#endif


extern l_int   mouse_flags;
extern l_color mouse_mode_color;

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

/* the limit of cursors in "cursor_system" DATAFILE */
#ifndef CUR_DATAMAX
#define CUR_DATAMAX     32
#endif

#define CURSOR_SYSTEM   cursor_system
#define CURSOR_STANDARD cursor_standard
#define CURSOR_FOCUS    cursor_focus

/* mouse cursors */
#ifndef CUR_ARROW
#define CUR_ARROW            0x00
#endif
#ifndef CUR_MOVE
#define CUR_MOVE             0x01
#endif
#ifndef CUR_GROW
#define CUR_GROW             0x02
#endif
#ifndef CUR_TEXT
#define CUR_TEXT             0x03
#endif
#ifndef CUR_CLOCK
#define CUR_CLOCK            0x04
#endif
#ifndef CUR_PENCIL
#define CUR_PENCIL           0x05
#endif
#ifndef CUR_TARGET
#define CUR_TARGET           0x06
#endif
#ifndef CUR_TARGET2
#define CUR_TARGET2          0x07
#endif
#ifndef CUR_STOP
#define CUR_STOP             0x08
#endif
#ifndef CUR_ZOOM
#define CUR_ZOOM             0x09
#endif
#ifndef CUR_SCRUP
#define CUR_SCRUP            0x0a
#endif
#ifndef CUR_SCRDN
#define CUR_SCRDN            0x0b
#endif
#ifndef CUR_SCRLF
#define CUR_SCRLF            0x0c
#endif
#ifndef CUR_SCRRG
#define CUR_SCRRG            0x0d
#endif
#ifndef CUR_TEXT2
#define CUR_TEXT2            0x0e
#endif
#ifndef CUR_TEXT2
#define CUR_TEXT2            0x0e
#endif
#ifndef CUR_DRAG
#define CUR_DRAG             0x0f
#endif
#ifndef CUR_FINGER
#define CUR_FINGER           0x10
#endif

/* return popinter to loaded DATAFILE, where cursors are stored from file "f"
   and ignore background color of cursors that's defined by r,g,b values
*/
#define cursor_load_file(f,r,g,b)  GrLoadSkipDataFile(f, r, g, b)

#define CURSOR_GETSYSTEMXFOCUS(cn)  ((CUR_DATAMAX>cn)?CURSOR_FOCUS[cn*2]:0)
#define CURSOR_GETSYSTEMYFOCUS(cn)  ((CUR_DATAMAX>cn)?CURSOR_FOCUS[cn*2+1]:0)
#define CURSOR_GETCURSOR(data,n)    ((data && n>=0 && n<CUR_DATAMAX && data[n].type==DAT_BITMAP)?((GrCONTEXT*)data[n].dat):NULL)
#define CURSOR_GETCURSORWIDTH(d,n)  ((CURSOR_SYSTEM && n>=0 && n<CUR_DATAMAX && d[n].type==DAT_BITMAP)?((GrCONTEXT*)d[n].dat)->w:NULL)
#define CURSOR_GETCURSORHEIGHT(d,n) ((CURSOR_SYSTEM && n>=0 && n<CUR_DATAMAX && d[n].type==DAT_BITMAP)?((GrCONTEXT*)d[n].dat)->h:NULL)
#define CURSOR_GETSYSTEM(n)         CURSOR_GETCURSOR(CURSOR_SYSTEM,n)
#define CURSOR_GETSYSTEMWIDTH(n)    CURSOR_GETCURSORWIDTH(CURSOR_SYSTEM, n)
#define CURSOR_GETSYSTEMHEIGHT(n)   CURSOR_GETCURSORHEIGHT(CURSOR_SYSTEM,n)

#define  INI_MOUSE     "mouse"

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

/*
  object t_mouse is used for getting information from mouse, or set cursors, speed,
  range, etc... This object is placed in t_driver object together with t_keyboard.
  Whenever is OBJECT(driver)->translate_event... called it calls also
  translate_event function from t_mouse structures. If the user operate with the mouse,
  it sets t_mouse.obclass.state or OBJECT(&t_mouse)->state to mouse flags ...see above.
*/
typedef struct t_mouse {

  /* inherite functions / variables from t_object class */
  struct t_object  obclass;

  /* speed of mouse, speed.x = 0 is maximal speed in x coord and
     speed.x = 10 is minimal speed. It's same in speed.y
  */
  struct t_point   speed;

  /* point of mouse, where cursor is placed on the screen */
  struct t_point   where;

  /* number of buttons are available for mouse */
  l_char           buttons;

  l_dword          state_last;
  l_int            dclick_milisec;
  l_big            dclick_old_time;


  /* return BITMAP of cursor */
  BITMAP*   (*get_cursor)(p_mouse o);

  /* return focus point of cursor. It's delta point from begining of
     cursor' BITMAP, where focus is placed. */
  t_point   (*get_focus)(p_mouse o);

  /* return range of the cursor, where is available on the screen */
  t_rect    (*get_range)(p_mouse o);

  /* return true if cursor is visible on screen, else return false */
  l_bool    (*is_visible)(p_mouse o);

  /* set double-click difference. Miliseconds between two clicks, when it
     defined as double-click
  */
  void      (*set_dclick_diff)(p_mouse o, l_int mili);

  /* show cursor on screen. true if ok, false if not. */
  l_bool    (*show)(p_mouse o);

  /* hide mouse from screen, true if ok, false if not. */
  l_bool    (*hide)(p_mouse o);

  /* block mouse in bounds. when curosr is visible and it's placed somewhere
     in these bounds, it hides cursor and return non-zero, otherwise it
     returns zero and not hide mouse. This value use later in mouse->unblock
     function, when you want to mouse show on screen.
  */
  l_int     (*block)(p_mouse o, t_rect r );

  /* unblock mouse, (i) is returned value from mouse->block function.
     this functions are used as push/pop in assembler.
  */
  void      (*unblock)(p_mouse o, l_int i );

  /* returns true, if mouse is blocked and it's not visible and false if
     isn't blocked
  */
  l_bool    (*is_block)(p_mouse o );

  /* set position of cursor to (where) point on the screen. */
  void      (*set_pos)( p_mouse o, t_point where );

  /* set cursor of mouse to BITMAP (cursor). returns true if cursor was
     succesfull set, else returns false.
  */
  l_bool    (*set_cursor)(p_mouse o, BITMAP *cursor);

  /* set cursor to BITMAP ( cursor ) and set focus point of cursor
     to (p). Return true if all is ok, else return false.
  */
  l_bool    (*set_cursor_focus)(p_mouse o, BITMAP *cursor, t_point p );

  /* set cursor mode (mode) in color (color) and rect (r).

     You can choose from these modes :

     MO_MO_RECT - efect like drag-window frame
     MO_MO_GROW - efect like resize-window frame

     returns true, if all is ok, else return false.
  */
  l_bool    (*set_mode)(p_mouse o, l_int mode, l_color col, t_rect r );

  /* set bounds of cursor, where the cursor will be available */
  void      (*set_range)(p_mouse o, t_rect r );

  /* set speed to (speed). This speed further get by mouse->speed */
  void      (*set_speed)(p_mouse o, t_point speed );

  /* set focus or hot-point of cursor */
  void      (*set_focus)(p_mouse o, t_point focus );

} t_mouse;

extern   t_mouse *mouse;
#define  MOUSE    mouse

extern DATAFILE  *cursor_system;
extern BITMAP    *cursor_standard;
extern l_byte     cursor_focus[];

/* set cursor from standard cursor' types, where (id) maybe one of CUR_XXXX values,
   see above.
*/
#define  mouse_set_cursor_focus_id(id)  MOUSE->set_cursor_focus(MOUSE, CURSOR_GETSYSTEM(id), point_assign(CURSOR_GETSYSTEMXFOCUS(id), CURSOR_GETSYSTEMYFOCUS(id)))



#define  mouse_set_focus_id(id)         MOUSE->set_focus(MOUSE, point_assign(CURSOR_GETSYSTEMXFOCUS(id), CURSOR_GETSYSTEMYFOCUS(id)))
#define  mouse_set_cursor_id(id)        MOUSE->set_cursor(MOUSE, CURSOR_GETSYSTEM(id))

#define  mouse_rect_block(ctx,i,r)          msblock##i = mouse->block(mouse, ctx, r)
#define  mouse_rect_unblock(i)              mouse->unblock(mouse, i)

/* mouse functions */

  /* init mouse */
  extern t_mouse*  (*mouse_init) ( p_mouse o );
  t_mouse*  _mouse_init ( p_mouse o );

  BITMAP*   mouse_get_cursor(p_mouse o);
  t_point   mouse_get_focus(p_mouse o);
  t_rect    mouse_get_range(p_mouse o);
  l_bool    xx_mouse_is_visible(p_mouse o);
  void      mouse_set_dclick_diff(p_mouse o, l_int mili);
  l_bool    mouse_show(p_mouse o);
  l_bool    mouse_hide(p_mouse o);
  void      mouse_set_pos(p_mouse o, t_point where );
  l_bool    mouse_set_cursor(p_mouse o, BITMAP *cursor);
  l_bool    mouse_set_cursor_focus(p_mouse o, BITMAP *cursor, t_point p );
  l_bool    mouse_set_mode(p_mouse o, l_int mode, l_color col, t_rect r );
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

