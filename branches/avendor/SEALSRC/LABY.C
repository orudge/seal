/* Laby the 1st stupid game for SEAL ! */

#include<random.h>

#include"allegro.h"
#include"seal.h"
#include"view.h"
#include"object.h"
#include"app.h"
#include"button.h"
#include"grfx.h"
#include"dialogs.h"

void draw ();

p_grfx laby = NULL;

l_long laby_dat[100][100];

l_int  where_x = 2;
l_int  where_y = 2;

void draw () {

p_view  vo = VIEW(laby);
t_rect  r = vo->size_limits(vo);
t_point pe;

BITMAP *out = vo->begin_paint(vo, &pe, r);

if (out) {

l_long x = where_x - 2;
l_long y = where_y - 2;
l_long stop_x  = where_x + 2;
l_long stop_y  = where_y + 2;

l_long vx = 0;
l_long vy = 0;
  vo->brush.color = COLOR(CO_SYSx23);

        rectfill(out, pe.x ,
                      pe.y ,
                      pe.x + r.b.x,
                      pe.y + r.b.y,
                      COLOR(CO_SYSx23));

while ( x <= stop_x ) {
 while ( y <= stop_y ) {
  l_int go = laby_dat[x][y];

  if (go == 1) {

      rectfill(out, pe.x + (vx * 60) + 1,
                    pe.y + (vy * 60) + 1,
                    pe.x + (vx * 60) + 60,
                    pe.y + (vy * 60) + 60,
                    COLOR(CO_SYSx23));
  } else if (go == 2) {

      rectfill(out, pe.x + (vx * 60) + 1,
                    pe.y + (vy * 60) + 1,
                    pe.x + (vx * 60) + 60,
                    pe.y + (vy * 60) + 60,
                    COLOR(CO_SYSx23));

      button(out,   pe.x + (vx * 60) + 20,
                    pe.y + (vy * 60) + 20,
                    pe.x + (vx * 60) + 40,
                    pe.y + (vy * 60) + 40,
                    COLOR(CO_SYSx18), COLOR(CO_WHITE));

  } else {

   rectfill(out, pe.x + (vx * 60) + 1,
                 pe.y + (vy * 60) + 1,
                 pe.x + (vx * 60) + 60,
                 pe.y + (vy * 60) + 60,
                 COLOR(CO_SYSx23));

   button(out, pe.x + (vx * 60) + 1,
               pe.y + (vy * 60) + 1,
               pe.x + (vx * 60) + 60,
               pe.y + (vy * 60) + 60,
               COLOR(CO_WHITE), COLOR(CO_SYSx18));
  };
  vy++;
  y++;
 };

y = where_y - 2;
vy = 0;
vx++;
x++;
};


   rectfill(out, pe.x + (2 * 60) + 25,
                 pe.y + (2 * 60) + 25,
                 pe.x + (2 * 60) + 35,
                 pe.y + (2 * 60) + 35,
                 COLOR(CO_LIGHTRED));

};
vo->end_of_paint(vo, r);

};

void   draw2 ( p_view o ) { draw(); };

static void trans_ev ( p_object o, p_event event ) {

  if ( o->is_state(o, OB_SF_FOCUSED) )

  if ( event->type & EV_KEYBOARD ) {

    if ( OBJECT(keyb)->state & KB_SF_KEYDOWN )

    switch ( KEY_TO(keyb->code) ) {

      case KB_RIGHT : {  /* arrow RIGHT was pressed */

      if (laby_dat[where_x + 1][where_y] != 0)
      where_x = where_x + 1;
      draw();

        clear_event(event);

      }; break;

      case KB_LEFT : {  /* arrow LEFT was pressed */
      if (laby_dat[where_x - 1][where_y] != 0)
      where_x = where_x - 1;
      draw();


        clear_event(event);

      }; break;

      case KB_DOWN : {  /* arrow DOWN was pressed */
      if (laby_dat[where_x][where_y + 1] != 0)
      where_y = where_y + 1;
      draw();


        clear_event(event);

      }; break;

      case KB_UP : {  /* arrow UP was pressed */
      if (laby_dat[where_x][where_y - 1] != 0)
      where_y = where_y - 1;
      draw();

        clear_event(event);
      }; break;

    };

  };
};

void ad( l_long x, l_long y) {  laby_dat[x][y] = 1; };


