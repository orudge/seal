/****************************************************************/
/*                                                              */
/*                            app.c                             */
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
#include"app.h"
#include"button.h"
#include"dialogs.h"

static DATAFILE *datfile = NULL;

t_appinfo appinfo; /* for transfers */

l_color  pal_appwin[] = {CO_SYSx20, CO_RED, CO_BLACK, CO_SYSx19, CO_BLACK, CO_WHITE, CO_SYSx21, CO_NOCOLOR};


p_appwin  (*appwin_init) ( p_appwin o, t_rect r, l_text caption, l_int flags, hdlx_t id,
                           void (*trans_ev)( p_object o, t_event *e )) = &_appwin_init;
p_appwin  (*window_init) ( p_appwin o, t_rect r, l_text caption, l_int flags );

p_appwin  (*message_init_ex) ( p_object owner, p_appwin o, t_rect r, l_text caption, l_int flags,
                               l_char    iflags,
                               l_dword  bflags, void (*trans_ev)( p_object o, t_event *e ),
                               l_text in_text, va_list argp ) = &_message_init_ex;

p_window   (*frame_init) ( p_window o, t_rect r ) = &_frame_init;



/* appwin functions */

t_rect  appwin_size_limits ( p_view o )
{
  return win_size_limits(o);
};


void  appwin_setup ( p_object o )
{
  p_button b;
  t_rect   r;
  t_rect   s;

  view_setup(o);

  r = VIEW(o)->get_local_extent(VIEW(o));
  s = VIEW(o)->size_limits(VIEW(o));

  r = rect_assign(r.b.x-21, r.a.y+2, r.b.x-1, s.a.y-1);

  b = button_init(_malloc(sizeof(t_button)), r, "5", MSG_CLOSE, BF_PUSH+BF_NORMAL);

  if ( b ) { /* make close button */

    /* ignore size limits of window */
    OBJECT(b)->set_options(OBJECT(b), VW_OF_IGNORELIM, true);
    /* can't be selected */
    OBJECT(b)->set_options(OBJECT(b), OB_OF_SELECTABLE, false);
    /* fot is symbol TTF font in size 10,10 */
    VIEW(b)->font = get_font_in_size("Symbols", 10, 10);
    /* move button, when window is growing, but still fixed size */
    VIEW(b)->align |= TX_ALIGN_RIGHT+TX_ALIGN_FIXEDX;

  };
  /* insert close button to window */
  o->insert(o, OBJECT(b));

  if ( WINDOW(o)->flags & WF_MAXIMIZE ) {

    /* maximize button */

    r = rect_move(r, -21, 0);
    b = button_init(_malloc(sizeof(t_button)), r, "0", MSG_MAXIMIZE, BF_PUSH+BF_NORMAL);

    if ( b ) {

         /* ignore size limits of window */
         OBJECT(b)->set_options(OBJECT(b), VW_OF_IGNORELIM, true);
         /* can't be selected */
         OBJECT(b)->set_options(OBJECT(b), OB_OF_SELECTABLE, false);
         /* fot is symbol TTF font in size 10,10 */
         VIEW(b)->font = get_font_in_size("Symbols", 10, 10);
         /* move button, when window is growing, but still fixed size */
         VIEW(b)->align |= TX_ALIGN_RIGHT+TX_ALIGN_FIXEDX;

    };

    /* insert maximize button to window */
    o->insert(o, OBJECT(b));

  };

  if ( WINDOW(o)->flags & WF_MINIMIZE ) {

    /* minimize button */
    r = rect_move(r, -21, 0);
    b = button_init(_malloc(sizeof(t_button)), r, "1", MSG_MINIMIZE, BF_PUSH+BF_NORMAL);

    if ( b ) {

         /* ignore size limits of window */
         OBJECT(b)->set_options(OBJECT(b), VW_OF_IGNORELIM, true);
         /* can't be selected */
         OBJECT(b)->set_options(OBJECT(b), OB_OF_SELECTABLE, false);
         /* fot is symbol TTF font in size 10,10 */
         VIEW(b)->font = get_font_in_size("Symbols", 10, 10);
         /* move button, when window is growing, but still fixed size */
         VIEW(b)->align |= TX_ALIGN_RIGHT+TX_ALIGN_FIXEDX;

    };

    /* insert minimize button to window */
    o->insert(o, OBJECT(b));

  };

};


