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

#ifndef __DIALOGS_H_INCLUDED__
#define __DIALOGS_H_INCLUDED__

#include <stdarg.h>
#include "app.h"
#include "button.h"
#include "scroller.h"
#include "stattext.h"

/* dialogs names */

#ifndef   TXT_SIZE
#define   TXT_SIZE     INI_TEXT("Size")
#endif
#ifndef   TXT_TEXT
#define   TXT_TEXT     INI_TEXT("Text")
#endif


/* memory delta to name in item */

#define LISTBOX_ITEM_NAME    sizeof(p_listbox_item)  /* from where(in listbox_item) start the name */


/* listbox item flags */

#define LIF_NONE          0x0000

/* icon has own memory for the item. it means the icon will be destryed automaticaly
   from the list, whenever is list of items destroyed
*/
#define LIF_MEMORY        0x0001


/* listbox flags */

#define LF_NONE           0x0000
/* select item whenever is mouse under it */
#define LF_UNDERSEL       0x0001
/* enable of selection of more items (for copying for example) */
#define LF_SELECTABLE     0x0002
/* whenever is object destroyed, it also destroy the list */
#define LF_SELFLIST       0x0004


/* textline flags */

/* none flags */
#define TF_NONE           0x0000
/*
   text in t_textline can't be rewrited. it's used, when you want only
   read text in t_textline. if this bit isn't set, it able writing into textline.
*/
#define TF_REWRITEUNABLE  0x00001

#define TF_ALIGN_RIGHT    0x00002


/* history flags */

/* none flags */
#define HF_NONE           TF_NONE
/*
   text in t_history can't be rewrited. it's used, when you want only
   read text in t_history. if this bit isn't set, it able writing into history.
*/
#define HF_REWRITEUNABLE  TF_REWRITEUNABLE
/*  the list of history will be destroyed, whenever is t_history object destroyed. */
#define HF_SELFLIST       LF_SELFLIST


/* history sizes */

/* used for internal using */
#define STANDARD_HISTORYBOX_ITEMS    10
#define STANDARD_HISTORYBUTTON_SIZE  15




/* listbox item structure */

/*
  listbox_item is used as an item for p_list in t_listbox object.
  for more information, how t_list really works, see list.h "t_list" object.
*/

typedef struct t_listbox_item *p_listbox_item;

typedef struct t_listbox_item {

  /* the text, that will be displayed in listbox */
  l_text    name;
  /* pointer to image that will be displayed before texts in listbox */
  BITMAP   *icon;
  /* define if listitem is selected */
  l_bool    sel;
  /* flags for listbox, see LF_XXXX above. */
  l_int     flags;

  /* used for later version */
  l_char    reserved[12];

} t_listbox_item;




/* t_process */

/*
  object t_process is used for drawing process status.
  It's useable in displaying of information about copy status for example.
*/

typedef struct t_process *p_process;

typedef struct t_process {


   /* inherited functions / variabes from t_view and t_object objects */
   struct t_view     obclass;

   /* define current status in promile = 1/10 of per cent */
   l_int             promile;
   /* point to l_dword, where status incresing */
   l_dword          *where;
   /* size is total status, it's when promile == 1000 => (promile/1000)*(*where) == size */
   l_dword           size;

   /* draw current line of process */
   void            (*draw_process) ( p_process o );
   /* move where to (where) and set promile to exact value + redraw the line */
   void            (*rewrite_promile) ( p_process o, l_dword where );

} t_process;

#define PROCESS(o) ((p_process)(o))



/* t_listbox */

/*
  object t_listbox is used for showing diferent items in one box. You can browse
  this items by keyboard or by the mouse. Items are placed in p_list (list) variable.
  for more information about t_list class see (list.h) t_list. items can be also
  selected together by left mouse button and shift press. If you once press shift you
  can select any item from the list. Another time you can deselect it. By this way
  there is an option how to select any item from the list. ( not like in Win, where
  you can select only group ).
*/

typedef struct t_listbox *p_listbox;

