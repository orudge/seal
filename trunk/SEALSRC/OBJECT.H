/****************************************************************/
/*                                                              */
/*                          object.h                            */
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



#ifndef __OBJECT_H_INCLUDED__
#define __OBJECT_H_INCLUDED__

#include<stdio.h>
#include<string.h>
#include"dataini.h"

#ifndef TXT_NOTENOUGHMEMEMORYFOROPERATION
#define TXT_NOTENOUGHMEMEMORYFOROPERATION INI_TEXT("Not enough memory for operation")
#endif
#ifndef TXT_PLEASESTOREYOURDATAANDEXITSOMEAPPLICATION
#define TXT_PLEASESTOREYOURDATAANDEXITSOMEAPPLICATION INI_TEXT("Please store your data and exit some application")
#endif

#ifndef strsize
#define strsize(p,v)    ((v)?((v)-(p)):strlen(p))
#endif
#ifndef min
#define min(x,y)    (((x) < (y)) ?  (x) : (y))
#endif
#ifndef max
#define max(x,y)    (((x) > (y)) ?  (x) : (y))
#endif
#ifndef imin
#define imin(x,y)   min((l_int)(x),(l_int)(y))
#endif
#ifndef imax
#define imax(x,y)   max((l_int)(x),(l_int)(y))
#endif
#ifndef umin
#define umin(x,y)   min((l_word)(x),(l_word)(y))
#endif
#ifndef umax
#define umax(x,y)   max((l_word)(x),(l_word)(y))
#endif
#ifndef lmin
#define lmin(x,y)   min((l_long)(x),(l_long)(y))
#endif
#ifndef lmax
#define lmax(x,y)   max((l_long)(x),(l_long)(y))
#endif
#ifndef dmin
#define dmin(x,y)   min((l_dword)(x),(l_dword)(y))
#endif
#ifndef dmax
#define dmax(x,y)   max((l_dword)(x),(l_dword)(y))
#endif
#ifndef abs
#define abs(x)      (((x)<0)?-(x):(x))
#endif
#ifndef labs
#define labs(x)     abs((l_long)(x))
#endif

#ifndef             NULL
#define NULL        ((void*)0)
#endif

#ifndef true
#define true        1
#endif
#ifndef false
#define false       0
#endif

#ifndef TRUE
#define TRUE        true
#endif
#ifndef FALSE
#define FALSE       false
#endif

#define l_bool      int
#define l_long      long
#define l_int       int
#define l_char      char
#define l_rect      long
#define l_color     int

typedef l_char         l_tag[32];

#define l_byte         unsigned l_char
#define l_word         unsigned l_int
#define l_dword        unsigned l_long
#define l_big          l_long l_long
#define l_tag_cpy(x,y) tag_strcpy((l_text)x, (l_text)y, sizeof(l_tag));
#define l_tag_cmp(x,y) (!tag_stricmp(x,y))
#define l_ptr          void*
#define l_text         char*
#define l_process      l_bool
#define l_font         GrFONT
#define l_iconlibrary  ICONLIBRARY

#define INI_TEXT(x) x

#define clear_type(t,s)  if (t) memset(t, 0, s)
#define clear_table(t,s) clear_type(t,s)
#define clear_event(t)   clear_type(t,sizeof(t_event))

/* include filter functions */
#include"filter.h"


/* test file */

extern FILE  *seal_test_file;
extern FILE  *seal_debug_file;


l_int   DEBUG_printf ( l_text text, ...); /* only for internal use */

#define DEBUG_test(i,x,y) if ( i ) x; else y


/* main *.ini file */

extern   char *ini_mainfile;

#define  INI_MAINFILE         ini_mainfile /* name of main .ini file */


/* main program, must be defined in program.c file */

extern struct t_object   program;



/* error's types */

#define ERR_NONTHING         0
#define ERR_NONREGULAR       1
#define ERR_INFO             2



/* phase settings */

#define PH_PREPROCESS         0x0000001
#define PH_POSTPROCESS        0x0000002


/* options settings */

#define OB_OF_TOPSELECT       0x0000001 /* object is over the all objects */
#define OB_OF_ENABLE          0x0000002 /* object is enable for group */
#define OB_OF_SELECTABLE      0x0000004 /* object can be selected */
#define OB_OF_NOTACTIVATE     0x0000008
#define OB_OF_STILLPROCESS    0x0000010 /* object is still in process */


/* tag settings */

