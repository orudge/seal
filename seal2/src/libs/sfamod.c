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

/******************************************/
/* MOD Driver for SFA                     */
/* Version 0.31                           */
/*                                        */
/* Copyright (c) Owen Rudge 2001-2002     */
/* All Rights Reserved                    */
/******************************************/

/* This software is licensed under the GNU General Public License. */

/* Revision History:
 *
 * 25/04/2001: Created driver
 * 16/09/2001: Updated new_soundformat_item call
 * 04/10/2001: Added about box
 * 11/11/2001: Updated driver for new SFA
 * 02/04/2002: Commented out test MOD length, etc, code (orudge)
 * 05/04/2002: Added send_custom_message function (orudge)
 */

#include "seal.h"
#include "sound.h"
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

l_bool mod_init_driver (p_soundformat_item o)
{
   install_mod(24);
   return(TRUE);
}

l_bool mod_init_file (p_soundformat_item o, l_text file, l_bool play)
{
   if (filename)
      _free(filename);

   filename = _strdup(file);

   LOCK_VARIABLE(modpos);
   LOCK_VARIABLE(paused);
   LOCK_FUNCTION(mod_timer);

   the_mod = load_mod(filename);

   if (the_mod == NULL)
   {
      seal_error(ERR_INFO, INI_TEXT("Cannot load file"));
      return(FALSE);
   }

   if (play == TRUE)
   {
      install_int(mod_timer, 1000);
      play_mod(the_mod, FALSE);
   }

   return(TRUE);
}

l_bool mod_play_file (p_soundformat_item o)
{
   if (the_mod != NULL)
   {
      install_int(mod_timer, 1000);
      play_mod(the_mod, FALSE);
      return(TRUE);
   }

   return(FALSE);
}
         

l_bool mod_poll_file(p_soundformat_item o)
{
   if (is_mod_playing() == FALSE)
   {
      remove_int(mod_timer);
      modpos = 0;
      return(FALSE);
   }

   return(TRUE);
}

l_bool mod_stop_file(p_soundformat_item o)
{
   stop_mod();
   remove_int(mod_timer);
   modpos = 0;

   return(TRUE);
}

l_bool mod_pause_file(p_soundformat_item o)
{
   toggle_pause_mode();
   paused = is_mod_paused();

   return(paused);
}

l_bool mod_forward_file(p_soundformat_item o, l_int relpos) /***** TODO: Fix *****/
{
//   goto_mod_track(mi.trk+((mi.bpm/60)/relpos));
   return(TRUE);
}

l_bool mod_rewind_file(p_soundformat_item o, l_int relpos) /***** TODO: Fix *****/
{
//   goto_mod_track(mi.trk-((mi.bpm/60)/relpos));
   return(TRUE);
}

l_bool mod_set_pos(p_soundformat_item o, l_int pos) /***** TODO: Fix *****/
{
//   goto_mod_track((mi.bpm/60)/pos);
   return(TRUE);
}

l_int mod_get_pos(p_soundformat_item o) /***** TODO: Fix *****/
{
   return(modpos); //return((mi.bpm/60)*mi.trk);
}

l_int mod_get_len_file(p_soundformat_item o) /***** TODO: Fix *****/
{
   return(0); //the_mod->no_trk*(mi.bpm/60));
}

l_bool mod_get_file_info(p_soundformat_item o, SFAINFO *inf)
{
   JGMOD_INFO ji;

   get_mod_info(filename, &ji);

   strcpy(inf->title, ji.name);
   strcpy(inf->comment, ji.type_name);

   inf->album[0] = 0;
   inf->author[0] = 0;
   inf->genre[0] = 0;

   inf->freq = 0;
   inf->bits = 0;
   inf->channels = 0;
   inf->bitrate = 0;

   return(TRUE);
}

l_text mod_get_friendly_name(p_soundformat_item o)
{
   JGMOD_INFO ji;

   get_mod_info(filename, &ji);

   if (strlen(ji.name) == 0)
      return(_strdup(filename));
   else
      return(_strdup(ji.name));
}

l_bool mod_config_driver(p_soundformat_item o)
{
   return(mod_display_about(o));
}

