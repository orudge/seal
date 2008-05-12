/****************************************************************/
/*                                                              */
/*                          load_htm.h                          */
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



#ifndef __LOAD_HTM_H_INCLUDED__
#define __LOAD_HTM_H_INCLUDED__

#include"scroller.h"
#include<dir.h>


/* htmloader texts */

#ifndef TXT_NOTFOUND
#define TXT_NOTFOUND  INI_TEXT("Not found")
#endif



/* htmloader messages */

#define MSG_ANCHORPRESSED     121


/*
  begin of HTML_OBS_DECLARATION
*/

#ifndef HTML_OBS_DECLARATION
#define HTML_OBS_DECLARATION

#define HTML_DEF_FONT                      l_font
#define HTML_DEF_BITMAP                    BITMAP
#define HTML_DEF_COLOR                     l_color

/*
  html object floating structure.
  it use for save information about alignment and floating arround objects
*/

typedef struct html_floating {

  long                  hfromy;
  long                  htoy;
  long                  hplusleft;
  long                  hminusright;
  struct html_floating *hnext;

} html_floating;


/*
  html object downloads structure.
  it use for save information about downloaded objects
*/

typedef struct html_dwns {
  char              *hid;       /* identification string */
  void              *hptr;      /* ptr to object */
  int                hidnum;    /* type of object. Can be one of xxx_S... */
  struct html_dwns  *hnext;     /* next object */
  char               hres[24];  /* reserved for future purpose */
} html_dwns;


/*
  definition of origins
*/
typedef struct html_rect {

  long         hleft;
  long         htop;
  long         hright;
  long         hbottom;

} html_rect;


extern html_rect HTML_RECT_EMPTY;



typedef struct  html_type {

  int                 hid;
  int                 hfont;
  HTML_DEF_COLOR      hfcolor;
  HTML_DEF_COLOR      hbcolor;
  HTML_DEF_COLOR      hacolor;
  int                 halign;
  int                 hstyle;
  int                 hignorespaces;
  int                 hnumber;
  int                 htdnumber;
  int                 htrnumber;
  int                 htdstyle;
  int                 hlisttype;
  int                 hnowrap;
  int                 htdborder;

  char               *hreference;
  char               *hanchorname;

  long               *hcolmin;
  long               *hcolmax;
  long               *hcolwidth;
  int                 htdcols;

  int                 htdcellspacing;
  int                 htdcellpadding;

  struct html_rect    htdframe;
  struct html_rect    hframe;

} html_type;



typedef struct html_standard {

  int                   hident;
  char                 *htext;
  int                   halign;
  int                   hstyle;
  char                 *hreference;
  char                 *hanchorname;
  struct html_type      hsafe;
  struct html_rect      hrect;
  struct html_rect      hframe;

} html_standard;


typedef struct html_horline {

  struct html_standard  hclass;
  int                   hnoshade;

} html_horline;


typedef struct html_blocklevel {

  struct html_standard  hclass;
  HTML_DEF_COLOR        hfcolor;
  int                   hfont;

  int                   hnumber;
  int                   hselected;
  int                   hlisttype;

} html_blocklevel;


typedef struct html_image {

  struct html_blocklevel  hclass;

  HTML_DEF_BITMAP        *hbitmap;
  HTML_DEF_BITMAP        *hlowbitmap;
  int                     hborderwidth;
  char                   *honabort;
  char                   *honerror;
  char                   *honload;

} html_image;



#define HTML_INPUT_NONE      0
#define HTML_INPUT_BUTTON    1
#define HTML_INPUT_CHECKBOX  2
#define HTML_INPUT_FILE      3
#define HTML_INPUT_HIDDEN    4
#define HTML_INPUT_IMAGE     5
#define HTML_INPUT_PASSWORD  6
#define HTML_INPUT_RADIO     7
#define HTML_INPUT_RESET     8
#define HTML_INPUT_SUBMIT    9
#define HTML_INPUT_TEXT     10


typedef struct html_input {

  struct html_blocklevel  hclass;

  int                     htype;
  char                   *hname;
  char                   *hvalue;
  int                     hivalue;
  int                     hselfcom;
  int                     hinserted;

} html_input;



