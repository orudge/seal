/*****************************************************************/
/*                                                               */
/*                                                               */
/*  File     :     API.H                                         */
/*                                                               */
/*  Producer :     Interactive Studio                            */
/*                                                               */
/*        Programmers :  Michal Stencl                           */
/*                                                               */
/*                                                               */
/*  Copyright (c) 2000, Interactive Studio                       */
/*  All Rights Reserved                                          */
/*                                                               */
/*  Contacts :                                                   */
/*                                                               */
/*        Address :  Kozia 29, Bratislava 81103, Slovak Republic */
/*        E-mail  :  developer@InteractiveStudio.sk              */
/*        WEB     :  http://www.InteractiveStudio.sk/            */
/*                                                               */
/*                                                               */
/*  This file is part of MatrixOS                                */
/*                                                               */
/*****************************************************************/



#ifndef __SEAL_H_INCLUDED__
#define __SEAL_H_INCLUDED__

/*
  object.h - include file
*/

#ifndef __OBJECT_H_INCLUDED__
#define __OBJECT_H_INCLUDED__

#include<stdio.h>

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


/* safe mem declarations */

#ifndef __SAFMEM_H_INCLUDED__
#define __SAFMEM_H_INCLUDED__

#include<dpmi.h>

#define  __SAFE_MEMORY__
#ifdef   __SAFE_MEMORY__
#ifndef _malloc
#define _malloc        sf_malloc
#endif
#ifndef _free
#define _free          sf_free
#endif
#ifndef _realloc
#define _realloc       sf_realloc
#endif
#ifndef  _strdup
#define  _strdup       sf_strdup
#endif
#ifndef  _calloc
#define  _calloc       sf_calloc
#endif
#ifndef  _cfree
#define  _cfree        sf_cfree
#endif
#endif

void     tag_strcpy ( void *dst, void *src, l_int size );
l_int    tag_stricmp ( void *dst, void *src );

void     sf_alert ( void );
void    *sf_malloc ( size_t size );
void     sf_free ( void *rec );
void    *sf_realloc ( void *rec, size_t size );
void     sf_cfree ( void *rec );
void    *sf_calloc ( size_t num_elements, size_t size );
char    *sf_strdup ( const char *source );

#endif



/* test file */

extern FILE  *seal_test_file;
extern FILE  *seal_debug_file;


l_int   DEBUG_printf ( l_text text, ...); /* only for internal use */

#define DEBUG_test(i,x,y) if ( i ) x; else y


/* main *.ini file */

extern   char *ini_mainfile;

#define  INI_MAINFILE         ini_mainfile /* name of main .ini file */



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
#define  EV_NOTHING           0x00
#define  EV_MOUSE             0x01
#define  EV_KEYBOARD          0x02
#define  EV_MESSAGE           0x04
#define  EV_INFO              0x08
#define  EV_RETYPE            0x10

#define  EV_IGNORE_ACTIVE_PROCESS(x) ( (x) == EV_INFO || (x) == EV_RETYPE )



/* standard retype messages */

#define  MSG_RETYPEPATH       1 /* this message redraw files in path, where event->info = path to file */


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
  data types
*/
#define  DAT_NONE      "NONE"    /* none data type */
#define  DAT_TEXT      "TEXT"    /* single text ended by zero */
#define  DAT_IMAGE     "IMAGE"   /* type of BITMAP */
#define  DAT_TFILE     "TFILE"   /* file t_file structure */
#define  DAT_LIST      "LIST"    /* list ( p_list) */
#define  DAT_ALLKNOW   "ALLKNOW" /* all data types are accepted */
#define  DAT_NUMBER    "NUMBER"  /* type is the number */
#define  DAT_FLOAT     "FLOAT"   /* type is the number with floating point */
#define  DAT_HTML      "HTML"    /* type is the HTML format */


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


/*
  list.h - inlcude file
*/

#ifndef __LIST_H_INCLUDED__
#define __LIST_H_INCLUDED__


/* memory delta to name in item */

#define ITEM_NAME       0  /* standard value, where name of rec start */



typedef struct t_list *p_list;
typedef struct t_item *p_item;


/* t_item structure */

typedef struct t_item {

  void   *rec;
  l_tag   tag;
  void  (*func_free) ( void* );

  p_item  prev;
  p_item  next;

  l_char  reserved[12];
} t_item;


/* t_list */

typedef struct t_list {

  p_item   last;
  l_tag    tag;

  void   (*func_free) ( void* );

  l_char   reserved[48];


  l_bool (*done)( p_list o );

  void   (*copy_ctx) ( p_list dst, p_list src );
  void  *(*first_rec) ( p_list o );
  p_item (*first) ( p_list o );
  p_item (*at_item)( p_list o, l_long index );
  void  *(*at)( p_list o, l_long index );
  l_long (*index_of)( p_list o, void *rec );
  l_long (*index_of_item)( p_list o, p_item item );
  p_item (*find_rec)( p_list o, void *rec );
  l_long (*insert_ex)( p_list o, void *rec, void (*f_free)(void *), l_tag tag );
  l_long (*insert)(p_list o, void *rec);
  l_long (*get_max_item)(p_list o);
  void   (*remove_index)(p_list o, l_long index );
  void   (*remove_item)(p_list o,  p_item item );
  void   (*free_index)(p_list o, l_long index );
  void   (*free_item)(p_list o, p_item item );
  void   (*free_all)(p_list o );
  l_bool (*for_each_item) ( p_list o, void *ob, l_int (*callback)(), l_dword *ind );
  l_bool (*for_each_item_to_item) ( p_list o, void *ob, void *item, l_int (*callback)(), l_dword *ind );
  l_bool (*collect_by_name_from)(p_list o, p_item from, l_int rec_delta);


} t_list;

#define collect_by_name(o, d)  o->collect_by_name_from(o, NULL, d)


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


void    dispose_list ( p_list *o, l_bool freeitem );

#endif


/*
  ALLTOGRX.H for Uniforms

  by      Michal Stencl
  e-mail: stenclpmd@ba.telecom.sk]
*/


#ifndef __ALLTOGRX_H_INCLUDED__
#define __ALLTOGRX_H_INCLUDED__

#ifndef  GRAPHICS_TYPES
#define  GRAPHICS_TYPES
#define  define_nothing
#define  ScreenContext    screen
#define  GrRGB            RGB
#define  GrCOLOR          int
#define  GrCONTEXT        BITMAP
#define  GrRLE_SPRITE     RLE_SPRITE
#define  GrIMAGE          GrCONTEXT
#define  GrFONT           void*
#define  GrFIXED          fixed
#define  GrV3D            V3D
#define  GrV3DF           V3D_f
#define  GrPALETTE        PALETTE

#define  GrRgb            GrRGB
#define  GrColor          GrCOLOR
#define  GrContext        GrCONTEXT
#define  GrRleSprite      GrRLE_SPRITE
#define  GrImage          GrContext
#define  GrFont           GrFONT
#define  GrFixed          GrFIXED
#define  GrV3d            GrV3D
#define  GrV3Df           GrV3DF
#define  GrPalette        GrPALETTE

#define  GrPALETTESIZE    PAL_SIZE

#endif

#ifndef  TX_NOCOLOR
#define  TX_NOCOLOR               -1
#endif

#ifndef  TX_ALIGNS
#define  TX_ALIGNS
#define  TX_ALIGN_LEFT     0x01
#define  TX_ALIGN_TOP      0x02
#define  TX_ALIGN_RIGHT    0x04
#define  TX_ALIGN_BOTTOM   0x08
#define  TX_ALIGN_CENTERX  0x10
#define  TX_ALIGN_CENTERY  0x20
#define  TX_ALIGN_CENTER   (TX_ALIGN_CENTERX|TX_ALIGN_CENTERY)
#define  TX_ALIGN_DEFAULT  (TX_ALIGN_TOP|TX_ALIGN_LEFT)
#define  TX_UNDERLINE      0x40
#define  TX_STRIKELINE     0x80
#define  TX_NOSMOOTHTEXT   0x100
#endif

#define  CONTEXT_WIDTH(d)  (d?d->w:0)
#define  CONTEXT_HEIGHT(d) (d?d->h:0)
#define  IMAGE_WIDTH(d)    CONTEXT_WIDTH(d)
#define  IMAGE_HEIGHT(d)   CONTEXT_HEIGHT(d)

#define  GET_DATA(d,x) ((d)?(d)[x].dat:NULL)

#define  FONT_DEFAULT_WIDTH   -1
#define  FONT_DEFAULT_HEIGHT  -1

#define  NOCOLOR_RED           255
#define  NOCOLOR_GREEN           0
#define  NOCOLOR_BLUE          255


#define  ICON_16               16
#define  ICON_32               32


#define  SF_FLIPX              0x01
#define  SF_FLIPY              0x02

extern int   (*textlen)(void *f, unsigned char *text, int len );
extern int   (*textheight)(void *f );
extern void  (*drawtext)(BITMAP *b, void *f, unsigned char *text, int x1, int y1, GrCOLOR color );

/* icon library */

typedef struct ICONLIBRARY {

  int         id;
  DATAFILE   *datfile;
  BITMAP     *icon16;
  BITMAP     *icon32;

  char        rserved[16];

  struct ICONLIBRARY *next;

} ICONLIBRARY;



/* only for low-level graphics */

/* begin of my allegro functions */
#ifdef __cplusplus
extern "C" {
#endif
/* mouse */
extern BITMAP *_mouse_sprite;
extern int  _drawing_in_screen;
extern int mouse_mode;
extern int mouse_mode_rect_ax;
extern int mouse_mode_rect_ay;
extern int mouse_mode_rect_bx;
extern int mouse_mode_rect_by;

extern void (*run_mouse_mode)(struct BITMAP *msb, int mdx, int mdy, int erase, int show );

int  mouse_is_block();
int  mouse_is_visible();
int  mouse_block(struct BITMAP *bmp, int x1, int y1, int x2, int y2);
void mouse_unblock(int i);
void get_mouse_focus(int *fx, int *fy);
void get_mouse_range(int *x1, int *y1, int *x2, int *y2);
/* end of my allegro functions */
#ifdef __cplusplus
};
#endif


#define FR_HOR 0x01
#define FR_VER 0x02

void get_size_of_ftext(char *text, GrFONT *font, int *x, int *y);
void point_vline ( BITMAP *out, int x1, int y1, int y2, GrCOLOR color );
void point_hline ( BITMAP *out, int x1, int y1, int x2, GrCOLOR color );
void fade_rect ( BITMAP *b, int x1, int y1, int x2, int y2, GrCOLOR one, GrCOLOR two, int direct );
void lined_rect ( BITMAP *b, int x1, int y1, int x2, int y2, GrCOLOR one, GrCOLOR two );
void light_image ( BITMAP *b, int x1, int y1, int x2, int y2, int dr, int dg, int db );
void button ( BITMAP *b, int x1, int y1, int x2, int y2, GrCOLOR c1, GrCOLOR c2 );
int  get_depth ( BITMAP *b );
int  get_mask_color ( BITMAP *b );
int  get_width ( BITMAP *b );
int  get_height ( BITMAP *b );
int  get_clip ( BITMAP *b, int *x1, int *y1, int *x2, int *y2 );
void get_real_box ( BITMAP *b, int *x1, int *y1, int *x2, int *y2 );