void  appwin_draw_title ( p_window o )
{

  t_rect  r = rect_assign(2, 2, rect_sizex(VIEW(o)->bounds)-2,
                          FONT_GETHEIGHT(VIEW(o)->font)+3);
  t_rect  safe = r;
  t_rect  t;
  t_point p;

  l_int dbx = (21*((o->flags & WF_MAXIMIZE)?1:0)) + (21*((o->flags & WF_MINIMIZE)?1:0));

  BITMAP *out = VIEW(o)->begin_paint(VIEW(o), &p, safe);

  if ( out ) {

    l_int   dy = (rect_sizey(r) - FONT_GETHEIGHT(VIEW(o)->font)) / 2;
    l_bool  ok = false;
    l_text  caption = set_format_text(NULL, "  %s  ", o->caption);

    l_color fcolor = VIEW(o)->get_color(VIEW(o), 6);
    l_color bcolor1 = VIEW(o)->get_color(VIEW(o), 3);
    l_color bcolor2 = VIEW(o)->get_color(VIEW(o), 4);

    if ( OBJECT(o)->state & OB_SF_SELECTED ) {

      fcolor = VIEW(o)->get_color(VIEW(o), 5);
      bcolor1 = VIEW(o)->get_color(VIEW(o), 1);
      bcolor2 = VIEW(o)->get_color(VIEW(o), 2);

    };

    fade_rect(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x-dbx, r.b.y+p.y, bcolor1, bcolor2, FR_HOR);

    if ( bcolor1 != bcolor2 )

       ok = false;

    else if ( OBJECT(o)->state & OB_SF_SELECTED ) {

      lined_rect(out, r.a.x+p.x+5, r.a.y+p.y+dy+3, r.b.x+p.x-dbx-5, r.a.y+p.y+dy+FONT_GETHEIGHT(VIEW(o)->font), COLOR(CO_BLACK), COLOR(CO_WHITE));

      ok = true;

    };

    t = rect_assign(r.b.x+p.x-dbx, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y);

    VIEW(o)->background(VIEW(o), out, t);

    textout_draw_rect(out, VIEW(o)->font, caption, -1, r.a.x+p.x, r.a.y+p.y+1,
                           r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTER, fcolor, ok?bcolor1:TX_NOCOLOR, 0);

    _free(caption);

  };

  VIEW(o)->end_of_paint(VIEW(o), safe);

};


void      appwin_translate_event ( p_object o, t_event *event )
{

  RETVIEW(o, event);

  if ( APPWIN(o)->app_trans_ev ) {

    o->phase = PH_PREPROCESS;

    APPWIN(o)->app_trans_ev(o, event);

    o->phase = 0;

  };

  if ( event->type & EV_MESSAGE ) { /* message event */

    switch ( event->message ) {

      case MSG_MAXIMIZE : { /* set info about maximalizing of this object */

        clear_type(&appinfo, sizeof(t_appinfo));

        appinfo.title = WINDOW(o)->caption;
        appinfo.obj = o;

        set_event_info(event, EV_INFO, MSG_MAXIMIZE, o, &appinfo);

        o->put_event(o, event);

        clear_event(event);

      }; break;

      case MSG_MINIMIZE : {  /* set info about minimalizing of this object */

        clear_type(&appinfo, sizeof(t_appinfo));

        appinfo.title = WINDOW(o)->caption;
        appinfo.obj = o;

        set_event_info(event, EV_INFO, MSG_MINIMIZE, o, &appinfo);

        o->put_event(o, event);

        clear_event(event);

      }; break;

      case MSG_RESIZE : {

        clear_event(event);

      }; break;

      case MSG_OK :
      case MSG_CLOSE : {

        if ( !o->is_state(o, OB_SF_MODAL) ) {

          clear_event(event);

          APPWIN(o)->app_trans_ev = NULL;

          if ( APPWIN(o)->app_id )

             DLXUnload(APPWIN(o)->app_id); /* function for unloading application */

          dispose(o);

        } else o->end_state = event->message;

        clear_event(event);

      }; break;

    };
  };

  win_translate_event(o, event); /* old translate_event function */


  if ( APPWIN(o)->app_trans_ev ) {

    o->phase = PH_POSTPROCESS;

    APPWIN(o)->app_trans_ev(o, event);

    o->phase = 0;

  };
};



