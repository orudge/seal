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

 /*********************************************************************
  Revision History:

  - 23/04/2001   Updates for Bad Seal compatible version, etc (orudge)
  - 02/09/2001   Updated for SEAL 2.0 kernel include (julien)
  - 16/09/2001   Updated for enhanced SFA Player (orudge)
  - 04/10/2001   Added support for About command (orudge)
  - 10/11/2001   Upped version to 1.0 and vastly improved API, cleaned
                 up code, etc - complete rewrite (orudge)
  - 23/03/2002   Added space in t_soundformat_item structure for driver
                 instance-specific data.
  - 03/03/2002 ? Integrated with SEAL 2.00.11    (orudge)
  - 05/04/2002   Added send_custom_message function (orudge)

  All dates are in UK format (DD/MM/YYYY)
  *********************************************************************
*/

#include <seal.h>
#include <app.h>
#include <sound.h>

#ifdef ___CPL_SUPPORT   // Control Panel support currently DOES NOT WORK!
   #include <cp.h>
#endif

#define TXT_INITSOUNDCARD        INI_TEXT("Launching sound driver: Initialising sound card...")
#define TXT_SOUNDCARD            INI_TEXT("Launching sound driver: Sound card found: %s")
#define TXT_CANNOTINITSOUNDCARD  INI_TEXT("Cannot initialise the soundcard: %s\n\nPlease edit allegro.cfg!\n")
#define TXT_CANNOTINITSOUNDINPUT INI_TEXT("Cannot install sound input: %s")

#ifdef ___CPL_SUPPORT
   #define TXT_SFACPL           INI_TEXT("SFA")
   #define TXT_SFACPLSETTINGS   INI_TEXT("SFA Driver Configuration")
   #define TXT_SFACPLINFO       INI_TEXT("Configuration of SFA drivers")

   p_cp_section cpl_section;
#endif

/* sound format item functions */

p_list sound_format_list;

/*****************************************************************
   Function: p_soundformat_item new_soundformat_item(<SEE BELOW>)

   Adds a new format to the sound format list.
   Returns a pointer to the soundformat object.
 *****************************************************************/

p_soundformat_item  new_soundformat_item(   l_text extension,
                                            l_text filedesc,
                                            l_text info,
                                            l_bool (*init_driver) (p_soundformat_item o),
                                            l_bool (*init_file) (p_soundformat_item o, l_text file, l_bool play),
                                            l_bool (*play_file) (p_soundformat_item o),
                                            l_bool (*poll_file) (p_soundformat_item o),                                            
                                            l_bool (*stop_file) (p_soundformat_item o),
                                            l_bool (*pause_file) (p_soundformat_item o),
                                            l_bool (*forward_file) (p_soundformat_item o, l_int time),
                                            l_bool (*rewind_file) (p_soundformat_item o, l_int time),
                                            l_bool (*set_pos) (p_soundformat_item o, l_int time),
                                            l_int (*get_pos) (p_soundformat_item o),
                                            l_int (*get_len_file)(p_soundformat_item o),
                                            l_bool (*get_file_info)(p_soundformat_item o, SFAINFO *inf),
                                            l_text (*get_friendly_name)(p_soundformat_item o),
                                            l_bool (*config_driver)(p_soundformat_item o),
                                            l_bool (*display_about)(p_soundformat_item o),
                                            l_bool (*close_file)(p_soundformat_item o),
                                            l_bool (*terminate_driver)(p_soundformat_item o),
                                            l_bool (*send_custom_message)(p_soundformat_item o, l_int msg, void *extra)
                                            )
{
   p_soundformat_item i = (p_soundformat_item)_malloc(sizeof(t_soundformat_item));

   if (i)
   {
      i->extension = _strdup(extension);
      i->info = _strdup(info);            // OCR: strdup to _strdup
      i->filedesc = _strdup(filedesc);    // OCR: strdup to _strdup

      i->init_driver = init_driver;
      i->init_file = init_file;
      i->play_file = play_file;
      i->poll_file = poll_file;
      i->stop_file = stop_file;
      i->pause_file = pause_file;
      i->forward_file = forward_file;
      i->rewind_file = rewind_file;
      i->set_pos = set_pos;
      i->get_pos = get_pos;
      i->get_len_file = get_len_file;
      i->get_file_info = get_file_info;
      i->get_friendly_name = get_friendly_name;
      i->config_driver = config_driver;
      i->display_about = display_about;
      i->close_file = close_file;
      i->terminate_driver = terminate_driver;
   }

   return(i);
}

