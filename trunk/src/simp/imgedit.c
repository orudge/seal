
#ifndef moy3
#define moy3( a, b, c )  ( a + b + c ) / 3
#endif

#ifndef is_b
#define is_b( a, b, c )  ( a < b  &&  b < c )
#endif

#include<seal.h>
#include<grfx-f.h>
#include"imgedit.h"
#include<inputbox.h>
#include<allegro/aintern.h>
#include"grfx-c.c"

/*

(c) Copyright 2001 Julien Etelain. All rights reserved.

*/

BITMAP *img_bmps[11];

#define IMGEDIT_COLSEL_IMG img_bmps[0]
#define TOOL_ZOOM_CUR      img_bmps[1]
#define TOOL_GET_CUR       img_bmps[2]
#define TOOL_POINT_CUR     img_bmps[3]
#define TOOL_FILL_CUR      img_bmps[4]
#define TOOL_TEXT_CUR      img_bmps[5]
#define TOOL_SELECT_CUR    img_bmps[6]
#define FORME_RECT_CUR     img_bmps[7]
#define FORME_LINE_CUR     img_bmps[8]
#define FORME_OVAL_CUR     img_bmps[9]
#define FORME_CIRCLE_CUR   img_bmps[10]

p_list      imgfilters = NULL;

p_colsel2   (*colsel2_init) ( p_colsel2 o, t_rect r ) = &_colsel2_init;

p_imgedit   (*imgedit_init) ( p_imgedit o, t_rect r, p_colsel2 cc, p_stattext st ) = &_imgedit_init;

////////////////////////////////////////////////////////////////////////////////
void  free_undoimgedit ( void *o ) {
 if ( o ) {
    DEBUG_printf ( "\nStart Free a Undo {\n");
    byebyebmp(undoimgedit(o)->picture);
    byebyebmp(undoimgedit(o)->selection);
    byebyebmp(undoimgedit(o)->temp);
    undoimgedit(o)->select = NULL;
    undoimgedit(o)->actsel = rect_assign(NULL,NULL,NULL,NULL);
    DEBUG_printf ( "}Free Ok;\n");
    _free(o);
  };
};
////////////////////////////////////////////////////////////////////////////////
void  free_imgfilter ( void *o ) {
  if ( o ) {
    if (imgfilter(o)->copyright)   _free(imgfilter(o)->copyright);
    if (imgfilter(o)->name)        _free(imgfilter(o)->name);
    if (imgfilter(o)->description) _free(imgfilter(o)->description);
    _free(o);
  };
};

////////////////////////////////////////////////////////////////////////////////

p_imgfilter  new_imgfilter(   void    (*smallpreview) ( BITMAP *bmp ),
                              void    (*filterbox)    ( BITMAP *bmp ),
                              l_text  copyright,
                              l_text  name,
                              l_text  description ) {

  p_imgfilter i = imgfilter(_malloc(sizeof(t_imgfilter)));

  if ( i ) {

    i->name         = _strdup(name);
    i->description  = _strdup(description);
    i->copyright    = _strdup(copyright);
    i->smallpreview = smallpreview;
    i->filterbox    = filterbox;

  };

  return i;

};

////////////////////////////////////////////////////////////////////////////////

void add_imgfilter(   void    (*smallpreview) ( BITMAP *bmp ),
                      void    (*filterbox)    ( BITMAP *bmp ),
                      l_text  copyright,
                      l_text  name,
                      l_text  description ) {

imgfilters->insert( imgfilters , new_imgfilter ( smallpreview, filterbox, copyright, name, description ) );
};

////////////////////////////////////////////////////////////////////////////////

p_imgfilter getimgfilter ( l_int id ) {

return list_at(imgfilters, id) ;

};

////////////////////////////////////////////////////////////////////////////////

void greylevelit (BITMAP *bmp) {
  l_uint x = 0;
  l_uint y = 0;
  while ( y < bmp->h ) {
    x=0;
    while ( x < bmp->w ) {
      l_int col = getpixel(bmp, x, y);
      l_int z = moy3( getr(col),getg(col),getb(col) );
      if ( col != bmp->vtable->mask_color ) putpixel(bmp, x, y, makecol(z, z, z));
      x++;
    };
    y++;
  };
};
////////////////////////////////////////////////////////////////////////////////
void invertit (BITMAP *bmp) {
  l_uint x = 0;
  l_uint y = 0;
  while ( y < bmp->h ) {
    x=0;
    while ( x < bmp->w ) {
      l_int col = getpixel(bmp, x, y);
      if ( col != bmp->vtable->mask_color ) putpixel(bmp, x, y, makecol(255-getr(col), 255-getg(col), 255-getb(col)));
      x++;
    };
    y++;
  };
};
////////////////////////////////////////////////////////////////////////////////

