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

/* Revision History (as of 01/04/2002):
 *
 * 01/04/2002 - Changed lnk_nice_link to lnk_get_title (orudge)
 */

#include <seal.h>
#include <txts.h>
#include <menus.h>
#include <button.h>
#include <app.h>
#include <cp.h>
#include <colors.h>
#include <bmps.h>
#include <vfile.h>
#include <files.h>
#include <unistd.h>
#include <dir.h>
#include <lnk.h>
#include <screen.h>
#include <stattext.h>
#include <dos.h>
#include <time.h>
#include <dialogs.h>
#include <grfx-f.h>

/* items of program menu */
static p_menuitem       program_items = NULL;

/* items of help menu */
static p_menuitem       help_items = NULL;

p_view IcoPrg = NULL, IcoSet = NULL, IcoDoc = NULL, IcoSht = NULL, Taskbar = NULL;
/* desktop menu about texts */

p_list Tasklst = NULL;

l_int BarPosition = 1;

p_object init_desktop ( void );

#define  DIR_PROGRAMS  "./PROGRAMS"

#define  DIR_HELP      "/documents/"

#define  DIR_IGNORE  '$'

#define  MSG_RUNAPP            100110
#define  MSG_xAPPLICATION      100111
#define  MSG_xSETTINGS         100112
#define  MSG_xHELP             100113

#define  ID_SIZE               36  /* desktop icon size */

t_rect   its_zone;

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

#define new_sub_filemenu(name,f,menu,icon,next) \
                                new_sub_filemenu_ex(name, true, NULL, NULL, icon, NULL, 0, f, menu, next)
#define new_filemenu_item(name,param,hk,msg,it,f,icon,next) \
                                new_filemenu_item_ex(name, param, hk, msg, true, it, 0, NULL, icon, NULL, 0, f, next)
#define new_filemenu_check_item(name,param,is_check,hk,msg,it,f,next) \
                                new_filemenu_item_ex(name, param, hk, msg, true, it, MIF_CHECK+(is_check*MIF_CHECKOK), NULL, NULL, NULL, 0, f, next)

static p_filemenuitem  new_filemenu_item_ex ( l_text name, l_text param, l_int hotkey, l_dword message,
                               l_bool enable, l_text info_text, l_int flags,
                               l_font *font, BITMAP *icon, l_font *font_symbol, l_byte chr,
                               l_text filename, p_menuitem next )
{

  p_filemenuitem fi = (p_filemenuitem)sf_malloc(sizeof(t_filemenuitem));

  p_menuitem f = new_menu_item_ex(name, param, hotkey, message, enable, info_text, flags, font, icon, font_symbol, chr, next);

  if ( f && fi ) {

     memcpy(fi, f, sizeof(t_menuitem));

     fi->filename = _strdup(filename);

  };

  sf_free(f);

  return fi;

};

static p_filemenuitem  new_sub_filemenu_ex ( l_text name, l_bool enable, l_text info_text,
                              l_font *font, BITMAP *icon, l_font *font_symbol, l_byte chr,
                              l_text filename, p_menu menu, p_menuitem next )
{

  p_filemenuitem fi = (p_filemenuitem)sf_malloc(sizeof(t_filemenuitem));

  p_menuitem f = new_sub_menu_ex(name, enable, info_text, font, icon, font_symbol, chr, menu, next);

  if ( f && fi ) {

     memcpy(fi, f, sizeof(t_menuitem));

     fi->filename = _strdup(filename);

  };

  sf_free(f);

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

            item = new_sub_filemenu(f.info.ff_name, alld, new_menu(it), get_icon_for_file(alld,f.info.ff_attrib,NULL), NULL);

            first_menu = add_menu_item_to_menuitem(first_menu, (p_menuitem)item);

            sf_free(path2);

        };

        sf_free(alld);

    };

    done = io_findnext(&f);

  };

  sf_free(fi);

  fi = io_realpath(path, filename);

  done = io_findfirst(fi, &f, FA_ARCH+FA_LINK);

  while ( !done ) {

        if ( f.info.ff_attrib & FA_ARCH ) {

          l_text allf = io_realpath(path, f.ff_filename);
          l_text nicename = _strdup(lnk_get_title(allf)); //f.ff_filename);   - OCR - 01/04/2002
//          l_text nicename = lnk_nice_link(f.ff_filename);
         // if ( io_isfile(allf) ) { /* it's file */

            l_int mymem = 0;

            BITMAP *icon = get_icon_for_file(allf,f.info.ff_attrib,&mymem);

            item = new_filemenu_item_ex(nicename, NULL, 0, MSG_RUNAPP,
                                    true, "", mymem?MIF_SELFICON:0, NULL, icon,
                                    NULL, 0, allf, NULL);

            first_menu = add_menu_item_to_menuitem(first_menu, (p_menuitem)item);

       //   };

          _free(allf);
          _free(nicename);
        };

        done = io_findnext(&f);
  };

