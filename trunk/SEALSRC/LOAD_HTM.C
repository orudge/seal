/****************************************************************/
/*                                                              */
/*                          load_htm.c                          */
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
#include"scroller.h"
#include"load_htm.h"
#include"app.h"
#include"dialogs.h"
#include"button.h"
#include<dir.h>
#include"htmldecl.c"

#define HTML_TABBLOCK_SIZE    50

l_color pal_htmloader[] = {CO_SYSx20, CO_BLACK, CO_BLUE, CO_RED, CO_NOCOLOR};

p_htmloader  (*htmloader_init) ( p_htmloader o, t_rect r, l_text filename ) = &_htmloader_init;
p_htmloader  (*htmloader_init_ex) ( p_htmloader o, t_rect r, l_text filename, l_dword flags ) = &_htmloader_init_ex;


static DATAFILE *datfile = NULL;




#include"html4.c"


/* t_htmloader functions */

void  htmloader_setup ( p_object o )
{

  HTMLOADER(o)->load_html(HTMLOADER(o), HTMLOADER(o)->filename);

  scroller_setup(o);

};


/*
  find the first rectangle from where anchor "anchorname" start

*/
t_rect  htmloader_find_anchor_name_ex ( p_htmloader o, HTML_DEF_ENGINE, l_rect dx, l_dword dy, l_text anchorname )
{

  if ( engine ) {

      p_scroller so = SCROLLER(o);
      html_obs  *ob = engine->hobjectsout;

      while ( ob ) {

         if (
              ob->hobjtype &&
              dy+ob->hobjtype->hrect.hbottom >= 0 &&
              ob->hview && ob->hobjtype && !html_rect_equals(ob->hobjtype->hrect, HTML_RECT_EMPTY)
            ) {

               t_rect v = rect_assign(ob->hobjtype->hrect.hleft, ob->hobjtype->hrect.htop, ob->hobjtype->hrect.hright, ob->hobjtype->hrect.hbottom);

                      v = rect_move(v, dx, dy); /* set position relative to scrolling */

               if ( ob->hobjtype->hanchorname &&
                   !stricmp(anchorname, ob->hobjtype->hanchorname) ) /* anchors is same ? */

                   return v;

               if ( ob->hidin == TABLE_S ) { /* <TABLE> command */

                  html_table *table = (html_table*)ob->hobjtype;

                  t_rect ro = o->find_anchor_name_ex(o, table->hengine, ob->hobjtype->hrect.hleft+dx, ob->hobjtype->hrect.htop+dy, anchorname);

                  if ( !rect_check_empty(ro) ) return ro;

               };

               if ( ob->hidin == TD_S ||  /* <TD> command */
                    ob->hidin == TH_S ) { /* <TH> command */

                  html_td *td = (html_td*)ob->hobjtype;

                  t_rect ro = o->find_anchor_name_ex(o, td->hengine, ob->hobjtype->hrect.hleft+dx, ob->hobjtype->hrect.htop+dy, anchorname);

                  if ( !rect_check_empty(ro) ) return ro;

               };

         };

         ob = ob->hnext;

      };

  };

  return rect_empty;

};


/*
  find the object where mouse is placed
*/