l_color adoucir ( BITMAP *bmp, l_int x, l_int y, l_int n, l_int max, l_int mode ) {
  #define gocol(x) if ( x != bmp->vtable->mask_color ) { rs += getr(x); gs += getg(x); bs += getb(x); nb++; };
  l_color  col = getpixel(bmp, x, y);
  l_int w = IMAGE_WIDTH ( bmp );
  l_int h = IMAGE_HEIGHT ( bmp );
  l_int rs = 0;
  l_int gs = 0;
  l_int bs = 0;
  l_int nb = 0;

  if ( x != 0 ) gocol(getpixel(bmp, x-1, y))
  if ( x != w ) gocol(getpixel(bmp, x+1, y))
  if ( y != 0 ) gocol(getpixel(bmp, x, y-1))
  if ( y != h ) gocol(getpixel(bmp, x, y+1))
  if ( x != w && y != h ) gocol(getpixel(bmp, x+1, y+1))
  if ( x != w && y != 0 ) gocol(getpixel(bmp, x+1, y-1))
  if ( x != 0 && y != h ) gocol(getpixel(bmp, x-1, y+1))
  if ( x != 0 && y != 0 ) gocol(getpixel(bmp, x-1, y-1))

  if ( col != bmp->vtable->mask_color ) {
    if (mode == 1 )
      return col2col(col,makecol(rs/nb,gs/nb,bs/nb),n,max);
    else
      return colNcol(col,makecol(rs/nb,gs/nb,bs/nb),n,max);
  } else
    return bmp->vtable->mask_color;
};
////////////////////////////////////////////////////////////////////////////////
void soft_it (BITMAP *bmp) {
  BITMAP *dest = create_bitmap(bmp->w,bmp->h);
  l_int x = 0;
  l_int y = 0;
  l_int n = 50;
  l_int max = 100;
  while ( y < bmp->h ) {
    x = 0;
    while ( x < bmp->w ) {
      putpixel(dest, x, y, adoucir(bmp,x,y,n,max,1));
      x++;
    };
    y++;
  };
  draw_sprite(bmp,dest ,0,0);
  destroy_bitmap(dest);
};
////////////////////////////////////////////////////////////////////////////////
void hard_it (BITMAP *bmp) {
  BITMAP *dest = create_bitmap(bmp->w,bmp->h);
  l_int x = 0;
  l_int y = 0;
  l_int n = 50;
  l_int max = 100;
  while ( y < bmp->h ) {
    x = 0;
    while ( x < bmp->w ) {
      putpixel(dest, x, y, adoucir(bmp,x,y,n,max,2));
      x++;
    };
    y++;
  };
  draw_sprite(bmp,dest ,0,0);
  destroy_bitmap(dest);
};
////////////////////////////////////////////////////////////////////////////////

void init_imgfilters ( ) {
  DEBUG_printf("\nInit image's filters\n");

  imgfilters = list_init(_malloc(sizeof(t_list)), &free_imgfilter, 0);

  add_imgfilter ( greylevelit, greylevelit,"(c) Copyright 2001 Julien Etelain. All rights reserved","Greys levels","Change picture in greys levels ");

  add_imgfilter ( soft_it, soft_it,"(c) Copyright 2001 Julien Etelain. All rights reserved","Softer","'Soft' the picture");
  add_imgfilter ( hard_it, hard_it,"(c) Copyright 2001 Julien Etelain. All rights reserved","Harder","'Hard' the picture");
  add_imgfilter ( invertit, invertit,"(c) Copyright 2001 Julien Etelain. All rights reserved","Invert","Invert the picture colors");

  DEBUG_printf("\n - %d Filters loaded.\nImage's Filters init done ...\n\n",list_get_max_item(imgfilters) + 1);
};

////////////////////////////////////////////////////////////////////////////////
void   colsel2_draw ( p_view o ) {
  t_rect  r = o->get_local_extent(o);
  t_point p;
  BITMAP *out = o->begin_paint(o, &p, r);
  if ( out ) {
    stretch_sprite(out, IMGEDIT_COLSEL_IMG, p.x, p.y, r.b.x, r.b.y - 10);
    if ( colsel2(o)->imgedit ) {
      rectfill(out, p.x, p.y + r.b.y - 10, p.x  + (r.b.x / 2), p.y + r.b.y, colsel2(o)->imgedit->col1);
      rectfill(out, p.x + (r.b.x / 2), p.y + r.b.y - 10, p.x + r.b.x, p.y + r.b.y, colsel2(o)->imgedit->col2);
      if ( colsel2(o)->imgedit->picture ) {
        if ( colsel2(o)->imgedit->col2 == colsel2(o)->imgedit->picture->vtable->mask_color )
          textout_draw_rect(out, font_system, "(trans)", -1, p.x + (r.b.x / 2), p.y + r.b.y - 10, p.x + r.b.x, p.y + r.b.y, TX_ALIGN_CENTER, COLOR(CO_BLACK),TX_NOCOLOR, 0);
        if ( colsel2(o)->imgedit->col1 == colsel2(o)->imgedit->picture->vtable->mask_color )
          textout_draw_rect(out, font_system, "(trans)", -1, p.x, p.y + r.b.y - 10, p.x  + (r.b.x / 2), p.y + r.b.y, TX_ALIGN_CENTER, COLOR(CO_BLACK), makecol(255,0,255), 0);
      };
    };
  };
  o->end_of_paint(o, r);
};

////////////////////////////////////////////////////////////////////////////////

