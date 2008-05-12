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

/* OCR (16/09/2001): Updated new_soundformat_item call */
/* OCR (04/10/2001): Added about box */
/* OCR (02/04/2002): Made timer pause if MIDI is paused */
/* OCR (05/04/2002): Added send_custom_message function */

#include <seal.h>
#include <sound.h>
#include <dialogs.h>

#define TXT_MIDIINFO     INI_TEXT(" ")

signed int mid_pause = false;
l_int old_zeit = 0;
l_text filename = 0;
MIDI *midi_sample = 0;
volatile int zeit = 0;

l_bool midi_init_driver(p_soundformat_item o)
{
   return(TRUE);
}

l_bool midi_init_file (p_soundformat_item o, l_text file, l_bool play)
{
   if (filename)
      _free(filename);
         
   filename = _strdup(file);

   midi_sample = load_midi(filename);

   if (!midi_sample)
      return(FALSE);
   
   if (play == TRUE)
   {
      play_midi(midi_sample, FALSE);
      zeit = 0;
   }

   return(TRUE);
}

l_bool midi_play_file (p_soundformat_item o)
{
   play_midi(midi_sample, FALSE);
   zeit = 0;

   return(TRUE);
}
         

l_bool midi_poll_file(p_soundformat_item o)
{
   if (midi_pos < 0)
      return(FALSE);
   else
      return(TRUE);
}

l_bool midi_stop_file(p_soundformat_item o)
{
   stop_midi();
   return(TRUE);
}

l_bool midi_pause_file(p_soundformat_item o)
{
   switch (mid_pause)
   {
      case FALSE:                            // pause
         mid_pause = TRUE;
         old_zeit = zeit;
         midi_pause();
         break;
      case TRUE:                             //plays after pause
         midi_resume();
         zeit = old_zeit;
         mid_pause = FALSE;
         break;
   }
         
   return(mid_pause);
}

l_bool midi_forward_file(p_soundformat_item o, l_int relpos)     // TODO: Fix
{
//   midi_seek(midi_pos+relpos);
   return(TRUE);
}

l_bool midi_rewind_file(p_soundformat_item o, l_int relpos)     // TODO: Fix
{
//   midi_seek(midi_pos-relpos);
   return(TRUE);
}

l_bool midi_set_pos(p_soundformat_item o, l_int pos)     // TODO: Fix
{
//   midi_seek(pos);
   return(TRUE);
}

l_int midi_get_pos(p_soundformat_item o)
{
   return(zeit);
}

l_int midi_get_len_file(p_soundformat_item o)
{
   return(0); //midi_loop_end - midi_loop_start);
}

l_bool midi_is_stereo(p_soundformat_item o)
{
   return(TRUE);
}

l_bool midi_get_file_info(p_soundformat_item o, SFAINFO *inf)
{
   inf->freq = 0;
   inf->bits = 0;
   inf->channels = 0;
   inf->bitrate = 0;

   inf->title[0] = NULL;
   inf->album[0] = NULL;
   inf->author[0] = NULL;
   inf->comment[0] = NULL;
   inf->genre[0] = NULL;

   return(TRUE);
}

l_text midi_get_friendly_name(p_soundformat_item o)
{
   return(_strdup(filename));
}

l_bool midi_config_driver(p_soundformat_item o)
{
   return(midi_display_about(o));
}

l_bool midi_display_about(p_soundformat_item o)
{
   msgbox(MW_INFO, MB_OK, "MIDI Driver for SFA\n\nCopyright (c) Florian Xaver 2000\nCopyright (c) Owen Rudge 2001-2002");
   return(TRUE);
}

l_bool midi_close_file(p_soundformat_item o)
{
   destroy_midi(midi_sample);
   return(TRUE);
}

l_bool midi_terminate_driver(p_soundformat_item o)
{
   return(TRUE);
}

l_bool midi_send_custom_message(p_soundformat_item o, l_int msg, void *extra)
{
   return(FALSE);
}

void time_process()
{
   if (mid_pause == false)
      zeit++;
}

END_OF_FUNCTION(time_process);

//lib_exportable;
#include "sfamidi.exp"

SetInfoAppName("MIDI Driver");
SetInfoDesciption("MIDI Driver for SFA");
SetInfoCopyright("Copyright (c) Florian Xaver 2000, Owen Rudge 2001-2002");
SetInfoManufacturer("Florian Xaver, Owen Rudge");


lib_begin (void)
{
   if (ap_process == AP_ALLOC)
   {
      AP_EXPORTLIB();
   }

   if (ap_process == AP_INIT)
   {
      p_soundformat_item k = 0;

      AP_EXPORTLIB();

      install_int_ex(time_process, SECS_TO_TIMER(1));
      LOCK_VARIABLE(zeit);

      //init sound format list!!
      k = new_soundformat_item(                      ("mid"),
                                                     ("MIDI files"),
                                                     ("Copyright (c) Florian Xaver 2000, Owen Rudge 2001-2002"),
                                                     &midi_init_driver,
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
                                                     &midi_get_file_info,
                                                     &midi_get_friendly_name,
                                                     &midi_config_driver,
                                                     &midi_display_about,
                                                     &midi_close_file,
                                                     &midi_terminate_driver,
                                                     &midi_send_custom_message
                                                     );




      if (k) sound_format_list->insert(sound_format_list, k);
   }

   if (ap_process == AP_DONE)
   {

   }

   if (ap_process == AP_FREE)
   {

   }
} lib_end;