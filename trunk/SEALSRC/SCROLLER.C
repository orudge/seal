/****************************************************************/
/*                                                              */
/*                          scroller.c                          */
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
#include<math.h>

l_bool   scroll_after = true;

l_color  pal_scrollbar[] = {CO_SYSx20, CO_SYSx25, CO_SYSx20, CO_NOCOLOR};

p_scroller   (*scroller_init) ( p_scroller o, t_rect r, l_int flags ) = &_scroller_init;
p_vscrollbar (*vscrollbar_init) ( p_vscrollbar o, t_rect r, p_scroller f ) = &_vscrollbar_init;
p_hscrollbar (*hscrollbar_init) ( p_hscrollbar o, t_rect r, p_scroller f ) = &_hscrollbar_init;


/* vscrollbar functions */

void   vscrollbar_translate_event ( p_object o, t_event *event )
{

  p_vscrollbar vsb = VSCROLLBAR(o);

  RETVIEW(o, event);

  view_translate_event(o, event);

  if ( !vsb->scroller && vsb->scroller->deltax <= 0 )

    return; /* if hasn't scroller or deltax <= 0 */

  if ( event->type & EV_MOUSE && OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) {

      p_scroller   scr = SCROLLER(vsb->scroller);
      p_vscrollbar so = VSCROLLBAR(o);
      t_point ml   = VIEW(o)->get_local_point(VIEW(o), mouse->where);
      t_rect  s    = VIEW(o)->size_limits(VIEW(o));
      l_rect  size = vsb->get_bar_size(vsb);
      l_rect  pos  = vsb->get_bar_pos(vsb);
      l_long  maxy = 0;

      scr->max_moves(scr, 0, &maxy);

      if ( ml.y > pos && ml.y < pos + STANDARD_SCROLLBUTTON_SIZE ) { /* UP scrolling by 1 */

        /* UP scrolling by 1*/

        #define ok_scroll_up ((OBJECT(mouse)->state & MO_SF_MOUSELPRESS) && (scr->scrolly > 0))

        while ( ok_scroll_up ) {

          l_rect   pos_mouse;
          t_point  p;

          scr->scroll_place(scr, 0, -scr->deltay, (vsb->flags & SBF_SCROLL_AFTER_MOVE));  /* up scrolling */

          clear_event(event);

          o->get_event(o, event); /* get events */

          if ( !ok_scroll_up )

             scr->scroll_place(scr, 0, 0, SCROLL_NOW); /* scroll if SBF_SCROLL_AFTER_MOVE is set */

          pos_mouse  = vsb->get_bar_pos(vsb)+(STANDARD_SCROLLBUTTON_SIZE/2);
          p  = point_assign(s.a.x+rect_sizex(s)/2, pos_mouse);

          mouse->set_pos(mouse, VIEW(o)->get_global_point(VIEW(o), p));

        };

      } else

      if ( ml.y < pos + size &&
           ml.y > pos + size - STANDARD_SCROLLBUTTON_SIZE ) { /* DOWN scrolling by 1 */

        /* DOWN scrolling by 1*/

        #define ok_scroll_down ((OBJECT(mouse)->state & MO_SF_MOUSELPRESS) && (scr->scrolly < maxy))

        while ( ok_scroll_down ) {

          l_rect   size;
          l_rect   pos_mouse;
          t_point  p;

          scr->scroll_place(scr, 0, scr->deltay, (vsb->flags & SBF_SCROLL_AFTER_MOVE)); /* down scrolling */

          clear_event(event);

          o->get_event(o, event); /* get events */

          if ( !ok_scroll_down )

             scr->scroll_place(scr, 0, 0, SCROLL_NOW); /* scroll if SBF_SCROLL_AFTER_MOVE is set */

          size = vsb->get_bar_size(vsb);
          pos_mouse  = vsb->get_bar_pos(vsb)+size-(STANDARD_SCROLLBUTTON_SIZE/2);
          p  = point_assign(s.a.x+rect_sizex(s)/2, pos_mouse);

          mouse->set_pos(mouse, VIEW(o)->get_global_point(VIEW(o), p));

        };

      } else

      if ( ml.y > pos + STANDARD_SCROLLBUTTON_SIZE &&
           ml.y < pos + size - STANDARD_SCROLLBUTTON_SIZE ) { /* 2 sides scrolling */

         l_rect  oldy = ml.y;

         /* UPDOWN scrolling */

         #define ok_scroll_updown (!(OBJECT(mouse)->state & MO_SF_MOUSELUP))

         while ( ok_scroll_updown ) {

            if ( ml.y != oldy ) {  /*  */

                  scr->scroll_place(scr, 0, so->get_real_pos(so, ml.y)-so->get_real_pos(so, oldy), (vsb->flags & SBF_SCROLL_AFTER_MOVE));

            };

            oldy = ml.y;

            o->get_event(o, event); /* get events */

            if ( !ok_scroll_updown ) /* scroll after */

                  scr->scroll_place(scr, 0, 0, SCROLL_NOW); /* scroll if SBF_SCROLL_AFTER_MOVE is set */

            ml = VIEW(o)->get_local_point(VIEW(o), mouse->where);

         };

      };

  };

};