void   colsel2_translate_event ( p_object o, t_event *event )
{

  RETVIEW(o, event); // Not yet !

    if ( event->type & EV_MOUSE ) {  // MO_SF_MOUSELDOWN
      if ( colsel2(o)->imgedit ) {
      p_view xo = VIEW(o);
      l_int ok = 0;
      t_point p2 = xo->get_local_point(xo, mouse->where);

     if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN  ) ok = 1;
     if ( OBJECT(mouse)->state & MO_SF_MOUSERDOWN  ) ok = 2;

     if (ok != 0) {

if ( p2.x > 0 && p2.x < rect_sizex(xo->bounds) &&  p2.y > 0 && p2.y < rect_sizey(xo->bounds) - 10 )  {

  t_rect  r = xo->get_local_extent(xo);
  t_point p;
  t_point pe =  VIEW(desktop)->get_local_point(VIEW(desktop), mouse->where);
  BITMAP *out = xo->begin_paint(xo, &p, r);
  l_color x = getpixel(out, pe.x, pe.y);
  if ( ok == 1 ) colsel2(o)->imgedit->col1 = x;
  if ( ok == 2 ) colsel2(o)->imgedit->col2 = x;

  xo->end_of_paint(xo, r);
  xo->draw_me(xo);
} else if ( p2.x > 0 && p2.x < rect_sizex(xo->bounds) / 2 &&  p2.y > rect_sizey(xo->bounds) - 10 && p2.y < rect_sizey(xo->bounds)  )  {
  colsel2(o)->imgedit->col1 = rgbcolorbox ( colsel2(o)->imgedit->col1 );
  xo->draw_me(xo);
} else if ( p2.x > rect_sizex(xo->bounds) / 2 && p2.x < rect_sizex(xo->bounds) &&  p2.y > rect_sizey(xo->bounds) - 10 && p2.y < rect_sizey(xo->bounds)  )  {
  colsel2(o)->imgedit->col2 = rgbcolorbox ( colsel2(o)->imgedit->col2 );
  xo->draw_me(xo);
};
     };
};
};
};


////////////////////////////////////////////////////////////////////////////////
void imgedit_reset ( p_imgedit o ) {
  o->zoom       = 1;
  o->editing    = 0;
  o->topx       = 0;
  o->topy       = 0;
  o->select     = 0;
  o->actsel     = rect_assign(0,0,0,0);
  byebyebmp(o->selection);
  byebyebmp(o->temp);
  byebyebmp(o->picture);

  if ( o->filename ) _free (o->filename);
  o->filename   = _strdup("New Image");

  if ( o->undolist )
    o->undolist->free_all(o->undolist);

  o->undolist   = list_init(_malloc(sizeof(t_list)), &free_undoimgedit, 0);

  o->pr         = 0;
  o->floodstyle = 0;
  o->set1       = 50;
  o->set2       = 50;
  o->floodtol   = 0;
};

////////////////////////////////////////////////////////////////////////////////

void statext_update ( p_stattext o, l_text text, ...) {

  va_list arg;
  va_start(arg, text);

  vsprintf(o->text, text, arg);

  va_end(arg);

  VIEW(o)->draw_me(VIEW(o));

};
////////////////////////////////////////////////////////////////////////////////
BITMAP *make_bmp_with_text ( l_text txt, l_color color, l_font f ) {
  l_int     w =  text_length  ( (FONT*)f, txt ) + 4;
  l_int     h =  text_height  ( (FONT*)f );
  BITMAP *bmp =  create_bitmap   ( w, h );
  rectfill( bmp, 0, 0, w + 4, h + 2, makecol( 255, 0, 255 ) );
  textout_draw_rect(bmp, f, txt, -1, 2, 0, w + 2, h, TX_ALIGN_DEFAULT, color, makecol( 255, 0, 255 ), 0);
  return bmp;
};
////////////////////////////////////////////////////////////////////////////////
BITMAP *bmpdup ( BITMAP *bmp ) {
  if ( bmp ) {
    BITMAP *m = create_bitmap(bmp->w,bmp->h);
    if ( m ) {
      blit(bmp,m,0,0,0,0,bmp->w,bmp->h);
      return m;
    };
    return NULL;
  };
  return NULL;
};
#define mk_copy_of bmpdup
////////////////////////////////////////////////////////////////////////////////

