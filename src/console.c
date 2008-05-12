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
#include <window.h>
#include <conio.h>

#define GRAPHIC_MODE true
#define TEXT_MODE    false

typedef struct t_console *p_console;
typedef struct t_vscr    *p_vscr;

typedef struct t_vscr { // Virtual Screen

   l_bool   mode; // Text or Graphics mode

   BITMAP  *bmp; // Screen Bitmap (the showed thing)

   unsigned    fntX; // Font width
   unsigned    fntY; // Font height

   FONT    *font; // Used font

   l_text   text; // Char table representing the screen in alpha mode, just for getchar function ...
   l_char  *tcol; // Color table representing the screen in alpha mode, just for getchar function ...

   l_char      cur_tcol; // Cursor color

   unsigned    cur_col; // Cursor X (colon)
   unsigned    cur_lgn; // Cursor Y (line)

   unsigned    penX; // Pen x position
   unsigned    penY; // Pen y position

   unsigned    txtX; // Width of screen (in chars)
   unsigned    txtY; // Height of screen (in chars)

   unsigned    resX; // Width of screen (in pixels)
   unsigned    resY; // Height of screeb (in pixels)

   unsigned    depth; // Number of colors (in bit)
} t_vscr;

typedef  struct t_vkeyb { // Virtual Keyboard
    l_int     code;       // Key's code
    l_int     shifts;     // Shifts state
    l_char    keychar;    // Key ascii code
    l_dword   state;      // Actual keyboad state
} t_vkeyb;

typedef struct t_vmouse { // Virtual mouse
    t_point   where;      // Position of mouse
    l_dword   state_last; // Last mouse state
    l_char    buttons;    // Number of buttons
    l_dword   state;      // Actual mouse stat
} t_vmouse;

typedef struct t_console {   // Console object

  struct t_view    obclass;  // Object/View definition
  struct t_vkeyb   keyboard; // Virtual keyboard
  struct t_vmouse  mouse;    // Virtual mouse
  struct t_vscr    scr;      // Virtual screen

} t_console;


#define src_ch(o) o->txtY
#define src_cw(o) o->txtX

FONT *def_font; // Default font used for console
////////////////////////////////////////////////////////////////////////////////

int VGA_SetMode(unsigned Xres,unsigned Yres,unsigned Depth);
int VGA_GetMode(unsigned*Height,unsigned*Width,unsigned*Depth);
void VGA_Exit(void);
//void VGA_SetPalette(PALETTEENTRY*pal,int start,int len);
//void VGA_SetQuadPalette(RGBQUAD*color,int start,int len);
//LPSTR VGA_Lock(unsigned*Pitch,unsigned*Height,unsigned*Width,unsigned*Depth);
void VGA_Unlock(void);

/* text mode */
int VGA_SetAlphaMode(unsigned Xres,unsigned Yres);
void VGA_GetAlphaMode(unsigned*Xres,unsigned*Yres);
void VGA_SetCursorPos(unsigned X,unsigned Y);
void VGA_GetCursorPos(unsigned*X,unsigned*Y);
void VGA_WriteChars(unsigned X,unsigned Y,unsigned ch,int attr,int count);

/* control
void CALLBACK VGA_Poll(ULONG_PTR arg);
void VGA_ioport_out(WORD port, BYTE val);
BYTE VGA_ioport_in(WORD port);
void VGA_Clean(void);*/


////////////////////////////////////////////////////////////////////////////////
void src_set_mode ( p_console o, unsigned resX, unsigned resY, unsigned depth, l_bool mode ) {

  o->scr.mode = mode;
  o->scr.font = def_font;
  o->scr.fntX = text_length(o->scr.font,"Aa") / 2;
  o->scr.fntY = text_height(o->scr.font);

  if ( mode == TEXT_MODE ) {

    o->scr.txtX = resX;
    o->scr.txtY = resY;
    o->scr.resX = o->scr.txtX*o->scr.fntX;
    o->scr.resY = o->scr.txtY*o->scr.fntY;

  } else {

    o->scr.resX = resX;
    o->scr.resY = resY;
    o->scr.txtX = o->scr.resX/o->scr.fntX;
    o->scr.txtY = o->scr.resY/o->scr.fntY;

  };

  o->scr.cur_tcol =  BLACK << 4 | WHITE;

  o->scr.cur_col = 0;
  o->scr.cur_lgn = 0;

  o->scr.penX = 0;
  o->scr.penY = 0;

  o->scr.bmp = create_bitmap(o->scr.resX,o->scr.resY);

  rectfill(o->scr.bmp,0,0,o->scr.resX,o->scr.resY,COLOR(o->scr.cur_tcol >> 4));

  o->scr.text = _malloc(o->scr.txtX*o->scr.txtY*sizeof(l_char));
  o->scr.tcol = _malloc(o->scr.txtX*o->scr.txtY*sizeof(l_char));

};

