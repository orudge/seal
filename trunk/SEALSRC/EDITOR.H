/****************************************************************/
/*                                                              */
/*                          editor.h                            */
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



#ifndef __EDITOR_H_INCLUDED__
#define __EDITOR_H_INCLUDED__

#include <stdarg.h>
#include "scroller.h"

/* info texts */

#ifndef TXT_NOTENOUGHMEMORY
#define TXT_NOTENOUGHMEMORY    INI_TEXT("Not enough memory")
#endif


/* editor' init flags */

#define  EF_REWRITEUNABLE    0x01
#define  EF_FIXEDFONT        0x02  /* font has fixed width of chars */



#define  EDITOR_CHAR_ENTER      '\n'  /* ascii 10 */
#define  EDITOR_LINE_SIZE       1
#define  EDITOR_TAB_SIZE        5


extern l_color  pal_editor[];

typedef struct t_editor *p_editor;

typedef struct t_editor {

   struct t_scroller obclass;

   l_text            text;
   l_text            line_ptr;
   l_long            line_from;
   l_long            line_num;
   l_long            line;
   l_long            line_pos;
   l_long            line_pos_from;
   l_long            line_longest;
   l_int             cursor_visible;

   t_rect            sel_from;
   t_point           sel_old;
   l_long            sel_ok;

   l_int             flags;

   l_text          (*lntxtlen) ( p_editor o, l_long line, l_long *len );
   l_long          (*lnmax) ( p_editor o, l_long *longest );
   l_text          (*lnptr) ( p_editor o, l_long line );
   l_text          (*posptr) ( p_editor o, l_long line, l_long pos );
   l_long          (*sizeto) ( l_text from, l_long deltaln, l_long deltapos );
   l_long          (*lnlen) ( p_editor o, l_long line );
   l_long          (*lnlenptr) ( p_editor o, l_text ptr );
   l_long          (*xsize_of_line) ( p_editor o, l_long line, l_long max );
   l_long          (*ysize_of_line) ( p_editor o, l_long line, l_long h, l_long (*p) );
   void            (*draw_cursor) ( p_editor o, l_long oldline, l_long oldpos );
   void            (*draw_cursor_ex) ( p_editor o, l_long line, l_long pos, l_int active );
   void            (*show_cursor) ( p_editor o, l_bool show );
   t_rect          (*get_cursor_rect) ( p_editor o, l_long oldline, l_long oldpos );
   l_long          (*charsin_size) ( p_editor o, l_rect size, l_long line, l_long pos, l_int plus );
   l_long          (*charsin) ( p_editor o, l_long line, l_long pos, l_int plus );
   l_long          (*linesin) ( p_editor o, l_long line, l_int plus );
   l_long          (*linesin_size) ( p_editor o, l_rect size, l_long line, l_int plus );
   l_int           (*lines_inpage) ( p_editor o );
   void            (*draw_select) ( p_editor o, t_rect dst, t_rect src );

   l_bool          (*insstr) ( p_editor o, l_long pos, l_text str );
   l_bool          (*delstr) ( p_editor o, l_long pos, l_long size );
   l_int           (*delchr) ( p_editor o, l_long line, l_long pos );
   l_bool          (*inschr) ( p_editor o, l_char chr, l_long line, l_long pos );

   l_bool          (*set_text) ( p_editor o, l_text txt );
   t_rect          (*line_rect) ( p_editor o, l_long line );

   void            (*draw_box_ex) ( p_editor o, l_long linefrom );
   void            (*draw_box) ( p_editor o );
   void            (*draw_line) ( p_editor o, l_long line );

   void            (*realpos) ( p_editor o, l_long *line, l_long *pos );
   void            (*redraw_text) ( p_editor o, l_long newline, l_long newpos, l_int keycode );

   t_point         (*cursor_mouse_pos) ( p_editor o, t_point where );

} t_editor;

#define EDITOR(o) ((p_editor)(o))

#define ed_sel_clear(o) ((o)->sel_from = rect_empty)
#define ed_is_wable(o)  (!((o)->flags & EF_REWRITEUNABLE))
#define ed_is_sel(o)    ((o)->sel_from.a.x != (o)->sel_from.b.x || (o)->sel_from.a.y != (o)->sel_from.b.y && ((o)->sel_from.a.x >= 0 && (o)->sel_from.a.y >= 0))

p_editor  _editor_init ( p_editor o, t_rect r, l_int flags );
extern p_editor  (*editor_init) ( p_editor o, t_rect r, l_int flags );

l_bool  editor_set_data ( p_object o, t_data *rec );
l_bool  editor_get_data ( p_object o, t_data *rec );
void    editor_set_state ( p_object o, l_dword st, l_bool set );
void    editor_set_options ( p_object o, l_dword op, l_bool set );
void    editor_translate_event ( p_object o, t_event *event );

t_rect  editor_size_limits ( p_view o );
void    editor_draw ( p_view o );
l_bool  editor_drag_where ( p_view o, t_data *rec, t_point where );


l_text  editor_lntxtlen ( p_editor o, l_long line, l_long *len );
l_long  editor_lnmax ( p_editor o, l_long *longest );
l_text  editor_lnptr ( p_editor o, l_long line );
l_text  editor_posptr ( p_editor o, l_long line, l_long pos );
l_long  editor_sizeto ( l_text from, l_long deltaln, l_long deltapos );
l_long  editor_lnlen ( p_editor o, l_long line );
l_long  editor_lnlenptr ( p_editor o, l_text ptr );
l_long  editor_xsize_of_line ( p_editor o, l_long line, l_long max );
l_long  editor_ysize_of_line ( p_editor o, l_long line, l_long h, l_long (*p) );
void    editor_draw_cursor ( p_editor o, l_long oldline, l_long oldpos );
void    editor_draw_cursor_ex ( p_editor o, l_long line, l_long pos, l_int active );
void    editor_show_cursor ( p_editor o, l_bool show );
t_rect  editor_get_cursor_rect ( p_editor o, l_long oldline, l_long oldpos );
l_long  editor_charsin_size ( p_editor o, l_rect size, l_long line, l_long pos, l_int plus );
l_long  editor_charsin ( p_editor o, l_long line, l_long pos, l_int plus );
l_long  editor_linesin ( p_editor o, l_long line, l_int plus );
l_long  editor_linesin_size ( p_editor o, l_rect size, l_long line, l_int plus );
l_int   editor_lines_inpage ( p_editor o );
void    editor_draw_select ( p_editor o, t_rect dst, t_rect src );

l_bool  editor_insstr ( p_editor o, l_long pos, l_text str );
l_bool  editor_delstr ( p_editor o, l_long pos, l_long size );
l_int   editor_delchr ( p_editor o, l_long line, l_long pos );
l_bool  editor_inschr ( p_editor o, l_char chr, l_long line, l_long pos );

l_bool  editor_set_text ( p_editor o, l_text txt );

t_rect  editor_line_rect ( p_editor o, l_long line );

void    editor_draw_box_ex ( p_editor o, l_long linefrom );
void    editor_draw_box ( p_editor o );
void    editor_draw_line ( p_editor o, l_long line );

void    editor_realpos ( p_editor o, l_long *line, l_long *pos );
void    editor_redraw_text ( p_editor o, l_long newline, l_long newpos, l_int keycode );

t_point editor_cursor_mouse_pos ( p_editor o, t_point where );

#endif