void imgedit_undo_add ( p_imgedit o ) {
/*p_undoimgedit u = undoimgedit(_malloc(sizeof(t_undoimgedit)));

u->select    = o->select;
u->actsel    = o->actsel;
u->selection = mk_copy_of(o->selection);
u->picture   = mk_copy_of(o->picture);
u->temp      = mk_copy_of(o->temp);

list_insert (o->undolist, u );   */

};
////////////////////////////////////////////////////////////////////////////////
void imgedit_undo ( p_imgedit o ) {
/*if ( o->undolist->last ) {
l_int x = o->undolist->get_max_item(o->undolist);
p_undoimgedit u = undoimgedit( o->undolist->at( o->undolist, x ) );

o->select    = u->select ;
o->actsel    = u->actsel;
o->selection = mk_copy_of(u->selection);
o->picture   = mk_copy_of(u->picture);
o->temp      = mk_copy_of(u->temp);

o->undolist->free_index( o->undolist, x );
VIEW(o)->draw_me(VIEW(o));
};*/
};
////////////////////////////////////////////////////////////////////////////////
void imgedit_pcopy_act ( p_imgedit o, l_int action ) {

  if ( action == PCOPY_COPY ) {

    clear_type(&clipboard,sizeof(t_data));

    clipboard.info_obj = OBJECT(o);
    l_tag_cpy(clipboard.id, DAT_IMAGE);

    if ( !o->select ) {

      clipboard.data = (void *)bmpdup(o->picture);

    } else {

      clipboard.data = (void *)bmpdup(o->selection);

    };

  } else if (action == PCOPY_PASTASNEWPIC ) {
    if ( l_tag_cmp(DAT_IMAGE, clipboard.id) ) {
      imgedit_reset( o );

      o->picture = mk_copy_of(clipboard.data);

      VIEW(o)->draw_me(VIEW(o));
    };
  } else if (action == PCOPY_PASTASNEWSEL ) {
    if ( l_tag_cmp(DAT_IMAGE, clipboard.id) ) {

      imgedit_make_it_sel ( o, bmpdup(clipboard.data), 0, 0 );

      VIEW(o)->draw_me(VIEW(o));
    };
  };

};
////////////////////////////////////////////////////////////////////////////////
void imgedit_make_it_sel ( p_imgedit o, BITMAP *bmp, l_int x , l_int y ) {
  t_rect r = rect_assign( x, y, x + bmp->w, y + bmp->h );
  if ( o->selection ) destroy_bitmap(o->selection);
  if ( o->temp )      destroy_bitmap(o->temp);
  o->temp = bmpdup(o->picture);
  o->selection = bmpdup(bmp);
  rectfill(o->picture,0,0,IMAGE_WIDTH(o->picture),IMAGE_HEIGHT(o->picture),makecol(255,0,255));
  draw_sprite(o->picture,o->selection,r.a.x,r.a.y);
  o->actsel = r;
  o->select = 1;
  imgedit_undo_add(o);
};
////////////////////////////////////////////////////////////////////////////////
void imgedit_change_select_mode ( p_imgedit o, l_int newvalue ) {
  if ( newvalue && !o->select ) {

    t_rect r = o->actsel;

    if ( o->selection ) destroy_bitmap(o->selection);

    o->selection = create_bitmap(abs(r.a.x-r.b.x),abs(r.a.y-r.b.y));

    blit(o->picture,o->selection,r.a.x,r.a.y,0,0,abs(r.a.x-r.b.x),abs(r.a.y-r.b.y));

    if ( o->temp )      destroy_bitmap(o->temp);

    o->temp = create_bitmap(IMAGE_WIDTH(o->picture),IMAGE_HEIGHT(o->picture));

    blit(o->picture,o->temp,0,0,0,0,IMAGE_WIDTH(o->picture),IMAGE_HEIGHT(o->picture));


    rectfill(o->temp,r.a.x,r.a.y,r.b.x-1,r.b.y-1,o->temp->vtable->mask_color);
    clear_to_color(o->picture,o->picture->vtable->mask_color);

    draw_sprite(o->picture,o->selection,r.a.x,r.a.y);


    o->select = 1;

  } else if ( !newvalue && o->select ) {

    t_rect r = o->actsel;
    t_rect u = rect_assign( max(r.a.x, 0),
                            max(r.a.y, 0),
                            min(r.b.x, IMAGE_WIDTH( o->picture )),
                            min(r.b.y, IMAGE_HEIGHT( o->picture )) );

    blit( o->picture, o->selection, u.a.x, u.a.y, abs(r.a.x - u.a.x), abs(r.a.y - u.a.y), abs(u.a.x - u.b.x), abs(u.a.y - u.b.y) );

    blit(o->temp,o->picture,0,0,0,0,IMAGE_WIDTH(o->temp),IMAGE_HEIGHT(o->temp));

    draw_sprite(o->picture,o->selection,r.a.x,r.a.y);

    o->select = 0;
    if ( o->selection ) {
      destroy_bitmap(o->selection);
      o->selection = NULL;
    };
    if ( o->temp ) {
      destroy_bitmap(o->temp);
      o->temp = NULL;
    };
  };
};
////////////////////////////////////////////////////////////////////////////////
t_point imgedit_get_x_y ( p_imgedit o ) {
  if ( o && o->zoom ) {
    t_point   p    =  VIEW(o)->get_local_point(VIEW(o), mouse->where);
    return point_assign( (p.x / o->zoom ) + o->topx, (p.y / o->zoom ) + o->topy );
  } else
    return point_assign(0,0);
};
////////////////////////////////////////////////////////////////////////////////
void imgedit_update_mouse  ( p_imgedit o ) {
  BITMAP *bmp = NULL;
  if (o->tool == TOOL_ZOOM) bmp = TOOL_ZOOM_CUR;
  else if (o->tool == TOOL_GET) bmp = TOOL_GET_CUR;
  else if (o->tool == TOOL_POINT) bmp = TOOL_POINT_CUR;
  else if (o->tool == TOOL_FILL) bmp = TOOL_FILL_CUR;
  else if (o->tool == TOOL_TEXT) bmp = TOOL_TEXT_CUR;
  else if (o->tool == TOOL_SELECT) bmp = TOOL_SELECT_CUR;
  else if (o->tool == TOOL_FORME) {
    if (o->forme == FORME_RECT) bmp = FORME_RECT_CUR;
    else if (o->forme == FORME_LINE) bmp = FORME_LINE_CUR;
    else if (o->forme == FORME_OVAL) bmp = FORME_OVAL_CUR;
    else if (o->forme == FORME_CIRCLE) bmp = FORME_CIRCLE_CUR;
  };

  if ( bmp ) {
    VIEW(o)->cursor = -1;
    if ( !is_same_bitmap(bmp, mouse_sprite ) ) {
      set_mouse_sprite(bmp);
      set_mouse_sprite_focus(2,2);
    };
  } else {
    VIEW(o)->cursor = CUR_ARROW;
    mouse_set_sys_cursor(CUR_ARROW);
  };
};

