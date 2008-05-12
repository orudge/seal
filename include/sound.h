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

/*****************************************************/
/* SFA Version 1.0                                   */
/* Copyright (c) Florian Xaver 2000                  */
/* Copyright (c) Owen Rudge 2001-2002                */
/*****************************************************/

/*
  t_soundformat_item is used as an item for p_list in t_soundformat object.
  for more information, how t_list really works, see list.h "t_list" object.
*/

#ifndef SFA__SOUND_H_INCLUDED

#define SFA__SOUND_H_INCLUDED

typedef struct {
   int freq;         // frequency in hertz
   int bits;         // bits
   int channels;     // channels (mono/stereo)
   int bitrate;      // bitrate

   char title[50];   // title
   char album[50];   // album
   char author[50];  // author
   char comment[50]; // comment
   char genre[50];   // genre
} SFAINFO;

typedef struct t_soundformat_item *p_soundformat_item;

typedef struct t_soundformat_item {
   l_text extension;
   l_text filedesc;
   l_text info;

   l_bool (*init_driver) (p_soundformat_item o);
   l_bool (*init_file) (p_soundformat_item o, l_text file, l_bool play);
   l_bool (*play_file) (p_soundformat_item o);
   l_bool (*poll_file) (p_soundformat_item o);
   l_bool (*stop_file) (p_soundformat_item o);
   l_bool (*pause_file) (p_soundformat_item o);
   l_bool (*forward_file) (p_soundformat_item o, l_int time);
   l_bool (*rewind_file) (p_soundformat_item o, l_int time);
   l_bool (*set_pos) (p_soundformat_item o, l_int time);
   l_int (*get_pos) (p_soundformat_item o);
   l_int (*get_len_file)(p_soundformat_item o);
   l_bool (*get_file_info)(p_soundformat_item o, SFAINFO *inf);
   l_text (*get_friendly_name)(p_soundformat_item o);
   l_bool (*config_driver)(p_soundformat_item o);
   l_bool (*display_about)(p_soundformat_item o);
   l_bool (*close_file)(p_soundformat_item o);
   l_bool (*terminate_driver)(p_soundformat_item o);
   l_bool (*send_custom_message)(p_soundformat_item o, l_int msg, void *extra);

   void *extra;            // pointer to instance-specific data, for the driver's internal use
} t_soundformat_item;

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
                                            );

void free_soundformat_item (void *o);


#define SOUNDFORMAT_ITEM_EXTENSION(o, i)  ((o)?((p_soundformat_item)(o)->at((o), i))->extension:NULL)

extern p_list sound_format_list;

typedef struct t_play *p_play;

typedef struct t_play {
   //file, which is played
   l_text filename;

   //pointer to library, which is used for this file
   p_soundformat_item used_library;

   l_bool (*init_driver) (p_play o);
   l_bool (*init_file) (p_play o, l_text file, l_bool play);
   l_bool (*play_file) (p_play o);
   l_bool (*poll_file) (p_play o);
   l_bool (*stop_file) (p_play o);
   l_bool (*pause_file) (p_play o);
   l_bool (*forward_file) (p_play o, l_int time);
   l_bool (*rewind_file) (p_play o, l_int time);
   l_bool (*set_pos) (p_play o, l_int time);
   l_int (*get_pos) (p_play o);
   l_int (*get_len_file)(p_play o);
   l_bool (*get_file_info)(p_play o, SFAINFO *inf);
   l_text (*get_friendly_name)(p_play o);
   l_bool (*config_driver)(p_play o);
   l_bool (*display_about)(p_play o);
   l_bool (*close_file)(p_play o);
   l_bool (*terminate_driver)(p_play o);
   l_bool (*send_custom_message)(p_play o, l_int msg, void *extra);
} t_play;

extern l_bool init_driver(p_play o);
extern l_bool init_file(p_play o, l_text file, l_bool play);
extern l_bool play_file(p_play o);
extern l_bool poll_file(p_play o);
extern l_bool stop_file(p_play o);
extern l_bool pause_file(p_play o);
extern l_bool forward_file(p_play o, l_int time);
extern l_bool rewind_file(p_play o, l_int time);
extern l_bool set_pos(p_play o, l_int time);
extern l_int get_pos(p_play o);
extern l_int get_len_file(p_play o);
extern l_bool get_file_info(p_play o, SFAINFO *inf);
extern l_text get_friendly_name(p_play o);
extern l_bool config_driver(p_play o);
extern l_bool display_about(p_play o);
extern l_bool close_file(p_play o);
extern l_bool terminate_driver(p_play o);
extern l_bool send_custom_message(p_play o, l_int msg, void *extra);

extern p_play SFA_init();

#ifdef SFA_INCLUDE_STANDARD_DRIVERS
   extern l_bool st_init_driver(p_soundformat_item o);
   extern l_bool st_init_file(p_soundformat_item o, l_text file, l_bool play);
   extern l_bool st_play_file(p_soundformat_item o);
   extern l_bool st_poll_file(p_soundformat_item o);
   extern l_bool st_stop_file(p_soundformat_item o);
   extern l_bool st_pause_file(p_soundformat_item o);
   extern l_bool st_forward_file(p_soundformat_item o, l_int time);
   extern l_bool st_rewind_file(p_soundformat_item o, l_int time);
   extern l_bool st_set_pos(p_soundformat_item o, l_int time);
   extern l_int st_get_pos(p_soundformat_item o);
   extern l_int st_get_len_file(p_soundformat_item o);
   extern l_bool st_get_file_info(p_soundformat_item o, SFAINFO *inf);
   extern l_text st_get_friendly_name(p_soundformat_item o);
   extern l_bool st_config_driver(p_soundformat_item o);
   extern l_bool st_display_about(p_soundformat_item o);
   extern l_bool st_close_file(p_soundformat_item o);
   extern l_bool st_terminate_driver(p_soundformat_item o);
   extern l_bool st_send_custom_message(p_soundformat_item o, l_int msg, void *extra);
#endif

extern l_int get_sound_input_installed();
extern l_int get_sound_installed();
extern void inc_count_player();
extern void dec_count_player();
extern l_int get_count_player();
extern l_int get_seal_volume();
extern l_int set_seal_volume(l_int s_v);
extern l_int get_seal_volume_midi();
extern l_int set_seal_volume_midi(l_int s_v);
extern l_int sound_input_installed;
extern l_int count_player;
extern l_int seal_volume;
extern l_int seal_volume_midi;

#endif // SFA__SOUND_H_INCLUDED