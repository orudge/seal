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

#define NEW_ALLEGRO
#define __MAIN_INCLUDED__

#include <allegro.h>
#include <allegro/internal/aintern.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <io.h>
#include <mss.h>
#include <zlib.h> // OCR - 05/04/2002




#define new_menu_item2(name,param,hk,msg,it,enable,img,next) new_menu_item_ex(name, param, hk, msg, enable, it, 0, NULL, img, NULL, 0, next)
#define new_sub_menu2(name,menu,enable,img,next) new_sub_menu_ex(name, enable, NULL, NULL, img, NULL, 0, menu, next)


////////////////////// H ////////////////////////////////////

#include <engine.h>

l_text TXT[] = { NULL };

l_text  SealApi = "SEAL";
l_text  SealApiCopyright = "Copyright (c) 1999-2000 Michal Stencl, Copyright (c) 2001-2002 SEAL Developers. All Rights Reserved.";
l_int   SealApiVersion = 200011; // 2.00.011
l_text  SealApiTxtVersion = "2.00.11";

l_bool safe_mode = 0;

/////////////////////////////////////////////////////////////

#ifndef BUFFER_FORMAT_TEXT
#define BUFFER_FORMAT_TEXT   1024
#endif

#ifndef TEXTOUT_BUFFER_SIZE
#define TEXTOUT_BUFFER_SIZE 1024
#endif

#ifndef FIFO_BUFFER_SIZE
#define FIFO_BUFFER_SIZE   512
#endif

FILE *seal_debug_file = NULL;

l_int  DEBUG_printf ( l_text text, ...)
{

   l_int nc = 0;
   va_list arg;

   if ( !seal_debug_file ) return 0;

   va_start(arg, text);

   nc = vfprintf(seal_debug_file, text, arg);

   va_end(arg);

   fflush(seal_debug_file);

   return nc;
};

/////////////////////////////////////////////////////////////

l_int  (*seal_error) ( l_int errtype, l_text str, ... );

/////////////////////////////////////////////////////////////

void RunAllRegistryEntries ( l_text rootkey, void(*callbak)(l_text) );

FILE *_fopen (l_text x, l_text y) { return fopen(x,y); };
void _fclose (FILE *f) { fclose(f); };
#define fopen(x,y) SealFileOpen(x,y)
//#define fclose(x) SealFileClose(x)
l_int   font_smooth = 1;
l_int   translucide = 0;

#include <object.h>

#include <safmem.h> // Safe Memory functions by Michal Stencl

//#include"text.c" // Text manipulation

#include<registry.h> // Registry functions by BadSector

#include<dataini.h> // DataIni function by Michal Stencl

//#include"object\filter.c" // Data flters
#include <filter.h>

//#include"object\rect.c" // Position and origin runtimes ...

//#include"object\object.c" // Object Run times ...

#include "alleg.c"  // Allegro and its expensions

//#include"grfx-f.c" // GRFX advanced grafics functions
#include "grfx-f.h"

extern PALETTE Gr_pal;

//#include"txts.c"
#include "txts.h"

//#include"colors.c"
#include "colors.h"

//#include"bmps.c"
#include "bmps.h"

//#include"skin.c"
#include "skin.h"

//#include"drivers.c"
#include "drivers.h"
#include "savescr.h"
#include "screen.h"

#define SFA_INCLUDE_STANDARD_DRIVERS
#include "sound.h"

//#include"list.c"   // Lists functions by Michal Stencl
#include "list.h"

#include<view.h> // View Run times ...

//#include"lnk.c"    // LNK system
#include "lnk.h"

//#include"files.c"  // Files manager functions by Julien Etelain
#include "files.h"

//#include"vfile.c"  // Virtual File System by Julien Etelain
#include "vfile.h"

//#include"ico.c"    // ICO and CUR files loader by Julien Etelain
#include "ico.h"

//#include"stimer.c" // Safe timer functions
#include "stimer.h"

//#include"controls\grfx.c"
#include "grfx.h"

#include <button.h>

//#include"controls\window.c"
#include "window.h"

#include<stattext.h>
#include<app.h>
#include<scroller.h>
#include<menus.h>
#include<dialogs.h>
#include<iodlg.h>
#include<editor.h>
#include<treeview.h>
#include<checkbox.h>

//#include"tips.c"

//#include"xdldata.c"

#include <inputbox.h>
#include <trackbar.h>
#include <seal/tabbook.h>
#include <compat.h>