//  DEBUG_printf("Desktop V: Done adding menu items for '%s'\n", path);
  sf_free(fi);

  return first_menu;

};
////////////////////////////////////////////////////////////////////////////////
void   clock_func_callback ( p_object o ) {

  struct time tm;
  gettime(&tm);

  STATTEXT(o)->set_text ( STATTEXT(o), "%02d:%02d",tm.ti_hour,tm.ti_min);

  VIEW(o)->draw_me(VIEW(o));
};
BITMAP *memb = NULL;
////////////////////////////////////////////////////////////////////////////////
void  dmpi_draw ( p_view o ) {

 t_rect  r = o->get_local_extent(o);
 t_point p;

 BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

   __dpmi_free_mem_info info;

   l_uchar pc = 0;
   l_color c;

   __dpmi_get_free_memory_information(&info);

   pc = info.total_number_of_free_pages*100/info.total_number_of_physical_pages;

   if ( pc > 50 ) c = col2col( COLOR(CO_YELLOW),COLOR(CO_LIGHTGREEN), pc-50,50);
   else c = col2col( COLOR(CO_LIGHTRED),COLOR(CO_YELLOW), pc,50);;

   set_format_text(&o->info_text, "%d%% free memory", pc);

   rectfill(out,p.x,p.y,p.x+r.b.x,p.y+r.b.y,color_deskbar_face);

   if ( memb ) masked_blit(memb,out,0,0,p.x,p.y,memb->w,memb->h);

   rectfill(out,p.x+4,p.y+3,p.x+10,p.y+7,c);
   button(out,p.x+4,p.y+3,p.x+10,p.y+7,col2col(c,COLOR(0),1,2),col2col(c,COLOR(15),2,3));

  };

  o->end_of_paint(o, r);
};
////////////////////////////////////////////////////////////////////////////////
void  dmpi_real_func_callback ( p_object o ) {
  VIEW(o)->draw_me(VIEW(o));
};
////////////////////////////////////////////////////////////////////////////////

typedef struct t_it *p_it;

typedef struct t_it {

  struct t_view    obclass;
  l_text filename;
  l_text caption;
  BITMAP *icon;
  l_bool ownmem;

} t_it;

#define IT(o)   ((p_it)(o))

p_list its = NULL;

#define MSG_IT_REORGANISE 6600
#define MSG_IT_OPEN_ME    6601
#define MSG_IT_DEL_ME     6602

static p_menuitem  it_menu_items (  ) {
  return  new_menu_item_ex("Reorganise icons",NULL,NULL,MSG_IT_REORGANISE,true,NULL,NULL,NULL,NULL,NULL,0,
          new_menu_item_ex("Open",            NULL,NULL,MSG_IT_OPEN_ME,   true,NULL,NULL,NULL,NULL,NULL,0,
          new_menu_item_ex("Del" ,            NULL,NULL,MSG_IT_DEL_ME,    true,NULL,NULL,NULL,NULL,NULL,0,
          NULL)));

};
void it_save_pos ( p_it o ) {
  l_text id = strupr(_strdup(o->filename));
  l_long x, y;
  x = VIEW(o)->bounds.a.x - its_zone.a.x;
  y = VIEW(o)->bounds.a.y - its_zone.a.y;
  setini_tofile ("desktop.inf",  id, "X", (l_text)&x, INI_DECANUM);
  setini_tofile ("desktop.inf",  id, "Y", (l_text)&y, INI_DECANUM);
  _free(id);
  // Else system don't update icons position when move the taskbar
  if (inimem) {_free(inimem);inimem=NULL;}
  if (inisavefile) {_free(inisavefile);inisavefile=NULL;}
};

void popup_go_menu ( p_it o ) {
  t_rect r = rect_assign(mouse->where.x,mouse->where.y,mouse->where.x,mouse->where.y);
  t_event event;
  p_menu m = new_menu( it_menu_items() );
  p_menuview menu;
  l_dword msg = NULL;

  if ( m )
    menu = menuview_init(malloc(sizeof(t_menuview)), r, m);

  if ( menu )
    msg = desktop->execute_view(desktop, VIEW(menu));

  if ( msg == MSG_IT_REORGANISE ) {
    t_rect rx = rect_assign(its_zone.a.x, its_zone.a.y, its_zone.a.x + 90, its_zone.a.y + 50);
    p_item p  = its->first(its);
    p_item x  = p;
    do {
      VIEW(p->rec)->change_bounds(VIEW(p->rec),rx);
      it_save_pos(IT(p->rec));
      rx.a.x += 90;
      rx.b.x += 90;
      if (rx.b.x > its_zone.b.x ) {
        rx.a.x = its_zone.a.x;
        rx.b.x = its_zone.a.x + 90;
        rx.a.y += 60;
        rx.b.y += 60;
      };
      p = p->next;
    } while ( x != p );

  } else if ( msg == MSG_IT_OPEN_ME ) {
    run_file(o->filename);
  } else if ( msg == MSG_IT_DEL_ME ) {

  };
};

