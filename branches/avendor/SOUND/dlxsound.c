/*

      Copyright (c) Florian Xaver 2000

      florianx@drdos.org

      SFA extended sound library for SEAL

      License: GPL



*/


#include"allegro.h"
#include"seal.h"
#include"app.h"
#include"dlxsound.h"
#include"dialogs.h"


#define TXT_INITSOUNDCARD        INI_TEXT("Launching sound driver, init soundcard ...")
#define TXT_SOUNDCARD            INI_TEXT("Launching sound driver, soundcard found: %s")
#define TXT_CANNOTINITSOUNDCARD  INI_TEXT("Cannot init the soundcard: %s\n\nPlease edit seal.cfg!")
#define TXT_CANNOTINITSOUNDINPUT INI_TEXT("Cannot install sound input: %s ")




/* sound format item functions */

/*
   make new sound format item

   look at dlxsound.h


*/
/*

p_soundformat_item  new_soundformat_item(   l_text extension,
                                            l_text info,
                                            l_int (*init_file) (l_text file),
                                            l_int (*play_file) (),
                                            l_int (*stop_file)(),
                                            l_int (*pause_file)(),
                                            l_int (*forward_file)(l_int relpos),
                                            l_int (*rewind_file)(l_int relpos),
                                            l_int (*set_pos)(l_int pos),
                                            l_int (*get_pos)(),
                                            l_int (*get_len_file)()
                                            )*/

// list of all sound format dlx loaded

p_list sound_format_list;


/**[txh]**********************************************************************
     
      Function: new_soundformat_item

      Description: for a library: Stores all funktions in a list
     
      Return: p_soundformat_item
      Example:       p_soundformat_item k = new_soundformat_item(("mp3"),
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

*****************************************************************************/

p_soundformat_item  new_soundformat_item(   l_text extension,
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
                                            )

                                            

{

  p_soundformat_item i = (p_soundformat_item)_malloc(sizeof(t_soundformat_item));

  if ( i ) {
     i->extension = _strdup(extension);
     i->info = strdup(info);

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
     i->get_time = get_time;
     i->is_stereo = is_stereo;
     i->get_additional_info = get_additional_info;

  };

  return i;

};

/* free soundformat item */

void  free_soundformat_item ( void *o )
{

   if ( o ) {

      if (((p_soundformat_item)o)->extension)_free(((p_soundformat_item)o)->extension);
      if (((p_soundformat_item)o)->info) _free(((p_soundformat_item)o)->info);

   };

};


/* standard funktions for sound formats, they do nothing*/

   //play file funktion
   l_int st_init_file (p_soundformat_item o, l_text file)
         {
         seal_error(ERR_INFO, "debug: init %s", file);
         return 1;
         };

   //play file funktion
   l_int st_play_file (p_soundformat_item o)
         {
         seal_error(ERR_INFO, "debug: play");
         return 1;
         };
         

   //poll file
   l_int st_poll_file(p_soundformat_item o)
         {
//         seal_error(ERR_INFO, "debug_ poll");
         return 1;
         };

   //stop file
   l_int st_stop_file(p_soundformat_item o)
         {
//         seal_error(ERR_INFO, "debug: stop");
         return 1;
         };

   //pause file
   l_int st_pause_file(p_soundformat_item o)
         {
//         seal_error(ERR_INFO, "File format isn't supported!");
         return 1;
         };

   //forward
   l_int st_forward_file(p_soundformat_item o, l_int relpos)
         {
//         seal_error(ERR_INFO, "File format isn't supported!");
         return 1;
         };

   //rewind
   l_int st_rewind_file(p_soundformat_item o, l_int relpos)
         {
//         seal_error(ERR_INFO, "File format isn't supported!");
         return 1;
         };

   //set pos
   l_int st_set_pos(p_soundformat_item o, l_int pos)
         {
//         seal_error(ERR_INFO, "File format isn't supported!");
         return 1;
         };

   //get current pos
   l_int st_get_pos(p_soundformat_item o)
         {
//         seal_error(ERR_INFO, "File format isn't supported!");
         return 1;
         };

   //lenght of file
   l_int st_get_len_file(p_soundformat_item o)
         {
//         seal_error(ERR_INFO, "File format isn't supported!");
         return 1;
         };

   // get current time
   l_int st_get_time(p_soundformat_item o)
         {
//         seal_error(ERR_INFO, "File format isn't supported!");
         return 1;
         };

   l_int st_is_stereo(p_soundformat_item o)
         {
//         seal_error(ERR_INFO, "File format isn't supported!");
         return 1;
         };

   l_text st_get_additional_info(p_soundformat_item o)
         {
         return _strdup("debugging!");
         }