////////////////////////////////////////////////////////////////////////////////
void imgedit_update_mouse_cursor ( p_view o ) {
imgedit_update_mouse(imgedit(o));
};
////////////////////////////////////////////////////////////////////////////////
void imgedit_update_status ( p_imgedit o ) {
  if ( o->status ) {
    t_point p   = imgedit_get_x_y(o);
    if ( !o->editing )  {
      o->status->set_text(o->status, "(%d,%d) - zoom : x%d - tool : %d - border : %d - image : %d x %d",p.x,p.y,o->zoom,o->tool,o->border,IMAGE_WIDTH(o->picture),IMAGE_HEIGHT(o->picture));
    } else {
      o->status->set_text(o->status, "(%d,%d) -> (%d,%d) - zoom : x%d - tool : %d - border : %d - image : %d x %d",o->oldx,o->oldy,p.x,p.y,o->zoom,o->tool,o->border,IMAGE_WIDTH(o->picture),IMAGE_HEIGHT(o->picture));
    };
  };
};

////////////////////////////////////////////////////////////////////////////////

#ifndef st_b
#define st_b( a, b, c )  if ( b < a ) b = a; if ( c < b ) b = c;
#endif

void imgedit_settop ( p_imgedit o, l_int x, l_int y) {
if ( o && o->zoom ) {
  t_rect  r = VIEW(o)->get_local_extent(VIEW(o));

  l_int img_w = IMAGE_WIDTH(o->picture);
  l_int img_h = IMAGE_HEIGHT(o->picture);
  l_int v_w   = r.b.x / o->zoom;
  l_int v_h   = r.b.y / o->zoom;

  l_int maxx =  img_w - v_w + 10;
  l_int maxy =  img_h - v_h + 10;
  l_int minx =  - 10 ;
  l_int miny =  - 10 ;

  if ( img_w > v_w ) {
    st_b ( minx, x, maxx )
    o->topx = x;
  } else {
    o->topx = -(v_w - img_w) / 2;
  };

  if ( img_h > v_h ) {
    st_b (  miny, y, maxy )
    o->topy = y;
  } else {
    o->topy = -(v_h - img_h) / 2;
  };
};

};

////////////////////////////////////////////////////////////////////////////////

void   imgedit_draw ( p_view o )
{
 t_rect  r = o->get_local_extent(o);
  t_point p;

 BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {
    l_int f = 0;
    l_int max = ((max(r.b.x,r.b.y)/10)*10)+10;
    l_color bc = color_flat_face;
    l_color lc = color_3d_face;

    rectfill(out, p.x+r.a.x, p.y+r.a.y, p.x + r.b.x, p.y + r.b.y, bc);
    set_clip(out, p.x+r.a.x, p.y+r.a.y, p.x + r.b.x, p.y + r.b.y);

    while ( f < max ) {
      line( out, p.x + f , p.y, p.x + max , p.y + max - f, lc );
      line( out, p.x, p.y + f, p.x + max - f, p.y + max, lc );
      line( out, p.x + f , p.y, p.x , p.y +  f, lc );
      line( out, p.x + f , p.y + max, p.x + max, p.y + f , lc );
      f += 10;
    };


    textout_draw_rect(out, font_system, "SIMP' 0.1 by Julien Etelain", -1, r.a.x+p.x, r.a.y+p.y,
                           r.b.x+p.x, r.b.y+p.y, TX_ALIGN_DEFAULT, lc, bc, 0);

    if ( imgedit(o)->picture ) {

      imgedit_settop( imgedit(o), imgedit(o)->topx, imgedit(o)->topy );

      if  ( imgedit(o)->select )
        stretch_sprite(out, imgedit(o)->temp, p.x - (imgedit(o)->topx * imgedit(o)->zoom),
                                              p.y - (imgedit(o)->topy * imgedit(o)->zoom),
                                              IMAGE_WIDTH(imgedit(o)->temp) * imgedit(o)->zoom,
                                              IMAGE_HEIGHT(imgedit(o)->temp)* imgedit(o)->zoom);

      stretch_sprite(out, imgedit(o)->picture , p.x - (imgedit(o)->topx * imgedit(o)->zoom),
                                                p.y - (imgedit(o)->topy * imgedit(o)->zoom),
                                                IMAGE_WIDTH(imgedit(o)->picture) * imgedit(o)->zoom,
                                                IMAGE_HEIGHT(imgedit(o)->picture)* imgedit(o)->zoom);

      rect(out, (p.x - (imgedit(o)->topx * imgedit(o)->zoom)) - 1,
                (p.y - (imgedit(o)->topy * imgedit(o)->zoom)) - 1,
                (p.x - (imgedit(o)->topx * imgedit(o)->zoom)) + (IMAGE_WIDTH(imgedit(o)->picture) * imgedit(o)->zoom),
                (p.y - (imgedit(o)->topy * imgedit(o)->zoom)) + (IMAGE_HEIGHT(imgedit(o)->picture) * imgedit(o)->zoom), lc);

      if ( imgedit(o)->zoom >= 10 ) {  // Auto - Grid
        l_int tx = max(p.x,(p.x + r.a.x - (imgedit(o)->topx * imgedit(o)->zoom)));
        l_int ty = max(p.y,(p.y + r.a.y - (imgedit(o)->topy * imgedit(o)->zoom)));
        l_int x = tx;
        l_int y = ty;
        l_int bx = min(r.b.x + p.x, (p.x + r.a.x - (imgedit(o)->topx * imgedit(o)->zoom)) + (IMAGE_WIDTH(imgedit(o)->picture) * imgedit(o)->zoom));
        l_int by = min(r.b.y + p.y, (p.y + r.a.y - (imgedit(o)->topy * imgedit(o)->zoom)) + (IMAGE_HEIGHT(imgedit(o)->picture) * imgedit(o)->zoom));

        while ( x <= bx ) {
          vline (out,x,ty,by,lc);
          x += imgedit(o)->zoom;
        };
        while ( y <= by ) {
          hline (out,tx,y,bx,lc);
          y += imgedit(o)->zoom;
        };
      };

      if ( imgedit(o)->select ) {
        p_imgedit vo = IMGEDIT(o);
        t_rect     b = vo->actsel;

        rect( out,  p.x + (b.a.x * vo->zoom) - (vo->topx * vo->zoom) - 1,
                    p.y + (b.a.y * vo->zoom) - (vo->topy * vo->zoom) - 1,
                    p.x + (b.b.x * vo->zoom) - (vo->topx * vo->zoom),
                    p.y + (b.b.y * vo->zoom) - (vo->topy * vo->zoom), color_selected_face );
      };

      if ( imgedit(o)->editing ) {
        p_imgedit vo  = IMGEDIT(o);
        l_int     d   = vo->zoom / 2;

        l_int     x   = (vo->oldx * vo->zoom) + p.x + r.a.x - (vo->topx * vo->zoom) + d;
        l_int     y   = (vo->oldy * vo->zoom) + p.y + r.a.y - (vo->topy * vo->zoom) + d;
        t_point   pe  =  imgedit_get_x_y(vo);

        l_color   wc1  = vo->col1;
        l_color   wc2  = vo->col2;

        pe.x = pe.x * vo->zoom - (vo->topx * vo->zoom) + d;
        pe.y = pe.y * vo->zoom - (vo->topy * vo->zoom) + d;

        if ( vo->bt ==  1) {
          wc1 =  vo->col2;
          wc2 =  vo->col1;
        };

        if ( vo-> tool == TOOL_FORME )
          go_draw_it (out,x,y, p.x + pe.x , p.y + pe.y , wc1, wc2,0,1, vo->forme,PEN_RECT);
        else if ( vo->tool == TOOL_SELECT ) {
          t_point   pb   =  imgedit_get_x_y(vo);
          t_rect     b   = rect_assign(vo->oldx,vo->oldy,pb.x,pb.y);
          rect( out,  p.x + (min(b.a.x,b.b.x) * vo->zoom) - (vo->topx * vo->zoom) - 1,
                      p.y + (min(b.a.y,b.b.y) * vo->zoom) - (vo->topy * vo->zoom) - 1,
                      p.x + (max(b.a.x,b.b.x) * vo->zoom) - (vo->topx * vo->zoom),
                      p.y + (max(b.a.y,b.b.y) * vo->zoom) - (vo->topy * vo->zoom), color_selected_face );
        };
      };
    };
  };

  o->end_of_paint(o, r);

  imgedit_update_status(imgedit(o));
};