/* appwin */

p_appwin  _appwin_init ( p_appwin o, t_rect r, l_text caption, l_int flags, hdlx_t id,
                         void (*trans_ev)( p_object o, t_event *e ))
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_appwin));

  win_init(&o->obclass, r, caption, flags);

  /* object's functions */

  OBJECT(o)->translate_event = &appwin_translate_event;
  OBJECT(o)->setup = &appwin_setup;


  /* view's functions */

  VIEW(o)->size_limits = &appwin_size_limits;


  /* windows's functions */

  WINDOW(o)->draw_title = &appwin_draw_title;


  /* function calling */

  VIEW(o)->set_palette(VIEW(o), pal_appwin);

  VIEW(o)->brush.color = VIEW(o)->get_color(VIEW(o), 0);

  /* appwin's declarations */

  o->app_trans_ev = trans_ev;
  o->app_id = id;

  return o;
};


/* window */

p_appwin  _window_init ( p_appwin o, t_rect r, l_text caption, l_int flags )
{
   return appwin_init(o, r, caption, flags, 0, NULL);
};



#define MSG_FLAGS_SETUP                            \
    switch ( wflags ) {                            \
      case MW_WARNING : {                          \
        iflag = MI_WARNING;                        \
        capt = TXT_WARNING;                        \
      }; break;                                    \
      case MW_INFO    : {                          \
        iflag = MI_INFO;                           \
        capt = TXT_INFO;                           \
      }; break;                                    \
      case MW_ERROR   : {                          \
        iflag = MI_ERROR;                          \
        capt = TXT_ERROR;                          \
      }; break;                                    \
      case MW_QUESTION : {                         \
        iflag = MI_QUESTION;                       \
        capt = TXT_QUESTION;                       \
      }; break;                                    \
      case MW_UNAVAILABLE : {                      \
        iflag = MI_UNAVAILABLE;                    \
        capt = TXT_UNAVAILABLE;                    \
      }; break;                                    \
      case MW_SETUP : {                            \
        iflag = MI_SETUP;                          \
        capt = TXT_SETUP;                          \
      }; break;                                    \
    }


/* message window init function */

