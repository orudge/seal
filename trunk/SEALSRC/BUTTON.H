/****************************************************************/
/*                                                              */
/*                          button.h                            */
/*                                                              */
/*                Seal - free desktop environment               */
/*                                                              */
/*                  Copyright (c) 1999,2000                     */
/*                       Michal Stencl                          */
/*                    All Rights Reserved                       */
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



#ifndef __BUTTON_H_INCLUDED__
#define __BUTTON_H_INCLUDED__

typedef struct t_button    *p_button;
typedef struct t_icon      *p_icon;
typedef struct t_icobutton *p_icobutton;

/*

   palette of button object :
   --------------------------

   standard colors from Seal.ini file, from section [colors] :

   index of palette        name in [colors]

   0                       3D_background
   1                       button_active_caption
   2                       button_pasive_caption

*/

extern l_color  pal_button[];

/* button flags, can use combination of followings */

/*
 normal button
*/
#define BF_NORMAL        0x00

/*
 button is arrounded by black rectangle and when enter is pressed, it calls the
 message of this button
*/
#define BF_DEFAULT       0x01

/*
 button is pushed. it means it has no 3D effect. If button is pressed then you will
 see 3D effect of pushed window. Good for icons.
*/
#define BF_PUSH          0x02

/*
 button is double arrounded by rectangle. only graphics efect.
*/
#define BF_DARROUND      0x04

/*
 not supported yet...
*/
#define BF_UNDERSEL      0x08  /* button is selected whenever is mouse under it */




/* icon flags...used for t_icon.flags. */

/*
 icon' image will be destroyed, whenever is object (icon) destroyed.
 If you use system or any other global icon, this flags CAN'T be used.
*/
#define IF_SELFIMAGE     0x80

/*
 icon' image will be stretched to the size of the icon object,
 otherwise it's centered.
*/
#define IF_STRETCH       0x100



/* icon button flags */

/* Warning ! Please do not change the IF_SELFIMAGE with IBF_SELFIMAGE. As you can see,
   I set the values at the same. The efect is the same.
*/
#define IBF_SELFIMAGE    0x80


/* t_icon */

/*
  t_icon class is used for showing icons. you are able to drag icon on one place and
  drop it to the other, because of t_view.drag_mode & DM_DRAGMOVE settings. If you
  don't want to use this settings, set t_view.drag_mode to ZERO after initialization.
*/

typedef struct t_icon {

  /* inherited functions / variabes from t_view and t_object objects */
  struct t_view    obclass;

  /*
     pointer to BITMAP, where's icon is placed. if (flags) is set to IF_SELFIMAGE,
     this BITMAP will be destroyed, whenever is object destroyed.
     see above (IF_SELFIMAGE flag )
  */
  BITMAP   *icon;

  /*
    contains message of icon. When icon is doubleclicked by the mouse, it sets event'
    type to EV_MESSAGE, message of event ( t_event.message ) to this "message" value.
  */
  l_dword   message;

  /*
    flags for the icon object. may be combination of the followings :

    IF_SELFIMAGE
    IF_STRETCH

    ....see above
  */
  l_int     flags;

} t_icon;

#define ICON(o)   ((p_icon)(o))


/*
  t_button class is used for standard button displaying and control. each button is
  defined by the caption ( name of button ), flags and message. "message" is the value
  what comes from the button, whenever button is pressed by the left mouse button or
  enter if the button is selected. if "flags" of button is set to BF_DEFAULT, message
  is called also when the ENTER is pressed somewhere in the selected window.
*/

/* t_button */
typedef struct t_button {

  /* inherited functions / variabes from t_view and t_object objects */
  struct t_view    obclass;

  /*
    button flags. see BF_XXXX above.
  */
  l_int     flags;

  /*
    caption of the button.
  */
  l_text    caption;

  /*
    contains message of the button. When button is pressed by the mouse or ENTER, it
    sets event' type to EV_MESSAGE, message of event ( t_event.message ) to this
    "message" value.
  */
  l_dword   message;

  /*
    draw state of the button. if (press) is true, it draws pressed button, otherwise
    normal defined by BF_XXXXX flags.
  */
  void     (*draw_state) ( p_button o, l_int press );

  /*
    test if button is set to BF_DEFAULT mode.
  */
  l_bool   (*is_default) ( p_button o );

} t_button;

#define BUTTON(o)   ((p_button)(o))


/*
  t_icobutton class is used for same things as t_button class, but not shows the
  caption, but it shows the icon. if the t_button.flags is set to IBF_SELFIMAGE,
  BITMAP of the icon is destroyed whenever is t_icobutton destroyed.
*/
/* t_icobutton */

typedef struct t_icobutton {

  /* inherited functions / variabes from t_view and t_object objects */
  struct t_button  obclass;

  /*
    contains pointer to image of the icon.
  */
  BITMAP   *icon;

} t_icobutton;

#define ICOBUTTON(o)   ((p_icobutton)(o))


/* icon's functions */

/*
 an equivalent to "l_bool t_object.done ( t_object * )" function. this function is
 inherited from the "l_bool view_done ( t_object * )" function. if the IF_SELFIMAGE
 is set for t_icon.flags, this destroy the t_icon.icon image, otherwise only destroy
 object.
*/
l_bool icon_done ( p_object o );

/*
 an equivalent to "void  t_object.set_state ( p_object o, l_dword st, l_bool set )"
 function. this function is inherited from the
 "void view_set_state ( p_object o, l_dword st, l_bool set )" function.
 if (st) is OB_SF_SELECTED ( object is selected ), this automatically redraw object
 to selected or deselected form.
*/
void   icon_set_state ( p_object o, l_dword st, l_bool set );

