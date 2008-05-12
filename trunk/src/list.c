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
//#include"object.h"
#include <list.h>
//#include<string.h>

p_list  (*list_init) ( p_list o, void (*func_free)(void *), l_tag tag ) = &_list_init;

l_bool  list_done ( p_list o )
{
  p_item v = o->last;

  while ( o->last ) {

    o->remove_item(o, o->last);

  };

  o->last = NULL;
  l_tag_cpy(o->tag, TAG_NONE);
  o->func_free = NULL;

  return true;

};


void    list_copy_ctx ( p_list dst, p_list src )
{

   p_item x = src->first(src);
   p_item f = x;

   if  ( f )
   do {

      dst->insert_ex(dst, x->rec, x->func_free, x->tag);

      x = x->next;

   } while ( x != f );

};

/*
  call function callback for each item in the list,

  callback contains argument ob = (object f.e.), o->item = item of list,
  ind what's indicator of process. This argument contains value of number of
  items ( f.e. copied size )

  if callback return false, for_each cycle is stoped

  else go while all items are done

  -  return false, if some callback return false, else return true

*/
l_bool    list_for_each_item ( p_list o, void *ob, l_int (*callback)(), l_dword *ind )
{

   p_item x = o->first(o);
   p_item f = x;
   l_bool ok = true;

   if  ( f && callback )
   do {

      ok = callback(ob, x->rec, ind);

      x = x->next;

   } while ( ok && x != f );

   return ok;

};


/*
  call function callback for each item in the list,

  callback contains argument ob, item and o->item = item of list,
  ind what's indicator of process. This argument contains value of number of
  items ( f.e. copied size )

  if callback return false, for_each cycle is stoped

  else go while all items are done

  -  return false, if some callback return false, else return true

*/
l_bool    list_for_each_item_to_item ( p_list o, void *ob, void *item, l_int (*callback)(), l_dword *ind )
{

   p_item x = o->first(o);
   p_item f = x;
   l_bool ok = true;

   if  ( f && callback )
   do {

      ok = callback(ob, item, x->rec, ind);

      x = x->next;

   } while ( ok && x != f );

   return ok;

};


void   *list_first_rec ( p_list o )
{
  if ( o->last ) return o->last->next->rec;

  return NULL;
};


p_item  list_first ( p_list o )
{
  if ( o->last ) return o->last->next;

  return NULL;
};


p_item  list_at_item ( p_list o, l_long index )
{
  p_item v = o->first(o);
  p_item f = v;

  if ( v && index >= 0 )

    do {

      if ( !index ) return v;

      v = v->next;

      index--;

    } while ( v != f );

  return NULL;
};


void   *list_at ( p_list o, l_long index )
{

  p_item v = o->at_item(o, index);

  if ( v ) return v->rec;

  return NULL;

};



l_long  list_index_of ( p_list o, void *rec )
{

  p_item v = o->first(o);
  p_item f = v;
  l_long index = 0;

  if ( v && rec )

    do {

      if ( v->rec == rec ) return index;

      v = v->next;

      index++;

    } while ( v != f );

  return -1;

};


l_long  list_index_of_item ( p_list o, p_item item )
{

  p_item v = o->first(o);
  p_item f = v;
  l_long index = 0;

  if ( v && item )

    do {

      if ( v == item ) return index;

      v = v->next;

      index++;

    } while ( v != f );


  return -1;

};


p_item  list_find_rec ( p_list o, void *rec )
{

  p_item v = o->first(o);
  p_item f = v;

  if ( v && rec )

    do {

      if ( v->rec == rec ) return v;

      v = v->next;

    } while ( v != f );


  return NULL;

};


l_long  list_insert_ex ( p_list o, void *rec, void (*f_free)(void *), l_tag tag )
{

  p_item xp = o->find_rec(o, rec);

  if ( !xp ) {

      p_item v = (p_item)_malloc(sizeof(t_item));

      if ( !v ) return -1;

      clear_type(v, sizeof(t_item));

      v->rec = rec;
      v->func_free = f_free;
      l_tag_cpy(v->tag, tag);
      v->next = v;
      v->prev = v;

      if ( !o->last ) o->last = v;

      else {

         v->prev = o->last;
         v->next = o->first(o);
         o->first(o)->prev = v;
         o->last->next = v;
         o->last = v;

      };

      return o->index_of_item(o, v);

  };

  return o->index_of_item(o, xp);

};