t_rect vscrollbar_size_limits ( p_view o )
{

  t_rect r = o->get_local_extent(o);

  return rect_assign(r.a.x+1, r.a.y+1, r.b.x-1, r.b.y-1);

};


t_point vscrollbar_size_minimum ( p_view o )
{

  return point_assign(MINIMUM_SCROLLBAR_WIDTH, MINIMUM_SCROLLBAR_SIZE);

};


void   vscrollbar_draw ( p_view o )
{
  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    o->background(o, out, rect_move(r, p.x, p.y));

    button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_BLACK), COLOR(CO_WHITE));

    VSCROLLBAR(o)->draw_bar(VSCROLLBAR(o));

  };

  o->end_of_paint(o, r);
};


void   vscrollbar_draw_bar ( p_vscrollbar o )
{
  p_view  vo = VIEW(o);
  t_rect  r = vo->size_limits(vo);
  t_rect  s = r;
  t_point p;

  l_rect  pos  = o->get_bar_pos(o);
  l_rect  size = o->get_bar_size(o);

  BITMAP *out;

  r = rect_assign(r.a.x, r.a.y+pos, r.b.x, min(r.b.y, r.a.y+pos+size));

  out = vo->begin_paint(vo, &p, r);

  if ( out ) {

    l_color bcolor = vo->get_color(vo, 1);

    l_int   minush = 8;
    l_int   plush  = 8;
    GrFONT *font   = get_font_in_size("Symbols", 8, 8);

    rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, bcolor);

    button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_WHITE), COLOR(CO_BLACK));

    draw_double_text(out, font, "f", -1, r.a.x+p.x+1, r.a.y+p.y,
                     r.b.x+p.x, r.a.y+p.y+minush, TX_ALIGN_CENTER, COLOR(CO_WHITE), COLOR(CO_SYSx16), 0);

    draw_double_text(out, font, "g", -1, r.a.x+p.x+1, r.b.y+p.y-plush,
                     r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTER, COLOR(CO_WHITE), COLOR(CO_SYSx16), 0);

  };

  vo->end_of_paint(vo, r);
};


l_rect vscrollbar_get_bar_size ( p_vscrollbar o )
{

  if ( o->scroller ) {

    l_rect  sizey = rect_sizey(VIEW(o)->size_limits(VIEW(o)));
    l_rect  sizey_scroller = rect_sizey(o->scroller->scroll_limits(o->scroller));
    l_long  maxy = 0;
    l_rect  size;

    o->scroller->scroll_size(o->scroller, NULL, &maxy);

    size = lmax(MINIMUM_SCROLLBOX_SIZE, (l_rect)((double)sizey*((double)sizey_scroller/(double)(lmax(1, maxy)))));

    return size;

  };

  return 0;

};


l_long  vscrollbar_get_real_pos ( p_vscrollbar o, l_rect pos )
{
  l_long i = 0;
  l_long m_x = 0;
  l_long m_y = 0;
  t_rect s = VIEW(o)->size_limits(VIEW(o));

  if ( !o->scroller ) return i;

  o->scroller->scroll_size(o->scroller, &m_x, &m_y);

  if ( rect_sizey(s) && m_y ) {

    double y = ((double)m_y / (double)max(1, rect_sizey(s))) * (double)pos;

    y = (ceil(y / max(1, o->scroller->deltay))*o->scroller->deltay);

    i = (l_long)y;

  };

  return i;

};


