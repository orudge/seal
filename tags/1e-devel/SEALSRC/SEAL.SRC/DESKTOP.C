/****************************************************************/
/*                                                              */
/*                           desktop.c                          */
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



#include"allegro.h"
#include"seal.h"
#include"menus.h"
#include"button.h"
#include"app.h"
#include"desktop.h"
#include<unistd.h>
#include<dir.h>


/* indicates if it can be drawed under desktop line. 1 = it is, 0 = it isn't  */
l_int            draw_under_line = 1;

/* list of minimalized objects */
static p_list           desktopline_minimalized = NULL;

/* items of program menu */
static p_menuitem       program_items = NULL;

/* items of settigns menu */
static p_menuitem       settings_items = NULL;

/* items of help menu */
static p_menuitem       help_items = NULL;

/* desktop line */
static p_desktopline  desktopline = NULL;


/* init function for desktop line */
p_desktopline  (*desktopline_init) ( p_desktopline o ) = &_desktopline_init;



/* menu icons */

static DATAFILE *datfile = NULL;


#define ICO_GET(n) (BITMAP*)GET_DATA(datfile, n)

#define ICO_xPROGRAMS 0
#define ICO_xHELP     1
#define ICO_xSHUTDOWN 2
#define ICO_xSETTINGS 3
#define ICO_xMINIZED  4


/* desktop menu about texts */

#define TXT_ABOUTPROGRAMS      INI_TEXT("")
#define TXT_ABOUTSETTINGS      INI_TEXT("")
#define TXT_ABOUTCLOSEWIN      INI_TEXT("")
#define TXT_ABOUTQUIT          INI_TEXT("")
#define TXT_ABOUTRUNAPP        INI_TEXT("")


#define  DIR_PROGRAMS  "./PROGRAMS"
#define  DIR_SETTINGS  "./SETTINGS"
#define  DIR_HELP      "./HELP"

#define  DIR_IGNORE  '$'


#define  MSG_REFRESHMINIMIZED  100109
#define  MSG_RUNAPP            100110
#define  MSG_xAPPLICATION      100111
#define  MSG_xSETTINGS         100112
#define  MSG_xHELP             100113
#define  MSG_xMINIZED          100114

#define  MSG_xRESTART          100120
#define  MSG_xREFRESH          100121

#define  ID_SIZE               35  /* desktop icon size */


/* t_minmenuitem structure */

typedef struct t_minmenuitem *p_minmenuitem;

typedef struct t_minmenuitem {

  struct t_menuitem  obclass;

  t_appinfo          appinfo;

} t_minmenuitem;



/* t_filemenuitem structure */

typedef struct t_filemenuitem *p_filemenuitem;

typedef struct t_filemenuitem {

  struct t_menuitem  obclass;

  l_text             filename;

} t_filemenuitem;


#define new_sub_filemenu(name,f,menu,next) \
                                new_sub_filemenu_ex(name, true, NULL, NULL, NULL, NULL, 0, f, menu, next)
#define new_filemenu_item(name,param,hk,msg,it,f,next) \
                                new_filemenu_item_ex(name, param, hk, msg, true, it, 0, NULL, NULL, NULL, 0, f, next)
#define new_filemenu_check_item(name,param,is_check,hk,msg,it,f,next) \
                                new_filemenu_item_ex(name, param, hk, msg, true, it, MIF_CHECK+(is_check*MIF_CHECKOK), NULL, NULL, NULL, 0, f, next)

static p_filemenuitem  new_filemenu_item_ex ( l_text name, l_text param, l_int hotkey, l_dword message,
                               l_bool enable, l_text info_text, l_int flags,
                               l_font *font, BITMAP *icon, l_font *font_symbol, l_byte chr,
                               l_text filename, p_menuitem next )
{

  p_filemenuitem fi = (p_filemenuitem)_malloc(sizeof(t_filemenuitem));

  p_menuitem f = new_menu_item_ex(name, param, hotkey, message, enable, info_text, flags, font, icon, font_symbol, chr, next);

  if ( f && fi ) {

     memcpy(fi, f, sizeof(t_menuitem));

     fi->filename = _strdup(filename);

  };

  _free(f);

  return fi;

};


#define new_minmenu_item(msg,f,next) \
                                new_minmenu_item_ex(f.title, NULL, 0, msg, true, NULL, 0, NULL, NULL, NULL, 0, f, next)

