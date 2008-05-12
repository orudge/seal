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

#ifndef __MENUS_H_INCLUDED__
#define __MENUS_H_INCLUDED__


/* menu flags */

/* menu is vertical */
#define MF_VERMENU        0x01
/* menu is horizontal */
#define MF_HORMENU        0x02
/* menu starts from bottom */
#define MF_BOTTOM_MENU    0x04


/* menu action flags */

#define MA_NONE             1
#define MA_HMFORWARD       -6
#define MA_OTHER           -5
#define MA_ESC             -4
#define MA_END             -3
#define MA_BACKWARD        -2
#define MA_COMMAND         -1
#define MA_IAMSUBMENU       2
#define MA_SUBMENU          3
#define MA_FORWARD          4
#define MA_ENTER            5


/* standard menu sizes */

/* diference size */
#define STANDARD_MENULINE_SIZE         7
/* horizontal line width */
#define STANDARD_MENULINE_WIDTH       25
/* size between text and start of icon */
#define STANDARD_MENUDIFF_SIZE        25
/* size between left and right origins */
#define STANDARD_MENUBOUNDSDIFF_SIZE  20



/* menuitem flags */

/* none flags in the item */
#define MIF_NONE      0x00000
/* item will be checkable */
#define MIF_CHECK     0x00001
/* item was pressed and now it's enable */
#define MIF_CHECKOK   0x00002
/* item has own memory of the icon.  */
#define MIF_SELFICON  0x00004


/*
   palette of menuview object :
   ----------------------------

   standard colors from Seal.ini file, from section [colors] :

   index of palette        name in [colors]

   0                       menu_background
   1                       menu_background_gradient
   2                       menu_pasive_caption
   3                       menu_active_title
   4                       menu_active_caption
   5                       menu_disable_caption
*/
extern l_color  pal_menu[];

/*
   palette of hormenu object :
   ---------------------------

   standard colors from Seal.ini file, from section [colors] :

   index of palette        name in [colors]

   0                       menu_horizontal_background
   1                       menu_horizontal_background_gradient
   2                       menu_pasive_caption
   3                       menu_active_title
   4                       menu_active_caption
   5                       menu_disable_caption
*/
extern l_color  pal_hormenu[];

typedef struct t_menu     *p_menu;
typedef struct t_menuitem *p_menuitem;


#define standard_menuicon_size   system_item_size
#define standard_menuitem_size   system_item_size+4

/* standard menu item font */
extern l_font* standard_menuitem_font;


/* t_menu structure */

typedef struct t_menu {

  /* point to all items in the menu */
  p_menuitem   items;
  /* point to last selected item in the menu */
  p_menuitem   current;

} t_menu;



/* t_menuitem structure */

typedef struct t_menuitem {

  /* point to the next menuitem in the menu. if NULL, this is the last item */
  p_menuitem next;

  /* text in the item */
  l_text     name;
  /* text in the right of the item. shows HOTKEY of item */
  l_text     param;
  /* keycode of the hotkey for this item */
  l_int      hotkey;
  /* message, that's called whenever is item pressed. If the item is sub-menu,
     this contains MSG_NOTHING
  */
  l_dword    message;
  /* true if item is enable, otherwise contains false */
  l_bool     enable;
  /* if this entry is true, this item was pressed. */
  l_bool     lastcall;
  /* text to be visible, wnehever mouse..+CTRL+F1 is pressed under the item */
  l_text     info_text;
  /* flags of the item, see MIF_XXXX above. */
  l_int      flags;

  /* font of the item */
  l_font    *font;
  /* font for the symbol, that's placed on the left in the item */
  l_font    *font_symbol;
  /* symbol, that's placed on the left in the item */
  l_byte     char_symbol;
  /* we can use symbol or icon, icon is the 16,32 bitmap. if this item point to
     the bitmap that was created only for this item, then (flags) must contains
     MIF_SELFICON flag
  */
  BITMAP    *icon;

  /* reserved for next versions */
  l_char     reserved[16];

  /* if the t_menuitem.message is MSG_NOTHING, this point to sub-menu */
  p_menu     submenu;

} t_menuitem;



typedef struct t_menuview *p_menuview;


/* t_menuview */

