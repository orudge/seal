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
#include <object.h>

#ifndef FIFO_BUFFER_SIZE
#define FIFO_BUFFER_SIZE   512
#endif

void _afree ( void **p )
{
   if ( p ) {
     if(*p)_free(*p);
     *p = NULL;
   };
};
// time functions //////////////////////////////////////////////////////////////

/*static*/ l_big atimer = 0;

void  aclock ( void )
{
   atimer += 20;
};

l_big time_get_mili ( void )
{
  return _time_get_mili();
};

l_big time_diff_mili ( l_big mili )
{
  return _time_diff_mili(mili);
};

// first in first out //////////////////////////////////////////////////////////

static char global_buf[FIFO_BUFFER_SIZE];

l_ptr fifo_add ( l_ptr list, l_ptr src, l_int size, l_int where )
{
   where = max(where, 0);
   if ( size <= 0 ) return NULL;
   if ( !list ) {
         list = _malloc(size);
         where = 0;
   } else
         list = _realloc(list, size*(where+1));
   if ( list )
       memcpy((l_ptr)((l_long)list+(where*size)), src, size);
   return list;
};

l_ptr fifo_get ( l_ptr *list, l_int size, l_int where )
{
   if ( !list || (size <= 0) ) return NULL;
   where = max(where, 0);
   if ( *list ) {

if (size>FIFO_BUFFER_SIZE ){
 DEBUG_printf("fifo buffer overflowed\n");
 exit(1);
}
         memcpy(global_buf, (l_ptr)((l_long)(*list)+(where*size)), size);
         if ( where > 0 )
            *list = _realloc(*list, size*where);
         else {
            _free(*list);
            *list = NULL;
        };
        return (l_ptr)(global_buf);
   };
   return NULL;
};

// object's function ///////////////////////////////////////////////////////////

l_bool     is_my_object ( t_object *o, t_object *e )
{

  if ( e ) {

    if ( e == o ) return true;

    else {

      p_object l;

      o = o->last;

      l = o;

      if ( o )

        do {

          if ( is_my_object(o, e) ) return true;

          o = o->next;

        } while ( o != l );

    };

  };

  return false;

};


l_bool     is_active ( t_object *e )
{

   if ( e )

     while ( e ) {

        if ( IS_ACTIVE_PROCESS(e) &&
            !e->is_options(e, OB_OF_NOTACTIVATE) )  return true;

        e = e->owner;

     };

  return false;

};


void  init_stillprocess ( p_object o, l_int milis )
{
   if ( !o ) return;

   if ( milis > 0 ) {

      o->process_tick = milis;
      o->set_options(o, OB_OF_STILLPROCESS, true);
      o->put_into_stillprocess(o, o);

   } else if ( o->options & OB_OF_STILLPROCESS ) {

      o->process_tick = 0;
      o->clear_from_stillprocess(o, o);
      o->set_options(o, OB_OF_STILLPROCESS, false);

   };

};



/* object functions */

l_bool  obj_done ( p_object o )
{
  if ( obj_exist(o) > 0 ) {

      t_object *l = o->last;
      t_object *x = l;

      done_stillprocess(o); /* get out from list of still-processing objects */
                            /* it's also in obj_remove, but... */

      if ( l ) /* if there is sub object */
      do {
         t_object *v = l->next;
         dispose(o->last);
         l = v;
      } while ( o->last );

      if ( o->owner ) {

         p_object owner = o->owner;
         owner->remove(owner, o);
         owner->reset_prefer(owner);

      };

      o->tag = TAG_DISPOSE;

      return true;
  };

  return false;
};



l_long    obj_index_of ( p_object o, p_object p )
{
  p_object  t = o->first(o);
  p_object  f = t;
  l_long i = 0;
  if ( !t || !p ) return 0;
  else
    do
     {
       if ( t == p ) return i;
       t = t->next;
       i++;
     } while ( t != f );
  return 0;
};