int  get_char_length ( void *f, unsigned char ch );
void draw_char ( BITMAP *out, GrFONT *f, unsigned char chr, int x1, int y1,
                 GrCOLOR f1, GrCOLOR b1 );
void textout_draw(BITMAP *bmp, void *f, unsigned char *s, int len, int x1, int y1, int align, GrColor color, GrColor bcolor );
void textout_draw_rect(BITMAP *bmp, void *f, unsigned char *s, int len, int x1, int y1, int x2, int y2, int align, GrColor color, GrColor bcolor, int clip );
void textout_printf(BITMAP *bmp, void *f, int len,
                  int x1, int y1, int x2, int y2, int align, GrColor color, GrColor bcolor,
                  int clip, char *s, ... );
void draw_double_text ( BITMAP *out, GrFONT *f, unsigned char *txt, int size,
                        int x1, int y1, int x2, int y2, int align,
                        GrCOLOR f1, GrCOLOR f2, int clip );
void draw_selected_text ( BITMAP *out, GrFONT *f,
                          unsigned char *txt, int size, int sfrom, int sto,
                          int x1, int y1, int x2, int y2, int align,
                          GrCOLOR f1, GrCOLOR b1, GrCOLOR f2, GrCOLOR b2, int clip );

void draw_flip_sprite ( BITMAP *b, BITMAP *s, int x, int y, int flag );
void __blit_hline ( BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int dw );
void blit_ex ( BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int sh, int dw, int dh );
void blit_hline ( BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int dw );
void blit_circlefill(BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int radius);
void blit_ellipsefill(BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int rx, int ry);
BITMAP *gen_neg ( int depth, BITMAP *src );
BITMAP *gen_emboss ( int depth, BITMAP *src );
BITMAP *conv_to_skipcolor_bitmap ( BITMAP *ctx, int r, int g, int b );
BITMAP  *_fixup_loaded_bitmap ( BITMAP *bmp, PALETTE pal, int bpp );
DATAFILE *conv_to_skipcolor_data ( DATAFILE *f, int r, int g, int b );
DATAFILE *load_skip_datafile ( char *filename, int r, int g, int b );
void     *get_datafile_object ( DATAFILE *dat, int obj_num );
ICONLIBRARY  *convert_to_icon_library ( DATAFILE *dat );
ICONLIBRARY  *load_icon_library ( char *filename, int r, int g, int b );
void  unload_icon_library ( ICONLIBRARY *icl );
BITMAP  *get_icon_from_library ( ICONLIBRARY *icl, int id, int size );
GrIMAGE *gr_load_bitmap ( char *filename );
int  _gr_setmode ( int _s, int _w, int _h, int _v_w, int _v_h, char _depth );
unsigned long    get_context_mem_size ( BITMAP *b );
unsigned char*   get_addr_line ( BITMAP *b, int line );


#ifndef __NEWSCR_H_INCLUDED__
#define __NEWSCR_H_INCLUDED__

extern GFX_VTABLE *oldscreenvtable;
extern GFX_VTABLE  newscreenvtable;

