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
#include <cp.h>
#include <grfx.h>
#include <button.h>
#include <dialogs.h>
#include <iodlg.h>
#include <inputbox.h>
////////////////////////////////////////////////////////////////////////////////


l_text colors_name[max_ctrls_col] = {
  "flat face", "flat border", "flat text",
  "3d face", "3d light", "3d shadow",
  "3d dark", "3d border", "3d text", "selected face",
  "selected text", "desktop face", "desktop text",
  "menu face", "menu face gardient", "menu text",
  "menu selected face", "menu selected face gardient",
  "menu selected text", "deskbar face", "deskbar light",
  "deskbar shadow", "deskbar dark", "deskbar border",
  "deskbar text", "deskbar selected face",
  "deskbar selected text", "window active title text",
  "window active title face", "window active title face gardient",
  "window passive title text", "window passive title face",
  "window passive title face gardient", NULL };

l_color colors[max_ctrls_col] = { NULL };

#define col_flat_face                      colors[0]
#define col_flat_border                    colors[1]
#define col_flat_text                      colors[2]
#define col_3d_face                        colors[3]
#define col_3d_light                       colors[4]
#define col_3d_shadow                      colors[5]
#define col_3d_dark                        colors[6]
#define col_3d_border                      colors[7]
#define col_3d_text                        colors[8]
#define col_selected_face                  colors[9]
#define col_selected_text                  colors[10]
#define col_desktop_face                   colors[11]
#define col_desktop_text                   colors[12]
#define col_menu_face                      colors[13]
#define col_menu_face_gardient             colors[14]
#define col_menu_text                      colors[15]
#define col_menu_selected_face             colors[16]
#define col_menu_selected_face_gardient    colors[17]
#define col_menu_selected_text             colors[18]
#define col_deskbar_face                   colors[19]
#define col_deskbar_light                  colors[20]
#define col_deskbar_shadow                 colors[21]
#define col_deskbar_dark                   colors[22]
#define col_deskbar_border                 colors[23]
#define col_deskbar_text                   colors[24]
#define col_deskbar_selected_face          colors[25]
#define col_deskbar_selected_text          colors[26]
#define col_window_act_title_text           colors[27]
#define col_window_act_title_face           colors[28]
#define col_window_act_title_face_gardient  colors[29]
#define col_window_pass_title_text          colors[30]
#define col_window_pass_title_face          colors[31]
#define col_window_pass_title_face_gardient colors[32]

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void save_color ( l_color col, l_text name ) {

  l_text kkey = key_in_path("current/seal/colors",name);

  l_text value = set_format_text(NULL,"%d,%d,%d",getr(col),getg(col),getb(col));

  if ( !key_exists( kkey ) ) create_key("current/seal/colors",name);


  set_key( kkey, value );

  _free(kkey); _free(value);

};
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void set_save( void ) {
  l_int a = 0;
  while ( ctrls_reg[a] ) {
    set_registry_color( colors[a], ctrls_reg[a] );
    ctrls_colors[a] = colors[a];
    a++;
  };
  desktop->brush.color = color_desktop_face;
  VIEW(desktop)->draw_view(VIEW(desktop));
};
////////////////////////////////////////////////////////////////////////////////
void _button3d(BITMAP *bmp, int x1, int y1, int x2, int y2, int down)
{
  if (down == 0) {
    line(bmp, x1, y1, x2, y1, col_3d_light);
    line(bmp, x1, y1, x1, y2, col_3d_light);
    line(bmp, x2-1, y1+1, x2-1, y2-1, col_3d_shadow);
    line(bmp, x1+1, y2-1, x2-1, y2-1, col_3d_shadow);
    line(bmp, x2, y1, x2, y2, col_3d_dark);
    line(bmp, x1, y2, x2, y2, col_3d_dark);
  } else {
    line(bmp, x1, y1, x2, y1, col_3d_dark);
    line(bmp, x1, y1, x1, y2, col_3d_dark);
    line(bmp, x1+1, y1+1, x2-1, y1+1, col_3d_shadow);
    line(bmp, x1+1, y1+1, x1+1, y2-1, col_3d_shadow);
    line(bmp, x2-1, y1+1, x2-1, y2-1, col_3d_face);
    line(bmp, x1+1, y2-1, x2-1, y2-1, col_3d_face);
    line(bmp, x2, y1, x2, y2, col_3d_light);
    line(bmp, x1, y2, x2, y2, col_3d_light);
  };
};
////////////////////////////////////////////////////////////////////////////////
void draw_vscroll_bar ( BITMAP *out, t_point p, t_rect r ) {
  l_int   minush = 8;
  l_int   plush  = 8;
  GrFONT *font   = get_font_in_size("Symbols", 8, 8);
  rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, col_3d_face);
  button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, col_3d_light, col_3d_shadow);
  draw_double_text(out, font, "f", -1, r.a.x+p.x+1, r.a.y+p.y,
                       r.b.x+p.x, r.a.y+p.y+minush, TX_ALIGN_CENTER, col_3d_light, col_3d_shadow, 0);
  draw_double_text(out, font, "g", -1, r.a.x+p.x+1, r.b.y+p.y-plush,
                       r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTER, col_3d_light, col_3d_shadow, 0);
};
////////////////////////////////////////////////////////////////////////////////
void draw_vscroll ( BITMAP *out, t_point p, t_rect r ) {

  rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, col_3d_face);
  button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, col_3d_shadow, col_3d_light);
  draw_vscroll_bar(out,p, rect_assign(r.a.x+1,r.a.y+((r.b.y-r.a.y) / 2) - ((r.b.y-r.a.y) / 4),r.b.x-1,r.a.y+((r.b.y-r.a.y) / 2) + ((r.b.y-r.a.y) / 4) ));

};
////////////////////////////////////////////////////////////////////////////////
void draw_editor ( BITMAP *out, t_point p, t_rect r, l_font *font ) {
  r.b.x-=16;
  rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, col_flat_face);

  button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, col_3d_shadow, col_3d_light);

  textout_draw_rect(out, font, "Welcome to the system colour selector", -1, r.a.x+p.x+2, r.a.y+p.y+2,
                             r.b.x+p.x-2, r.b.y+p.y-2, TX_ALIGN_DEFAULT, col_flat_text, TX_NOCOLOR, 0);

  textout_draw_rect(out, font, "This is selected!", -1, r.a.x+p.x+2, r.a.y+p.y+2+FONT_GETHEIGHT(font)+2,
                             r.b.x+p.x-2, r.b.y+p.y-2, TX_ALIGN_DEFAULT, col_selected_text, col_selected_face, 0);

  /*textout_draw_rect(out, font, "(c) 2001 Julien Etelain <julien@pmad.net>", -1, r.a.x+p.x+2, r.a.y+p.y+2+(FONT_GETHEIGHT(font)+2)*2,
                             r.b.x+p.x-2, r.b.y+p.y-2, TX_ALIGN_DEFAULT, col_flat_text, TX_NOCOLOR, 0);*/

  r.a.x = r.b.x+3;
  r.b.x = r.a.x+13;

  draw_vscroll (out,p, r );

};
////////////////////////////////////////////////////////////////////////////////
void draw_boutton ( BITMAP *out, t_point p, t_rect r, l_text caption, l_int press, l_font *font ) {

      rectfill(out,p.x+r.a.x,p.y+r.a.y,p.x+r.b.x,p.y+r.b.y,col_3d_face);


      textout_draw_rect(out, font, caption, -1, r.a.x+p.x+2, r.a.y+p.y+2,
                             r.b.x+p.x-2, r.b.y+p.y-2, TX_ALIGN_CENTER, col_3d_text, TX_NOCOLOR, 0);


      _button3d( out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, ( press == 1 ) ? 1 : 0 );

};
////////////////////////////////////////////////////////////////////////////////
void draw_window ( BITMAP *out, t_point p, t_rect br, l_text caption, l_font *font, l_int win_selected ) {

  if ( 1 ) {
    t_rect  r = br;

    rectfill(out,p.x+r.a.x,p.y+r.a.y,p.x+r.b.x,p.y+r.b.y,col_3d_face);

    button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, col_3d_face, col_3d_dark);
    button(out, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x-1, r.b.y+p.y-1, col_3d_light, col_3d_shadow);

  };

  if ( 1 ) {
    t_rect  r = rect_assign(br.a.x+2, br.a.y+2, br.b.x-2,
                            br.a.y+FONT_GETHEIGHT(font)+3);

    l_color fcolor = col_window_pass_title_text;

    l_color bcolor1 = col_window_pass_title_face;
    l_color bcolor2 = col_window_pass_title_face_gardient;

    if ( win_selected ) {

      fcolor = col_window_act_title_text;
      bcolor1 = col_window_act_title_face;
      bcolor2 = col_window_act_title_face_gardient;
    };

    rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, col_3d_face);

    fade_rect(out, r.a.x+p.x+1, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, bcolor1, bcolor2, FR_HOR);

    textout_draw_rect(out, font, caption, -1, r.a.x+p.x, r.a.y+p.y+1,
                           r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTER, fcolor, TX_NOCOLOR, 0);
  };
};
////////////////////////////////////////////////////////////////////////////////
void   preview_draw ( p_view o )
{
  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {
    t_rect w1 = rect_assign(5,5, r.b.x - 15, r.b.y - 15);
    t_rect w2 = rect_assign(10, 5 + FONT_GETHEIGHT(o->font)+3+3, r.b.x - 5, r.b.y -5 );
    t_rect b1 = rect_assign(15,r.b.y-30,115,r.b.y-10);
    t_rect b2 = rect_assign(120,r.b.y-30,220,r.b.y-10);
    t_rect ed = rect_assign( w2.a.x + 3,w2.a.y + (FONT_GETHEIGHT(o->font)*2) + 3+3+3 ,w2.b.x-3,w2.b.y-32);

    rectfill( out, p.x, p.y, p.x+r.b.x, p.y+r.b.y, col_desktop_face );

    draw_window(out,p,w1,"Inactive window",o->font,0);
    draw_window(out,p,w2,"Active window",o->font,1);
    draw_boutton(out,p,b1,TXT_OK,2,o->font);
    draw_boutton(out,p,b2,TXT_CANCEL,0,o->font);
    draw_editor(out,p,ed,o->font);

  };

  o->end_of_paint(o, r);
};
////////////////////////////////////////////////////////////////////////////////
static p_list colors_list ( void ) {
  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);
  if ( p ) {
    l_int a = 0;
    while ( colors_name[a] ) {
      p->insert(p, new_history_item(set_format_text(NULL,"%s", colors_name[a]) ,NULL,0,NULL));
      a++;
    };
  };
  return p;
};
////////////////////////////////////////////////////////////////////////////////
p_history  col_lst = NULL;
p_grfx     preview = NULL;
#define  MSG_CCOLOR 1000080
////////////////////////////////////////////////////////////////////////////////
static p_list get_img_items(void)
{
   p_list p = list_init(_malloc(sizeof(t_list)), &free_filehistory_item, 0);

   if (p)
   {
     p->insert(p, new_filehistory_item("Windows Bitmap files (*.bmp)", "*.bmp"));
     p->insert(p, new_filehistory_item("JPEG files (*.jpg)", "*.jpg"));
     p->insert(p, new_filehistory_item("GIF files (*.gif)", "*.gif"));
     p->insert(p, new_filehistory_item("PCX files (*.pcx)", "*.pcx"));
     p->insert(p, new_filehistory_item("All files (*.*)", "*.*"));
   }

   return(p);
};
////////////////////////////////////////////////////////////////////////////////
void set_trans_ev( p_object o, t_event *event ) {

  if ( o->process == PH_PREPROCESS && event->type & EV_MESSAGE ) {
    l_dword msg = event->message;

    if ( msg == MSG_CCOLOR ) {
      l_int sel = col_lst->current;
      colors[sel] = selectcolor ( colors[sel] );
      view_draw_view(VIEW(preview));
    };

  };

};
////////////////////////////////////////////////////////////////////////////////
void load_sets ( void ) {
     l_int a = 0;
     while ( ctrls_reg[a] ) {
       colors[a] = registry_color(ctrls_reg[a]);
       a++;
     };
     a = 0;
};
////////////////////////////////////////////////////////////////////////////////
void set_load ( p_object o ) {

t_rect       r = VIEW(o)->get_local_extent(VIEW(o));
l_int        a = r.b.y - (25);
p_stattext lbl = NULL;
p_button     b = NULL;

load_sets();

lbl  =    stattext_init( _malloc(sizeof(t_stattext)),
                         rect_assign( 5, a , 100, a + 20 ),
                         TX_ALIGN_CENTER,
                         TXT_COLOR);
obj_insert(o,OBJECT(lbl));
b  =        button_init( _malloc(sizeof(t_button)),
                         rect_assign( r.b.x - 55, a , r.b.x - 5, a + 20 ),
                         TXT_CHANGE ,
                         MSG_CCOLOR,
                         BF_DEFAULT);
obj_insert(o,OBJECT(b));
col_lst  = history_init( _malloc(sizeof(t_history)),
                         rect_assign( 105, a, r.b.x - 75, a + 20 ),
                         colors_list() ,
                         150,
                         HF_REWRITEUNABLE|LF_SELFLIST);
obj_insert(o,OBJECT(col_lst));
preview  =    grfx_init( _malloc(sizeof(t_grfx)),
                         rect_assign( 5, 5, r.b.x - 5, a - 5 ));
VIEW(preview)->draw = &preview_draw;
obj_insert(o,OBJECT(preview));

};


////////////////////////////////////////////////////////////////////////////////
SetInfoAppName("System colors selector");
SetInfoDesciption("Sub Program of control panel");
SetInfoCopyright("(c) Copyright 2001 Point Mad. All rights reserved");
SetInfoManufacturer("Point Mad");

app_begin ( void ) {


  if ( ap_process == AP_INIT ) {

     add_cp_loader(TXT_SYSTEM_COLORS,NULL,cps_display,load_image("bmp/colors.ico,32"),load_image("bmp/colors.ico,16"),&set_load,&set_trans_ev,&set_save);
  };

} app_end;