typedef struct t_listbox {

  /* inherited functions / variabes from t_scroller, t_view and t_object objects */
  struct t_scroller  obclass;

  /* point to list, that you use. t_listbox can operate by items ( t_listbox_item )
     or inherited from that. new t_listbox_item may create by function new_listbox_item
  */
  p_list  list;

  /*
    the current item that's selected
  */
  l_long  current;

  /*
    from which item you see the box of items. When the listbox can display 10 items
    in the box and you are at 15, this value is 5.
  */
  l_long  from;

  /*
    number of cells in box. You can use 1,> values. When this value is 1, the listbox
    will be able to scroll in (y), otherwise in (x).
  */
  l_int   cells;

  /*
    listbox flags. see above.
  */
  l_int   flags;

  /*
    it's used for defining the status of shifts.
  */
  l_int   shift_flag;

  /*
    define the distance between items.
  */
  t_point between;

  /*
    draw the item (item), and (recitem) is list->at(item), so the item'th
    t_listbox_item. (sel) defines if the item is selected or not. It's used only for
    internal purpose.
  */
  void    (*draw_item_ptr) ( p_listbox o, l_long item, void *recitem, l_bool sel );

  /*
    draw the item (item). (sel) defines if the item is selected or not.
  */
  void    (*draw_item) ( p_listbox o, l_long item, l_bool sel );

  /*
    draw all items in the visible box.
  */
  void    (*draw_box) ( p_listbox o );

  /*
    move the item to ( item ). now the t_listbox.current will contains this value.
    if the (item) is greater then t_listbox.get_max_in_box(...)-t_listbox.from or
    lower than t_listbox.from, this scroll the listbox.
    USE THIS FUNCTION FOR MOVING BETWEEN ITEMS.
  */
  void    (*rewrite_item) ( p_listbox o, l_long item );

  /*
    get the number of items, that can be shown in one visible box.
  */
  l_int   (*get_max_in_box) ( p_listbox o );

  /*
    return number of items from (list). Same as list->get_max_item(list);
  */
  l_long  (*get_max) ( p_listbox o );

  /*
    return number of rows in the listbox. if the t_listbox.cells is 1, this is the
    same as t_listbox.get_max, owtherwise it's the same to
    t_listbox.get_max() / t_listbox.cells
  */
  l_int   (*get_rows) ( p_listbox o );

  /*
    return the with/height of the item. t_listbox.get_item_size().x is the width and
    t_listbox.get_item_size().y is the height of the item.
  */
  t_point (*get_item_size) ( p_listbox o );

  /*
    return the rect, where is (item) placed in the t_lsitbox.
  */
  t_rect  (*get_item_rect) ( p_listbox o, l_long item );

  /*
    return (item), where mouse is over it. (where) is the local point of mouse->where.
  */
  l_long  (*get_pos_from_xy) ( p_listbox o, t_point where );

  /*
    return the list of selected/unselected items. (sel == true ) returns the list of
    selected items, (sel == false) returns the list of deselected items. After using
    the returned list you should free it by function ( dispose_list(...) see list.h )

  */
  p_list  (*get_selected_items) ( p_listbox o, l_bool sel );

  /*
    set the list and all set to initial values + redraw the box of items. If you are
    going to set the new list, you should free the old one by using function
    dispose_list(...) as :

    dispose_list(t_listbox.list, true);

  */
  l_bool  (*set_list) ( p_listbox o, p_list p );

} t_listbox;

#define LISTBOX(o) ((p_listbox)(o))

/* get text for (i)th listbox item */
#define LISTBOX_ITEM_TEXT(o, i) ((o)?((p_listbox_item)(o)->at((o), i))->name:NULL)
/* get icon for (i)th listbox item */
#define LISTBOX_ITEM_ICON(o, i) ((o)?((p_listbox_item)(o)->at((o), i))->icon:NULL)
/* get selected flag for (i)th listbox item */
#define LISTBOX_ITEM_SEL(o, i)  ((o)?((p_listbox_item)(o)->at((o), i))->sel:0)
/* get flags for (i)th listbox item */
#define LISTBOX_ITEM_FLAGS(o, i)((o)?((p_listbox_item)(o)->at((o), i))->flags:0)
/* get (i)th pointer to listbox item */
#define LISTBOX_ITEM_AT(o, i)   ((o)?(o)->at((o), i):NULL)