// Desktop's icons /////////////////////////////////////////////////////////////
void   it_draw ( p_view o )
{
 t_rect  r = o->get_local_extent(o);
 t_point p;

 BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    stretch_sprite(out, IT(o)->icon, r.a.x+p.x+((r.b.x-r.a.x)/2)-16, r.a.y+p.y, 32 , 32 );

    textout_draw_rect_ex(out, o->font, IT(o)->caption, -1,
                                        p.x + r.a.x,
                                        p.y + r.a.y + 34,
                                        p.x + r.b.x,
                                        p.y + r.b.y,
                                        TX_ALIGN_CENTERX, color_desktop_text, color_desktop_face, 1,2);
  };

  o->end_of_paint(o, r);
};
////////////////////////////////////////////////////////////////////////////////
void   it_translate_event ( p_object o, t_event *event )
{

 if ( event->type & EV_MOUSE ) {
    if ( OBJECT(mouse)->state & MO_SF_MOUSELDOUBLE ) {

      clear_event(event);
      run_file(IT(o)->filename);


    } else if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) {
      l_word dm = (VIEW(o)->is_top_view(VIEW(o)) && VIEW(o)->drag_mode & DM_DRAGCONTEXT)?DM_DRAGCONTEXT:0;
      t_rect old = VIEW(o)->bounds;


      VIEW(o)->drag_view(VIEW(o), DM_DRAGMOVE+dm, event);
      VIEW(o)->draw_me(VIEW(o));

      if ( !rect_equals(old,VIEW(o)->bounds) ) it_save_pos(IT(o));

      clear_event(event);
    } else if ( OBJECT(mouse)->state & MO_SF_MOUSERUP ) {
      clear_event(event);
      popup_go_menu(IT(o));
    };
  };


};
l_bool it_done ( p_object o ) {

  if ( IT(o)->filename ) _free(IT(o)->filename);
  if ( IT(o)->caption ) _free(IT(o)->caption);
  if ( IT(o)->ownmem && IT(o)->icon ) destroy_bitmap(IT(o)->icon);

  return view_done(o);
};
////////////////////////////////////////////////////////////////////////////////
p_it   _it_init ( p_it o, t_rect r, l_text filename , l_text caption , BITMAP *icon, l_bool ownmem )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_it));

  view_init(VIEW(o), r);

  o->filename = _strdup(filename);
  o->caption  = _strdup(caption);
  o->icon     = icon;
  o->ownmem   = ownmem;
  VIEW(o)->drag_mode |=  DM_DRAGMOVE+drag_context_ok*DM_DRAGCONTEXT;
  VIEW(o)->draw_mode &= ~(DWM_CANTACCELMOVE*accel_moving_ok);

  OBJECT(o)->translate_event = &it_translate_event;
  OBJECT(o)->done = &it_done;

  VIEW(o)->draw = &it_draw;
  VIEW(o)->transparent = true;

  OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE+OB_OF_ENABLE, false);

  return o;

};
////////////////////////////////////////////////////////////////////////////////
void free_it ( void *o ) {
  if ( o )
    dispose(OBJECT(o));

};
////////////////////////////////////////////////////////////////////////////////
p_list load_it_from_dir_in_object(p_object object, t_rect r, l_text path ) {

  p_list list = list_init(_malloc(sizeof(t_list)),&free_it,0);

  t_rect rx = rect_assign(r.a.x, r.a.y, r.a.x + 90, r.a.y + 50);

  struct t_ffblk f;

  l_int done = 0;
  l_text fi = io_realpath(path, "*.*");

  its_zone = r;

  done = io_findfirst(fi, &f, FA_ARCH+FA_DIREC);

  while ( !done ) {
    if ( io_isfilename (f.ff_filename) ) {
      l_text realfile = io_realpath(path, f.ff_filename);
      l_text nicename = lnk_nice_link(f.ff_filename);
      l_bool ownmem = false;
      l_text id = strupr(_strdup(realfile));
      l_long *p = NULL, x = 0, y = 0;
      p_it it = NULL;
      BITMAP *bmp = get_icon_for_file_ex(realfile,f.info.ff_attrib,&ownmem,32);
      t_rect fr = rx;

      p = (long *)getini_fromfile("desktop.inf", id, "X");
      if ( p ) x = *(p);
      p = (long *)getini_fromfile("desktop.inf", id, "Y");
      if ( p ) y = *(p);
      if ( p ) {
        fr = rect_assign(r.a.x+x,r.a.y+y,r.a.x+x+90,r.a.y+y+50);
        if (fr.b.x > r.b.x ) fr = rx;
      };

      it = _it_init( _malloc(sizeof(t_it)), fr, realfile , nicename, bmp,ownmem );
      object->insert(object,OBJECT(it));

      list->insert(list,it);
      rx.a.x += 90;
      rx.b.x += 90;
      if (rx.b.x > r.b.x ) {
        rx.a.x = r.a.x;
        rx.b.x = r.a.x + 90;
        rx.a.y += 60;
        rx.b.y += 60;
      };
      _free(nicename);
      _free(realfile);
      _free(id);
    };
    done = io_findnext(&f);
  };
  _free(fi);
  return list;
};
////////////////////////////////////////////////////////////////////////////////
typedef struct t_userbox *p_userbox;