static p_minmenuitem  new_minmenu_item_ex ( l_text name, l_text param, l_int hotkey, l_dword message,
                               l_bool enable, l_text info_text, l_int flags,
                               l_font *font, BITMAP *icon, l_font *font_symbol, l_byte chr,
                               t_appinfo info, p_menuitem next )
{

  p_minmenuitem fi = (p_minmenuitem)_malloc(sizeof(t_minmenuitem));

  p_menuitem f = new_menu_item_ex(name, param, hotkey, message, enable, info_text, flags, font, icon, font_symbol, chr, next);

  if ( f && fi ) {

     memcpy(fi, f, sizeof(t_menuitem));

     fi->appinfo = info;

  };

  _free(f);

  return fi;

};



static p_filemenuitem  new_sub_filemenu_ex ( l_text name, l_bool enable, l_text info_text,
                              l_font *font, BITMAP *icon, l_font *font_symbol, l_byte chr,
                              l_text filename, p_menu menu, p_menuitem next )
{

  p_filemenuitem fi = (p_filemenuitem)_malloc(sizeof(t_filemenuitem));

  p_menuitem f = new_sub_menu_ex(name, enable, info_text, font, icon, font_symbol, chr, menu, next);

  if ( f && fi ) {

     memcpy(fi, f, sizeof(t_menuitem));

     fi->filename = _strdup(filename);

  };

  _free(f);

  return fi;

};


p_menuitem  menu_load_from_dir ( l_text path, l_text filename )
{

  struct t_ffblk f;
  p_filemenuitem item = NULL;
  p_menuitem first_menu = NULL;
  l_int done = 0;

  l_text fi = io_realpath(path, "*.*");

  done = io_findfirst(fi, &f, FA_DIREC);

  while ( !done ) {

    if ( f.info.ff_attrib & FA_DIREC && io_isfilename(f.info.ff_name) &&
         f.info.ff_name[0] != DIR_IGNORE ) {

        l_text alld = io_realpath(path, f.info.ff_name);

        if ( io_isdir(alld) ) {

            l_text path2 = io_realpath(path, f.info.ff_name);

            p_menuitem it = path2?menu_load_from_dir(path2, filename):NULL;

            item = new_sub_filemenu(f.info.ff_name, alld, new_menu(it), NULL);

            first_menu = add_menu_item_to_menuitem(first_menu, (p_menuitem)item);

            _free(path2);

        };

        _free(alld);

    };

    done = io_findnext(&f);

  };

  _free(fi);

  fi = io_realpath(path, filename);

  done = io_findfirst(fi, &f, FA_ARCH+FA_LINK);

  while ( !done ) {

        if ( f.info.ff_attrib & FA_ARCH ) {

          l_text allf = io_realpath(path, f.ff_filename);

          if ( io_isfile(allf) ) { /* it's file */

            l_int mymem = 0;

            BITMAP *icon = NULL;//load_file_icon(f.ff_filename, standard_menuicon_size, &mymem);

            item = new_filemenu_item_ex(f.ff_filename, NULL, 0, MSG_RUNAPP,
                                    true, TXT_ABOUTRUNAPP, mymem?MIF_SELFICON:0, NULL, icon,
                                    NULL, 0, allf, NULL);

            first_menu = add_menu_item_to_menuitem(first_menu, (p_menuitem)item);

          };

          _free(allf);

        };

        done = io_findnext(&f);
  };

  _free(fi);

  return first_menu;

};


static p_menuitem  get_quit_items ( void )
{

   p_menuitem p = new_menu_item(TXT_REFRESHDESKTOP, 0, 0, MSG_xREFRESH, NULL,
                new_menu_item(TXT_RESTART, 0, 0, MSG_xRESTART, NULL,
                new_menu_line(
                new_menu_item(TXT_SHUTDOWN, 0, 0, MSG_QUIT, NULL,
   NULL))));

   return p;

};


static p_menuitem  get_programs_items ( void )
{

   if ( !program_items ) program_items = menu_load_from_dir(DIR_PROGRAMS, "*.*");

   return program_items;

};


static p_menuitem  get_settings_items ( void )
{

   if ( !settings_items ) settings_items = menu_load_from_dir(DIR_SETTINGS, "*.*");

   return settings_items;

};