html_obs*  htmloader_find_object_under_mouse_ex ( p_htmloader o, HTML_DEF_ENGINE, l_rect dx, l_dword dy, t_point where )
{

  if ( engine ) {

      p_scroller so = SCROLLER(o);
      html_obs  *ob = engine->hobjectsout;
      t_rect     r  = VIEW(o)->size_limits(VIEW(o));

      while ( ob ) {

         if (
              ob->hobjtype &&
              dy+ob->hobjtype->hrect.hbottom-so->scrolly >= 0 &&
//              dy+ob->hobjtype->hrect.htop-so->scrolly <= rect_sizey(r) &&
              ob->hview && ob->hobjtype && !html_rect_equals(ob->hobjtype->hrect, HTML_RECT_EMPTY)
            ) {

               t_rect v = rect_assign(r.a.x+ob->hobjtype->hrect.hleft, r.a.y+ob->hobjtype->hrect.htop, r.a.x+ob->hobjtype->hrect.hright, r.a.y+ob->hobjtype->hrect.hbottom);

                      v = rect_move(v, dx-so->scrollx, dy-so->scrolly); /* set position relative to scrolling */

               if ( ob->hidin == TABLE_S ) { /* <TABLE> command */

                  html_table *table = (html_table*)ob->hobjtype;

                  html_obs *p = o->find_object_under_mouse_ex(o, table->hengine, ob->hobjtype->hrect.hleft+dx, ob->hobjtype->hrect.htop+dy, where);

                  if ( p ) return p;

               };

               if ( ob->hidin == TD_S ||  /* <TD> command */
                    ob->hidin == TH_S ) { /* <TH> command */

                  html_td *td = (html_td*)ob->hobjtype;

                  html_obs *p = o->find_object_under_mouse_ex(o, td->hengine, ob->hobjtype->hrect.hleft+dx, ob->hobjtype->hrect.htop+dy, where);

                  if ( p ) return p;

               };

               if ( rect_contains(v, where) ) return ob;

         };

         ob = ob->hnext;

      };

  };

  return NULL;

};


t_rect  htmloader_find_anchor_name ( p_htmloader o, HTML_DEF_ENGINE, l_text anchorname )
{

  if ( o->hengine && anchorname )

      return  o->find_anchor_name_ex(o, o->hengine, 0, 0, anchorname);

  return rect_empty;

};


html_obs*  htmloader_find_object_under_mouse ( p_htmloader o, t_point where )
{

  if ( o->hengine )

      return  o->find_object_under_mouse_ex(o, o->hengine, 0, 0, where);

  return NULL;

};

/*
  return anchor-object where mouse is over it
*/
html_obs *htmloader_find_anchor ( p_htmloader o )
{

  t_point w = VIEW(o)->get_local_point(VIEW(o), mouse->where);

  html_obs *ob = HTMLOADER(o)->find_object_under_mouse(HTMLOADER(o), w);

  /* if object is under mouse and has some anchor */

  if ( ob && ob->hobjtype && ob->hobjtype->hreference )

     return ob;


  return NULL;

};


void    htmloader_set_mouse_cursor ( p_view o )
{

  if ( HTMLOADER(o)->find_anchor(HTMLOADER(o)) )

      mouse_set_cursor_focus_id(CUR_FINGER);

  else

      view_set_mouse_cursor(o);  /* old function */

};


t_rect  htmloader_size_limits ( p_view o )
{

  return rect_assign(22, 2, rect_sizex(o->bounds)-10, rect_sizey(o->bounds)-2);

};


void  htmloader_draw ( p_view o )
{
  t_rect  r = o->get_local_extent(o);
  t_rect  safe = r;
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, safe);

  if ( out ) {

    if ( HTMLOADER(o)->flags & HLF_3D ) {

      button(out, p.x+r.a.x, p.y+r.a.y, p.x+r.b.x, p.y+r.b.y, COLOR(CO_BLACK), COLOR(CO_WHITE));
      button(out, p.x+r.a.x+1, p.y+r.a.y+1, p.x+r.b.x-1, p.y+r.b.y-1, COLOR(CO_SYSx18), COLOR(CO_SYSx24));

      r = rect_assign(p.x+r.a.x+2, p.y+r.a.y+2, p.x+r.b.x-2, p.y+r.b.y-2);

    } else

      r = rect_assign(p.x+r.a.x, p.y+r.a.y, p.x+r.b.x, p.y+r.b.y);


    if ( HTMLOADER(o)->hengine ) { /* set background */

      o->brush.background = HTMLOADER(o)->hengine->hpattern;
      o->brush.color = HTMLOADER(o)->hengine->hbcolor;

    };

    o->background(o, out, r);

    HTMLOADER(o)->draw_box(HTMLOADER(o));

  };

  o->end_of_paint(o, safe);
};