l_rect vscrollbar_get_bar_pos ( p_vscrollbar o )
{

  if ( o->scroller ) {

    l_rect  sizey = rect_sizey(VIEW(o)->size_limits(VIEW(o)));
    l_rect  sizey_scroller = rect_sizey(o->scroller->scroll_limits(o->scroller));
    l_long  maxy = 0;
    l_long  sy = o->scroller->scrolly;
    l_rect  pos;
    l_rect  size;
    l_int   dy = 0;

    o->scroller->scroll_size(o->scroller, NULL, &maxy);

    size = (l_rect)((double)sizey*((double)sizey_scroller/(double)(lmax(1, maxy))));

    if ( size < MINIMUM_SCROLLBOX_SIZE ) dy = MINIMUM_SCROLLBOX_SIZE-size;

    pos = (l_rect)((double)(sizey-dy)*((double)sy/(double)(max(1, maxy))));

    if ( pos+size > sizey ) pos = lmax(0, sizey-size);

    return pos;

  };

  return 0;

};



/* hscrollbar functions */

void   hscrollbar_translate_event ( p_object o, t_event *event )
{

  p_hscrollbar hsb = HSCROLLBAR(o);

  RETVIEW(o, event);

  view_translate_event(o, event);

  if ( !hsb->scroller && hsb->scroller->deltax <= 0 )

    return; /* if hasn't scroller or deltax <= 0 */

  if ( event->type & EV_MOUSE && OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) {

      p_scroller scr = SCROLLER(hsb->scroller);
      p_hscrollbar so = HSCROLLBAR(o);
      t_point ml   = VIEW(o)->get_local_point(VIEW(o), mouse->where);
      t_rect  s    = VIEW(o)->size_limits(VIEW(o));
      l_rect  size = hsb->get_bar_size(hsb);
      l_rect  pos  = hsb->get_bar_pos(hsb);
      l_long  maxx = 0;

      scr->max_moves(scr, &maxx, 0);

      if ( ml.x > pos && ml.x < pos + STANDARD_SCROLLBUTTON_SIZE ) { /* LEFT scrolling by 1 */

        /* LEFT scrolling by 1*/

        #define ok_scroll_left ((OBJECT(mouse)->state & MO_SF_MOUSELPRESS) && (scr->scrollx > 0))

        while ( ok_scroll_left ) {

          l_rect   pos_mouse;
          t_point  p;

          scr->scroll_place(scr, -scr->deltax, 0, (hsb->flags & SBF_SCROLL_AFTER_MOVE));  /* left scrolling */

          clear_event(event);

          o->get_event(o, event); /* get events */

          if ( !ok_scroll_left )

             scr->scroll_place(scr, 0, 0, SCROLL_NOW); /* scroll if SBF_SCROLL_AFTER_MOVE is set */

          pos_mouse  = hsb->get_bar_pos(hsb)+(STANDARD_SCROLLBUTTON_SIZE/2);
          p  = point_assign(pos_mouse, s.a.y+rect_sizey(s)/2);

          mouse->set_pos(mouse, VIEW(o)->get_global_point(VIEW(o), p));

        };

      } else

      if ( ml.x < pos + size &&
           ml.x > pos + size - STANDARD_SCROLLBUTTON_SIZE ) { /* RIGHT scrolling by 1 */

        /* RIGHT scrolling by 1*/

        #define ok_scroll_right ((OBJECT(mouse)->state & MO_SF_MOUSELPRESS) && (scr->scrollx < maxx))

        while ( ok_scroll_right ) {

          l_rect   size;
          l_rect   pos_mouse;
          t_point  p;

          scr->scroll_place(scr, scr->deltax, 0, (hsb->flags & SBF_SCROLL_AFTER_MOVE)); /* right scrolling */

          clear_event(event);

          o->get_event(o, event); /* get events */

          if ( !ok_scroll_right )

             scr->scroll_place(scr, 0, 0, SCROLL_NOW); /* scroll if SBF_SCROLL_AFTER_MOVE is set */

          size = hsb->get_bar_size(hsb);
          pos_mouse  = hsb->get_bar_pos(hsb)+size-(STANDARD_SCROLLBUTTON_SIZE/2);
          p  = point_assign(pos_mouse, s.a.y+rect_sizey(s)/2);

          mouse->set_pos(mouse, VIEW(o)->get_global_point(VIEW(o), p));

        };

      } else

      if ( ml.x > pos + STANDARD_SCROLLBUTTON_SIZE &&
           ml.x < pos + size - STANDARD_SCROLLBUTTON_SIZE ) { /* 2 sides scrolling */

         l_rect oldx = ml.x;

         /* LEFTRIGHT scrolling */

         #define ok_scroll_leftright (!(OBJECT(mouse)->state & MO_SF_MOUSELUP))

         while ( ok_scroll_leftright ) {

            if ( ml.x != oldx ) {  /*  */

                  scr->scroll_place(scr, so->get_real_pos(so, ml.x)-so->get_real_pos(so, oldx), 0, (hsb->flags & SBF_SCROLL_AFTER_MOVE));

            };

            oldx = ml.x;

            o->get_event(o, event); /* get events */

            if ( !ok_scroll_leftright ) /* scroll after */

                  scr->scroll_place(scr, 0, 0, SCROLL_NOW); /* scroll if SBF_SCROLL_AFTER_MOVE is set */

            ml = VIEW(o)->get_local_point(VIEW(o), mouse->where);

         };

      };

  };

};


