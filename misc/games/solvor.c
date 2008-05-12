/* Solvor the 2nd stupid game for SEAL ! */

#include"float.h"
#include"allegro.h"
#include"seal.h"
#include"view.h"
#include"object.h"
#include"app.h"
#include"button.h"
#include"grfx.h"
#include"dialogs.h"
#include"random.h"
#include"math.h"
l_long HowManyMines  = 75;
p_grfx solvor = NULL;
l_long width = 20;
l_int  solvor_dat[30][30];       // 0        1     2      3       4
                                 // Nothing, mine, Viwed, marked, Marked2
p_appwin p = NULL;
l_long fin = 0;


void cleansolvor () {
l_long x = 0;
l_long y = 0;
l_long i = 0;
while(x < width) {
while(y < width) {
solvor_dat[x][y] = 0;
y++;
};
y = 0;
x++;
};

while (i <  HowManyMines + 1) {
x = abs(random() % (width - 2));
y = abs(random() % (width - 2));
x++;
y++;
if (solvor_dat[x][y] == 0) {
solvor_dat[x][y] = 1;
i++;
};
};

fin = 0;

};

l_long howmany_good () {
l_long x = 0;
l_long y = 0;
l_long i = 0;

while(x < width) {
while(y < width) {
if (solvor_dat[x][y] == 4) i++;
y++;
};
y = 0;
x++;
};

return i;
};

l_long howmany_bad () {
l_long x = 0;
l_long y = 0;
l_long i = 0;
while(x < width) {
while(y < width) {
if (solvor_dat[x][y] == 3) i++;
y++;
};
y = 0;
x++;
};
return i;
};

l_long howmany (l_long x, l_long y) {
l_long i = 0;

if (solvor_dat[x + 1][y + 1] == 1) i++;
if (solvor_dat[x - 1][y - 1] == 1) i++;
if (solvor_dat[x + 1][y - 1] == 1) i++;
if (solvor_dat[x - 1][y + 1] == 1) i++;

if (solvor_dat[x][y + 1] == 1) i++;
if (solvor_dat[x][y - 1] == 1) i++;
if (solvor_dat[x + 1][y] == 1) i++;
if (solvor_dat[x - 1][y] == 1) i++;

if (solvor_dat[x + 1][y + 1] == 4) i++;
if (solvor_dat[x - 1][y - 1] == 4) i++;
if (solvor_dat[x + 1][y - 1] == 4) i++;
if (solvor_dat[x - 1][y + 1] == 4) i++;
if (solvor_dat[x][y + 1] == 4) i++;
if (solvor_dat[x][y - 1] == 4) i++;
if (solvor_dat[x + 1][y] == 4) i++;
if (solvor_dat[x - 1][y] == 4) i++;

return i;
};

void mark(l_long x, l_long y) {

if (solvor_dat[x][y] == 0) {
solvor_dat[x][y] = 3;
} else if (solvor_dat[x][y] == 1){
 solvor_dat[x][y] = 4;
} else if (solvor_dat[x][y] == 3){
solvor_dat[x][y] = 0;
} else if (solvor_dat[x][y] == 4){
solvor_dat[x][y] = 1;
};

};



l_long go(l_long x, l_long y) {

if (solvor_dat[x][y] == 1) { // It's a mine
solvor_dat[x][y] = 5;
fin = 1;
return 1;
};

if (solvor_dat[x][y] == 0) {
solvor_dat[x][y] = 2;
if (howmany(x,y) == 0) {
if ((solvor_dat[x][y + 1] == 0) && ( y != width - 1)) go(x, y + 1);
if ((solvor_dat[x][y - 1] == 0) && ( y != 0 )) go(x, y - 1);
if ((solvor_dat[x + 1][y] == 0) && ( x != width - 1)) go(x + 1, y);
if ((solvor_dat[x - 1][y] == 0) && ( y != 0 )) go(x - 1, y);
if ((solvor_dat[x + 1][y + 1] == 0)&& ( x != width - 1)&& ( y != width - 1)) go(x + 1, y + 1);
if ((solvor_dat[x - 1][y - 1] == 0)&& ( x != 0)&& ( y != 0)) go(x - 1, y - 1);
if ((solvor_dat[x + 1][y - 1] == 0)&& ( x != width - 1)&& ( y != 0)) go(x + 1, y - 1);
if ((solvor_dat[x - 1][y + 1] == 0)&& ( x != 0)&& ( y != width - 1)) go(x - 1, y + 1);
};

};
fin = 0;
return 0;
};