static p_menuitem  get_help_items ( void )
{

   if ( !help_items ) help_items = menu_load_from_dir(DIR_HELP, "*.*");

   return help_items;

};


static p_menuitem  get_minized_items ( void )
{

  p_menuitem item = NULL;

  if ( desktopline_minimalized ) {

      p_item p = desktopline_minimalized->first(desktopline_minimalized);

      p_item f = p;

      if ( p )
      do {

           p_minmenuitem x = (p_minmenuitem)(p->rec);

           if ( x ) {

               p_minmenuitem i = new_minmenu_item(MSG_REFRESHMINIMIZED, x->appinfo, NULL);

               item = add_menu_item_to_menuitem(item, (p_menuitem)i);

           };

           p = p->next;

      } while ( p != f );

  };

  return item;

};

/*
  translate minimalized menu, that return right object that was minimalized

  and pushed.
*/

static void    minmenu_trans ( p_object o, p_event event )
{
       l_dword msg = desktop->execute_view(desktop, VIEW(o));

       if ( msg != MSG_NOTHING && msg != MSG_CANCEL ) {

          if ( msg == MSG_REFRESHMINIMIZED ) { /* refresh minimized */

              p_minmenuitem i = (p_minmenuitem)menu_get_lastitem_called(MENUVIEW(o)->menu);

              l_long pos = menu_get_item_pos(MENUVIEW(o)->menu, (p_menuitem)i);

              clear_event(event);

              if ( i && i->appinfo.obj ) { /* object exist */

                  VIEW(i->appinfo.obj)->show(VIEW(i->appinfo.obj)); /* show it */

                  if ( pos >= 0 ) /* free it */

                      desktopline_minimalized->free_index(desktopline_minimalized, pos);


              };

          } else { /* other message */

               set_event(event, EV_MESSAGE, msg, o);

               o->put_event(o, event);

               clear_event(event);
          };

       };
};




/*
  translate application menu, that return right application that was

  pushed in application menu.
*/

void   filemenu_trans ( p_object o, p_event event )
{
       l_dword msg = desktop->execute_view(desktop, VIEW(o));

       if ( msg != MSG_NOTHING && msg != MSG_CANCEL ) {

          if ( msg == MSG_RUNAPP ) { /* run application was pressed */

              p_filemenuitem i = (p_filemenuitem)menu_get_lastitem_called(MENUVIEW(o)->menu);

              clear_event(event);

              if ( i ) {

                  if ( !run_file(i->filename) )  /* run application */

                      seal_error(ERR_INFO, "%s \n\n%s", TXT_CANTRUNFILE, i->filename);  /* error in application running */

              };

          } else { /* other message */

               set_event(event, EV_MESSAGE, msg, o);

               o->put_event(o, event);

               clear_event(event);
          };

       };
};




/* t_desktopline */

t_rect  desktop_size_limits ( p_view o ) /* !!! redefine desktop size_limits */
{

  return rect_assign(0, ID_SIZE+2, rect_sizex(o->bounds), rect_sizey(o->bounds));

};


t_rect  desktopline_size_limits ( p_view o )
{

  return rect_assign(1, 1, rect_sizex(o->bounds)-1, rect_sizey(o->bounds)-1);

};


void       desktopline_remove ( p_object o, p_object sub )
{

   obj_remove(o, sub);

   DESKTOPLINE(o)->refresh_place(DESKTOPLINE(o));

};


t_object*  desktopline_insert ( p_object o, p_object sub )
{

   p_object ob = obj_insert(o, sub);

   if ( ob ) DESKTOPLINE(o)->refresh_place(DESKTOPLINE(o));

   return ob;

};


void    desktopline_draw ( p_view o )
{
  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    o->background(o, out, rect_move(r, p.x, p.y));

    button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_WHITE), COLOR(CO_WHITE));

  };

  o->end_of_paint(o, r);
};


