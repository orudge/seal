/****************************************************************/
/*                                                              */
/*                          drivers.c                           */
/*                                                              */
/*                Seal - free desktop environment               */
/*                                                              */
/*                    Copyright (c) 1999,2000                   */
/*                         Michal Stencl                        */
/*                     All Rights Reserved                      */
/*                                                              */
/* mail : stenclpmd@ba.telecom.sk                               */
/* web  : http://www.home.sk/public/seal/                       */
/*                                                              */
/* This file is part of Seal.                                   */
/*                                                              */
/* Seal is free software; you can redistribute it and/or        */
/* modify it under the terms of the GNU General Public License  */
/* as published by the Free Software Foundation; either version */
/* 2, or (at your option) any later version.                    */
/*                                                              */
/* Seal is distributed in the hope that it will be useful, but  */
/* WITHOUT ANY WARRANTY; without even the implied warranty of   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See    */
/* the GNU General Public License for more details.             */
/*                                                              */
/* You should have received a copy of the GNU General Public    */
/* License along with Seal; see the file COPYING.  If not,      */
/* write to the Free Software Foundation, 675 Mass Ave,         */
/* Cambridge, MA 02139, USA.                                    */
/*                                                              */
/****************************************************************/

#include"brffunc.c"
#include"drivers.h"
#include"colors.h"
#include"fonts.c"
#include"sig_err.c"


/* names of system icons */

l_text  icon_id[256] = {"application", "browser", "controlpanel", "run", "network",
                        "shutdown", "editor", "folder", "openfolder", "fonts",
                         "mycomputer", "internetdialer", "internet", "internetsettings",
                         "help", "mailer", "printers", "settings", "systemfolder",
                         "pcsystem", "template", "trashempty", "trashfull", "utility",
                         "drive3_5", "drive5_25", "drivecd", "drivehd", "favorites",
                         "documents", "seallogo", "audio", "desktop", NULL};

/* icon library, get icon from this library by icon_sysget(id, size), ...see above */

l_iconlibrary *icon_system = NULL;


/* if non-zero the safe mode is set */
l_int   safe_mode = 0;


/* array of saved timers */
p_timer_def safe_timers = NULL;

/* font declarations */

/* defines if the font will be smooth */
l_int   font_smooth = 0;

/* defines if the font will 1,25* greater than normal font */
l_long  font_times_1_25 = 0;

/* pointer to regular system font */
l_font *font_system = NULL;
/* pointer to bold system font */
l_font *font_system_bd = NULL;
/* pointer to italic system font */
l_font *font_system_i = NULL;
/* pointer to bold+italic system font */
l_font *font_system_bi = NULL;


/* this function is called before Seal starting. You can initialize it to your own
   shot function.
*/
void (*screen_shot) ( void ) = NULL;




/* safe timer */
void  safe_timer ( void (*stop)(), void (*reload)() )
{

   p_timer_def x = safe_timers;

   /* search for timer */
   while ( x && x->stop != stop ) x = x->next;

   if ( !x ) { /* not found, so insert */

       p_timer_def x = (p_timer_def)_malloc(sizeof(t_timer_def));

       if ( x ) /* enough memory */

          clear_type(x, sizeof(t_timer_def)); /* all to ZERO */

          x->stop = stop;
          x->reload = reload;
          x->active = 1;
          x->next = safe_timers;

          safe_timers = x;
   };

};


/* erase previous saved timer */
void  erase_safe_timer ( void (*stop)() )
{
   p_timer_def x = safe_timers;
   p_timer_def last = NULL;

   /* search for timer */
   while ( x ) {

      p_timer_def s = x->next;

      if ( !last ) { /* first item */

         safe_timers = x->next;

         _free(x);

         return;

      } else { /*  */

          last->next = x->next;

          _free(x);

          return;

      };

      last = x;

      x = s;

   };

};


