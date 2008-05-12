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

/* SEAL Tetris 1.0
 * By Callum Lawson <callum@pmad.net>
 */

/* Revision History (as of 04/04/2002):
 *
 * 04/04/2002: Updated with new icon (orudge)
 */

#include <seal.h>
#include <menus.h>
#include <app.h>
#include <button.h>
#include <dialogs.h>
#include <iodlg.h>
#include "tetris.h"

SetInfoAppName("Tetris");
SetInfoDesciption("A very famous game!");
SetInfoCopyright("(c) Copyright 2001 SealSystem. All rights reserved");
SetInfoManufacturer("Callum Lawson");

#define MSG_GAMEOVER	100002


char figures[19][4][4]=
{{"X...",
  "X...",
  "X...",
  "X..."},
 {"XXXX",
  "....",
  "....",
  "...."},
 {"XXX.",
  "X...",
  "....",
  "...."},
 {"X...",
  "X...",
  "XX..",
  "...."},
 {"..X.",
  "XXX.",
  "....",
  "...."},
 {"XX..",
  ".X..",
  ".X..",
  "...."},
 {"XXX.",
  "..X.",
  "....",
  "...."},
 {"XX..",
  "X...",
  "X...",
  "...."},
 {"X...",
  "XXX.",
  "....",
  "...."},
 {".X..",
  ".X..",
  "XX..",
  "...."},
 {".X..",
  "XXX.",
  "....",
  "...."},
 {".X..",
  "XX..",
  ".X..",
  "...."},
 {"XXX.",
  ".X..",
  "....",
  "...."},
 {"X...",
  "XX..",
  "X...",
  "...."},
 {"X...",
  "XX..",
  ".X..",
  "...."},
 {".XX.",
  "XX..",
  "....",
  "...."},
 {".X..",
  "XX..",
  "X...",
  "...."},
 {"XX..",
  ".XX.",
  "....",
  "...."},
 {"XX..",
  "XX..",
  "....",
  "...."}};

int shift[19]={1,0,3,4,5,2,7,8,9,6,11,12,13,10,15,14,17,16,18};

int pot2[8]={128,64,32,16,8,4,2,1};

int board[10][15];
int ax,ay,akt,n;

l_bool isvalid (int x, int y, int shape)
{
 int i,j;
 for (i=0;i<4;i++)
  for (j=0;j<4;j++)
   if((figures[shape][i][j]=='X')&&((i+x>=10)||(j+y>=15)))
    return false;
 for (i=0;i<4;i++)
  for (j=0;j<4;j++)
   if((i+x<10)&&(j+y<15)&&(figures[shape][i][j]=='X')&&(board[i+x][j+y]!=0))
    return false;
 return true;
};

void tetris_draw (p_view o)
{
  t_rect  r = o->get_local_extent(o);
  t_point p;
  BITMAP *out = o->begin_paint(o, &p, r);
  void pr (int x, int y)
   {
    rectfill (out,p.x+10+20*(x-1),p.y+10+20*(y-1),p.x+8+20*x,p.y+8+20*y,makecol(191,31,0));
   };
  if ( out ) {
  int i,j;
  rectfill (out,p.x+r.a.x,p.y+r.a.y,p.x+r.b.x,p.y+r.b.y,COLOR(CO_BLUE));
  for (i=1;i<=10;i++)
   for (j=1;j<=15;j++)
    if(board[i-1][j-1]!=0)
     pr(i,j);
  };
  o->end_of_paint(o, r);
};

void ins (int shape, int x, int y)
{
 int i,j;
 for (i=0;i<4;i++)
  for (j=0;j<4;j++)
   if(figures[akt][i][j]=='X')
    board[x+i][y+j]=1;
};

void del (int shape, int x, int y)
{
 int i,j;
 for (i=0;i<4;i++)
  for (j=0;j<4;j++)
   if(figures[akt][i][j]=='X')
    board[x+i][y+j]=0;
};

l_bool full(int j)
{
 int i;
 for(i=0;i<10;i++)
  if(board[i][j]!=1) return false;
 return true;
}

void kasuj(void)
{
 int i,j,k;
 for (j=0;j<15;j++)
  if(full(j))
   for(k=j;k>0;k--)
    for(i=0;i<10;i++)
     board[i][k]=board[i][k-1];
}

l_bool gameover(void)
{
 int i;
 for(i=0;i<19;i++)
  if(!isvalid(0,0,i)) return true;
 return false;
}

static void callback (p_object s)
{
 n++; if(n==5)n=0;
 ins(akt,ax,ay);
 VIEW(s)->draw_view(VIEW(s));
 del(akt,ax,ay);
 if(n==0){
 if(isvalid(ax,ay+1,akt))
  {ay++;}
 else
  {ins(akt,ax,ay);ay=0;ax=0;akt=random()%19;
   if(gameover())
    {
 /* hang Seal trying to pass the close event to the application...
    to be fixed */
     p_event ev;
     clear_event(ev);
     set_event(ev, EV_MESSAGE, MSG_GAMEOVER, s);
     s->put_event(s,ev);
     clear_event(ev);
    };
  };
 kasuj();
 }
}

p_view tetris_init (p_view o, t_rect r)
{
 if ( !o ) return NULL;

 view_init(o, r);

 OBJECT(o)->process_tick = 50;
 OBJECT(o)->func_callback = &callback;

 o->draw = &tetris_draw;

 OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
 OBJECT(o)->set_options(OBJECT(o), OB_OF_STILLPROCESS, true);

 VIEW(o)->brush.color = COLOR(CO_BLACK);

 return o;
};

