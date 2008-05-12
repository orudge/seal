/****************************************************************/
/*                                                              */
/*                           htmlwin.c                          */
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
#include"htmlwin.h"
#include"menus.h"
#include"button.h"
#include"dialogs.h"
#include"iodlg.h"
#include"editor.h"
#include <dir.h>

#define HTMLWIN_ICON_SIZE  45 /* size of icons */

#define ICO_GET(n) (BITMAP*)GET_DATA(datfile, n)

#define ICO_BACK     0
#define ICO_FORWARD  1
#define ICO_OPEN     2
#define ICO_RELOAD   3

static DATAFILE *datfile = NULL; /* contains icons of html window */


p_htmlwin  (*htmlwin_init) ( p_htmlwin o, t_rect r, l_text title, l_text fname ) = &_htmlwin_init;

static l_text last_opened = NULL;


l_bool  htmlwin_done ( p_object o )
{
  if ( obj_exist(o) <= 0 ) return false;

  afree(&(HTMLWIN(o)->filename));

  dispose_list(&(HTMLWIN(o)->queue), 1);

  return view_done(o);

};


void  htmlwin_setup ( p_object o )
{
   p_view    v = VIEW(o);
   p_htmlwin h = HTMLWIN(o);
   p_object  b = NULL;

   t_rect r = v->size_limits(v);
   t_rect s = r;

   appwin_setup(o);

   if ( !h->htmloader ) { /* init new t_htmloader */

         t_rect x = r;

         /* move origins for htmloader */
         x.b.x -= STANDARD_SCROLLBAR_WIDTH;
         x.b.y -= STANDARD_SCROLLBAR_WIDTH;
         x.a.y += HTMLWIN_ICON_SIZE+2;

         h->htmloader = htmloader_init(
                                _malloc(sizeof(t_htmloader)), /* memory for t_htmloader class */
                                x,  /* area of htmloader */
                                h->filename);

         if ( h->htmloader )

               VIEW(h->htmloader)->align |= TX_ALIGN_RIGHT+TX_ALIGN_BOTTOM;

         o->insert(o, OBJECT(h->htmloader)); /* insert htmloader into the window  */

         h->push_link(h, h->filename, false);

   };

    r.b.y = r.a.y + HTMLWIN_ICON_SIZE;
    r.b.x = r.a.x + HTMLWIN_ICON_SIZE;

    /* init new button (BACK) */
    b = OBJECT(icobutton_init(
                            _malloc(sizeof(t_icobutton)), /* memory for class t_button */
                            r, /* button area */
                            ICO_GET(ICO_BACK),
                            TXT_BACKTOPREVIUSPAGE,
                            MSG_HTMLBACK,         /* message of the button */
                            BF_PUSH)     /* flags of the button */
               );

    o->insert(o, b); /* insert BACK button into the window */


    r = rect_move(r, HTMLWIN_ICON_SIZE+2, 0);

    /* init new button (FORWARD) */
    b = OBJECT(icobutton_init(
                            _malloc(sizeof(t_icobutton)), /* memory for class t_button */
                            r, /* button area */
                            ICO_GET(ICO_FORWARD),
                            TXT_FORWARDTONEXTPAGE,
                            MSG_HTMLNEXT,         /* message of the button */
                            BF_PUSH)     /* flags of the button */
               );

    o->insert(o, b); /* insert FORWARD button into the window */


    r = rect_move(r, HTMLWIN_ICON_SIZE+10, 0);

    /* init new button (OPEN) */
    b = OBJECT(icobutton_init(
                            _malloc(sizeof(t_icobutton)), /* memory for class t_button */
                            r, /* button area */
                            ICO_GET(ICO_OPEN),
                            TXT_OPENNEWPAGE,
                            MSG_HTMLOPEN,         /* message of the button */
                            BF_PUSH)     /* flags of the button */
               );

    o->insert(o, b); /* insert OPEN button into the window */


    r = rect_move(r, HTMLWIN_ICON_SIZE+2, 0);

    /* init new button (RELOAD) */
    b = OBJECT(icobutton_init(
                            _malloc(sizeof(t_icobutton)), /* memory for class t_button */
                            r, /* button area */
                            ICO_GET(ICO_RELOAD),
                            TXT_RELOADCURRENTPAGE,
                            MSG_HTMLRELOAD,         /* message of the button */
                            BF_PUSH|BF_DEFAULT)  /* flags of the button */
               );

    o->insert(o, b); /* insert RELOAD button into the window */

    r = rect_move(r, HTMLWIN_ICON_SIZE+10, 0);

    r.b.x = r.a.x+30;
    r.a.y = r.b.y-FONT_GETHEIGHT(font_system);

    /* init new stattext (url:) */
    b = OBJECT(stattext_init(
                            _malloc(sizeof(t_stattext)), /* memory for class t_button */
                            r, /* button area */
                            TX_ALIGN_DEFAULT, /* alignment, same as 0 */
                            "url:")     /* text of stattext */
               );

    o->insert(o, b); /* insert "url:" stattext into the window */

    if ( !h->urlline ) { /* init new t_texline for URL */

         r.a.x = r.b.x+2;
         r.b.y -= 2;
         r.a.y  = r.b.y - 20;
         r.b.x  = lmax(r.a.x+50, s.b.x - 10);

         /* init new url line (URL LINE) */
         b = OBJECT(textline_init(
                            _malloc(sizeof(t_textline)), /* memory for class t_textline */
                            r, /* button area */
                            IO_URL_LIMIT,
                            0)     /* flags of the textline => rewrite able */
               );

         if ( b ) { /* set current URL to new textline */

               TEXTLINE(b)->set_text(TEXTLINE(b), h->filename);

               VIEW(b)->align |= TX_ALIGN_RIGHT;

         };

         h->urlline = TEXTLINE(b);

         o->insert(o, b); /* insert QUESTION button into the window */
    };

};