void free_soundformat_item(void *o)
{
   if (o)
   {
      if (((p_soundformat_item)o)->extension)_free(((p_soundformat_item)o)->extension);
      if (((p_soundformat_item)o)->info) _free(((p_soundformat_item)o)->info);
      if (((p_soundformat_item)o)->filedesc) _free(((p_soundformat_item)o)->filedesc);

      _free(o);
   }
}


/* Dummy functions */

l_bool st_init_driver(p_soundformat_item o)
{
   return(FALSE);
}

l_bool st_init_file(p_soundformat_item o, l_text file, l_bool play)
{
   return(FALSE);
}

l_bool st_play_file(p_soundformat_item o)
{
   return(FALSE);
}

l_bool st_poll_file(p_soundformat_item o)
{
   return(FALSE);
}

l_bool st_stop_file(p_soundformat_item o)
{
   return(FALSE);
}

l_bool st_pause_file(p_soundformat_item o)
{
   return(FALSE);
}

l_bool st_forward_file(p_soundformat_item o, l_int time)
{
   return(FALSE);
}

l_bool st_rewind_file(p_soundformat_item o, l_int time)
{
   return(FALSE);
}

l_bool st_set_pos(p_soundformat_item o, l_int time)
{
   return(FALSE);
}

l_int st_get_pos(p_soundformat_item o)
{
   return(0);
}

l_int st_get_len_file(p_soundformat_item o)
{
   return(0);
}

l_bool st_get_file_info(p_soundformat_item o, SFAINFO *inf)
{
   return(FALSE);
}

l_text st_get_friendly_name(p_soundformat_item o)
{
   return(NULL);
}

l_bool st_config_driver(p_soundformat_item o)
{
   msgbox(MW_INFO, MB_OK, "This driver does not have any user-configurable options.");
   return(FALSE);
}

l_bool st_display_about(p_soundformat_item o)
{
   return(FALSE);
}

l_bool st_close_file(p_soundformat_item o)
{
   return(FALSE);
}

l_bool st_terminate_driver(p_soundformat_item o)
{
   return(FALSE);
}

l_bool st_send_custom_message(p_soundformat_item o, l_int msg, void *extra)
{
   return(FALSE);
}

/**************************************************
   Function: p_play SFA_init()

   Initialises SFA and returns a p_play object.
 **************************************************/

p_play SFA_init()
{
   p_play p = (p_play)_malloc(sizeof(t_play));

   if (p)
   {
      p->init_driver = &init_driver;
      p->init_file = &init_file;
      p->play_file = &play_file;
      p->poll_file = &poll_file;
      p->stop_file = &stop_file;
      p->pause_file = &pause_file;
      p->forward_file = &forward_file;
      p->rewind_file = &rewind_file;
      p->set_pos = &set_pos;
      p->get_pos = &get_pos;
      p->get_len_file = &get_len_file;
      p->get_file_info = &get_file_info;
      p->get_friendly_name = &get_friendly_name;
      p->config_driver = &config_driver;
      p->display_about = &display_about;
      p->close_file = &close_file;
      p->terminate_driver = &terminate_driver;
      p->send_custom_message = &send_custom_message;

      p->filename = 0;
      p->used_library = 0;
   }

   return(p);
}

/**************************************************
   Function: l_bool init_driver(p_play o)

   Initialises the driver for a single session.
   Returns TRUE on success, FALSE otherwise.
 **************************************************/