t_rect hscrollbar_size_limits ( p_view o )
{

  t_rect r = o->get_local_extent(o);

  return rect_assign(r.a.x+1, r.a.y+1, r.b.x-1, r.b.y-1);

};


t_point hscrollbar_size_minimum ( p_view o )
{

  return point_assign(MINIMUM_SCROLLBAR_SIZE, MINIMUM_SCROLLBAR_WIDTH);

};


void   hscrollbar_draw ( p_view o )
{
  t_rect  r = o->get_local_extent(o);
  t_point p;

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

    o->background(o, out, rect_move(r, p.x, p.y));

    button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_BLACK), COLOR(CO_WHITE));

    HSCROLLBAR(o)->draw_bar(HSCROLLBAR(o));

  };

  o->end_of_paint(o, r);
};


void   hscrollbar_draw_bar ( p_hscrollbar o )
{
  p_view  vo = VIEW(o);
  t_rect  r = vo->size_limits(vo);
  t_point p;

  l_rect  pos  = o->get_bar_pos(o);
  l_rect  size = o->get_bar_size(o);

  BITMAP *out;

  r = rect_assign(r.a.x+pos, r.a.y, min(r.b.x, r.a.x+pos+size), r.b.y);

  out = vo->begin_paint(vo, &p, r);

  if ( out ) {

    l_color bcolor = vo->get_color(vo, 1);

    GrFONT *font   = get_font_in_size("Symbols", 8, 8);
    l_int   minusw = FONT_GETWIDTH(font, '!');
    l_int   plusw  = FONT_GETWIDTH(font, '"');

    if ( plusw > minusw ) minusw = plusw;

    rectfill(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, bcolor);

    button(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, COLOR(CO_WHITE), COLOR(CO_BLACK));

    draw_double_text(out, font, "e", -1, r.a.x+p.x, r.a.y+p.y,
                     r.a.x+p.x+minusw+1, r.b.y+p.y, TX_ALIGN_CENTER, COLOR(CO_WHITE), COLOR(CO_SYSx16), 0);

    draw_double_text(out, font, "b", -1, r.b.x+p.x-plusw-1, r.a.y+p.y,
                     r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTER, COLOR(CO_WHITE), COLOR(CO_SYSx16), 0);

  };

  vo->end_of_paint(vo, r);
};


l_rect hscrollbar_get_bar_size ( p_hscrollbar o )
{
  if ( o->scroller ) {

    l_rect  sizex = rect_sizex(VIEW(o)->size_limits(VIEW(o)));
    l_rect  sizex_scroller = rect_sizex(o->scroller->scroll_limits(o->scroller));
    l_long  maxx;
    l_rect  size;

    o->scroller->scroll_size(o->scroller, &maxx, NULL);

    size = lmax(MINIMUM_SCROLLBOX_SIZE, (l_rect)((double)sizex*((double)sizex_scroller/(double)(max(1, maxx)))));

    return size;

  };

  return 0;

};


l_long  hscrollbar_get_real_pos ( p_hscrollbar o, l_rect pos )
{
  l_long i = 0;
  l_long m_x = 0;
  l_long m_y = 0;
  t_rect s = VIEW(o)->size_limits(VIEW(o));

  if ( !o->scroller ) return i;

  o->scroller->scroll_size(o->scroller, &m_x, &m_y);

  if ( rect_sizex(s) && m_x ) {

    double x = ((double)m_x / max(1, (double)rect_sizex(s))) * (double)pos;

    x = (ceil(x / max(1, o->scroller->deltax))*o->scroller->deltax);

    i = (l_long)x;

  };

  return i;
};