typedef struct t_userbox {

  struct t_view obclass;

} t_userbox;
p_userbox userbox = NULL;
////////////////////////////////////////////////////////////////////////////////
void userbox_draw(p_view o)
{
  t_rect r = o->get_local_extent(o);
  t_point p;
  BITMAP *out = o->begin_paint(o, &p, r);
  if (out) {
    rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, color_deskbar_face);
    button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, color_deskbar_shadow, color_deskbar_light );
  };

  o->end_of_paint(o, r);
};
////////////////////////////////////////////////////////////////////////////////
p_userbox userbox_init(p_userbox o, t_rect r)
{
  if (!o) return NULL;

  clear_type(o, sizeof(t_userbox));

  view_init(&o->obclass, r);

  VIEW(o)->draw = &userbox_draw;

  return o;
};
////////////////////////////////////////////////////////////////////////////////
void refresh_userbox() {
 if ( !OBJECT(userbox) || !OBJECT(userbox)->last ) return;
 if ( BarPosition == 1 || BarPosition == 3 ) {

   p_object p = OBJECT(userbox)->first_view(OBJECT(userbox));
   p_object f = p;
   t_rect   b = rect_assign(VIEW(userbox)->bounds.a.x,VIEW(userbox)->bounds.a.y,VIEW(userbox)->bounds.b.x,VIEW(userbox)->bounds.a.y+2);
   l_int   ht = 2;
   l_int   wt = abs (b.a.x - b.b.x);

   do {
     t_rect r = VIEW(p)->bounds;
     l_int h = abs(r.a.y - r.b.y);
     l_int w = abs(r.a.x - r.b.x);
     r.a.y = ht;
     r.b.y = ht + 16;
     r.a.x = (wt-w)/2;
     r.b.x = (wt-w)/2+w;
     VIEW(p)->change_bounds(VIEW(p), r);
     ht += 18;
     p = OBJECT(OBJECT(p)->next_view(OBJECT(p)));
   } while(f != p);

   b.b.y = ht+2;

   VIEW(userbox)->change_bounds(VIEW(userbox), b);

 } else {
   p_object p = OBJECT(userbox)->first_view(OBJECT(userbox));
   p_object f = p;
   t_rect   b = rect_assign(VIEW(userbox)->bounds.b.x-2,VIEW(userbox)->bounds.a.y,VIEW(userbox)->bounds.b.x,VIEW(userbox)->bounds.b.y);
   l_int   wt = 2;

   do {
     t_rect r = VIEW(p)->bounds;
     l_int w = abs(r.a.x - r.b.x);
     b.a.x -= w + 2;
     wt += w + 2;
     p = p->next_view(p);
   } while(f != p);

   p = OBJECT(userbox)->first_view(OBJECT(userbox));

   do {
     t_rect r = VIEW(p)->bounds;
     l_int w = abs(r.a.x - r.b.x);
     r.a.x = wt - w - 2;
     r.b.x = wt - 2;
     r.a.y = 2;
     r.b.y = 18;
     VIEW(p)->change_bounds(VIEW(p), r);
     wt -= w + 2;
     p = p->next_view(p);
   } while(f != p);

   VIEW(userbox)->change_bounds(VIEW(userbox), b);
   if ( Taskbar ) {
     Taskbar->bounds.b.x = b.a.x - 2;
     Taskbar->draw_me(Taskbar);
   };
 };
 VIEW(userbox)->draw_view(VIEW(userbox));
};
////////////////////////////////////////////////////////////////////////////////
void add_to_userbox(p_object o)
{
  obj_insert(OBJECT(userbox), o);

  refresh_userbox();
};
////////////////////////////////////////////////////////////////////////////////
void remove_from_userbox(p_object o)
{
  obj_remove(OBJECT(userbox), o);

  refresh_userbox();
};
////////////////////////////////////////////////////////////////////////////////
typedef struct t_taskbar {

  struct t_view    obclass;

  p_object relative;
  l_int    oldnbwin;
  l_int    oldnbviswin;
  p_object oldtopwin;

  p_list lst;

  l_int sel;

} t_taskbar, *p_taskbar;
#define TASKBAR(o) ((p_taskbar)(o))
////////////////////////////////////////////////////////////////////////////////
void get_num_view_from( p_object o, l_dword tag, l_int *nb, l_int *visnb ) {
   if ( o && o->last ) {

     p_object p = o->first_view(o);
     p_object f = p;
     (*nb) = 0;
     (*visnb)=0;
     do {
       if ( p->tag & tag && nb ) {
         (*nb)++;
         if ( p->is_state(p, OB_SF_VISIBLE) ) (*visnb)++;
       };
       p = p->next_view(p);
     } while(f != p);
   };
};
l_bool taskbar_done ( p_object o ) {
  if ( Taskbar == VIEW(o) ) Taskbar = NULL;
  return view_done(o);
};
////////////////////////////////////////////////////////////////////////////////
void taskbar_update ( p_taskbar o ) {
  if ( o ) {
    get_num_view_from ( o->relative, TAG_WINDOW, &o->oldnbwin, &o->oldnbviswin );
    o->oldtopwin = o->relative->first(o->relative);
  };
};
////////////////////////////////////////////////////////////////////////////////
l_bool taskbar_changed( p_taskbar o ) {
  if ( o ) {
    l_bool result = false;
    l_int nb, visnb;
    get_num_view_from ( o->relative, TAG_WINDOW, &nb, &visnb );
    if ( nb != o->oldnbwin || visnb != o->oldnbviswin ) result = true;
    if ( o->oldtopwin != o->relative->first(o->relative) ) result = true;
    return result;
  };
};
////////////////////////////////////////////////////////////////////////////////
t_rect taskbar_rect( p_taskbar o, l_int id ) {
  t_rect  r = VIEW(o)->get_local_extent(VIEW(o));
  if ( BarPosition == 0 || BarPosition == 2 ) {
    l_int   w = min ( 150, r.b.x / o->oldnbwin );
    return  rect_assign ( r.a.x + ( id * w ), r.a.y, r.a.x + ( id * w ) + w - 2, r.b.y );
  } else {
    return  rect_assign ( r.a.x, r.a.y + ( id * 20 ), r.b.x, r.a.y + ( id * 20 ) + 18 );
  };
};
////////////////////////////////////////////////////////////////////////////////
void   taskbar_func_callback ( p_object o ) {
  if ( o && taskbar_changed(TASKBAR(o)) ) VIEW(o)->draw_me(VIEW(o));
};
////////////////////////////////////////////////////////////////////////////////
void    taskbar_draw ( p_view o ) {
  t_rect  r = o->get_local_extent(o);
  t_point p;
  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {
    l_int dwn = 0;

    if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN  ) dwn = 1;

    rectfill(out, p.x+r.a.x, p.y+r.a.y, p.x+r.b.x, p.y+r.b.y, color_deskbar_face );

    taskbar_update(TASKBAR(o));

    if ( TASKBAR(o)->oldnbwin ) {
      p_item xpp   = TASKBAR(o)->lst->first(TASKBAR(o)->lst);
      p_item f     = xpp;
      l_int id     = 0;

      do {
        p_object pp = OBJECT(xpp->rec);
          t_rect item_r = rect_move( taskbar_rect(TASKBAR(o),id), p.x, p.y);
          l_int dec = 0, z = 0;

          if ( (pp->is_state(pp, OB_SF_FOCUSED)) && (pp->is_state(pp, OB_SF_VISIBLE))  ) {
            button(out, item_r.a.x,
                        item_r.a.y,
                        item_r.b.x,
                        item_r.b.y,
                        color_deskbar_shadow, color_deskbar_light);

            rectfill(out, item_r.a.x+1, item_r.a.y+1, item_r.b.x-1,item_r.b.y-1, color_deskbar_selected_face);
            z = 1;
          } else {
            button(out, item_r.a.x,
                        item_r.a.y,
                        item_r.b.x,
                        item_r.b.y,
                        color_deskbar_light, color_deskbar_shadow);
          };
          if ( id == TASKBAR(o)->sel )

            if ( dwn ) { button(out, item_r.a.x,
                                        item_r.a.y,
                                        item_r.b.x,
                                        item_r.b.y,
                                        color_deskbar_shadow, color_deskbar_light);
              z = 1;
            };
            if ( pp->setup == &appwin_setup )
                if ( APPWIN(pp)->icon16 && WINDOW(pp)->flags & WF_SYSMENU ) {
                  l_int x = item_r.a.x + 4;
                  l_int y = item_r.a.y + ( ( item_r.b.y - item_r.a.y ) - APPWIN(pp)->icon16->h ) / 2;
                  draw_sprite(out, APPWIN(pp)->icon16, x+z, y+z);
                  dec += APPWIN(pp)->icon16->w + 4;
                };


            if ( WINDOW(pp)-> caption )
                  textout_draw_rect(out, o->font, WINDOW(pp)->caption, -1,
                                        item_r.a.x + 4 + dec + z,
                                        item_r.a.y + 2 + z,
                                        item_r.b.x - 4 + z,
                                        item_r.b.y - 2 + z,
                                       TX_ALIGN_CENTERY, (pp->is_state(pp, OB_SF_FOCUSED)) && (pp->is_state(pp, OB_SF_VISIBLE)) ? color_deskbar_selected_text : color_deskbar_text , TX_NOCOLOR, 1);
            id++;
           xpp = xpp->next; // Next ...
           }while(f != xpp) ;

      };



  };

  o->end_of_paint(o, r);


};
////////////////////////////////////////////////////////////////////////////////
void   taskbar_translate_event ( p_object o, t_event *event ) {

  RETVIEW(o, event);

  view_translate_event(o, event);

  if ( event->type & EV_MOUSE && ( OBJECT(mouse)->state & MO_SF_MOUSERUP || OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) ) {

    clear_event(event);

    if ( TASKBAR(o)->oldnbwin ) {
      t_point  p   = VIEW(o)->get_local_point(VIEW(o), mouse->where);
      p_item xpp   = TASKBAR(o)->lst->first(TASKBAR(o)->lst);
      p_item f     = xpp;
      l_int id     = 0;

      do {
        p_object pp = OBJECT(xpp->rec);
          t_rect item_r = taskbar_rect(TASKBAR(o),id);
          if ( rect_contains(item_r,p) ) {
            TASKBAR(o)->sel = id;
            if ( OBJECT(mouse)->state & MO_SF_MOUSERUP  ) {
              if ( pp->setup == &appwin_setup ) { // is it a appwin ?
                if ( APPWIN(pp)->menu ) {
                 t_point   xp = VIEW(o)->get_global_point(VIEW(o), point_assign(item_r.a.x,item_r.b.y+1));
                 t_rect     r = rect_assign(xp.x,xp.y,xp.x,xp.y);
                 l_dword msg;

                 p_menu m = new_menu(APPWIN(pp)->menu);

                 p_menuview menu;

                 clear_event(event);
                 if (m) menu = menuview_init(malloc(sizeof(t_menuview)), r, m);
                 if (menu) {
                   msg = desktop->execute_view(desktop, VIEW(menu));
                   if ( msg == MSG_MAXIMIZE && !pp->is_state(pp, OB_SF_VISIBLE) ) {
                     VIEW(pp)->show(VIEW(pp));
                     OBJECT(pp)->select(OBJECT(pp));
                   } else {
                     set_event_info(event, EV_MESSAGE, msg, pp, NULL);

                     pp->put_event(pp, event);

                     clear_event(event);
                   };
                 };
                };
              };
            } else if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) {
              l_bool in_view = false;

              VIEW(o)->draw_me(VIEW(o));

              while ( OBJECT(mouse)->state & MO_SF_MOUSELPRESS ) {
	            l_bool safe_in_view = in_view;
                p = VIEW(o)->get_local_point(VIEW(o), mouse->where);
                in_view = rect_contains(item_r,p);
                if ( in_view != safe_in_view )
                  VIEW(o)->draw_me(VIEW(o));

                o->get_event(o, event);
              };

              if ( in_view ) {
                VIEW(pp)->show(VIEW(pp));
                OBJECT(pp)->select(OBJECT(pp));
                VIEW(o)->draw_me(VIEW(o));
                clear_event(event);
              };
            };
          };
          id++;
      xpp = xpp->next; // Next ...
      }while(f != xpp) ;
    };
  };
};
////////////////////////////////////////////////////////////////////////////////
p_taskbar taskbar_init ( p_taskbar o, t_rect r, p_object relative ) {

  if (!o) return NULL;

  clear_type(o, sizeof(t_taskbar));

  view_init(VIEW(o), r);

  o->relative = relative;

  if ( !Tasklst ) Tasklst = list_init(malloc(sizeof(t_list)),0,0);

  o->lst = Tasklst;
  OBJECT(o)->done = &taskbar_done;
  VIEW(o)->draw = &taskbar_draw;

  OBJECT(o)->func_callback = &taskbar_func_callback;
  OBJECT(o)->translate_event = &taskbar_translate_event;

  OBJECT(o)->process_tick = 200;

  OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
  OBJECT(o)->set_options(OBJECT(o), OB_OF_STILLPROCESS, true);

  taskbar_update(o);

  return o;
};
////////////////////////////////////////////////////////////////////////////////
t_object*  dsk_insert ( p_object o, p_object sub ) {
  if ( sub->tag & TAG_WINDOW && Tasklst ) {
    Tasklst->insert(Tasklst,sub);
  };
  return obj_insert(o,sub);
};
t_object*  dsk_insert_before ( p_object o, p_object sub, p_object before ) {
  if ( sub->tag & TAG_WINDOW && Tasklst ) {
    Tasklst->insert(Tasklst,sub);
  };
  return obj_insert_before(o,sub,before);
};
void dsk_remove ( p_object o, p_object sub ) {
  if ( sub->tag & TAG_WINDOW && Tasklst ) {
    p_item it = Tasklst->find_rec( Tasklst,sub);
    if ( it ) Tasklst->remove_item( Tasklst,it);
  };
  obj_remove(o,sub);
};
void dsk_put_in_front_of ( p_object o, p_object before ){
  if ( o->owner ) {
    p_object owner = o->owner;
    owner->set_prefer(owner, o);
    if ( o != before ) {
      VIEW(o)->draw_overlays(VIEW(o));
      obj_remove(owner, o);
      obj_insert_before(owner, o, before);
      owner->reset_prefer(owner);
    };
  };
};