l_bool init_driver(p_play o)
{
   p_item x = sound_format_list->first(sound_format_list);
   p_item f = x;
   signed int ok = 0;
   l_text pfile = GetFile(o->filename);
   l_int used_l = 0;

   o->used_library = 0;

   do
   {
      if (!stricmp(((p_soundformat_item)x->rec)->extension, get_extension(pfile)))
      {
         o->used_library = (p_soundformat_item)x->rec;               
         ok = 1;
      }
      

      x = x->next;


   } while (!ok && x != f);

   if (o->used_library)
   {
      used_l = (o->used_library)->init_driver(o->used_library);

#ifdef ___CPL_SUPPORT
      add_cp_loader((o->used_library)->filedesc, "", cpl_section, NULL, NULL, sfa_cpl_load, sfa_cpl_trans_ev, sfa_cpl_save);
#endif
   }
   else
   {
      DEBUG_printf("SFA Error: Unknown file type for \"%s\"\n", pfile);
      return(FALSE);
   }

   if (pfile) _free(pfile);
   return (used_l);
}

/**************************************************
   Function: l_bool init_file(p_play o, l_text file)

   Loads a file. If play is TRUE, it starts playing.
   Returns TRUE on success, FALSE otherwise.
 **************************************************/

l_bool init_file (p_play o, l_text file, l_bool play)
{
   p_item x = sound_format_list->first(sound_format_list);
   p_item f = x;
   signed int ok = 0;
   l_text pfile = GetFile(file);
   l_int used_l = 0;
          

   o->used_library = 0;

   do
   {
      if (!stricmp(((p_soundformat_item)x->rec)->extension , get_extension(pfile)))
      {
         o->used_library = (p_soundformat_item)x->rec;
         ok = 1;
      }
      
      x = x->next;
   } while (!ok && x != f);

   if (o->used_library)
      used_l = (o->used_library)->init_file(o->used_library, pfile, play);
   else
   {
      DEBUG_printf("SFA Error: Unknown file type for \"%s\"\n", pfile);
      return(FALSE);
   }

   if (pfile) _free(pfile);

   return(used_l);
}

/**************************************************
   Function: l_bool play_file(p_play o)

   Plays the file.
   Returns TRUE on success, FALSE otherwise.
 **************************************************/

l_bool play_file (p_play o)
{
   if (o->used_library)
      return((o->used_library)->play_file(o->used_library));
}

/**************************************************
   Function: l_bool poll_file(p_play o)

   Polls the driver. Should be called very often.
   Returns TRUE on success, FALSE otherwise.
 **************************************************/

l_bool poll_file (p_play o)
{
   if (o->used_library)
      return((o->used_library)->poll_file(o->used_library));
}
      
/**************************************************
   Function: l_bool stop_file(p_play o)

   Stops playing.
   Returns TRUE on success, FALSE otherwise.
 **************************************************/

l_bool stop_file(p_play o)
{
   if (o->used_library)
      return(o->used_library->stop_file(o->used_library));
}

/**************************************************
   Function: l_bool pause_file(p_play o)

   Pauses/unpauses the file.
   Returns TRUE if now paused, FALSE otherwise.
 **************************************************/

l_bool pause_file(p_play o)
{
   if (o->used_library)
      return((o->used_library)->pause_file(o->used_library));
}

/*****************************************************
   Function: l_bool forward_file(p_play o, l_int time)

   Fast-forwards the file by <time> seconds.
   Returns TRUE if on success, FALSE otherwise.
 *****************************************************/
l_bool forward_file(p_play o, l_int time)
{
   if (o->used_library)
      return((o->used_library)->forward_file(o->used_library, time));
}

/****************************************************
   Function: l_bool rewind_file(p_play o, l_int time)

   Rewinds the file by <time> seconds.
   Returns TRUE on success, FALSE otherwise.
 ****************************************************/
