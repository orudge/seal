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

#include <seal.h>
#include <treeview.h>

p_treeview  (*treeview_init) ( p_treeview o, t_rect r ) = &_treeview_init;

void    treeitem_set_data_from ( p_treeitem a, p_treeitem b ) {

  a->extended  = b->extended;
  a->caption   = b->caption;
  a->name      = b->name;
  a->bmp       = b->bmp;
  //a->first     = b->first;
  a->last      = b->last;
  a->load_subs = b->load_subs;
  a->level     = b->level;

};

////////////////////////////////////////////////////////////////////////////////
l_bool  treeitem_collect_by_name_from ( p_treeitem parent, p_treeitem from ) {
   p_treeitem f = (parent->last)?parent->last->next:NULL;//parent->first;
   p_treeitem m = from;
   if ( !from ) from = m = f;
   else from = m = from->next;

   if( from == from->next ) return false;

   if ( from && m )
   do {
      from = m;
      do {
            l_text str1 = from->caption;
            l_text str2 = m->caption;
            if ( str1 && str2 && stricmp(str1, str2) < 0 ) {
              t_treeitem old;
              treeitem_set_data_from(&old,m);
              treeitem_set_data_from(m,from);
              treeitem_set_data_from(from,&old);
            };
         from = from->next;
      } while ( from != f );
      m = m->next;
   } while ( m != f );
   return true;
};