p_appwin  _message_init_ex ( p_object owner, p_appwin o, t_rect r, l_text caption, l_int flags,
                             l_char   iflag,
                             l_dword  bflags, void (*trans_ev)( p_object o, t_event *e ),
                             l_text in_text, va_list argp )
{

  t_rect t;
  t_rect ric;
  t_rect sf = r;
  p_stattext st   = NULL;
  p_icon     icn  = NULL;
  BITMAP    *img  = NULL;
  l_int      n  = 2;
  l_int      xf = 0;
  l_dword    i  = 0;
  l_int      bn = 0;  /* number of buttons, gets from bflags */
  l_int      bw = 90; /* width of button */
  l_int      bh = 25; /* height of button */
  l_int      be = 20; /* between place */

  if ( !o || !owner ) return NULL;

  appwin_init(o, r, caption, flags, 0, trans_ev);

  t = VIEW(o)->size_limits(VIEW(o));

  st = stattext_init_ex(_malloc(sizeof(t_stattext)), t, TX_ALIGN_LEFT, in_text, argp);

  /* get number of buttons */

  while ( i < sizeof(l_dword)*8 ) {

    if ( bflags & ((l_dword)1<<i) ) bn++;

    i++;

  };


  /* no buttons */
  if ( !bflags ) {

     bh = 0; /* there is no button */
     be = 10;

  };


  /* get default place for stattext */
  t.a.x += be;
  t.a.y += 5;
  t.b.y -= bh+be; /* place for buttons */
  t.b.x -= be;

  /* get icon of dialog box ( from app.dlx end of file) */
  img = (iflag<MI_ICONS)?(BITMAP*)GET_DATA(datfile, iflag):NULL;


  /* ok, stattext exist => enough memory */
  if ( st ) {

    l_int x;
    l_int y;

    /* get size of formated text in (y) and (x) */
    get_size_of_ftext(st->text, VIEW(st)->font, &x, &y);

    /* image ( icon ) of dialog exist */
    if ( img ) {

      /* get width of the image plus size between buttons */
      xf = IMAGE_WIDTH(img)+be;
      /* make rectangle for icon */
      ric = rect_assign(t.a.x, t.a.y, t.a.x+IMAGE_WIDTH(img), t.a.y+IMAGE_HEIGHT(img));

    };

    t = VIEW(o)->size_limits(VIEW(o));

    /* make sizes of window */
    r.b.x = r.a.x+max(bn*bw+be*2+(bn-1)*10, x+xf+(be*n))+10;
    r.b.y = r.a.y+y+t.a.y+bh+(be*3);

    /* align window to center */
    VIEW(o)->align |= TX_ALIGN_CENTER;

    if ( flags & WF_MINSIZE )

          /* size of window is minimal to (sf) */
          r.b.x = r.a.x+max(rect_sizex(r), rect_sizex(sf));

    /* set right ! bounds of window */
    VIEW(o)->set_bounds(VIEW(o), r);

    /* get limits of window for objects inserted into the window */
    t = VIEW(o)->size_limits(VIEW(o));

    /* allocate place for stattext */
    t.a.x += be+xf;
    t.a.y += be;
    t.b.y -= bh+be; /* place for buttons */
    t.b.x -= be;

    /* set right ! bounds for stattext */
    VIEW(st)->set_bounds(VIEW(st), t);

    /* icon exist ? */
    if ( img ) {

      /* insert icon to the center of (y) */
      ric.a.y += (rect_sizey(t)-IMAGE_HEIGHT(img))/2;
      ric.b.y  = ric.a.y+IMAGE_HEIGHT(img);

      /* allocate icon and enable to stretch it */
      icn = icon_init ( _malloc(sizeof(t_icon)), ric, img, 0, IF_STRETCH);

      if ( icn ) {

        /* icon can't be selected and is disable !!! */
        OBJECT(icn)->set_options(OBJECT(icn), OB_OF_SELECTABLE+OB_OF_ENABLE, false);

        /* icon can't be dragging */
        VIEW(icn)->drag_mode = 0;

      };

    };

  };

  t.a.x -= xf;

  /* insert window to his owner */

  owner->insert(owner, OBJECT(o));

  /* insert stattext to the window */
  OBJECT(o)->insert(OBJECT(o), OBJECT(st));

  /* insert icon to the window */
  OBJECT(o)->insert(OBJECT(o), OBJECT(icn));

  t.a.y  = t.b.y + 5; /* make place between text and buttons */
  t.b.y  = t.a.y + bh;

  /* button FLAGS !!! */

  if ( bflags & MB_YES ) { /* button YES */

    /* move button rectangle for the right place */
    t.b.x = t.a.x+bw;

    /* insert button into the window */
    OBJECT(o)->insert(OBJECT(o), OBJECT(button_init(_malloc(sizeof(t_button)), t, TXT_YES, MSG_YES, BF_NORMAL)));

    t = rect_move(t, bw+10, 0);

  };

  if ( bflags & MB_OK ) { /* button OK */

    /* move button rectangle for the right place */
    t.b.x = t.a.x+bw;

    /* insert button into the window */
    OBJECT(o)->insert(OBJECT(o), OBJECT(button_init(_malloc(sizeof(t_button)), t, TXT_OK, MSG_OK, BF_NORMAL)));

    t = rect_move(t, bw+10, 0);

  };

  if ( bflags & MB_CLOSE ) { /* button CLOSE */

    /* move button rectangle for the right place */
    t.b.x = t.a.x+bw;

    /* insert button into the window */
    OBJECT(o)->insert(OBJECT(o), OBJECT(button_init(_malloc(sizeof(t_button)), t, TXT_CLOSE, MSG_CLOSE, BF_NORMAL)));

    t = rect_move(t, bw+10, 0);

  };

  if ( bflags & MB_CANCEL ) {  /* button CANCEL */

    /* move button rectangle for the right place */
    t.b.x = t.a.x+bw;

    /* insert button into the window */
    OBJECT(o)->insert(OBJECT(o), OBJECT(button_init(_malloc(sizeof(t_button)), t, TXT_CANCEL, MSG_CANCEL, BF_NORMAL)));

    t = rect_move(t, bw+10, 0);

  };

  if ( bflags & MB_HELP ) { /* button HELP */

    /* move button rectangle for the right place */
    t.b.x = t.a.x+bw;

    /* insert button into the window */
    OBJECT(o)->insert(OBJECT(o), OBJECT(button_init(_malloc(sizeof(t_button)), t, TXT_HELP, MSG_HELP, BF_NORMAL)));

    t = rect_move(t, bw+10, 0);

  };

  if ( bflags & MB_NO ) { /* button NO */

    /* move button rectangle for the right place */
    t.b.x = t.a.x+bw;

    /* insert button into the window */
    OBJECT(o)->insert(OBJECT(o), OBJECT(button_init(_malloc(sizeof(t_button)), t, TXT_NO, MSG_NO, BF_NORMAL)));

    t = rect_move(t, bw+10, 0);

  };

  if ( bflags & MB_CONTINUE ) { /* button CONTINUE */

    /* move button rectangle for the right place */
    t.b.x = t.a.x+bw;

    /* insert button into the window */
    OBJECT(o)->insert(OBJECT(o), OBJECT(button_init(_malloc(sizeof(t_button)), t, TXT_CONTINUE, MSG_CONTINUE, BF_NORMAL)));

    t = rect_move(t, bw+10, 0);

  };

  if ( bflags & MB_EXIT ) { /* button EXIT */

    /* move button rectangle for the right place */
    t.b.x = t.a.x+bw;

    /* insert button into the window */
    OBJECT(o)->insert(OBJECT(o), OBJECT(button_init(_malloc(sizeof(t_button)), t, TXT_EXIT, MSG_EXIT, BF_NORMAL)));

    t = rect_move(t, bw+10, 0);

  };

  if ( bflags & MB_YESTOALL ) { /* button YESTOALL */

    /* move button rectangle for the right place */
    t.b.x = t.a.x+bw;

    /* insert button into the window */
    OBJECT(o)->insert(OBJECT(o), OBJECT(button_init(_malloc(sizeof(t_button)), t, TXT_YESTOALL, MSG_YESTOALL, BF_NORMAL)));

    t = rect_move(t, bw+10, 0);

  };

  if ( bflags & MB_NOTOALL ) { /* button NOTOALL */

    /* move button rectangle for the right place */
    t.b.x = t.a.x+bw;

    /* insert button into the window */
    OBJECT(o)->insert(OBJECT(o), OBJECT(button_init(_malloc(sizeof(t_button)), t, TXT_NOTOALL, MSG_NOTOALL, BF_NORMAL)));

    t = rect_move(t, bw+10, 0);

  };

  return o; /* return window */

};


