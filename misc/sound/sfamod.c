/******************************************/
/* SFA MOD playing library                */
/* Copyright (c) Owen Rudge, 25/4/2001    */
/* Based on SFAMP3.C by Florian Xaver.    */
/******************************************/

#include "allegro.h"
#include "seal.h"
#include "dlxsound.h"
#include "dialogs.h"
#include "jgmod.h"

JGMOD *the_mod;
l_text filename;
volatile int modpos;
volatile int paused;

void mod_timer();

void mod_timer(void)
{
   if (paused == FALSE)
      modpos++;
}

END_OF_FUNCTION(mod_timer);

//play file funktion
l_int mod_init_file (p_soundformat_item o, l_text file)
{
   if (filename)
   {
      _free(filename);
   }

   filename = _strdup(file);

   LOCK_VARIABLE(modpos);
   LOCK_VARIABLE(paused);
   LOCK_FUNCTION(mod_timer);

//         seal_error(ERR_INFO, "filename: %s, file: %s", filename, file);

   install_mod(24);
   return 1;
};

//play file funktion
l_int mod_play_file (p_soundformat_item o)
{
   the_mod = load_mod(filename);
   if (the_mod == NULL)
   {
      seal_error(ERR_INFO, INI_TEXT("Cannot load file"));
      return 0;
   }
   else
   {
      install_int(mod_timer, 1000);
      play_mod(the_mod, FALSE);
   }

//         seal_error(ERR_INFO, "play %s", filename);
   return 1;
};
         

//poll file
l_int mod_poll_file(p_soundformat_item o)
{
//         seal_error(ERR_INFO, "poll");
   if (is_mod_playing() == FALSE)
   {
      remove_int(mod_timer);
      modpos = 0;
      return 0;
   }
   return 1;
};

//stop file
l_int mod_stop_file(p_soundformat_item o)
{
//         seal_error(ERR_INFO, "Stop");
   stop_mod();
   remove_int(mod_timer);
   modpos = 0;
   destroy_mod(the_mod);
   return 1;
};

//pause file
l_int mod_pause_file(p_soundformat_item o)
{
   toggle_pause_mode();
   paused = is_mod_paused();

   return 1;
};

//forward
l_int mod_forward_file(p_soundformat_item o, l_int relpos)
{
   return 1;
};

//rewind
l_int mod_rewind_file(p_soundformat_item o, l_int relpos)
{
   return 1;
};

//set pos
l_int mod_set_pos(p_soundformat_item o, l_int pos)
{
   return 1;
};

//get current pos
l_int mod_get_pos(p_soundformat_item o)
{
   return -1;
};

//lenght of file
l_int mod_get_len_file(p_soundformat_item o)
{
   return 0;
};

// get current time
l_int mod_get_time(p_soundformat_item o)
{
   return modpos;
};

l_int mod_is_stereo(p_soundformat_item o)
{       
   return -1;
};

l_text mod_get_additional_info(p_soundformat_item o)
{
   JGMOD_INFO ji;

   get_mod_info(filename, &ji);
   return(set_format_text(NULL, "%s - %s", ji.type_name, ji.name));
};


//lib_exportable;
#include "sfamod.exp"


lib_begin ( void ) {

  if ( ap_process == AP_ALLOC )
     {
     AP_EXPORTLIB();
     }


  if ( ap_process == AP_INIT ) {



      p_soundformat_item k, k2, k3, k4, k5;

      AP_EXPORTLIB();

//      AP_SETNUMOFCALLS(1);

      


      //init sound format list!!

      k = new_soundformat_item(                      ("mod"),
                                                     ("Copyright (c) Owen Rudge 2001"),
                                                     &mod_init_file,
                                                     &mod_play_file,
                                                     &mod_poll_file,
                                                     &mod_stop_file,
                                                     &mod_pause_file,
                                                     &mod_forward_file,
                                                     &mod_rewind_file,
                                                     &mod_set_pos,
                                                     &mod_get_pos,
                                                     &mod_get_len_file,
                                                     &mod_get_time,
                                                     &mod_is_stereo,
                                                     &mod_get_additional_info
                                                     );

     sound_format_list->insert(sound_format_list, k);

      k2 = new_soundformat_item(                      ("xm"),
                                                     ("Copyright (c) Owen Rudge 2001"),
                                                     &mod_init_file,
                                                     &mod_play_file,
                                                     &mod_poll_file,
                                                     &mod_stop_file,
                                                     &mod_pause_file,
                                                     &mod_forward_file,
                                                     &mod_rewind_file,
                                                     &mod_set_pos,
                                                     &mod_get_pos,
                                                     &mod_get_len_file,
                                                     &mod_get_time,
                                                     &mod_is_stereo,
                                                     &mod_get_additional_info
                                                     );

     sound_format_list->insert(sound_format_list, k2);

      k3 = new_soundformat_item(                      ("s3m"),
                                                     ("Copyright (c) Owen Rudge 2001"),
                                                     &mod_init_file,
                                                     &mod_play_file,
                                                     &mod_poll_file,
                                                     &mod_stop_file,
                                                     &mod_pause_file,
                                                     &mod_forward_file,
                                                     &mod_rewind_file,
                                                     &mod_set_pos,
                                                     &mod_get_pos,
                                                     &mod_get_len_file,
                                                     &mod_get_time,
                                                     &mod_is_stereo,
                                                     &mod_get_additional_info
                                                     );


     sound_format_list->insert(sound_format_list, k3);

      k4 = new_soundformat_item(                      ("it"),
                                                     ("Copyright (c) Owen Rudge 2001"),
                                                     &mod_init_file,
                                                     &mod_play_file,
                                                     &mod_poll_file,
                                                     &mod_stop_file,
                                                     &mod_pause_file,
                                                     &mod_forward_file,
                                                     &mod_rewind_file,
                                                     &mod_set_pos,
                                                     &mod_get_pos,
                                                     &mod_get_len_file,
                                                     &mod_get_time,
                                                     &mod_is_stereo,
                                                     &mod_get_additional_info
                                                     );


     sound_format_list->insert(sound_format_list, k4);

      k5 = new_soundformat_item(                      ("jgm"),
                                                     ("Copyright (c) Owen Rudge 2001"),
                                                     &mod_init_file,
                                                     &mod_play_file,
                                                     &mod_poll_file,
                                                     &mod_stop_file,
                                                     &mod_pause_file,
                                                     &mod_forward_file,
                                                     &mod_rewind_file,
                                                     &mod_set_pos,
                                                     &mod_get_pos,
                                                     &mod_get_len_file,
                                                     &mod_get_time,
                                                     &mod_is_stereo,
                                                     &mod_get_additional_info
                                                     );


     sound_format_list->insert(sound_format_list, k5);

//     seal_error(ERR_INFO, "Now extension \"mod\" is supported!");

  }



  if ( ap_process == AP_DONE ) {
  
     remove_int(mod_timer);
     modpos = 0;



  }


  if ( ap_process == AP_FREE ) {



  };

} lib_end;