static l_text app_about_appname;
static void redraw_about_dialog(p_view v);
static BITMAP *app_about_bmp;

void seal2_app_about(l_text appname, BITMAP *icon)
{
   p_appwin  winAbout;
   p_button  btnClose;
   p_view    vInfo;
   l_dword   msg = MSG_NOTHING;
   l_text    title;

   title = set_format_text(NULL, "About %s", appname);
   app_about_appname = appname;

   if (icon == NULL)
      app_about_bmp = load_image("/system/bmp/app.ico,32");
   else
      app_about_bmp = icon;

   winAbout = appwin_init(_malloc(sizeof(t_appwin)),
                          rect_assign(0, 0, 310, 175),
                          title, 
                          NULL, NULL, NULL);
   if (winAbout) VIEW(winAbout)->align |= TX_ALIGN_CENTER;
   WINDOW(winAbout)->flags -= WF_SYSMENU;
   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(winAbout));

   btnClose = button_init(_malloc(sizeof(t_button)),
                          rect_assign(118, 140, 200, 165),
                          TXT_OK, MSG_OK, BF_NORMAL);
   OBJECT(winAbout)->insert(OBJECT(winAbout), OBJECT(btnClose));

   vInfo = view_init(_malloc(sizeof(t_view)),
                     rect_assign(0, 0, 310, 140));
   VIEW(vInfo)->draw = &redraw_about_dialog;
   OBJECT(winAbout)->insert(OBJECT(winAbout), OBJECT(vInfo));

   msg = desktop->execute_view(desktop, VIEW(winAbout));
   dispose(OBJECT(winAbout));
}

static void redraw_about_dialog(p_view v)
{
   l_int x;
   t_point p;
   t_rect  r = v->size_limits(v); 
   BITMAP  *out = v->begin_paint(v, &p, r);

   if (out)
   {
      if (!use_skins)
         rectfill(out, p.x, p.y, p.x+310, p.y+175, color_3d_face);

      masked_blit(app_about_bmp, out, 0, 0, p.x + 15, p.y + 30, 32, 32);

      textout_printf(out, v->font, -1, 
                     p.x+65,  p.y+30, p.x+310,  p.y+115, 
                     TX_ALIGN_LEFT, color_3d_text, TX_NOCOLOR, 0, 
                     "SEAL %s\n%s\n\nCopyright \xA9 1999-2002 SEAL Developers.\nAll Rights Reserved.\n\nE-mail:      orudge@users.sourceforge.net\nWeb site:  http://sealsystem.sourceforge.net/", SealApiTxtVersion, app_about_appname);
   };
   v->end_of_paint(v, r);
}

/////////////////////////////////////////////////////////////
void RunAllRegistryEntries ( l_text rootkey, void(*callbak)(l_text) ) {
    l_text filedlx;
    p_registry_search inf = (p_registry_search) malloc(sizeof(t_registry_search));
    DEBUG_printf ( "\n  - Run all registry entries from '%s'\n", rootkey );
    if ( reg_find_first(rootkey, inf) ) do {

      if ( callbak ) {
        l_text s = (char *) malloc(2048);
        l_text s2 = key_in_path(inf->name, "title");
        if (key_exists(s2)) strcpy(s, get_key(s2)); else strcpy(s, get_key(inf->name));
        callbak ( s );
        _free(s2);
        _free(s);
      };

      filedlx = get_key(inf->name);
      DEBUG_printf ("    Run :: %s\n", filedlx );
      run_file(filedlx);
      free(filedlx);
    } while (reg_find_next(inf));
};
/////////////////////////////////////////////////////////////
void ListLoadedApp ( void ) {

  DEBUG_printf("Loaded XDL list :\n");
  DEBUG_printf("  XDL no[id] : [appname] ([filename]) by [manufacturer]\n");
  if ( listapp && listapp->last ) {

    p_item x = listapp->first(listapp);
    p_item f = x;

    if  ( f )
    do {
       p_app o = ((p_app)(x->rec));

       DEBUG_printf ("  XDL no %d %d : %s (%s) by %s\n",o->id ,o->appname,o->filename,o->manufacturer);

       x = x->next;

    } while ( x != f );
  };
};

/////////////////////////////////////////////////////////////