int  sd_getpixel(struct BITMAP *bmp, int x, int y);
void sd_putpixel(struct BITMAP *bmp, int x, int y, int color);
void sd_vline(struct BITMAP *bmp, int x, int y1, int y2, int color);
void sd_hline(struct BITMAP *bmp, int x1, int y, int x2, int color);
void sd_line(struct BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
void sd_rectfill(struct BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
void sd_draw_sprite(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y);
void sd_draw_256_sprite(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y);
void sd_draw_sprite_v_flip(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y);
void sd_draw_sprite_h_flip(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y);
void sd_draw_sprite_vh_flip(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y);
void sd_draw_trans_sprite(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y );
void sd_draw_lit_sprite(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y, int color);
void sd_draw_rle_sprite(struct BITMAP *bmp, struct RLE_SPRITE *sprite, int x, int y);
void sd_draw_trans_rle_sprite(struct BITMAP *bmp, struct RLE_SPRITE *sprite, int x, int y);
void sd_draw_lit_rle_sprite(struct BITMAP *bmp, struct RLE_SPRITE *sprite, int x, int y, int color);
void sd_draw_character(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y, int color);
void sd_textout_fixed(struct BITMAP *bmp, void *f, int h, unsigned char *str, int x, int y, int color);
void sd_blit_from_memory(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void sd_blit_to_memory(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void sd_blit_to_self(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void sd_blit_to_self_forward(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void sd_blit_to_self_backward(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void sd_masked_blit(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void sd_clear_to_color(struct BITMAP *bitmap, int color);

void init_newscreenvtable();
void done_newscreenvtable();

#endif

#define  GrLoadBitmap(f)               gr_load_bitmap(f)
#define  load_image(f)                 GrLoadBitmap(f)
#define  GrLoadSkipDataFile(f,r,g,b)   load_skip_datafile(f, r, g, b)
#define  GrLoadDataFile(n)             load_datafile(n)
#define  GrUnloadDataFile(d)           unload_datafile(d)
#define  GrLoadDataFileObject(f,on)    load_datafile_object(f, on)
#define  GrUnloadDataFileObject(f,on)  unload_datafile_object(dat)
#define  GrGetDataFileProperty(d,t)    get_datafile_property(dat, t)
#define  GrRegisterDataFileObject(i,l,s) \
                                       register_datafile_object(i, l, s)
#define  GrFixupDataFile(d)            fixup_datafile(data)

#define  GrDepth(b)                              get_depth(b)
#define  GrMaskColor(b)                          get_mask_color(b)
#define  GrAddressLine(b,l)                      get_addr_line(b, l)
#define  GrDirectAddress(b)                      (b?b->dat:NULL)
#define  GrGetContextMemorySize(b)               get_context_mem_size(b)

#define  GrSetTextDrawingMode(i)                text_mode(i)
#define  GrSetMode(s,w,h,v_w,v_h,d)             _gr_setmode(s, w, h, v_w, v_h, d)
#define  GrSetTextMode()                        _gr_setmode(GFX_TEXT, 80, 25, 0, 0, 4)
#define  GrScreenW()                            SCREEN_W
#define  GrScreenH()                            SCREEN_H

#define  GrGenerateNegativ(d,s)                 gen_neg(d, s)
#define  GrGenerateEmboss(d,s)                  gen_emboss(d, s)
#define  GrConvertToSkipColorContext(c,r,g,b)   conv_to_skipcolor_bitmap(c, r, g, b)
#define  GrConvertToSkipColorData(d,n,r,g,b)    conv_to_skipcolor_data(d, n, r, g, b)
#define  GrCreateContext(w,h)                   create_bitmap(w, h)
#define  GrCreateContextEx(cd,w,h)              create_bitmap_ex(cd, w, h)
#define  GrCreateSubContext(p,x1,y1,w,h)        create_sub_bitmap(p, x1, y1, w, h)
#define  GrFixupLoadedBitmap(b,p,d)             ((b)?_fixup_loaded_bitmap(b,p,d):NULL)
#define  GrDestroyContext(c)                    destroy_bitmap(c)

#define  GrDoLine(b,x1,y1,x2,y2,d,p)            do_line(b,x1,y1,x2,y2,d,p)
#define  GrDoCircle(b,x1,y1,r,d,p)              do_circle(b,x1,y1,r,d,p)
#define  GrDoEllipse(b,x1,y1,rx1,ry1,d,p)       do_ellipse(b,x1,y1,rx1,ry1,d,p)
#define  GrCalcSpline(p,npts,px,py)             calc_spline(p, npts, px, py)

#define  GrSetDrawMode(m,p,xa,ya)               drawing_mode(m, p, xa, ya)
#define  GrSetClipBox(b,x1,y1,x2,y2)            set_clip(b, x1, y1, x2, y2)
#define  GrSetContext(b)                        CurrentContext = b
#define  GrWidth(b)                             get_width(b)
#define  GrHeight(b)                            get_height(b)
#define  GrGetClipBox(b,x1,y1,x2,y2)            get_clip(b, x1, y1, x2, y2)
#define  GrGetRealBox(b,x1,y1,x2,y2)            get_real_box(b, x1, y1, x2, y2)

#define  GrClearClip(b)                         clear(b)
#define  GrClearClipColor(b,c)                  clear_to_color(b,c)

#define  GrDrawSprite(b,s,x,y)                  draw_sprite(b, s, x, y)
#define  GrDrawSpriteLit(b,s,x,y,c)             draw_lit_sprite(b, s, x, y, c)
#define  GrDrawSpriteTrans(b,s,x,y)             draw_trans_sprite(b, s, x, y)
#define  GrDrawSpriteFlip(b,s,x,y,f)            draw_flip_sprite(b, s, x, y, f)
#define  GrDrawRleSprite(b,s,x,y)               draw_rle_sprite(b, s, x, y)
#define  GrDrawRleSpriteLit(b,s,x,y,c)          draw_lit_rle_sprite(b, s, x, y, c)
#define  GrDrawRleSpriteTrans(b,s,x,y)          draw_trans_rle_sprite(b, s, x, y)

#define  GrPutPixel(b,x1,y1,c)                  putpixel(b, x1, y1, c)
#define  GrGetPixel(b,x1,y1)                    getpixel(b, x1, y1)
#define  GrSpline(b,p,c)                        spline(b, p, c)
#define  GrHLine(b,x1,y1,w,c)                   hline(b, x1, y1, x1+w, c)
#define  GrVLine(b,x1,y1,h,c)                   vline(b, x1, y1, y1+h, c)
#define  GrLine(b,x1,y1,x2,y2,c)                line(b, x1, y1, x2, y2, c)
#define  GrRectColor(b,x1,y1,x2,y2,c)           rect(b, x1, y1, x2, y2, c)
#define  GrRectFilled(b,x1,y1,x2,y2,c)          rectfill(b, x1, y1, x2, y2, c)
#define  GrPolygon(b,v,p,c)                     polygon(b, v, p, c)
#define  GrCircle(b,x1,y1,r,c)                  circle(b, x1, y1, r, c)
#define  GrCircleFilled(b,x1,y1,r,c)            circlefill(b, x1, y1, r, c)
#define  GrEllipse(b,x1,y1,rx1,ry1,c)           ellipse(b, x1, y1, rx1, ry1, c)
#define  GrEllipseFilled(b,x1,y1,rx1,ry1,c)     ellipsefill(b, x1, y1, rx1, ry1, c)
#define  GrFloodFill(b,x1,y1,c)                 floodfill(b, x1, y1, c)
#define  GrBitBlt(d,x1d,y1d,s,x1s,y1s,x2s,y2s)  blit(s, d, x1s, y1s, x1d, y1d, (x2s-x1s)+1, (y2s-y1s)+1)
#define  GrBitBltEx(d,x1d,y1d,x2d,y2d,s,x1s,y1s,x2s,y2s)  \
                                                blit_ex(s, d, x1s, y1s, x1d, y1d, (x2s-x1s)+1, (y2s-y1s)+1, (x2d-x1d)+1, (y2d-y1d)+1)
#define  GrStretchSprite(d,s,x,y,w,h)           stretch_sprite(d, s, x, y, w, h)
#define  GrHLineBlt(d,x1d,y1d,dw,s,x1s,y1s,sw)  \
                                                blit_hline(s, d, x1s, y1s, x1d, y1d, sw, dw)
#define  GrCircleBlt(d,x1d,y1d,s,x1s,y1s,sw,r)  blit_circlefill(s, d, x1s, y1s, x1d, y1d, sw, r)
#define  GrEllipseBlt(d,x1d,y1d,s,x1s,y1s,sw,rx,ry)  \
                                                blit_ellipsefill(s, d, x1s, y1s, x1d, y1d, sw, rx, ry)
#define  GrMaskedBlt(d,x1d,y1d,s,x1s,y1s,x2s,y2s) \
                                                masked_blit(s, d, x1s, y1s, x1d, y1d, (x2s-x1s)+1, (y2s-y1s)+1)
#define  GrStretchBlt(d,x1d,y1d,x2d,y2d,s,x1s,y1s,x2s,y2s)  \
                                                stretch_blit(s, d, x1s, y1s, (x2s-x1s)+1, (y2s-y1s)+1, x1d, y1d, (x2d-x1d)+1, (y2d-y1d)+1)
#define  GrRotateSprite(d,x1d,y1d,s,a)          rotate_sprite(d, s, x1d, y1d, a)
#define  GrRotateStretchedSprite(d,x1d,y1d,s,a,sc) \
                                                rotate_scaled_sprite(d, s, x1d, y1d, a, sc)
#define  GrDrawText(b,f,str,l,x1,y1,a,c)        textout_draw(b, f, str, l, x1, y1, a, c)
#define  GrDrawTextRect(b,f,str,l,x1,y1,x2,y2,a,c,ic) \
                                                textout_draw_rect(b, f, str, l, x1, y1, x2, y2, a, c, ic)

#define  GrFontTextLength(f,str,l)              (*textlen)(f,str,l)
#define  GrFontCharLength(f,ch)			         get_char_length(f,ch)
#define  GrFontTextHeight(f)                    (*textheight)(f)


#define  GrPolygon3d(b,t,s,vc,vtx)              polygon3d(b, t, s, vc, vtx)
#define  GrPolygon3df(b,t,s,vc,vtx)             polygon3d_f(b, t, s, vc, vtx)
#define  GrTriangle3d(b,t,s,v1,v2,v3)           triangle3d(b, t, s, v1, v2, v3)
#define  GrTriangle3df(b,t,s,v1,v2,v3)          triangle3d_f(b, t, s, v1, v2, v3)
#define  GrQuad3d(b,t,s,v1,v2,v3,v4)            quad3d(b, t, s, v1, v2, v3, v4)
#define  GrQuad3df(b,t,s,v1,v2,v3,v4)           quad3d_f(b, t, s, v1, v2, v3, v4)

#define  __putpixel(b,x1,y1,c)                  _putpixel(b, x1, y1, c)
#define  __getpixel(b,x1,y1)                    _getpixel(b, x1, y1)

/* end of low-level graphics */

// colors

#define  CoSetColor(i,p)                       set_color(i, p)
#define  CoSetPalette(p)                       set_pallete(p)
#define  CoSetPaletteRange(p,f,t,vs)           set_pallete_range(p, f, t, vs)
#define  CoGetColor(i,p)                       get_color(i, p)
#define  CoGetPalette(p)                       get_pallete(p)
#define  CoGetPaletteRange(p,f,t)              get_pallete_range(p, f, t)

#define  CoFadeInterpolate(s,d,o,p,f,t)        fade_interpolate(s, d, o, p, f, t)
#define  CoFadeFromRange(s,d,sp,f,t)           fade_from_range(s, d, sp, f, t)
#define  CoFadeInRange(p,s,f,t)                fade_in_range(p, sp, f, t)
#define  CoFadeOutRange(sp, f, t)              fade_out_range(sp, f, t)
#define  CoFadeFrom(s,d,sp)                    fade_from(s, d, sp)
#define  CoFadeIn(p,sp)                        fade_in(p, sp)
#define  CoFadeOut(sp)                         fade_out(sp)

#define  CoSelectPalette(p)                    select_pallete(p)
#define  CoUnselectPalette()                   unselect_pallete()

#define  CoGenerate332Palette(p)               generate_332_palette(p)
#define  CoGenerateOptimizedPalette(b,p,c)     generate_optimized_palette(b, p, c)
#define  CoCreateRgbTable(t,p,callback)        create_rgb_table(t, p, callback)
#define  CoCreateLightTable(t,p,r,g,b,callback) \
                                               create_light_table(t, p, r, g, b, callback)
#define  CoCreateTransTable(t,p,r,g,b,callback) \
                                               create_trans_table(t, p, r, g, b, callback)
#define  CoCreateColorTable(t,p,blendfunc,callback) \
                                               create_color_table(t, p, blendfunc, callback)
#define  CoSetBlenderMode(b15,b16,b24,r,g,b,a) \
                                               set_blender_mode(b15, b16, b24, r, g, b, a)
#define  CoSetTransBlender(r,g,b,a)            set_trans_blender(r, g, b, a)
#define  CoHsvToRgb(h,s,v,r,g,b)               hsv_to_rgb(h, s, v, r, g, b)
#define  CoRgbToHsv(r,g,b,h,s,v)               rgb_to_hsv(r, g, b, h, s, v)
#define  CoBestFitColor(p,r,g,b)               bestfit_color(p, r, g, b)

#define  CoMakeColor(r,g,b)                    makecol(r, g, b)
#define  CoMakeColor8(r,g,b)                   makecol8(r, g, b)
#define  CoMakeColorDepth(d,r,g,b)             makecol_depth(d, r, g, b)

#define  CoGetRed(c)                           getr(c)
#define  CoGetGreen(c)                         getg(c)
#define  CoGetBlue(c)                          getb(c)
#define  CoGetRedDepth(d,c)                    getr_depth(d, c)
#define  CoGetGreenDepth(d,c)                  getg_depth(d, c)
#define  CoGetBlueDepth(d,c)                   getb_depth(d, c)

#define  CoMakeColor15(r,g,b)                  makecol15(r, g, b)
#define  CoMakeColor16(r,g,b)                  makecol16(r, g, b)
#define  CoMakeColor24(r,g,b)                  makecol24(r, g, b)
#define  CoMakeColor32(r,g,b)                  makecol32(r, g, b)

#define  CoGetRed8(c)                          getr8(c)
#define  CoGetGreen8(c)                        getg8(c)
#define  CoGetBlue8(c)                         getb8(c)
#define  CoGetRed15(c)                         getr15(c)
#define  CoGetGreen15(c)                       getg15(c)
#define  CoGetBlue15(c)                        getb15(c)
#define  CoGetRed16(c)                         getr16(c)
#define  CoGetGreen16(c)                       getg16(c)
#define  CoGetBlue16(c)                        getb16(c)
#define  CoGetRed24(c)                         getr24(c)
#define  CoGetGreen24(c)                       getg24(c)
#define  CoGetBlue24(c)                        getb24(c)
#define  CoGetRed32(c)                         getr32(c)
#define  CoGetGreen32(c)                       getg32(c)
#define  CoGetBlue32(c)                        getb32(c)

/*
  grxfont.h
*/
#ifndef __GRXFONT_H_INCLUDED__
#define __GRXFONT_H_INCLUDED__

/* GRX font file reader by Mark Wodrich.
 *
 * GRX FNT files consist of the header data (see struct below). If the font
 * is proportional, followed by a table of widths per character (unsigned 
 * shorts). Then, the data for each character follows. 1 bit/pixel is used,
 * with each line of the character stored in contiguous bytes. High bit of
 * first byte is leftmost pixel of line.
 *
 * Note : FNT files can have a variable number of characters, so we must
 *        check that the chars 32..127 exist.
 */

#define FONTMAGIC       0x19590214L

/* .FNT file header */
typedef struct {
   unsigned long  magic;
   unsigned long  bmpsize;
   unsigned short width;
   unsigned short height;
   unsigned short minchar;
   unsigned short maxchar;
   unsigned short isfixed;
   unsigned short reserved;
   unsigned short baseline;
   unsigned short undwidth;
   char           fname[16];
   char           family[16];
} FNTfile_header;


/* all we need is an array of bytes for each character */
typedef unsigned char * GRX_BITMAP;


#define GRX_TMP_SIZE    4096

void convert_grx_bitmap(int width, int height, GRX_BITMAP src, GRX_BITMAP dest);
GRX_BITMAP *load_grx_bmps(PACKFILE *f, FNTfile_header *hdr, int numchar, unsigned short *wtable);
static void stretch_character(BITMAP *bmp, BITMAP *sprite, int x, int y, int w, int h);
void *grx_loadfont(char *fname, int w, int h);

#endif

extern void   *(*load_font)(char* _filename, char *xxname, int w, int h, int from, int to);
extern void    (*unload_font)(void *f);

#define  GrLoadFont(f,x,w,h,fr,t)              (load_font?(*load_font)(f,x,w,h,fr,t):NULL)
#define  GrUnloadFont(f)                       (unload_font?(*unload_font)(f):NULL)

#endif


/*
 fonts.h
*/

#ifndef __FONTS_H_INCLUDED__
#define __FONTS_H_INCLUDED__

#define INI_TTFFONTS      "true_type_fonts"
#define INI_TTFFONTSMEM   "true_type_fonts_memory"

typedef struct t_fonts_loaded {

  void                     *font;
  char                     *name;
  int                       weight;
  struct t_fonts_loaded    *next;

  char                      reserved[24];

} t_fonts_loaded;


extern t_fonts_loaded *system_fonts;


int     add_font_to_system ( void *font, char *name, int weight );
void    load_supported_fonts ( char *ini_array_name, l_bool in_mem );
void    unload_system_fonts ( void );
l_font *get_font_in_size ( char *fontname, int w, int h );
void   *load_ttf_to_mem ( char* _filename, char *xxname, int w, int h, int from, int to );

#endif



/*
  DATAINI.H - read and write to ini files

  by      Michal Stencl
  e-mail: stenclpmd@ba.telecom.sk]
*/

#ifndef __DATAINI_H_INCLUDED__
#define __DATAINI_H_INCLUDED__

#define INI_STRING      1
#define INI_HEXANUM     2
#define INI_DECANUM     3

/*
  from "c" return red color
  F.e. "c" == 0xFF0015
  Red color is FF
  Green color is 00
  Blue color is 15
*/

#define getini_red(c)   ((unsigned char)((c)>>16))
#define getini_green(c) ((unsigned char)(((c)<<16)>>24))
#define getini_blue(c)  ((unsigned char)(((c)<<24)>>24))

#define ini_data  char

typedef struct ini_rgb {

  int r;
  int g;
  int b;
  int x;

} ini_rgb;

#ifdef __cplusplus
extern "C" {
#endif
char **getini_function ( char **def, char *one );
char *getini_line ( char **def, ini_data *dat, int line );
char **getini_values ( char *strvalue );
void  freeini_values ( char **values );
ini_rgb  getcolor_from_ini ( char *filename, char *ininame, char *ixname );
int   getini_linenum ( ini_data *dat );
void  setini_tofile ( char *filename, char *ininame, char *ixname, char *value, int type );
char *getini_value ( ini_data *dat, char *ixname );
char *getini_fromfile ( char *filename, char *ininame, char *ixname );
long  getininum_fromfile ( char *filename, char *ininame, char *ixname );
ini_data *getinidata_fromfile ( char *filename, char *ininame );
#ifdef __cplusplus
}
#endif

#endif



/*
   DLX Dynamic Loading and eXecution V3.0
   Copyright (c) 1997-1998, Nanosoft, Inc.
*/


#ifndef __DLX_INCLUDED_3__
#define __DLX_INCLUDED_3__

struct dlxheader {

        long magic;
        long numimports;
        long numrelocs;
        long libmainpos;
        long extablepos;
        long libloadpos;
        long prgsize;

};


typedef long long hdlx_t;
#define l_hdlx hdlx_t

struct dlxiddesc {  /* applies only to V2.0 DLX */

        hdlx_t MFID; /* manufacturer ID */
        hdlx_t PRID; /* product ID */
        hdlx_t VTBL; /* version description ID */
        hdlx_t UNID; /* unique DLX ID */

};


extern void(*dlx_first_ctor[])() __asm__("dlx_first_ctor");
extern void(*dlx_last_ctor[])() __asm__("dlx_last_ctor");
extern void(*dlx_first_dtor[])() __asm__("dlx_first_dtor");
extern void(*dlx_last_dtor[])() __asm__("dlx_last_dtor");


#define DLX_MAGIC          0x584c44
#define DLX2_MAGIC         0x32584c44
#define DLX3_MAGIC         0x33584c44
#define DLX_BUILD_DOS      0x534f444d4249LL

#ifdef __cplusplus

        #define DLXUSE_BEGIN extern "C" {
        #define DLXUSE_END }
        #define DLX_FN extern "C" {
        #define DLX_EF }
        #define DLX_IMPORT extern "C" {
        #define DLX_ENDIMPORT }

#else
        #define DLXUSE_BEGIN
        #define DLXUSE_END
        #define DLX_FN 
        #define DLX_EF 
        #define DLX_IMPORT 
        #define DLX_ENDIMPORT
        
#endif

#define LIBINFO_BEGIN
#define LIBINFO_END
#define LIBLOADS_BEGIN char* _LIBTOLOAD[]={
#define LIBLOADS_END "\0\0"};
#define LIBEXPORT_BEGIN char* _LIBEXPORTTABLE[]= {
#define LIBEXPORT_END 0, 0 };
#define LIBEXPORTCLASS(x) "_"#x, (char *)((FUNCTYPE)(x)),
#define LIBEXPORT(x) "_"#x, (char *)(&x),
#define LIBALIAS(x, y) "_"#x, (char *)((FUNCTYPE)y),
#define LIBENTRY(x) (char*)0L, (char *)1L, #x, (char *)&x,
#define LIBWEAK(x) (char*)0L, (char *)2L, "_"#x, (char *)&x,
#define LIBLOAD(x) #x,
#define LIBCONSTRUCT  do {                                            \
                        long i=0;                                     \
                        for (i=0; i<dlx_last_ctor-dlx_first_ctor; i++)\
                        dlx_first_ctor[i]();                          \
                      } while (0);
#define LIBDESTRUCT   do {                                            \
                        long i=0;                                     \
                        for (i=0; i<dlx_last_dtor-dlx_first_dtor; i++)\
                        dlx_first_dtor[i]();                          \
                      } while(0);
#define LIBVERSION_BEGIN long long _DLXVERSIONTABL[]={
#define LIBVERSION_END 0,0,0,0};
#define LIBVERSION(x) x##LL,
#define LIBMYHANDLE _DLXVERSIONTABL[3]
#define LIBMCLASS(x) long _DLXMEMORYCLASS=x;
#define LIBDPLID(x) long long _DLXDPLCLASS=x;


extern void* (*DLXOpenFile)(char*);
extern void  (*DLXCloseFile)(void*);
extern long  (*DLXReadFile)(void*,long,void*);

extern void   (*DLXError)(long,char*);
extern hdlx_t (*DLXGetID)(char*);

extern void* (*DLXCodeAlloc)(unsigned long, long mclas);
extern void* (*DLXMalloc)(unsigned long);
extern void  (*DLXFree)(void*);
extern void* (*DLXRealloc)(void*,unsigned long);

extern void* (*DLXGetFileData)(hdlx_t n); /* my */
extern long  (*DLXGetLMaxInstance)(hdlx_t n); /* my */
extern long  (*DLXGetLInstance)(hdlx_t n); /* my */
extern int   (*DLXGetLOverload)(hdlx_t n); /* my */
extern void  (*DLXSetLMaxInstance)(hdlx_t n, long i); /* my */
extern void  (*DLXSetLOverload)(hdlx_t n, int i); /* my */

extern hdlx_t (*DLXLoad)(char* name, char* args);
extern int    (*DLXUnload)(hdlx_t handle);
extern int    (*DLXUnloadEx)(char *name);
extern int    (*DLXImport)(char** symbols);
extern void*  (*DLXGetEntry)(hdlx_t target, char* name);
extern void*  (*DLXGetMemoryBlock)(hdlx_t target);
extern long   (*DLXGetMemoryBlockLength)(hdlx_t target);
extern void*  (*DLXGetResource)(hdlx_t target);
extern long   (*DLXGetResourceLength)(hdlx_t target);

#ifndef __MAIN_INCLUDED__
static hdlx_t ap_id = 0; /* id for dlx aplication */
#endif

#define AP_ALLOC   0
#define AP_INIT    2
#define AP_DONE    3
#define AP_FREE    1

#define AP_CALL             DLXGetLInstance(ap_id)
#define AP_SETNUMOFCALLS(x) DLXSetLMaxInstance(ap_id, x)
#define AP_SETOVERLOAD(x)   DLXSetLOverload(ap_id, x)
#define AP_ISOVERLOAD       DLXGetLOverload(ap_id)
#define AP_DATAFILE         DLXGetFileData(ap_id)
#define AP_EXPORTLIB()      DLXImport(_LIBEXPORTTABLE)


#define AP_RET
#define AP_END } DLX_EF

#define app_end                             }
#define app_main ap_begin
#define app_begin(_x)                       \
                                             \
lib_exportable                                \
                                              \
DLX_FN void LibMain(int ap_process, hdlx_t id, char* ap_args) {     \
    if ( ap_process == AP_ALLOC ) {                  \
      LIBCONSTRUCT                            \
      ap_id = id;                             \
    }                                         \
    if ( ap_process == AP_FREE ) {                  \
      LIBDESTRUCT                             \
    }



#define lib_exportable                       \
DLXUSE_BEGIN                                 \
  LIBLOADS_BEGIN                             \
  LIBLOADS_END                               \
  LIBEXPORT_BEGIN                            \
  LIBEXPORT_END                              \
DLXUSE_END                                   \


#define lib_end                              }
#define lib_main lib_begin

#define lib_begin(_x)                        \
                                             \
                                             \
DLX_FN void LibMain(int ap_process, hdlx_t id, char* ap_args) {     \
    if ( ap_process == AP_ALLOC ) {                  \
      LIBCONSTRUCT                            \
      ap_id = id;                             \
    }                                         \
    if ( ap_process == AP_FREE ) {                  \
      LIBDESTRUCT                             \
    }


#endif

/*
  EXEDLX.H

  by      Michal Stencl
  e-mail: stenclpmd@ba.telecom.sk]
*/

#ifndef __EXEDLX_H_INCLUDED__
#define __EXEDLX_H_INCLUDED__

#define INI_EXTRUNNERS   "extension_runners"

#define EXT_LIBOSX       "%lib-seal"


typedef struct t_ico_loaders *p_ico_loaders;
typedef struct t_ext_runners *p_ext_runners;

/* ico loaders - all function that know to load icons */

typedef struct t_ico_loaders {

  l_char      *ext;
  BITMAP*    (*load_icon)(l_text filename, l_int size, l_int *ownmem);

  struct t_ico_loaders *next;

} t_ico_loaders;



typedef struct t_ext_runners {

  l_char      *ext;
  l_char      *app;
  l_char       reserved[24];

} t_ext_runners;

extern l_int (*read_set_file)(l_char *_filename);
extern l_int (*dos_command) ( l_text newpath, l_text command );
extern p_ext_runners  *ext_runners;
extern p_ico_loaders   ico_loaders;

void     put_into_runners ( l_char *ext, l_char *program );
l_char*  get_from_runners ( l_char *ext );
l_int    run_file ( l_char *args );
l_int    run_file_args ( l_text file, l_char *args );
BITMAP*  load_file_icon ( l_char *args, l_int size, l_int *ownmem );
BITMAP*  get_icon_for_file ( l_text filename, l_int attrib, l_int *ownmem );
BITMAP*  get_icon_for_file_ex ( l_text filename, l_int attrib, l_int *ownmem, l_int size );
p_list   get_args ( l_char *args );
void     add_to_loadfileicon ( l_char *ext, BITMAP* (*load_icon)(l_text filename, l_int size, l_int *ownmem) );

/* not for seal.h */
BITMAP*  _load_exe_icon ( l_char *args, l_int size, l_int *ownmem );
BITMAP*  _load_ldv_icon ( l_char *args, l_int size, l_int *ownmem );

void     init_loadfileicon ( void );
void     free_loadfileicon ( void );

l_int    _dos_command ( l_text newpath, l_text command );

l_int  _read_set_file ( l_char *_filename );

#endif




#ifndef ___IODIR_H_INCLUDED__
#define ___IODIR_H_INCLUDED__

#include<dir.h>

#define INI_DRIVES   "drives"

#define DRIVE_DESKTOP "desktop"
#define DRIVE_PC      "pc"

#define IO_FILE_LIMIT   180
#define IO_DIR_LIMIT    280


#define FA_LINK         64
#define FA_LINKDIR     128
#define FA_LINKDRIVE   256
#define FA_NOTPARENT   512

#define FA_ALL        (FA_RDONLY|FA_ARCH|FA_HIDDEN|FA_DIREC|FA_SYSTEM|FA_LINK|FA_LINKDIR)
#define FA_ISFILE(t)  (!((t) & FA_DIREC && (t) & FA_LINKDIR))


/* t_dirinfo flags */

#define DIF_SIZE          0x00001 /* get size of directory */
#define DIF_HALT          0x00002 /* process was halted */
#define DIF_DIRCALLBACK   0x00004 /* callback is call for directory too */


typedef struct t_drives *p_drives;


typedef struct t_drives {

   l_int            id;
   l_text           name;
   l_text           path;

   l_char           reserved[24];

   struct t_drives *next;

} t_drives;


typedef struct t_ffblk {

   struct ffblk info;

   l_int        ff_attrib;
   l_char       ff_filename[260];

   l_char       ff_reserved[24];

} t_ffblk;


/*
  fat is used for DAT_FILE id.
*/

typedef struct t_file {

  l_text       path;
  l_text       filename;
  l_int        attrib;

  l_word       time;  /* hours:5, minutes:6, (seconds/2):5 */
  l_word       date;  /* (year-1980):7, month:4, day:5 */
  l_dword      size;  /* size of file */

  l_char       reserved[24];

} t_file;



typedef struct t_dirinfo {

   l_dword          dirs;
   l_dword          files;
   l_dword          size;
   l_int            flags;

   l_char           reserved[24];

} t_dirinfo;


extern p_drives drives;

/* seal.h exportable functions */

t_file *new_tfile ( l_text path, l_text filename, l_int attrib, l_word time, l_word date, l_dword size );
void    free_tfile ( void *p );

int     io_findfirst(const char *pathname, struct t_ffblk *ffblk, int attrib);
int     io_findnext(struct t_ffblk *ffblk);
t_dirinfo io_foreach_file_ex ( l_text path, l_int flags, l_int (*callback)(), p_object o, l_dword *ind );
#define   io_foreach_file(p,f,o,i) io_foreach_file_ex(p, f, NULL, o, i)
l_int   io_foreach_file_copy ( l_text dst, l_text path, p_object o, l_dword *ind );
l_int   io_removefile ( p_object ob, t_file *f, l_dword *ind );
l_int   io_copyfile ( p_object ob, t_file *dst, t_file *src, l_dword *ind );
l_int   io_numberfile ( p_object ob, t_file *dst, l_dword *ind );
l_int   io_rename ( l_text nameold, l_text namenew );
l_int   _io_removefile ( l_text src );
l_int   _io_copyfile ( l_text dst, l_text src );
l_text  io_realpath ( l_text path, l_text file );
void    io_linkedpath_ex ( l_text file, l_text *link, l_text *icon );
void    io_linkedfile_ex (  l_text file, l_text *link, l_text *icon );
l_text  io_linkedpath ( l_text file );
l_text  io_linkedfile ( l_text file );
void    io_set_linkedfile_ex ( l_text file, l_text link, l_text icon );
void    io_set_linkedpath_ex ( l_text file, l_text link, l_text icon );
void    io_set_linkedpath ( l_text file, l_text link );
void    io_set_linkedfile ( l_text file, l_text link );
l_bool  io_mkdir ( l_text dir );
l_text  io_parentdir ( l_text path );
l_text  io_uniquedir ( l_text path );

l_bool  io_exist ( l_text file );
l_int   io_remove_file ( l_text file );
l_bool  io_isext ( l_text file );
l_bool  io_issame ( l_text file1, l_text file2 );
l_bool  io_isfile ( l_text file );
l_bool  io_isdir ( l_text file );
l_text  io_nicelink ( l_text filename );
void    io_cleartfile ( t_file *f );
#define io_tfiletofile(i) io_realpath((i)->path, (i)->filename)
t_file  io_filetotfile ( l_text filename );
l_bool  io_isfilename ( l_text file );
l_bool  io_isas ( l_text file, l_text ext );
#define io_getfilename(fn) ((fn)?get_filename(fn):NULL)
#define io_filename(fn)    _strdup(io_getfilename(fn))
#define io_path(fn)        stridup(fn, strsize(fn, io_getfilename(fn)))
#define io_sealfile(fn)    io_linkedfile(fn)
#define io_sealpath(fn)    io_linkedpath(fn)

l_text  drv_findfirst ( p_drives *drv );
l_text  drv_findnext ( p_drives *drv );
l_text  drv_fixname ( l_text drive );
l_text  drv_fixreal ( l_text path );
l_text  drv_fixid ( l_int id );
l_text  drv_isdriver ( l_text path );


/* end */

void    drv_set ( l_text namedrive, l_text path, l_int id );
void    drv_init ( void );
void    drv_done ( void );

l_text  file_gettext ( l_text filename );
l_bool  file_puttext ( l_text filename, l_text text );

#endif



#ifndef __KEYBOARD_H_INCLUDED__
#define __KEYBOARD_H_INCLUDED__

#define INI_KEYBOARD     "keyboard"

#define KB_ESC               1     /* keyboard scan codes  */
#define KB_1                 2 
#define KB_2                 3 
#define KB_3                 4
#define KB_4                 5
#define KB_5                 6
#define KB_6                 7
#define KB_7                 8
#define KB_8                 9
#define KB_9                 10
#define KB_0                 11
#define KB_MINUS             12
#define KB_EQUALS            13
#define KB_BACKSPACE         14
#define KB_TAB               15 
#define KB_Q                 16
#define KB_W                 17
#define KB_E                 18
#define KB_R                 19
#define KB_T                 20
#define KB_Y                 21
#define KB_U                 22
#define KB_I                 23
#define KB_O                 24
#define KB_P                 25
#define KB_OPENBRACE         26
#define KB_CLOSEBRACE        27
#define KB_ENTER             28
#define KB_CONTROL           29
#define KB_LCONTROL          29
#define KB_A                 30
#define KB_S                 31
#define KB_D                 32
#define KB_F                 33
#define KB_G                 34
#define KB_H                 35
#define KB_J                 36
#define KB_K                 37
#define KB_L                 38
#define KB_COLON             39
#define KB_QUOTE             40
#define KB_TILDE             41
#define KB_LSHIFT            42
#define KB_BACKSLASH         43
#define KB_Z                 44
#define KB_X                 45
#define KB_C                 46
#define KB_V                 47
#define KB_B                 48
#define KB_N                 49
#define KB_M                 50
#define KB_COMMA             51
#define KB_STOP              52
#define KB_SLASH             53
#define KB_RSHIFT            54
#define KB_ASTERISK          55
#define KB_ALT               56
#define KB_SPACE             57
#define KB_CAPSLOCK          58
#define KB_F1                59
#define KB_F2                60
#define KB_F3                61
#define KB_F4                62
#define KB_F5                63
#define KB_F6                64
#define KB_F7                65
#define KB_F8                66
#define KB_F9                67
#define KB_F10               68
#define KB_NUMLOCK           69
#define KB_SCRLOCK           70
#define KB_HOME              71
#define KB_UP                72
#define KB_PGUP              73
#define KB_MINUS_PAD         74
#define KB_LEFT              75
#define KB_5_PAD             76
#define KB_RIGHT             77
#define KB_PLUS_PAD          78
#define KB_END               79
#define KB_DOWN              80
#define KB_PGDN              81
#define KB_INSERT            82
#define KB_DEL               83
#define KB_PRTSCR            84
#define KB_F11               87
#define KB_F12               88
#define KB_LWIN              91
#define KB_RWIN              92
#define KB_MENU              93
#define KB_PAD               100
#define KB_RCONTROL          120
#define KB_ALTGR             121
#define KB_SLASH2            122
#define KB_PAUSE             123

#define KEY_TO(c)       ((c)>>8)
#define TO_KEY(c)       SCANCODE_TO_KEY(c)
#define TO_CHAR(c)      ((c) & 0xff)
#define TO_CAPS(c)      SCANCODE_TO_CAPS(c)
#define TO_SHIFT(c)     SCANCODE_TO_SHIFT(c)
#define TO_CTRL(c)      SCANCODE_TO_CONTROL(c)
#define TO_ALTGR(c)     SCANCODE_TO_ALTGR(c)
#define TO_ALT(c)       SCANCODE_TO_ALT(c)

#define KEYPRESSED(c)   (KEYB && KEYB->obclass.state & KB_SF_KEYDOWN && KEYB->code==c)
#define CHARPRESSED(c)  (KEYB && KEYB->obclass.state & KB_SF_KEYDOWN && KEYB->keychar==c)

#define KEYCTRL(c)   ( KEYB->code>>8 == c && KEYB->shifts & KB_CTRL_FLAG )
#define KEYSHIFT(c)  ( KEYB->code>>8 == c && KEYB->shifts & KB_SHIFT_FLAG )
#define KEYALT(c)    ( KEYB->code>>8 == c && KEYB->shifts & KB_ALT_FLAG )

/* state settings */

#define  KB_SF_KEYDOWN       0x0000020 /* key was pull down */


typedef struct t_keyboard *p_keyboard;

typedef struct t_keyboard {

  struct t_object obclass; /* object structure */

  l_int     code;
  l_int     shifts;
  l_char    keychar;

  void   (*simulate_keypress) ( p_keyboard o, l_int keycode );
  void   (*set_ctrl_alt_del_flag) ( p_keyboard o, l_bool enable );
  l_bool (*get_ctrl_alt_del_flag) ( p_keyboard o );
  l_int  (*readkey) ( p_keyboard o );
  void   (*clear_buffer) ( p_keyboard o );
  l_bool (*keypressed) ( p_keyboard o );

} t_keyboard;

extern  t_keyboard   *keyb;
#define KEYB          keyb

  extern t_keyboard* (*key_init) ( p_keyboard o );
  t_keyboard* _key_init ( p_keyboard o );

  l_int  key_readkey ( p_keyboard o );
  void   key_simulate_keypress ( p_keyboard o, l_int keycode );
  void   key_set_ctrl_alt_del_flag ( p_keyboard o, l_bool enable );
  l_bool key_get_ctrl_alt_del_flag ( p_keyboard o );
  void   key_clear_buffer ( p_keyboard o );
  l_bool key_keypressed ( p_keyboard o );

  /* from object */

  void   key_translate_event ( t_object *o, t_event *event );
  l_bool key_done ( t_object *o );

#endif


#ifndef __MOUSE_H_INCLUDED__
#define __MOUSE_H_INCLUDED__

extern l_int   mouse_flags;
extern l_color mouse_mode_color;


extern l_int   msblock0;
extern l_int   msblock1;
extern l_int   msblock2;
extern l_int   msblock3;
extern l_int   msblock4;


#define MO_MO_RECT      0x01
#define MO_MO_GROW      0x02


#ifndef CUR_SKIP_COLORS
#define CUR_SKIP_COLORS
#define CUR_SKIP_RED    NOCOLOR_RED
#define CUR_SKIP_GREEN  NOCOLOR_GREEN
#define CUR_SKIP_BLUE   NOCOLOR_BLUE
#endif

#ifndef CUR_DATAMAX
#define CUR_DATAMAX     32
#endif

#define CURSOR_SYSTEM   cursor_system
#define CURSOR_STANDARD cursor_standard
#define CURSOR_FOCUS    cursor_focus

#ifndef CUR_ARROW
#define CUR_ARROW            0x00
#endif
#ifndef CUR_MOVE
#define CUR_MOVE             0x01
#endif
#ifndef CUR_GROW
#define CUR_GROW             0x02
#endif
#ifndef CUR_TEXT
#define CUR_TEXT             0x03
#endif
#ifndef CUR_CLOCK
#define CUR_CLOCK            0x04
#endif
#ifndef CUR_PENCIL
#define CUR_PENCIL           0x05
#endif
#ifndef CUR_TARGET
#define CUR_TARGET           0x06
#endif
#ifndef CUR_TARGET2
#define CUR_TARGET2          0x07
#endif
#ifndef CUR_STOP
#define CUR_STOP             0x08
#endif
#ifndef CUR_ZOOM
#define CUR_ZOOM             0x09
#endif
#ifndef CUR_SCRUP
#define CUR_SCRUP            0x0a
#endif
#ifndef CUR_SCRDN
#define CUR_SCRDN            0x0b
#endif
#ifndef CUR_SCRLF
#define CUR_SCRLF            0x0c
#endif
#ifndef CUR_SCRRG
#define CUR_SCRRG            0x0d
#endif
#ifndef CUR_TEXT2
#define CUR_TEXT2            0x0e
#endif
#ifndef CUR_TEXT2
#define CUR_TEXT2            0x0e
#endif
#ifndef CUR_DRAG
#define CUR_DRAG             0x0f
#endif
#ifndef CUR_FINGER
#define CUR_FINGER           0x10
#endif

#define cursor_load_file(f,n,r,g,b) GrLoadSkipDataFile(f, n, r, g, b)

#define CURSOR_GETSYSTEMXFOCUS(cn)  ((CUR_DATAMAX>cn)?CURSOR_FOCUS[cn*2]:0)
#define CURSOR_GETSYSTEMYFOCUS(cn)  ((CUR_DATAMAX>cn)?CURSOR_FOCUS[cn*2+1]:0)
#define CURSOR_GETCURSOR(data,n)    ((data && n>=0 && n<CUR_DATAMAX && data[n].type==DAT_BITMAP)?((GrCONTEXT*)data[n].dat):NULL)
#define CURSOR_GETCURSORWIDTH(d,n)  ((CURSOR_SYSTEM && n>=0 && n<CUR_DATAMAX && d[n].type==DAT_BITMAP)?((GrCONTEXT*)d[n].dat)->w:NULL)
#define CURSOR_GETCURSORHEIGHT(d,n) ((CURSOR_SYSTEM && n>=0 && n<CUR_DATAMAX && d[n].type==DAT_BITMAP)?((GrCONTEXT*)d[n].dat)->h:NULL)
#define CURSOR_GETSYSTEM(n)         CURSOR_GETCURSOR(CURSOR_SYSTEM,n)
#define CURSOR_GETSYSTEMWIDTH(n)    CURSOR_GETCURSORWIDTH(CURSOR_SYSTEM, n)
#define CURSOR_GETSYSTEMHEIGHT(n)   CURSOR_GETCURSORHEIGHT(CURSOR_SYSTEM,n)

#define  INI_MOUSE     "mouse"

#define  MOUSE_TIME_DOUBLECLICK  500

#define  MO_SF_MOUSENOTHING      0x0000000
#define  MO_SF_MOUSEMOVE         0x0000020
#define  MO_SF_MOUSELDOWN        0x0000040
#define  MO_SF_MOUSELUP          0x0000080
#define  MO_SF_MOUSERDOWN        0x0000100
#define  MO_SF_MOUSERUP          0x0000200
#define  MO_SF_MOUSEMDOWN        0x0000400
#define  MO_SF_MOUSEMUP          0x0000800
#define  MO_SF_MOUSESHOW         0x0001000
#define  MO_SF_MOUSEHIDE         0x0002000
#define  MO_SF_MOUSELAUTO        0x0004000
#define  MO_SF_MOUSEMAUTO        0x0008000
#define  MO_SF_MOUSERAUTO        0x0010000
#define  MO_SF_MOUSELDOUBLE      0x0020000
#define  MO_SF_MOUSERDOUBLE      0x0040000
#define  MO_SF_MOUSEMDOUBLE      0x0080000
#define  MO_SF_MOUSEDOUBLE       (MO_SF_MOUSELDOUBLE|MO_SF_MOUSERDOUBLE|MO_SF_MOUSEMDOUBLE)
#define  MO_SF_MOUSEAUTO         (MO_SF_MOUSELAUTO|MO_SF_MOUSERAUTO|MO_SF_MOUSEMAUTO)
#define  MO_SF_MOUSEDOWN         (MO_SF_MOUSELDOWN|MO_SF_MOUSERDOWN|MO_SF_MOUSEMDOWN)
#define  MO_SF_MOUSEUP           (MO_SF_MOUSELUP|MO_SF_MOUSERUP|MO_SF_MOUSEMUP)
#define  MO_SF_MOUSEPRESS        (MO_SF_MOUSEAUTO|MO_SF_MOUSEDOWN)
#define  MO_SF_MOUSELPRESS       (MO_SF_MOUSELAUTO|MO_SF_MOUSELDOWN)
#define  MO_SF_MOUSEMPRESS       (MO_SF_MOUSEMAUTO|MO_SF_MOUSEMDOWN)
#define  MO_SF_MOUSERPRESS       (MO_SF_MOUSERAUTO|MO_SF_MOUSERDOWN)


typedef struct t_mouse *p_mouse;

typedef struct t_mouse {

  struct t_object  obclass;
  struct t_point   speed;
  struct t_point   where;
  l_char           buttons;
  l_dword          state_last;
  l_int            dclick_milisec;
  l_big            dclick_old_time;

  BITMAP*   (*get_cursor)(p_mouse o);
  t_point   (*get_focus)(p_mouse o);
  t_rect    (*get_range)(p_mouse o);
  l_bool    (*is_visible)(p_mouse o);
  void      (*set_dclick_diff)(p_mouse o, l_int mili);
  l_bool    (*show)(p_mouse o);
  l_bool    (*hide)(p_mouse o);
  l_int     (*block)(p_mouse o, t_rect r );
  void      (*unblock)(p_mouse o, l_int i );
  l_bool    (*is_block)(p_mouse o );
  void      (*set_pos)( p_mouse o, t_point where );
  l_bool    (*set_cursor)(p_mouse o, BITMAP *cursor);
  l_bool    (*set_cursor_focus)(p_mouse o, BITMAP *cursor, t_point p );
  l_bool    (*set_mode)(p_mouse o, l_int mode, l_color col, t_rect r );
  void      (*set_range)(p_mouse o, t_rect r );
  void      (*set_speed)(p_mouse o, t_point speed );
  void      (*set_focus)(p_mouse o, t_point focus );

} t_mouse;


extern DATAFILE  *cursor_system;
extern BITMAP    *cursor_standard;

extern l_byte     cursor_focus[];

extern   t_mouse *mouse;

#define  MOUSE    mouse

#define  mouse_set_focus_id(id)         MOUSE->set_focus(MOUSE, point_assign(CURSOR_GETSYSTEMXFOCUS(id), CURSOR_GETSYSTEMYFOCUS(id)))
#define  mouse_set_cursor_focus_id(id)  MOUSE->set_cursor_focus(MOUSE, CURSOR_GETSYSTEM(id), point_assign(CURSOR_GETSYSTEMXFOCUS(id), CURSOR_GETSYSTEMYFOCUS(id)))
#define  mouse_set_cursor_id(id)        MOUSE->set_cursor(MOUSE, CURSOR_GETSYSTEM(id))


#define  mouse_rect_block(ctx,i,r)          msblock##i = mouse->block(mouse, ctx, r)
#define  mouse_rect_unblock(i)              mouse->unblock(mouse, i)

/* mouse functions */

  extern t_mouse*  (*mouse_init) ( p_mouse o );
  t_mouse*  _mouse_init ( p_mouse o );

  BITMAP*   mouse_get_cursor(p_mouse o);
  t_point   mouse_get_focus(p_mouse o);
  t_rect    mouse_get_range(p_mouse o);
  l_bool    xx_mouse_is_visible(p_mouse o);
  void      mouse_set_dclick_diff(p_mouse o, l_int mili);
  l_bool    mouse_show(p_mouse o);
  l_bool    mouse_hide(p_mouse o);
  void      mouse_set_pos(p_mouse o, t_point where );
  l_bool    mouse_set_cursor(p_mouse o, BITMAP *cursor);
  l_bool    mouse_set_cursor_focus(p_mouse o, BITMAP *cursor, t_point p );
  l_bool    mouse_set_mode(p_mouse o, l_int mode, l_color col, t_rect r );
  void      mouse_set_range(p_mouse o, t_rect r );
  void      mouse_set_speed(p_mouse o, t_point speed );
  void      mouse_set_focus(p_mouse o, t_point focus );

  l_int     xx_mouse_block(p_mouse o, t_rect r );
  void      xx_mouse_unblock(p_mouse o, l_int i );
  l_bool    xx_mouse_is_block(p_mouse o );

  /* from object */

  l_bool    mouse_done ( t_object *o );
  void      mouse_translate_event ( t_object *o, t_event *event );

  void      mouse_load_cursor ( l_char *filename );
  l_bool    mouse_set_cursor_system_file ( l_char *cursorfile, l_int n, l_int r, l_int g, l_int b );


void    clear_event_mouse ( void );


#endif


#ifndef __SCREEN_H_INCLUDED__
#define __SCREEN_H_INCLUDED__

#define  INI_SCREEN     "screen"

extern BITMAP *screen_virtual;

extern l_int screen_width;
extern l_int screen_height;

l_bool   screen_init ( void );
void     screen_done ( void );

l_bool  screen_reload ( void );
void    screen_halt ( void );

#endif

#ifndef __DRIVERS_H_INCLUDE__
#define __DRIVERS_H_INCLUDE__


#define SET_DRIVERS "drivers.set"

#define INI_FONTS   "fonts"
#define INI_ICONS   "icons"


/* name's of system icons */

extern l_text  icon_id[256];


/* system icons declarations */

#define ICO_APPLICATION        0
#define ICO_BROWSER            1
#define ICO_CONTROLPANEL       2
#define ICO_RUN                3
#define ICO_NETWORK            4
#define ICO_SHUTDOWN           5
#define ICO_EDITOR             6
#define ICO_FOLDER             7
#define ICO_OPENFOLDER         8
#define ICO_FONTS              9
#define ICO_MYCOMPUTER        10
#define ICO_INTERNETDIALER    11
#define ICO_INTERNET          12
#define ICO_INTERNETSETTINGS  13
#define ICO_HELP              14
#define ICO_MAILER            15
#define ICO_PRINTERS          16
#define ICO_SETTINGS          17
#define ICO_SYSTEMFOLDER      18
#define ICO_PCSYSTEM          19
#define ICO_TEMPLATE          20
#define ICO_TRASHEMPTY        21
#define ICO_TRASHFULL         22
#define ICO_UTILITY           23
#define ICO_DRIVE3_5          24
#define ICO_DRIVE5_25         25
#define ICO_DRIVECD           26
#define ICO_DRIVEHD           27
#define ICO_FAVORITES         28
#define ICO_DOCUMENTS         29
#define ICO_SEALLOGO          30
#define ICO_AUDIO             31
#define ICO_DESKTOP           32


#define icon_application(s)      icon_sysget(ICO_APPLICATION, s)
#define icon_browser(s)          icon_sysget(ICO_BROWSER, s)
#define icon_controlpanel(s)     icon_sysget(ICO_CONTROLPANEL, s)
#define icon_run(s)              icon_sysget(ICO_RUN, s)
#define icon_network(s)          icon_sysget(ICO_NETWORK, s)
#define icon_shutdown(s)         icon_sysget(ICO_SHUTDOWN, s)
#define icon_editor(s)           icon_sysget(ICO_EDITOR, s)
#define icon_folder(s)           icon_sysget(ICO_FOLDER, s)
#define icon_openfolder(s)       icon_sysget(ICO_OPENFOLDER, s)
#define icon_fonts(s)            icon_sysget(ICO_FONTS, s)
#define icon_mycomputer(s)       icon_sysget(ICO_MYCOMPUTER, s)
#define icon_internetdialer(s)   icon_sysget(ICO_INTENETDIALER, s)
#define icon_internet(s)         icon_sysget(ICO_INTERNET, s)
#define icon_internetsettings(s) icon_sysget(ICO_INTERNETSETTINGS, s)
#define icon_help(s)             icon_sysget(ICO_HELP, s)
#define icon_mailer(s)           icon_sysget(ICO_MAILER, s)
#define icon_printers(s)         icon_sysget(ICO_PRINTERS, s)
#define icon_settings(s)         icon_sysget(ICO_SETTINGS, s)
#define icon_systemfolder(s)     icon_sysget(ICO_SYSTEMFOLDER, s)
#define icon_pcsystem(s)         icon_sysget(ICO_PCSYSTEM, s)
#define icon_template(s)         icon_sysget(ICO_TEMPLATE, s)
#define icon_trashempty(s)       icon_sysget(ICO_TRASHEMPTY, s)
#define icon_trashfull(s)        icon_sysget(ICO_TRASHFULL, s)
#define icon_utility(s)          icon_sysget(ICO_UTILITY, s)
#define icon_drive3_5(s)         icon_sysget(ICO_DRIVE3_5, s)
#define icon_drive5_25(s)        icon_sysget(ICO_DRIVE5_25, s)
#define icon_drivecd(s)          icon_sysget(ICO_DRIVECD, s)
#define icon_drivehd(s)          icon_sysget(ICO_DRIVEHD, s)
#define icon_favorites(s)        icon_sysget(ICO_FAVORITES, s)
#define icon_documents(s)        icon_sysget(ICO_DOCUMENTS, s)
#define icon_seallogo(s)         icon_sysget(ICO_SEALLOGO, s)
#define icon_audio(s)            icon_sysget(ICO_AUDIO, s)
#define icon_desktop(s)          icon_sysget(ICO_DESKTOP, s)


#define ICON_SYSGET(id, size)   get_icon_from_library(icon_system, id, size)
#define icon_sysget(id, size)   ICON_SYSGET(id, size)
#define icon_getid(s)           get_icon_id(s)

extern l_iconlibrary *icon_system;


/* fonts delcarations */

extern l_long  font_times_1_25;

extern l_font *font_system;
extern l_font *font_system_bd;
extern l_font *font_system_i;
extern l_font *font_system_bi;

extern void (*screen_shot) ( void );

#define     FONT_GETSTRWIDTH(f,str,l)  (f?GrFontTextLength(f,str,l):0)
#define     FONT_GETSTRHEIGHT(f)       (f?GrFontTextHeight(f):0)
#define     FONT_GETWIDTH(f,ch)        (f?GrFontCharLength(f,ch):0)
#define     FONT_GETHEIGHT(f)          FONT_GETSTRHEIGHT(f)

typedef struct t_drivers *p_drivers;

typedef struct t_drivers {

  t_object  obclass;

} t_drivers;


void icons_init ( void );
void icons_done ( void );
void fonts_init ( void );
void fonts_done ( void );

l_int  get_icon_id ( l_text n );

l_bool  drivers_done ( t_object *o );
t_drivers *drivers_init ( t_drivers *o );

#endif

#ifndef __VIEW_H_INCLUDED__
#define __VIEW_H_INCLUDED__


/* drag mode settings */

#define DM_DRAGMOVE           0x0000001 /* it's possible to drag and move it */
#define DM_DRAGGROW           0x0000002 /* it's possible to drag and grow it */
#define DM_DRAGCONTEXT        0x0000004 /* drag by context */


/* options settings */

#define VW_OF_VISIBILITY      0x0000020 /* it's able to visible */
#define VW_OF_IGNORELIM       0x0000040 /* view ignore owner's limitation */


/* next alignments */

#define  TX_ALIGN_FIXEDX      0x1000    /* sizey is fixed to size of group */
#define  TX_ALIGN_FIXEDY      0x2000    /* sizey is fixed to size of group */


/* draw mode settings */

#define DWM_TESTSUBVIEWS      0x0000001 /* clip sub views */
#define DWM_ONLYTOBUFFER      0x0000002 /* output is only to buffer */
#define DWM_ONLYTOSCREEN      0x0000004 /* output is only to screen */
#define DWM_CANTACCELMOVE     0x0000008 /* disable accel moving */
#define DWM_ICONICDRAW        0x0000010 /* draw under icon */


/* brush states */

#define BRUSH_STRETCH         0x0000001 /* background is stretched */
#define BRUSH_SELFIMG         0x0000002 /* image is only in this brush */
#define BRUSH_GRADIENT        0x0000004 /* color is gradient to color2 */
#define BRUSH_GRADIENT_HOR    0x0000008 /* gradient is horizontal, else is vertical */
#define BRUSH_CENTER          0x0000010 /* center image to the center */


#define OUTBUFFER(o)          (*(o))

#define RETVIEW(o,e)          if ( !(o->state & OB_SF_VISIBLE) || ( e->type == EV_MOUSE && !VIEW(o)->is_mouse_in_view(VIEW(o))) ) return

typedef struct t_brush {

  l_long     state;
  l_color    color;
  BITMAP    *background;
  l_color    color2;
  l_char     reserved[22];

} t_brush;



/* info board */

extern l_int   info_board_key;
/* info board foreground color = black */
extern l_color info_board_fcolor;
/* info board background color */
extern l_color info_board_b1color;
/* info board background color */
extern l_color info_board_b2color;
/* info board font = helv15 */
extern l_font *info_board_font;

void   info_board_init ( void );
void   view_ini ( void );


/* size of standard item (menu, listbox, ... )*/

extern l_int   system_item_size;


/* able to drag all context of object */
extern l_bool  drag_context_ok;
/* enable accelaration of moving */
extern l_bool  accel_moving_ok;


typedef struct t_view *p_view;

typedef struct t_view {

  struct t_object   obclass;

  struct t_brush    brush;
  struct t_rect     bounds;
  struct t_rect     clip;
  struct t_rect    *safe_clip;

  BITMAP           *draw_buffer;
  BITMAP          **draw_out;
  l_font           *font;
  l_int             cursor;

  l_text            info_text;
  l_int             align;

  l_color          *palette;

  l_int             draw_mode;
  l_int             drag_mode;
  l_long            draw_lock;

  l_int             icon_size;


  l_dword (*execute_view) ( p_view o, p_view sub );

  p_view  (*get_top_view_under_mouse) ( p_view o );
  p_view  (*top_view) ( p_view o );
  void    (*show_info_board) ( p_view o );

  p_data  (*drag_data) ( p_view o );
  l_bool  (*drag_where) ( p_view o, p_data rec, t_point where );
  l_bool  (*drop_data) ( p_view o, p_data rec, t_point where );

  void    (*drag_view) ( p_view o, l_word mode, t_event *event );
  t_rect  (*drag) ( p_view o, l_word mode, t_event *event );
  t_point (*size_minimum) ( p_view o );
  t_rect  (*get_limits) ( p_view o );

  void    (*reset_align) ( p_view o );

  void    (*draw) ( p_view o );
  void    (*draw_view) ( p_view o );
  void    (*draw_sub_views) ( p_view o, p_view from, p_view to );

  void    (*set_mouse_cursor) ( p_view o );
  l_bool  (*is_mouse_in_view) ( p_view o );
  p_view  (*get_view_under_mouse) ( p_view o );
  void    (*lock_drawing) ( p_view o );
  l_bool  (*unlock_drawing) ( p_view o );
  l_bool  (*is_draw_lock) ( p_view o );

  BITMAP* (*begin_paint) ( p_view o, t_point *p, t_rect rwhere );
  void    (*end_of_paint) ( p_view o, t_rect rwhere );

  void    (*draw_overlays) ( p_view o );
  l_bool  (*draw_mini_box) ( p_view o, BITMAP *out, BITMAP *buffer, t_rect rwhere );

  void    (*draw_under_view) ( p_view o );
  void    (*draw_under_rect) ( p_view o, t_rect r );
  void    (*draw_in_rect) ( p_view o, t_rect r );
  void    (*draw_me) ( p_view o );

  l_color (*get_color) ( p_view o, l_int cpos );
  void    (*set_palette) ( p_view o, l_color *palette );

  l_bool  (*is_top_view) ( p_view o );
  void    (*move_accel) ( p_view o, t_rect from, t_rect to );
  void    (*move_view) ( p_view o, t_rect r );
  void    (*grow_view) ( p_view o, t_rect r );

  t_rect  (*get_local_extent) ( p_view o );

  t_rect  (*get_global_bounds) ( p_view o, t_rect r );
  t_rect  (*get_local_bounds) ( p_view o, t_rect r );
  t_point (*get_global_point) ( p_view o, t_point p );
  t_point (*get_local_point) ( p_view o, t_point p );

  t_rect* (*get_not_overlays) ( p_view o, t_rect rlocal, t_rect rgroup,
                                p_view p, p_view target, l_long *rnum,
                                t_rect **rout, l_bool sub, l_bool ws );

  void    (*show) ( p_view o );
  void    (*hide) ( p_view o );

  void    (*for_each_sub_view_set_state) ( p_view o, l_dword st, l_bool set );
  void    (*set_draw_mode) ( p_view o, l_int dm, l_bool set );
  l_bool  (*is_draw_mode) ( p_view o, l_int dm );

  t_rect  (*size_limits) ( p_view o );
  t_rect  (*get_cliped_extent) ( p_view o );

  void    (*set_bounds) ( p_view o, t_rect r );

  void    (*change_bounds) ( p_view o, t_rect nr );

  void    (*set_clip) ( p_view o, BITMAP *out, t_rect r );
  void    (*background) ( p_view o, BITMAP *out, t_rect r );

  void    (*set_clips) ( p_view o, t_rect r );
  void    (*reset_clips) ( p_view o );

} t_view;

  #define VIEW(o)   ((p_view)(o))


  #define icon_size(o)  (VIEW(o)->icon_size)

  #define reset_clip(o, out)  set_clip(o, out, o->clip)


/* object's functions */

  void    view_reset_prefer ( p_object o );
  l_bool  view_done ( p_object o );
  void    view_after_init ( p_object o );
  void    view_setup ( p_object o );
  void    view_set_state ( p_object o, l_dword st, l_bool set );
  void    view_put_in_front_of ( p_object o, p_object before );
  void    view_translate_event ( p_object o, t_event *event );
  void    view_play_process ( p_object o, t_event *event );


/* view's functions */

  extern p_view  (*view_init) ( p_view o, t_rect r );
  p_view  _view_init ( p_view o, t_rect r );

  void    view_reset_align ( p_view o );
  l_dword view_execute_view ( p_view o, p_view sub );

  p_view  view_get_top_view_under_mouse ( p_view o );
  p_view  view_top_view ( p_view o );
  void    view_show_info_board ( p_view o );
  void    view_set_mouse_cursor ( p_view o );
  l_bool  view_is_mouse_in_view ( p_view o );

  p_data  view_drag_data ( p_view o );
  l_bool  view_drag_where ( p_view o, p_data rec, t_point where );
  l_bool  view_drop_data ( p_view o, p_data rec, t_point where );

  void    view_drag_view ( p_view o, l_word mode, t_event *event );
  t_rect  view_drag ( p_view o, l_word _mode, t_event *event );
  t_point view_size_minimum ( p_view o );
  t_rect  view_get_limits ( p_view o );


  void    view_draw ( p_view o );
  void    view_draw_view ( p_view o );
  void    view_draw_sub_views ( p_view o, p_view from, p_view to );
  void    view_draw_me ( p_view o );

  l_color view_get_color ( p_view o, l_int cpos );
  void    view_set_palette ( p_view o, l_int *palette );

  l_bool  view_is_top_view ( p_view o );
  void    view_move_accel ( p_view o, t_rect from, t_rect to );
  void    view_move_view ( p_view o, t_rect r );
  void    view_grow_view ( p_view o, t_rect r );

  p_view  view_get_view_under_mouse ( p_view o );
  void    view_lock_drawing ( p_view o );
  l_bool  view_unlock_drawing ( p_view o );
  l_bool  view_is_draw_lock ( p_view o );

  BITMAP* view_begin_paint ( p_view o, t_point *p, t_rect rwhere );
  void    view_end_of_paint ( p_view o, t_rect rwhere );

  void    view_draw_overlays ( p_view o );
  l_bool  view_draw_mini_box ( p_view o, BITMAP *out, BITMAP *buffer, t_rect rwhere );

  void    view_draw_under_rect ( p_view o, t_rect r );
  void    view_draw_under_view ( p_view o );
  void    view_draw_in_rect ( p_view o, t_rect r );

  t_rect  view_get_local_extent ( p_view o );

  t_rect  view_get_global_bounds ( p_view o, t_rect r );
  t_rect  view_get_local_bounds ( p_view o, t_rect r );
  t_point view_get_global_point ( p_view o, t_point p );
  t_point view_get_local_point ( p_view o, t_point p );

  t_rect* view_get_not_overlays ( p_view o, t_rect rlocal, t_rect rgroup,
                                  p_view p, p_view target, l_long *rnum,
                                  t_rect **rout, l_bool sub, l_bool ws );

  void    view_show ( p_view o );
  void    view_hide ( p_view o );

  void    view_for_each_sub_view_set_state ( p_view o, l_dword st, l_bool set );
  void    view_set_draw_mode ( p_view o, l_int dm, l_bool set );
  l_bool  view_is_draw_mode ( p_view o, l_int dm );

  t_rect  view_size_limits ( p_view o );
  t_rect  view_get_cliped_extent ( p_view o );

  void    view_set_bounds ( p_view o, t_rect r );

  void    view_change_bounds ( p_view o, t_rect nr );

  void    view_set_clip ( p_view o, BITMAP *out, t_rect r );
  void    view_background ( p_view o, BITMAP *out, t_rect r );

  void    view_set_clips ( p_view o, t_rect r );
  void    view_reset_clips ( p_view o );


void  _show_info_board ( p_view from, l_text text );
l_bool is_top_view_under_mouse_in_me ( p_view o );


#define BLIT(src,dst,sx,sy,dx,dy,w,h)                                    \
do {                                                                     \
    l_bool iv = mouse->is_visible(mouse);                                \
    t_rect sc;                                                           \
    t_rect dc;                                                           \
    get_clip(src, (int*)(&sc.a.x), (int*)(&sc.a.y), (int*)(&sc.b.x), (int*)(&sc.b.y)); \
    get_clip(dst, (int*)(&dc.a.x), (int*)(&dc.a.y), (int*)(&dc.b.x), (int*)(&dc.b.y)); \
    set_clip(dst, dx, dy, dx+w, dy+h);                                   \
    set_clip(src, sx, sy, sx+w, sy+h);                                   \
    mouse->hide(mouse);                                                  \
    blit(src, dst, sx, sy, dx, dy, w+1, h+1);                            \
    if ( iv ) mouse->show(mouse);                                        \
    set_clip(dst, dc.a.x, dc.a.y, dc.b.x, dc.b.y);                       \
    set_clip(src, sc.a.x, sc.a.y, sc.b.x, sc.b.y);                       \
} while (0)


#define NOT_ACCEL_MOVE(o, x)                                            \
do {                                                                    \
    l_bool  tsv = VIEW(o)->is_draw_mode(VIEW(o), DWM_CANTACCELMOVE);    \
    VIEW(o)->set_draw_mode(VIEW(o), DWM_CANTACCELMOVE, true);           \
    ##x;                                                                \
    VIEW(o)->set_draw_mode(VIEW(o), DWM_CANTACCELMOVE, tsv);            \
} while (0)


#define TEST_SUB_VIEWS(o, x)                                            \
do {                                                                    \
    l_bool  tsv = VIEW(o)->is_draw_mode(VIEW(o), DWM_TESTSUBVIEWS);     \
    VIEW(o)->set_draw_mode(VIEW(o), DWM_TESTSUBVIEWS, true);            \
    ##x;                                                                \
    VIEW(o)->set_draw_mode(VIEW(o), DWM_TESTSUBVIEWS, tsv);             \
} while (0)


#define VIEW_CLIPED(o, r) rect_cliped(VIEW(o)->get_cliped_extent(VIEW(o)), r)


#endif


#ifndef __COLORS_H_INCLUDED__
#define __COLORS_H_INCLUDED__

#define  INI_COLORS             "colors"

#define  CO_SKIP_RED             255
#define  CO_SKIP_GREEN             0
#define  CO_SKIP_BLUE            255

#define  CO_ISCOLOR(x)           ((x) != CO_NOCOLOR)

#define  CO_NOCOLOR             -1
#define  CO_BLACK                0
#define  CO_BLUE                 1
#define  CO_GREEN                2
#define  CO_CYAN                 3
#define  CO_RED                  4
#define  CO_MAGENDA              5
#define  CO_BROWN                6
#define  CO_LIGHTGRAY            7
#define  CO_DARKGRAY             8
#define  CO_LIGHTBLUE            9
#define  CO_LIGHTGREEN           10
#define  CO_LIGHTCYAN            11
#define  CO_LIGHTRED             12
#define  CO_LIGHTMAGENDA         13
#define  CO_YELLOW               14
#define  CO_WHITE                15

#define  CO_SYSx0                0
#define  CO_SYSx1                1
#define  CO_SYSx2                2
#define  CO_SYSx3                3
#define  CO_SYSx4                4
#define  CO_SYSx5                5
#define  CO_SYSx6                6
#define  CO_SYSx7                7
#define  CO_SYSx8                8
#define  CO_SYSx9                9
#define  CO_SYSx10               10
#define  CO_SYSx11               11
#define  CO_SYSx12               12
#define  CO_SYSx13               13
#define  CO_SYSx14               14
#define  CO_SYSx15               15
#define  CO_SYSx16               16
#define  CO_SYSx17               17
#define  CO_SYSx18               18
#define  CO_SYSx19               19
#define  CO_SYSx20               20
#define  CO_SYSx21               21
#define  CO_SYSx22               22
#define  CO_SYSx23               23
#define  CO_SYSx24               24
#define  CO_SYSx25               25
#define  CO_SYSx26               26
#define  CO_SYSx27               27

#define COLOR(n) ((n)<colors_standard[0]?colors_standard[n+1]:0)

extern l_color *colors_standard;

extern l_byte palette_standard[];

l_color *color_trans_palette ( l_byte *rgb );
l_color  color_get_from_ini ( char *name_of_color );

void colors_init ( void );
void colors_done ( void );

#endif

#ifndef __WINDOW_H_INCLUDED__
#define __WINDOW_H_INCLUDED__

#include<string.h>

typedef struct t_window* p_window;

extern l_int  pal_window[];


typedef struct t_window {

  struct t_view  obclass;

  l_text         caption;
  l_int          flags;


  void       (*draw_title) ( p_window o );

} t_window;


#define WINDOW(o)   ((p_window)(o))


l_bool    win_done ( p_object o );
void      win_set_state ( p_object o, l_dword st, l_bool set );
t_rect    win_size_limits ( p_view o );
void      win_draw ( p_view o );
void      win_translate_event ( p_object o, t_event *event );
void      win_draw_title ( p_window o );

extern p_window  (*win_init) ( p_window o, t_rect r, l_text caption, l_int flags );
p_window  _win_init ( p_window o, t_rect r, l_text caption, l_int flags );

#endif


#ifndef __PROGRAM_H_INCLUDED__
#define __PROGRAM_H_INCLUDED__

#define INI_OPTIMALIZATION  "optimalization"
#define INI_DESKTOP         "desktop"

/* the difference between ticks */
extern l_int  task_tick;
/* the main program object */
extern t_object   program;
/* list of objects that use OB_OF_STILLPROCESS option flag */
extern t_list     stillprocess_list;
/* drivers, the main object for drivers, such as keyboard, mouse, ... */
extern p_drivers  drivers;
/* destkop, the main object of visible objects */
extern p_view     desktop;
/* define if clock-cursor must be included */
extern l_int      clock_including;

int  program_int ( void );

int  program_put_into_stillprocess ( p_object o, p_object s );
void program_put_event ( t_object *o, t_event *event );
void program_get_event ( t_object *o, t_event *event );

void program_translate_event ( t_object *o, t_event *event );
l_bool  program_done ( t_object *o );

t_object *program_init ( t_object *o );
void      program_call_each_stillprocess ( p_list o );

#define   program_shutdown()   do {                                            \
                set_event(&event_main, EV_MESSAGE, MSG_QUIT, OBJECT(&program));\
                OBJECT(&program)->put_event(OBJECT(&program), &event_main);    \
          } while (0)


#ifdef    ENABLE_MULTITASKER
l_bool    __multi_process = true;
#define   HALT_MULTITASKER() __multi_process=false
#define   while(t1)      _while((__multi_process)&&(##t1))
#endif

#endif

#endif