////////////////////////////////////////////////////////////////////////////////
void src_color ( p_console con, l_int col ) { // Set the cursor color
  con->scr.cur_tcol = col;
};

////////////////////////////////////////////////////////////////////////////////
void src_add_lgn ( p_console con ) { // Add a line in console's screen (\n)
  con->scr.cur_lgn++;
  con->scr.cur_col = 0;
  if ( con->scr.cur_lgn == con->scr.txtY ) {

    l_int a = 1;
    while ( a < con->scr.txtY ) {
      l_int b = 0;
      while ( b < con->scr.txtX ) {
        con->scr.text[((a-1)*con->scr.txtX)+b] = con->scr.text[((a)*con->scr.txtX)+b];
        b++;
      };
      a++;
    };

    blit(con->scr.bmp,con->scr.bmp,0,con->scr.fntY,0,0,con->scr.resX,con->scr.resY-con->scr.fntY);
    rectfill(con->scr.bmp,0,con->scr.resY-con->scr.fntY,con->scr.resY,con->scr.resY,COLOR(con->scr.cur_tcol >> 4));


    con->scr.cur_lgn--;
  };

};
////////////////////////////////////////////////////////////////////////////////
void src_print_char ( p_console con, char c ) { // Print a char where the cursor is (on console)
  if ( c == '\n'  ) { // NL/LF
    src_add_lgn(con);
  } else if ( c == '\r' ) { // CR
    con->scr.cur_col = 0;
  } else if ( c == '\b' ) { // BS
    if ( con->scr.cur_col > 0 ) con->scr.cur_col--;
  } else if ( c == '\a' ) { // BEL
    sound(1000);delay(30);nosound();
  } else if ( c == '\t' ) { // HT
    con->scr.cur_col += (con->scr.cur_col/8)*8+8;
    if ( con->scr.cur_col > con->scr.txtX ) src_add_lgn(con);
  } else if ( c == '\v' ) { // VT
    l_int a = 0;
    while ( a < 5 ) {
      src_add_lgn(con);
      a++;
    };
  } else if ( c == '\f' ) { // FF -> printer ...

  } else if ( c == '\0' ) { // NUL -> not drawed

  } else {

    //l_text txt = set_format_text(NULL,"%c",c);
    l_char txt[] = { c, 0 };
    l_char bcol = 0;
    l_char fcol = 0;

    bcol = con->scr.cur_tcol >> 4;
    fcol = con->scr.cur_tcol - ( bcol << 4 );

    textout_draw_rect( con->scr.bmp, con->scr.font, &txt, -1,
                       con->scr.cur_col * con->scr.fntX,
                       con->scr.cur_lgn * con->scr.fntY,
                       con->scr.cur_col * con->scr.fntX + con->scr.fntX,
                       con->scr.cur_lgn * con->scr.fntY + con->scr.fntY,
                       TX_ALIGN_CENTER, COLOR(fcol), COLOR(bcol), 1);

    //_free(txt);
    con->scr.text[(con->scr.cur_lgn*con->scr.txtX)+con->scr.cur_col] = c;
    con->scr.tcol[(con->scr.cur_lgn*con->scr.txtX)+con->scr.cur_col] = con->scr.cur_tcol;
    con->scr.cur_col++;

    if ( con->scr.cur_col == con->scr.txtX ) src_add_lgn(con);

  };

};
////////////////////////////////////////////////////////////////////////////////
void src_print ( p_console con, l_text text ) { // Print a string on console

  l_int a = 0;
  l_int l = strlen(text);

  _while ( a < l ) {
    src_print_char ( con, text[a] );
    a++;
  };

  view_draw_view(VIEW(con));
};