p_object  obj_at ( p_object o, l_long index )
{
  p_object  t = o->first(o);
  p_object  f = t;
  l_long i = 0;
  if ( !t || index < 0 ) return NULL;
  else
    do
     {
       if ( i == index ) return t;
       i++;
       t = t->next;
     } while (( t != f ) && ( i <= index ));
  return NULL;
};


p_object obj_find_match ( p_object o, l_dword sta, l_dword opt, l_bool forward )
{
  p_object s = o;

  if ( forward ) { /* forward */

    do {

      o = o->next;

      if ( o->is_options(o, OB_OF_ENABLE) && (!sta || o->is_state(o, sta)) &&
           (!opt || o->is_options(o, opt)) )
        return o;

    } while ( o != s );

  } else /* backward */

    do {

      o = o->prev;

      if ( o->is_options(o, OB_OF_ENABLE) && (!sta || o->is_state(o, sta)) &&
           (!opt || o->is_options(o, opt)) )
        return o;

    } while ( o != s );

  return NULL;
};


p_object obj_find_match_view ( p_object o, l_dword sta, l_dword opt, l_bool forward )
{
  p_object s = o;

  if ( forward ) { /* forward */

    do {

      o = o->next_view(o);

      if ( o->is_options(o, OB_OF_ENABLE) && (!sta || o->is_state(o, sta)) &&
           (!opt || o->is_options(o, opt)) )
        return o;

    } while ( o && o != s );

  } else /* backward */

    do {

      o = o->prev_view(o);

      if ( o->is_options(o, OB_OF_ENABLE) && (!sta || o->is_state(o, sta)) &&
           (!opt || o->is_options(o, opt)) )
        return o;

    } while ( o && o != s );

   return NULL;
};


p_object  obj_call_trans_events ( p_object o, l_dword st, l_dword op )
{

};


p_object  obj_owner_view ( p_object o )
{
  if ( o->owner ) {

    if ( o->owner->tag & TAG_VIEW ) return o->owner;
    else return o->owner->owner_view(o->owner);

  } else return NULL;
};


p_object  obj_next_view ( p_object o )
{
  t_object *t = o;

  do {

    t = t->next;

    if ( t->tag & TAG_VIEW ) return t;

  } while ( t != o );

  return NULL;
};


p_object  obj_prev_view ( p_object o )
{
  t_object *t = o;

  do {

    t = t->prev;

    if ( t->tag & TAG_VIEW ) return t;

  } while ( t != o );

  return NULL;
};


p_object  obj_last_view ( p_object o )
{

  if ( !o->last ) return NULL;

  if ( o->last->tag & TAG_VIEW ) return o->last;
  else {
    t_object *p = o->last->next_view(o->last);
    if ( p ) /* last view exist */
      return p;
    else /* sub-last view doesn't exist */
      return o->last->last_view(o->last);
  };

};


p_object  obj_first_view ( p_object o )
{
  if ( !o->first(o) ) return NULL;

  if ( o->first(o)->tag & TAG_VIEW ) return o->first(o);
  else {
    t_object *p = o->first(o)->prev_view(o->first(o));
    if ( p ) /* first view exist */
      return p;
    else
      return o->first(o)->first_view(o->first(o));
  };

};


p_object  obj_prev_view_to_first ( p_object o )
{
  if ( o->owner )
    if ( o != o->owner->first_view(o->owner) ) return o->prev_view(o);

  return NULL;
};


p_object  obj_next_view_to_last ( p_object o )
{
  if ( o->owner )
    if ( o != o->owner->last_view(o->owner) ) return o->next_view(o);

  return NULL;
};


p_object  obj_prev_to_first ( p_object o )
{
  if ( o->owner )
    if ( o != o->owner->first(o->owner) ) return o->prev;

  return NULL;
};


p_object  obj_next_to_last ( p_object o )
{
  if ( o->owner )
    if ( o != o->owner->last ) return o->next;

  return NULL;
};