void safe_timer_halt ( void )
{
   p_timer_def x = safe_timers;

   /* for all timers */
   while ( x ) {

      /* it's able to stop it */
      if ( x->active ) {

            x->stop(); /* stop timer */
            x->active = 0; /* set to passive timer */

      };
      /* go to the next one */
      x = x->next;
   };
};


void safe_timer_reload ( void )
{
   p_timer_def x = safe_timers;

   /* for all timers */
   while ( x ) {

      /* it's able to reload it */
      if ( !x->active ) {

            x->reload(); /* stop timer */
            x->active = 1; /* set to passive timer */

      };
      /* go to the next one */
      x = x->next;
   };
};


void safe_timer_free_all ( void )
{
   p_timer_def x = safe_timers;

   /* for all timers */
   while ( x ) {

      p_timer_def n = x->next;

      /* free memory */
      _free(x);

      /* go to the next one */
      x = n;
   };

   safe_timers = NULL;
};


/* initialize icons. function is called in function drivers_init */

void icons_init ( void )
{

    char *icon_sys = getini_fromfile (INI_MAINFILE, INI_ICONS, "system_icons");

    DEBUG_printf("\n...initializing icons from '%s'\n", icon_sys); /* error message, if some occures */

    /* icon loading... */

    icon_system = load_icon_library(icon_sys, NOCOLOR_RED, NOCOLOR_GREEN, NOCOLOR_BLUE);

    if ( icon_system ) /* debug info */

        DEBUG_printf(" - icons was succesfull loaded\n"); /* error message, if some occures */

    else

        DEBUG_printf(" - ERROR :: icons wasn't succesfull loaded\n"); /* error message, if some occures */

    _free(icon_sys); /* free text of path */
};


/* destroy icons. function is called in function drivers_done */

void icons_done ( void )
{

  unload_icon_library(icon_system);
  icon_system = NULL;

};


/* return (id) of the system font from the name (n) */
l_int  get_icon_id ( l_text n )
{

  l_int i = 0;

  for ( i; i < 256 && icon_id[i]; i++ ) {

     if ( !stricmp(icon_id[i], n) ) return i;

  };

  return -1;

};


/* initialize fonts. function is called in function drivers_init */