/* t_textline */

/*
  object t_textline is used for text manipulating in one line.
*/
typedef struct t_textline *p_textline;

typedef struct t_textline {

  /* inherited functions / variabes from t_view and t_object objects */
  struct t_view  obclass;

  /* flags for the textline, see TF_XXXX above */
  l_int          flags;

  /* current text of the t_textline */
  l_text         text;

  /* current position of the cursor. it point to character of the text, that is
     highlighted by the cursor
  */
  l_int          pos;

  /*
    from which char you see the line of chars. When the textline can display 10 chars
    in the box and you are at 15, this value is 5.
  */
  l_int          line;

  /*
    maximum number of characters, that text may contains.
  */
  l_int          limit;

  /*
    from which character is text selected. if the value is < 0, it's no selected text
  */
  l_int          sel_from;

  /*
    to which character is text selected. if the value is < 0, it's no selected text
  */
  l_int          sel_to;

  /* for internal use. the first time where we start to select the text */
  l_int          sel_first_from;
  l_int          sel_first_to;

  /* indicates if the text is selected or not */
  l_int          sel_ok;

  /* if true, cursor is visible, othewise was hiden */
  l_bool         cursor_visible;

  /* insert text (text) into the position (pos) of the t_textline.text */
  void          (*ins_text)( p_textline o, l_int pos, l_text text );

  /* insert character (chr) into the position (pos) of the t_textline.text */
  void          (*ins_char)( p_textline o, l_int pos, l_char chr );

  /* delete number of chars (size) from the position (pos) of the t_textline.text */
  void          (*del_text)( p_textline o, l_int pos, l_int size );

  /* delete one char from the position (pos) of the t_textline.text */
  void          (*del_char)( p_textline o, l_int pos );

  /* draw cursor at the position pos and from the first visible character in the
     box defined by (line). if (active) is true it cursor show, otherwise it hide
  */
  void          (*draw_cursor_ex)( p_textline o, l_int line, l_int pos, l_int active );

  /* redraw cursor from the old (oldline) and (oldpos) to new one position defined
     by (t_textline.line) and (t_textline.pos).
  */
  void          (*draw_cursor)( p_textline o, l_int oldline, l_int oldpos );

  /*
    show or hide the cursor. when (show) is true it show the cursor, otherwise it hide
    one.
  */
  void          (*show_cursor)( p_textline o, l_bool show );

  /*
    draw the text of the textline.
  */
  void          (*draw_text)( p_textline o );

  /*
    move the current cursor to the (newpos) of text and insert the character
    (keycode) to the (newpos). if the keycode is ZERO, it only move the cursor.
    you can use TO_KEY(KB_DEL), TO_KEY(KB_BACKSPACE), ....
  */
  void          (*redraw_text) ( p_textline o, l_int newpos, l_int keycode );

  /*
    selects or deselects the whole text (t_textline.text). if (selfrom) and (selfto) are
    lower than ZERO, it deselects the text, otherwise in the case of real values it
    selects one.
  */
  void          (*sel_all) ( p_textline o, l_int selfrom, l_int selto );

  /* get number of caharcters to the first/end of text line box. if the (plus) is -1,
     it counts from char (from) to first visible char in textline. if the (plus) is +1,
     it counts from char (from) to last visible char in textline
  */
  l_int         (*charsin) ( p_textline o, l_int from, l_int plus );

  /* same as charsin, but it not use width of textline box, but (size) as width.
  */
  l_int         (*charsin_size) ( p_textline o, l_int size, l_int from, l_int plus );

  /*
    return (char), where mouse is over it. (p) is the local point of mouse->where.
  */
  l_int         (*get_pos_from_xy) ( p_textline o, t_point p );

  /*
    set text to newone (text). this function allocate the new memory for the new text
    and free the old one. it also set all to initial values.
  */
  l_bool        (*set_text) ( p_textline o, l_text text );

} t_textline;

#define TEXTLINE(o) ((p_textline)(o))