////////////////////////////////////////////////////////////////////////////////

void console_translate_event ( p_object o, t_event *event ) {

RETVIEW(o,event); // Is the event realy for me ?

if ( event->type & EV_KEYBOARD ) { // Keyboard event
  p_console con = ((p_console)(o));

  con->keyboard.code    = keyb->code;
  con->keyboard.shifts  = keyb->shifts;
  con->keyboard.keychar = keyb->keychar;
  con->keyboard.state   = keyb->obclass.state;
};

if ( event->type & EV_MOUSE ) { // Mouse event
  p_console con = ((p_console)(o));

  con->mouse.where      = VIEW(o)->get_local_point(VIEW(o), mouse->where);
  con->mouse.state_last = mouse->state_last;
  con->mouse.buttons    = mouse->buttons;
  con->mouse.state      = mouse->obclass.state;

};


};

////////////////////////////////////////////////////////////////////////////////
void draw_console ( p_view o ) {

 t_rect  r = o->get_local_extent(o);
 t_point p;

 BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {
    p_console con = ((p_console)(o));

    blit( con->scr.bmp, out, 0, 0, p.x, p.y,  con->scr.resX, con->scr.resY );

  };

  o->end_of_paint(o, r);

};
////////////////////////////////////////////////////////////////////////////////

p_console _console_init ( p_console o, t_rect r ) {

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_console));

  src_set_mode(o,80,25,4,TEXT_MODE);

  r.b.x = r.a.x + o->scr.resX;
  r.b.y = r.a.y + o->scr.resY;



  view_init(VIEW(o), r);

  OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
  OBJECT(o)->set_options(OBJECT(o), OB_OF_STILLPROCESS, true);

  VIEW(o)->draw = &draw_console;
  OBJECT(o)->translate_event = &console_translate_event;

  return o;

};
////////////////////////////////////////////////////////////////////////////////
p_console app_con = NULL;

p_console _load_console ( p_console mcon ) {
  t_rect r;
  //p_appwin win = NULL;
  p_window win = NULL;
  p_console con = _console_init( mcon ,rect_assign(3,20,0,0));

  r = VIEW(con)->bounds;

  win = win_init( _malloc(sizeof(t_window)),
                  rect_assign(0,0,r.b.x+3,r.b.y+3),
                  "Console",
                  NULL );

  obj_insert(OBJECT(desktop),OBJECT(win));
  obj_insert(OBJECT(win),OBJECT(con));
  return con;
};

#define load_console() app_con = _load_console(_malloc(sizeof(t_console)))
#define print(txt) src_print(app_con,txt)

////////////////////////////////////////////////////////////////////////////////
void init_console(void){
    l_int a = 1;

    def_font =  (FONT*) get_font_in_size ( "console", 16, 16 ); // Load the default font

    load_console(); // Show the console

    print("Say welcome to the new console system, it will support vga hardware emulation, in this way we will be able to make a dos VM in a seal's window !\n\nBut this is only a console not a VM !\n");
    print("∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞\n");
    print("∞±±±±±±±±±±±±±±±±±±±±±±±±±±±∞\n");
    print("∞±≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤±∞\n");
    print("∞±≤€€€€€€€€€€€€€€€€€€€€€€€≤±∞\n");
    print("∞±≤€ SEAL CONSOLE SYSTEM €≤±∞\n");
    print("∞±≤€€€€€€€€€€€€€€€€€€€€€€€≤±∞\n");
    print("∞±≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤≤±∞\n");
    print("∞±±±±±±±±±±±±±±±±±±±±±±±±±±±∞\n");
    print("∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞\n");
};

SetInfoAppName("Console");
SetInfoDesciption("Console test");
SetInfoCopyright("(c) Copyright 2001 Point Mad. All rights reserved");
SetInfoManufacturer("Point Mad");
////////////////////////////////////////////////////////////////////////////////
app_begin ( void ) {


  if ( ap_process == AP_INIT ) {

    init_console();

  };

  if ( ap_process == AP_DONE ) {

  };

} app_end;