l_long  list_insert ( p_list o, void *rec )
{

  return o->insert_ex(o, rec, o->func_free, o->tag);

};


l_long  list_get_max_item ( p_list o )
{

  return o->index_of_item(o, o->last);

};


void    list_remove_index ( p_list o, l_long index )
{

  p_item v = o->at_item(o, index);

  o->remove_item(o, v);

};


void    list_remove_item ( p_list o, p_item item )
{
  if ( item ) {

    p_item x = item->prev;

    if ( o->last == item ) o->last = x;
    if ( o->last == item ) o->last = NULL;

    x->next = item->next;
    item->next->prev = x;

    item->next = NULL;
    item->prev = NULL;

    _free(item);

  };
};


void    list_free_index ( p_list o, l_long index )
{

  p_item v = o->at_item(o, index);

  o->free_item(o, v);

};


void    list_free_item ( p_list o, p_item item )
{

  if ( item ) {

    if ( item->func_free ) item->func_free(item->rec);
    else
    if ( o->func_free ) o->func_free(item->rec);

    o->remove_item(o, item);

  };

};



void    list_free_all ( p_list o ) {

  //p_item v = o->last;

  while ( o->last ) {

    o->free_item(o, o->last);

  };

  o->last = NULL;
  l_tag_cpy(o->tag, TAG_NONE);
  o->func_free = NULL;

};


l_bool  list_collect_by_name_from ( p_list o, p_item from, l_int rec_delta )
{

   p_item f = o->first(o);
   p_item m = from;

   if ( !from ) from = m = f;

   else from = m = from->next;

   if ( from && m )

   do {

      from = m;

      do {

         if ( !l_tag_cmp(o->tag, DAT_TEXT) && from->rec && m->rec ) { /* it's structure */

            struct s_t {
               l_text s;
            };

            l_text str1 = ((struct s_t*)from->rec)->s;
            l_text str2 = ((struct s_t*)m->rec)->s;


            if ( str1 && str2 && stricmp(str1, str2) < 0 ) {

                  void *old = m->rec;

                  m->rec = from->rec;
                  from->rec = old;

            };


         } else { /* it's text */

            l_text str1 = from->rec;
            l_text str2 = m->rec;

            if ( str1 && str2 && stricmp(str1, str2) < 0 ) {

                  void *old = m->rec;

                  m->rec = from;
                  from->rec = old;

            };

         };

         from = from->next;

      } while ( from != f );

      m = m->next;

   } while ( m != f );

   return true;

};



void  dispose_list ( p_list *o, l_bool freeitem )
{

   if ( o && (*o) ) {

     if ( freeitem )

         (*o)->free_all(*o);
     else
         (*o)->done(*o);

     _free(*o);

     (*o) = NULL;

   };

};


p_list  _list_init ( p_list o, void (*func_free)(void *), l_tag tag )
{

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_list));

  o->func_free = func_free;
  l_tag_cpy(o->tag, tag);

  /* list's functions */


  o->done = &list_done;

  o->copy_ctx = &list_copy_ctx;
  o->first_rec = &list_first_rec;
  o->first = &list_first;
  o->at_item = &list_at_item;
  o->at = &list_at;
  o->index_of = &list_index_of;
  o->index_of_item = &list_index_of_item;
  o->find_rec = &list_find_rec;
  o->insert_ex = &list_insert_ex;
  o->insert = &list_insert;
  o->get_max_item = &list_get_max_item;
  o->remove_index = &list_remove_index;
  o->remove_item = &list_remove_item;
  o->free_index = &list_free_index;
  o->free_item = &list_free_item;
  o->free_all = &list_free_all;
  o->for_each_item = &list_for_each_item;
  o->for_each_item_to_item = &list_for_each_item_to_item;
  o->collect_by_name_from = &list_collect_by_name_from;

  return o;
};