////////////////////////////////////////////////////////////////////////////////
p_treeitem treeitem_init ( p_treeview t, l_bool extended, l_text caption, l_text name, BITMAP *bmp, p_treeitem next, p_treeitem prev, p_treeitem parent, p_treeitem first, p_treeitem last,void(*load_subs) ( p_treeview o, p_treeitem m  ) ) {
  p_treeitem o = treeitem( _malloc(sizeof(t_treeitem)));

  o->extended = extended;
  o->caption  = caption;
  o->name     = name;
  o->bmp      = bmp;
  o->next     = next;
  o->prev     = prev;
  o->parent   = parent;
  //o->first    = first;
  o->last     = last;
  o->load_subs = load_subs;
  if ( parent )
     o->level= parent->level + 1;
  else
     o->level = 0;

  return o;
};
////////////////////////////////////////////////////////////////////////////////
void all_treeitem ( p_list list, p_treeitem it, l_bool ex ) {
  if ( list && it && it->last ) {
    p_treeitem p = it->last->next;
    p_treeitem f = p;
    if ( p )
    do {

       list->insert(list,p);

       if ( ex || ( !ex && p->extended ) )
          if ( p->last )
             all_treeitem( list, p , ex);

       p = p->next;

    } while( p && f != p ) ;
  };
};
////////////////////////////////////////////////////////////////////////////////
p_treeitem get_treeitem ( p_treeview o, l_text name ) {
  if ( !o || !name ) return NULL;
  if ( stricmp( name, "root" ) ) {
    if ( o->llist && o->llist->last ) {
      p_item x = o->llist->last;
      p_item f = x;
      if ( x )
      do {
        if ( !stricmp( treeitem(x->rec)->name, name ) ) return x->rec;
        x = x->next;
      } while ( x != f );
    };
    return NULL;
  } else return o->root;
};
////////////////////////////////////////////////////////////////////////////////
void update_tree ( p_treeview o ) {
  if ( o && o->root && o->root->last ) {
    while ( o->visiblelist->last ) {
      o->visiblelist->remove_item(o->visiblelist,o->visiblelist->last);
    };
    o->all_treeitem( o->visiblelist, o->root ,false );
  };
};
////////////////////////////////////////////////////////////////////////////////
p_treeitem  add_treeitem_ex ( p_treeview o, p_treeitem m , l_text name, l_text caption, BITMAP *bmp, l_bool extended, void(*load_subs) ( p_treeview o, p_treeitem m ) ) {
  if ( o && m ) {
    p_treeitem me = o->treeitem_init(o,extended,caption,name,bmp,(m->last)?m->last->next:NULL,m->last,m,NULL,NULL,load_subs);

    if ( m->last ) {
      m->last->next->prev = me;
      m->last->next = me;
      m->last = me;
    } else {
      m->last = me;
      //m->first = me;
      me->next=me;
      me->prev=me;
    };

    o->llist->insert(o->llist,me);
    return me;
  };
  return NULL;
};
////////////////////////////////////////////////////////////////////////////////
void delete_treeitem ( p_treeview o, p_treeitem it );
////////////////////////////////////////////////////////////////////////////////
void delete_treeitem ( p_treeview o, p_treeitem it ) {
  if ( o && it ) {
    DEBUG_printf("Treeview :: Del ''%s''\n",it->name);

    if  ( o->selected == it ) o->selected = NULL;
    if  ( o->root == it ) o->root = NULL;
    while ( it->last ) delete_treeitem(o,it->last);

    if ( it->parent ) {

      p_treeitem m = it->parent;

      if ( m->last == it ) m->last = it->prev;

      if ( m->last == it ) m->last = NULL;

    };

    if ( it->next ) {
      p_treeitem x = it->prev;

      x->next = it->next;
      it->next->prev = x;

      it->next = NULL;
      it->prev = NULL;
    };
  };
};
////////////////////////////////////////////////////////////////////////////////
p_treeitem  add_treeitem ( p_treeview o, l_text parent, l_text name, l_text caption, BITMAP *bmp, l_bool extended,void(*load_subs) ( p_treeview o, p_treeitem m ) ) {
  p_treeitem mm = NULL;

  if ( parent ) {
    p_treeitem m = get_treeitem ( o, parent );

    if ( !m ) return NULL;

    mm = o->add_ex( o, m ,  name,  caption, bmp, extended,load_subs );

    o->update(o);

    VIEW(o)->draw_me(VIEW(o));

    draw_scroller_bars(o);
  };
  return mm;
};
////////////////////////////////////////////////////////////////////////////////
void load_subs_reg ( p_treeview o, p_treeitem m  ) {
  load_treeitem_from_reg_ex(o,m,_strdup(m->name));
};
////////////////////////////////////////////////////////////////////////////////
void load_treeitem_from_reg_ex ( p_treeview o, p_treeitem m, l_text rootkey  ) {

    p_registry_search inf = (p_registry_search) malloc(sizeof(t_registry_search));

    if ( reg_find_first(rootkey, inf) ) do {
      l_text name    = _strdup(inf->name);
      l_text caption = _strdup(inf->key->name);

      p_treeitem it = o->add_ex(o,m,name,caption,IMG_DIR16,false,&load_subs_reg);

    } while (reg_find_next(inf));

    _free(inf);
};
////////////////////////////////////////////////////////////////////////////////
void load_treeitem_from_reg ( p_treeview o,l_text m, l_text rootkey ) {

  o->working = 1;

  VIEW(o)->draw_me(VIEW(o));

  load_treeitem_from_reg_ex(o, get_treeitem ( o, m ),rootkey);

  update_tree(o);
  o->working = 0;

  VIEW(o)->draw_me(VIEW(o));

  draw_scroller_bars(o);
};
////////////////////////////////////////////////////////////////////////////////
void load_subs_dir ( p_treeview o, p_treeitem m  ) {
  load_treeitem_from_dir_ex(o,m,_strdup(m->name));
};
////////////////////////////////////////////////////////////////////////////////
void load_treeitem_from_dir_ex ( p_treeview o, p_treeitem m, l_text path ) {
  struct t_ffblk f;
  l_int done = 0;

  l_text fi = io_realpath(path, "*.*");

  done = io_findfirst(fi, &f, FA_DIREC);

  while ( !done ) {

    if ( (f.info.ff_attrib & FA_DIREC) && io_isfilename(f.info.ff_name) ) {

        l_text alld = io_realpath(path, f.info.ff_name);

        o->add_ex(o,m,_strdup(alld),_strdup(f.info.ff_name),IMG_DIR16,false,&load_subs_dir);

        _free(alld);

    };

    done = io_findnext(&f);

  };

  _free(fi);

  treeitem_collect_by_name_from(m,NULL);

};
////////////////////////////////////////////////////////////////////////////////
void load_treeitem_from_dir ( p_treeview o,l_text m, l_text path ){

  o->working = 1;

  VIEW(o)->draw_me(VIEW(o));

  load_treeitem_from_dir_ex(o, get_treeitem ( o, m ),path);

  update_tree(o);
  o->working = 0;

  VIEW(o)->draw_me(VIEW(o));

  draw_scroller_bars(o);
};
////////////////////////////////////////////////////////////////////////////////
void   treeview_draw ( p_view o )
{
 t_rect  r = o->get_local_extent(o);
  t_point p;

 BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {

  rectfill(out, p.x+r.a.x, p.y+r.a.y, p.x+r.b.x, p.y+r.b.y, color_flat_face);

  if (  treeview(o)->working == 0 ) {
   if ( treeview(o)->visiblelist && treeview(o)->visiblelist->first ) {

   p_item x = treeview(o)->visiblelist->first(treeview(o)->visiblelist);
   p_item f = x;
   l_int a = -treeview(o)->top;
   if ( x )
   do {
     if ( x->rec ) {
      p_treeitem it = treeitem(x->rec);
      l_int w = it->caption ? get_text_length(o->font,it->caption,-1) : 0;
      t_rect item_r = rect_assign ( p.x + ( it->level * 20 ), p.y + ( a * 20 ) + 1, p.x + ( it->level * 20 ) + 36 + w, p.y + ( a * 20 ) + 20 );


      if ( it->last || it->load_subs ) {

        t_rect ppl = rect_assign( item_r.a.x-13,item_r.a.y+5,item_r.a.x-5,item_r.b.y-6);

        rect(out,ppl.a.x,ppl.a.y,ppl.b.x,ppl.b.y,COLOR(7));

        hline (out,ppl.a.x+2,ppl.a.y+4,ppl.b.x-2,COLOR(8));

        if ( !it->extended )
          vline (out,ppl.a.x+4,ppl.a.y+2,ppl.b.y-2,COLOR(8));


      };

      /*if ( treeview(o)->selected == it ) rectfill(out, item_r.a.x, item_r.a.y, item_r.b.x, item_r.b.y, color_selected_face);*/

      if ( it->caption )
        textout_draw_rect_ex(out, o->font, it->caption, -1,
                                        item_r.a.x + 28,
                                        item_r.a.y,
                                        item_r.b.x,
                                        item_r.b.y,
                                       TX_ALIGN_CENTERY, ( treeview(o)->selected == it  ) ? color_selected_text : color_flat_text, ( treeview(o)->selected == it  ) ? color_selected_face : TX_NOCOLOR, 1,2);

      if ( it->bmp )
        if ( treeview(o)->selected == it && get_depth(out) >= 16 ) {
          set_trans_blender(getr(color_selected_face), getg(color_selected_face), getb(color_selected_face), 0);
          draw_lit_sprite(out,it->bmp, item_r.a.x+4,item_r.a.y+2,128);
          solid_mode();
        } else
          draw_sprite(out,it->bmp,item_r.a.x+4,item_r.a.y+2);

      a++;
      };
      x = x->next;
   } while ( x && x != f );
  };
  };
    button(out, p.x+r.a.x, p.y+r.a.y, p.x+r.b.x, p.y+r.b.y, color_3d_shadow, color_3d_face);

    draw_scroller_bars(o);

  };

  o->end_of_paint(o, r);
};
////////////////////////////////////////////////////////////////////////////////
void   treeview_translate_event ( p_object o, t_event *event )
{

  RETVIEW(o, event);


    if ( event->type & EV_MOUSE ) {
       t_point p     =  VIEW(o)->get_local_point(VIEW(o), mouse->where);
       l_int a       =  p.y / 20;
       l_int id      = treeview(o)->top + a;
       p_treeitem it = ( list_get_max_item(treeview(o)->visiblelist) >= id ) ? list_at(treeview(o)->visiblelist, id ) : NULL;

       if ( it && treeview(o)->working == 0 ) {
         l_int w = it->caption ? get_text_length(VIEW(o)->font,it->caption,-1) : 0;
         t_rect item_r = rect_assign ( ( it->level * 20 ), ( a * 20 ) + 1, p.x + ( it->level * 20 ) + 36 + w, p.y + ( a * 20 ) + 20 );
         t_rect cross = rect_assign( item_r.a.x-13,item_r.a.y+5,item_r.a.x-5,item_r.b.y-6);

         if ( ( rect_contains(item_r,p) && OBJECT(mouse)->state & MO_SF_MOUSELDOUBLE ) ||
              ( rect_contains(cross,p) && OBJECT(mouse)->state & MO_SF_MOUSELUP ) ) {
           clear_event(event);
           it->extended = ( it->extended ) ? false : true;
           if ( it->load_subs ) {
             it->load_subs(treeview(o),it);
             it->load_subs = NULL;
           };
           update_tree(treeview(o));
           VIEW(o)->draw_me(VIEW(o));
           draw_scroller_bars(o);

       } else if ( rect_contains(item_r,p) && OBJECT(mouse)->state & MO_SF_MOUSELUP ) {
         clear_event(event);
         if ( treeview(o)->selected != it ) {
           treeview(o)->selected = it;
           set_event(event, EV_MESSAGE, treeview(o)->message, o);
           o->put_event(o, event);
           VIEW(o)->draw_me(VIEW(o));
         };
       };


       };

  };

  if ( o->is_state(o, OB_SF_FOCUSED) )

  if ( event->type & EV_KEYBOARD ) {

    p_item it = (treeview(o)->selected && treeview(o)->visiblelist)?list_find_rec(treeview(o)->visiblelist,treeview(o)->selected):0;
    p_treeitem newsel = NULL;

    if ( OBJECT(keyb)->state & KB_SF_KEYDOWN ) {

    switch ( KEY_TO(keyb->code) ) {

      case KB_DOWN : {
        if ( it ) newsel = it->next->rec;
        clear_event(event);
      }; break;
      case KB_UP : {
	    if ( it ) newsel = it->prev->rec;
        clear_event(event);
      }; break;
      case KB_HOME : {
	    if ( treeview(o)->visiblelist ) newsel = treeview(o)->visiblelist->last?treeview(o)->visiblelist->last->next->rec:0;
        clear_event(event);
      }; break;
      case KB_END : {
	    if ( treeview(o)->visiblelist ) newsel = treeview(o)->visiblelist->last?treeview(o)->visiblelist->last->rec:0;
        clear_event(event);
      }; break;
      default :
        if ( (l_byte)TO_CHAR(keyb->code) >= 32 &&
             (l_byte)TO_CHAR(keyb->code) <= 255 ) {
          if ( it ) {
	        p_item a = it->next;
	        p_item b = a;
	        do {
		      p_treeitem i = a->rec;
		      if ( i->caption ) if ( tolower(i->caption[0]) == tolower(TO_CHAR(keyb->code)) ) {
			    newsel = a->rec;
			    b=a;
			  } else
		        a=a->next;
	        } while ( a != b );

          };
          clear_event(event);
        }; break;
      };
      if ( newsel && newsel != treeview(o)->selected ) {
	    t_rect  r = VIEW(o)->get_local_extent(VIEW(o));
	    l_long id = list_index_of(treeview(o)->visiblelist,newsel);
	    if ( id < treeview(o)->top ) treeview(o)->top = id;
	    if ( id > treeview(o)->top + (r.b.y / 20) ) treeview(o)->top = min( max( id-(r.b.y / 20), 0 ), list_get_max_item(treeview(o)->visiblelist));
	    treeview(o)->selected = newsel;
	    set_event(event, EV_MESSAGE, treeview(o)->message, o);
        o->put_event(o, event);
        SCROLLER(o)->scrolly=treeview(o)->top*20;
        draw_scroller_bars(o);
        VIEW(o)->draw_me(VIEW(o));
      };
    };
  };

};
////////////////////////////////////////////////////////////////////////////////
t_rect treeview_size_limits ( p_view o )
{

  return rect_assign(1, 1, rect_sizex(o->bounds)-1, rect_sizey(o->bounds)-1);

};
////////////////////////////////////////////////////////////////////////////////
t_rect treeview_scroll_limits ( p_scroller o )
{
  t_rect r = VIEW(o)->size_limits(VIEW(o));

  return r;

};
////////////////////////////////////////////////////////////////////////////////
void   treeview_scroll_size ( p_scroller o, l_long *x, l_long *y )
{
  p_treeview tv = treeview(o);
  t_rect r = o->scroll_limits(o);

  if ( y ) {
    if ( tv->visiblelist && tv->visiblelist->last ) *(y) = tv->visiblelist->index_of_item(tv->visiblelist,tv->visiblelist->last)*20+20;
    else  *(y) = rect_sizey(r);
  };
  if ( x )
    *(x) = rect_sizex(r);

};