/* clear textline text-selection */
#define sel_clear(o) (o)->sel_from = (o)->sel_to = (o)->sel_first_from = (o)->sel_first_to = (o)->sel_ok = 0
/* return true if text is rewrite able, otherwise it returns false */
#define is_wable(o)  (!((o)->flags & TF_REWRITEUNABLE))
/* return true if text is selected */
#define is_sel(o)    ((o)->sel_from != (o)->sel_to)


/* t_history */

/*
  object t_history is used as combination of t_listbox and t_textline.
  it shows the t_textline with button, that shows the listbox when it's pressed.
*/
typedef struct t_history *p_history;

typedef struct t_history {

  /* inherited functions / variabes from t_textline, t_view and t_object objects */
  struct t_textline  obclass;

  /* current position of the selected item in the list */
  l_long         current;

  /* list of items, see t_listbox_item. you can add to the list by function
     new_history_item, what is same to new_listbox_item for now.
  */
  p_list         list;

  /* list box, that's showed whenever is button pressed
  */
  p_listbox      listbox;

  /* button that show the listbox
  */
  p_button       button;


  l_dword        message;

  /*
    execute process in selected listbox. the process is halt, whenever mouse is pressed
    outside from the listbox or the ESC is pressed. in this case it returns MSG_CANCEL.
    if the item is choosed or ENTER is pressed it returns MSG_OK.
  */
  l_dword       (*listbox_execute) ( p_object o );

  /*
    show listbox box and run listbox_execute. it also redraw the selected item and set
    t_textline (TEXTLINE(&t_history)->text) to text of selected item. return the same values as
    the listbox_execute.
  */
  l_dword       (*show_box) ( p_history o );

  /*
    move the item of the t_listbox->current to ( item ) and set text
    (TEXTLINE(&t_history)->text) to the current item.
    USE THIS FUNCTION FOR MOVING BETWEEN ITEMS.
  */
  void          (*rewrite_item) ( p_history o, l_long item );

  /*
    set the list and all set to initial values and redraw the textline. If you are
    going to set the new list, you should free the old one by using function
    dispose_list(...) as :

    dispose_list(t_listbox.list, true);

  */
  l_bool        (*set_list) ( p_history o, p_list p );

} t_history;

#define HISTORY(o) ((p_history)(o))



/* t_stattext */


/*
  object t_stattext is used drawing formated text and placed it to the other object.
*/
/*typedef struct t_stattext *p_stattext;

typedef struct t_stattext {


  struct t_view  obclass;


  l_char         text[256];


  l_int          align;

} t_stattext;

#define STATTEXT(o) ((p_stattext)(o))

*/

/* t_process */

/*
  create object t_process to area (r). (size) is the end of (*where).
  For example you can use this argument for size of file.
  (where) is pointer from 0 to (size). This may be position in file as is copying.

  example :
  ---------

  l_dword size = 100000;
  l_dword pos  = 0;

  p_process x = process_init(_malloc(sizeof(t_process)), r, size, &pos);

  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(x));

  _while ( pos++ < size );
*/
extern p_process  (*process_init) ( p_process o, t_rect r, l_dword size, l_dword *where );
p_process   _process_init ( p_process o, t_rect r, l_dword size, l_dword *where );

void   process_translate_event ( p_object o, t_event *event );
void   process_func_callback ( p_object o );
void   process_draw ( p_view o );
t_rect process_size_limits ( p_view o );

void   process_rewrite_promile ( p_process o, l_dword where );
void   process_draw_process ( p_process o );



/* t_history */

