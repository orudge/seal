/*

(c) Copyright 2001 Julien Etelain. All rights reserved.

*/

#ifndef __IMGEDIT_H_INCLUDED__
#define __IMGEDIT_H_INCLUDED__

#include<scroller.h>
#include<dialogs.h>
#include"grfx-c.h"
#include<list.h>


typedef struct t_colsel2   *p_colsel2;

typedef struct t_imgedit   *p_imgedit;

typedef struct t_imgfilter *p_imgfilter;

typedef struct t_undoimgedit *p_undoimgedit;

typedef struct t_imgfilter {

  void    (*smallpreview) ( BITMAP *bmp );
  void    (*filterbox)    ( BITMAP *bmp );
  l_text  copyright;
  l_text  name;
  l_text  description;

} t_imgfilter;

typedef struct t_undoimgedit {

  l_int  select;
  t_rect actsel;
  BITMAP *selection;
  BITMAP *picture;
  BITMAP *temp;

} t_undoimgedit;

typedef struct t_colsel2 {

  struct t_view    obclass;

  p_imgedit  imgedit;

} t_colsel2;



typedef struct t_imgedit {

  struct t_view    obclass;
  //struct t_scroller obclass;

  BITMAP* picture;
  l_long  tool;

  l_color col1;
  l_color col2;
  l_color col3; // << will be used for the midle button ...

  t_point realold;

  l_int zoom;
  l_int topx;
  l_int topy;

  l_int oldx;
  l_int oldy;

  l_int editing;

  l_int bt;

  l_int fillmode;

  l_int floodtol;      // << Not yet used ...
  l_int floodstyle;    // << ``  ``   ``

  t_rect actsel;
  l_int  select;

  l_int border;
  l_int penstyle;
  l_int forme;

  l_int floodmode;     // <- transmode  !
  l_int floodlevel;    // <- translevel !

  p_stattext status;  // Stettext used for status
  p_colsel2 colsel;   // Color selector

  BITMAP *temp;
  BITMAP *selection;

  p_list undolist;

  l_int pr;
  l_int set1;
  l_int set2;

  l_text filename;

  void (*save)   ( p_imgedit o, l_text filename, l_long quality); // quality :  for futur versions ...
  void (*load)   ( p_imgedit o, l_text filename);
  void (*newbmp) ( p_imgedit o, l_long width, l_long height);

} t_imgedit;

////////////////////////////////////////////////////////////////////////////////

#define imgedit(o)      ((p_imgedit)(o))
#define IMGEDIT(o)      ((p_imgedit)(o))

#define colsel2(o)      ((p_colsel2)(o))
#define COLSEL2(o)      ((p_colsel2)(o))

#define imgfilter(o)    ((p_imgfilter)(o))
#define IMGFILTER(o)    ((p_imgfilter)(o))

#define undoimgedit(o)  ((p_undoimgedit)(o))
#define UNDOIMGEDIT(o)  ((p_undoimgedit)(o))

////////////////////////////////////////////////////////////////////////////////

extern p_list      imgfilters;

////////////////////////////////////////////////////////////////////////////////

#define TOOL_MOVE     0
#define TOOL_ZOOM     1
#define TOOL_GET      2
#define TOOL_POINT    3
#define TOOL_FILL     4
#define TOOL_FORME    5
#define TOOL_SELECT   6
#define TOOL_TEXT     7

////////////////////////////////////////////////////////////////////////////////

#define PCOPY_COPY           0
#define PCOPY_PASTASNEWPIC   1
#define PCOPY_PASTASNEWSEL   2

////////////////////////////////////////////////////////////////////////////////

void  free_imgfilter ( void *o );
void  add_imgfilter (  void    (*smallpreview) ( BITMAP *bmp ),
                      void    (*filterbox)    ( BITMAP *bmp ),
                      l_text  copyright,
                      l_text  name,
                      l_text  description );
p_imgfilter  new_imgfilter(   void    (*smallpreview) ( BITMAP *bmp ),
                              void    (*filterbox)    ( BITMAP *bmp ),
                              l_text  copyright,
                              l_text  name,
                              l_text  description );
p_imgfilter getimgfilter ( l_int id );
void init_imgfilters ( );

////////////////////////////////////////////////////////////////////////////////

void      imgedit_draw             ( p_view o );
void      imgedit_save             ( p_imgedit o, l_text filename, l_long quality);
void      imgedit_load             ( p_imgedit o, l_text filename);
void      imgedit_newbmp           ( p_imgedit o, l_long width, l_long height);
void      imgedit_translate_event  ( p_object o, t_event *event );
void      imgedit_settop           ( p_imgedit o, l_int x, l_int y);
void      imgedit_update_mouse     ( p_imgedit o );
void      imgedit_update_mouse_cursor ( p_view o );
t_point   imgedit_get_x_y          ( p_imgedit o );
void      imgedit_update_status    ( p_imgedit o );

void      imgedit_make_it_sel      ( p_imgedit o, BITMAP *bmp, l_int x , l_int y );
void      imgedit_pcopy_act        ( p_imgedit o, l_int action );
void      imgedit_change_select_mode ( p_imgedit o, l_int newvalue );
void      imgedit_undo_add         ( p_imgedit o );
void      imgedit_undo             ( p_imgedit o );
////////////////////////////////////////////////////////////////////////////////

void colsel2_draw    ( p_view o );
void colsel2_translate_event  ( p_object o, t_event *event );

////////////////////////////////////////////////////////////////////////////////

p_colsel2 _colsel2_init ( p_colsel2 o, t_rect r );
extern p_colsel2 (*colsel2_init) ( p_colsel2 o, t_rect r );

////////////////////////////////////////////////////////////////////////////////

p_imgedit _imgedit_init ( p_imgedit o, t_rect r, p_colsel2 cc , p_stattext st );
extern p_imgedit (*imgedit_init) ( p_imgedit o, t_rect r, p_colsel2 cc, p_stattext st );


#endif