void       obj_setup ( p_object o )
{
  if ( o->owner ) o->owner->reset_prefer(o->owner);
};


void       obj_after_init ( p_object o )
{

};


l_bool     obj_select ( p_object o )
{
  if ( !o->is_options(o, OB_OF_ENABLE) || !o->is_state(o, OB_SF_VISIBLE) )

    return false;

  if ( !o->is_options(o, OB_OF_SELECTABLE) )

    return false;

  if ( o->is_options(o, OB_OF_TOPSELECT) && o->owner )

    o->put_in_front_of(o, o->owner->first_view(o->owner));

  else if ( o->owner ) o->owner->set_prefer(o->owner, o);

  return true;
};


int        obj_put_into_stillprocess ( p_object o, p_object s )
{
  if ( o->owner ) o = o->owner;

  o->put_into_stillprocess(o, s);

};


int        obj_clear_from_stillprocess ( p_object o, p_object s )
{
  if ( o->owner ) o = o->owner;

  o->clear_from_stillprocess(o, s);

};


t_object*  obj_insert ( p_object o, p_object sub )
{
  if ( sub ) sub->after_init(sub);

  if ( o->last && sub && sub->is_options(sub, OB_OF_TOPSELECT) )

    o->insert_before(o, sub, o->first(o));

  else o->insert_before(o, sub, NULL);

  if ( sub ) {

    if ( sub->is_options(sub, OB_OF_STILLPROCESS) ) {

      init_stillprocess(sub, sub->process_tick);

    };

    sub->setup(sub);

  };

  return sub;
};


t_object*  obj_insert_before ( p_object o, p_object sub, p_object before )
{
  p_object t = before;

  if ( !sub ) return NULL;

  sub->owner = o;

  if ( before )
  {
    before  = before->prev;
    sub->next = t;
    before->next = sub;
    t->prev = sub;
    sub->prev = before;
  }
  else
  {
    if ( !o->last )
    {

      sub->next = sub;
      sub->prev = sub;

    } else
    {
      t = o->last->next;
      sub->prev = o->last;
      sub->next = t;
      t->prev = sub;
      o->last->next = sub;
    };
    o->last = sub;
  };

  return sub;

};


void obj_put_in_front_of ( p_object o, p_object before )
{
  if ( o->owner ) {
    p_object owner = o->owner;
    owner->set_prefer(owner, o);
    owner->remove(owner, o);
    owner->insert_before(owner, o, before);
  };
};


t_object*  obj_first ( p_object o )
{
  if ( o->last ) return o->last->next; else return NULL;
};


void       obj_set_state ( p_object o, l_dword st, l_bool set )
{
  if ( set ) o->state |= st;
  else o->state &= ~st;

  if ( st & OB_SF_FOCUSED ) { /* focus flag for sub selected object */

    p_object p = o->first(o);

    if ( p ) {

      p = p->find_match(p, set?OB_SF_SELECTED:OB_SF_FOCUSED, 0, true);

      if ( p )

        p->set_state(p, OB_SF_FOCUSED, set);

    };

  };

};


l_bool     obj_is_state ( p_object o, l_dword st )
{
  return (l_bool)(o->state & st);
};


void       obj_set_options ( p_object o, l_dword op, l_bool set )
{

  if ( set ) o->options |= op;
  else o->options &= ~op;

};


l_bool     obj_is_options ( p_object o, l_dword op )
{
  return (l_bool)(o->options & op);
};


void       obj_remove ( p_object o, p_object sub )
{

  t_object* t = NULL;
  if ( !o->last || !sub ) return;
  if ( sub->next != sub )
  {
    done_stillprocess(sub);
    t = sub->prev;
    t->next = sub->next;
    t->next->prev = t;
    if ( sub == o->last ) o->last = t;
    if ( sub == o->prefer ) o->prefer = NULL;
  }
  else {
    done_stillprocess(sub);
    o->last = NULL;
    o->prefer = NULL;
  };
  sub->owner = NULL;
};