void  htmloader_scroll_size ( p_scroller o, l_long *x, l_long *y )
{
  t_rect r = o->scroll_limits(o);

  if ( x ) *(x) = lmax(rect_sizex(r), calc_maxscrollx(o, HTMLOADER(o)->x_last));
  if ( y ) *(y) = lmax(rect_sizey(r), calc_maxscrolly(o, HTMLOADER(o)->y_last));

};


void  htmloader_draw_table ( p_htmloader o, BITMAP *out, HTML_DEF_ENGINE, t_point p, l_long dx, l_long dy )
{

  p_view  vo = VIEW(o);
  p_scroller  so = SCROLLER(o);
  t_rect  r = vo->size_limits(vo);

  if ( out ) {

     if ( engine ) { /* exit engine */

         html_obs *ob = engine->hobjectsout;

         while ( ob ) {

         if ( dy+ob->hobjtype->hrect.hbottom-so->scrolly >= 0 &&
              dy+ob->hobjtype->hrect.htop-so->scrolly <= rect_sizey(r) &&
              ob->hview && ob->hobjtype && !html_rect_equals(ob->hobjtype->hrect, HTML_RECT_EMPTY)
            ) {

               /* set rectangular area of object into t_rect struct */
               t_rect x = rect_assign(r.a.x+ob->hobjtype->hrect.hleft, r.a.y+ob->hobjtype->hrect.htop, r.a.x+ob->hobjtype->hrect.hright, r.a.y+ob->hobjtype->hrect.hbottom);
               t_rect v = rect_assign(r.a.x+ob->hobjtype->hrect.hleft+p.x, r.a.y+ob->hobjtype->hrect.htop+p.y, r.a.x+ob->hobjtype->hrect.hright+p.x, r.a.y+ob->hobjtype->hrect.hbottom+p.y);

                      v = rect_move(v, -so->scrollx+dx, -so->scrolly+dy); /* set position relative to scrolling */
                      x = rect_move(x, dx, dy); /* set position relative dx/dy */

               if ( ob->hidin == TABLE_S ) { /* <TABLE> command */

                  html_table *table = (html_table*)ob->hobjtype;

                  if ( table->hbgcolor != -1 ) /* i can draw background */

                     rectfill(out, v.a.x, v.a.y, v.b.x, v.b.y, table->hbgcolor);

                  if ( table->hborder )

                     button(out, v.a.x, v.a.y, v.b.x, v.b.y, COLOR(CO_WHITE), COLOR(CO_DARKGRAY));

                  o->draw_table(o, out, table->hengine, p, ob->hobjtype->hrect.hleft+dx, ob->hobjtype->hrect.htop+dy);

               };

               if ( ob->hidin == TD_S ||  /* <TD> command */
                    ob->hidin == TH_S ) { /* <TH> command */

                  html_td *td = (html_td*)ob->hobjtype;

                  if ( td->hbgcolor != -1 ) /* i can draw background */

                     rectfill(out, v.a.x, v.a.y, v.b.x, v.b.y, td->hbgcolor);

                  if ( td->hborder )

                     button(out, v.a.x, v.a.y, v.b.x, v.b.y, COLOR(CO_DARKGRAY), COLOR(CO_WHITE));

                  o->draw_table(o, out, td->hengine, p, ob->hobjtype->hrect.hleft+dx+td->hcellpadding, ob->hobjtype->hrect.htop+dy+td->hcellpadding);

               };

               if ( ob->hid == HR_S ) { /* <hr> command */

                  button(out, v.a.x, v.a.y, v.b.x, v.b.y, COLOR(CO_BLACK), COLOR(CO_LIGHTGRAY));

               };


               if (  ob->hidin == INPUT_S || /* input */
                     ob->hidin == IMG_S ||  /* image */
                    ((ob->hid == A_S ||     /* anchor text */
                     ID_BLOCKLEVEL(ob->hid) || /* block levels || lists */
                     ID_LISTS(ob->hid))
                     && strexist(ob->hobjtype->htext)) )
               {

                  l_int align = 0;


                  if ( ob->hid == LI_S ) { /* <LI> */

                     v.a.x -= 14; /* move list bulletins */

                     if ( ((html_blocklevel*)ob->hobjtype)->hlisttype == HTML_LISTTYPE_CIRCLE )

                        /* circle bulletin */
                        circle(out, v.a.x, v.b.y-10, 3, COLOR(CO_BLACK));

                     else

                     if ( ((html_blocklevel*)ob->hobjtype)->hlisttype == HTML_LISTTYPE_SQUARE )

                        /* rectangle bulletin */
                        rectfill(out, v.a.x-3, v.b.y-13, v.a.x+3, v.b.y-7, COLOR(CO_BLACK));

                     else

                        /* circle filled bulletin */
                        circlefill(out, v.a.x, v.b.y-10, 3, COLOR(CO_BLACK));

                     v.a.x += 14;
                  };

                  if ( ob->hobjtype->hreference )  /* <A> */

                     align |= TX_UNDERLINE;


                  if ( ob->hidin != IMG_S ) /* not image => print text */

                     textout_draw_rect(out, HTMLFONTid(((html_blocklevel*)ob->hobjtype)->hfont),
                                            ob->hobjtype->htext, -1, v.a.x, v.a.y,
                                            v.b.x, v.b.y, align, ((html_blocklevel*)ob->hobjtype)->hfcolor,
                                            TX_NOCOLOR, 0);

                  else { /* <IMG> image */

                     if ( ((html_image*)ob->hobjtype)->hbitmap ) { /* exist */

                        draw_sprite(out, ((html_image*)ob->hobjtype)->hbitmap, v.a.x, v.a.y);

                     } else

                        if ( rect_sizex(v) > HTML_BITMAP_MINIMUMX &&
                             rect_sizey(v) > HTML_BITMAP_MINIMUMY )  /* draw info about image place */

                           if ( HLOADIMG(HI_NOIMAGE) ) {

                              draw_sprite(out, HLOADIMG(HI_NOIMAGE), v.a.x+4, v.a.y+4);

                              button(out, v.a.x, v.a.y, v.b.x, v.b.y, COLOR(CO_BLACK), COLOR(CO_LIGHTGRAY));
                           };

                  };

                  /* inputs */

                  if ( ob->hidin == INPUT_S && /* input wasn't inserted yet */
                    !((html_input*)ob->hobjtype)->hinserted ) {

                        html_input *inp = (html_input*)ob->hobjtype;

                        /* input is button or submit */
                        if ( inp->htype == HTML_INPUT_BUTTON || /* */
                             inp->htype == HTML_INPUT_SUBMIT ) { /* make button */

                            OBJECT(o)->insert(OBJECT(o), OBJECT( /* insert button */
                                                         button_init(_malloc(sizeof(t_button)),
                                                             x,
                                                             inp->hvalue,
                                                             MSG_NOTHING,
                                                             BF_NORMAL)
                                                         ));

                            inp->hinserted = 1;

                        } else
                        /* input is textline */
                        if ( inp->htype == HTML_INPUT_TEXT ) {

                            p_textline t;

                            t = textline_init(_malloc(sizeof(t_textline)),
                                                     x,
                                                     inp->hivalue, 0);


                            OBJECT(o)->insert(OBJECT(o), OBJECT(t)); /* insert textline */

                            inp->hinserted = 1;

                            t->set_text(t, inp->hvalue); /* set text of text line */

                        };

                  };

                };

           };

           ob = ob->hnext; /* look for next html object */

         };

     };

  };

};


