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

/************************************************************/
/* WAV driver for SFA                                       */
/* Version 0.1                                              */
/*                                                          */
/* Copyright (c) Owen Rudge 2001-2002. All Rights Reserved. */
/************************************************************/

/* Revision History:
 *
 * 24/11/2001: Made compatible with new version of SFA (orudge)
 * 23/03/2002: Added support for new "extra" item in structure (orudge)
 * 05/04/2002: Added send_custom_message function (orudge)
 */

#include <allegro.h>
#include <stdio.h>
#include "seal.h"
#include "sound.h"
#include "dialogs.h"

#define TXT_WAVINFO    INI_TEXT("%dHz, %d bit, %s")
#define TXT_STEREO     INI_TEXT("stereo")
#define TXT_MONO       INI_TEXT("mono")

typedef struct {
   int paused;

   int freq;
   int bits;
   int channels;

   int len, length;
   l_text filename;
   int scnt, ssize;

   PACKFILE *f;
   AUDIOSTREAM *theme;

   int pos;
   l_bool loaded;
} WAV_DATA;

/*int paused = FALSE;

int freq = 22050;
int bits = 8;
int channels = 1;

int len, length;
l_text filename = NULL;*/

#define BUFFER_SIZE(s) (s->freq * (s->bits/8) * s->channels)

#define BITS(s)     s->bits
#define RATE(s)     s->freq
#define STEREO(s)   ((s->channels == 2) ? TRUE : FALSE)

/*int scnt,ssize;
PACKFILE *f;
AUDIOSTREAM *theme;*/

void InitStream(WAV_DATA *);
void StreamAudio(WAV_DATA *);
void wav_timer();

volatile int wavpos;

//l_bool loaded = FALSE;

void wav_timer(void)
{
//   if (paused == FALSE)
      wavpos++;
}

END_OF_FUNCTION(wav_timer);

void InitStream(WAV_DATA *wd)
{
   char buffer[25];
   int i;
   
   wd->f = pack_fopen(wd->filename,"rb");

   if (!wd->f)
      return;

   pack_fread(buffer, 12, wd->f);          /* check RIFF header */
   if (memcmp(buffer, "RIFF", 4) || memcmp(buffer+8, "WAVE", 4))
      goto getout;

   while (!pack_feof(wd->f))
   {
      if (pack_fread(buffer, 4, wd->f) != 4)
	   break;

      wd->length = pack_igetl(wd->f);          /* read chunk length */

      if (memcmp(buffer, "fmt ", 4) == 0)
      {
	   i = pack_igetw(wd->f);            /* should be 1 for PCM data */
	   wd->length -= 2;
	   if (i != 1) 
	      goto getout;

         wd->channels = pack_igetw(wd->f);     /* mono or stereo data */
         wd->length -= 2;

         if ((wd->channels != 1) && (wd->channels != 2))
            goto getout;

         wd->freq = pack_igetl(wd->f);         /* sample frequency */
         wd->length -= 4;

         pack_igetl(wd->f);                /* skip six bytes */
         pack_igetw(wd->f);
         wd->length -= 6;

         wd->bits = pack_igetw(wd->f);         /* 8 or 16 bit data? */
         wd->length -= 2;

         if ((wd->bits != 8) && (wd->bits != 16))
            goto getout;
      }
      else if (memcmp(buffer, "data", 4) == 0)
      {
         wd->len = wd->length / wd->channels;

         if (wd->bits == 16)
            wd->len /= 2;

         goto notwav; //return(1); // we're just where we want to be, so let's go
      }
   }

notwav: ;
   //printf("%s - %dHz, %d bit, %s\n", filename, freq, bits, ((channels == 2) ? "stereo" : "mono"));
   wd->loaded = TRUE;   
   return;

getout: ;
   pack_fclose(wd->f);
   return;
}

void StreamAudio(WAV_DATA *wd)
{
   unsigned char *buff;
   unsigned short *buf2;
   int i;

   buff = get_audio_stream_buffer(wd->theme);
   if ( buff != NULL )
   {
      if (wd->scnt >= wd->ssize)
      {
//         rewind(sfp);
           wd->scnt = -1;
      }

      wd->scnt += pack_fread(buff,BUFFER_SIZE(wd),wd->f);
      if (wd->bits == 16)
      {
        // transform
         buf2 = buff;
         for (i = 0; i < (BUFFER_SIZE(wd) / 2); i++)
         {
            *buf2 ^= 0x8000; // xor with 0x8000 to transform from unsigned to signed format
            buf2++;
         }
      }
      free_audio_stream_buffer(wd->theme);
   }
}

/* wave_init_driver: Initialisation function for WAV driver */
l_bool wave_init_driver (p_soundformat_item o)
{
   return(TRUE);
}

/* wave_init_file: Initialisation function for WAV file */
l_int wave_init_file (p_soundformat_item o, l_text file, l_bool play)
{
   char buffer[25];
   WAV_DATA *wav_data;

   o->extra = malloc(sizeof(WAV_DATA));
   wav_data = (WAV_DATA *) o->extra;

/*   if (filename)
      _free(filename);*/

   wav_data->filename = _strdup(file);
   wav_data->pos = 0;

   InitStream(wav_data);

   if (play && wav_data->loaded)
      wave_play_file(o);
}