/*
  object t_menuview is used for showing vertical menu or ANY menus that's inherited
  from this one.
*/

typedef struct t_menuview {

  /* inherite variables / functions from t_view and t_object class */
  struct t_view  obclass;

  /* pointer to menu items */
  p_menu         menu;
  /* internal variable */
  l_char         action;
  /* indicates (y) distance between two items in vertical menu, in horizontal (x) */
  t_point        between;
  /* (y) size of the item */
  l_rect         item_size;
  /* saved desktop. this desktop is saved whenever is menu showed and freed when the
     menu is hiden.
  */
  BITMAP        *safe_desktop;

  /* sub-menu of this menu */
  p_menuview     sub_menu;
  /* parent-menu of this menu */
  p_menuview     parent_menu;

  /* indicated flags of the menu, that can be one of MF_XXXX, see above. */
  l_int          flags;
  /* true if process is inside the menu */
  l_bool         inside;

  /* save desktop under menu before menu showing */
  void        (*save_desktop) ( p_menuview o );

  /* return order of the item (p) */
  l_int       (*at) ( p_menuview o, p_menuitem p );

  /* return item defined by index "item" */
  p_menuitem  (*index_of) ( p_menuview o, l_int item );

  /* draw item "item" */
  void        (*draw_item) ( p_menuview o, p_menuitem item );

  /* draw selected item */
  void        (*draw_current_item) ( p_menuview o );

  /* get area of item "item" moved from r.a.x, r.a.y */
  t_rect      (*get_item_box_ex) ( p_menuview o, p_menuitem item, t_rect r );

  /* get area of item "item" */
  t_rect      (*get_item_box) ( p_menuview o, p_menuitem item );

  /* get area of item "item" specific for the mouse. */
  t_rect      (*get_mouseitem_box) ( p_menuview o, p_menuitem item );

  /* return prevoius item from "item" */
  p_menuitem  (*prev_item) ( p_menuview o, p_menuitem item );

  /* return next item from "item" */
  p_menuitem  (*next_item) ( p_menuview o, p_menuitem item );

  /* return next / previous item from "item", whenever is
     forward=true / forward=false
  */
  p_menuitem  (*get_next_item) ( p_menuview o, p_menuitem item, l_bool forward );

  /* create new sub-menu defined by items in "menu" in rect (r) and "action" will
     contain last action from sub-menu. return message if sub-menu was pressed,
     otherwise it returns MSG_NOTHING
  */
  l_dword     (*run_new_menu) ( p_menuview o, t_rect r, l_int *action, p_menu menu );

  /* get item of menuview, where's the mouse pointer is under it. */
  p_menuitem  (*get_item_from_mouse) ( p_menuview o, t_point p );

  /* return true, if mouse pointer is in parent menu */
  l_bool      (*is_mouse_in_one_of_parent_menu) ( p_menuview o, t_point p );

  /* return true, if mouse pointer is in parent item, the item from where this
     or prant, .... menu were created
  */
  l_bool      (*is_mouse_in_one_of_parent_item) ( p_menuview o, t_point p );

  /* return true, if one of sub-menus has state (st) */
  l_bool      (*is_one_of_subs_state) ( p_menuview o, l_dword st );

  /* return oldest parent menuview. the menuview from others were created */
  p_menuview  (*top_menuview) ( p_menuview o );

  /* if horizontal parent exists, this returns pointer to horizontal menu, otherwise
     returns NULL
  */
  p_menuview  (*is_hor_menu_one_of_parents) ( p_menuview o );

} t_menuview;


#define MENUVIEW(o)  ((p_menuview)(o))


/* t_hormenu functions */

void        hormenu_draw ( p_view o );
l_dword     hormenu_execute ( p_object o );

t_rect      hormenu_get_item_box_ex ( p_menuview o, p_menuitem item, t_rect r );
void        hormenu_save_desktop ( p_menuview o );
void        hormenu_draw_item ( p_menuview o, p_menuitem item );


extern p_menuview  (*hormenu_init) ( p_menuview o, t_rect r, p_menu menu );
p_menuview  _hormenu_init ( p_menuview o, t_rect r, p_menu menu );