l_int  txt_seal_error ( l_int errtype, l_text str, ... )
{

  switch ( errtype ) {

    case ERR_NONREGULAR : {

      va_list argptr;
      va_start(argptr, str);
      printf("\nError :\n");
      vprintf(str, argptr);
      va_end(argptr);

      return 0;

    }; break;

    case ERR_INFO : {

      va_list argptr;
      va_start(argptr, str);
      printf("\nError :\n");
      vprintf(str, argptr);
      va_end(argptr);

      return 1;

    }; break;

  };

  return 0;
};
/////////////////////////////////////////////////////////////

#include<djc.h> // All Djgpp functions declaration

/////////////////////////////////////////////////////////////

#include"engine.exp" // Dlx export functions table

////////////////////////////////////////////////////////////////////////////////


l_big     event_timer = 0;

p_object  event_stop  = NULL;

t_event   event_main;

l_bool    go_process = true;


t_data  clipboard;

/* the main program object */
l_int  task_tick        = 20;

/* the main program object */
t_object   program;
/* list of objects that use OB_OF_STILLPROCESS option flag */
t_list     stillprocess_list;

/* drivers, the main object for drivers, such as keyboard, mouse, ... */
p_drivers  drivers = NULL;

/* destkop, the main object of visible objects */
p_view     desktop = NULL;

/* define if clock-cursor must be included */
l_int      clock_including = 0;

/* contains queue of events ( if you use put_event function ) */
static     t_event event_panding;

/* saves the last time of caling program_translate_event function */
static     l_big   event_oldtimer;

/* insert event to event queue */
void program_put_event ( t_object *o, t_event *event )
{
  event_panding = (*event);
};

////////////////////////////////////////////////////////////////////////////////
/*

   insert object (s) to stillprocess objects

   return 1, if all is OK
   return 0, on error

*/

int  program_put_into_stillprocess ( p_object o, p_object s )
{

  if ( stillprocess_list.insert(&stillprocess_list, s) != -1 )

    return 1;

  return 0;

};

////////////////////////////////////////////////////////////////////////////////
/*

  clear object (s) from stillprocess objects

  return 1, if everything is OK
  return 0, on error

*/

int  program_clear_from_stillprocess ( p_object o, p_object s )
{

  p_item p = stillprocess_list.find_rec(&stillprocess_list, s);

  if ( p ) {

      stillprocess_list.remove_item(&stillprocess_list, p);

      return 1;

  };

  return 0;

};

////////////////////////////////////////////////////////////////////////////////
/*
  main get_event function.

  - get last event from event's queue
*/

void program_get_event ( t_object *o, t_event *event )
{
  STOP_PROCESS();

  clear_event(event);

  /* save the last time of calling */

  event_oldtimer = time_get_mili();


  if ( event_panding.type != EV_NOTHING ) { /* something in queue */

    (*event) = event_panding; /* new event will contain last from queue */

    clear_event(&event_panding); /* clear queue */

  } else { /* nothing in queue */

    if ( drivers ) { /* drivers exists */


        /* call each object in drivers object */
        ((p_object)drivers)->translate_event((p_object)drivers, event);

    };
  };

  START_PROCESS();


  /* call each stillprocess object */
  program_call_each_stillprocess(&stillprocess_list);
};
////////////////////////////////////////////////////////////////////////////////
l_int scr_shots = 1;

void fast_scr_shot ( void ) {
  BITMAP *scr = create_bitmap(screen_width, screen_height);
  if ( scr ) {
    l_text file = set_format_text(NULL,"shot%d.bmp",scr_shots);
    if ( file ) {
      blit(screen,scr,0,0,0,0,screen_width,screen_height);
      save_image(file,scr,NULL);
      _free(file);
      scr_shots++;
    };
    destroy_bitmap(scr);
  };
};

l_bool fast_system = true;
////////////////////////////////////////////////////////////////////////////////
p_list get_loaded_xdl ( void ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_listbox_item, 0);

  if ( p ) {


    if ( listapp && listapp->last ) {

    p_item x = listapp->first(listapp);
    p_item f = x;

    if  ( f )
    do {
       p_app o = ((p_app)(x->rec));

       p->insert(p, new_listbox_item(set_format_text(NULL,"%s (%s)",o->appname,o->filename), NULL, 0, NULL));

       x = x->next;

    } while ( x != f );
  };

  };

  return p;
};
#define MSG_END_OF_TASK 9999 // ;) no possible bugs : 9/9/99 is passed !!!

