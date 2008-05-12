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

/***************************************/            // THIS VERSION IS BUGGY!
/* MP3 driver for SFA                  */
/* Copyright (c) Florian Xaver 2000    */
/* Copyright (c) Owen Rudge 2001-2002  */
/*                                     */
/* Uses code from GEMP3. Please see    */
/* http://www.owenrudge.co.uk/GEM/     */
/* for more information.               */
/***************************************/

/* Revision History:
 *
 * 16/09/2001: Added MP2 file extension support and updated new_soundformat_item call. Also
 *             added ID3 support. (orudge)
 *
 * 04/10/2001: Added about box (orudge)
 *
 * 10/11/2001: Revamped driver for new SFA version. Tidied up code, etc. (orudge)
 *
 * 02/04/2002: Used AllegroMP3 instead of LibAmp (orudge)
 */

#include <stdio.h>
#include <seal.h>
#include <sound.h>
#include <dialogs.h>
#include <almp3.h>

#define TXT_UNSPECIFIED         INI_TEXT("Unspecified")
#define TXT_ERROR_UNABLE_OPEN   INI_TEXT("Unable to open MP3.")
#define TXT_ERROR_UNABLE_READ   INI_TEXT("Unable to read from MP3.")
#define TXT_ERROR_UNABLE_CREATE INI_TEXT("Unable to create MP3 stream buffer.")

char *GenreStr[]={
"Blues","Classic Rock","Country","Dance","Disco","Funk","Grunge","Hip-Hop", // 0-7
"Jazz","Metal","New Age","Oldies","Other","Pop","R&B","Rap","Reggae","Rock",// 8-17
"Techno","Industrial","Alternative","Ska","Death Metal","Pranks",           // 18-23
"Soundtrack","Euro-Techno","Ambient","Trip-Hop","Vocal","Jazz+Funk",        // 24-29
"Fusion","Trance","Classical","Instrumental","Acid","House","Game",         // 30-36
"Sound Clip","Gospel","Noise","Alt. Rock","Bass","Soul","Punk","Space",     // 37-44
"Meditative","Instrumental Pop","Instrumental Rock","Ethnic","Gothic",      // 45-49
"Darkwave","Techno-Industrial","Electronic","Pop-Folk","Eurodance","Dream", // 50-55
"Southern Rock","Comedy","Cult","Gangsta Rap","Top 40","Christian Rap",     // 56-61
"Pop/Funk","Jungle","Native American","Cabaret","New Wave","Psychedelic",   // 62-67
"Rave","Showtunes","Trailer","Lo-Fi","Tribal","Acid Punk","Acid Jazz",      // 63-74
"Polka","Retro","Musical","Rock & Roll","Hard Rock","Folk","Folk/Rock",     // 75-81
"National Folk","Swing","Fast-Fusion","Bebob","Latin","Revival","Celtic",   // 82-88
"Bluegrass","Avantgarde","Gothic Rock","Progressive Rock",                  // 89-92
"Psychedelic Rock","Symphonic Rock","Slow Rock","Big Band","Chorus",        // 93-97
"Easy Listening","Acoustic","Humour","Speech","Chanson","Opera",            // 98-103
"Chamber Music","Sonata","Symphony","Booty Bass","Primus","Porn Groove",    // 104-109
"Satire","Slow Jam","Club","Tango","Samba","Folklore","Ballad",             // 110-116
"Power Ballad","Rhythmic Soul","Freestyle","Duet","Punk Rock","Drum Solo",  // 117-122
"Acapella","Euro-House","Dance Hall","Goa","Drum & Bass","Club-House",      // 123-128
"Hardcore","Terror","Indie","BritPop","Negerpunk","Polsk punk","Beat",      // 129-135
"Christian Gangsta Rap","Heavy Metal","Black Metal","Crossover",            // 136-139
"Contemporary Christian","Christian Rock"                                   // 140-141
};

#define DATASZ  (1<<15) /* (32768) amount of data to read from disk each time */
#define BUFSZ   (1<<16) /* (65536) size of audiostream buffer */

const int maxKnownGenre=sizeof(GenreStr)/sizeof(char *);

signed int mp3_pause = false;
//signed int PausedPos = 0;
//l_text filename;
//l_bool libamp_installed = FALSE;

typedef struct {
   l_bool paused;
   l_bool loaded;
   l_text filename;

   int buflen;

   PACKFILE *f;
   ALMP3_MP3STREAM *s;
} MP3_DATA;

l_bool mp3_init_driver (p_soundformat_item o)
{
   return(TRUE);
}