l_bool wave_play_file (p_soundformat_item o)
{
   WAV_DATA *wd;

   wd = (WAV_DATA *) o->extra;

   if (wd->loaded == FALSE)
      return(FALSE);

   wd->theme = play_audio_stream((BUFFER_SIZE(wd)/(wd->bits/8)/wd->channels),wd->bits,STEREO(wd),wd->freq,255,128); // 1= BUFFER_SIZE
   wd->scnt = 0;

   install_int(wav_timer, 1000);
}

l_int wave_stop_file(p_soundformat_item o)
{
   WAV_DATA *wd;

   wd = (WAV_DATA *) o->extra;

   stop_audio_stream(wd->theme);
   remove_int(wav_timer);

   return(1);
}

l_int wave_poll_file(p_soundformat_item o)
{
   WAV_DATA *wd;

   wd = (WAV_DATA *) o->extra;

   StreamAudio(wd);

   if (wd->scnt == -1)
      return(0);
   else
      return(1);
}

l_int wave_pause_file(p_soundformat_item o)
{
   return(1);
}

l_int wave_forward_file(p_soundformat_item o, l_int relpos)
{
   return(1);
}

l_int wave_rewind_file(p_soundformat_item o, l_int relpos)
{
   return(1);
}

l_int wave_set_pos(p_soundformat_item o, l_int pos)
{
   return(1);
}

l_int wave_get_pos(p_soundformat_item o)
{
   return(wavpos);
}

l_int wave_get_len_file(p_soundformat_item o)
{
   WAV_DATA *wd;

   wd = (WAV_DATA *) o->extra;
   return(wd->len/wd->freq);
}

l_bool wave_get_file_info(p_soundformat_item o, SFAINFO *inf)
{
   WAV_DATA *wd;

   wd = (WAV_DATA *) o->extra;

   inf->freq = wd->freq;
   inf->bits = wd->bits;
   inf->channels = wd->channels;
   inf->bitrate = 0; // TODO: fix - file len / freq 

   inf->title[0] = 0;
   inf->album[0] = 0;
   inf->author[0] = 0;
   inf->comment[0] = 0;
   inf->genre[0] = 0;

   return(TRUE);
}

l_text wave_get_friendly_name(p_soundformat_item o)
{
   WAV_DATA *wd;

   wd = (WAV_DATA *) o->extra;
   return(_strdup(wd->filename));
}

l_bool wave_config_driver(p_soundformat_item o)
{
   return(wave_display_about(o));
}

l_int wave_display_about(p_soundformat_item o)
{
   msgbox(MW_INFO, MB_OK, "WAVE Driver for SFA\nVersion 0.11\n\nCopyright (c) Owen Rudge 2001\nAll Rights Reserved.\n\nIMPORTANT\nThis is a prerelease version. It has bugs,\nand is NOT complete.");
   return(0);
}

l_bool wave_close_file(p_soundformat_item o)
{
   WAV_DATA *wd;

   wd = (WAV_DATA *) o->extra;
   pack_fclose(wd->f);

   free(o->extra);
   return(TRUE);
}

l_bool wave_terminate_driver(p_soundformat_item o)
{
   return(TRUE);
}

l_bool wave_send_custom_message(p_soundformat_item o, l_int msg, void *extra)
{
   return(FALSE);
}

#include "sfawav.exp"

SetInfoAppName("WAV Driver");
SetInfoDesciption("WAV Driver for SFA");
SetInfoCopyright("Copyright (c) Owen Rudge 2001-2002");
SetInfoManufacturer("Owen Rudge");

lib_begin(void)
{
   if (ap_process == AP_ALLOC)
   {
      AP_EXPORTLIB();
   }
   else if (ap_process == AP_INIT)
   {
      p_soundformat_item k = 0; //, k2 = 0;

      AP_EXPORTLIB();    

      k = new_soundformat_item(                      ("wav"),
                                                     ("WAVE files"),
                                                     ("Copyright (c) Owen Rudge 2001"),
                                                     &wave_init_driver,
                                                     &wave_init_file,
                                                     &wave_play_file,
                                                     &wave_poll_file,
                                                     &wave_stop_file,
                                                     &wave_pause_file,
                                                     &wave_forward_file,
                                                     &wave_rewind_file,
                                                     &wave_set_pos,
                                                     &wave_get_pos,
                                                     &wave_get_len_file,
                                                     &wave_get_file_info,
                                                     &wave_get_friendly_name,
                                                     &wave_config_driver,
                                                     &wave_display_about,
                                                     &wave_close_file,
                                                     &wave_terminate_driver,
                                                     &wave_send_custom_message
                                                     );

     if (k) sound_format_list->insert(sound_format_list, k);

/*      k2 = new_soundformat_item(                      ("voc"),
                                                     ("Creative Labs VOC files"),
                                                     ("Copyright (c) Owen Rudge 2001"),
                                                     &wave_init_file,
                                                     &wave_play_file,
                                                     &wave_poll_file,
                                                     &wave_stop_file,
                                                     &wave_pause_file,
                                                     &wave_forward_file,
                                                     &wave_rewind_file,
                                                     &wave_set_pos,
                                                     &wave_get_pos,
                                                     &wave_get_len_file,
                                                     &wave_get_time,
                                                     &wave_is_stereo,
                                                     &wave_get_additional_info,
                                                     &wave_display_about
                                                     );

     if (k2) sound_format_list->insert(sound_format_list, k2);*/

  }
  else if (ap_process == AP_DONE)
  {
  }
  else if (ap_process == AP_FREE)
  {
  }
} lib_end;