p_listbox XDL_LISTBOX = NULL;
////////////////////////////////////////////////////////////////////////////////
void  xdl_man_trans_ev ( p_object o, p_event event ) {

  if ( event->type & EV_MESSAGE ) {
  l_dword msg = event->message;

  if ( msg == MSG_END_OF_TASK ) {

    l_int nb = XDL_LISTBOX->current;

    p_item it;

    p_app xo;

    hdlx_t id;

    it = listapp->at_item(listapp,nb);

    xo = ((p_app)(it->rec));

    memcpy(&id,&xo->id,sizeof(hdlx_t));

    DLXUnload(id);

    clear_event(event);

    set_event(event, EV_MESSAGE, MSG_CLOSE, o);

    o->put_event(o, event);

    clear_event(event);

  };

  };
};
////////////////////////////////////////////////////////////////////////////////
void  fast_trans_ev ( p_object o, p_event event ) {

  if ( event->type == EV_KEYBOARD && OBJECT(keyb)->state & KB_SF_KEYDOWN && KEYCTRL(KB_F8) ) {

    fast_scr_shot ( );

    clear_event(event);

  };

  if ( event->type == EV_KEYBOARD && OBJECT(keyb)->state & KB_SF_KEYDOWN && KEYCTRL(KEY_F12) ) {

    p_appwin win;
    p_listbox lst;
    p_button b;
    p_stattext txt;
    t_rect r = rect_assign(0,0,320,270);

    clear_event(event);

    win = appwin_init( _malloc(sizeof(t_appwin)),
                                 r,
                                 "XDL Task Manager",
                                 WF_MINIMIZE,
                                 NULL,
                                 &xdl_man_trans_ev);
    if ( !win ) return;

    VIEW(win)->align |= TX_ALIGN_CENTER;

    lst = listbox_init( _malloc(sizeof(t_listbox)),rect_assign(5,20,r.b.x-20,r.b.y-50),
                        get_loaded_xdl(),1,LF_SELFLIST);
    XDL_LISTBOX = lst;
    OBJECT(win)->insert(OBJECT(win),OBJECT(lst));

    txt = stattext_init( _malloc(sizeof(t_stattext)),rect_assign(5,r.b.y-45,r.b.x-5,r.b.y-30),
                         TX_ALIGN_CENTER, "Warning: Some programs are needed by others!");
    OBJECT(win)->insert(OBJECT(win), OBJECT(txt));

    b = button_init(_malloc(sizeof(t_button)),
                            rect_assign(5,r.b.y-25,(r.b.x/2)-5,r.b.y-5),
                            "End Task",
                            MSG_END_OF_TASK,
                            BF_DEFAULT);
    OBJECT(win)->insert(OBJECT(win), OBJECT(b));

    b = button_init(_malloc(sizeof(t_button)),
                            rect_assign((r.b.x/2)+5,r.b.y-25,r.b.x-5,r.b.y-5),
                            "(x) Close",
                            MSG_CLOSE,
                            BF_DEFAULT);
    OBJECT(win)->insert(OBJECT(win), OBJECT(b));

    OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(win));

  };

};
////////////////////////////////////////////////////////////////////////////////
/* main translate event function */
void program_translate_event ( t_object *o, t_event *event )
{
  /* save the last time of calling */
  event_timer = time_get_mili();

  /* event type is message */
  if ( event->type == EV_MESSAGE ) {

    /* event message is MSG_QUIT */
    if ( event->message == MSG_QUIT ) {

      clear_event(event);
      /* set end_state of program to MSG_QUIT

         this is used in function obj_execute
      */
      o->end_state = MSG_QUIT;

    };

    if ( event->message == MSG_FST_SCR_SHOT ) {
      clear_event(event);
      fast_scr_shot ( );
    };

  };

  /* call each object in program hierarchy */
  obj_translate_event(o, event);

  if ( event->type == EV_KEYBOARD && OBJECT(keyb)->state & KB_SF_KEYDOWN
       && KEYPRESSED( KB_PRTSCR ) ) {


    fast_scr_shot ( );

    clear_event(event);

  };
  /* if keyboard was pressed and the keycode of the key is ALT+X */
  if ( event->type == EV_KEYBOARD && OBJECT(keyb)->state & KB_SF_KEYDOWN
       && KEYPRESSED(TO_ALT(KB_TAB)) ) {

    clear_event(event);

    /* set event by these arguments */
    set_event(event, EV_MESSAGE, MSG_QUIT, o);

    /* put event to a queue */
    o->put_event(o, event);

    clear_event(event);

  };

  /* if keyboard was pressed and the keycode of the key is ALT+D */
  if ( event->type == EV_KEYBOARD && OBJECT(keyb)->state & KB_SF_KEYDOWN
       && KEYPRESSED(TO_ALT(KB_D)) ) {

        p_object v = NULL;

        /* find the selected object in the desktop */
        if ( OBJECT(desktop)->prefer )

           /* search for the next visible, selectable, enable object in the desktop */
           v = OBJECT(desktop)->prefer->find_match_view(OBJECT(desktop)->prefer, OB_SF_VISIBLE, OB_OF_SELECTABLE+OB_OF_ENABLE, false);

  	     clear_event(event);

        /* if found some, select it */
        if ( v ) v->select(v);

  };


};

