#include"allegro.h"
#include"seal.h"
#include"dlxsound.h"
#include"dialogs.h"
#include"libamp.h"


#define        TXT_STEREO      INI_TEXT("Stereo")
#define        TXT_MONO        INI_TEXT("Mono")
#define        TXT_MP3INFO     INI_TEXT("Layer: %d, Mpeg: %d, %s, %dHz, %s")
#define        TXT_SURROUND    INI_TEXT("Surround")
#define        TXT_NOSURROUND  INI_TEXT("No surround")

   signed int mp3_pause = false;
   signed int PausedPos = 0;
   l_text filename;
   




   //play file funktion
   l_int mp3_init_file (p_soundformat_item o, l_text file)
         {


         if (filename)
          {
          _free(filename);
          }

         filename = (l_text)malloc(260);
         
         strcpy(filename, file);

//         seal_error(ERR_INFO, "filename: %s, file: %s", filename, file);

         if (install_amp() == FALSE)
               {
                     seal_error(ERR_INFO, INI_TEXT("Cannot init mp3 library!"));
                     return 0;
               };



         return 1;
         };

   //play file funktion
   l_int mp3_play_file (p_soundformat_item o)
         {
            if (load_amp(filename,FALSE) == FALSE)
                  {
                  seal_error(ERR_INFO, INI_TEXT("Cannot load file"));
                  return 0;
                  };

//         seal_error(ERR_INFO, "play %s", filename);
         return 1;
         };
         

   //poll file
   l_int mp3_poll_file(p_soundformat_item o)
         {
//         seal_error(ERR_INFO, "poll");
         if ( run_amp() == -1) return 0;
         return 1;
         };

   //stop file
   l_int mp3_stop_file(p_soundformat_item o)
         {
//         seal_error(ERR_INFO, "Stop");
         unload_amp();
         return 1;
         };

   //pause file
   l_int mp3_pause_file(p_soundformat_item o)
         {

   switch (mp3_pause)
   {

   case false:                            // pause
     {
     mp3_pause = true;     
     stop_amp();
     PausedPos=amp_frame;     
     break;
     }
   case true:                             //plays after pause
     {
     seek_amp_abs(PausedPos);
     mp3_pause = false;          
     break;
     }

     }
     
         
         return 1;
         };

   //forward
   l_int mp3_forward_file(p_soundformat_item o, l_int relpos)
         {
         seek_amp_rel(relpos*amp_samprat/amp_pollsize);
         return 1;
         };

   //rewind
   l_int mp3_rewind_file(p_soundformat_item o, l_int relpos)
         {
         seek_amp_rel(-relpos*amp_samprat/amp_pollsize);
         return 1;
         };

   //set pos
   l_int mp3_set_pos(p_soundformat_item o, l_int pos)
         {
         seek_amp_abs(pos);
         return 1;
         };

   //get current pos
   l_int mp3_get_pos(p_soundformat_item o)
         {
         return amp_frame;

         };

   //lenght of file
   l_int mp3_get_len_file(p_soundformat_item o)
         {

         return 0;
         };

   // get current time
   l_int mp3_get_time(p_soundformat_item o)
         {
         return amp_time;
         };

   l_int mp3_is_stereo(p_soundformat_item o)
         {
         
         return amp_stereo;
         };

   l_text mp3_get_additional_info(p_soundformat_item o)
         {
         return set_format_text(NULL, TXT_MP3INFO, amp_layer, amp_mpg_ver, amp_stereo == 1 ? TXT_STEREO : TXT_MONO, amp_samprat, amp_reverse_phase == 1 ? TXT_SURROUND : TXT_NOSURROUND);
         };


//lib_exportable;
#include "sfamp3.exp"



lib_begin ( void ) {

  if ( ap_process == AP_ALLOC )
     {
     AP_EXPORTLIB();
     }


  if ( ap_process == AP_INIT ) {



      p_soundformat_item k;

      AP_EXPORTLIB();

//      AP_SETNUMOFCALLS(1);

      


      //init sound format list!!

      k = new_soundformat_item(                      ("mp3"),
                                                     ("(c) Florian Xaver GNU GPL - virtual sound file"),
                                                     &mp3_init_file,
                                                     &mp3_play_file,
                                                     &mp3_poll_file,
                                                     &mp3_stop_file,
                                                     &mp3_pause_file,
                                                     &mp3_forward_file,
                                                     &mp3_rewind_file,
                                                     &mp3_set_pos,
                                                     &mp3_get_pos,
                                                     &mp3_get_len_file,
                                                     &mp3_get_time,
                                                     &mp3_is_stereo,
                                                     &mp3_get_additional_info
                                                     );




     sound_format_list->insert(sound_format_list, k);


//     seal_error(ERR_INFO, "Now extension \"mp3\" is supported!");

  }



  if ( ap_process == AP_DONE ) {
  



  }


  if ( ap_process == AP_FREE ) {



  };

} lib_end;