extern p_menuview  (*hormenu_init_ex) ( p_menuview o, t_rect r, l_rect item_size, l_rect icon_size, p_menu menu );
p_menuview  _hormenu_init_ex ( p_menuview o, t_rect r, l_rect item_size, l_rect icon_size, p_menu menu );



/* t_menuview functions */

l_bool      menuview_done ( p_object o );
l_dword     menuview_execute ( p_object o );
void        menuview_translate_event ( p_object o, t_event *event );
void        menuview_set_state ( p_object o, l_dword st, l_bool set );

t_rect      menuview_size_limits ( p_view o );
void        menuview_draw ( p_view o );
void        menuview_draw_under_rect ( p_view o, t_rect r );

void        menuview_save_desktop ( p_menuview o );
p_menuview  menuview_is_hor_menu_one_of_parents ( p_menuview o );
p_menuitem  menuview_index_of ( p_menuview o, l_int item );
l_int       menuview_at ( p_menuview o, p_menuitem p );
void        menuview_draw_item ( p_menuview o, p_menuitem item );
void        menuview_draw_current_item ( p_menuview o );
t_rect      menuview_get_item_box_ex ( p_menuview o, p_menuitem item, t_rect r );
t_rect      menuview_get_mousitem_box ( p_menuview o, p_menuitem item );
t_rect      menuview_get_item_box ( p_menuview o, p_menuitem item );
p_menuitem  menuview_prev_item ( p_menuview o, p_menuitem item );
p_menuitem  menuview_next_item ( p_menuview o, p_menuitem item );
p_menuitem  menuview_get_next_item ( p_menuview o, p_menuitem item, l_bool forward );
l_dword     menuview_run_new_menu ( p_menuview o, t_rect r, l_int *action, p_menu menu );
p_menuitem  menuview_get_item_from_mouse ( p_menuview o, t_point p );
l_bool      menuview_is_mouse_in_one_of_parent_menu ( p_menuview o, t_point p );
l_bool      menuview_is_mouse_in_one_of_parent_item ( p_menuview o, t_point p );
l_bool      menuview_is_one_of_subs_state ( p_menuview o, l_dword st );
p_menuview  menuview_top_menuview ( p_menuview o );


/*
 this function initialize menu to rect (r).
 Information about menu are stored in (menu) pointer we explained under.
 If r.b.x < r.a.x, it automaticaly set r.b.x to minimum width of menu,
 r.b.y is set automaticaly to minimum height.

 example :

 t_rect r = rect_assign(100, 100, 0, 0);

 p_menu p = new_menu(

                new_menu_item_ex("Hello", NULL, 0, MSG_CLOSE, true,
                                 "Info about Hello", MIF_NONE, font_system_bi, NULL, NULL, 0,
                new_menu_line(
                new_menu_item_ex("Quit", NULL, 0, MSG_QUIT, true,
                                 "This quit Seal", MIF_NONE, font_system_bd, NULL, NULL, 0,
               NULL)))
            );



 p_object menu = OBJECT(menuview_init(
                               malloc(sizeof(t_menuview)),
                               r,
                               p)
                        );


 ---show menu into the desktop

 desktop->execute_view(desktop, VIEW(menu));
*/
extern p_menuview  (*menuview_init) ( p_menuview o, t_rect r, p_menu menu );
p_menuview  _menuview_init ( p_menuview o, t_rect r, p_menu menu );

/*
 function is same like menuview_init function but was optimalized for
 the horizontal menu.
*/
extern p_menuview  (*menuview_init_ex) ( p_menuview o, t_rect r, l_rect item_size, l_rect icon_size, p_menu menu );
p_menuview  _menuview_init_ex ( p_menuview o, t_rect r, l_rect item_size, l_rect icon_size, p_menu menu );


/* t_menu and t_menuitem functions */

#define     menu_clear_lastitem_called(m) menu_get_lastitem_called(m)
l_long      menu_get_item_pos ( p_menu m, p_menuitem x );
/*
  When you have get some action from the menu, you can use the following functions
  for more information about the action. An Action may be difference by messages,
  but sometimes you need to distinguish two items that have the same message.
  The best example is the desktop menu, that contains same messages for file running.
  You can use these two function for distinguish it:
*/