void fonts_init ( void )
{
  font_times_1_25  = getininum_fromfile(INI_MAINFILE, INI_FONTS, "size 125%");
  font_smooth      = getininum_fromfile(INI_MAINFILE, INI_FONTS, "antialiazing");

  DEBUG_printf("\n...initializing fonts in "); /* error message, if some occures */
  DEBUG_test(font_times_1_25, DEBUG_printf("125%% size \n"),
                              DEBUG_printf("100%% size \n"));

  if ( font_times_1_25 ) {

    char *f_reg = getini_fromfile (INI_MAINFILE, INI_FONTS, "regular 125%");
    char *f_bd  = getini_fromfile (INI_MAINFILE, INI_FONTS, "bold 125%");
    char *f_i   = getini_fromfile (INI_MAINFILE, INI_FONTS, "italic 125%");
    char *f_bi  = getini_fromfile (INI_MAINFILE, INI_FONTS, "bold+italic 125%");

    font_system    = GrLoadFont(f_reg, NULL, FONT_DEFAULT_WIDTH, FONT_DEFAULT_HEIGHT, -1, -1);
    font_system_bd = GrLoadFont(f_bd, NULL, FONT_DEFAULT_WIDTH, FONT_DEFAULT_HEIGHT, -1, -1);
    font_system_i  = GrLoadFont(f_i, NULL, FONT_DEFAULT_WIDTH, FONT_DEFAULT_HEIGHT, -1, -1);
    font_system_bi = GrLoadFont(f_bi, NULL, FONT_DEFAULT_WIDTH, FONT_DEFAULT_HEIGHT, -1, -1);

    if ( !font_system ) { /* regular system font */

      DEBUG_printf(" - ERROR :: system regular font not loaded in 125%% size from '%s' and try to load in 100% size from ", f_reg); /* error message, if some occures */

      _free(f_reg);

      f_reg   = getini_fromfile (INI_MAINFILE, INI_FONTS, "regular");
      font_system   = GrLoadFont(f_reg, NULL, FONT_DEFAULT_WIDTH, FONT_DEFAULT_HEIGHT, -1, -1);

      DEBUG_printf("%s \n", f_reg);

    };


    if ( !font_system_bd ) { /* bold system font */

      DEBUG_printf(" - ERROR :: system bold font not loaded in 125%% size from '%s' and try to load in 100% size from ", f_bd); /* error message, if some occures */

      _free(f_bd);

      f_bd   = getini_fromfile (INI_MAINFILE, INI_FONTS, "bold");
      font_system_bd   = GrLoadFont(f_bd, NULL, FONT_DEFAULT_WIDTH, FONT_DEFAULT_HEIGHT, -1, -1);

      DEBUG_printf("%s \n", f_bd);

    };


    if ( !font_system_i ) { /* italic system font */

      DEBUG_printf(" - ERROR :: system italic font not loaded in 125%% size from '%s' and try to load in 100% size from ", f_i); /* error message, if some occures */

      _free(f_i);

      f_i   = getini_fromfile (INI_MAINFILE, INI_FONTS, "italic");
      font_system_i   = GrLoadFont(f_i, NULL, FONT_DEFAULT_WIDTH, FONT_DEFAULT_HEIGHT, -1, -1);

      DEBUG_printf("%s \n", f_i);

    };

    if ( !font_system_bi ) {

      DEBUG_printf(" - ERROR :: system bold italic font not loaded in 125%% size from '%s' and try to load in 100% size from ", f_bi); /* error message, if some occures */

      _free(f_bi);

      f_bi   = getini_fromfile (INI_MAINFILE, INI_FONTS, "bold+italic");
      font_system_bi   = GrLoadFont(f_bi, NULL, FONT_DEFAULT_WIDTH, FONT_DEFAULT_HEIGHT, -1, -1);

      DEBUG_printf("%s \n", f_bi);

    };

    _free(f_reg);
    _free(f_bd);
    _free(f_i);
    _free(f_bi);

  } else {

    char *f_reg   = getini_fromfile (INI_MAINFILE, INI_FONTS, "regular");
    char *f_bd    = getini_fromfile (INI_MAINFILE, INI_FONTS, "bold");
    char *f_i     = getini_fromfile (INI_MAINFILE, INI_FONTS, "italic");
    char *f_bi    = getini_fromfile (INI_MAINFILE, INI_FONTS, "bold+italic");

    font_system    = GrLoadFont(f_reg, NULL, FONT_DEFAULT_WIDTH, FONT_DEFAULT_HEIGHT, -1, -1);
    font_system_bd = GrLoadFont(f_bd, NULL, FONT_DEFAULT_WIDTH, FONT_DEFAULT_HEIGHT, -1, -1);
    font_system_i  = GrLoadFont(f_i, NULL, FONT_DEFAULT_WIDTH, FONT_DEFAULT_HEIGHT, -1, -1);
    font_system_bi = GrLoadFont(f_bi, NULL, FONT_DEFAULT_WIDTH, FONT_DEFAULT_HEIGHT, -1, -1);

    _free(f_reg);
    _free(f_bd);
    _free(f_i);
    _free(f_bi);

  };

  /* debuger infos */

  DEBUG_test(font_system, DEBUG_printf(" - system regular font was succesfull loaded\n"),
                          DEBUG_printf(" - ERROR :: system regular font wasn't succesfull loaded\n"));
  DEBUG_test(font_system_bd, DEBUG_printf(" - system bold font was succesfull loaded\n"),
                          DEBUG_printf(" - ERROR :: system bold font wasn't succesfull loaded\n"));
  DEBUG_test(font_system_i, DEBUG_printf(" - system italic font was succesfull loaded\n"),
                          DEBUG_printf(" - ERROR :: system italic font wasn't succesfull loaded\n"));
  DEBUG_test(font_system_bi, DEBUG_printf(" - system bold italic font was succesfull loaded\n"),
                          DEBUG_printf(" - ERROR :: system bold italic font wasn't succesfull loaded\n"));


  load_supported_fonts(INI_TTFFONTS, false); /* load system true type fonts, quick */

  load_supported_fonts(INI_TTFFONTSMEM, true); /* load system true type fonts to memory */

};


