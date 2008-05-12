/* ------------------------------------------------- */
/*   TaskBar object
           by Julien Etelain <julien@pmad.mail22.com>

        (c) 2000 COPYRIGHT JULIEN ETALAIN
                      ALL RIGHTS RESERVED

   This is a part of SEAL ...
       -> Have a look to about.txt
                                                     */
/* ------------------------------------------------- */


#include <stdio.h>
#include <stdarg.h>
#include"allegro.h"
#include"seal.h"
#include"taskobj.h"
#include<math.h>
#include"app.h"
l_color  pal_taskbar[] =  {CO_SYSx23, CO_WHITE, CO_SYSx18, CO_BLACK};
p_object oldtopwin = NULL;
p_taskbar   (*taskbar_init) ( p_taskbar o, t_rect r ) = &_taskbar_init;

l_long old_cnt_window = 0;
/* t_taskbar functions */

l_long get_num_of_windows() {

   l_long cnt = 0;
   p_object p = OBJECT(OBJECT(desktop)->first_view(OBJECT(desktop)));
   p_object f = p;
   do {
     if ( p->tag & TAG_WINDOW ) { // Ho it's a window !
         cnt=cnt + 1;
     };
     p = OBJECT(OBJECT(p)->next_view(OBJECT(p))); // Next ...
   }while(f != p) ;

   return cnt;
};

void   taskbar_draw ( p_view o )
{
 t_rect  r = o->get_local_extent(o);
  t_point p;

 BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

 //   button(out, p.x+r.a.x, p.y+r.a.y, p.x+r.b.x, p.y+r.b.y, COLOR(CO_BLACK), COLOR(CO_WHITE));
 //   o->brush.color = COLOR(CO_SYSx23);
    taskbar(o)->draw_taskbar(taskbar(o));

  };

  o->end_of_paint(o, r);
};


t_rect taskbar_size_limits ( p_view o )
{

   return rect_assign(1, 1, rect_sizex(o->bounds)-1, rect_sizey(o->bounds)-1);

};


void   taskbar_draw_taskbar ( p_taskbar o )
{

  p_view  vo = VIEW(o);
 t_rect  r = vo->size_limits(vo);
  t_point pe;

  BITMAP *out = vo->begin_paint(vo, &pe, r);
 // clear(out);
  vo->brush.color = pal_taskbar[0];
  rectfill(out, r.a.x+pe.x, r.a.y+pe.y, r.b.x+pe.x, r.b.y+pe.y, vo->brush.color);

  if ( out ) {
      l_long nb = get_num_of_windows();


      if (nb != 0) {
         p_object p = OBJECT(OBJECT(desktop)->first_view(OBJECT(desktop)));
         l_long width_item = r.b.x / nb ;
         p_object f = p;
         l_long cnt = 0;

           do {
           if ( p->tag & TAG_WINDOW ) { // Ho it's a window !
               l_text win_caption = WINDOW(p)-> caption;
               t_rect item_r = rect_assign(pe.x + cnt * width_item + 1 ,
                                           pe.y + 0 ,
                                           pe.x +  cnt * width_item + width_item ,
                                           pe.y + 20);
               if( cnt == 0 ) {
               button(out, item_r.a.x + 1,
                           item_r.a.y + 1,
                           item_r.b.x,
                           item_r.b.y,
                           pal_taskbar[2], pal_taskbar[1]);
               } else {
               button(out, item_r.a.x + 1,
                           item_r.a.y + 1,
                           item_r.b.x,
                           item_r.b.y,
                           pal_taskbar[1], pal_taskbar[2]);
               };
               if ( win_caption != "" )
                textout_draw_rect(out, vo->font, win_caption, -1,
                                        item_r.a.x + 3,
                                        item_r.a.y + 2,
                                        item_r.b.x - 1,
                                        item_r.b.y - 1,
                                       TX_ALIGN_CENTERY, pal_taskbar[3], pal_taskbar[0], 1);

           cnt=cnt + 1;
           };
           p = OBJECT(OBJECT(p)->next_view(OBJECT(p))); // Next ...
           }while(f != p) ;

      };

  };

  vo->end_of_paint(vo, r);  // here

};