/*
 an equivalent to "void  t_object.translate_event ( p_object o, t_event *event )"
 function. this function is inherited from the
 "void view_translate_event ( p_object o, t_event *event )" function.
 whenever event comes from the mouse and this event signals, that's left mouse button
 was doublepressed, it sets event to t_icon.message. or if icon is drag by the left
 mouse button, it calls function t_view.drag_view, that control dragging of the
 view.
*/
void   icon_translate_event ( p_object o, t_event *event );


/*
 an equivalent to "void  t_view.draw ( p_view )" function. this function draw t_icon
 object. ...see t_view.draw ( t_view *) for more details.
*/
void   icon_draw ( p_view o );

/*
  this function make an new t_icon object. object for icon showing or moving.
  (r) is the rect of the icon, where will be placed. (icon) is the pointer to
  image of the icon. If you want to destory this icon,
  whenever is t_icon object destroyed you, set flags to IF_SELFIMAGE. (message) is
  the message of icon that will be called even when the icon is pressed or
  doubleclicked.
*/
p_icon  _icon_init ( p_icon o, t_rect r, BITMAP *icon, l_dword message, l_int flags );
extern p_icon  (*icon_init) ( p_icon o, t_rect r, BITMAP *icon, l_dword message, l_int flags );



/* button's functions */
/*
 an equivalent to "l_bool t_object.done ( t_object * )" function. this function is
 inherited from the "l_bool view_done ( t_object * )" function. Function free the
 memory of t_button.caption and destroy object.
*/
l_bool button_done ( p_object o );

/*
 an equivalent to "void  t_object.set_state ( p_object o, l_dword st, l_bool set )"
 function. this function is inherited from the
 "void view_set_state ( p_object o, l_dword st, l_bool set )" function.
 if (st) is OB_SF_SELECTED ( object is selected ), this automatically redraw object
 to selected or deselected form.
*/
void   button_set_state ( p_object o, l_dword st, l_bool set );

/*
 an equivalent to "void  t_object.set_options ( p_object o, l_dword op, l_bool set )"
 function. this function is inherited from the
 "void obj_set_options ( p_object o, l_dword op, l_bool set )" function.
 if (st) is OB_OF_ENABLE ( object is enable/disable ), this automatically redraw object
 to enable or desable form.
*/
void   button_set_options ( p_object o, l_dword op, l_bool set );

/*
 an equivalent to "void  t_object.translate_event ( p_object o, t_event *event )"
 function. this function is inherited from the
 "void view_translate_event ( p_object o, t_event *event )" function.
 whenever event comes from the mouse and this event signals, that's left mouse button
 was pressed or enter was pressed, it sets event to t_button.message.
*/
void   button_translate_event ( p_object o, t_event *event );

/*
 an equivalent to "void  t_view.draw ( p_view )" function. this function draw t_button
 object. ...see t_view.draw ( t_view *) for more details. calls t_button.draw_state
 and sets 2nd argument ( l_int press ) to false mode.
*/
void   button_draw ( p_view o );

/*
 draw state of the button. if (press) is true, it draws pressed button, otherwise
 normal defined by BF_XXXXX flags.
*/
void   button_draw_state ( p_button o, l_int press );

/*
 test if button is set to BF_DEFAULT mode.
*/
l_bool button_is_default ( p_button o );


/*
 create t_button object in area defined by (r), by title (caption) by message
 (message) and by combination of supported flags :

 'BF_NORMAL'      - normal button
 'BF_DEFAULT'     - normal button, but when enter in other object in
 			           group is pressed it call message (message). In
          			  normal mode it call message only if button is focused.
 'BF_PUSH'        - button can't be selected. For example close button
			           of window is set to this flag.
 'BF_DARROUND'    - button is double arounded
 'BF_UNDERSEL'    - not supported....

 ...please do not allocate new memory for the caption. it's automatically
 allocated by this function.
*/

p_button  _button_init ( p_button o, t_rect r, l_text caption, l_dword message, l_int flags );
extern p_button  (*button_init) ( p_button o, t_rect r, l_text caption, l_dword message, l_int flags );


/* icobutton's functions*/

/*
 an equivalent to "l_bool t_object.done ( t_object * )" function. this function is
 inherited from the "l_bool view_done ( t_object * )" function. if the IBF_SELFIMAGE
 is set for t_button.flags, this destroy the t_icobutton.icon image,
 otherwise only destroy object.
*/
l_bool icobutton_done ( p_object o );

/*
 draw state of the button. if (press) is true, it draws pressed button, otherwise
 normal defined by BF_XXXXX flags.
*/
void   icobutton_draw_state ( p_button o, l_int press );

/*
  create new t_icobutton class. it's used for the same thing as t_button class,
  but not shows the caption, but it shows the icon. if the t_button.flags is set
  to IBF_SELFIMAGE, BITMAP of the icon is destroyed whenever is t_icobutton destroyed.

  if you use "caption", this caption is used as info banner, that will be draw whenever
  mouse is under object and Ctrl+F1 is pressed.
*/
p_icobutton _icobutton_init ( p_icobutton o, t_rect r, BITMAP *icon, l_text caption, l_dword message, l_int flags );
extern p_icobutton (*icobutton_init) ( p_icobutton o, t_rect r, BITMAP *icon, l_text caption, l_dword message, l_int flags );


#endif

