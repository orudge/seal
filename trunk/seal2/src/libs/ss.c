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

/* Revision History:
 *
 * 29/03/2002 - Added automatic resolution changing (orudge)
 * 01/04/2002 - Disabled automatic resolution changing due to some problems
                (orudge)
 */

#include <seal.h>
#include <cp.h>
#include <grfx.h>
#include <button.h>
#include <dialogs.h>
#include <stattext.h>
#include <screen.h>
#include <colors.h>
#include <checkbox.h>

p_history  size   = NULL;
p_history  depth  = NULL;
p_grfx     preview = NULL;
p_checkbox is_2dn_buf = NULL;
l_bool changed = false;

#define MSG_HISUPDATE  10002

//BITMAP *my_bmp = NULL;

extern l_int screen_viswidth;
extern l_int screen_visheight;
extern l_int screen_depth;

////////////////////////////////////////////////////////////////////////////////

l_long p_w[] = { 640, 800, 1024 , NULL };
l_long p_h[] = { 480, 600, 768 , NULL };
l_long p_d[] = { 8, 16, 24, 32 , NULL };
////////////////////////////////////////////////////////////////////////////////
void set_save(void)
{
   if (changed)
   {
      msgbox(MW_INFO, MB_OK, "You must restart SEAL to use the new screen settings.");
      set_key_integer("system/graphics/depth",p_d[depth->current]);
      set_key_integer("system/graphics/height",p_h[size->current]);
      set_key_integer("system/graphics/width",p_w[size->current]);
      set_key_integer("system/graphics/double_buffer",(is_2dn_buf->state&CB_CHECKED)?1:0);

/*      remove_mouse();    // remove mouse driver
      screen_halt();

      screen_viswidth = p_w[size->current];
      screen_visheight = p_h[size->current];
      screen_width = p_w[size->current];
      screen_height = p_h[size->current];
      screen_depth = p_d[size->current];

      screen_reload();

      install_mouse();                                     // reinstall mouse driver
      set_mouse_speed(mouse->speed.x, mouse->speed.y);

      mouse_set_sys_cursor(CUR_ARROW);
      mouse_show(mouse);*/

      changed = false;
   }
}
////////////////////////////////////////////////////////////////////////////////
/*void   preview_draw ( p_view o )
{
 t_rect  r = o->get_local_extent(o);
 t_point p;

 BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    rectfill(out,p.x,p.y,p.x+r.b.x,p.y+r.b.y,color_3d_face);
    if ( my_bmp ) draw_sprite(out, my_bmp, p.x+(r.b.x-IMAGE_WIDTH(my_bmp))/2, p.y+(r.b.y-IMAGE_HEIGHT(my_bmp))/2);

  };

  o->end_of_paint(o, r);
};*/

////////////////////////////////////////////////////////////////////////////////
void set_trans_ev( p_object o, t_event *event ) {

  if ( o->process == PH_PREPROCESS && event->type & EV_MESSAGE )
    if ( event->message == MSG_HISUPDATE ) {
      changed = true;
      clear_event(event);
    };

};
////////////////////////////////////////////////////////////////////////////////
static p_list size_list ( void ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);

  if ( p ) {

     p->insert(p, new_history_item("640x480", NULL, 0, NULL));
     p->insert(p, new_history_item("800x600", NULL, 0, NULL));
     p->insert(p, new_history_item("1024x768", NULL, 0, NULL));

  };

  return p;
};
////////////////////////////////////////////////////////////////////////////////
static p_list depth_list ( void ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);

  if ( p ) {

     p->insert(p, new_history_item("256 colours (8-bit)", NULL, 0, NULL));
     p->insert(p, new_history_item("High colour (16-bit)", NULL, 0, NULL));
     p->insert(p, new_history_item("True colour (24-bit)", NULL, 0, NULL));
     p->insert(p, new_history_item("True colour (32-bit)", NULL, 0, NULL));

  };

  return p;
};
////////////////////////////////////////////////////////////////////////////////
void set_load ( p_object o ) {
  t_rect       r = VIEW(o)->get_local_extent(VIEW(o));
  l_int        a = r.b.y - (3*25);
  p_stattext lbl = NULL;
  p_button     b = NULL;

  lbl  =    stattext_init( _malloc(sizeof(t_stattext)),
                         rect_assign( 5, a, 120, a + 20 ),
                         TX_ALIGN_CENTERY,
                         TXT_SCREEN_SIZE);
  o->insert(o,OBJECT(lbl));

  lbl  =    stattext_init( _malloc(sizeof(t_stattext)),
                         rect_assign( 5, a + 25, 120, a + 45 ),
                         TX_ALIGN_CENTERY,
                         TXT_SCREEN_DEPTH);
  o->insert(o,OBJECT(lbl));


  size  = history_init( _malloc(sizeof(t_history)),
                         rect_assign( 125, a, r.b.x - 20, a + 20 ),
                         size_list() ,
                         150,
                         HF_REWRITEUNABLE|LF_SELFLIST);

  depth  = history_init( _malloc(sizeof(t_history)),
                         rect_assign( 125, a+25, r.b.x - 20, a + 45 ),
                         depth_list() ,
                         150,
                         HF_REWRITEUNABLE|LF_SELFLIST);

  is_2dn_buf = checkbox_init(_malloc(sizeof(t_checkbox)),rect_assign( 5, a+50, r.b.x , a + 65 ), TXT_SCREEN_DBL_BUFFER_INFO,(get_key_integer("system/graphics/double_buffer"))?CB_CHECKED:NULL,MSG_HISUPDATE);



  /*preview  =    grfx_init( _malloc(sizeof(t_grfx)),
                           rect_assign( 50, 5, r.b.x - 50, a - 5 ));

  VIEW(preview)->draw = &preview_draw;*/

  switch ( get_key_integer("system/graphics/depth") ) {
    case 8  : depth->current = 0; break;
    case 16 : depth->current = 1; break;
    case 24 : depth->current = 2; break;
    case 32 : depth->current = 3; break;
  };

  switch ( get_key_integer("system/graphics/width") ) {
    case 640 : size->current = 0; break;
    case 800 : size->current = 1; break;
    case 1024 : size->current = 2; break;
  };

  size->message = MSG_HISUPDATE;
  depth->message = MSG_HISUPDATE;

  o->insert(o,OBJECT(size));
  o->insert(o,OBJECT(depth));
  //o->insert(o,OBJECT(preview));
  o->insert(o,OBJECT(is_2dn_buf));

};


////////////////////////////////////////////////////////////////////////////////
SetInfoAppName("Screen Settings");
SetInfoDesciption("Sub Program of control panel");
SetInfoCopyright("(c) Copyright 2001 Point Mad. All rights reserved");
SetInfoManufacturer("Point Mad");

app_begin ( void ) {


  if ( ap_process == AP_INIT ) {

     //my_bmp = load_image("ss.bmp");

     //conv_to_skipcolor_bitmap(my_bmp,255,0,255);

     add_cp_loader(TXT_SCREEN_DEFINITION,NULL,cps_hardware,NULL,NULL,&set_load,&set_trans_ev,&set_save);
  };

} app_end;