void draw () {

p_view  vo = VIEW(solvor);
t_rect  r = vo->size_limits(vo);
t_point pe;

BITMAP *out = vo->begin_paint(vo, &pe, r);

if (out) {
      rectfill(out, pe.x ,
                    pe.y ,
                    pe.x + 300,
                    pe.y + 300,
                    COLOR(CO_SYSx23));

if (fin == 0) {
l_long x = 0;
l_long y = 0;
l_long ww = 300 / width;
while (x < width) {
while (y < width) {
  vo->brush.color = COLOR(CO_SYSx23);

      if ((solvor_dat[x][y] == 0) || (solvor_dat[x][y] == 1)) {

      button(out,   pe.x + (x * ww) + 1,
                    pe.y + (y * ww) + 1,
                    pe.x + (x * ww) + ww,
                    pe.y + (y * ww) + ww,
                 COLOR(CO_WHITE), COLOR(CO_SYSx18));

      } else if (solvor_dat[x][y] == 2) {


      if (howmany(x,y) !=  0)

      textout_draw_rect(out, vo->font, set_format_text(NULL, "%d", howmany(x,y)), -1,
                                        pe.x + (x * ww) + 2,
                                        pe.y + (y * ww) + 2,
                                        pe.x + (x * ww) + ww - 1,
                                        pe.y + (y * ww) + ww - 1,
                                        TX_ALIGN_CENTERY|TX_ALIGN_CENTERX, COLOR(CO_BLACK), COLOR(CO_SYSx23), 1);

      } else {

      line(out,   pe.x + (x * ww) + 3,
                    pe.y + (y * ww) + 3,
                    pe.x + (x * ww) + ww - 3,
                    pe.y + (y * ww) + ww - 3,
                    COLOR(CO_BLACK));

      line(out,   pe.x + (x * ww) + 3,
                    pe.y + (y * ww) + ww - 3,
                    pe.x + (x * ww) + ww - 3,
                    pe.y + (y * ww) + 3,
                    COLOR(CO_BLACK));

      button(out,   pe.x + (x * ww) + 1,
                    pe.y + (y * ww) + 1,
                    pe.x + (x * ww) + ww,
                    pe.y + (y * ww) + ww,
                    COLOR(CO_WHITE), COLOR(CO_SYSx18));

      };


y++;
};
y = 0;
x++;
};

} else if (fin == 1) {
      textout_draw_rect(out, vo->font, "BOOM ! A mine exploded !", -1,
                                        pe.x ,
                                        pe.y ,
                                        pe.x + 300,
                                        pe.y + 300,
                                        TX_ALIGN_CENTERY, COLOR(CO_BLACK), COLOR(CO_SYSx23), 1);
} else {
      textout_draw_rect(out, vo->font, "YOU WIN :) !", -1,
                                        pe.x ,
                                        pe.y ,
                                        pe.x + 300,
                                        pe.y + 300,
                                        TX_ALIGN_CENTERY, COLOR(CO_BLACK), COLOR(CO_SYSx23), 1);
};

};
vo->end_of_paint(vo, r);

};

void   draw2 ( p_view o ) { draw(); };




static void trans_ev ( p_object o, p_event event ) {

   RETVIEW(o, event);

//------------------------------
if ( event->type == EV_MESSAGE )
switch ( event->message ) {
case MSG_OK : {
cleansolvor();
draw();
clear_event(event);
}; break;
};
//------------------------------

};


static void trans_ev2 ( p_object o, p_event event ) {
if (fin == 0) {
    if ( event->type & EV_MOUSE ) {

      t_rect  r = VIEW(o)->size_limits(VIEW(o));
      t_point p =  VIEW(o)->get_local_point(VIEW(o), mouse->where);
      l_long x = 0;
      l_long y = 0;
      l_long ww = 300 / width;
      l_long x2 = width + 1;
      l_long y2 = width + 1;
      p.x = p.x - r.a.x;
      p.y = p.y - r.a.y;

      while ( x2 > -1 ) {
      if ( p.x < x2 * ww) x = x2 - 1;
      x2--;
      };

      while ( y2 > -1 ) {
      if ( p.y < y2 * ww) y = y2 - 1;
      y2--;
      };


      if ( OBJECT(mouse)->state & MO_SF_MOUSELUP  ) {

      if ( go(x, y) == 1) {
      //cleansolvor();
      fin = 1;
      draw();
      } else {
      draw();
      };

/*      } else if ( OBJECT(mouse)->state & MO_SF_MOUSELDOUBLE  ) {

      if ( go2(x, y) == 1) {
      //cleansolvor();
      fin = 1;
      draw();
      } else {
      draw();
      };  */

      } else if ( OBJECT(mouse)->state & MO_SF_MOUSERUP ) {
      mark(x,y);
             if (howmany_good() - 1 == HowManyMines) { fin = 2; } else { fin = 0; };
      draw();
      };

      clear_event(event);

      };


};

};
void init_window ( void )
{

   t_rect   r = rect_assign(0, 0, 310, 370);

   p = appwin_init(_malloc(sizeof(t_appwin)),
                            r,
                            "Solvor for SEAL !",
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

      r = rect_assign(158, 330, 308, 347);

      b = button_init(_malloc(sizeof(t_button)),
                            r,
                            "Restart",
                            MSG_OK,
                            BF_DEFAULT);

      OBJECT(p)->insert(OBJECT(p), OBJECT(b));

      r = rect_assign(3, 25, 303, 325);

      solvor = grfx_init(_malloc(sizeof(t_grfx)),r );

      VIEW(solvor)->draw = &draw2;
      OBJECT(solvor)->translate_event =  &trans_ev2;
      OBJECT(p)->insert(OBJECT(p), OBJECT(solvor));

      cleansolvor();

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