l_bool mp3_init_file (p_soundformat_item o, l_text file, l_bool play)
{
   MP3_DATA *mp3_data;
   PACKFILE *f = NULL;
   ALMP3_MP3STREAM *s = NULL;
   char data[DATASZ];
   l_text mp3_err;
   int len;

   o->extra = malloc(sizeof(MP3_DATA));
   mp3_data = (MP3_DATA *) o->extra;

   mp3_data->filename = _strdup(file);
         
   if (!(f = pack_fopen(mp3_data->filename, F_READ)))
   {
      mp3_err = TXT_ERROR_UNABLE_OPEN;
      goto error;
   }

   if ((len = pack_fread(data, DATASZ, f)) <= 0)
   {
      mp3_err = TXT_ERROR_UNABLE_READ;
      goto error;
   }

   if (len < DATASZ)
   {
      if (!(s = almp3_create_mp3stream(data, len, TRUE)))
      {
         mp3_err = TXT_ERROR_UNABLE_CREATE;
         goto error;
      }
   }
   else
   {
      if (!(s = almp3_create_mp3stream(data, DATASZ, FALSE)))
      {
         mp3_err = TXT_ERROR_UNABLE_CREATE;
         goto error;
      }
   }

   mp3_data->f = f;
   mp3_data->s = s;
   mp3_data->loaded = TRUE;
   mp3_data->paused = FALSE;

   if (play == TRUE)
      mp3_play_file(o);

   DEBUG_printf("SFAMP3: Got to end of mp3_init_file()\n");
   return(TRUE);

error:
   seal_error(ERR_INFO, mp3_err);

   pack_fclose(f);
   free(mp3_data);

   return(FALSE);
}

l_bool mp3_play_file (p_soundformat_item o)
{
   MP3_DATA *d;
   int ret;

   d = (MP3_DATA *) o->extra;

   if (d->loaded == TRUE)
   {
      ret = almp3_play_mp3stream(d->s, d->buflen, 255, 128);

      if (ret == ALMP3_OK)
         ret = TRUE;
      else
         ret = FALSE;
   }
   else
      ret = FALSE;

   return(ret);
}

l_bool mp3_poll_file(p_soundformat_item o)
{
   MP3_DATA *mp3_data;
   char *data;
   long len;

   mp3_data = (MP3_DATA *) o->extra;

   if (mp3_data->loaded == FALSE)
      return(FALSE);

   data = (char *) almp3_get_mp3stream_buffer(mp3_data->s);

   if (data)
   {
      len = pack_fread(data, DATASZ, mp3_data->f);

      if (len < DATASZ)
         almp3_free_mp3stream_buffer(mp3_data->s, len);
      else
         almp3_free_mp3stream_buffer(mp3_data->s, -1);
   }

   if (almp3_poll_mp3stream(mp3_data->s) == ALMP3_OK)
      return(TRUE);
   else
      return(FALSE);
}

l_bool mp3_stop_file(p_soundformat_item o)
{
   MP3_DATA *mp3_data;

   mp3_data = (MP3_DATA *) o->extra;

   if (mp3_data->loaded == FALSE)
      return(FALSE);

   almp3_stop_mp3stream(mp3_data->s);
   return(TRUE);
}

l_bool mp3_pause_file(p_soundformat_item o)
{
   MP3_DATA *mp3_data;

   mp3_data = (MP3_DATA *) o->extra;

   if (mp3_data->loaded == FALSE)
      return(FALSE);

   switch (mp3_data->paused)
   {
      case FALSE:                            // pause the file
         mp3_data->paused = TRUE;
         mp3_stop_file(o);
         break;
      case TRUE:                             // unpause the file
         mp3_data->paused = FALSE;
         mp3_play_file(o);
         break;
   }

   return(mp3_data->paused);
}

l_bool mp3_forward_file(p_soundformat_item o, l_int relpos)
{
   MP3_DATA *mp3_data;
   int pos;

   mp3_data = (MP3_DATA *) o->extra;

   if (mp3_data->loaded == FALSE)
      return(FALSE);

   pos = almp3_seek_rel_secs_mp3stream(mp3_data->s, relpos, file_size(mp3_data->filename));
   pack_fseek(mp3_data->f, pos);

   return(TRUE);
}

l_bool mp3_rewind_file(p_soundformat_item o, l_int relpos)
{
   MP3_DATA *mp3_data;
   int pos;

   mp3_data = (MP3_DATA *) o->extra;

   if (mp3_data->loaded == FALSE)
      return(FALSE);

   pos = almp3_seek_rel_secs_mp3stream(mp3_data->s, -relpos, file_size(mp3_data->filename));
   pack_fseek(mp3_data->f, pos);

   return(TRUE);
}

l_bool mp3_set_pos(p_soundformat_item o, l_int pos)
{
   MP3_DATA *mp3_data;
   int seek_pos;

   mp3_data = (MP3_DATA *) o->extra;

   if (mp3_data->loaded == FALSE)
      return(FALSE);

   seek_pos = almp3_seek_abs_secs_mp3stream(mp3_data->s, pos, file_size(mp3_data->filename));
   pack_fseek(mp3_data->f, pos);

   return(TRUE);
}

l_int mp3_get_pos(p_soundformat_item o)
{
   MP3_DATA *mp3_data;

   mp3_data = (MP3_DATA *) o->extra;
   return(almp3_get_pos_secs_mp3stream(mp3_data->s));
}

l_int mp3_get_len_file(p_soundformat_item o)
{
   MP3_DATA *mp3_data;

   mp3_data = (MP3_DATA *) o->extra;
   return(almp3_get_length_secs_mp3stream(mp3_data->s, file_size(mp3_data->filename)));
}