p_view  dsk_view_init ( p_view o, t_rect r ) {
  o = _view_init(o,r);
  OBJECT(o)->put_in_front_of = &dsk_put_in_front_of;
  return o;
};

static p_menuitem  get_programs_items ( void )
{

   if ( !program_items ) program_items = menu_load_from_dir(DIR_PROGRAMS, "*.*");
//   DEBUG_printf("Desktop V: get_programs_items() finished\n");
   return program_items;

};


static p_menuitem  get_help_items ( void )
{

   if ( !help_items ) help_items = menu_load_from_dir(DIR_HELP, "*.*");

   return help_items;

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
				  DEBUG_printf("DESKTOP V :: try to run \"%s\"\n",i->filename);
                  if ( !run_file(i->filename) )  /* run application */

                      seal_error(ERR_INFO, "%s \n\n%s", "Can't run file", i->filename);  /* error in application running */

              };

          } else { /* other message */

               set_event(event, EV_MESSAGE, msg, o);

               o->put_event(o, event);

               clear_event(event);
          };

       };
};

////////////////////////////////////////////////////////////////////////////////

void deskbar_setup ( p_object o ) {
  t_rect   r;
  t_rect   br;
  p_object b;
  l_int i;
  view_setup(o);

  r = VIEW(o)->get_local_extent(VIEW(o));

  if ( BarPosition == 0 || BarPosition == 2 )
    br = rect_assign(2, 2, r.b.y-2, r.b.y-2);
  else if ( BarPosition == 1 )
    br = rect_assign( r.b.x - 37, 2, r.b.x-2, 37);
  else
    br = rect_assign( 2, 2, 37, 37);

  b = OBJECT(icobutton_init( _malloc(sizeof(t_icobutton)),
                            br,
                            GETBMP("ICO_xPROGRAMS"),
                            TXT_PROGRAMS,
                            MSG_xAPPLICATION,
                            BF_NORMAL | BF_UNDERSEL) );

  if ( b ) {
     VIEW(b)->brush.color = color_deskbar_face;
     o->insert(o,b);
     if ( BarPosition == 0 || BarPosition == 2 )
       br = rect_move(br,r.b.y+2,0);
     else
       br = rect_move(br,0,38);
     IcoPrg = VIEW(b);
  };

  b = OBJECT(icobutton_init( _malloc(sizeof(t_icobutton)),
                            br,
                            GETBMP("ICO_xSETTINGS"),
                            TXT_CONTROL_PANEL,
                            MSG_xSETTINGS,
                            BF_NORMAL | BF_UNDERSEL) );

  if ( b ) {
     VIEW(b)->brush.color = color_deskbar_face;
     o->insert(o,b);

     if ( BarPosition == 0 || BarPosition == 2 )
       br = rect_move(br,r.b.y+2,0);
     else
       br = rect_move(br,0,38);

     IcoSet = VIEW(b);
  };

  b = OBJECT(icobutton_init( _malloc(sizeof(t_icobutton)),
                            br,
                            GETBMP("ICO_xHELP"),
                            TXT_DOCUMENTS,
                            MSG_xHELP,
                            BF_NORMAL | BF_UNDERSEL) );

  if ( b ) {
     VIEW(b)->brush.color = color_deskbar_face;
     o->insert(o,b);
     if ( BarPosition == 0 || BarPosition == 2 )
       br = rect_move(br,r.b.y+2,0);
     else
       br = rect_move(br,0,38);

     IcoDoc = VIEW(b);
  };

  b = OBJECT(icobutton_init( _malloc(sizeof(t_icobutton)),
                            br,
                            GETBMP("ICO_xSHUTDOWN"),
                            TXT_SHUT_DOWN,
                            MSG_QUIT,
                            BF_NORMAL | BF_UNDERSEL) );

  if ( b ) {
     VIEW(b)->brush.color = color_deskbar_face;
     o->insert(o,b);
     if ( BarPosition == 0 || BarPosition == 2 )
       br = rect_move(br,r.b.y+2,0);
     else
       br = rect_move(br,0,38);

     IcoSht = VIEW(b);
  };

  i = ( r.b.y - 20 ) / 2;

  if ( BarPosition == 0 || BarPosition == 2 )
    br = rect_assign(br.a.x,r.a.y+i,r.b.x - 8,r.b.y-i);
  else
    br = rect_assign( 3, br.a.y + 2, r.b.x - 3, r.b.y - 2);

  b = OBJECT(taskbar_init(_malloc(sizeof(t_taskbar)),br,OBJECT(desktop)));

  if ( b ) {
    o->insert(o,b);
    Taskbar = VIEW(b);
  };

  if ( BarPosition == 0 || BarPosition == 2 )
    br = rect_assign(br.b.x+2,br.a.y,r.b.x-4,br.b.y);
  else if ( BarPosition == 1 )
    br = rect_assign(2,2,r.b.x - 40,6);
  else
    br = rect_assign(40,2,r.b.x-2,6);

  if ( !userbox ) userbox = userbox_init(_malloc(sizeof(t_userbox)), br);
  else VIEW(userbox)->bounds = br;

  if ( userbox ) {
	o->insert(o,OBJECT(userbox));
	refresh_userbox();
  };

};
////////////////////////////////////////////////////////////////////////////////
void    deskbar_draw ( p_view o )
{
  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    o->background(o, out, rect_move(r, p.x, p.y));

    button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, color_deskbar_light, color_deskbar_shadow);

  };

  o->end_of_paint(o, r);
};