/*

 create new history object. (r) is rectangular area of the object,
 (list) is list of pulldown box. (limit) is a limit of maximum characters
 that can be write in case of writeable history.
 (flags) may be one or  combination HF_XXXXX ( see above )

  example :
  ---------

  p_list get_list ( void )
  {

      p_list list = list_init(_malloc(sizeof(t_list)), &free_history_item, DAT_LIST);

      list's items->rec will be freed by function free_history_item

      if ( list ) {

         list->insert(list, new_listbox_item("Name", NULL, true, LIF_NONE));
         list->insert(list, new_listbox_item("New Name", NULL, true, LIF_NONE));

      };

      return list;

   };

   t_rect r = rect_assign(100, 100, 120, 200);

   p_history his = history_init(_malloc(sizeof(t_history)), r,
                                get_list(),
				                    100,   max number of chars in line
					                 HF_REWRITEUNABLE);  readonly !

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(his));

   r = rect_assign(100, 130, 200, 150);

   his = history_init(_malloc(sizeof(t_history)), r,
                      get_list(),
				          60,   max number of chars in line
					       HF_NONE);  read/write

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(his));
*/
extern p_history   (*history_init) ( p_history o, t_rect r, p_list list, l_int limit, l_int flags );
p_history   _history_init ( p_history o, t_rect r, p_list list, l_int limit, l_int flags );

void     history_translate_event ( p_object o, t_event *event );
void     history_setup ( p_object o );
void     history_change_bounds ( p_view o, t_rect nr );

l_dword  history_listbox_execute ( p_object o );
l_dword  history_show_box ( p_history o );
void     history_rewrite_item ( p_history o, l_long item );
l_bool   history_set_list ( p_history o, p_list p );

#define          new_history_item   new_listbox_item
#define          free_history_item  free_listbox_item



/* listbox item functions */

/* t_listbox */

/*
 create new object t_listbox, where (r) is area of listbox, where will be placed,
 (list) is list of items that will be shown, (rows) is number of rows.
 when <=1, it will scroll down and up, otherwise left and right.
 (flags) can be combination of LF_XXXXX (see above) :

 p_list must contains compatible record items with t_listbox_item

 typedef struct t_listbox_item {

      l_text    name;
      BITMAP   *icon;
      l_bool    sel;
      l_int     flags;

      l_char    reserved[12];

 } t_listbox_item;
*/
extern p_listbox   (*listbox_init) ( p_listbox o, t_rect r, p_list list, l_byte rows, l_int flags );
p_listbox  _listbox_init ( p_listbox o, t_rect r, p_list list, l_byte rows, l_int flags );


/*
 make new item. insert name (name) by function _strdup, icon (icon),
 sel if it's selected, (flags) - combination of LIF_XXXX ( see above ).

   example :
   ---------


   p_list get_list ( void )
   {

      p_list list = list_init(_malloc(sizeof(t_list)), &free_listbox_item, DAT_LIST);

      list's items->rec will be freed by function free_listbox_item

      if ( list ) {

         list->insert(list, new_listbox_item("Name", NULL, true, LIF_NONE));
         list->insert(list, new_listbox_item("New Name", NULL, true, LIF_NONE));

      };

      return list;

   };

   t_rect r = rect_assign(100, 100, 200, 200);

   p_listbox box = listbox_init(_malloc(sizeof(t_listbox)), r, get_list(), 1, 0);

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(box));
*/
p_listbox_item   new_listbox_item ( l_text name, BITMAP *icon, l_bool sel, l_int flags );
void             free_listbox_item ( void* t );

void   listbox_translate_event ( p_object o, t_event *event );
void   listbox_set_state ( p_object o, l_dword st, l_bool set );
void   listbox_set_options ( p_object o, l_dword op, l_bool set );
l_bool listbox_done ( p_object o );
l_bool listbox_get_data ( p_object o, t_data *rec );
l_bool listbox_set_data ( p_object o, t_data *rec );
void   listbox_draw ( p_view o );
t_rect listbox_size_limits ( p_view o );
void   listbox_scroll_size ( p_scroller o, l_long *x, l_long *y );
t_rect listbox_scroll_limits ( p_scroller o );
void   listbox_recalc_positions ( p_scroller o, l_long x, l_long y );
l_bool listbox_set_list ( p_listbox o, p_list p );

l_long  listbox_get_pos_from_xy ( p_listbox o, t_point where );
void    listbox_draw_item_ptr ( p_listbox o, l_long item, void *recitem, l_bool set );
void    listbox_draw_item ( p_listbox o, l_long item, l_bool set );
void    listbox_draw_box ( p_listbox o );
void    listbox_rewrite_item ( p_listbox o, l_long item );
l_int   listbox_get_max_in_box ( p_listbox o );
l_long  listbox_get_max ( p_listbox o );
l_int   listbox_get_rows ( p_listbox o );
t_point listbox_get_item_size ( p_listbox o );
t_rect  listbox_get_item_rect ( p_listbox o, l_long item );
p_list  listbox_get_selected_items ( p_listbox o, l_bool sel );