l_rect hscrollbar_get_bar_pos ( p_hscrollbar o )
{

  if ( o->scroller ) {

    l_rect  sizex = rect_sizex(VIEW(o)->size_limits(VIEW(o)));
    l_rect  sizex_scroller = rect_sizex(o->scroller->scroll_limits(o->scroller));
    l_long  maxx;
    l_rect  pos;
    l_rect  size;
    l_int   dx = 0;

    o->scroller->scroll_size(o->scroller, &maxx, NULL);

    size = (l_rect)((double)sizex*((double)sizex_scroller/(double)(max(1, maxx))));

    if ( size < MINIMUM_SCROLLBOX_SIZE ) dx = MINIMUM_SCROLLBOX_SIZE-size;

    pos = (l_rect)((double)(sizex-dx)*((double)o->scroller->scrollx/(double)(max(1, maxx))));

    if ( pos+size > sizex ) pos = lmax(0, sizex-size);

    return pos;

  };

  return 0;

};



/* scroller functions */

l_bool   scroller_done ( p_object o )
{

  if ( SCROLLER(o)->verbar )

    SCROLLER(o)->verbar = VSCROLLBAR(dispose(OBJECT(SCROLLER(o)->verbar)));

  if ( SCROLLER(o)->horbar )

    SCROLLER(o)->horbar = HSCROLLBAR(dispose(OBJECT(SCROLLER(o)->verbar)));


  return view_done(o);

};


void   scroller_setup ( p_object o )
{
  p_object owner = NULL;

  view_setup(o);

  owner = o->owner_view(o);

  if ( owner ) { /* if owner_view exist */

    if ( SCROLLER(o)->flags & SF_HORSCROLLBAR &&
        !SCROLLER(o)->horbar ) { /* horizontal scrollbar */

         t_rect r = VIEW(o)->bounds;

         r = rect_assign(r.a.x, r.b.y+1, r.b.x, r.b.y+1+STANDARD_SCROLLBAR_WIDTH);

         SCROLLER(o)->horbar = hscrollbar_init((p_hscrollbar)_malloc(sizeof(t_hscrollbar)), r, SCROLLER(o));

         if ( SCROLLER(o)->horbar )  /* set to top_select if listbox was set too */

               OBJECT(SCROLLER(o)->horbar)->set_options(OBJECT(SCROLLER(o)->horbar), OB_OF_TOPSELECT, o->is_options(o, OB_OF_TOPSELECT));

         owner->insert(owner, OBJECT(SCROLLER(o)->horbar));

    };

    if ( SCROLLER(o)->flags & SF_VERSCROLLBAR &&
        !SCROLLER(o)->verbar ) { /* vertical scrollbar */

         t_rect r = VIEW(o)->bounds;

         r = rect_assign(r.b.x+1, r.a.y, r.b.x+1+STANDARD_SCROLLBAR_WIDTH, r.b.y);

         SCROLLER(o)->verbar = vscrollbar_init((p_vscrollbar)_malloc(sizeof(t_vscrollbar)), r, SCROLLER(o));

         if ( SCROLLER(o)->verbar ) /* set to top_select if listbox was set too */

            OBJECT(SCROLLER(o)->verbar)->set_options(OBJECT(SCROLLER(o)->verbar), OB_OF_TOPSELECT, o->is_options(o, OB_OF_TOPSELECT));

         owner->insert(owner, OBJECT(SCROLLER(o)->verbar));

    };

  };

};


void   scroller_change_bounds ( p_view o, t_rect nr )
{

  view_change_bounds(o, nr);

  if ( SCROLLER(o)->horbar ) {

    p_view hb = VIEW(SCROLLER(o)->horbar);
    t_rect r  = o->bounds;

    hb->change_bounds(hb, rect_assign(r.a.x, r.b.y+1, r.b.x, r.b.y+1+STANDARD_SCROLLBAR_WIDTH));

  };

  if ( SCROLLER(o)->verbar ) {

    p_view vb = VIEW(SCROLLER(o)->verbar);
    t_rect r  = o->bounds;

    vb->change_bounds(vb, rect_assign(r.b.x+1, r.a.y, r.b.x+1+STANDARD_SCROLLBAR_WIDTH, r.b.y));

  };

};