void  htmloader_draw_box ( p_htmloader o )
{

  p_view  vo = VIEW(o);
  p_scroller  so = SCROLLER(o);
  t_rect  r = vo->size_limits(vo);
  t_point p;

  BITMAP *out = vo->begin_paint(vo, &p, r);

  if ( out ) {

     if ( o->hengine ) {

          o->draw_table(o, out, o->hengine, p, 0, 0);

     } else /* print text "File not found" */

       textout_draw_rect(out, VIEW(o)->font, TXT_FILENOTFOUND, -1, r.a.x+p.x+2, r.a.y+p.y+2,
                              r.b.x+p.x-2, r.b.y+p.y-2, 0, COLOR(CO_BLACK), TX_NOCOLOR, 0);


  };

  vo->end_of_paint(vo, r);

};


/*
   - if engine exist, delete this engine
   - create new engine from file (filename)
   - redraw scrollbars
*/

l_bool  htmloader_init_engine ( p_htmloader o, l_text filename )
{

  l_bool   ok = false;

  t_rect r = VIEW(o)->size_limits(VIEW(o));

  HTML_ENGINE *eng = NULL; /* save old engine */

  if ( !filename ) return false;

  eng = html_init_engine(0, 0, rect_sizex(r), rect_sizey(r));

  if ( eng ) {

         l_text text = html_file_open(eng, filename);

         l_bool exist = (l_bool)text;

         if ( text ) { /* file found */

            if ( io_isas(filename, "htm") ||
                 io_isas(filename, "html") ) { /* file is HTML file */

                  eng->htab_blockquote = HTML_TABBLOCK_SIZE;
                  eng->htab_list = HTML_TABBLOCK_SIZE;

                  html_file_translate(eng, text, -1); /* translate html */

                  html_done_engine(o->hengine);

                  o->hengine = eng;

                  o->x_last = html_get_max_width(o->hengine);

                  o->y_last = html_get_max_y(o->hengine->hobjectsout);

                  reset_scroller(o);

                  VIEW(o)->draw_view(VIEW(o)); /* redraw html view */

                  draw_scroller_bars(o); /* redraw scrollbars */

                  _free(text);

                  return true; /* return ok */

            };

         };

         _free(text);

         html_done_engine(eng); /* delete engine */

         if ( exist )

              ok = run_file(filename); /* open file */

  };

  if ( !ok && filename ) /* make error dialog */

      seal_error(ERR_INFO, "URL: %s %s", filename, strlwr(TXT_NOTFOUND));

  return ok;
};