static p_list  htmlwin_open_history_list ( void )
{

  p_list p = list_init(_malloc(sizeof(t_list)), &free_filehistory_item, 0);

  if ( p ) {

     p->insert(p, new_filehistory_item("HTML files (*.htm)", "*.htm"));
     p->insert(p, new_filehistory_item("All files (*.*)", "*.*"));

  };

  return p;

};


void   htmlwin_redraw_title ( p_htmlwin o )
{
    _free(WINDOW(o)->caption);

    if ( o->htmloader && o->htmloader->hengine &&
         o->htmloader->hengine->htitle )

         WINDOW(o)->caption = set_format_text(NULL, "HPage - %s", o->htmloader->hengine->htitle);

    else

         WINDOW(o)->caption = set_format_text(NULL, "HPage - %s", TXT_NONAME);

    TEST_SUB_VIEWS(VIEW(o), WINDOW(o)->draw_title(WINDOW(o)));
};


/* reload current link */

l_bool  htmlwin_reload ( p_htmlwin o )
{

  if ( o->htmloader && o->urlline && o->urlline->text ) {

     l_bool ret = o->htmloader->load_html(o->htmloader, o->urlline->text); /* load new html */

     if ( ret )  /* redraw title */

          o->redraw_title(o);

     return ret;

  };

  return false;

};


/*
  add new link to queue

  - if there is the same one, it only moves it to last position
*/
void   htmlwin_push_link ( p_htmlwin o, l_text link, l_bool reload )
{
   if ( o->queue && link ) {

         p_item x = NULL;
         p_item f = o->queue->first(o->queue);

         if ( o->urlline ) /* set urlline */

               o->urlline->set_text(o->urlline, link);

         if ( reload ) o->reload(o);

         else o->redraw_title(o);

         x = o->queue->at_item(o->queue, o->index+1);

         if ( x )

         while ( x != f ) {  /* remove from this index */

            p_item n = x->next;

            o->queue->free_item(o->queue, x);

            x = n;
         };

         o->queue->insert(o->queue, _strdup(link)); /* insert new item */

         o->index = o->queue->get_max_item(o->queue);

   };
};


/*
  remove last link from queue

  - set current filename to previus link

  + reload previus link
*/
void   htmlwin_back_link ( p_htmlwin o )
{
   /* clear last item only if there's more items than one */
   if ( o->queue && o->index > 0 ) {

         p_item item;

         o->index--;

         item = o->queue->at_item(o->queue, o->index);

         if ( !item || !item->rec ) return;

         if ( o->urlline ) /* set urlline */

              o->urlline->set_text(o->urlline, (l_text)item->rec);

         o->reload(o);

   };
};


