/****************************************************************/
/*                                                              */
/*                           list.h                             */
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



#ifndef __LIST_H_INCLUDED__
#define __LIST_H_INCLUDED__


/* memory delta to name in item */

#define ITEM_NAME       0  /* standard value, where name of rec start */



typedef struct t_list *p_list;
typedef struct t_item *p_item;


/* t_item structure */

/*
 List items are defined in t_item structure :
*/

typedef struct t_item {

  /* point to data of the item */
  void   *rec;
  /* identification of data */
  l_tag   tag;
  /* function for data (rec) releasing */
  void  (*func_free) ( void* );

  /* prev item in group of items */
  p_item  prev;
  /* next item in group of items */
  p_item  next;

  /* reserved for later versions */
  l_char  reserved[12];

} t_item;


/* t_list */

typedef struct t_list {

  /* point to last item in data group. First item is defined as
     last->next.
  */
  p_item   last;

  /* default identification of data, that's placed in t_item.rec */
  l_tag    tag;

  /* default function for releasing of data. default function is
     free(void*) function. If some item t_item.func_free is set to
     ZERO,this default function func_free is called in releasing of data
     t_data.rec.
  */
  void   (*func_free) ( void* );

  /* reserved for next versions of SEAL */
  l_char   reserved[48];


  /* removes items from list, but not frees data in items. Return true
     if list was succesfull destroyed. Otherwise returns false.
  */
  l_bool (*done)( p_list o );

  /* copy context of list (dst) to end of another list (src). */
  void   (*copy_ctx) ( p_list dst, p_list src );

  /* returns first data in list. Same as last->next->rec, but tests
     existance of all pointers
  */
  void  *(*first_rec) ( p_list o );

  /* return first item from the list */
  p_item (*first) ( p_list o );

  /* return the (index)th item from the list */
  p_item (*at_item)( p_list o, l_long index );

  /* return the (index)th data from the list */
  void  *(*at)( p_list o, l_long index );

  /* return the position of item, where data (rec) are placed in the list */
  l_long (*index_of)( p_list o, void *rec );

  /* return the position where item is stored in the list */
  l_long (*index_of_item)( p_list o, p_item item );

  /* return item that contains data "rec" */
  p_item (*find_rec)( p_list o, void *rec );

  /* insert new data to item, that will be inserted into the list.
     (rec) will be deallocated by function (f_free). If this function
     is NULL, it use default list' func_free function. Return index of
     new item.
  */
  l_long (*insert_ex)( p_list o, void *rec, void (*f_free)(void *), l_tag tag );

  /* same as insert_ex, but 3rd and 4th arguments are set to ZERO. So
     use defaults values defined by list_init function. Return index
     of new item.
  */
  l_long (*insert)(p_list o, void *rec);

  /* return number of items in list. */
  l_long (*get_max_item)(p_list o);

  /* remove the (index)th item from list, but not frees data
     t_item.rec
  */
  void   (*remove_index)(p_list o, l_long index );

  /* remove item from list, but not frees data from item->rec */
  void   (*remove_item)(p_list o,  p_item item );

  /* remove the (index)th item from list, and frees data from the item
     t_item.rec pointer and also free memory of item strucure
  */
  void   (*free_index)(p_list o, l_long index );

  /* remove item from list, t_item.rec pointer and also free memory of
     item strucure.
  */
  void   (*free_item)(p_list o, p_item item );

  /* remove all items from the list, and frees data from each item and
     also free memory allocated for each item. This memory is
     allocated in function insert_ex.
  */
  void   (*free_all)(p_list o );

  /* calls function callback by arguments (ob), (item->rec) and (ind)
     for each item's data (item->rec) in the list. If callback returns
     ZERO process will be broken, otherwise continue. Returns true, if
     process was broken, else false if not.
  */
  l_bool (*for_each_item) ( p_list o, void *ob, l_int (*callback)(), l_dword *ind );

  /* calls function callback by arguments (ob), (item), (item->rec) and
     (ind) for each item's data (item->rec) in the list. If callback
     returns ZERO process will be broken, otherwise continue. Returns
     true, if process was broken, else false if not.
  */
  l_bool (*for_each_item_to_item) ( p_list o, void *ob, void *item, l_int (*callback)(), l_dword *ind );

  /* collects data by name. List must contains group of text data or
     structures where text is ocurred in first position. (from) is
     pointer from which list must be collect data (NULL = from last
     item) to last item. If tag of item->tag is DAT_TEXT it means that
     item->rec is string, otherwise use structure, where first 4 bytes
     are pointer to string. Returns true.
  */
  l_bool (*collect_by_name_from)(p_list o, p_item from, l_int rec_delta);


} t_list;

#define collect_by_name(o, d)  o->collect_by_name_from(o, NULL, d)

/*
  disposes list structure and if freeitem is true, it also frees context
  of each t_item.rec pointer in the list. Use this function for releasing
  t_list structure.
*/
void    dispose_list ( p_list *o, l_bool freeitem );


/*
  create new t_list, where (o) is pointer to new t_list structure,
  (func_free) is function that will be used for data releasing and
  (tag) is identification number of data ( default is 0 ).
  function returns new object of t_list structure.

  example :

  p_list p = list_init(malloc(sizeof(t_list)), &free, 0);

  p->insert(p, (void*)strdup("Hello world !"));
  p->insert(p, (void*)strdup("SEAL is free desktop environmnet"));
  p->insert(p, (void*)strdup("Hello Mr. Stencl !"));
  collect_by_name(p, 0);

  ...now list (p) contains 3 items, and in the each of them are strings.

  p->free_index(p, 0);

  this function removes 0th item, that contains ("Hello world !")
  data and this data are free by function free(void*), that was declared in
  list_init function.

  free list and memory of each string from the item (rec)

  dispose_list(p, true);

*/
extern p_list  (*list_init) ( p_list o, void (*func_free)(void *), l_tag tag );

p_list  _list_init ( p_list o, void (*func_free)(void *), l_tag tag );

l_bool  list_done ( p_list o );
void    list_copy_ctx ( p_list dst, p_list src );
l_bool  list_for_each_item ( p_list o, void *ob, l_int (*callback)(), l_dword *ind );
l_bool  list_for_each_item_to_item ( p_list o, void *ob, void *item, l_int (*callback)(), l_dword *ind );
void   *list_first_rec ( p_list o );
p_item  list_first ( p_list o );
p_item  list_at_item ( p_list o, l_long index );
void   *list_at ( p_list o, l_long index );
l_long  list_index_of ( p_list o, void *rec );
l_long  list_index_of_item ( p_list o, p_item item );
p_item  list_find_rec ( p_list o, void *rec );
l_long  list_insert_ex ( p_list o, void *rec, void (*f_free)(void *), l_tag tag );
l_long  list_insert ( p_list o, void *rec );
void    list_remove_index ( p_list o, l_long index );
void    list_remove_item ( p_list o, p_item item );
void    list_free_index ( p_list o, l_long index );
void    list_free_item ( p_list o, p_item item );
void    list_free_all ( p_list o );
l_long  list_get_max_item (p_list o);
l_bool  list_collect_by_name_from ( p_list o, p_item from, l_int rec_delta_to_name );


#endif