void   scroller_background ( p_view o, BITMAP *out, t_rect r )
{
  if ( o->brush.background ) {

    t_rect i = {0, 0, IMAGE_WIDTH(o->brush.background),
                      IMAGE_HEIGHT(o->brush.background)};

    r.a.x -= SCROLLER(o)->scrollx;
    r.a.y -= SCROLLER(o)->scrolly;

    if ( o->brush.state & BRUSH_STRETCH )

      stretch_sprite(out, o->brush.background, r.a.x, r.a.y, rect_sizex(r)+1, rect_sizey(r)+1);

    else

    if ( o->brush.state & BRUSH_CENTER ) {

      l_rect dx = (rect_sizex(r)-IMAGE_WIDTH(o->brush.background))/2;
      l_rect dy = (rect_sizey(r)-IMAGE_HEIGHT(o->brush.background))/2;

      rectfill(out, r.a.x, r.a.y, r.b.x, r.b.y, o->brush.color);

      draw_sprite(out, o->brush.background, r.a.x+dx, r.a.y+dy);

    } else

      blit_ex(o->brush.background, out, i.a.x, i.a.y, r.a.x, r.a.y,
              IMAGE_WIDTH(o->brush.background), IMAGE_HEIGHT(o->brush.background),
              rect_sizex(r)+1, rect_sizey(r)+1);

  } else {

    if (  o->brush.color == o->brush.color2 ||
        !(o->brush.state & BRUSH_GRADIENT) )

       rectfill(out, r.a.x, r.a.y, r.b.x, r.b.y, o->brush.color);

    else

       if ( o->brush.state & BRUSH_GRADIENT_HOR )

          fade_rect(out, r.a.x, r.a.y, r.b.x, r.b.y, o->brush.color, o->brush.color2, FR_HOR);

       else

          fade_rect(out, r.a.x, r.a.y, r.b.x, r.b.y, o->brush.color, o->brush.color2, FR_VER);

  };

};


t_rect scroller_scroll_limits ( p_scroller o )
{

  return VIEW(o)->size_limits(VIEW(o));

};


void   scroller_recalc_positions ( p_scroller o, l_long x, l_long y )
{
};


void   scroller_scroll_size ( p_scroller o, l_long *x, l_long *y )
{
  t_rect r = o->scroll_limits(o);

  if ( x ) *(x) = rect_sizex(r);
  if ( y ) *(y) = rect_sizey(r);

};


static l_bool self_rect ( t_rect r, p_view _this )
{

  p_view  p = _this;
  p_view  t = NULL;

  while ( p ) {

    t_rect v = r;

    v = rect_move(r, p->bounds.a.x, p->bounds.a.y);

    t = VIEW(OBJECT(p)->owner_view(OBJECT(p)));

    if ( t ) {

      t = VIEW(OBJECT(t)->first_view(OBJECT(t)));

      while ( t != p ) {

        if ( OBJECT(t)->is_state(OBJECT(t), OB_SF_VISIBLE) &&
             rect_overlay(t->bounds, v) )

          return false;

        t = VIEW(OBJECT(t)->next_view(OBJECT(t)));

      };

    };

    p = VIEW(OBJECT(p)->owner_view(OBJECT(p)));

  };

  return true;
};


/*
   scroll place.

   function scroll place of object (o), by delta x and delta y.

   dx - horizontal move
   dy - vertical move
   aft_move - if 0, than place is redraw now, if non-zero place is redraw
              when aft_move is set to 0, it only move coordinates. Then call
              scroll_place by 0, 0, SCROLL_NOW arguments. It's for
              slowly redrawing of place ( when accelerated functions
              of blitting are not support for video card ).

   - it also redraw horizontal and vertical scrollbars.

   - area to be scrolled is defined in scroll_limits function,
     that call size_limits function as default. (see t_view structure)

   - when object is scrolled, function recalc_positions is called for this
     object by arguments dx and dy. This function may set o->scrollx and
     o->scrolly to rounded values for specific object.

  - this function is supported in horizontal & vertical scrollbars.

*/