/*-------------------------*/

// is the "Konstruktor"
//
// example:
//
//  p_play ex = SFA_init();
//
//  // Now you can play as many files as you want
//
//  ex->init_file(filename);
//  ex->play();
//  while (ex->poll());
//

     /**[txh]**********************************************************************
     
      Function: SFA_init

      Description: for a player: Init SFA in a player. Must be executed before playing a file.
     
      Return: p_play
      Example: p_play pp = SFA_init();
     
     *****************************************************************************/



p_play SFA_init()
{
   p_play p = (p_play)_malloc(sizeof(t_play));

   if (p)
     {
     p->init_file = &init_file;
     p->play_file = &play_file;
     p->poll_file = &poll_file;
     p->rewind_file = &rewind_file;
     p->forward_file = &forward_file;
     p->stop_file = &stop_file;
     p->pause_file = &pause_file;
     p->get_info = &get_info;
     p->set_pos = &set_pos;
     p->get_pos = &get_pos;
     p->get_len_file = &get_len_file;
     p->get_time = &get_time;
     p->is_stereo = &is_stereo;
     p->get_additional_info = &get_additional_info;

     p->filename = 0;
     p->used_library = 0;

     }


   return p;
}


     /**[txh]**********************************************************************
     
      Function: init_file

      Description: Init the file
     
      Return: 0 if error
      Example: p_play pp = 0;
               pp->init_file(pp, file)
     
     *****************************************************************************/


   //init file
l_int init_file (p_play o, l_text file)
       {


       
          p_item x = sound_format_list->first(sound_format_list);
          p_item f = x;
          signed int ok = 0;

          o->used_library = 0;

//          seal_error(ERR_INFO, "searching extension %s", get_extension(file));
   
          do {
          
//            seal_error(ERR_INFO, "searching: %s found", ((p_soundformat_item)x->rec)->extension);

            if (!stricmp( ((p_soundformat_item)x->rec)->extension , get_extension(file) ))
               {
               o->used_library = (p_soundformat_item)x->rec;               
//               seal_error(ERR_INFO, "Extension found: %s Used_library: %d Found_library: %d", ((p_soundformat_item)x->rec)->extension,
//               (o->used_library) ? 1 : 0,  ((p_soundformat_item)x->rec) ? 1 : 0);
               ok = 1;

               }
      

            x = x->next;


          } while ( !ok  && x != f );
//            } while (1);




         if (o->used_library != 0)
          {


//          seal_error(ERR_INFO, "Want to play file #2 [%s]", file);
          
          return (o->used_library)->init_file(o->used_library, file);
          }

//          else

//          seal_error(ERR_INFO, "Want to play file #2: error used_library == 0");

         

         return 0;

      };

     /**[txh]**********************************************************************
     
      Function: play_file

      Description: for a player: plays the file (must be init)
     
      Return: 0 if error
      Example: pp->play_file(pp)
     
     *****************************************************************************/


   //play file
l_int play_file (p_play o)
      {
      if (o->used_library != 0)
         {
         return (o->used_library)->play_file(o->used_library);
         }

      };

     /**[txh]**********************************************************************
     
      Function: poll_file

      Description: for a player: Must be called very often!
     
      Return: 0 if file finished
      Example: file_is_playing = pp->poll_file(pp);
     
     *****************************************************************************/


l_int poll_file (p_play o)
      {
      if (o->used_library != 0)
         {
         return (o->used_library)->poll_file(o->used_library);
         }
      };
      
     /**[txh]**********************************************************************
     
      Function: stop_file

      Description: for a player: stop playing the file
     
      Return:
      Example: pp->stop_file(pp);
     
     *****************************************************************************/


   //stop file
l_int stop_file(p_play o)
      {
      if (o->used_library != 0)
         {
         return (o->used_library)->stop_file(o->used_library);
         }
      };

     /**[txh]**********************************************************************
     
      Function: pause_file

      Description: for a player: pauses the file
     
      Return:
      Example: pp->pause(pp);
     
     *****************************************************************************/


   //pause file
l_int pause_file(p_play o)
      {
      if (o->used_library != 0)
         {
         return (o->used_library)->pause_file(o->used_library);
         }
      };

     /**[txh]**********************************************************************
     
      Function: forward_file

      Description: for a player: Set relative position (forward)
     
      Return:
      Example: pp->forward_file(pp, 10);
     
     *****************************************************************************/


   //forward