void   htmlwin_next_link ( p_htmlwin o )
{
   /* clear last item only if there's more items than one */
   if ( o->queue && o->index < o->queue->get_max_item(o->queue) ) {

         p_item item;

         o->index++;

         item = o->queue->at_item(o->queue, o->index);

         if ( !item || !item->rec ) return;

         if ( o->urlline ) /* set urlline */

              o->urlline->set_text(o->urlline, (l_text)item->rec);

         o->reload(o);

   };
};


static void  trans_ev ( p_object o, p_event event )
{

  RETVIEW(o, event);

  if ( o->phase == PH_PREPROCESS ) {

    if ( (event->type == EV_MESSAGE || event->type == EV_INFO)

         && is_my_message(o, event) ) {

         switch ( event->message ) {

            case MSG_HTMLRELOAD : { /* reload page */

                 clear_event(event);

                 HTMLWIN(o)->reload(HTMLWIN(o));

                 clear_event(event);

            }; break;

            case MSG_HTMLBACK : { /* back to prevoius page */

                 clear_event(event);

                 HTMLWIN(o)->back_link(HTMLWIN(o));

            }; break;

            case MSG_HTMLNEXT : { /* forward to next page */

                 clear_event(event);

                 HTMLWIN(o)->next_link(HTMLWIN(o));

            }; break;

            case MSG_ANCHORPRESSED : { /* some of anchors was pressed */

               if ( event->type == EV_INFO &&
                    event->obj == OBJECT(HTMLWIN(o)->htmloader) &&
                    event->info ) {

                    HTMLWIN(o)->push_link(HTMLWIN(o), (l_text)event->info, false);

                    clear_event(event); /* clear events */

                    _free((l_text)event->info);

               };

            }; break;

            case MSG_HTMLOPEN : {
                               /* get history list of files */
                 p_list hist = htmlwin_open_history_list();

                 l_text f = open_dialog(last_opened, "", hist); /* open new dialog */

                 if ( f ) {

                     _free(last_opened);

                     last_opened = io_path(f);

                     HTMLWIN(o)->push_link(HTMLWIN(o), f, true);

                     _free(f);

                 };

                 dispose_list(&hist, 1); /* dispose history list */

                 clear_event(event); /* clear events */

            }; break;

         };

    };

  };

};



static t_point htmlwin_size_minimum ( p_view o )
{

  return point_assign(300, 200);

};


p_htmlwin  _htmlwin_init ( p_htmlwin o, t_rect r, l_text title, l_text fname )
{

   if ( !o ) return NULL;

   clear_type(o, sizeof(t_htmlwin));

   appwin_init(APPWIN(o), r, title, WF_MINIMIZE, 0, &trans_ev);


   /* object declarations */

   OBJECT(o)->done = &htmlwin_done;
   OBJECT(o)->setup = &htmlwin_setup;


   /* view declarations */

   VIEW(o)->drag_mode |= DM_DRAGGROW; /* able to grow view */
   VIEW(o)->size_minimum = &htmlwin_size_minimum;


   /* htmlwin declarations */

   o->filename = _strdup(fname);

   o->queue = list_init(_malloc(sizeof(t_list)), &_free, DAT_TEXT); /* make new file queue */

   o->push_link = &htmlwin_push_link;
   o->back_link = &htmlwin_back_link;
   o->next_link = &htmlwin_next_link;
   o->reload = &htmlwin_reload;
   o->redraw_title = &htmlwin_redraw_title;

   return o;

};


#include"htmlwin.exp"

lib_begin ( void ) {

  if ( ap_process == AP_ALLOC ) {

          AP_EXPORTLIB(); /* export table in app.exp */

  } else

  if ( ap_process == AP_INIT ) {

          AP_SETNUMOFCALLS(1); /* set max num of calls */

          datfile = conv_to_skipcolor_data(DLXGetFileData(ap_id), CO_SKIP_RED, CO_SKIP_GREEN, CO_SKIP_BLUE);

          last_opened = _strdup("c:/");

  } else

  if ( ap_process == AP_FREE ) {


    unload_datafile(datfile);
    datfile = NULL;

    _free(last_opened);

  };


} lib_end;