////////////////////////////////////////////////////////////////////////////////
/*
  done program

  return true, if succesfull, otherwise false
*/
l_bool  program_done ( p_object o ) {
  DEBUG_printf("Done program...\n");

  if ( !obj_done(o) ) return false;

  DEBUG_printf("Done stillprocess...\n");

  stillprocess_list.done(&stillprocess_list);

  drv_done(); /* done drives */

  DEBUG_printf("Done filter...\n");

  filter_remove_all(); /* remove all filters from the array */

  registry_done();

  DEBUG_printf("Done program : FINISHED\n");

  return true;
};
////////////////////////////////////////////////////////////////////////////////
void  desktop_draw ( p_view o ) {
  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {
//	l_text capt = set_format_text(NULL,"%s %s - Testing purposes only", SealApi,SealApiTxtVersion);

    o->background(o, out, rect_move(r, p.x, p.y));

/*    draw_double_text(out, o->font, capt, -1,
                        r.a.x+p.x, r.a.y+p.y, r.b.x+p.x-120, r.b.y+p.y-50,
                        TX_ALIGN_RIGHT|TX_ALIGN_BOTTOM,
                        color_desktop_text, color_flat_face, 0);

    _free(capt);*/
  };

  o->end_of_paint(o, r);
};
////////////////////////////////////////////////////////////////////////////////
t_object *program_init ( p_object o ) {
  char *OS = "????";

  if ( !o ) return NULL;

  seal_error = &txt_seal_error;

  DEBUG_printf(" - Initializing %s %s\n",SealApi,SealApiTxtVersion);

  DEBUG_printf("\n");

  obj_init(o);

  DEBUG_printf("\n - Start registry.\n");

  registry_init();

  DEBUG_printf("\n - Install Allegro.\n");

  install_allegro( SYSTEM_AUTODETECT, NULL, NULL );

  set_uformat(U_ASCII);

  check_cpu();

  DEBUG_printf("  System informations :\n");
  DEBUG_printf("    CPU\n");
  DEBUG_printf("      vendor : %s\n",&cpu_vendor);
  DEBUG_printf("      family : %d Model : %d\n",cpu_family,cpu_model);
  DEBUG_printf("      fpu : %d mmx : %d 3dnow :%d\n",cpu_fpu,cpu_mmx,cpu_3dnow);
  OS[0] = os_type>>24;
  OS[1] = os_type>>16;
  OS[2] = os_type>>8;
  OS[3] = os_type;
  DEBUG_printf("    OS\n");
  DEBUG_printf("      name : %s\n",OS);
  DEBUG_printf("      version : %d revision : %d\n",os_version,os_revision);


  DEBUG_printf("\n - Initialise drivers.\n");

  drv_init();

  if ( get_depth(screen) < 16 )
    translucide = 0;
  else
    translucide = get_key_integer("current/seal/optical/translucide");

  view_ini();

  clear_event(&event_panding);

  list_init(&stillprocess_list, NULL, 0);

  o->get_event = &program_get_event;
  o->put_event = &program_put_event;
  o->done = &program_done;
  o->translate_event = &program_translate_event;
  o->put_into_stillprocess = &program_put_into_stillprocess;
  o->clear_from_stillprocess = &program_clear_from_stillprocess;

  o->set_state(o, OB_SF_FOCUSED, true);
  o->set_options(o, OB_OF_TOPSELECT+OB_OF_NOTACTIVATE, true);

  if ( !desktop ) {/* install desktop */
    desktop = view_init(_malloc(sizeof(t_view)), rect_assign(0, 0, screen_width-1, screen_height-1));
    desktop->draw = &desktop_draw;
  };
  standard_menuitem_font = get_font_in_size("Arial", 8, 8);

  appsys_init();

  iodlg_init ();

  if ( fast_system ) {

    p_object fast = obj_init(_malloc(sizeof(t_object)));

    fast->translate_event = &fast_trans_ev;

    OBJECT(drivers)->insert(OBJECT(drivers), fast);
  };

  if ( screen_double_buffer )
    draw_to_2nd_buffer=1;

  if ( desktop ) {
    l_text fname = get_key("current/desktop/wallpaper");
    l_int style = get_key_integer("current/desktop/placement");

    desktop->brush.color = color_desktop_face;
    if ( fname ) {
      desktop->brush.background = load_image(GetFile(fname));
      if ( desktop->brush.background ) {
        desktop->brush.state     |= BRUSH_SELFIMG;
        if ( style == 1 )
          desktop->brush.state     |= BRUSH_STRETCH;
        else
        if ( style == 2 )
          desktop->brush.state     |= BRUSH_CENTER;
      };
    _free(fname);
    };

    o->insert(o, OBJECT(desktop));
  };

  tips_system_init(); // By Pablo Borobia !

  OBJECT(desktop)->insert(OBJECT(desktop), screensaver_init(_malloc(sizeof(t_object))) );

  RunAllRegistryEntries("system/startup/load",NULL);

  if ( screen_double_buffer && draw_to_2nd_buffer ) {
    draw_to_2nd_buffer = 0;
    set_clip(screen,0,0,screen_width,screen_height);
    if ( translucide > 0 ) {
      BITMAP *buffer = create_bitmap(screen_width,screen_height);
      l_int i = 0;
      blit(screen, buffer, 0, 0, 0, 0, screen_width, screen_height);
      for (i=0; i<160; i+=10) {
        set_trans_blender(0, 0, 0, i);
        draw_trans_sprite(buffer, screen_double_buffer, 0, 0);
        blit(buffer, screen, 0, 0, 0, 0, screen_width, screen_height);
      };
      destroy_bitmap(buffer);
      solid_mode();
    };
    blit(screen_double_buffer, screen, 0, 0, 0, 0, screen_width, screen_height);
  };

  DEBUG_printf(" - Initialisation finished.\n");

  mouse->show(mouse);

  return o;
};