////////////////////////////////////////////////////////////////////////////////
void  deskbar_translate_event ( p_object o, p_event event )
{
  RETVIEW(o, event);

  view_translate_event(o, event); /* call old function */

  if ( event->type == EV_INFO ) {
     switch ( event->message ) {
        case MSG_MINIMIZE : if ( event->info ) {
            p_object  m = ((p_appinfo)(event->info))->obj;
            clear_event(event);
            if ( m ) VIEW(m)->hide(VIEW(m));
        }; break;
        case MSG_MAXIMIZE : if ( event->info ) {

            t_appinfo a = *((p_appinfo)(event->info));
            p_object  m = ((p_appinfo)(event->info))->obj;

            clear_event(event);

            if ( m ) {
              if (WINDOW(m)->flags & WF_MAXSIZE) {  /* if is already maximized... */
                t_rect rr = APPWIN(m)->orig_bounds;
                VIEW(m)->change_bounds(VIEW(m), rr);
                WINDOW(m)->flags -= WF_MAXSIZE;
              } else {                              /* ... or if isn't maximized... */
                t_rect   r = rect_assign ( ( BarPosition == 1 ) ? VIEW(o)->bounds.b.x + 1 : 0,
                                           ( BarPosition == 0 ) ? VIEW(o)->bounds.b.y + 1 : 0,
                                           ( BarPosition == 3 ) ? VIEW(o)->bounds.a.x - 1 : screen_width - 1,
                                           ( BarPosition == 2 ) ? VIEW(o)->bounds.a.y - 1 : screen_height - 1);
                APPWIN(m)->orig_bounds = VIEW(m)->bounds;
                VIEW(m)->change_bounds(VIEW(m), r);

                WINDOW(m)->flags |= WF_MAXSIZE;
              };
            };
        }; break;
     };
  };

  if ( event->type == EV_MESSAGE )

     switch ( event->message ) {

        case MSG_xAPPLICATION : {

            t_rect r = VIEW(IcoPrg)->get_global_bounds(VIEW(IcoPrg),VIEW(IcoPrg)->get_local_extent(VIEW(IcoPrg)));

            p_menu m = new_menu(get_programs_items());

            p_menuview menu;

            if ( BarPosition != 2 )
              r.a.y = r.b.y+1;
            else
              r.a.y -= menu_get_ver_size(m, standard_menuitem_size).y+1;

            clear_event(event);

            if ( m )

               menu = menuview_init(malloc(sizeof(t_menuview)), r, m);

            if ( menu ) filemenu_trans(OBJECT(menu), event);

        }; break;

        case MSG_xSETTINGS : {

             load_cp();

             clear_event(event);
        }; break;

        case MSG_xHELP : {

            t_rect r = VIEW(IcoDoc)->get_global_bounds(VIEW(IcoDoc),VIEW(IcoDoc)->get_local_extent(VIEW(IcoDoc)));

            p_menu m = new_menu(get_help_items());

            p_menuview menu;

            if ( BarPosition != 2 )
              r.a.y = r.b.y+1;
            else
              r.a.y -= menu_get_ver_size(m, standard_menuitem_size).y+1;

            clear_event(event);

            if ( m )

               menu = menuview_init(malloc(sizeof(t_menuview)), r, m);

            if ( menu ) filemenu_trans(OBJECT(menu), event);

        }; break;

     };
  if ( event->type & EV_MOUSE  &&  OBJECT(mouse)->state & MO_SF_MOUSELDOUBLE ) {

    if ( its ) dispose_list(&its, true);

    clear_event(event);

    obj_remove(o,OBJECT(userbox));

    dispose(o);

    desktop->draw_me(desktop);

    BarPosition++;
    if ( BarPosition == 4 ) BarPosition = 0;

    set_key_integer("current/desktop/barpos",BarPosition);

    o = init_desktop();

    if ( OBJECT(desktop) && OBJECT(desktop)->last ) {
     p_object p = OBJECT(desktop)->first_view(OBJECT(desktop));
     p_object f = p;
     do {
       if ( p->tag & TAG_WINDOW  ) if ( WINDOW(p)->flags & WF_MAXSIZE ) {
         t_rect   r = rect_assign ( ( BarPosition == 1 ) ? VIEW(o)->bounds.b.x + 1 : 0,
                                    ( BarPosition == 0 ) ? VIEW(o)->bounds.b.y + 1 : 0,
                                    ( BarPosition == 3 ) ? VIEW(o)->bounds.a.x - 1 : screen_width - 1,
                                    ( BarPosition == 2 ) ? VIEW(o)->bounds.a.y - 1 : screen_height - 1);
         VIEW(p)->change_bounds(VIEW(p), r);
       };
       p = p->next_view(p);
     } while(f != p);
    };

    Taskbar->draw_me(Taskbar);
  };

};
////////////////////////////////////////////////////////////////////////////////
p_view deskbar_init ( p_view o, t_rect r ) {

  if (!o) return NULL;

  clear_type(o, sizeof(t_view));

  view_init(o, r);

  VIEW(o)->brush.color = color_deskbar_face;

  VIEW(o)->draw = &deskbar_draw;

  OBJECT(o)->translate_event = &deskbar_translate_event;

  OBJECT(o)->setup = &deskbar_setup;

  OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
  OBJECT(o)->set_options(OBJECT(o), OB_OF_STILLPROCESS, true);
  OBJECT(o)->set_options(OBJECT(o), OB_OF_TOPSELECT, true);

  return o;
};
////////////////////////////////////////////////////////////////////////////////
l_dword    program_valid ( p_object o, l_dword msg )
{

   if ( msgbox( MW_QUESTION, MB_YES|MB_NO, TXT_AREYOUSURETOQUITSEAL ) == MSG_YES ) {
     if ( its ) dispose_list(&its, true);
     return 0;
   };
   return 1;
};