l_bool rewind_file(p_play o, l_int relpos)
{
   if (o->used_library)
      return((o->used_library)->rewind_file(o->used_library, relpos));
}

/****************************************************
   Function: l_bool set_pos(p_play o, l_int time)

   Sets the position of the file in seconds.
   Returns TRUE on success, FALSE otherwise.
 ****************************************************/
l_bool set_pos(p_play o, l_int pos)
{
   if (o->used_library)
      return((o->used_library)->set_pos(o->used_library, pos));
}

/******************************************************
   Function: l_int get_pos(p_play o)

   Returns the current position of the file in seconds.
 ******************************************************/
l_int get_pos(p_play o)
{
   if (o->used_library)
      return((o->used_library)->get_pos(o->used_library));
}

/******************************************************
   Function: l_int get_len_file(p_play o)

   Returns the length of the file in seconds.
 ******************************************************/
l_int get_len_file(p_play o)
{
   if (o->used_library)
      return((o->used_library)->get_len_file(o->used_library));
}

/**********************************************************
   Function: l_bool get_file_info(p_play o, SFAINFO *inf)

   Returns extra information on the file. Returns TRUE on
   success, or FALSE otherwise.
 **********************************************************/

l_bool get_file_info(p_play o, SFAINFO *sfa)
{
   if (o->used_library)
      return((o->used_library)->get_file_info(o->used_library, sfa));
}

/**************************************************
   Function: l_text get_friendly_name(p_play o)

   Returns a 'friendly name' for a file.
 **************************************************/

l_text get_friendly_name(p_play o)
{
   if (o->used_library)
      return((o->used_library)->get_friendly_name(o->used_library));
}

/**************************************************
   Function: l_bool config_driver(p_play o)

   Displays a configuration dialog for the driver.
   Returns TRUE on success, or FALSE otherwise.
 **************************************************/

l_bool config_driver(p_play o)
{
   if (o->used_library)
      return((o->used_library)->config_driver(o->used_library));
}

/**************************************************
   Function: l_bool display_about(p_play o)

   Displays an about dialog for the driver.
   Returns TRUE on success, or FALSE otherwise.
 **************************************************/

l_bool display_about(p_play o)
{
   if (o->used_library)
      return((o->used_library)->display_about(o->used_library));
}

/**************************************************
   Function: l_bool close_file(p_play o)

   Closes the current file.
   Returns TRUE on success, or FALSE otherwise.
 **************************************************/

l_bool close_file(p_play o)
{
   if (o->used_library)
      return((o->used_library)->close_file(o->used_library));
}

/**************************************************
   Function: l_bool terminate_driver(p_play o)

   Shuts down this "session" of the driver.
   Returns TRUE on success, or FALSE otherwise.
 **************************************************/

l_bool terminate_driver(p_play o)
{
   if (o->used_library)
      return((o->used_library)->terminate_driver(o->used_library));
}

/**********************************************************************
   Function: l_bool send_custom_message(p_play o, l_int msg, void *extra)

   Sends a custom message to the driver.
 **********************************************************************/

l_bool send_custom_message(p_play o, l_int msg, void *extra)
{
   if (o->used_library)
      return((o->used_library)->send_custom_message(o->used_library, msg, extra));
}

l_int seal_volume = 255;
l_int seal_volume_midi = 255;
l_int count_player = 0;
l_int sound_input_installed = false;
l_int sound_installed = true;

/**************************************************
   Function: l_int get_sound_input_installed()

   Returns >1 if sound input is installed
 **************************************************/

l_int get_sound_input_installed()
{
   return(sound_input_installed);
}

/****************************************************
   Function: l_int get_sound_installed()

   Returns TRUE if the sound routines are installed.
 ****************************************************/

l_int get_sound_installed()
{
   return(sound_installed);
}



/**************************************************
   Function: void inc_count_player()

   Must be called if your program starts playing a
   file.
 **************************************************/

void inc_count_player()
{
   count_player++;
}

/***************************************************
   Function: void dec_count_player()

   Must be called if your program finishes playing a
   file.
 ***************************************************/