l_bool   htmloader_load_html ( p_htmloader o, l_text anchor )
{

   if ( anchor ) {

       l_text in = strchr(anchor, '#');

       if ( !in || !(*in) )

           return o->init_engine ( o, anchor );

       else {

           l_text x = strdup(in+1);
           t_rect r;
           l_text win = stridup(anchor, strsize(anchor, in));

           if ( o->hengine && !win ) { /* it's internal anchor */

               r = o->find_anchor_name(o, o->hengine, x);

               SCROLLER(o)->scroll_place(SCROLLER(o), r.a.x-SCROLLER(o)->scrollx, r.a.y-SCROLLER(o)->scrolly, SCROLL_NOW);

               _free(x);

               return true;

           } else if ( win ) { /* it's external engine */

               l_bool ok = o->init_engine ( o, win );

               if ( ok ) {

                  t_rect r = o->find_anchor_name(o, o->hengine, x);

                  SCROLLER(o)->scroll_place(SCROLLER(o), r.a.x-SCROLLER(o)->scrollx, r.a.y-SCROLLER(o)->scrolly, SCROLL_NOW);

               };

               _free(win);
               _free(x);

               return ok;

           };

           _free(x);

           return false;
       };

   };

   return false;
};


l_bool  htmloader_done ( p_object o )
{

  if ( obj_exist(o) <= 0 ) return false;

  html_done_engine(HTMLOADER(o)->hengine); /* delete engine */

  return obj_done(o);

};


