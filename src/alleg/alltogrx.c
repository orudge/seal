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

#include <seal.h>
#include <alltogrx.h>

int  get_text_length ( void *f, unsigned char *str, int len );
int  get_text_height ( void *f );
void draw_text_out ( BITMAP *b, void *f, unsigned char *text, int x1, int y1, GrCOLOR color );
void _change_font_size ( void *f, int w, int h );

int   (*textlen)(void *f, unsigned char *text, int len ) = &get_text_length;
int   (*textheight)(void *f ) = &get_text_height;
void  (*drawtext)(BITMAP *b, void *f, unsigned char *text, int x1, int y1, GrCOLOR color ) = &draw_text_out;
void  (*change_font_size)(void *f, int w, int h) = &_change_font_size;

#include"newscr.c"
