/*
  t_soundformat_item is used as an item for p_list in t_soundformat object.
  for more information, how t_list really works, see list.h "t_list" object.
*/

typedef struct t_soundformat_item *p_soundformat_item;

typedef struct t_soundformat_item {

   //extension of the supported file
   l_text extension;

   //info - only an infomessage
   l_text info;

   //init file
   l_int (*init_file)(p_soundformat_item o, l_text file);

   //play file funktion
   l_int (*play_file) (p_soundformat_item o);

   //poll funktion, must be launched as often as you can
   l_int (*poll_file)(p_soundformat_item o);

   //stop file
   l_int (*stop_file)(p_soundformat_item o);

   //pause file
   l_int (*pause_file)(p_soundformat_item o);

   //forward
   l_int (*forward_file)(p_soundformat_item o, l_int relpos);

   //rewind
   l_int (*rewind_file)(p_soundformat_item o, l_int relpos);

   //set pos
   l_int (*set_pos)(p_soundformat_item o, l_int pos);

   //get current pos
   l_int (*get_pos)(p_soundformat_item o);

   //lenght of file
   l_int (*get_len_file)(p_soundformat_item o);


   //current time (in second)
   l_int (*get_time)(p_soundformat_item o);

   //current time (in second)
   l_int (*is_stereo)(p_soundformat_item o);

   //get additional info about the file
   l_text (*get_additional_info)(p_soundformat_item o);
   


} t_soundformat_item;



p_soundformat_item  new_soundformat_item(  l_text extension,
                                            l_text info,
                                            l_int (*init_file) (p_soundformat_item o, l_text file),
                                            l_int (*play_file) (p_soundformat_item o),
                                            l_int (*poll_file)(p_soundformat_item o),
                                            l_int (*stop_file)(p_soundformat_item o),
                                            l_int (*pause_file)(p_soundformat_item o),
                                            l_int (*forward_file)(p_soundformat_item o, l_int relpos),
                                            l_int (*rewind_file)(p_soundformat_item o, l_int relpos),
                                            l_int (*set_pos)(p_soundformat_item o, l_int pos),
                                            l_int (*get_pos)(p_soundformat_item o),
                                            l_int (*get_len_file)(p_soundformat_item o),
                                            l_int (*get_time)(p_soundformat_item o),
                                            l_int (*is_stereo)(p_soundformat_item o),
                                            l_text (*get_additional_info)(p_soundformat_item o)
                                            );

void                 free_soundformat_item ( void *o );


#define SOUNDFORMAT_ITEM_EXTENSION(o, i)  ((o)?((p_soundformat_item)(o)->at((o), i))->extension:NULL)

extern p_list sound_format_list;

/* If you want to play a file, you have to use this funktions*/

typedef struct t_play *p_play;

typedef struct t_play {

   //file, which is played
   l_text filename;

   //pointer to library, which is used for this file
   p_soundformat_item used_library;

   //init file
   l_int (*init_file)(p_play o, l_text file);

   // play file
   l_int (*play_file) (p_play o);

   // get file format info
   l_text     (*get_info)(p_play o);

   //
   l_int (*poll_file)(p_play o);

   //stop file
   l_int (*stop_file)(p_play o);

   //pause file
   l_int (*pause_file)(p_play o);

   //forward
   l_int (*forward_file)(p_play o, l_int relpos);

   //rewind
   l_int (*rewind_file)(p_play o, l_int relpos);

   //set pos
   l_int (*set_pos)(p_play o, l_int pos);

   //get current pos
   l_int (*get_pos)(p_play o);

   //lenght of file
   l_int (*get_len_file)(p_play o);

   //get current time in second
   l_int (*get_time)(p_play o);

   //if this file is in stereo or mono
   l_int (*is_stereo)(p_play o);

   //get additional info about the file
   l_text (*get_additional_info)(p_play o);
   

} t_play;

   l_int init_file(p_play o, l_text file);
   l_int play_file(p_play o);
   l_text     get_info(p_play o);
   l_int poll_file(p_play o);
   l_int stop_file(p_play o);
   l_int pause_file(p_play o);
   l_int forward_file(p_play o, l_int relpos);
   l_int rewind_file(p_play o, l_int relpos);
   l_int set_pos(p_play o, l_int pos);
   l_int get_pos(p_play o);
   l_int get_len_file(p_play o);
   l_int get_time(p_play o);
   l_int is_stereo(p_play o);
   l_text get_additional_info(p_play o);

p_play SFA_init();


