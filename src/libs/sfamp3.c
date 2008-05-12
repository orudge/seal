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

/************************************/
/* MP3 driver for SFA               */
/* Copyright (c) Florian Xaver 2000 */
/* Copyright (c) Owen Rudge 2001    */
/*                                  */
/* Uses code from GEMP3. Please see */
/* http://www.owenrudge.co.uk/GEM/  */
/* for more information.            */
/************************************/

/* Revision History:
 *
 * 16/09/2001: Added MP2 file extension support and updated new_soundformat_item call. Also
 *             added ID3 support. (orudge)
 *
 * 04/10/2001: Added about box (orudge)
 *
 * 10/11/2001: Revamped driver for new SFA version. Tidied up code, etc. (orudge)
 *
 * 05/04/2002: Added send_custom_message function (orudge)
 */

#include <stdio.h>
#include <seal.h>
#include <sound.h>
#include <dialogs.h>
#include <libamp.h>

#include "audio.h"
#include "getbits.h"
#include "formats.h"

extern struct AUDIO_HEADER amp_cur_header;

/*#define TXT_STEREO      INI_TEXT("Stereo")
#define TXT_MONO        INI_TEXT("Mono")
#define TXT_MP3INFO     INI_TEXT("MPEG %d Layer %d, %s, %dHz, %dkbps, %s")
#define TXT_SURROUND    INI_TEXT("Surround")
#define TXT_NOSURROUND  INI_TEXT("No Surround")

#define TXT_ID3INFO     INI_TEXT("MPEG %d Layer %d, %s, %dHz, %dkbps, %s\n\nTitle: %s\nAuthor: %s\nAlbum: %s\nComment: %s\nGenre: %s")*/
#define TXT_UNSPECIFIED INI_TEXT("Unspecified")

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

const int maxKnownGenre=sizeof(GenreStr)/sizeof(char *);

signed int mp3_pause = false;
signed int PausedPos = 0;
l_text filename;
l_bool libamp_installed = FALSE;

l_bool mp3_init_driver (p_soundformat_item o)
{
   if (install_amp() == FALSE)
   {
      seal_error(ERR_INFO, INI_TEXT("Cannot initialise MP3 decoder"));
      libamp_installed = FALSE;
      return(FALSE);
   }

   libamp_installed = TRUE;
   return(TRUE);
}

l_bool mp3_init_file (p_soundformat_item o, l_text file, l_bool play)
{
   if (filename)
      _free(filename);

   filename = _strdup(file);
         
   if (libamp_installed == FALSE)
   {
      seal_error(ERR_INFO, INI_TEXT("LibAmp not installed"));
      return(FALSE);
   }

   if (play == TRUE)
   {
      if (load_amp(filename, FALSE) == FALSE)
      {
         seal_error(ERR_INFO, INI_TEXT("Unable to load file."));
         return(FALSE);
      }
   }

   return(TRUE);
}

l_bool mp3_play_file (p_soundformat_item o)
{
   if (load_amp(filename, FALSE) == FALSE)
   {
      seal_error(ERR_INFO, INI_TEXT("Unable to load file."));
      return(FALSE);
   }

   return(TRUE);
}
         

l_bool mp3_poll_file(p_soundformat_item o)
{
   if (run_amp() == -1)
      return(FALSE);

   return(TRUE);
}

l_bool mp3_stop_file(p_soundformat_item o)
{
   stop_amp();
   return(TRUE);
}

l_bool mp3_pause_file(p_soundformat_item o)
{
   switch (mp3_pause)
   {
      case FALSE:                            // pause
         mp3_pause = TRUE;
         stop_amp();
         PausedPos=amp_frame;     
         break;
      case TRUE:                             //plays after pause
         seek_amp_abs(PausedPos);
         mp3_pause = FALSE;          
         break;
   }

   return(mp3_pause);
}

l_bool mp3_forward_file(p_soundformat_item o, l_int relpos)
{
   seek_amp_rel(relpos*amp_samprat/amp_pollsize);
   return(TRUE);
}

l_bool mp3_rewind_file(p_soundformat_item o, l_int relpos)
{
   seek_amp_rel(-relpos*amp_samprat/amp_pollsize);
   return(TRUE);
}

l_bool mp3_set_pos(p_soundformat_item o, l_int pos)
{
   seek_amp_abs(pos*amp_samprat/amp_pollsize);
   return(TRUE);
}

l_int mp3_get_pos(p_soundformat_item o)
{
   return(amp_time);
}

l_int mp3_get_len_file(p_soundformat_item o)
{
   long TotalLen, fileLen;
   FILE *f;

   f = fopen(filename,"rb");
  
   if (f)
   {
      fseek(f,-128,SEEK_END);
      fileLen = ftell(f);
      fclose(f);
   }

   TotalLen=(int)(fileLen/(amp_bitrate*1000.0)*8.0);
   return(TotalLen);
}

l_bool mp3_get_file_info(p_soundformat_item o, SFAINFO *inf)
{
   char title[31], author[31], album[35];
   unsigned char comment[31];
   char *genre = 0;
   char *buf;

   FILE *f;
   char buffer[128];
   long ret;

   title[0] = 0;
   author[0] = 0;
   album[0] = 0;
   comment[0] = 0;
   genre = "\0";

   f = fopen(filename,"rb");

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

   inf->freq = t_sampling_frequency[amp_cur_header.ID][amp_cur_header.sampling_frequency];
   inf->bits = 16; // assumed ;-)
   inf->channels = amp_stereo == TRUE ? 2 : 1;
   inf->bitrate = t_bitrate[amp_cur_header.ID][3-amp_cur_header.layer][amp_cur_header.bitrate_index];

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

   mp3_get_file_info(o, &inf);

   if (inf.title != NULL && inf.author != NULL)
      return(set_format_text(NULL, "%s - %s", inf.author, inf.title));
   else
      return(_strdup(filename));
}

l_bool mp3_config_driver(p_soundformat_item o)
{
   return(mp3_display_about(o));
}

l_bool mp3_display_about(p_soundformat_item o)
{
   msgbox(MW_INFO, MB_OK, "MP3 Driver for SFA\nVersion 1.0\n\nCopyright (c) Florian Xaver 2000\nCopyright (c) Owen Rudge 2001\n\nUses LibAmp plus some code from GEMP3\n(www.owenrudge.co.uk/GEM/)");
   return(0);
}

l_bool mp3_close_file(p_soundformat_item o)
{
   unload_amp();
   return(TRUE);
}

l_bool mp3_terminate_driver(p_soundformat_item o)
{
   return(TRUE);
}

l_bool mp3_send_custom_message(p_soundformat_item o, l_int msg, void *extra)
{
   return(FALSE);
}

//lib_exportable;
#include "sfamp3.exp"

SetInfoAppName("MP3 Codec");
SetInfoDesciption("MP3 Codec for SFA");
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
                                                     &mp3_terminate_driver,
                                                     &mp3_send_custom_message
                                                     );

      k2 = new_soundformat_item(                     ("mp2"),
                                                     ("MP2 files"),
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
                                                     &mp3_terminate_driver,
                                                     &mp3_send_custom_message
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