////////////////////////////////////////////////////////////////////////////////

void imgedit_save ( p_imgedit o, l_text filename, l_long quality ) {
  if ( o->filename ) _free(o->filename);
  o->filename = _strdup(filename);
  save_bitmap(filename, o->picture, NULL);
};

////////////////////////////////////////////////////////////////////////////////

void imgedit_load ( p_imgedit o, l_text filename ) {
  imgedit_reset(o);
  o->picture = load_image(filename);
  if ( o->filename ) _free(o->filename);
  o->filename = _strdup(filename);
  imgedit_undo_add(o);
  VIEW(o)->draw_me(VIEW(o));
};

////////////////////////////////////////////////////////////////////////////////

void imgedit_newbmp ( p_imgedit o, l_long width, l_long height ) {
  imgedit_reset(o);
  o->picture = create_bitmap(width,height);
  rectfill(o->picture, 0, 0, width, height, o->col2);
  imgedit_undo_add(o);
  VIEW(o)->draw_me(VIEW(o));
};

////////////////////////////////////////////////////////////////////////////////

void imgedit_newpts ( p_imgedit o, l_int x, l_int y, l_int oldx, l_int oldy, l_color wc1, l_color wc2, l_int bt ) {
  if ( !o || !o->zoom ) return;

  switch ( o->tool ) {
    case TOOL_ZOOM : {
      if ( !bt ) { o->zoom++; } else { if (o->zoom > 1) o->zoom--; };
      imgedit_settop ( o,
                       x - (((VIEW(o)->bounds.b.x - VIEW(o)->bounds.a.x) / o->zoom) / 2),
                       y - (((VIEW(o)->bounds.b.y - VIEW(o)->bounds.a.y) / o->zoom) / 2));
    }; break;
    case TOOL_GET : {
      if ( !bt )
        o->col1 = getpixel(o->picture, x, y);
      else
        o->col2 = getpixel(o->picture, x, y);
      VIEW(o->colsel)->draw_me(VIEW(o->colsel));
    }; break;
    case TOOL_POINT : {
      go_draw_point( o->picture, x, y, wc1 ,o->border,o->penstyle ,o->floodmode,o->floodlevel);
      imgedit_undo_add(o);
    }; break;
    case TOOL_FILL : {
      if ( o->floodstyle ) {
        if ( demon_bmp ) destroy_bitmap(demon_bmp);
        demon_bmp = create_bitmap(IMAGE_WIDTH(o->picture), IMAGE_HEIGHT(o->picture));
        rectfill(demon_bmp,0,0,IMAGE_WIDTH(demon_bmp), IMAGE_HEIGHT(demon_bmp),wc2);
        if (o->select == 1)
          x_gardient(demon_bmp,o->actsel.a.x,o->actsel.a.y,o->actsel.b.x,o->actsel.b.y,wc1,wc2,o->floodstyle-1,o->set1,o->set2);
        else
          x_gardient(demon_bmp,0,0,IMAGE_WIDTH(demon_bmp),IMAGE_HEIGHT(demon_bmp),wc1,wc2,o->floodstyle-1,o->set1,o->set2);
        wc1 = -1;
      };
      floodit( o->picture, x, y, wc1 ,o->floodtol);
      imgedit_undo_add(o);
    }; break;
    case TOOL_FORME : {
      if ( !o->editing )
        o->editing = 1;
      else {
        go_draw_it (o->picture,oldx,oldy, x , y , wc1, wc2,o->fillmode,o->border,o->forme,o->penstyle);
        o->editing = 0;
        imgedit_undo_add(o);
      };
    }; break;
    case TOOL_SELECT : { //select
      if ( !o->editing && !o->select && !bt) {
        o->editing = 1;
      } else if ( !o->select && !bt) {
        o->actsel = rect_assign(min(oldx,x),min(oldy,y),max(x,oldx),max(y,oldy));
        o->editing = 0;
        imgedit_change_select_mode ( o , 1 );
        imgedit_undo_add(o);
      } else if ( bt ) {
        imgedit_change_select_mode ( o , 0 );
        o->editing = 0;
        imgedit_undo_add(o);
      };
    }; break;
    case TOOL_TEXT : {
      l_text t = inputbox ("Tool Text","Text ?","");
      if ( !t ) return;
      imgedit_make_it_sel( o, make_bmp_with_text(t,wc1,VIEW(o)->font), x,y);
      o->tool = TOOL_SELECT;
      _free(t);
    }; break;
  };
};