#define HTML_TABLE_RENDER                1

#define HTML_STYLE_TABLE_NONE            0
#define HTML_STYLE_TABLE_MAX          0x01
#define HTML_STYLE_TABLE_MIN          0x02
#define HTML_STYLE_TABLE_RENDER       0x04
#define HTML_STYLE_TABLE_LASTTOEND    0x08
#define HTML_STYLE_TABLE_NOW          0x10

typedef struct html_table {

  struct html_blocklevel  hclass;

  int                     hborder;
  int                     hcellpadding;
  int                     hcellspacing;
  int                     hheight;
  int                     hwidth;
  int                     hcols;
  HTML_DEF_COLOR          hbgcolor;

  long                   *hcolmin;
  long                   *hcolmax;
  long                   *hcolwidth;
  struct html_engine     *hengine;

} html_table;


typedef struct html_tr {

  struct html_blocklevel  hclass;
  int                     hvalign;
  HTML_DEF_COLOR          hbgcolor;
  int                     hcellpadding;
  int                     hborder;

} html_tr;



typedef struct html_td {

  struct html_blocklevel  hclass;
  int                     hvalign;
  int                     hcolspan;
  int                     hrowspan;
  int                     hrow;
  int                     hheight;
  int                     hwidth;
  HTML_DEF_COLOR          hbgcolor;
  int                     hcellpadding;
  int                     hborder;
  struct html_engine     *hengine;

} html_td;

#define html_lists   html_blocklevel
#define html_anchor  html_blocklevel
#define html_title   html_standard
#define html_nobr    html_standard



#define HTML_LISTTYPE_DISC            0
#define HTML_LISTTYPE_CIRCLE          1
#define HTML_LISTTYPE_SQUARE          2
#define HTML_LISTTYPE_A               3
#define HTML_LISTTYPE_a               4
#define HTML_LISTTYPE_I               5
#define HTML_LISTTYPE_i               6
#define HTML_LISTTYPE_1               7


/*
  objects for html output
*/
typedef struct html_obs {

   int                   hid;                 /* type of object */
   int                   hidin;
   int                   hend;
   int                   hview;
   int                   hwasalign;
   long                  hline;
   struct html_standard *hobjtype;
   struct html_obs      *hnext;               /* next object in the directory */
   struct html_obs      *hprev;

} html_obs;


#define conv_standard(p) ((html_standard*)p)


#define HTML_ST_BLOCKLEVEL    0x01
#define HTML_ST_LI            0x02
#define HTML_ST_DL            0x04
#define HTML_ST_DT            0x08
#define HTML_ST_DD            0x10


typedef struct  html_safe_table {

   long                    *hcolmin;
   long                    *hcolmax;
   long                    *hcolwidth;
   long                     hid;

   struct html_safe_table  *next;

} html_safe_table;



typedef struct  html_safe_td {

   int                      hmin;
   int                      hmax;
   long                     hid;

   struct html_safe_td  *next;

} html_safe_td;


/*
  main definition actual for object engine
*/

#ifndef    HTML_ENGINE
#define    HTML_ENGINE           html_engine
#endif
#ifndef    HTML_DEF_ENGINE
#define    HTML_DEF_ENGINE       HTML_ENGINE *engine
#endif

typedef struct  html_engine {

  /* internal definitions */
  int                        hid;
  int                        hidrealnow;
  int                        hlockenter;
  int                        hendenter;
  int                        hlinespace;
  int                        hlinealign;
  int                        hview;
  int                        hstyle;

  int                        hlastlinespace;
  int                        hlastwasspace;

  int                        hnowrap;

  long                       hline;
  long                       hlinemove;

  char                      *horiginalpos;
  struct html_safe_table    *hsafetable;
  struct html_safe_td       *hsafetd;
  struct html_engine        *hfirstengine;

  /* out */
  HTML_DEF_COLOR             hbcolor;
  HTML_DEF_COLOR             hfcolor;
  HTML_DEF_COLOR             halcolor;
  HTML_DEF_COLOR             hlicolor;
  HTML_DEF_COLOR             hvlcolor;
  HTML_DEF_BITMAP           *hpattern;

  struct html_rect           hrect;
  struct html_rect           hframe;
  struct html_type           htype;
  struct html_type           hstandard;
  struct html_obs           *hobjectlast;
  struct html_floating      *hfloats;

  struct html_dwns          *hdownloads;
  struct html_obs           *hobjectsout;

  int                        htablestyle;

  int                        htab_blockquote;
  int                        htab_list;

  char                      *hpath;
  char                      *htitle;

  char                       hreserved[124];

} html_engine;