void  desktopline_refresh_place ( p_desktopline o )
{

   p_object ob = OBJECT(o);
   p_object f = ob->first_view(ob);
   p_object p = f;

   t_rect   d = VIEW(desktop)->size_limits(VIEW(desktop));
   t_rect   d1 = VIEW(o)->size_limits(VIEW(o));

   t_rect   r = rect_assign(d.a.x, d.a.y, d.b.x, d1.b.y);
   t_rect   s = r;
   t_rect   x = r;
   t_rect   h = VIEW(o)->bounds;

   r.b.x = r.a.x;
   s.b.y = r.a.y + ID_SIZE;

   if ( p )
   do {

      t_rect c = VIEW(p)->bounds;

      r.b.x += rect_sizex(c);

      if ( r.b.x > s.b.x ) { /* longer as line */

         r.a.y = s.a.y = r.b.y;
         r.b.y = s.b.y += ID_SIZE;
         r.a.y = s.a.y;

         r.a.x = s.a.x;
         r.b.x = s.a.x + rect_sizex(c);
      };

      VIEW(p)->change_bounds(VIEW(p), r);

      r.b.x++;
      r.a.x = r.b.x;

      p = p->next_view(p);

   } while ( p != f );

   r = rect_assign(h.a.x, h.a.y, s.a.y > x.a.y ? x.b.x : r.a.x+5, s.b.y+1);

   VIEW(o)->change_bounds(VIEW(o), r);

};



/* t_desktopline init */

p_desktopline  _desktopline_init ( p_desktopline o )
{
  t_rect r = desktop->size_limits(desktop);

  if ( !o ) return NULL;

  r.b.y = r.a.y+ID_SIZE+2;

  clear_type(o, sizeof(t_desktopline));

  view_init(VIEW(o), r);

  /* object's functions */

  OBJECT(o)->remove = &desktopline_remove;
  OBJECT(o)->insert = &desktopline_insert;


  /* view's functions */

  VIEW(o)->size_limits = &desktopline_size_limits;
  VIEW(o)->draw = &desktopline_draw;


  /* desktopline's functions */

  o->refresh_place = &desktopline_refresh_place;

  /* desktop limits redeclaration */

  if ( draw_under_line )

      desktop->size_limits = &desktop_size_limits; /* redefine desktop size limits */

  OBJECT(o)->set_options(OBJECT(o), VW_OF_IGNORELIM, true);
  OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE, false);

  return o;

};


void  desktopline_translate_event ( p_object o, p_event event )
{
  RETVIEW(o, event);

  view_translate_event(o, event); /* call old function */

  if ( event->type == EV_INFO )

     switch ( event->message ) {

        case MSG_MINIMIZE : if ( event->info ) {

            t_appinfo a = *((p_appinfo)(event->info));
            p_object  m = ((p_appinfo)(event->info))->obj;
            l_text    t = ((p_appinfo)(event->info))->title;

            clear_event(event);

            if ( m ) {

                VIEW(m)->hide(VIEW(m)); /* hide object */

                desktopline_minimalized->insert(desktopline_minimalized,
                                                new_minmenu_item(MSG_REFRESHMINIMIZED, a, NULL));

            };

        }; break;
     };

  if ( event->type == EV_MESSAGE )

     switch ( event->message ) {

        case MSG_xAPPLICATION : {

            t_rect r = VIEW(o)->bounds;

            p_menu m = new_menu(get_programs_items());

            p_menuview menu;

            r.a.y += r.b.y+1;

            clear_event(event);

            if ( m )

               menu = menuview_init(malloc(sizeof(t_menuview)), r, m);

            if ( menu ) filemenu_trans(OBJECT(menu), event);

        }; break;

        case MSG_xSETTINGS : {

            t_rect r = VIEW(o)->bounds;

            p_menu m = new_menu(get_settings_items());

            p_menuview menu;

            r.a.x += ID_SIZE;
            r.a.y += r.b.y+1;

            clear_event(event);

            if ( m )

               menu = menuview_init(malloc(sizeof(t_menuview)), r, m);

            if ( menu ) filemenu_trans(OBJECT(menu), event);

        }; break;

        case MSG_xHELP : {

            t_rect r = VIEW(o)->bounds;

            p_menu m = new_menu(get_help_items());

            p_menuview menu;

            r.a.x += ID_SIZE*2;
            r.a.y += r.b.y+1;

            clear_event(event);

            if ( m )

               menu = menuview_init(malloc(sizeof(t_menuview)), r, m);

            if ( menu ) filemenu_trans(OBJECT(menu), event);

        }; break;

        case MSG_xMINIZED : {

            t_rect r = VIEW(o)->bounds;

            p_menu m = new_menu(get_minized_items());

            p_menuview menu;

            r.a.x += ID_SIZE*4;
            r.a.y += r.b.y+1;

            clear_event(event);

            if ( m )

               menu = menuview_init(malloc(sizeof(t_menuview)), r, m);

            if ( menu ) minmenu_trans(OBJECT(menu), event);

        }; break;

     };

};