void   scroller_scroll_place ( p_scroller o, l_long dx, l_long dy, l_bool aft_move )
{
  t_rect  s;
  t_rect  t;
  l_long  sx;
  l_long  sy;
  l_long  maxx;
  l_long  maxy;

  l_long  old_scrollx = o->scrollx;
  l_long  old_scrolly = o->scrolly;

  p_view  p = NULL;

  if ( !dx && !dy && !o->aftmovex && !o->aftmovey ) return;

  o->scroll_size(o, &sx, &sy);
  o->max_moves(o, &maxx, &maxy);

  s = VIEW_CLIPED(o, o->scroll_limits(o));

  if ( o->scroll_auto & 0x01 )

     o->scrollx = lmax(0, lmin(maxx, o->scrollx+dx));

  else

     o->scrollx += dx;

  if ( o->scroll_auto & 0x02 )

     o->scrolly = lmax(0, lmin(maxy, o->scrolly+dy));

  else

     o->scrolly += dy;


  dx = o->scrollx - old_scrollx;

  dy = o->scrolly - old_scrolly;

  if ( !dx && !dy && !o->aftmovex && !o->aftmovey ) return;

  if ( aft_move ) { /* redraw after move */

    o->aftmovex += dx;
    o->aftmovey += dy;

    o->recalc_positions(o, dx, dy);

    if ( dx )

       VIEW(o->horbar)->draw_view(VIEW(o->horbar));

    if ( dy )

       VIEW(o->verbar)->draw_view(VIEW(o->verbar));

    return;

  } else {

    o->recalc_positions(o, dx, dy);

    if ( o->aftmovex ) dx = o->aftmovex;
    if ( o->aftmovey ) dy = o->aftmovey;

    o->aftmovex = 0;
    o->aftmovey = 0;

  };


  p = VIEW(OBJECT(o)->first_view(OBJECT(o)));

  while ( p ) {

    t = p->bounds;
    t = rect_move(t, -dx, -dy);

    p->set_bounds(p, t);

    p = VIEW(OBJECT(p)->next_view_to_last(OBJECT(p)));

  };

  if ( labs(dx) > rect_sizex(s) || labs(dy) > rect_sizey(s) ) {

    VIEW(o)->draw_in_rect(VIEW(o), s);

    if ( dx )

       VIEW(o->horbar)->draw_view(VIEW(o->horbar));

    if ( dy )

       VIEW(o->verbar)->draw_view(VIEW(o->verbar));

    return;

  };

  t = rect_move(s, -dx, -dy);

  if ( self_rect(s, VIEW(o)) ) {

     t_point pt = VIEW(o)->get_global_point(VIEW(o), point_assign(0, 0));

     t_rect  d = rect_assign(pt.x+s.a.x, pt.y+s.a.y, pt.x+s.b.x+1, pt.y+s.b.y+1);

     l_int   i = 0;
     l_int   i2 = 0;

     VIEW(o)->set_clips(VIEW(o), s);

     /* blit function */

     i  = mouse->block(mouse, d);

     blit(OUTBUFFER(VIEW(o)->draw_out), OUTBUFFER(VIEW(o)->draw_out), pt.x+s.a.x, pt.y+s.a.y, pt.x+t.a.x, pt.y+t.a.y, rect_sizex(s)+1, rect_sizey(s)+1);

     mouse->unblock(mouse, i);

     VIEW(o)->reset_clips(VIEW(o));

     rect_double_overlay(&t, &s);

     VIEW(o)->draw_in_rect(VIEW(o), t);
     VIEW(o)->draw_in_rect(VIEW(o), s);

  } else {

    TEST_SUB_VIEWS(o, VIEW(o)->draw_in_rect(VIEW(o), s));

  };

  if ( dx )

     VIEW(o->horbar)->draw_view(VIEW(o->horbar));

  if ( dy )

     VIEW(o->verbar)->draw_view(VIEW(o->verbar));

};


void   scroller_max_moves ( p_scroller o, l_long *dx, l_long *dy )
{
  t_rect l = o->scroll_limits(o);
  l_long sx = rect_sizex(l);
  l_long sy = rect_sizey(l);

  o->scroll_size(o, &sx, &sy);

  if ( dx ) *(dx) = calc_maxscrollx(o, sx-rect_sizex(l));
  if ( dy ) *(dy) = calc_maxscrolly(o, sy-rect_sizey(l));
};




/* hscrollbar */

/*
  object hscrollbar

  - this object is used for controling scroll boxes in horizontal coordinates.

  Arguments:

  r   - rect of hscrollbar, default height is STANDARD_SCROLLBAR_WIDTH defined
        in scroller.h file.

  f   - pointer to object, that scroll box. ( t_listbox, ... )

  If object is pressed to right, it run f->scroll_place(f, ax, 0, 0)
  function, where ax is f->deltax, if to the left ax is set to -f->deltax.

  note1 : dx and dy in scroll_place function is control by f->recalc_positions
          function for specific object. [ see dialogs.c ( t_listbox ) ]

  NOTE2:

  if you are using t_scroller initialization by (flag & SF_HORSCROLLBAR)
  this object is automatically added to window by f = pointer to your
  t_scroller.

*/

