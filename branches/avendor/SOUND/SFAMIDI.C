#include"allegro.h"
#include"seal.h"
#include"dlxsound.h"
#include"dialogs.h"



#define        TXT_MIDIINFO     INI_TEXT(" ")

   signed int mid_pause = false;
   l_int old_zeit = 0;
   l_text filename;
   MIDI *midi_sample = 0;
   volatile int zeit = 0;




   //play file funktion
   l_int midi_init_file (p_soundformat_item o, l_text file)
         {
         if (filename) _free(filename);
         
         filename = _strdup(file);

//         seal_error(ERR_INFO, "Init");


         return 1;
         };

   //play file funktion
   l_int midi_play_file (p_soundformat_item o)
         {

         midi_sample = load_midi(filename);
         if (!midi_sample) return 0;
         play_midi(midi_sample, FALSE);
         zeit = 0;

//         seal_error(ERR_INFO, "play %s", filename);
         return 1;
         };
         

   //poll file
   l_int midi_poll_file(p_soundformat_item o)
         {
//         seal_error(ERR_INFO, "poll");
         return 1;
         };

   //stop file
   l_int midi_stop_file(p_soundformat_item o)
         {
//         seal_error(ERR_INFO, "Stop");
         destroy_midi(midi_sample);
         return 1;
         };

   //pause file
   l_int midi_pause_file(p_soundformat_item o)
         {

   switch (mid_pause)
   {

   case false:                            // pause
     {
     mid_pause = true;
     old_zeit = zeit;
     midi_pause();
     break;
     }
   case true:                             //plays after pause
     {
     midi_resume();
     zeit = old_zeit;
     mid_pause = false;          
     break;
     }

     }
     
         
         return 1;
         };

   //forward
   l_int midi_forward_file(p_soundformat_item o, l_int relpos)
         {
         midi_seek(midi_pos+relpos);
         return 1;
         };

   //rewind
   l_int midi_rewind_file(p_soundformat_item o, l_int relpos)
         {
         midi_seek(midi_pos-relpos);
         return 1;
         };

   //set pos
   l_int midi_set_pos(p_soundformat_item o, l_int pos)
         {
         midi_seek(pos);
         return 1;
         };

   //get current pos
   l_int midi_get_pos(p_soundformat_item o)
         {
         return midi_pos;

         };

   //lenght of file
   l_int midi_get_len_file(p_soundformat_item o)
         {

         return midi_loop_end - midi_loop_start;
         };

   // get current time
   l_int midi_get_time(p_soundformat_item o)
         {
         return zeit;
         };

   l_int midi_is_stereo(p_soundformat_item o)
         {
         
         return 0;
         };

   l_text midi_get_additional_info(p_soundformat_item o)
         {
         return set_format_text(NULL, TXT_MIDIINFO);
         };



void time_process()
      {
      zeit++;
      }

END_OF_FUNCTION(time_process);




//lib_exportable;
#include "sfamidi.exp"



lib_begin ( void ) {

  if ( ap_process == AP_ALLOC )
     {
     AP_EXPORTLIB();
     }



  if ( ap_process == AP_INIT ) {


      p_soundformat_item k = 0;

      AP_EXPORTLIB();
//      AP_SETNUMOFCALLS(2);
      

      install_int_ex(time_process, SECS_TO_TIMER(1));
      LOCK_VARIABLE(zeit);
      


      //init sound format list!!

      k = new_soundformat_item(                      ("mid"),
                                                     ("(c) Florian Xaver GNU GPL - virtual sound file"),
                                                     &midi_init_file,
                                                     &midi_play_file,
                                                     &midi_poll_file,
                                                     &midi_stop_file,
                                                     &midi_pause_file,
                                                     &midi_forward_file,
                                                     &midi_rewind_file,
                                                     &midi_set_pos,
                                                     &midi_get_pos,
                                                     &midi_get_len_file,
                                                     &midi_get_time,
                                                     &midi_is_stereo,
                                                     &midi_get_additional_info
                                                     );




     if (k) sound_format_list->insert(sound_format_list, k);


//     seal_error(ERR_INFO, "Now extension \"midi\" is supported!");

  }



  if ( ap_process == AP_DONE ) {
  



  }


  if ( ap_process == AP_FREE ) {



  };

} lib_end;