l_int forward_file(p_play o, l_int relpos)
      {
      if (o->used_library != 0)
         {
         return (o->used_library)->forward_file(o->used_library, relpos);
         }
      };

     /**[txh]**********************************************************************
     
      Function: rewind_file

      Description: for a player: Set relative position (rewind)
     
      Return:
      Example: pp->rewind_file(pp, 10);
     
     *****************************************************************************/


   //rewind
l_int rewind_file(p_play o, l_int relpos)
      {
      if (o->used_library != 0)
         {
         return (o->used_library)->rewind_file(o->used_library, relpos);
         }
      };

     /**[txh]**********************************************************************
     
      Function: set_pos

      Description: for a player: Sets position in the file
     
      Return:
      Example: pp->set_pos(pp, 1000);
     
     *****************************************************************************/


   //set pos
l_int set_pos(p_play o, l_int pos)
      {
      if (o->used_library != 0)
         {
         return (o->used_library)->set_pos(o->used_library, pos);
         }
      };

     /**[txh]**********************************************************************
     
      Function: get_pos

      Description: for a player: gets position
     
      Return: position in the file
      Example: position = pp->get_position(pp);
     
     *****************************************************************************/


   //get current pos
l_int get_pos(p_play o)
      {
      if (o->used_library != 0)
         {
         return (o->used_library)->get_pos(o->used_library);
         }
      };

     /**[txh]**********************************************************************
     
      Function: get_len_file

      Description: for a player: gets len of file
     
      Return: len of file
      Example: len_of_file = pp->get_len_file(pp);
     
     *****************************************************************************/


   //lenght of file
l_int get_len_file(p_play o)
      {
      if (o->used_library != 0)
         {
         return (o->used_library)->get_len_file(o->used_library);
         }
      };

     /**[txh]**********************************************************************
     
      Function: get_info

      Description: for a player: get info of file (string), you can free it with _free();
     
      Return: char * string;
      Example: l_text info = pp->get_info(pp);
               ...
               _free(info);
     
     *****************************************************************************/


   // get file format info
l_text     get_info(p_play o)
      {
      if (o->used_library != 0)
         {
         return _strdup((o->used_library)->info);
         }
      };

     /**[txh]**********************************************************************
     
      Function: get_time

      Description: for a player: gets current time in seconds
     
      Return: time in seconds
      Example: current_time = pp->get_time(pp);
     
     *****************************************************************************/


   // get current time in seconds
   
l_int     get_time(p_play o)
      {
      if (o->used_library != 0)
         {
         return (o->used_library)->get_time(o->used_library);
         }
      };

     /**[txh]**********************************************************************
     
      Function: is_stereo

      Description: for a player: is the file stereo, or mono?
     
      Return: 1...stereo, 0...mono
      Example: file_stereo = pp->is_stereo(pp);
     
     *****************************************************************************/


l_int     is_stereo(p_play o)
      {
      if (o->used_library != 0)
         {
         return (o->used_library)->is_stereo(o->used_library);
         }
      };


     /**[txh]**********************************************************************
     
      Function: get_additional_info

      Description: for a player: gets additional info (use _free() to free it)
     
      Return: char *, l_text
      Example: l_text add_info = pp->get_additional_info(pp);
     
     *****************************************************************************/
      

l_text   get_additional_info(p_play o)
      {
      if (o->used_library != 0)
         {
         return (o->used_library)->get_additional_info(o->used_library);
         }
      };




l_int seal_volume = 255;
l_int count_player = 0;
l_int sound_input_installed = true;

     /**[txh]**********************************************************************
     
      Function: get_sound_input_installed

      Description:
     
      Return:  >1 if sound input is installed
      Example:
     
     *****************************************************************************/


l_int get_sound_input_installed()
{
return sound_input_installed;
}

     /**[txh]**********************************************************************
     
      Function: inc_count_player

      Description: If your player starts a file you have to call it.
     
      Return:
      Example: inc_count_player();
     
     *****************************************************************************/


void inc_count_player()
{
 count_player++;
}

     /**[txh]**********************************************************************
     
      Function: dec_count_player

      Description: If your player stops playing a file you have to execute it.
     
      Return:  void
      Example: dec_count_player();
     
     *****************************************************************************/


void dec_count_player()
{
 count_player--;
}

     /**[txh]**********************************************************************
     
      Function: get_count_player

      Description:
     
      Return: how many files are playing
      Example:
     
     *****************************************************************************/