p_hscrollbar   _hscrollbar_init ( p_hscrollbar o, t_rect r, p_scroller f )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_hscrollbar));

  view_init(VIEW(o), r);


  /* object's functions */

  OBJECT(o)->translate_event = &hscrollbar_translate_event;


  /* view's functions */

  VIEW(o)->draw = &hscrollbar_draw;
  VIEW(o)->size_limits = &hscrollbar_size_limits;
  VIEW(o)->size_minimum = &hscrollbar_size_minimum;


  /* hscrollbar's functions and declarations */

  o->scroller = f;

  if ( scroll_after ) o->flags |= SBF_SCROLL_AFTER_MOVE;

  o->draw_bar = &hscrollbar_draw_bar;
  o->get_bar_size = &hscrollbar_get_bar_size;
  o->get_bar_pos = &hscrollbar_get_bar_pos;
  o->get_real_pos = &hscrollbar_get_real_pos;


  /* function calls */

  OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE, false);

  VIEW(o)->set_palette(VIEW(o), pal_scrollbar);

  VIEW(o)->brush.color = VIEW(o)->get_color(VIEW(o), 0);
  VIEW(o)->brush.color2 = VIEW(o)->get_color(VIEW(o), 2);
  VIEW(o)->brush.state |= BRUSH_GRADIENT;

  return o;

};



/* vscrollbar */

/*
  object vscrollbar

  - this object is used for controling scroll boxes in horizontal coordinates.

  Arguments:

  r   - rect of vscrollbar, default width is STANDARD_SCROLLBAR_WIDTH defined
        in scroller.h file.

  f   - pointer to object, that scroll box. ( t_listbox, ... )

  If object is pressed up, it run f->scroll_place(f, 0, ay, 0)
  function, where ay is f->deltay, if down ay is set to -f->deltay.

  note1 : dx and dy in scroll_place function is control by f->recalc_positions
          function for specific object. [ see dialogs.c ( t_listbox ) ]

  NOTE2:

  if you are using t_scroller initialization by (flag & SF_VERSCROLLBAR)
  this object is automatically added to window by f = pointer to your
  t_scroller.

*/
p_vscrollbar   _vscrollbar_init ( p_vscrollbar o, t_rect r, p_scroller f )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_vscrollbar));

  view_init(VIEW(o), r);


  /* object's functions */

  OBJECT(o)->translate_event = &vscrollbar_translate_event;


  /* view's functions */

  VIEW(o)->draw = &vscrollbar_draw;
  VIEW(o)->size_limits = &vscrollbar_size_limits;
  VIEW(o)->size_minimum = &vscrollbar_size_minimum;


  /* vscrollbar's functions and declarations */

  o->scroller = f;
  if ( scroll_after ) o->flags |= SBF_SCROLL_AFTER_MOVE;

  o->draw_bar = &vscrollbar_draw_bar;
  o->get_bar_size = &vscrollbar_get_bar_size;
  o->get_bar_pos = &vscrollbar_get_bar_pos;
  o->get_real_pos = &vscrollbar_get_real_pos;


  /* function calls */

  OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE, false);

  VIEW(o)->set_palette(VIEW(o), pal_scrollbar);

  VIEW(o)->brush.color = VIEW(o)->get_color(VIEW(o), 0);
  VIEW(o)->brush.color2 = VIEW(o)->get_color(VIEW(o), 2);
  VIEW(o)->brush.state |= BRUSH_GRADIENT+BRUSH_GRADIENT_HOR;

  return o;

};



/* scroller */

p_scroller   _scroller_init ( p_scroller o, t_rect r, l_int flags )
{

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_scroller));

  view_init(VIEW(o), r);

  /* object's functions */

  OBJECT(o)->setup = &scroller_setup;
  OBJECT(o)->done = &scroller_done;


  /* view's functions */

  VIEW(o)->background = &scroller_background;
  VIEW(o)->change_bounds = &scroller_change_bounds;


  /* scroller's functions and declarations */

  o->scrollx = 0;
  o->scrolly = 0;
  o->deltax = 5;
  o->deltay = 5;

  o->flags  = flags;

  o->scroll_auto = 0x03;

  o->scroll_limits = &scroller_scroll_limits;
  o->scroll_size = &scroller_scroll_size;
  o->scroll_place = &scroller_scroll_place;
  o->max_moves = &scroller_max_moves;
  o->recalc_positions = &scroller_recalc_positions;

  return o;

};


#include"scroller.exp"

lib_begin ( void ) {

  if ( ap_process == AP_ALLOC ) {

          AP_EXPORTLIB(); /* export table in app.exp */

  } else

  if ( ap_process == AP_INIT ) {

          AP_SETNUMOFCALLS(1); /* set max num of calls */

          pal_scrollbar[0] = color_get_from_ini("scrollbar_background");
          pal_scrollbar[1] = color_get_from_ini("scrollbar_bar");
          pal_scrollbar[2] = color_get_from_ini("scrollbar_background_gradient");

          scroll_after = !getininum_fromfile(INI_MAINFILE, INI_OPTIMALIZATION, "scroll_nice");

  } else

  if ( ap_process == AP_FREE ) {


  };


} lib_end;