p_object init_desktop ( void ) {


          p_view o;
          t_rect r;

          switch ( BarPosition ) {
            case 0 : r = rect_assign(0,0,screen_width-1,38); break;
            case 1 : r = rect_assign(0,0,100,screen_height-1); break;
            case 2 : r = rect_assign(0,screen_height-39,screen_width-1,screen_height-1); break;
            case 3 : r = rect_assign(screen_width-101,0,screen_width-1,screen_height-1); break;
          };

          o = deskbar_init(_malloc(sizeof(t_view)),r);

          OBJECT(desktop)->insert(OBJECT(desktop),OBJECT(o));

          its = load_it_from_dir_in_object( OBJECT(desktop),
                                            rect_assign( ( BarPosition == 1 ) ? r.b.x+10 : 10,
                                            ( BarPosition == 0 ) ? r.b.y+10 : 10,
                                            ( BarPosition == 3 ) ? r.a.x-10 : screen_width - 10,
                                            ( BarPosition == 2 ) ? r.a.y-10 : screen_height - 10 ) , "/");

          program.valid = &program_valid;

          o->draw_view(o);

          return OBJECT(o);
};

////////////////////////////////////////////////////////////////////////////////
SetInfoAppName("Desktop V");
SetInfoDesciption("A new desktop with taskbar and cool stuff");
SetInfoCopyright("(c) Copyright 2001 Point Mad. All rights reserved");
SetInfoManufacturer("Point Mad");