void   taskbar_func_callback ( p_object o ) // Call each 20s (on my computer ...)
{

l_long nb = get_num_of_windows();
l_long ok = (oldtopwin != OBJECT(OBJECT(desktop)->first_view(OBJECT(desktop))));
if (ok !=1) ok = (nb != old_cnt_window);
if (ok) {

taskbar(o)->draw_taskbar(taskbar(o));
old_cnt_window  = nb;

oldtopwin = OBJECT(OBJECT(desktop)->first_view(OBJECT(desktop)));

};

};


void   taskbar_translate_event ( p_object o, t_event *event )
{

  RETVIEW(o, event); // Not yet !

    if ( event->type & EV_MOUSE ) {  // MO_SF_MOUSELDOWN
     l_long ok = 0;
     ok = ( OBJECT(mouse)->state & MO_SF_MOUSELUP  );
     if (ok == 0) ok = ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN  );

     if ( ok  ) {
     l_long nb = get_num_of_windows();


      if (nb != 0) {
      t_rect  r = VIEW(o)->size_limits(VIEW(o));
      l_long width_item = r.b.x / nb ;
      t_point pe =  VIEW(o)->get_local_point(VIEW(o), mouse->where);
      p_object p = OBJECT(OBJECT(desktop)->first_view(OBJECT(desktop)));
      p_object f = p;
      l_long cnt = 0;
      pe.x = pe.x - r.a.x;
           do {
           if ( p->tag & TAG_WINDOW ) { // Ho it's a window !

           if ((pe.x >=  cnt * width_item + 1 ) && (pe.x <=  cnt * width_item + width_item )) {

           if ( OBJECT(mouse)->state & MO_SF_MOUSELUP  ) {

            VIEW(OBJECT(p))->show(VIEW(OBJECT(p)));
           OBJECT(p)->select(OBJECT(p));
           taskbar(o)->draw_taskbar(taskbar(o));

           } else {
             p_view  vo = VIEW(o);
             t_rect  r2 = vo->size_limits(vo);
             t_point pe2;
             BITMAP *out = vo->begin_paint(vo, &pe2, r2);

             t_rect item_r = rect_assign(pe2.x + cnt * width_item + 1 ,
                                           pe2.y + 0 ,
                                           pe2.x +  cnt * width_item + width_item ,
                                           pe2.y + 20);

             button(out, item_r.a.x + 1,
                           item_r.a.y + 1,
                           item_r.b.x,
                           item_r.b.y,
                           pal_taskbar[2], pal_taskbar[1]);

             vo->end_of_paint(vo, r);
           };
           };
           cnt=cnt + 1;
           };
           p = OBJECT(OBJECT(p)->next_view(OBJECT(p))); // Next ...
           }while(f != p) ;

     };

        clear_event(event);


     };


  };

};




p_taskbar   _taskbar_init ( p_taskbar o, t_rect r )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_taskbar));

  view_init(VIEW(o), r);


  /* object's declarations */

  OBJECT(o)->translate_event = &taskbar_translate_event;
  OBJECT(o)->func_callback = &taskbar_func_callback;


  /* view's declarations */

  VIEW(o)->draw = &taskbar_draw;
  VIEW(o)->size_limits = &taskbar_size_limits;


  /* taskbar's declarations */

  //  o->where = where;
  //  o->size  = size;

  o->draw_taskbar = &taskbar_draw_taskbar;

  OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
  OBJECT(o)->set_options(OBJECT(o), OB_OF_STILLPROCESS, true);

  VIEW(o)->set_palette(VIEW(o), pal_taskbar);

  //init_stillprocess ( OBJECT(o), 10 );
  return o;

};

 #include"taskobj.exp"


lib_begin ( void ) {

  if ( ap_process == AP_ALLOC ) {

          AP_EXPORTLIB(); /* export table in app.exp */

  } else

  if ( ap_process == AP_INIT ) {

          AP_SETNUMOFCALLS(1); /* set max num of calls */

          pal_taskbar[0] = color_get_from_ini("taskbar_background");
          pal_taskbar[1] = color_get_from_ini("taskbar_color1");
          pal_taskbar[2] = color_get_from_ini("taskbar_color2");
          pal_taskbar[3] = color_get_from_ini("taskbar_text");
  } else

  if ( ap_process == AP_FREE ) {


  };


} lib_end;