////////////////////////////////////////////////////////////////////////////////
void   treeview_recalc_positions ( p_scroller o, l_long x, l_long y )
{

  p_treeview tv = treeview(o);

  tv->top += y / 20;

};
////////////////////////////////////////////////////////////////////////////////
l_bool treeview_done ( p_object o ) {

  delete_treeitem(treeview(o),treeview(o)->root);

  return scroller_done ( o );
};

////////////////////////////////////////////////////////////////////////////////

p_treeview   _treeview_init ( p_treeview o, t_rect r )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_treeview));

  scroller_init(SCROLLER(o), r, SF_VERSCROLLBAR );



    o->add           = &add_treeitem;
    o->add_ex        = &add_treeitem_ex;
    o->load_from_dir = &load_treeitem_from_dir;
    o->treeitem_init = &treeitem_init;
    o->all_treeitem  = &all_treeitem;
    o->get           = &get_treeitem;
    o->update        = &update_tree;
  o->visiblelist = list_init(_malloc(sizeof(t_list)), NULL, 0);
  o->llist = list_init(_malloc(sizeof(t_list)), NULL, 0);
  o->root = o->treeitem_init(o,true,_strdup("root"),_strdup("root"), NULL,NULL,NULL,NULL,NULL,NULL,NULL);
  o->top = 0;
  o->message = MSG_UPDATE;
  VIEW(o)->draw = &treeview_draw;
  VIEW(o)->size_limits = &treeview_size_limits;

  //OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
  //OBJECT(o)->set_options(OBJECT(o), OB_OF_STILLPROCESS, true);
  OBJECT(o)->translate_event = &treeview_translate_event;
  OBJECT(o)->done = &treeview_done;
  SCROLLER(o)->deltay = 20;
  SCROLLER(o)->scroll_size = &treeview_scroll_size;
  SCROLLER(o)->recalc_positions = &treeview_recalc_positions;
  SCROLLER(o)->scroll_limits =&treeview_scroll_limits;

  return o;

};