DLXUSE_BEGIN
  LIBLOADS_BEGIN
  LIBLOADS_END
  LIBEXPORT_BEGIN
    LIBEXPORT(add_to_userbox)
    LIBEXPORT(remove_from_userbox)
  LIBEXPORT_END
DLXUSE_END

////////////////////////////////////////////////////////////////////////////////
lib_begin ( void ) {

  if ( ap_process == AP_ALLOC ) {

          AP_EXPORTLIB(); /* export table defined by lib_exportable you find in seal.h */

  } else

  if ( ap_process == AP_INIT ) {

          p_stattext st;
          p_view br = NULL;
          struct time tm;

          AP_SETNUMOFCALLS(1); /* set max num of calls */

          if ( !key_exists("current/desktop/barpos") ) create_key("current/desktop","barpos");

          else BarPosition = get_key_integer("current/desktop/barpos");

          OBJECT(desktop)->insert = &dsk_insert;
          OBJECT(desktop)->remove = &dsk_remove;
          OBJECT(desktop)->insert_before = &dsk_insert_before;

          view_init = &dsk_view_init;



          init_desktop();

          get_programs_items();
          get_help_items();

          gettime(&tm);
          st = stattext_init(_malloc(sizeof(t_stattext)),rect_assign(0, 0, 30, 16),TX_ALIGN_CENTER,"%02d:%02d",tm.ti_hour,tm.ti_min);
          VIEW(st)->brush.color = color_deskbar_face;
          OBJECT(st)->func_callback = &clock_func_callback;
          st->color_text = color_deskbar_text;
          add_to_userbox(OBJECT(st));

          memb = load_image("bmp/mem.bmp");

          br = view_init( _malloc(sizeof(t_view)),rect_assign(0, 0, 16, 16) );
          VIEW(br)->draw = &dmpi_draw;
          OBJECT(br)->func_callback = &dmpi_real_func_callback;

          add_to_userbox(OBJECT(br));

          init_stillprocess ( OBJECT(st), 1000 );
          init_stillprocess ( OBJECT(br), 1000 );



  } else

  if ( ap_process == AP_FREE ) { /* releasing of library */


  };


} lib_end;