l_bool mp3_get_file_info(p_soundformat_item o, SFAINFO *inf)
{
   char title[31], author[31], album[35];
   unsigned char comment[31];
   char *genre = 0;
   char *buf;

   MP3_DATA *mp3_data;

   FILE *f;
   char buffer[128];
   long ret;

   mp3_data = (MP3_DATA *) o->extra;

   title[0] = 0;
   author[0] = 0;
   album[0] = 0;
   comment[0] = 0;
   genre = "\0";

   f = fopen(mp3_data->filename, "rb");

   if (!f)
      goto get_other_info;

   fseek(f, -128, SEEK_END);
   fread(buffer, 128, 1, f);
   ret = ftell(f);
  
   if (buffer[0] == 'T' && buffer[1] == 'A' && buffer[2] == 'G')
   {
      unsigned char genre_id = buffer[127];

      memcpy(title, buffer + 3,30);
      title[30] = 0;

      memcpy(author, buffer + 33,30);
      author[30] = 0;

      memcpy(album, buffer + 63, 34); // Plus year
      album[34] = 0;

      memcpy(comment, buffer + 97, 30);
      comment[30] = 0;

      if (genre_id >= maxKnownGenre)
         genre = TXT_UNSPECIFIED;
      else
         genre = GenreStr[genre_id];
   }

   fclose(f);

get_other_info: ;

   inf->freq = almp3_get_wave_freq_mp3stream(mp3_data->s);
   inf->bits = almp3_get_wave_bits_mp3stream(mp3_data->s);
   inf->channels = almp3_get_wave_is_stereo_mp3stream(mp3_data->s) == TRUE ? 2 : 1;
   inf->bitrate = almp3_get_bitrate_mp3stream(mp3_data->s) / 1000;

   strcpy(inf->title, title);
   strcpy(inf->album, album);
   strcpy(inf->author, author);
   strcpy(inf->comment, comment);
   strcpy(inf->genre, genre);

   return(TRUE);
}

l_text mp3_get_friendly_name(p_soundformat_item o)
{
   SFAINFO inf;
   MP3_DATA *mp3_data;

   mp3_data = (MP3_DATA *) o->extra;
   mp3_get_file_info(o, &inf);

   if (inf.title != NULL && inf.author != NULL)
      return(set_format_text(NULL, "%s - %s", inf.author, inf.title));
   else
      return(_strdup(mp3_data->filename));
}

l_bool mp3_config_driver(p_soundformat_item o)
{
   return(mp3_display_about(o));
}

l_bool mp3_display_about(p_soundformat_item o)
{
   msgbox(MW_INFO, MB_OK, "MP3 Driver for SFA\nVersion 1.1\n\nCopyright \xA9 Florian Xaver 2000\nCopyright \xA9 Owen Rudge 2001-2002\n\nUses AllegroMP3 plus some code from GEMP3\n(www.owenrudge.co.uk/GEM/)");
   return(0);
}

l_bool mp3_close_file(p_soundformat_item o)
{
   MP3_DATA *mp3_data;

   mp3_data = (MP3_DATA *) o->extra;

   pack_fclose(mp3_data->f);
   almp3_destroy_mp3stream(mp3_data->s);

   free(o->extra);

   return(TRUE);
}

l_bool mp3_terminate_driver(p_soundformat_item o)
{
   return(TRUE);
}

//lib_exportable;
#include "sfamp3.exp"

SetInfoAppName("MP3 Driver");
SetInfoDesciption("MP3 Driver for SFA");
SetInfoCopyright("Copyright (c) Florian Xaver 2000, Owen Rudge 2001-2002");
SetInfoManufacturer("Florian Xaver, Owen Rudge");

lib_begin (void)
{
   if (ap_process == AP_ALLOC)
      AP_EXPORTLIB();

   if (ap_process == AP_INIT)
   {
      p_soundformat_item k, k2;

      AP_EXPORTLIB();

      k = new_soundformat_item(                      ("mp3"),
                                                     ("MP3 files"),
                                                     ("Copyright (c) Florian Xaver 2000, Owen Rudge 2001-2002"),
                                                     &mp3_init_driver,
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
                                                     &mp3_get_file_info,
                                                     &mp3_get_friendly_name,
                                                     &mp3_config_driver,
                                                     &mp3_display_about,
                                                     &mp3_close_file,
                                                     &mp3_terminate_driver
                                                     );

      k2 = new_soundformat_item(                     ("mp2"),
                                                     ("MP2 files"),
                                                     ("Copyright (c) Florian Xaver 2000, Owen Rudge 2001"),
                                                     &mp3_init_driver,
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
                                                     &mp3_get_file_info,
                                                     &mp3_get_friendly_name,
                                                     &mp3_config_driver,
                                                     &mp3_display_about,
                                                     &mp3_close_file,
                                                     &mp3_terminate_driver
                                                     );

      sound_format_list->insert(sound_format_list, k);
      sound_format_list->insert(sound_format_list, k2);
   }

   if (ap_process == AP_DONE)
   {

   }

   if (ap_process == AP_FREE)
   {

   }
} lib_end;