/* destroy icons. function is called in function drivers_done  */

void fonts_done ( void )
{

  GrUnloadFont(font_system);
  GrUnloadFont(font_system_bd);
  GrUnloadFont(font_system_i);
  GrUnloadFont(font_system_bi);

  font_system = NULL;
  font_system_bd = NULL;
  font_system_i = NULL;
  font_system_bi = NULL;

  unload_system_fonts();

  safe_timer_free_all();

};


/* done all drivers. this function is called in function program_done in "program.c" */

l_bool  drivers_done ( p_object o )
{

  if ( !obj_done((p_object)o) ) return false;

  fonts_done();

  icons_done();

  screen_done();

  colors_done();

  signal_done();

  allegro_exit();

  return true;

};


/* init all drivers. this function is called in function program_init in "program.c".

   return pointer to drivers, where objects such as mouse, keyboard, ... are stored
*/
t_drivers *drivers_init ( t_drivers *o )
{
  if ( !o ) return NULL;

  memset(o, 0, sizeof(t_drivers));

  obj_init(&o->obclass);

  OBJECT(o)->done = &drivers_done;

  set_config_file("startup.cfg");

  DEBUG_printf(" - drivers_init : allegro_init() function \n");
  /* init allegro */
  allegro_init();

  DEBUG_printf(" - drivers_init : install_timer() function \n");

  /* install allegro timer */
  install_timer();

  DEBUG_printf(" - drivers_init : install timer for aclock function to 20 milsc \n");

  /* put aclock to timer queue. each 20 milisecs call this function. */
  install_int(&aclock, 20);

  DEBUG_printf(" - drivers_init : brfont_install_functions() function \n");

  brfont_install_functions(); /* install true type loading functions */

  DEBUG_printf(" - drivers_init : read set file '%s' \n", SET_DRIVERS);

  /* read set file for drivers "driver.set" */
  read_set_file(SET_DRIVERS);

  DEBUG_printf(" - drivers_init : screen_init() \n");

  /* init screen */
  screen_init();

  DEBUG_printf(" - drivers_init : colors_init() \n");

  /* init colors */
  colors_init();

  DEBUG_printf(" - drivers_init : keyboard inserting into drivers \n");

  /* make new keyboard and insert it to drivers object. ...see keyboard.h */
  o->obclass.insert((p_object)o, (p_object)key_init(_malloc(sizeof(t_keyboard))));

  DEBUG_printf(" - drivers_init : mouse inserting into drivers \n");

  /* make new mouse and insert it to drivers object. ...see mouse.h */
  o->obclass.insert((p_object)o, (p_object)mouse_init(_malloc(sizeof(t_mouse))));

  /* not call this object from program.translate_event ! */
  OBJECT(o)->set_options(OBJECT(o), OB_OF_NOTACTIVATE, true);

  DEBUG_printf(" - drivers_init : screen_shot() \n");

  /* draw something to screen before, we initialize all drivers and libraries */
  if ( screen_shot )

     screen_shot();

  DEBUG_printf(" - drivers_init : icons_init() \n");

  /* icons initialization */
  icons_init();

  DEBUG_printf(" - drivers_init : fonts_init() \n");

  /* init fonts */
  fonts_init();

  DEBUG_printf(" - drivers_init : signal_init() \n");

  /* signal errors init */
  signal_init();


  DEBUG_printf(" - drivers_init : return(o) \n\n");

  return o;

};