static void message_ev ( p_object o, t_event *e )
{
  RETVIEW(o, e);

  if ( o->phase == PH_PREPROCESS ) {

    if ( e->type == EV_MESSAGE && is_my_message(o, e) ) {

      o->end_state = e->message;

      clear_event(e);

    };

  };

};



l_dword  msgbox_arg ( l_dword wflags, l_dword  bflags, l_text in_text, va_list args )
{

  p_appwin w = (p_appwin)_malloc(sizeof(t_appwin));

  l_dword  msg = MSG_NOTHING;

  if ( w ) {

    t_rect  r = rect_assign(0, 0, 300, 200);
    l_text  capt = NULL;
    l_char  iflag = 0;

    MSG_FLAGS_SETUP;

    w = message_init_ex(OBJECT(desktop), w, r, capt, 0, iflag, bflags, &message_ev, in_text, args);

    msg = desktop->execute_view(desktop, VIEW(w));

    dispose(OBJECT(w));

  };

  return msg;

};


l_dword  msgbox ( l_dword wflags, l_dword  bflags, l_text in_text, ... )
{
   va_list arg;
   l_dword msg;

   va_start(arg, in_text);

   msg = msgbox_arg(wflags, bflags, in_text, arg);

   va_end(arg);

   return msg;
};