////////////////////////////////////////////////////////////////////////////////

void imgedit_autopts ( p_imgedit o, l_int x, l_int y, l_int oldx, l_int oldy, l_color wc1, l_color wc2, l_int bt ) {
  if ( !o || !o->zoom ) return;
  o->pr = 1;
  switch ( o->tool ) {
    case TOOL_MOVE : {
      t_point m = VIEW(o)->get_local_point(VIEW(o), mouse->where);

      imgedit_settop( o, o->topx + (o->realold.x - m.x) / o->zoom,
                         o->topy + (o->realold.y - m.y) / o->zoom);
    }; break;
    case TOOL_POINT :
      go_draw_line ( o->picture, x, y, oldx, oldy, wc1, o->border, o->penstyle ,o->floodmode,o->floodlevel );
    break;
    case TOOL_SELECT : if ( !bt && o->select ) {
      t_rect r = o->actsel;
      t_rect u = rect_assign(  max(r.a.x, 0),
                               max(r.a.y, 0),
                               min(r.b.x, IMAGE_WIDTH( o->picture )),
                               min(r.b.y, IMAGE_HEIGHT( o->picture )) );
      blit( o->picture, o->selection, u.a.x, u.a.y, abs(r.a.x - u.a.x), abs(r.a.y - u.a.y), abs(u.a.x - u.b.x), abs(u.a.y - u.b.y) );
      rectfill(o->picture,r.a.x,r.a.y,r.b.x,r.b.y,makecol(255,0,255));
      r.a.x -= oldx - x;
      r.b.x -= oldx - x;
      r.a.y -= oldy - y;
      r.b.y -= oldy - y;
      draw_sprite(o->picture,o->selection ,r.a.x,r.a.y);
      o->actsel = r;
    }; break;
  };
};

////////////////////////////////////////////////////////////////////////////////