#define TAG_NONE              0x0000000
#define TAG_DISPOSE           TAG_NONE
#define TAG_OBJECT            0x0000001
#define TAG_VIEW              0x0000002
#define TAG_WINDOW            0x0000004
#define TAG_BUTTON            0x0000008



/* state settings */

#define  OB_SF_MODAL          0x0000001 /* process is call only for this object */
#define  OB_SF_VISIBLE        0x0000002 /* object is visible */
#define  OB_SF_SELECTED       0x0000004 /* object is select */
#define  OB_SF_FOCUSED        0x0000008 /* object is focused */



typedef struct t_object *p_object;


/*
  event types
*/
#define  EV_NOTHING           0x0000
#define  EV_MOUSE             0x0001
#define  EV_KEYBOARD          0x0002
#define  EV_MESSAGE           0x0004
#define  EV_INFO              0x0008
#define  EV_RETYPE            0x0010

#define  EV_IGNORE_ACTIVE_PROCESS(x) ((x) & EV_RETYPE)


/* standard retype messages */

#define  MSG_RETYPEPATH       1 /* this message redraw files, where event->info = path to file */


#define  RETYPE_FILES_IN_PATH(p)       message_all_info(EV_RETYPE, MSG_RETYPEPATH, NULL, (p))



/*
  standard messages
*/
#define  MSG_NOTHING          0
#define  MSG_QUIT             1
#define  MSG_CLOSE            2
#define  MSG_CANCEL           3
#define  MSG_COPY             4
#define  MSG_PASTE            5
#define  MSG_CUT              6
#define  MSG_OK               7
#define  MSG_YES              8
#define  MSG_NO               9
#define  MSG_HELP            10

#define  MSG_HISTORY         11
#define  MSG_SELECTALL       12

#define  MSG_CONTINUE        13
#define  MSG_EXIT            14
#define  MSG_YESTOALL        15
#define  MSG_NOTOALL         16


/*
  data style
*/

#define  DS_ALL        0x00000001  /* get all data from object */
#define  DS_SELECTED   0x00000002  /* get selected data from object */
#define  DS_WHATEVER   0x00000004  /* get data that object want to send */
#define  DS_DELETE     0x00000008  /* delete data from object call for set_data */


typedef struct t_data *p_data;

/*
  data structure
*/
typedef struct t_data {

  l_tag     id;
  l_dword   style;
  l_ptr     data;
  p_object  info_obj;

  l_char    reserved[12];

} t_data;


/* clipboard */

extern t_data  clipboard;

/* clipboard must be freed in all copy function */



typedef struct t_event *p_event;

/*
  event structure
*/
typedef struct t_event {

  l_dword   type;       /* type of event */
  l_dword   message;    /* message of event */
  p_object  obj;        /* pointer to object that made event */
  l_int     time;
  void     *info;

  l_char    reserved[20];

} t_event;


extern t_event   event_main;
extern l_big     event_timer;
extern p_object  event_stop;


/*
  t_point structure
*/

typedef struct t_point {

  l_rect    x;
  l_rect    y;

} t_point;


/*
  rectangle structure
*/
typedef struct t_rect {

  struct t_point   a;
  struct t_point   b;

} t_rect;


extern t_rect rect_empty;


/* process definition */

extern l_bool     go_process;

#define PROCESS_ACTIVE  true
#define PROCESS_PASSIVE false

#define ACTIVE_PROCESS(o)    OBJECT(o)->process = PROCESS_ACTIVE
#define PASSIVE_PROCESS(o)   OBJECT(o)->process = PROCESS_PASSIVE
#define REMOVE_PROCESS(o)    OBJECT(o)->process = PROCESS_PASSIVE
#define GET_PROCESS(o)       OBJECT(o)->process
#define RESET_PROCESS(o,pr)  OBJECT(o)->process = (pr)
#define IS_ACTIVE_PROCESS(o) (OBJECT(o)->process == PROCESS_ACTIVE)
#define PLAY_PROCESS(o,ev)   OBJECT(o)->play_process(o, ev)
#define IS_OKTOSUBPROCESS(o) 1

#define STOP_PROCESS()  go_process = false
#define START_PROCESS() go_process = true

int               program_int ( void );

#define INTMAIN(e)     program.translate_event(&program, e);