/* t_worklistbox */

extern p_listbox   (*worklistbox_init) ( p_listbox o, t_rect r, p_list list, l_byte cells, l_int flags );
p_listbox  _worklistbox_init ( p_listbox o, t_rect r, p_list list, l_byte cells, l_int flags );


t_rect worklistbox_size_limits ( p_view o );
void   worklistbox_draw ( p_view o );



/* t_textline */

/*
  create new textline to rect (r).
  (limit) is maximum number of characters, that can be displayed in the
  current textline. (flags) are settings for the textline,
  you can use as combination of the TF_XXXX ( see above ).

  example :
  ---------

  t_rect r = rect_assign(100, 100, 300, 120);

  p_textline o = textline_init ( _malloc(sizeof(t_textline)),
					                  r,
					                  100, limit of text
				    	               0);  rewrite able

  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(o));

  o->set_text("Hello world");  set text to "Hello world"

  if ( !strcmp(o->text, "Hello world") )  text is "Hello world" ?

	      o->set_text("Hello Mr. Michal Stencl");
*/
p_textline  _textline_init ( p_textline o, t_rect r, l_int limit, l_int flags );
extern p_textline  (*textline_init) ( p_textline o, t_rect r, l_int limit, l_int flags );


void   textline_ins_text ( p_textline o, l_int pos, l_text text );
void   textline_ins_char ( p_textline o, l_int pos, l_char chr );
void   textline_del_text ( p_textline o, l_int pos, l_int size );
void   textline_del_char ( p_textline o, l_int pos );
l_bool textline_done ( p_object o );
void   textline_set_state ( p_object o, l_dword st, l_bool set );
void   textline_set_options ( p_object o, l_dword op, l_bool set );
void   textline_draw ( p_view o );
l_dword  textline_execute ( p_object o );
void   textline_translate_event ( p_object o, t_event *event );
t_rect textline_size_limits ( p_view o );
void   textline_draw_cursor_ex ( p_textline o, l_int line, l_int pos, l_int active );
void   textline_draw_cursor ( p_textline o, l_int oldline, l_int oldpos );
void   textline_show_cursor ( p_textline o, l_bool show );
void   textline_draw_text ( p_textline o );
void   textline_redraw_text ( p_textline o, l_int newpos, l_int keycode );
void   textline_sel_all ( p_textline o, l_int selfrom, l_int selto );
l_int  textline_charsin ( p_textline o, l_int from, l_int plus );
l_int  textline_charsin_size ( p_textline o, l_int size, l_int from, l_int plus );
l_int  textline_get_pos_from_xy ( p_textline o, t_point p );

l_bool textline_select_data ( p_object o, l_int data_style, l_bool set );
l_bool textline_get_data ( p_object o, t_data *rec );
l_bool textline_set_data ( p_object o, t_data *rec );
l_bool textline_set_text ( p_textline o, l_text text );
l_bool textline_drag_where ( p_view o, t_data *rec, t_point where );





p_textline   _dyntext_init ( p_textline o, t_rect r, l_int limit );
extern p_textline  (*dyntext_init) ( p_textline o, t_rect r, l_int limit );

void   dyntext_draw ( p_view o );


/* t_workline */

/*
 create new textline, that is not pushed down, but only arrounded
 by the black rectangle.
*/

p_textline   _worktextline_init ( p_textline o, t_rect r, l_int limit );
extern p_textline  (*worktextline_init) ( p_textline o, t_rect r, l_int limit );

t_rect worktextline_size_limits ( p_view o );
void   worktextline_draw ( p_view o );

p_menuitem get_edit_menuitem ( p_object o, l_bool selection );
void show_edit_menu ( p_object o, l_bool selection );

void   listbox_change_bounds ( p_view o, t_rect nr );

#endif