l_int get_count_player()
{
 return count_player;
}

     /**[txh]**********************************************************************
     
      Function: get_seal_volume

      Description:
     
      Return: current volume (0...255)
      Example: volume = get_seal_volume();
     
     *****************************************************************************/


l_int get_seal_volume()
{
return seal_volume;
}

     /**[txh]**********************************************************************
     
      Function: set_seal_volume

      Description: you can set the sound volume (0...255)
     
      Return: returns volume of seal
      Example: set_seal_volume(255);
     
     *****************************************************************************/


l_int set_seal_volume(l_int s_v)
{
if (s_v < 0) s_v = 0;
if (s_v > 255) s_v = 255;
seal_volume = s_v;
set_volume(seal_volume,seal_volume);
}


int  ff_walker(const char* path, const struct ffblk *ff)
     {

       l_text temp;
       if (stricmp("dlx", get_extension(path))) return 0;


       temp = _strdup(ff->ff_name);


       if (!strstr(temp, "sfa")) return 0;

//       seal_error(ERR_INFO,"%s",path);

       run_file(path);

       _free(temp);
       
       DEBUG_printf("Loaded SFA:");
       DEBUG_printf(path);
       DEBUG_printf("\n");
        
       
/*       if (ff->ff_attrib & 1)
         printf("R");
       if (ff->ff_attrib & 2)
         printf("H");
       if (ff->ff_attrib & 4)
         printf("S");
       if (ff->ff_attrib & 8)
         printf("V");
       if (ff->ff_attrib & 0x10)
         printf("D");
       if (ff->ff_attrib & 0x20)
         printf("A");
       printf("\n");`*/
     
       if (strcmp(ff->ff_name, "XXXXX") == 0)
         return 42;
       return 0;
}
     



void find_sfa(l_text path)
{

            __file_tree_walk(path, ff_walker);
            
/*           if (errno)
             seal_error(ERR_INFO, "Error: No SFA files found (%s)", path);*/




}


//lib_exportable;
#include "dlxsound.exp"




lib_begin ( void ) {




  if ( ap_process == AP_INIT ) {

      void *p;
      p_soundformat_item k;
      l_text path;
//      p_play pp;
//      p_stattext o = 0;
      t_rect r;

      AP_EXPORTLIB();
//      AP_SETNUMOFCALLS(1);


      //init sound format list!!

      sound_format_list = list_init(_malloc(sizeof(t_list)), &free_soundformat_item, 0);
      

      k = new_soundformat_item(                      _strdup("st"),
                                                     _strdup("(c) Florian Xaver GNU GPL - virtual sound file"),
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
                                                     &st_get_time,
                                                     &st_is_stereo,
                                                     &st_get_additional_info
                                                     );




     sound_format_list->insert(sound_format_list, k);




     

      p = show_info(0, MW_INFO, TXT_INITSOUNDCARD);
      
      DEBUG_printf("Sound init: init sound...\n");
      
      /* reserve_voice is very important !!!!! (mod player)*/
      reserve_voices (32, -1);     // call this before install_sound if needed (for the mod player !!!)
      
      DEBUG_printf("Sound init: voices reserved.\n");
      DEBUG_printf("Sound init: Installing sound...");

      if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "") == -1)
          {
          hide_info(p);          
          seal_error(ERR_INFO, TXT_CANNOTINITSOUNDCARD, allegro_error);
          DEBUG_printf(allegro_error);
          return;
          }

      DEBUG_printf("   sound installed!\n");
      DEBUG_printf("Sound input init:");

/*      if (install_sound_input(DIGI_AUTODETECT, MIDI_AUTODETECT) != 0)
         {
         hide_info(p);
         DEBUG_printf("  Error:");
         DEBUG_printf(allegro_error);
         seal_error(ERR_INFO, TXT_CANNOTINITSOUNDINPUT, allegro_error);
         sound_input_installed = false;
         }
      else*/
         DEBUG_printf("  installed");




      // setting volume from seal.ini
      seal_volume = (l_int *)getini_fromfile ( "seal.ini", "sound", "volume" );
      set_volume(seal_volume, seal_volume);

      DEBUG_printf("Sound init: volume set");

      path = getini_fromfile (INI_MAINFILE, "paths", "main");


      find_sfa(path);

      hide_info(p);
      _free(path);


  }

  if ( ap_process == AP_ALLOC ) {


  
  }



  if ( ap_process == AP_DONE ) {
  



  }


  if ( ap_process == AP_FREE ) {



  };

} lib_end;
