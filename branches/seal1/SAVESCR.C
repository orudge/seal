/*
   SAVESCR.C

	Copyright 1999 (c) by Michal Stencl

	stenclpmd@ba.telecom.sk
*/

/*
 - save screen = ALT+F8
*/

#include"allegro.h"
#include"seal.h"

void  trans_ev ( p_object o, p_event event ) {

   if ( keyb->code == TO_ALT(KB_F8) ) {

       set_clip(screen, 0, 0, SCREEN_W, SCREEN_H);

       save_bitmap("scrshot.jpg", screen, NULL);

   };

};



app_begin ( void ) {


  if ( ap_process == AP_INIT ) {

     t_object *o = obj_init(_malloc(sizeof(t_object)));

     o->translate_event = &trans_ev;

     OBJECT(drivers)->insert(OBJECT(drivers), o);

  };

} app_end;