p_object desktopline_genbutton ( l_int ico, l_text info, l_dword msg )
{
  t_rect   r = rect_assign(0, 0, ID_SIZE, ID_SIZE);

  /* init new button (OPEN) */
  p_object b = OBJECT(icobutton_init(
                            _malloc(sizeof(t_icobutton)), /* memory for class t_button */
                            r, /* button area */
                            ICO_GET(ico),
                            info,
                            msg,    /* message of the button */
                            BF_PUSH)     /* flags of the button */
               );

  if ( b ) {

     VIEW(b)->brush.color = COLOR(CO_BLACK);

  };

  return b;
};


/*
   if program is quit, it calls this routine. It check for validity and

   return ZERO if it's able to quit Seal, otherwise it return NON-ZERO.
*/

l_dword    program_valid ( p_object o, l_dword msg )
{

   if ( msg == MSG_QUIT ) {

            t_rect r = VIEW(desktopline)->bounds;

            p_menu m = new_menu(get_quit_items());

            p_menuview menu = NULL;

            r.a.x += ID_SIZE*3;
            r.a.y += r.b.y+1;

            if ( m )

               menu = menuview_init(_malloc(sizeof(t_menuview)), r, m);

            if ( menu ) {

                  l_dword mg = desktop->execute_view(desktop, VIEW(menu));

                  if ( mg == MSG_NOTHING ) return 1;

                  else {

                      switch ( mg ) {

                           case MSG_xREFRESH : {  /* redraw screen */

                                 desktop->draw_view(desktop);

                                 mg = 0;

                           }; break;

                           case MSG_xRESTART : {  /* restart Seal */

                               /* not supported yet */

                           }; break;

                      };

                  };

                  if ( mg )  /* I'm sure to quit */

                     if ( msgbox(MW_QUESTION, MB_YES|MB_NO, "%s ?", TXT_AREYOUSUREYOUWANTTOQUITSEAL) != MSG_YES )

                         mg = 0;


                  return !mg;
            };
   };

   return 1;
};


static void  desktopline_standard_insertions ( void )
{

  program.valid = &program_valid;

  OBJECT(desktopline)->translate_event = &desktopline_translate_event;

  OBJECT(desktopline)->insert(OBJECT(desktopline), desktopline_genbutton(ICO_xPROGRAMS, NULL, MSG_xAPPLICATION));
  OBJECT(desktopline)->insert(OBJECT(desktopline), desktopline_genbutton(ICO_xSETTINGS, NULL, MSG_xSETTINGS));
  OBJECT(desktopline)->insert(OBJECT(desktopline), desktopline_genbutton(ICO_xHELP, NULL, MSG_xHELP));
  OBJECT(desktopline)->insert(OBJECT(desktopline), desktopline_genbutton(ICO_xSHUTDOWN, NULL, MSG_QUIT));
  OBJECT(desktopline)->insert(OBJECT(desktopline), desktopline_genbutton(ICO_xMINIZED, NULL, MSG_xMINIZED));

};



lib_exportable;


lib_begin ( void ) {

  if ( ap_process == AP_ALLOC ) {

          AP_EXPORTLIB(); /* export table defined by lib_exportable you find in seal.h */

  } else

  if ( ap_process == AP_INIT ) {

          AP_SETNUMOFCALLS(1); /* set max num of calls */

          draw_under_line = getininum_fromfile(INI_MAINFILE, INI_DESKTOP, "draw_under_line");

          datfile = conv_to_skipcolor_data(DLXGetFileData(ap_id), CO_SKIP_RED, CO_SKIP_GREEN, CO_SKIP_BLUE);

          get_programs_items();
          get_settings_items();
          get_help_items();

          desktopline_minimalized = list_init(_malloc(sizeof(t_list)), ((void (*)(void*))&free_menu_item), DAT_TEXT);
          desktopline = desktopline_init(_malloc(sizeof(t_desktopline)));

          OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(desktopline));

          desktopline_standard_insertions();

  } else

  if ( ap_process == AP_FREE ) { /* releasing of library */

      unload_datafile(datfile);
      datfile = NULL;

      dispose_list(&desktopline_minimalized, true);

  };


} lib_end;