/*
  main object structure
*/
typedef struct t_object {

  l_dword   tag;          /* tag of object = id of object type */
  l_dword   state;        /* current state of object */
  l_dword   options;      /* options of object */
  l_dword   end_state;
  l_int     phase;        /* current phase of process for sub objects */
  l_int     tick;
  l_tag     data_type;
  l_process process;
  l_big     process_time;
  l_int     process_tick;

  p_object  owner;        /* the parent of object */
  p_object  next;         /* next object */
  p_object  prev;         /* previous object */
  p_object  last;         /* the last sub object */
  p_object  prefer;       /* the sub object, that is prefere */

  l_char    reserved[48];

  l_bool     (*done) ( p_object o );

  p_object   (*find_match_view) ( p_object o, l_dword sta, l_dword opt, l_bool forward );
  p_object   (*find_match) ( p_object o, l_dword sta, l_dword opt, l_bool forward );
  p_object   (*owner_view) ( p_object o );
  p_object   (*next_view) ( p_object o );
  p_object   (*prev_view) ( p_object o );
  p_object   (*last_view) ( p_object o );
  p_object   (*first_view) ( p_object o );
  p_object   (*next_to_last) ( p_object o );
  p_object   (*prev_to_first) ( p_object o );
  p_object   (*prev_view_to_first) ( p_object o );
  p_object   (*next_view_to_last) ( p_object o );

  void       (*setup) ( p_object o );
  void       (*after_init) ( p_object o );
  p_object   (*insert) ( p_object o, p_object sub );
  p_object   (*insert_before) ( p_object o, p_object sub, p_object before );
  void       (*put_in_front_of) ( p_object o, p_object before );
  void       (*remove) ( p_object o, p_object sub );
  p_object   (*first) ( p_object o );

  l_bool     (*select) ( p_object o );
  void       (*set_state) ( p_object o, l_dword st, l_bool set );
  void       (*set_options) ( p_object o, l_dword op, l_bool set );
  l_bool     (*is_state) ( p_object o, l_dword st );
  l_bool     (*is_options) ( p_object o, l_dword op );

  p_object   (*at) ( p_object o, l_long index );
  l_long     (*index_of) ( p_object o, p_object p );

  void       (*func_callback) ( p_object s );
  int        (*put_into_stillprocess) ( p_object o, p_object s );
  int        (*clear_from_stillprocess) ( p_object o, p_object s );

  l_bool     (*get_data) ( p_object o, t_data *rec );
  l_bool     (*set_data) ( p_object o, t_data *rec );
  l_bool     (*select_data) ( p_object o, l_int data_style, l_bool set );

  void       (*set_prefer) ( p_object o, p_object prefer );
  void       (*reset_prefer) ( p_object o );

  l_dword    (*valid) ( p_object o, l_dword msg );
  void       (*get_event) ( p_object o, t_event *event );
  void       (*put_event) ( p_object o, t_event *event );
  l_dword    (*execute) ( p_object o );

  void       (*play_process) ( p_object o, t_event *event );
  void       (*for_each_event) ( p_object o, t_event *event );
  void       (*translate_event) ( p_object o, t_event *event );


} t_object;

extern t_object   program;


#define OBJECT(o) ((p_object)(o))


/* others functions */

void _afree ( void **p );

#define afree(x) _afree((void**)(x));


/* time functions */

void  aclock ( void );
l_big time_get_mili ( void );
l_big time_diff_mili ( l_big mili );
#define _time_get_mili()  ((l_big)atimer)
#define _time_diff_mili(m) ((l_big)atimer-(l_big)(m))


/* string function */

l_text  set_format_text ( l_text *dest, l_text format, ... );
l_text  set_format_text_nice ( l_text *dest, l_int size, l_text format, ... );

l_text  insstr ( l_text dest, l_text str, l_long pos, l_long size );
l_text  delstr ( l_text dest, l_long pos, l_long size );
l_text  stridup ( l_text str, l_long num );
l_text  stristr ( l_text s, l_text d );


/* error's functions */
extern l_int  (*seal_error) ( l_int errtype, l_text str, ... );
l_int  _seal_error ( l_int errtype, l_text str, ... );


/* other object functions */

l_bool     is_active ( t_object *e );
l_bool     is_my_object ( t_object *o, t_object *e );
#define    is_my_message(o,e) is_my_object(o, e->obj)


/* first in first out */

l_ptr fifo_add ( l_ptr list, l_ptr src, l_int size, l_int where );
l_ptr fifo_get ( l_ptr *list, l_int size, l_int where );

#define fifo_add_rect(l,s,w) ((t_rect*)fifo_add((l_ptr)l, (l_ptr)s, sizeof(t_rect), w))
#define fifo_get_rect(l,w)   ((t_rect*)fifo_get((l_ptr*)l, sizeof(t_rect), w))


/* origin function */

