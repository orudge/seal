/* TicTacTic the 2nd stupid game for SEAL ! */

#include"float.h"
#include"allegro.h"
#include"seal.h"
#include"view.h"
#include"object.h"
#include"app.h"
#include"button.h"
#include"grfx.h"
#include"dialogs.h"


p_grfx tictactic = NULL;
l_int  tictactic_dat[3][3];
p_appwin p = NULL;

void draw () {

p_view  vo = VIEW(tictactic);
t_rect  r = vo->size_limits(vo);
t_point pe;

BITMAP *out = vo->begin_paint(vo, &pe, r);

if (out) {

l_long x = 0;
l_long y = 0;

        rectfill(out, pe.x ,
                      pe.y ,
                      pe.x + r.b.x,
                      pe.y + r.b.y,
                      COLOR(CO_SYSx23));

while (x < 3) {
while (y < 3) {
  vo->brush.color = COLOR(CO_SYSx23);

      rectfill(out, pe.x + (x * 100) + 1,
                    pe.y + (y * 100) + 1,
                    pe.x + (x * 100) + 100,
                    pe.y + (y * 100) + 100,
                    COLOR(CO_SYSx23));

      button(out,   pe.x + (x * 100) + 1,
                    pe.y + (y * 100) + 1,
                    pe.x + (x * 100) + 100,
                    pe.y + (y * 100) + 100,
                 COLOR(CO_SYSx18), COLOR(CO_WHITE));

      if (tictactic_dat[x][y] == 1)  rectfill(out, pe.x + (x * 100) + 31,
                                                   pe.y + (y * 100) + 31,
                                                   pe.x + (x * 100) + 70,
                                                   pe.y + (y * 100) + 70,
                                                   COLOR(CO_RED));

      if (tictactic_dat[x][y] == 2)   /* rectfill(out, pe.x + (x * 100) + 31,
                                                   pe.y + (y * 100) + 31,
                                                   pe.x + (x * 100) + 70,
                                                   pe.y + (y * 100) + 70,
                                                   COLOR(CO_GREEN));  */
      circlefill(out,pe.x + (x * 100) +  50,pe.y + (y * 100) + 50, 20 , COLOR(CO_GREEN));
y++;
};
y = 0;
x++;
};
};
vo->end_of_paint(vo, r);

};

void   draw2 ( p_view o ) { draw(); };

l_bool cht(l_long x, l_long y) {

if (tictactic_dat[x][y] == 0) {
tictactic_dat[x][y] = 2;
return true;
} else {
return false;
};

};

l_long computer_turn () { // Computer turn

l_long x  = 0;
l_long y  = 0;
while(x < 3) {
if (tictactic_dat[x][0] == tictactic_dat[x][1] != 0 ){ if (cht(x, 2)) return 0;};
if (tictactic_dat[x][1] == tictactic_dat[x][2] != 0 ){ if (cht(x, 0)) return 0;};
if (tictactic_dat[x][0] == tictactic_dat[x][2] != 0 ){ if (cht(x, 1)) return 0;};

if (tictactic_dat[0][x] == tictactic_dat[1][x] != 0 ){ if (cht(2, x)) return 0;};
if (tictactic_dat[1][x] == tictactic_dat[2][x] != 0 ){ if (cht(0, x)) return 0;};
if (tictactic_dat[0][x] == tictactic_dat[2][x] != 0 ){ if (cht(1, x)) return 0;};

x++;
};

if (tictactic_dat[0][0] == tictactic_dat[1][1] != 0 ){ if (cht(2, 2)) return 0;};
if (tictactic_dat[1][1] == tictactic_dat[2][2] != 0 ){ if (cht(0, 0)) return 0;};
if (tictactic_dat[0][2] == tictactic_dat[1][1] != 0 ){ if (cht(2, 0)) return 0;};
if (tictactic_dat[1][1] == tictactic_dat[2][0] != 0 ){ if (cht(0, 2)) return 0;};


x = 0;
y = 0;
while(x < 3) {
while(y < 3) {
if (cht(x,y)) return 0;
y++;
};
y = 0;
x++;
};

return 1; // I can't play ... It's not normal !
};

l_long someonewin() { // Check if someone have win
l_long x  = 0;
l_long ok = 0;

while(x < 3) {
if ((tictactic_dat[x][0] == tictactic_dat[x][1])&&(tictactic_dat[x][1] == tictactic_dat[x][2])) ok = tictactic_dat[x][0];
if ((tictactic_dat[0][x] == tictactic_dat[1][x])&&(tictactic_dat[1][x] == tictactic_dat[2][x])) ok = tictactic_dat[0][x];
x++;
};
if ((tictactic_dat[0][0] == tictactic_dat[1][1])&&(tictactic_dat[1][1] == tictactic_dat[2][2])) ok = tictactic_dat[0][0];
if ((tictactic_dat[0][2] == tictactic_dat[1][1])&&(tictactic_dat[1][1] == tictactic_dat[2][0])) ok = tictactic_dat[0][2];

return ok;
};



void userclick (l_long x , l_long y) { // User click somewhere
if (( tictactic_dat[x][y] == 0  ) && ( someonewin() == 0 )) {

tictactic_dat[x][y] = 1;

if ( someonewin() == 0 )  {
if (computer_turn() == 1) DEBUG_printf("TicTacTic : Computer ERR !\n");

};

draw();


};
};

void cleanttt () {
l_long x = 0;
l_long y = 0;
while(x < 3) {
while(y < 3) {
tictactic_dat[x][y] = 0;
y++;
};
y = 0;
x++;
};
};


static void trans_ev ( p_object o, p_event event ) {

   RETVIEW(o, event);

    if ( event->type & EV_MOUSE ) {

     if ( OBJECT(mouse)->state & MO_SF_MOUSEUP  ) {

      t_rect  r = VIEW(o)->size_limits(VIEW(o));
      t_point p =  VIEW(o)->get_local_point(VIEW(o), mouse->where);
      l_long x = 0;
      l_long y = 0;
      p.x = p.x - r.a.x;
      p.y = p.y - r.a.y;

       x = 2;
      if (p.x < 200) x = 1;
      if (p.x < 100) x = 0;

       y = 2;
      if (p.y < 200) y = 1;
      if (p.y < 100) y = 0;

      userclick(x, y);
      clear_event(event);
};
};

//------------------------------
if ( event->type == EV_MESSAGE )
switch ( event->message ) {
case MSG_OK : {
cleanttt();
draw();
clear_event(event);
}; break;
};
//------------------------------

};

void init_window ( void )
{

   t_rect   r = rect_assign(0, 0, 310, 370);

   p = appwin_init(_malloc(sizeof(t_appwin)),
                            r,
                            "TicTacTic for SEAL !",
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
                            "! CLEAN !",
                            MSG_OK,
                            BF_DEFAULT);

      OBJECT(p)->insert(OBJECT(p), OBJECT(b));

      r = rect_assign(3, 25, 303, 325);

      tictactic = grfx_init(_malloc(sizeof(t_grfx)),r );

      VIEW(tictactic)->draw = &draw2;

      OBJECT(p)->insert(OBJECT(p), OBJECT(tictactic));


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