void init_window ( void )
{

   t_rect   r = rect_assign(0, 0, 310, 370);

   p_appwin p = appwin_init(_malloc(sizeof(t_appwin)),
                            r,
                            "LABY for SEAL !",
                            WF_MINIMIZE,
                            ap_id,
                            &trans_ev);


   if ( p ) VIEW(p)->align |= TX_ALIGN_CENTER;

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(p));

      if ( p ) {
      p_button b = NULL;

      r = rect_assign(3, 330, 153, 347);

      b = button_init(_malloc(sizeof(t_button)),
                            r,
                            "Bye Bye !",
                            MSG_CLOSE,
                            BF_DEFAULT);

      OBJECT(p)->insert(OBJECT(p), OBJECT(b));


      r = rect_assign(3, 25, 303, 325);

      laby = grfx_init(_malloc(sizeof(t_grfx)),r );

      VIEW(laby)->draw = &draw2;

      OBJECT(p)->insert(OBJECT(p), OBJECT(laby));
/* Default Map by julien Etelain (c) Copyright 2000 Julien Etelain. All rights reserved */
      // 2
      ad(2,2);
      ad(2,3);
      ad(2,4);
      ad(2,5);
      ad(2,6);
      ad(2,7);
      ad(2,8);
      ad(2,9);
      ad(2,10);
      ad(2,11);
      ad(2,12);
      ad(2,13);
      ad(2,14);
      ad(2,15);
      ad(2,16);
      // 3
      ad(3,2);
      ad(3,7);
      ad(3,14);
      // 4
      ad(4,2);
      ad(4,3);
      ad(4,4);
      ad(4,5);
      ad(4,7);
      ad(4,9);
      ad(4,10);
      ad(4,11);
      ad(4,13);
      ad(4,14);
      ad(4,15);
      ad(4,16);
      ad(4,17);
      ad(4,18);
      // 5
      ad(5,5);
      ad(5,7);
      ad(5,9);
      ad(5,13);
      ad(5,15);
      ad(5,18);
      // 6
      ad(6,2);
      ad(6,3);
      ad(6,4);
      ad(6,5);
      ad(6,7);
      ad(6,8);
      ad(6,9);
      ad(6,11);
      ad(6,12);
      ad(6,13);
      ad(6,15);
      ad(6,17);
      ad(6,18);
      // 7
      ad(7,2);
      ad(7,5);
      ad(7,7);
      ad(7,13);
      ad(7,15);
      ad(7,19);
      // 8
      ad(8,2);
      ad(8,4);
      ad(8,5);
      ad(8,7);
      ad(8,8);
      ad(8,9);
      ad(8,12);
      ad(8,13);
      ad(8,15);
      ad(8,16);
      ad(8,17);
      ad(8,19);
      // 9
      ad(9,2);
      ad(9,5);
      ad(9,7);
      ad(9,9);
      ad(9,10);
      ad(9,12);
      ad(9,15);
      ad(9,19);

      // 10
      ad(10,3);
      ad(10,5);
      ad(10,7);
      ad(10,14);
      ad(10,15);
      ad(10,17);
      ad(10,18);
      ad(10,19);

      // 11
      ad(11,2);
      ad(11,3);
      ad(11,4);
      ad(11,5);
      ad(11,6);
      ad(11,8);
      ad(11,9);
      ad(11,10);
      ad(11,12);
      ad(11,16);
      ad(11,14);
      ad(11,18);

      // 12
      ad(12,2);
      ad(12,4);
      ad(12,6);
      ad(12,7);
      ad(12,8);
      ad(12,10);
      ad(12,12);
      ad(12,14);
      ad(12,15);
      ad(12,16);
      ad(12,18);
      ad(12,19);

      // 13
      ad(13,2);
      ad(13,4);
      ad(13,10);
      ad(13,11);
      ad(13,12);
      ad(13,16);
      ad(13,19);

      // 14
      ad(14,2);
      ad(14,6);
      ad(14,7);
      ad(14,8);
      ad(14,13);
      ad(14,14);
      ad(14,19);

      // 15
      ad(15,2);
      ad(15,3);
      ad(15,4);
      ad(15,5);
      ad(15,6);
      ad(15,8);
      ad(15,9);
      ad(15,13);
      ad(15,14);
      ad(15,15);
      ad(15,17);
      ad(15,18);
      ad(15,19);

      // 16
      ad(16,2);
      ad(16,7);
      ad(16,8);
      ad(16,9);
      ad(16,11);
      ad(16,12);
      ad(16,13);
      ad(16,15);
      ad(16,17);

      // 17
      ad(17,2);
      ad(17,3);
      ad(17,4);
      ad(17,7);
      ad(17,9);
      ad(17,10);
      ad(17,11);
      ad(17,13);
      ad(17,15);
      ad(17,17);

        // 18
      ad(18,15);
      ad(18,16);
      ad(18,17);

      laby_dat[7][19] = 2;
      draw();
      };
};


app_begin ( void ) {


  if ( ap_process == AP_INIT ) { /* When ap start */

      AP_SETNUMOFCALLS(1); /* Set MAX of Calls */

      init_window(); /* Run the init void */

  };

  if ( ap_process == AP_FREE ) {   /*   */

  };
  if ( ap_process == AP_DONE ) {  /* When ap done */
  };
} app_end;

