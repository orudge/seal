#include"allegro.h"
#include"seal.h"
#include"app.h"
#include"button.h"
#include"dialogs.h"
#include"iodlg.h"
#include"dlxsound.h"


p_play pp = 0;
l_int file_is_playing = 0;  // 0 = false

p_list get_player_filelist ( void ) {

                                                /* function for free-ing items */
  p_list p = list_init(malloc(sizeof(t_list)), &free_filehistory_item, 0);

  if ( p ) {

     p->insert(p, new_filehistory_item("MPEG Layer 2/3 (*.mp?)", "*.mp?"));
     p->insert(p, new_filehistory_item("MIDI (*.mid)", "*.mid"));
     p->insert(p, new_filehistory_item("MOV (*.mov)", "*.mov"));
     p->insert(p, new_filehistory_item("MOD (*.mod)", "*.mod"));
     p->insert(p, new_filehistory_item("MOD - XM (*.xm)", "*.xm"));
     p->insert(p, new_filehistory_item("MOD - IT (*.it)", "*.it"));
     p->insert(p, new_filehistory_item("MOD - S3M (*.s3m)", "*.s3m"));
     p->insert(p, new_filehistory_item("MOD - JGM (*.jgm)", "*.jgm"));
//     p->insert(p, new_filehistory_item("(*.)", "*."));
     p->insert(p, new_filehistory_item("All files (*.*)", "*.*"));

  };


  return p;

};



void play(l_text file) {

//     seal_error(ERR_INFO, "Want to play file [%s]", file);

     
     if (!pp->init_file(pp, file))
     {
        seal_error(ERR_INFO, "Error opening file [%s]", file);
        
     }
     else
     {
     if (!pp->play_file(pp)) return;
     file_is_playing = 1;
     }

}

void  poll_func_callback ( p_object s ) /* it's call each second */
{

     file_is_playing = pp->poll_file(pp);
     
};


void  player_translate_event ( p_object o, t_event *e )
{

   RETVIEW(o, e);
//   file_is_playing = pp->poll_file(pp);

   if ( o->phase == PH_PREPROCESS ) {

      if (file_is_playing == 0)
      {
         set_event(e, EV_MESSAGE, MSG_CLOSE, o);
      }



      if ( e->type == EV_MESSAGE ) {

         switch ( e->message ) {
         case MSG_CLOSE:
         pp->stop_file(pp);

         }
      };

   };

};


static t_point   new_win_size_minimum ( p_view o )
{

  return point_assign(100, 20);

};

void  stat1_func_callback ( p_object s ) /* it's call each second */
{


     TEXTLINE(s)->set_text(TEXTLINE(s),
                               set_format_text(NULL, "%02d:%02d",
                               pp->get_time(pp)/60,pp->get_time(pp)%60
                               ));

};





app_begin ( void ) {

  if ( ap_process == AP_INIT ) {

    t_rect r;
    p_object o = 0;
    p_object stat = 0;
    p_textline stat1 = 0;
    l_text file=0;

    AP_SETNUMOFCALLS(1);


    if (get_sound_installed() == false);
    // dummy, this program don't start if dlxsound hasn't loaded before





    if ( !ap_args ) file = open_dialog("c:/", NULL, get_player_filelist());
    else file = _strdup(ap_args);

    if ( !file ) return;



    pp = SFA_init();

    

    play(file);

    _free(file);

    r = rect_assign(0, 0, 400, 100);


    o = appwin_init(malloc(sizeof(t_appwin)), /* make window */
                            r,
                            "SFA Player .06 Beta",
//                            ap_args,
                            0x0,
                            ap_id, /* application id */
                            &player_translate_event);

     VIEW(o)->size_minimum = &new_win_size_minimum;

     
     OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(o));

     r = rect_assign(10, 30, 290, 70);

     stat = stattext_init ( malloc(sizeof(t_stattext)),
					 r, 
					 TX_ALIGN_LEFT, // limit
					 "Playing %s...\n%s",
                (l_text) get_filename(ap_args),
                (l_text) pp->get_additional_info(pp));

     OBJECT(stat)->func_callback = &poll_func_callback;
     OBJECT(o)->insert(OBJECT(o), OBJECT(stat));


//     OBJECT(stat)->process_tick = 20; /* each second rewrite it */

     init_stillprocess ( OBJECT(stat), 100 );

     
      r = rect_assign(10, 70, 190, 90);

     stat1 = dyntext_init( _malloc(sizeof(t_textline)),
                            r,
                            255
                            );

     OBJECT(stat1)->func_callback = &stat1_func_callback;


     OBJECT(o)->insert(OBJECT(o), OBJECT(stat1));

     stat1->set_text(stat1, "Welcome!");

     init_stillprocess ( OBJECT(stat1), 1000 );



  } else

  if ( ap_process == AP_FREE ) {



  };

} app_end;