////////////////////////////////////////////////////////////////////////////////
/*
  call each object's func_callback, that is placed in stillprocess list.
*/
void program_call_each_stillprocess ( p_list o ) {
  if ( o ) {

    p_item v = o->first(o); /* find first object */
    p_item f = v;
    if ( v )

      do {
        if ( v->rec &&
             /* difference between last calling and current is GE to process_tick of object */
             time_diff_mili(OBJECT(v->rec)->process_time) >= OBJECT(v->rec)->process_tick ) {

          /* set current time */
          OBJECT(v->rec)->process_time = time_get_mili();
          /* call function of the object */
          OBJECT(v->rec)->func_callback(OBJECT(v->rec));

        };

        v = v->next;

      } while ( v != f );

  };

};
////////////////////////////////////////////////////////////////////////////////
static void  aclock2 ( void ) {
  if ( !mouse->is_block(mouse) )
    if ( _time_diff_mili(event_oldtimer) >= 500 )
      mouse_set_sys_cursor(CUR_CLOCK);
    else if ( mouse->get_cursor(mouse) == system_cursors[CUR_CLOCK] )
      mouse_set_sys_cursor(CUR_ARROW);
};
////////////////////////////////////////////////////////////////////////////////
int program_int ( void ) {

  /* if not pause in multitasking */
  if ( go_process ) {

    /* difference is greater then task_tick */
    if ( _time_diff_mili(event_timer) >= task_tick ) {

      /* get current time */
      event_timer = time_get_mili();

      /* get events */
      program.get_event(&program, &event_main);

      program.translate_event(&program, &event_main);

    };

  };

  return 1;
};

////////////////////////////////////////////////////////////////////////////////

main ( int argc, char **argv ) {
  printf("%s %s Starting ...\n\n\n",SealApi,SealApiTxtVersion);

  if ( argc > 1 && argv[1] && !stricmp(argv[1], "-safe") ) // If arg '-safe' is set

      safe_mode = 1; // Set Safe Mode

  seal_debug_file = _fopen("seal.dbg", "wt"); // Open debug file

  ini_vfile ( ) ; // Start Virtual File System

  program_init(&program); // Init program

  program.execute(&program); // Execute program

  program.done(&program); // Close program

  fclose(seal_debug_file);  // Close Debug File

  return 0; // Errorlevel 0 : no error
};
////////////////////////////////////////////////////////////////////////////////