static p_menu tetris_menu_init(void)
{
 p_menu menu = new_menu(
                new_sub_menu("Game",new_menu(
                  new_menu_item("About...", "", 0, MSG_ABOUT, NULL,
 			new_menu_item("Quit", "ALT+F4", TO_ALT(KB_F4), MSG_QUIT, NULL,
                NULL))),
               NULL));
 return menu;
}

void trans_ev (p_object o, p_event event)
{
 if (event->type & EV_KEYBOARD) {
  if (OBJECT(keyb)->state & KB_SF_KEYDOWN) {
   switch (KEY_TO(keyb->code)) {
    case KB_RIGHT:
     if(isvalid(ax+1,ay,akt)) ax++;
     clear_event(event);
     break;
    case KB_LEFT:
     if(isvalid(ax-1,ay,akt)) ax--;
     clear_event(event);
     break;
    case KB_UP:
     if(isvalid(ax,ay,shift[akt])) akt=shift[akt];
     clear_event(event);
     break;
    case KB_DOWN:
     if(isvalid(ax,ay+1,akt)) ay++;
     clear_event(event);
     break;
   }
  }
 }

 if (event->type == EV_MESSAGE) {
    switch (event->message) {
      case MSG_ABOUT: {
//         msgbox(MW_INFO, MB_OK, "         SEAL Tetris Game 2.0 for SEAL\n           by Callum Lawson <callum@pmad.net>\n\n       Copyright(c) SealSystem 2001\n            All Rights Reserved.\n\nThe simple game of Tetris!");
         seal2_app_about("Tetris", load_image("/system/bmp/tetris.ico,32"));
         clear_event(event);
       } break;

       case MSG_GAMEOVER: {
         p_event ev;
         msgbox(MW_INFO, MB_OK, "Game Over!");
         clear_event(ev);
         set_event(ev, EV_MESSAGE, MSG_CLOSE, o);
         o->put_event(o, ev);
         clear_event(ev);
       } break;

    }
  }

}

void welc_draw (p_view o)
{
  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {
   int x,y,o; unsigned char c,d;
   o=0;
   for(y=0;y<85;y++)
    for(x=0;x<21;x++)
     {d=picture[o++];
      for(c=0;c<8;c++)
       if(d>=pot2[c]){d=d-pot2[c]; putpixel(out,p.x+8*x+c+20,p.y+y+10,COLOR(CO_BLACK));};
     };
  };

  o->end_of_paint(o, r);

};

p_view init_welcome(p_view o, t_rect r)
{
 if (!o) return NULL;
 view_init(o, r);
 o->draw = &welc_draw;
 return o;
};

p_object get_button(t_rect r, l_text str, l_dword msg, l_int flags)
{
 p_button  o = button_init(_malloc(sizeof(t_button)),r,str,msg,flags);
 return OBJECT(o);
}

void tetris_init_window (void)
{
 p_menuview menu;
 p_window hi;
 t_rect r,t;
 p_appwin w;
 int i,j;

 menu = NULL;

 r = rect_assign(200, 200, 400, 340);
 hi = win_init(malloc(sizeof(t_window)),r,TXT_WELCOME,0);
 r = VIEW(hi)->size_limits(VIEW(hi));
 r = rect_assign(r.a.x,r.a.y,r.b.x,r.b.y-20);
 OBJECT(hi)->insert(OBJECT(hi),OBJECT(init_welcome(_malloc(sizeof(t_view)),r)));
 r = VIEW(hi)->size_limits(VIEW(hi));
 r = rect_assign(r.a.x+50,r.b.y-19,r.b.x-50,r.b.y);
 OBJECT(hi)->insert(OBJECT(hi),get_button(r,"Start",MSG_CLOSE,BF_NORMAL));
 desktop->execute_view(desktop,VIEW(hi));

 r = rect_assign(100, 100, 330, 465);
 t = r;
 w = appwin_init(malloc(sizeof(t_appwin)),
                          r,
                          "Seal Tetris",
                          0,
                          ap_id,
                          &trans_ev);

 w->icon16 = load_image("/system/bmp/tetris.ico,16");

 for (i=0;i<10;i++)
  for (j=0;j<15;j++)
   board[i][j]=0;
 akt = random()%19; ax=0; ay=0; n=0;
 ins(akt,ax,ay);
 OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(w));
 r = VIEW(w)->size_limits(VIEW(w));
 r = rect_assign(r.a.x,r.a.y+1,r.b.x,r.a.y+20);
 menu = hormenu_init(_malloc(sizeof(t_menuview)),r,tetris_menu_init());
 OBJECT(w)->insert(OBJECT(w),OBJECT(menu));
 r = VIEW(w)->size_limits(VIEW(w));
 r = rect_assign(r.a.x,r.a.y+21,r.b.x,r.b.y);
 OBJECT(w)->insert(OBJECT(w), OBJECT(tetris_init(_malloc(sizeof(t_view)), r)));
};

app_begin (void) {
 switch(ap_process)
 {
  case AP_ALLOC:
   break;
  case AP_INIT:
   AP_SETNUMOFCALLS(1);
   tetris_init_window();
   break;
  case AP_FREE:
   break;
  case AP_DONE:
   break;
 }
} app_end;