/* same as msgbox, but it sets minimum width of window */

l_dword  msgboxsize_arg ( l_rect minx, l_dword wflags, l_dword  bflags, l_text in_text, va_list args )
{
  p_appwin w = (p_appwin)_malloc(sizeof(t_appwin));

  l_dword  msg = MSG_NOTHING;

  if ( w ) {

    t_rect  r = rect_assign(0, 0, minx, 200);
    l_text  capt = NULL;
    l_char  iflag = 0;

    MSG_FLAGS_SETUP;

    w = message_init_ex(OBJECT(desktop), w, r, capt, WF_MINSIZE, iflag, bflags, &message_ev, in_text, args);

    msg = desktop->execute_view(desktop, VIEW(w));

    dispose(OBJECT(w));

  };

  return msg;

};


l_dword  msgboxsize ( l_rect minx, l_dword wflags, l_dword  bflags, l_text in_text, ... )
{
   va_list arg;
   l_dword msg;

   va_start(arg, in_text);

   msg = msgboxsize_arg(minx, wflags, bflags, in_text, arg);

   va_end(arg);

   return msg;
};


/*
   show info window
*/

p_appwin msginfo_arg ( l_rect minx, l_dword wflags, l_text in_text, va_list args )
{
  p_appwin w = (p_appwin)_malloc(sizeof(t_appwin));

  if ( w ) {

    t_rect  r = rect_assign(0, 0, minx, 200);
    l_text  capt = NULL;
    l_char  iflag = 0;

    MSG_FLAGS_SETUP;

    w = message_init_ex(OBJECT(desktop), w, r, capt, WF_MINSIZE, iflag, 0, NULL, in_text, args);

    delay(500);

  };

  return w;

};


p_appwin msginfo ( l_rect minx, l_dword wflags, l_text in_text, ... )
{
   va_list  arg;
   p_appwin w;

   va_start(arg, in_text);

   w = msginfo_arg(minx, wflags, in_text, arg);

   va_end(arg);

   return w;
};


/*
   show prosess window
*/

p_appwin msgprocess_ex ( l_rect minx, l_text process_title, l_text in_text,
                         l_dword size, l_dword *ind, l_int flags, l_hdlx id,
                         void (*trans_ev)(p_object, p_event) )
{

   t_rect   r = rect_assign(0, 0, minx, 150);
   p_appwin o = appwin_init(APPWIN(_malloc(sizeof(t_appwin))), r, process_title, flags, id, trans_ev);

   if ( o ) {

       VIEW(o)->align |= TX_ALIGN_CENTER; /* align to center */

       OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(o));

       r = VIEW(o)->size_limits(VIEW(o));

       r = rect_assign(r.a.x+10, r.a.y+10, r.b.x-10, r.a.y+30);

       OBJECT(o)->insert(OBJECT(o), OBJECT(stattext_init( /* insert text to window */
                                           STATTEXT(_malloc(sizeof(t_stattext))),
                                           r,
                                           TX_ALIGN_LEFT,
                                           in_text)
                                    ));

       r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+25);

       OBJECT(o)->insert(OBJECT(o), OBJECT(process_init( /* insert process object to window */
                                           PROCESS(_malloc(sizeof(t_process))),
                                           r,
                                           size, ind)
                                    ));

       r = rect_assign(r.a.x, r.b.y+20, r.a.x+100, r.b.y+50);

       OBJECT(o)->insert(OBJECT(o), OBJECT(button_init( /* insert cancel button to window */
                                           BUTTON(_malloc(sizeof(t_button))),
                                           r,
                                           TXT_CANCEL,
                                           MSG_CLOSE,
                                           BF_DEFAULT)
                                    ));



   };

   return o;

};