void    htmloader_translate_event ( p_object o, t_event *event )
{

  RETVIEW(o, event);

  view_translate_event(o, event);

  if ( event->type & EV_MOUSE ) { /* mouse in view */

     if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) { /* mouse is pressed */

          p_html_obs ob = HTMLOADER(o)->find_anchor(HTMLOADER(o));

          if ( ob ) {

              l_text anch = ob->hobjtype->hreference;

              l_text path = io_realpath(HTMLOADER(o)->hengine->hpath, anch);

              if ( !path ) path = strdup(anch);

              clear_event(event);

              if ( HTMLOADER(o)->load_html(HTMLOADER(o), path) ) { /* ok init engine */

                     set_event_info(event, EV_INFO, MSG_ANCHORPRESSED, o, path);

                     o->put_event(o, event);

              } else _free(path);

              clear_event(event);

          };
     };

  };

};


/* t_htmloader */

p_htmloader  _htmloader_init_ex ( p_htmloader o, t_rect r, l_text filename, l_dword flags )
{
  l_int xf = 0;

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_htmloader));

  if ( flags & HLF_HORSCROLLBAR ) xf |= SF_HORSCROLLBAR;
  if ( flags & HLF_VERSCROLLBAR ) xf |= SF_VERSCROLLBAR;

  scroller_init(SCROLLER(o), r, xf);


  /* object's functions */

  OBJECT(o)->setup = &htmloader_setup;
  OBJECT(o)->done = &htmloader_done;
  OBJECT(o)->translate_event = &htmloader_translate_event;


  /* view's functions */

  VIEW(o)->draw = &htmloader_draw;
  VIEW(o)->size_limits = &htmloader_size_limits;
  VIEW(o)->set_mouse_cursor = htmloader_set_mouse_cursor;


  /* scroller's functions */

  SCROLLER(o)->deltax = 20;
  SCROLLER(o)->deltay = 20;

  SCROLLER(o)->scroll_size = &htmloader_scroll_size;


  /* htmloader's functions */

  o->filename = _strdup(filename);
  o->flags = flags;

  o->init_engine = &htmloader_init_engine;
  o->load_html = &htmloader_load_html;
  o->draw_box = &htmloader_draw_box;
  o->draw_table = &htmloader_draw_table;
  o->find_anchor = &htmloader_find_anchor;
  o->find_object_under_mouse = &htmloader_find_object_under_mouse;
  o->find_object_under_mouse_ex = &htmloader_find_object_under_mouse_ex;
  o->find_anchor_name = &htmloader_find_anchor_name;
  o->find_anchor_name_ex = &htmloader_find_anchor_name_ex;


  /* function calls */

  VIEW(o)->set_palette(VIEW(o), pal_htmloader);

  VIEW(o)->brush.color = VIEW(o)->get_color(VIEW(o), 0);

  return o;
};


/* t_htmloader */

p_htmloader  _htmloader_init ( p_htmloader o, t_rect r, l_text filename )
{
   return htmloader_init_ex(o, r, filename, HLF_DEFAULT);
};


#include"load_htm.exp"

lib_begin ( void ) {

  if ( ap_process == AP_ALLOC ) {

          AP_EXPORTLIB(); /* export table in app.exp */

  } else

  if ( ap_process == AP_INIT ) {

          AP_SETNUMOFCALLS(1); /* set max num of calls */

          html_load_fonts();

          pal_htmloader[0] = color_get_from_ini("htmloader_background");
          pal_htmloader[1] = color_get_from_ini("htmloader_text");
          pal_htmloader[2] = color_get_from_ini("htmloader_reference");
          pal_htmloader[3] = color_get_from_ini("htmloader_sel_reference");

          datfile = conv_to_skipcolor_data(DLXGetFileData(ap_id), CO_SKIP_RED, CO_SKIP_GREEN, CO_SKIP_BLUE);

  } else

  if ( ap_process == AP_FREE ) {

    unload_datafile(datfile);
    datfile = NULL;

  };


} lib_end;