l_dword    obj_valid ( p_object o, l_dword msg )
{

  p_object f = o->first(o);
  p_object p = f;

  if ( f )
  do {

     l_dword msg = f->valid(f, msg);

     if ( msg ) return msg;

  } while ( p != f );

  return 0;

};


void       obj_get_event ( p_object o, t_event *event )
{

  if ( o->owner ) o = o->owner;

  o->get_event(o, event);

};


void       obj_put_event ( p_object o, t_event *event )
{

  if ( o->owner ) o = o->owner;

  o->put_event(o, event);

};


l_dword    obj_execute ( p_object o )
{
/*  t_event event;*/
  clear_event(&event_main);

  do {
    o->end_state = 0;

    do {

      o->get_event(o, &event_main);

      INTMAIN(&event_main);

      PLAY_PROCESS(o, &event_main);

    } while ( !o->end_state );

  } while ( o->valid(o, o->end_state) != 0 );

  return o->end_state;

};


void  obj_reset_prefer ( p_object o )
{
  o->set_prefer(o, o->first(o));
};


void  obj_set_prefer ( p_object o, p_object prefer )
{
  #define defSelect(_p, _enable ) \
      if ( _p && (!_enable||(_p)->is_options(_p, OB_OF_SELECTABLE))    \
          && (_p)->is_options(_p, OB_OF_ENABLE))                       \
            (_p)->set_state(_p, OB_SF_SELECTED, _enable);

  #define  defFocus(_p, _enable ) \
      if ( _p && o->is_state(o, OB_SF_FOCUSED) && (!_enable||(_p)->is_options(_p, OB_OF_SELECTABLE)) \
           && (_p)->is_options(_p, OB_OF_ENABLE) ) \
        (_p)->set_state(_p, OB_SF_FOCUSED, _enable);

  if ( o->prefer != prefer )
  {
    defSelect(prefer, TRUE);
    defSelect(o->prefer, FALSE);
    defFocus(prefer, TRUE);
    defFocus(o->prefer, FALSE);
    o->prefer = prefer;
  };

};


void       obj_for_each_event ( p_object o, t_event *event )
{
  t_object *f = o->first(o);
  t_object *x = f;

  if ( f )

    do {

      if ( IS_OKTOSUBPROCESS(x) && x != o->prefer )

            PLAY_PROCESS(x, event);

      x = x->next;

    } while ( x != f );
};


void       obj_play_process ( p_object o, t_event *event )
{
  if ( !o ) return;

  if ( !IS_ACTIVE_PROCESS(o) || o->is_options(o, OB_OF_NOTACTIVATE) ||

        EV_IGNORE_ACTIVE_PROCESS(event->type)
  ) {

    ACTIVE_PROCESS(o);

    event_stop = o;

    o->translate_event(o, event);

    PASSIVE_PROCESS(o);

  };

};



void       obj_translate_event ( p_object o, t_event *event )
{

  if ( o->prefer && IS_OKTOSUBPROCESS(o->prefer) )

    PLAY_PROCESS(o->prefer, event);


  o->for_each_event(o, event);


  /* object messages */

  if ( event->type & EV_MESSAGE )

     switch ( event->message ) {

         case MSG_PASTE : if ( o == event->obj ) { /* paste data */

                              o->set_data(o, &clipboard); /* from clipboard */

                              clear_event(event);
         }; break;

         case MSG_COPY  : if ( o == event->obj ) { /* copy data */

                              clipboard.style = DS_WHATEVER; /* get what object want to sent */

                              o->get_data(o, &clipboard); /* to clipboard */

                              clear_event(event);
         }; break;

         case MSG_CUT  : if ( o == event->obj ) { /* delete data */

                              clipboard.style = DS_WHATEVER;

                              o->get_data(o, &clipboard);

         };
         case MSG_DELETE : if ( o == event->obj ) {

                              t_data delclip;

                              clear_type(&delclip, sizeof(t_data)); /* from object */

                              delclip.style = DS_DELETE|DS_WHATEVER; /* delete what object want to */

                              l_tag_cpy(delclip.id, o->data_type); /* id of clipboard = o->data_type */

                              o->set_data(o, &delclip);

                              clear_event(event);
         }; break;

     };


};