p_appwin msgprocess ( l_rect minx, l_text process_title, l_text in_text, l_dword size, l_dword *ind )
{

   return msgprocess_ex ( minx, process_title, in_text, size, ind, 0, 0, NULL );
};





/* frame object */

void    frame_draw_title ( p_window o )
{
};


void    frame_draw ( p_view o )
{
  t_rect  r = o->get_local_extent(o);
  t_rect  safe = r;
  t_point p = o->get_global_point(o, r.a);

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    o->background(o, out, rect_move(r, p.x, p.y));
    button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_SYSx24), COLOR(CO_SYSx19));
    button(out, r.a.x+p.x+1, r.a.y+p.y+1, r.b.x+p.x-1, r.b.y+p.y-1, COLOR(CO_WHITE), COLOR(CO_BLACK));
    button(out, r.a.x+p.x+2, r.a.y+p.y+2, r.b.x+p.x-2, r.b.y+p.y-2, COLOR(CO_BLACK), COLOR(CO_WHITE));

  };

  o->end_of_paint(o, safe);
};


p_window  _frame_init ( p_window o, t_rect r )
{

  if ( !o ) return NULL;

  win_init(o, r, NULL, 0);

  VIEW(o)->draw = &frame_draw;
  o->draw_title = &frame_draw_title;

  return o;

};



static l_int  _seal_app_error ( l_int errtype, l_text str, ... )
{

  switch ( errtype ) {

    case ERR_NONREGULAR : {

      va_list arg;
      l_int ret = 0;

      va_start(arg, str);

      ret = (msgbox_arg(MW_ERROR, MB_EXIT|MB_CONTINUE, str, arg)==MSG_CONTINUE);

      va_end(arg);

      return ret;

    }; break;

    case ERR_INFO : {

      va_list arg;

      va_start(arg, str);

      msgbox_arg(MW_WARNING, MB_OK, str, arg);

      va_end(arg);

      return 1;

    }; break;

  };

  return 0;

};


#include"app.exp"

lib_begin ( void ) {

  if ( ap_process == AP_ALLOC ) {

          AP_EXPORTLIB(); /* export table in app.exp */

  } else

  if ( ap_process == AP_INIT ) {

          AP_SETNUMOFCALLS(1); /* set max num of calls */

          datfile = conv_to_skipcolor_data(DLXGetFileData(ap_id), CO_SKIP_RED, CO_SKIP_GREEN, CO_SKIP_BLUE);

          pal_appwin[0] = color_get_from_ini("3D_background");
          pal_appwin[1] = color_get_from_ini("window_active_title");
          pal_appwin[2] = color_get_from_ini("window_active_title_gradient");
          pal_appwin[3] = color_get_from_ini("window_pasive_title");
          pal_appwin[4] = color_get_from_ini("window_pasive_title_gradient");
          pal_appwin[5] = color_get_from_ini("window_active_caption");
          pal_appwin[6] = color_get_from_ini("window_pasive_caption");

          seal_error = &_seal_app_error;

  } else

  if ( ap_process == AP_FREE ) {


    unload_datafile(datfile);
    datfile = NULL;

  };


} lib_end;