void dec_count_player()
{
   count_player--;
}

/**************************************************
   Function: l_int get_count_player()

   Returns the number of files playing.
 **************************************************/

l_int get_count_player()
{
   return(count_player);
}

/**************************************************
   Function: l_int get_seal_volume()

   Returns the current volume.
 **************************************************/
l_int get_seal_volume()
{
   return(seal_volume);
}

/**************************************************
   Function: l_int set_seal_volume()

   Sets the volume.
 **************************************************/

l_int set_seal_volume(l_int s_v)
{
   if (s_v < 0)
      s_v = 0;
   else if (s_v > 255)
      s_v = 255;

   seal_volume = s_v;
   set_volume(seal_volume,seal_volume_midi);

   // Write to config file
   set_config_file(GetFile("/system/allegro.cfg"));
   set_config_int("sound", "digi_volume", s_v);
}

/**************************************************
   Function: l_int get_seal_volume_midi()

   Returns the current MIDI volume.
 **************************************************/
l_int get_seal_volume_midi()
{
   return(seal_volume_midi);
}

/**************************************************
   Function: l_int set_seal_volume_midi()

   Sets the volume.
 **************************************************/

l_int set_seal_volume_midi(l_int s_v)
{
   if (s_v < 0)
      s_v = 0;
   else if (s_v > 255)
      s_v = 255;

   seal_volume_midi = s_v;
   set_volume(seal_volume,seal_volume_midi);

   // Write to config file
   set_config_file(GetFile("/system/allegro.cfg"));
   set_config_int("sound", "midi_volume", s_v);
}

#ifdef ___CPL_SUPPORT
p_cp_section get_sfa_cpl_section()
{
   return(cpl_section);
}
#endif

void ini_sound (void)
{
   p_soundformat_item k;
   l_text path = 0;
   t_rect r;

#ifdef ___CPL_SUPPORT
   cpl_section = add_cp_section(TXT_SFACPL, TXT_SFACPLSETTINGS, TXT_SFACPLINFO, NULL, load_image("bmp/cp_sfa.ico"));
#endif

   sound_format_list = list_init(_malloc(sizeof(t_list)), &free_soundformat_item, 0);
      

   k = new_soundformat_item(                      _strdup("*"),
                                                  _strdup("All Files"),
                                                  _strdup("(c) Florian Xaver GNU GPL - virtual sound file"),
                                                  &st_init_driver,
                                                  &st_init_file,
                                                  &st_play_file,
                                                  &st_poll_file,
                                                  &st_stop_file,
                                                  &st_pause_file,
                                                  &st_forward_file,
                                                  &st_rewind_file,
                                                  &st_set_pos,
                                                  &st_get_pos,
                                                  &st_get_len_file,
                                                  &st_get_file_info,
                                                  &st_get_friendly_name,
                                                  &st_config_driver,
                                                  &st_display_about,
                                                  &st_close_file,
                                                  &st_terminate_driver,
                                                  &st_send_custom_message
                                                  );




   sound_format_list->insert(sound_format_list, k);

   DEBUG_printf("Sound init: init sound...\n");
      
   /* reserve_voices is very important for the MOD player */
   reserve_voices (32, -1);
      
   DEBUG_printf("Sound init: voices reserved.\n");
   DEBUG_printf("Sound init: Installing sound...");

   if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "") == -1)
   {
      sound_installed = false;
      seal_error(ERR_INFO, TXT_CANNOTINITSOUNDCARD, allegro_error);
      DEBUG_printf("SFA Initialisation Error: Cannnot initialise sound card: %s\n",allegro_error);

      return;
   }

   DEBUG_printf("   sound installed!\n");

   set_config_file(GetFile("/system/allegro.cfg"));
   set_seal_volume(get_config_int("sound", "digi_volume", 255));
   set_seal_volume_midi(get_config_int("sound", "midi_volume", 255));

   DEBUG_printf("Sound init: volume set");
}