l_bool mod_display_about(p_soundformat_item o)
{
   msgbox(MW_INFO, MB_OK, "MOD Driver for SFA\nVersion 0.31\n\nCopyright (c) Owen Rudge 2001-2002\n\nUses JGMOD by Jeffery Guan.");
   return(TRUE);
}

l_bool mod_close_file(p_soundformat_item o)
{
   destroy_mod(the_mod);
   return(TRUE);
}

l_bool mod_terminate_driver(p_soundformat_item o)
{
   remove_mod(); // might as well, as we init it above....
   return(TRUE);
}

l_bool mod_send_custom_message(p_soundformat_item o, l_int msg, void *extra)
{
   return(FALSE);
}

//lib_exportable;
#include "sfamod.exp"

SetInfoAppName("MOD Driver");
SetInfoDesciption("MOD Driver for SFA");
SetInfoCopyright("Copyright (c) Owen Rudge 2001-2002");
SetInfoManufacturer("Owen Rudge");

lib_begin (void)
{
   if (ap_process == AP_ALLOC)
      AP_EXPORTLIB();

   if (ap_process == AP_INIT)
   {
      p_soundformat_item k, k2, k3, k4, k5;

      AP_EXPORTLIB();

      k = new_soundformat_item(                      ("mod"),
                                                     ("MOD files"),
                                                     ("Copyright (c) Owen Rudge 2001-2002"),
                                                     &mod_init_driver,
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
                                                     &mod_get_file_info,
                                                     &mod_get_friendly_name,
                                                     &mod_config_driver,
                                                     &mod_display_about,
                                                     &mod_close_file,
                                                     &mod_terminate_driver,
                                                     &mod_send_custom_message
                                                     );

      sound_format_list->insert(sound_format_list, k);

      k2 = new_soundformat_item(                     ("xm"),
                                                     ("XM files"),
                                                     ("Copyright (c) Owen Rudge 2001-2002"),
                                                     &mod_init_driver,
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
                                                     &mod_get_file_info,
                                                     &mod_get_friendly_name,
                                                     &mod_config_driver,
                                                     &mod_display_about,
                                                     &mod_close_file,
                                                     &mod_terminate_driver,
                                                     &mod_send_custom_message
                                                     );

      sound_format_list->insert(sound_format_list, k2);

      k3 = new_soundformat_item(                     ("s3m"),
                                                     ("S3M files"),
                                                     ("Copyright (c) Owen Rudge 2001-2002"),
                                                     &mod_init_driver,
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
                                                     &mod_get_file_info,
                                                     &mod_get_friendly_name,
                                                     &mod_config_driver,
                                                     &mod_display_about,
                                                     &mod_close_file,
                                                     &mod_terminate_driver,
                                                     &mod_send_custom_message
                                                     );

      sound_format_list->insert(sound_format_list, k3);

      k4 = new_soundformat_item(                     ("it"),
                                                     ("IT files"),
                                                     ("Copyright (c) Owen Rudge 2001-2002"),
                                                     &mod_init_driver,
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
                                                     &mod_get_file_info,
                                                     &mod_get_friendly_name,
                                                     &mod_config_driver,
                                                     &mod_display_about,
                                                     &mod_close_file,
                                                     &mod_terminate_driver,
                                                     &mod_send_custom_message
                                                     );


      sound_format_list->insert(sound_format_list, k4);

      k5 = new_soundformat_item(                     ("jgm"),
                                                     ("JGMOD files"),
                                                     ("Copyright (c) Owen Rudge 2001-2002"),
                                                     &mod_init_driver,
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
                                                     &mod_get_file_info,
                                                     &mod_get_friendly_name,
                                                     &mod_config_driver,
                                                     &mod_display_about,
                                                     &mod_close_file,
                                                     &mod_terminate_driver,
                                                     &mod_send_custom_message
                                                     );

      sound_format_list->insert(sound_format_list, k5);
   }

   if (ap_process == AP_DONE)
   {
      remove_int(mod_timer);
      modpos = 0;
   }

   if (ap_process == AP_FREE)
   {

   }
} lib_end;