void imgedit_func_callback ( p_object o ) {
if ( o ) {
if ( imgedit(o)->editing ) {
t_point m = VIEW(o)->get_local_point(VIEW(o), mouse->where);
t_rect  r = VIEW(o)->get_local_extent(VIEW(o));
l_bool  c = 0;
t_point p = point_assign(imgedit(o)->topx,imgedit(o)->topy);

if ( is_b ( r.a.x, m.x, r.a.x + 20) ) { p.x-=2; c = 1; };
if ( is_b ( r.b.x - 20, m.x, r.b.x) ) { p.x+=2; c = 1; };

if ( is_b ( r.a.y, m.y, r.a.y + 20) ) { p.y-=2; c = 1; };
if ( is_b ( r.b.y - 20, m.y, r.b.y) ) { p.y+=2; c = 1; };

if ( c )  imgedit_settop(imgedit(o),p.x,p.y);
};
};
};
////////////////////////////////////////////////////////////////////////////////
void   imgedit_translate_event ( p_object o, t_event *event ) {
 if ( o ) {
   RETVIEW(o, event);

   if ( event->type & EV_MOUSE ) {  // MO_SF_MOUSELDOWN
     l_char ok = 0;

     if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) ok = 1;
     if ( OBJECT(mouse)->state & MO_SF_MOUSERDOWN ) ok = 2;

     if ( OBJECT(mouse)->state & MO_SF_MOUSELAUTO ) ok = 3;
     if ( OBJECT(mouse)->state & MO_SF_MOUSERAUTO ) ok = 4;

     if ( ok ) {

       p_imgedit vo = IMGEDIT(o);

       l_long    bt = ( ok == 2 || ok == 4 ) ? 1 : 0;

       t_point p   = imgedit_get_x_y(vo);

       clear_event(event);

       if (ok == 1) imgedit_newpts(vo, p.x, p.y, vo->oldx, vo->oldy, vo->col1, vo->col2, bt);
       if (ok == 2) imgedit_newpts(vo, p.x, p.y, vo->oldx, vo->oldy, vo->col2, vo->col1, bt);

       if ( vo->oldx != p.x || vo->oldy != p.y ) {

          if (ok == 3)
            imgedit_autopts(vo, p.x, p.y, vo->oldx, vo->oldy, vo->col1, vo->col2, bt);
          else if ( ok == 4 )
            imgedit_autopts(vo, p.x, p.y, vo->oldx, vo->oldy, vo->col2, vo->col1, bt);


          vo->oldx = p.x;
          vo->oldy = p.y;

          vo->realold  = VIEW(vo)->get_local_point(VIEW(vo), mouse->where);

          VIEW(o)->draw_me(VIEW(o));

        /*} else {

          VIEW(o)->draw_me(VIEW(o));*/

        };

      } else if ((OBJECT(mouse)->state & MO_SF_MOUSEMOVE) && imgedit(o)->editing ) {
        VIEW(o)->draw_me(VIEW(o));
        clear_event(event);
      };

      // if (OBJECT(mouse)->state & MO_SF_MOUSEMOVE) imgedit_update_status(imgedit(o));

      if ( OBJECT(mouse)->state & MO_SF_MOUSEUP && imgedit(o)->editing && imgedit(o)->pr == 1 ) {
        imgedit_undo_add(imgedit(o));
        imgedit(o)->pr = 0;
        clear_event(event);
      };
    };
  };
};

////////////////////////////////////////////////////////////////////////////////

p_colsel2   _colsel2_init ( p_colsel2 o, t_rect r )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_colsel2));

  view_init(VIEW(o), r);

  VIEW(o)->draw = &colsel2_draw;
  OBJECT(o)->translate_event = &colsel2_translate_event;

  OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
  OBJECT(o)->set_options(OBJECT(o), OB_OF_STILLPROCESS, true);


  return o;

};

////////////////////////////////////////////////////////////////////////////////

p_imgedit   _imgedit_init ( p_imgedit o, t_rect r, p_colsel2 cc, p_stattext st  )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_imgedit));

  // Get some space for scrollbars
  // r.b.y -= 18;
  // r.b.x -= 18;

  view_init(VIEW(o), r);
  // Initialise ScrollBars ...
  // scroller_init(SCROLLER(o), r, SF_HORSCROLLBAR+SF_VERSCROLLBAR);

  VIEW(o)->draw = &imgedit_draw;
  VIEW(o)->set_mouse_cursor = &imgedit_update_mouse_cursor;

  OBJECT(o)->translate_event = &imgedit_translate_event;

  o->save       = &imgedit_save;
  o->load       = &imgedit_load;
  o->newbmp     = &imgedit_newbmp;
  o->col1       = COLOR(CO_BLACK);
  o->col2       = COLOR(CO_WHITE);
  o->border     = 1;
  o->fillmode   = FILL_NO;
  o->forme      = FORME_LINE;
  o->penstyle   = PEN_RECT;
  o->floodmode  = FLOOD_NO;
  o->floodlevel = 100;
  o->floodtol   = 0;
  o->tool       = TOOL_MOVE;

  imgedit_reset(o);
  //OBJECT(o)->func_callback = &imgedit_func_callback;
  OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
  OBJECT(o)->set_options(OBJECT(o), OB_OF_STILLPROCESS, true);

  o->status = st;
  o->colsel = cc;
  if ( cc ) cc->imgedit = o;

  return o;

};

////////////////////////////////////////////////////////////////////////////////

void imgedit_xdl_init ( ) {

  img_bmps[0] = Get_Bitmap(COLORS);
  img_bmps[1] = Get_Bitmap(ZOOM_CUR);
  img_bmps[2] = Get_Bitmap(MOVE_CUR);
  img_bmps[3] = Get_Bitmap(PEN_CUR);
  img_bmps[4] = Get_Bitmap(FLOOD_CUR);
  img_bmps[5] = Get_Bitmap(TEXT_CUR);
  img_bmps[6] = Get_Bitmap(SELECT_CUR);
  img_bmps[7] = Get_Bitmap(RECT_CUR);
  img_bmps[8] = Get_Bitmap(LINE_CUR);
  img_bmps[9] = Get_Bitmap(OVAL_CUR);
  img_bmps[10] = Get_Bitmap(OVAL_CUR);

  init_imgfilters();

};

/*#include"imgedit.exp"

SetInfoAppName("Image Editor Control");
SetInfoDesciption("Image Editor Control used by The SIMP'");
SetInfoCopyright("(c) Copyright 2001 Point Mad. All rights reserved");
SetInfoManufacturer("Point Mad");

lib_begin ( void ) {
  if ( ap_process == AP_ALLOC ) {
    AP_EXPORTLIB();
  };
  if ( ap_process == AP_INIT ) {
      imgedit_xdl_init();
  };

  if ( ap_process == AP_DONE ) {

  };

} lib_end;*/