t_point point_assign ( l_rect x, l_rect y );
t_rect  rect_assign ( l_rect ax, l_rect ay, l_rect bx, l_rect by );
t_rect  rect_move ( t_rect r, l_rect mx, l_rect my );
t_point rect_size ( t_rect r );
t_rect  rect_cliped ( t_rect r, t_rect d );
l_bool  rect_overlay ( t_rect r, t_rect d );
l_rect  rect_sizex ( t_rect r );
l_rect  rect_sizey ( t_rect r );
l_bool  rect_check_empty ( t_rect r );
l_bool  rect_equals ( t_rect r, t_rect nr );
l_bool  rect_contains ( t_rect r, t_point p );
void    rect_double_overlay ( t_rect* fr, t_rect *lr );


/* object functions */

  extern t_object *(*obj_init) ( t_object *o );
  t_object *_obj_init ( t_object *o );

  l_bool   obj_done ( p_object o );

  p_object obj_find_match_view ( p_object o, l_dword sta, l_dword opt, l_bool forward );
  p_object obj_find_match ( p_object o, l_dword sta, l_dword opt, l_bool forward );

  t_object*  obj_owner_view ( p_object o );
  t_object*  obj_next_view ( p_object o );
  t_object*  obj_prev_view ( p_object o );
  t_object*  obj_last_view ( p_object o );
  t_object*  obj_first_view ( p_object o );

  p_object  obj_next_to_last ( p_object o );
  p_object  obj_prev_to_first ( p_object o );

  t_object*  obj_prev_view_to_first ( p_object o );
  t_object*  obj_next_view_to_last ( p_object o );

  void       obj_after_init ( p_object o );
  void       obj_setup ( p_object o );
  t_object*  obj_insert ( p_object o, p_object sub );
  t_object*  obj_insert_before ( p_object o, p_object sub, p_object before );
  void       obj_put_in_front_of ( p_object o, p_object before );
  void       obj_remove ( p_object o, p_object sub );
  t_object*  obj_first ( p_object o );

  l_bool     obj_select ( p_object o );
  void       obj_set_state ( p_object o, l_dword st, l_bool set );
  void       obj_set_options ( p_object o, l_dword ot, l_bool set );
  l_bool     obj_is_state ( p_object o, l_dword st );
  l_bool     obj_is_options ( p_object o, l_dword ot );

  int        obj_put_into_stillprocess ( p_object o, p_object s );
  int        obj_clear_from_stillprocess ( p_object o, p_object s );

  l_bool     obj_get_data ( p_object o, t_data *rec );
  l_bool     obj_set_data ( p_object o, t_data *rec );
  l_bool     obj_select_data ( p_object o, l_int data_style, l_bool set );

  p_object   obj_at ( p_object o, l_long index );
  l_long     obj_index_of ( p_object o, p_object p );

  void       obj_set_prefer ( p_object o, p_object prefer );
  void       obj_reset_prefer ( p_object o );

  l_dword    obj_execute ( p_object o );
  l_dword    obj_valid ( p_object o, l_dword msg );
  void       obj_get_event ( p_object o, t_event *event );
  void       obj_put_event ( p_object o, t_event *event );

  void       obj_play_process ( p_object o, t_event *event );
  void       obj_for_each_event ( p_object o, t_event *event );
  void       obj_translate_event ( p_object o, t_event *event );

  t_object*  dispose ( t_object *o );
  void       dispose_all ( t_object *o );
  void       init_stillprocess ( p_object o, l_int milis );
  #define    done_stillprocess(o) init_stillprocess(o, -1)

  void       set_event ( t_event *event, l_dword type, l_dword message, p_object obj );
  void       set_event_info ( t_event *event, l_dword type, l_dword message, p_object obj, void *rec );
  void       message_all_info ( l_dword type, l_dword message, p_object obj, void *info );
  void       message_info ( p_object o, l_dword type, l_dword message, p_object obj, void *info );
  #define    message(o,t,m,ob) message_info(o,t,m,ob,NULL)
  #define    message_all(t,m,ob) message_all_info(t,m,ob,NULL)
  l_ptr      copy_type ( l_ptr  what, l_long size );

  #define    obj_exist(o) (o ? OBJECT(o)->tag: -1)

  #define WHILE(t)       while(program_int()&&(##t))
  #define FOR(t1,t2,t3)  for((##t1);program_int()&&(##t2);(##t3))
  #define _while(t)      WHILE(t)
  #define _for(t1,t2,t3) FOR(t1,t2,t3)


#endif