#endif /* end of HTML_OBS_DECLARATION */



extern l_color pal_htmloader[];


/* texts for htmloader */

#define TXT_FILENOTFOUND INI_TEXT("File Not Found")


typedef struct html_engine *p_html_engine;
typedef struct html_obs    *p_html_obs;

/* t_htmloader */

typedef struct t_htmloader *p_htmloader;

typedef struct t_htmloader {

   struct t_scroller obclass;

   l_text            filename;

   p_html_engine     hengine;

   l_dword           flags;

   l_long            y_last;
   l_long            x_last;

   l_bool            (*init_engine) ( p_htmloader o, l_text filename );
   l_bool            (*load_html) ( p_htmloader o, l_text filename );
   void              (*draw_box) ( p_htmloader o );
   void              (*draw_table) ( p_htmloader o, BITMAP *out, HTML_DEF_ENGINE, t_point p, l_long dx, l_long dy );
   p_html_obs        (*find_anchor) ( p_htmloader o );
   t_rect            (*find_anchor_name_ex) ( p_htmloader o, HTML_DEF_ENGINE, l_rect dx, l_dword dy, l_text anchorname );
   t_rect            (*find_anchor_name) ( p_htmloader o, HTML_DEF_ENGINE, l_text anchorname );
   p_html_obs        (*find_object_under_mouse) ( p_htmloader o, t_point where );
   p_html_obs        (*find_object_under_mouse_ex) ( p_htmloader o, HTML_DEF_ENGINE, l_rect dx, l_dword dy, t_point where );

} t_htmloader;

#define HTMLOADER(o) ((p_htmloader)(o))


/* htmloader flags */

#define HLF_HORSCROLLBAR     0x000001
#define HLF_VERSCROLLBAR     0x000002

#define HLF_3D               0x000004
#define HLF_FULLSCREEN       0
#define HLF_DEFAULT          (HLF_3D|HLF_HORSCROLLBAR|HLF_VERSCROLLBAR)


/* t_htmloader functions */

void    htmloader_setup ( p_object o );
t_rect  htmloader_size_limits ( p_view o );
void    htmloader_draw ( p_view o );
l_bool  htmloader_done ( p_object o );
void    htmloader_scroll_size ( p_scroller o, l_long *x, l_long *y );
void    htmloader_translate_event ( p_object o, t_event *event );

void        htmloader_draw_box ( p_htmloader o );
void        htmloader_draw_table ( p_htmloader o, BITMAP *out, HTML_DEF_ENGINE, t_point p, l_long dx, l_long dy );
l_bool      htmloader_init_engine ( p_htmloader o, l_text filename );
l_bool      htmloader_load_html ( p_htmloader o, l_text filename );
t_rect      htmloader_find_anchor_name_ex ( p_htmloader o, HTML_DEF_ENGINE, l_rect dx, l_dword dy, l_text anchorname );
t_rect      htmloader_find_anchor_name ( p_htmloader o, HTML_DEF_ENGINE, l_text anchorname );
p_html_obs  htmloader_find_anchor ( p_htmloader o );
p_html_obs  htmloader_find_object_under_mouse ( p_htmloader o, t_point where );
p_html_obs  htmloader_find_object_under_mouse_ex ( p_htmloader o, HTML_DEF_ENGINE, l_rect dx, l_dword dy, t_point where );

extern p_htmloader  (*htmloader_init) ( p_htmloader o, t_rect r, l_text filename );
p_htmloader  _htmloader_init ( p_htmloader o, t_rect r, l_text filename );

extern p_htmloader  (*htmloader_init_ex) ( p_htmloader o, t_rect r, l_text filename, l_dword flags );
p_htmloader  _htmloader_init_ex ( p_htmloader o, t_rect r, l_text filename, l_dword flags );

#endif