/*
 returns pointer t_menuitem structure, that contains information about last
 pushed item. When menu was closed without pushing, this return NULL. (m)
 is pointer to menu, from where you want to check the state.
*/
p_menuitem  menu_get_lastitem_called ( p_menu m );

/* returns (flags) from the item that's defined by the message (message).*/
l_int       menu_get_item_flags ( p_menu m, l_dword message );

l_dword     menu_get_hotkey_message ( p_menu m, l_int keycode );
l_int       menu_get_max_item ( p_menu m );
t_point     menu_get_ver_size ( p_menu menu, l_int h );
t_point     menu_get_hor_size ( p_menu menu, l_int h, l_int icon_size );

/* adds new item (src) at the end of (dst) item array. if (dst) is NULL, this
   return (src).
*/
p_menuitem  add_menu_item_to_menuitem ( p_menuitem dst, p_menuitem src );

/* adds items (items) into the position (pos) in menu (p) and returns true if all
   was successfull.
*/
l_bool      add_menu_item_to_menu ( p_menu p, l_int pos, p_menuitem items );

/*
 create new menu-line. Returns pointer to information about menu line.
*/
p_menuitem  new_menu_line ( p_menuitem next ); /* make new line */

/* free one menu item */
void        free_menu_item ( p_menuitem i );

/* free menu and all items */
void        free_menu ( p_menu i );

/* create new section for the menu. (item) is the first item of the menu.
   returns pointer you will use in initalization menuview function.
*/
p_menu      new_menu ( p_menuitem item ); /* make new menu */

/*
  create new submenu. Returns pointer to info about this submenu
  ( to new t_menuitem structure ). (name) is name of the item, (enable) is set
  to true if the item is enable and can be pushed, otherwise is set to false.
  (info_text) is text that will be showed whenever CTRL+F1 and mouse cursor is
  under this menu-item. (font) is font that will be used for this item.
  (icon) is icon of the item. (font_symbol) - sometimes you need to use some of
  the font symbols instead of the icon. (chr) is character of the symbol.
  If you not use it, simple set zero. (menu) is new menu that contains
  other menu-items. (next) is next item in this hierarchy.
*/
p_menuitem  new_sub_menu_ex ( l_text name, l_bool enable, l_text info_text,
                              l_font *font, BITMAP *icon, l_font *font_symbol, l_byte chr,
                              p_menu menu, p_menuitem next );

/*
  create new menuitem. Returns pointer to info about this item
  ( to new t_menuitem structure ). (name) is the name of the item, (param) is
  text that will be showed for (hotkey)-what's keycode of the item.(message) is
  the message that is called even when the item is pushed down.(enable) is set
  to true if the item is enable and can be pushed, otherwise is set to false.
  (info_text) is text that will be showed whenever CTRL+F1 and mouse cursor is
  under this menu-item. (font) is font that will be used for this item. (icon)
  is icon of the item. (font_symbol) - sometimes you need to use some of the font
  symbols instead of the icon. (chr) is character of the symbol.
  If you don't use it, simple set zero. (menu) is new menu that contains other
  menu-items. (next) is next item in this hierarchy.
*/
t_rect  menuview_get_mouseitem_box ( p_menuview o, p_menuitem item );

p_menuitem  new_menu_item_ex ( l_text name, l_text param, l_int hotkey, l_dword message,
                               l_bool enable, l_text info_text, l_int flags,
                               l_font *font, BITMAP *icon, l_font *font_symbol, l_byte chr,
                               p_menuitem next );

/* it's short version of new_sub_menu_ex function. */
#define new_sub_menu(name,menu,next) \
                                new_sub_menu_ex(name, true, NULL, NULL, NULL, NULL, 0, menu, next)

/* it's short version of new_menu_item_ex function. */
#define new_menu_item(name,param,hk,msg,it,next) \
                                new_menu_item_ex(name, param, hk, msg, true, it, 0, NULL, NULL, NULL, 0, next)

/*
  create new check menu item. This item is same like menuitem, but when you hit it,
  it shows (\/) - checkbox symbol.
*/
#define new_menu_check_item(name,param,is_check,hk,msg,it,next) \
                                new_menu_item_ex(name, param, hk, msg, true, it, MIF_CHECK+(is_check*MIF_CHECKOK), NULL, NULL, NULL, 0, next)

#endif