/*

  l_bool  t_object . get_data(t_object *o, t_data *rec)


  - function that is called even when some data are draged from the object,
    or data are copied from this object into clipboard, etc.

  - see set data for t_data structure.

  - return true, if data was copied from the object, else return false

  - usually used structure of function :

  if ( rec ) {

     rec->info_obj = o;
     rec->id = _your_id;

     switch ( rec->style ) {

       case DS_SELECTED : {

            rec->data = _your_selected_new_pointer;

       }; break;

       case DS_ALL : {

          rec->data = _your_alldata_new_pointer;

          return true;

       }; break;

       case DS_WHATEVER : {

          rec->style = DS_SELECTED;

          return OBJECT(o)->get_data(OBJECT(o), rec);

       }; break;

     };

     rec->id = DAT_NONE;

  };

  return false;

*/

l_bool     obj_get_data ( p_object o, t_data *rec )
{
  return false;
};



/*

  t_object . set_data(t_object *o, t_data *rec)


  - function that is called even when some data are droped into the object,
    or data are pasted into object from clipboard, etc. set_data function
    may set data only if o->id is same to accepted (id's) that you select in
    t_object . data_type variable.

    - rec is pointer to t_data structure, that contains following information:

         l_tag     id;       - identification of data, may contains more
                               than one bit settings

            DAT_NONE    - none data type, it not contains any data
            DAT_TEXT    - t_data->data is text end by zero ( C text )
            DAT_IMAGE   - t_data->data contains pointer to Allegro BITMAP
            DAT_FILE    - t_data->data contains special info about file,
                          see t_fat structure that you will find in
                          _iodir.h file.
            DAT_LIST    - t_data->data is pointer to p_list structure. See id
                          of this ( t_list ) structure.
            DAT_ALLKNOW - all data (id's) are accepted, it's used in trash
                          BIN f.e.


         l_dword   style; - styles of t_data paste/copy

            DS_ALL       - get all data from object in current (id).
            DS_SELECTED  - get only selected data from object
            DS_WHATEVER  - object select, what style it want to copy to
                           clipboard
            DS_DELETE    - delete data from object. ( This may be combined
                           with previous styles. DS_ALL|DS_DELETE => delete
                           all data from object, DS_SELECTED|DS_DELETE => delete
                           only what's selected in object...)

         l_ptr    *data; - pointer to data ( l_ptr = void ), you get what
                           type of pointer it's from (id).

         p_object  info_obj; - pointer to object, from where were data copied
                               to clipboard.

  - return true, if data was pasted into this object, else return false


  - default structure of function :


  l_bool ok  = false;

  if ( rec && l_tag_cmp(o->data_type, rec->id) ) {

     if ( rec->style & DS_DELETE ) {

       if ( o->is_state(o, OB_SF_FOCUSED) )  { delete only if focused


         if ( rec->style & DS_ALL ) {

               delete all

               ok = true;

         } else {

               delete only_selected

               if ( was_some_selected ) ok = true;

               else return false;

         };

       };

     } else {  data are pasted into object

        paste_data;

        ok = true;

     };

     if ( ok )

         some redraw function;

  };

  return ok;

*/

l_bool     obj_set_data ( p_object o, t_data *rec )
{
  return false;
};


/*
  - function select data in object,

  - data_syle   can be one of following :

            DS_ALL       - select all data in object ( default, also 0 )
            ...

  - set  - true if select else unselect

    return true if selected/unselected, else false

*/

l_bool     obj_select_data ( p_object o, l_int data_style, l_bool set )
{
   return false;
};




t_object* dispose ( t_object *o )
{

  if ( o && o->done(o) )

     _free(o);

  return NULL;

};


void  dispose_all ( t_object *o )
{

   if ( obj_exist(o) > 0 ) {

      t_object *l = OBJECT(o)->last;
      t_object *x = l;

      if ( l ) /* if there is sub object */
      do {
         t_object *v = l->next;
         l->done(l);
         l = v;
      } while ( o->last );

      o->last = NULL;

      o->tag = TAG_DISPOSE;

   };

};



void      message_info ( p_object o, l_dword type, l_dword message, p_object obj, void *info )
{

  if ( o ) {

    t_event ev;

    set_event_info(&ev, type, message, obj, info);

    PLAY_PROCESS(o, &ev);

  };

};


void      message_all_info ( l_dword type, l_dword message, p_object obj, void *info )
{

   message_info(&program, type, message, obj, info);

};


void      set_event ( t_event *event, l_dword type, l_dword message, p_object obj )
{
  t_event e;
  clear_event(&e);
  e.type = type;
  e.message = message;
  e.obj = obj;
  (*event) = e;
};


void      set_event_info ( t_event *event, l_dword type, l_dword message, p_object obj, void *rec )
{
  t_event e;
  clear_event(&e);
  e.type = type;
  e.message = message;
  e.obj = obj;
  e.info = rec;
  (*event) = e;
};


l_ptr    copy_type ( l_ptr what, l_long size )
{

   void *ptr = NULL;

   if ( what && size > 0 )

       ptr = _malloc(size);

   if ( ptr )

      memcpy(ptr, what, size);

   return ptr;
};


static void  obj_func_nothing ( p_object o )
{
};



/* t_object */

t_object *_obj_init ( t_object *o )
{
  if ( !o ) return NULL;

  memset(o, 0, sizeof(t_object));

  o->options = OB_OF_ENABLE;
  o->tag = TAG_OBJECT;
  o->process_tick = 20;

  o->owner = NULL;

  o->find_match_view = &obj_find_match_view;
  o->find_match = &obj_find_match;

  o->owner_view = &obj_owner_view;
  o->put_in_front_of = &obj_put_in_front_of;

  o->func_callback = &obj_func_nothing;
  o->next_view = &obj_next_view;
  o->prev_view = &obj_prev_view;
  o->last_view = &obj_last_view;
  o->first_view = &obj_first_view;
  o->prev_to_first = &obj_prev_to_first;
  o->next_to_last = &obj_next_to_last;
  o->prev_view_to_first = &obj_prev_view_to_first;
  o->next_view_to_last = &obj_next_view_to_last;

  o->set_prefer = &obj_set_prefer;
  o->reset_prefer = &obj_reset_prefer;

  o->select = &obj_select;
  o->set_state = &obj_set_state;
  o->is_state = &obj_is_state;
  o->set_options = &obj_set_options;
  o->is_options = &obj_is_options;
  o->get_data = &obj_get_data;
  o->set_data = &obj_set_data;
  o->select_data = &obj_select_data;

  o->at = &obj_at;
  o->index_of = &obj_index_of;

  o->play_process = &obj_play_process;
  o->translate_event = &obj_translate_event;
  o->for_each_event = &obj_for_each_event;
  o->first = &obj_first;
  o->after_init = &obj_after_init;
  o->setup = &obj_setup;
  o->insert_before = &obj_insert_before;
  o->insert = &obj_insert;
  o->remove = &obj_remove;
  o->done = &obj_done;
  o->valid = &obj_valid;

  o->put_into_stillprocess = &obj_put_into_stillprocess;
  o->clear_from_stillprocess = &obj_clear_from_stillprocess;

  o->get_event = &obj_get_event;
  o->put_event = &obj_put_event;
  o->execute = &obj_execute;

  return o;

};


t_object *(*obj_init) ( t_object *o ) = &_obj_init;
