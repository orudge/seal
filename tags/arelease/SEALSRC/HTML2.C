/****************************************************************/
/*                                                              */
/*                           html2.c                            */
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


#ifndef __FHT_HTML2_INCLUDED__
#define __FHT_HTML2_INCLUDED__


#include<ctype.h>
#include<io.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define __FREE_HYPER_TEXT_VERSION__       "1.00"
#define __FREE_HYPER_TEXT_NAME__          "FreeHyperText"
#define __FREE_HYPER_TEXT_AUTHOR__        "Michal Stencl"
#define __FREE_HYPER_TEXT_DATE__          "05. april 1999"




/* - PART FOR OWN REDECLARATION ************************************** */


#ifndef _malloc
#define _malloc      malloc
#endif
#ifndef _free
#define _free        free
#endif
#ifndef _realloc
#define _realloc     realloc
#endif
#ifndef  _strdup
#define  _strdup     strdup
#endif
#ifndef  _calloc
#define  _calloc     calloc
#endif
#ifndef  _cfree
#define  _cfree      cfree
#endif

/* definitions for spaces between lines */

#ifndef HTML_BLOCKLEVEL_SPACE
#define HTML_BLOCKLEVEL_SPACE    1
#endif
#ifndef HTML_LISTS_SPACE
#define HTML_LISTS_SPACE         1
#endif
#define HTML_SPECIAL_SPACE       HTML_BLOCKLEVEL_SPACE

#ifndef HTML_LINE_SPACE
#define HTML_LINE_SPACE          0
#endif

#ifndef HTML_BRLINE_SPACE
#define HTML_BRLINE_SPACE        1 /* in graphics use as LINESPACE */
#endif


/* color functions */

#ifndef HTML_COLOR_MAKE
#define HTML_COLOR_MAKE             1
#endif

#ifndef HTML_COLOR_ANCHOR
#define HTML_COLOR_ANCHOR           0
#endif


#ifndef HTML_CLASS
#define HTML_CLASS(x)              x
#endif


/* max number of characters alocated for path */

#ifndef HTML_PATH_MAX
#define HTML_PATH_MAX               79
#endif


/*
  font structure. Redefine by yourself
*/
#ifndef HTML_FUNC_FONT_LOAD
#define HTML_FUNC_FONT_LOAD(f,w,h)  NULL
#endif

#ifndef HTML_FUNC_FONT_UNLOAD
#define HTML_FUNC_FONT_UNLOAD(f)
#endif

#ifndef HTML_FUNC_FONT_GETCHARWIDTH
#define HTML_FUNC_FONT_GETCHARWIDTH(f,c)              1
#endif

#ifndef HTML_FUNC_FONT_GETSTRWIDTH
#define HTML_FUNC_FONT_GETSTRWIDTH(f,str,s)           s
#endif

#ifndef HTML_FUNC_FONT_GETHEIGHT
#define HTML_FUNC_FONT_GETHEIGHT(f)                   1
#endif



/*
  make long color from r,g,b
*/
#ifndef HTML_FUNC_COLOR_MAKERGB
#define HTML_FUNC_COLOR_MAKERGB(r,g,b)                0
#endif


/*
  font alignments
*/
#ifndef HTML_ALIGN_NOALIGN
#define HTML_ALIGN_NOALIGN     0x00
#endif

#ifndef HTML_ALIGN_CENTERX
#define HTML_ALIGN_CENTERX     0x01
#endif

#ifndef HTML_ALIGN_RIGHT
#define HTML_ALIGN_RIGHT       0x02
#endif

#ifndef HTML_ALIGN_LEFT
#define HTML_ALIGN_LEFT        0x04
#endif

#ifndef HTML_ALIGN_TOP
#define HTML_ALIGN_TOP         0x08
#endif

#ifndef HTML_ALIGN_BOTTOM
#define HTML_ALIGN_BOTTOM      0x10
#endif

#ifndef HTML_ALIGN_MIDDLE
#define HTML_ALIGN_MIDDLE      0x20
#endif

#ifndef HTML_ALIGN_SELFALIGN
#define HTML_ALIGN_SELFALIGN   0x40
#endif

#ifndef HTML_ALIGN_CENTERY
#define HTML_ALIGN_CENTERY     0x80
#endif



#ifndef HTML_STYLE_UNDERLINED
#define HTML_STYLE_UNDERLINED  0x01
#endif

#ifndef HTML_STYLE_FLOAT
#define HTML_STYLE_FLOAT       0x02
#endif


#define HTML_NO_ALIGNMENT(i) ((i) == HTML_ALIGN_NOALIGN)



/*

  input sizes

*/

#ifndef HTML_INPUT_BUTTON_WIDTH
#define HTML_INPUT_BUTTON_WIDTH(x)       0
#endif
#ifndef HTML_INPUT_BUTTON_HEIGHT
#define HTML_INPUT_BUTTON_HEIGHT(x)      0
#endif

#ifndef HTML_INPUT_CHECKBOX_WIDTH
#define HTML_INPUT_CHECKBOX_WIDTH(x)     0
#endif
#ifndef HTML_INPUT_CHECKBOX_HEIGHT
#define HTML_INPUT_CHECKBOX_HEIGHT(x)    0
#endif

#ifndef HTML_INPUT_FILE_WIDTH
#define HTML_INPUT_FILE_WIDTH(x)         0
#endif
#ifndef HTML_INPUT_FILE_HEIGHT
#define HTML_INPUT_FILE_HEIGHT(x)        0
#endif

#ifndef HTML_INPUT_HIDDEN_WIDTH
#define HTML_INPUT_HIDDEN_WIDTH(x)       0
#endif
#ifndef HTML_INPUT_HIDDEN_HEIGHT
#define HTML_INPUT_HIDDEN_HEIGHT(x)      0
#endif

#ifndef HTML_INPUT_IMAGE_WIDTH
#define HTML_INPUT_IMAGE_WIDTH(x)        0
#endif
#ifndef HTML_INPUT_IMAGE_HEIGHT
#define HTML_INPUT_IMAGE_HEIGHT(x)       0
#endif

#ifndef HTML_INPUT_PASSWORD_WIDTH
#define HTML_INPUT_PASSWORD_WIDTH(x)     0
#endif
#ifndef HTML_INPUT_PASSWORD_HEIGHT
#define HTML_INPUT_PASSWORD_HEIGHT(x)    0
#endif

#ifndef HTML_INPUT_RADIO_WIDTH
#define HTML_INPUT_RADIO_WIDTH(x)        0
#endif
#ifndef HTML_INPUT_RADIO_HEIGHT
#define HTML_INPUT_RADIO_HEIGHT(x)       0
#endif

#ifndef HTML_INPUT_RESET_WIDTH
#define HTML_INPUT_RESET_WIDTH(x)        0
#endif
#ifndef HTML_INPUT_RESET_HEIGHT
#define HTML_INPUT_RESET_HEIGHT(x)       0
#endif

#ifndef HTML_INPUT_SUBMIT_WIDTH
#define HTML_INPUT_SUBMIT_WIDTH(x)       0
#endif
#ifndef HTML_INPUT_SUBMIT_HEIGHT
#define HTML_INPUT_SUBMIT_HEIGHT(x)      0
#endif

#ifndef HTML_INPUT_TEXT_WIDTH
#define HTML_INPUT_TEXT_WIDTH(x)         0
#endif
#ifndef HTML_INPUT_TEXT_HEIGHT
#define HTML_INPUT_TEXT_HEIGHT(x)        0
#endif



/*
  bitmap funcs
*/
#ifndef HTML_FUNC_BITMAP_DESTROY
#define HTML_FUNC_BITMAP_DESTROY(b)
#endif

#ifndef HTML_FUNC_BITMAP_LOAD
#define HTML_FUNC_BITMAP_LOAD(f)         NULL
#endif

/*
  HTML_FUNC_BITMAP_STRETCH stretch bitmap to another.
  it means, your definition of function must create new bitmap in "w", "h"
  sizes, stretch old bitmap "b" to new one, free old bitmap "b" and
  return new bitmap.
*/
#ifndef HTML_FUNC_BITMAP_STRETCH
#define HTML_FUNC_BITMAP_STRETCH(b,w,h)    NULL
#endif

#ifndef HTML_FUNC_BITMAP_GETWIDTH
#define HTML_FUNC_BITMAP_GETWIDTH(b)       0
#endif

#ifndef HTML_FUNC_BITMAP_GETHEIGHT
#define HTML_FUNC_BITMAP_GETHEIGHT(b)      0
#endif

/* define sizex of image that represented no-image */
#ifndef HTML_BITMAP_NOSIZEX
#define HTML_BITMAP_NOSIZEX                0
#endif

/* define sizey of image that represented no-image */
#ifndef HTML_BITMAP_NOSIZEY
#define HTML_BITMAP_NOSIZEY                0
#endif

#ifndef HTML_BITMAP_MINIMUMX
#define HTML_BITMAP_MINIMUMX               0
#endif

#ifndef HTML_BITMAP_MINIMUMY
#define HTML_BITMAP_MINIMUMY               0
#endif



/*
  font's names
*/
#ifndef HTML_FONT_NAMES
#define HTML_FONT_NAMES
char   *HTML_FONT_REGULAR_NAME          = NULL;
char   *HTML_FONT_ITALIC_NAME           = NULL;
char   *HTML_FONT_BOLD_NAME             = NULL;
char   *HTML_FONT_BOLDITALIC_NAME       = NULL;
char   *HTML_FONT_FIXED_NAME            = NULL;
char   *HTML_FONT_FIXEDBOLD_NAME        = NULL;
char   *HTML_FONT_FIXEDITALIC_NAME      = NULL;
char   *HTML_FONT_FIXEDBOLDITALIC_NAME  = NULL;
char   *HTML_FONT_DIRECTORY_NAME        = NULL;
#endif


#ifndef HTML_COLOR_VLINK
#define HTML_COLOR_VLINK   0
#endif


#ifndef HTML_COLOR_ALINK
#define HTML_COLOR_ALINK   0
#endif


/*
  HTML error printf function. You can declare your self function like this:
  html_err = &my_printf();
*/
int __html_err_func (const char *str, ...){};
int (*html_err_func)(const char *str, ...) = &__html_err_func;



#ifndef strsize
#define strsize(p,v)    ((v)?((v)-(p)):strlen(p))
#endif
#ifndef max
#define max(x,y)    (((x) > (y)) ?  (x) : (y))
#endif
#ifndef min
#define min(x,y)    (((x) < (y)) ?  (x) : (y))
#endif
#ifndef lmax
#define lmax(x,y)   max((long)(x),(long)(y))
#endif
#ifndef lmin
#define lmin(x,y)   min((long)(x),(long)(y))
#endif
#ifndef _while
#define _while(h) while(h)
#endif


#define __HTML_UNIX_PATH_DECLARATION__

#define HTML_HEXANUM   "#"
#define C_HEXANUM      "0x"

#define BEGIN_S        "<"
#define BEGIN_E        ">"
#define END_S          "</"
#define END_E          ">"

#define html_com           char*

#define  HTML_COMMAND_MAX  1024


#define DOCUMENT_TAGS_S    0
#define HTML_S             0
#define HEAD_S             1
#define BODY_S             2
#define DOCUMENT_TAGS_E    2



#define HEADING_TAGS_S     3
#define TITLE_S            3
#define BASE_S             4
#define META_S             5
#define STYLE_S            6
#define LINK_S             7
#define HEADING_TAGS_E     7



#define BLOCKLEVEL_S       8
#define ADDRESS_S          8
#define BLOCKQUOTE_S       9
#define DIV_S             10
#define H1_S              11
#define H2_S              12
#define H3_S              13
#define H4_S              14
#define H5_S              15
#define H6_S              16
#define P_S               17
#define PRE_S             18
#define XMP_S             19
#define BLOCKLEVEL_E      19
#define ID_BLOCKLEVEL(id) ((id)>=BLOCKLEVEL_S && (id)<=BLOCKLEVEL_E)
#define ID_HEADINGS(id) ((id)>=H1_S && (id)<=H6_S)

#define LISTS_S           20
#define DD_S              20
#define DIR_S             21
#define DL_S              22
#define DT_S              23
#define LI_S              24
#define MENU_S            25
#define OL_S              26
#define UL_S              27
#define LISTS_E           27
#define ID_LISTS(id) ((id)>=LISTS_S && (id)<=LISTS_E)

#define TEXTCHAR_S        28
#define B_S               28
#define BASEFONT_S        29
#define BIG_S             30
#define BLINK_S           31
#define CITE_S            32
#define CODE_S            33
#define EM_S              34
#define FONT_S            35
#define I_S               36
#define KBD_S             37
#define PLAINTEXT_S       38
#define S_S               39
#define SMALL_S           40
#define STRIKE_S          41
#define STRONG_S          42
#define SUB_S             43
#define SUP_S             44
#define TT_S              45
#define U_S               46
#define VAR_S             47
#define TEXTCHAR_E        47
#define ID_TEXTCHAR(id) ((id)>=TEXTCHAR_S && (id)<=TEXTCHAR_E)

#define ANCHOR_S          48
#define A_S               48
#define ANCHOR_E          48
#define ID_ANCHOR(id)    ((id)>=ANCHOR_S && (id)<=ANCHOR_E)

#define IMAGES_S          49
#define IMG_S             49
#define AREA_S            50
#define MAP_S             51
#define IMAGES_E          51
#define ID_BLOCK(id)      ((id)>=IMAGES_S && (id)<=IMAGES_E)

#define TABLES_S          52
#define TABLE_S           52
#define CAPTION_S         53
#define TR_S              54
#define TD_S              55
#define TH_S              56
#define TABLES_E          56

#define FORMS_S           57
#define FORM_S            57
#define INPUT_S           58
#define SELECT_S          59
#define OPTION_S          60
#define TEXTAREA_S        61
#define KEYGEN_S          62
#define ISINDEX_S         63
#define FORMS_E           63

#define FRAMES_S          64
#define FRAME_S           64
#define FRAMESET_S        65
#define NOFRAMES_S        66
#define FRAMES_E          66

#define POSITIONING_S     67
#define LAYER_S           67
#define ILAYER_S          68
#define NOLAYER_S         69
#define POSITIONING_E     69

#define SCRIPTS_S         70
#define SCRIPT_S          70
#define NOSCRIPT_S        71
#define SERVER_S          72
#define SCRIPTS_E         72

#define APPLETS_S         73
#define APPLET_S          73
#define PARAM_S           74
#define EMBED_S           75
#define NOEMBED_S         76
#define OBJECT_S          77
#define APPLETS_E         77

#define FORMATTING_S      78
#define BR_S              78
#define CENTER_S          79
#define HR_S              80
#define MULTICOL_S        81
#define NOBR_S            82
#define SPACER_S          83
#define SPAN_S            84
#define WBR_S             85
#define FORMATTING_E      85

#define DISABLE_S         86
#define TEXT_S            87

#define ID_SETBYNOTEXT(id) (ID_TEXTCHAR(id) || id==CENTER_S)

/*
  begin of HTML_OBS_DECLARATION
*/

#ifndef HTML_OBS_DECLARATION
#define HTML_OBS_DECLARATION

#define HTML_DEF_FONT                      void
#define HTML_DEF_BITMAP                    void
#define HTML_DEF_COLOR                     int

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


html_com HTMLCOMMANDS[HTML_COMMAND_MAX] = {"HTML", "HEAD", "BODY", "TITLE",
            "BASE", "META", "STYLE", "LINK", "ADDRESS", "BLOCKQUOTE", "DIV",
            "H1",
            "H2", "H3", "H4", "H5", "H6", "P", "PRE", "XMP", "DD", "DIR", "DL",
            "DT", "LI", "MENU", "OL", "UL", "B", "BASEFONT", "BIG", "BLINK",
            "CITE", "CODE", "EM", "FONT", "I", "KBD", "PLAINTEXT", "S",
            "SMALL", "STRIKE", "STRONG", "SUB", "SUP", "TT", "U", "VAR", "A",
            "IMG", "AREA", "MAP", "TABLE", "CAPTION", "TR", "TD", "TH", "FORM",
            "INPUT", "SELECT", "OPTION", "TEXTAREA", "KEYGEN", "ISINDEX",
            "FRAME", "FRAMESET", "NOFRAMES", "LAYER", "ILAYER", "NOLAYER",
            "SCRIPT", "NOSCRIPT", "SERVER", "APPLET", "PARAM", "EMBED",
            "NOEMBED", "OBJECT", "BR", "CENTER", "HR", "MULTICOL", "NOBR",
            "SPACER", "SPAN", "WBR", "!--",
            NULL};

#define  HTMLid(n) ((n<HTML_COMMAND_MAX&&n>=0)?HTMLCOMMANDS[n]:NULL)



#define HTML_FONT_MAX         56

#define HTML_FONTS             7

#define HTML_FONT_1            0
#define HTML_FONT_2            1
#define HTML_FONT_3            2
#define HTML_FONT_4            3
#define HTML_FONT_5            4
#define HTML_FONT_6            5
#define HTML_FONT_7            6

#define HTML_FONT_1_BOLD       7
#define HTML_FONT_2_BOLD       8
#define HTML_FONT_3_BOLD       9
#define HTML_FONT_4_BOLD      10
#define HTML_FONT_5_BOLD      11
#define HTML_FONT_6_BOLD      12
#define HTML_FONT_7_BOLD      13

#define HTML_FONT_1_ITALIC    14
#define HTML_FONT_2_ITALIC    15
#define HTML_FONT_3_ITALIC    16
#define HTML_FONT_4_ITALIC    17
#define HTML_FONT_5_ITALIC    18
#define HTML_FONT_6_ITALIC    19
#define HTML_FONT_7_ITALIC    20

#define HTML_FONT_1_BI        21
#define HTML_FONT_2_BI        22
#define HTML_FONT_3_BI        23
#define HTML_FONT_4_BI        24
#define HTML_FONT_5_BI        25
#define HTML_FONT_6_BI        26
#define HTML_FONT_7_BI        27

#define HTML_FONT_1_FIXED     28
#define HTML_FONT_2_FIXED     29
#define HTML_FONT_3_FIXED     30
#define HTML_FONT_4_FIXED     31
#define HTML_FONT_5_FIXED     32
#define HTML_FONT_6_FIXED     33
#define HTML_FONT_7_FIXED     34

#define HTML_FONT_1_FIXEDBOLD    35
#define HTML_FONT_2_FIXEDBOLD    36
#define HTML_FONT_3_FIXEDBOLD    37
#define HTML_FONT_4_FIXEDBOLD    38
#define HTML_FONT_5_FIXEDBOLD    39
#define HTML_FONT_6_FIXEDBOLD    40
#define HTML_FONT_7_FIXEDBOLD    41

#define HTML_FONT_1_FIXEDITALIC    42
#define HTML_FONT_2_FIXEDITALIC    43
#define HTML_FONT_3_FIXEDITALIC    44
#define HTML_FONT_4_FIXEDITALIC    45
#define HTML_FONT_5_FIXEDITALIC    46
#define HTML_FONT_6_FIXEDITALIC    47
#define HTML_FONT_7_FIXEDITALIC    48

#define HTML_FONT_1_FIXEDBI   48
#define HTML_FONT_2_FIXEDBI   50
#define HTML_FONT_3_FIXEDBI   51
#define HTML_FONT_4_FIXEDBI   52
#define HTML_FONT_5_FIXEDBI   53
#define HTML_FONT_6_FIXEDBI   54
#define HTML_FONT_7_FIXEDBI   55



HTML_DEF_FONT *HTMLFONTS[HTML_FONT_MAX] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            }; /* global variable */

#define  HTMLFONTid(n) ((n<HTML_FONT_MAX&&n>=0)?HTMLFONTS[n]:NULL)



void  html_load_fonts ( void );
void  html_unload_fonts ( void );



#define HTML_FONT_NORMAL            0
#define HTML_FONT_BOLD              1
#define HTML_FONT_ITALIC            2
#define HTML_FONT_BI                3
#define HTML_FONT_FIXED             4
#define HTML_FONT_FIXEDBOLD         5
#define HTML_FONT_FIXEDITALIC       6
#define HTML_FONT_FIXEDBI           7


int   ret_type_of_font ( HTML_DEF_FONT *fnt );
int   change_type_of_font ( int fnt, char type );
int   ret_weight_of_font ( int fnt );
int   change_weight_of_font ( int fnt, int weight );
int   ret_maintype_of_font ( int fnt );

#define HTML_FONT_TYPE(f)               ret_type_of_font(f)
#define HTML_FONT_MAINTYPE(nf)          ret_maintype_of_font(nf)
#define HTML_FONT_CHANGE_TYPE(nf,t)     change_type_of_font(nf, t)
#define HTML_FONT_WEIGHT(nf)            ret_weight_of_font(nf)
#define HTML_FONT_CHANGE_WEIGHT(nf,w)   change_weight_of_font(nf,w)


int   ret_align_id_from_str ( char *str );
#define  HTMLALIGNid(str) ret_align_id_from_str(str)

long  ret_size_id_from_str ( char *str, int *isdelta );
#define  HTMLSIZEid(str) ret_size_id_from_str(str,NULL)
#define  HTMLSIZEIDid(str,id) ret_size_id_from_str(str,id)




html_rect HTML_RECT_EMPTY = {-1,-1,-1,-1};


#define HTML_FLOAT_INS(f,t,p,m)  html_float_ins(engine, f, t, p, m)
#define HTML_FLOAT_TRANS(r)      html_float_trans(engine, r)



char      *html_path2file ( char *path, char *id );

void      *html_obj_get_downloads ( html_dwns *main, char *id );
html_dwns *html_obj_ins_downloads ( html_dwns **main, char *id, int idnum, void *ptr );
html_dwns *html_obj_del_downloads ( html_dwns **main );
void      *html_obj_load_downloads ( html_dwns *main, char *path, char *id, int idnum );


#define HTML_DOWNLOADS_GET(id)      html_obj_get_downloads(ENGdownloads, id)
#define HTML_DOWNLOADS_INS(id,n,p)  html_obj_ins_downloads(&ENGdownloads, id, n, p)
#define HTML_DOWNLOADS_DEL()        html_obj_del_downloads(&ENGdownloads)
#define HTML_DOWNLOADS_LOAD(id,n)   html_obj_load_downloads(ENGdownloads, HTML_PATH_TO_FILE, id, n)


/*
  html lines structure.
  it use for save information about height of lines
*/
typedef struct html_lns {
  long               hline;     /* number of lines */
  int                hheight;   /* height of line */
  struct html_lns   *hnext;     /* next line */
  char               hres[8];   /* reserved for future purpose */
} html_lns;


html_lns *html_lines_get ( html_lns *main, long line );
int       html_lines_get_height ( html_lns *main, long line );
html_lns *html_lines_set ( html_lns **main, long line, int height );
html_lns *html_lines_set_max ( html_lns **main, long line, int height );
html_lns *html_lines_del ( html_lns **main );



#define html_bankspc_char_size  4
#define html_space_char_size    4

static char html_bankspc_char[html_bankspc_char_size+1] = {' ', '>', ',', 10, '\0'};

static char html_space_char[html_space_char_size+1] = {' ', 10, 9, 13, '\0'};

#define HAS_SELFALIGN(o)  (o && o->hobjtype&&(o->hobjtype->halign&HTML_ALIGN_SELFALIGN))

#define    HTML_ENGINE           html_engine
#define    HTML_DEF_ENGINE       HTML_ENGINE *engine

#define    ENGid                 ((*engine).hid)
#define    ENGalcolor            ((*engine).halcolor)
#define    ENGlicolor            ((*engine).hlicolor)
#define    ENGvlcolor            ((*engine).hvlcolor)
#define    ENGbcolor             ((*engine).hbcolor)
#define    ENGfcolor             ((*engine).hfcolor)
#define    ENGpattern            ((*engine).hpattern)
#define    ENGidrealnow          ((*engine).hidrealnow)
#define    ENGline               ((*engine).hline)
#define    ENGlinealign          ((*engine).hlinealign)
#define    ENGlastwasspace       ((*engine).hlastwasspace)
#define    ENGstyle              ((*engine).hstyle)
#define    ENGview               ((*engine).hview)
#define    ENGmain               ((*engine).hobjectsout)
#define    ENGlast               ((*engine).hobjectlast)
#define    ENGlastview           (ENGlast?ENGlast->hview:NULL)
#define    ENGrealid             (ENGlast?ENGlast->hidin:ENGid)
#define    ENGrect               ((*engine).hrect)
#define    ENGframe              ((*engine).hframe)
#define    ENGlisttype           ((*engine).hlisttype)
#define    ENGnumber             ((*engine).hnumber)
#define    ENGtype               ((*engine).htype)
#define    ENGstandard           ((*engine).hstandard)
#define    ENGendenter           ((*engine).hendenter)
#define    ENGlockenter          ((*engine).hlockenter)
#define    ENGfloats             ((*engine).hfloats)
#define    ENGlinespace          ((*engine).hlinespace)
#define    ENGlastlinespace      ((*engine).hlastlinespace)
#define    ENGtab_blockquote     ((*engine).htab_blockquote)
#define    ENGtab_list           ((*engine).htab_list)
#define    ENGlinemove           ((*engine).hlinemove)
#define    ENGnowrap             ((*engine).hnowrap)
#define    ENGtablestyle         ((*engine).htablestyle)
#define    ENGtablesafe          ((*engine).htablesafe)
#define    ENGfirstengine        ((*engine).hfirstengine)
#define    ENGoriginalpos        ((*engine).horiginalpos)
#define    ENGminx               ((*engine).hminx)
#define    ENGtitle              ((*engine).htitle)
#define    ENGsafetable          (ENGfirstengine->hsafetable)
#define    ENGsafetd             (ENGfirstengine->hsafetd)
#define    ENGpath               (ENGfirstengine->hpath)
#define    ENGdownloads          (ENGfirstengine->hdownloads)


#define    ENGlasttype           (ENGlast?ENGlast->hobjtype:NULL)
#define    ENGrealalign          (ENGlasttype?ENGlast->hobjtype->halign:HTML_ALIGN_NOALIGN)
#define    ENGtext               (ENGlasttype?ENGlast->hobjtype->htext:NULL)

#define    ENGenter_lock         ENGlockenter=1
#define    ENGenter_unlock       ENGlockenter=0

#define    ENGSETtypetext(p)     if ( ENGlasttype ) ENGlast->hobjtype->htext=(p)

#define    HTML_SET_MINX(s)      ENGminx = max(ENGminx, s)


#define    TYPframe              (ENGtype.hframe)
#define    TYPid                 (ENGtype.hid)
#define    TYPfont               (ENGtype.hfont)
#define    TYPalign              (ENGtype.halign)
#define    TYPstyle              (ENGtype.hstyle)
#define    TYPfcolor             (ENGtype.hfcolor)
#define    TYPbcolor             (ENGtype.hbcolor)
#define    TYPacolor             (ENGtype.hacolor)
#define    TYPignorespaces       (ENGtype.hignorespaces)
#define    TYPreference          (ENGtype.hreference)
#define    TYPanchorname         (ENGtype.hanchorname)
#define    TYPlisttype           (ENGtype.hlisttype)
#define    TYPnumber             (ENGtype.hnumber)
#define    TYPtdnumber           (ENGtype.htdnumber)
#define    TYPtrnumber           (ENGtype.htrnumber)
#define    TYPminwidth           (ENGtype.hminwidth)
#define    TYPmaxwidth           (ENGtype.hmaxwidth)
#define    TYPcolmin             (ENGtype.hcolmin)
#define    TYPcolmax             (ENGtype.hcolmax)
#define    TYPcolwidth           (ENGtype.hcolwidth)
#define    TYPtdframe            (ENGtype.htdframe)
#define    TYPtdstyle            (ENGtype.htdstyle)
#define    TYPtdcols             (ENGtype.htdcols)
#define    TYPnowrap             (ENGtype.hnowrap)
#define    TYPtdcellspacing      (ENGtype.htdcellpadding)
#define    TYPtdcellpadding      (ENGtype.htdcellspacing)
#define    TYPtdborder           (ENGtype.htdborder)

#define    STDframe              (ENGstandard.hframe)
#define    STDfont               (ENGstandard.hfont)
#define    STDalign              (ENGstandard.halign)
#define    STDfcolor             (ENGstandard.hfcolor)
#define    STDbcolor             (ENGstandard.hbcolor)
#define    STDacolor             (ENGstandard.hacolor)


#define    HTML_STYLE            (ENGstyle)

#define ENG_SET_ALIGN(o,align)            html_objects_set_align(o, align)
#define ENG_OBJECT_INS(type,in,from,size) html_objects_ins(engine, &ENGmain, type, in, from, size)
#define ENG_OBJECT_END(o,b)               ENGlast = html_objects_end(ENGmain, o, b)

#define END_OBJECT_RETURN_PREV  1 /* set end to 1, prev */
#define END_OBJECT              0 /* set end to 1, forward */
#define END_OBJECT2             2 /* set end to 2, forward */

#define ENG_DOWNLOADS_LOAD(p,i,n)         html_obj_load_downloads(ENGdownloads, p, i, n)

#define hsnch2(c)                         ((c) == 0x20)


void    html_switch ( HTML_DEF_ENGINE );

#ifndef HTML_FUNC_SWITCH
#define HTML_FUNC_SWITCH(engine)          html_switch(engine)
#endif


#define  HTML_FONT_GETWORDSINWIDTH(f,str,sizex,a)  html_font_get_words_in_width(f, str, sizex, a)


#define  HTML_COLOR_RED(c)                html_color_get_red_from(c)
#define  HTML_COLOR_GREEN(c)              html_color_get_green_from(c)
#define  HTML_COLOR_BLUE(c)               html_color_get_blue_from(c)
#define  HTML_COLOR_RGB_SET(c)            HTML_FUNC_COLOR_MAKERGB(HTML_COLOR_RED(c), HTML_COLOR_GREEN(c), HTML_COLOR_BLUE(c))



int   SIZE_OF ( int id ) {

  if ( ID_BLOCKLEVEL(id) ) return sizeof(html_blocklevel);
  else
  if ( ID_LISTS(id) ) return sizeof(html_lists);
  else
    return sizeof(html_standard);

};


/*
  load starndard html_fonts. Use before all html classes call
*/
#ifndef HTML_LOAD_FONT_FUNC

void  html_load_fonts ( void ) {

  char* curdir = _malloc(HTML_PATH_MAX);

  curdir = getcwd(curdir, HTML_PATH_MAX);

  if ( !HTML_FONT_DIRECTORY_NAME || !chdir(HTML_FONT_DIRECTORY_NAME) ) { /* ok */

    int i = 0;

    HTMLFONTid(HTML_FONT_1) = HTML_FUNC_FONT_LOAD(HTML_FONT_REGULAR_NAME,  8, 8);
    HTMLFONTid(HTML_FONT_2) = HTML_FUNC_FONT_LOAD(HTML_FONT_REGULAR_NAME, 10, 10);
    HTMLFONTid(HTML_FONT_3) = HTML_FUNC_FONT_LOAD(HTML_FONT_REGULAR_NAME, 12, 12);
    HTMLFONTid(HTML_FONT_4) = HTML_FUNC_FONT_LOAD(HTML_FONT_REGULAR_NAME, 14, 14);
    HTMLFONTid(HTML_FONT_5) = HTML_FUNC_FONT_LOAD(HTML_FONT_REGULAR_NAME, 18, 18);
    HTMLFONTid(HTML_FONT_6) = HTML_FUNC_FONT_LOAD(HTML_FONT_REGULAR_NAME, 24, 24);
    HTMLFONTid(HTML_FONT_7) = HTML_FUNC_FONT_LOAD(HTML_FONT_REGULAR_NAME, 36, 36);

    HTMLFONTid(HTML_FONT_1_ITALIC) = HTML_FUNC_FONT_LOAD(HTML_FONT_ITALIC_NAME,  8, 8);
    HTMLFONTid(HTML_FONT_2_ITALIC) = HTML_FUNC_FONT_LOAD(HTML_FONT_ITALIC_NAME, 10, 10);
    HTMLFONTid(HTML_FONT_3_ITALIC) = HTML_FUNC_FONT_LOAD(HTML_FONT_ITALIC_NAME, 12, 12);
    HTMLFONTid(HTML_FONT_4_ITALIC) = HTML_FUNC_FONT_LOAD(HTML_FONT_ITALIC_NAME, 14, 14);
    HTMLFONTid(HTML_FONT_5_ITALIC) = HTML_FUNC_FONT_LOAD(HTML_FONT_ITALIC_NAME, 18, 18);
    HTMLFONTid(HTML_FONT_6_ITALIC) = HTML_FUNC_FONT_LOAD(HTML_FONT_ITALIC_NAME, 24, 24);
    HTMLFONTid(HTML_FONT_7_ITALIC) = HTML_FUNC_FONT_LOAD(HTML_FONT_ITALIC_NAME, 36, 36);

    HTMLFONTid(HTML_FONT_1_BOLD) = HTML_FUNC_FONT_LOAD(HTML_FONT_BOLD_NAME,  8, 8);
    HTMLFONTid(HTML_FONT_2_BOLD) = HTML_FUNC_FONT_LOAD(HTML_FONT_BOLD_NAME, 10, 10);
    HTMLFONTid(HTML_FONT_3_BOLD) = HTML_FUNC_FONT_LOAD(HTML_FONT_BOLD_NAME, 12, 12);
    HTMLFONTid(HTML_FONT_4_BOLD) = HTML_FUNC_FONT_LOAD(HTML_FONT_BOLD_NAME, 14, 14);
    HTMLFONTid(HTML_FONT_5_BOLD) = HTML_FUNC_FONT_LOAD(HTML_FONT_BOLD_NAME, 18, 18);
    HTMLFONTid(HTML_FONT_6_BOLD) = HTML_FUNC_FONT_LOAD(HTML_FONT_BOLD_NAME, 24, 24);
    HTMLFONTid(HTML_FONT_7_BOLD) = HTML_FUNC_FONT_LOAD(HTML_FONT_BOLD_NAME, 36, 36);

    HTMLFONTid(HTML_FONT_1_BI) = HTML_FUNC_FONT_LOAD(HTML_FONT_BOLDITALIC_NAME,  8, 8);
    HTMLFONTid(HTML_FONT_2_BI) = HTML_FUNC_FONT_LOAD(HTML_FONT_BOLDITALIC_NAME, 10, 10);
    HTMLFONTid(HTML_FONT_3_BI) = HTML_FUNC_FONT_LOAD(HTML_FONT_BOLDITALIC_NAME, 12, 12);
    HTMLFONTid(HTML_FONT_4_BI) = HTML_FUNC_FONT_LOAD(HTML_FONT_BOLDITALIC_NAME, 14, 14);
    HTMLFONTid(HTML_FONT_5_BI) = HTML_FUNC_FONT_LOAD(HTML_FONT_BOLDITALIC_NAME, 18, 18);
    HTMLFONTid(HTML_FONT_6_BI) = HTML_FUNC_FONT_LOAD(HTML_FONT_BOLDITALIC_NAME, 24, 24);
    HTMLFONTid(HTML_FONT_7_BI) = HTML_FUNC_FONT_LOAD(HTML_FONT_BOLDITALIC_NAME, 36, 36);

    HTMLFONTid(HTML_FONT_1_FIXED) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXED_NAME,  8, 8);
    HTMLFONTid(HTML_FONT_2_FIXED) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXED_NAME, 10, 10);
    HTMLFONTid(HTML_FONT_3_FIXED) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXED_NAME, 12, 12);
    HTMLFONTid(HTML_FONT_4_FIXED) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXED_NAME, 14, 14);
    HTMLFONTid(HTML_FONT_5_FIXED) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXED_NAME, 18, 18);
    HTMLFONTid(HTML_FONT_6_FIXED) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXED_NAME, 24, 24);
    HTMLFONTid(HTML_FONT_7_FIXED) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXED_NAME, 36, 36);

    HTMLFONTid(HTML_FONT_1_FIXEDBOLD) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDBOLD_NAME,  8, 8);
    HTMLFONTid(HTML_FONT_2_FIXEDBOLD) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDBOLD_NAME, 10, 10);
    HTMLFONTid(HTML_FONT_3_FIXEDBOLD) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDBOLD_NAME, 12, 12);
    HTMLFONTid(HTML_FONT_4_FIXEDBOLD) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDBOLD_NAME, 14, 14);
    HTMLFONTid(HTML_FONT_5_FIXEDBOLD) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDBOLD_NAME, 18, 18);
    HTMLFONTid(HTML_FONT_6_FIXEDBOLD) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDBOLD_NAME, 24, 24);
    HTMLFONTid(HTML_FONT_7_FIXEDBOLD) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDBOLD_NAME, 36, 36);

    HTMLFONTid(HTML_FONT_1_FIXEDITALIC) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDITALIC_NAME,  8, 8);
    HTMLFONTid(HTML_FONT_2_FIXEDITALIC) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDITALIC_NAME, 10, 10);
    HTMLFONTid(HTML_FONT_3_FIXEDITALIC) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDITALIC_NAME, 12, 12);
    HTMLFONTid(HTML_FONT_4_FIXEDITALIC) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDITALIC_NAME, 14, 14);
    HTMLFONTid(HTML_FONT_5_FIXEDITALIC) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDITALIC_NAME, 18, 18);
    HTMLFONTid(HTML_FONT_6_FIXEDITALIC) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDITALIC_NAME, 24, 24);
    HTMLFONTid(HTML_FONT_7_FIXEDITALIC) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDITALIC_NAME, 36, 36);

    HTMLFONTid(HTML_FONT_1_FIXEDBI) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDBOLDITALIC_NAME,  8, 8);
    HTMLFONTid(HTML_FONT_2_FIXEDBI) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDBOLDITALIC_NAME, 10, 10);
    HTMLFONTid(HTML_FONT_3_FIXEDBI) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDBOLDITALIC_NAME, 12, 12);
    HTMLFONTid(HTML_FONT_4_FIXEDBI) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDBOLDITALIC_NAME, 14, 14);
    HTMLFONTid(HTML_FONT_5_FIXEDBI) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDBOLDITALIC_NAME, 18, 18);
    HTMLFONTid(HTML_FONT_6_FIXEDBI) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDBOLDITALIC_NAME, 24, 24);
    HTMLFONTid(HTML_FONT_7_FIXEDBI) = HTML_FUNC_FONT_LOAD(HTML_FONT_FIXEDBOLDITALIC_NAME, 36, 36);

    if ( HTML_FONT_DIRECTORY_NAME )

      chdir(curdir);

  } else html_err_func("HTML fonts directory not found.");

  _free(curdir);
};

#endif



/*
  destroy starndard html_fonts. Use after all html classes end-call
*/
void  html_unload_fonts ( void ) {
#ifdef HTML_UNLOAD_FONT_FUNC
  int n = 0;
  _while ( n < HTML_FONT_MAX ) {
    HTML_FUNC_FONT_UNLOAD(HTMLFONTid(n));
    HTMLFONTS[n] = NULL;
    n++;
  };
#endif
};



/*
  return origins in html_rect structure
*/
static html_rect html_rect_assign ( long left, long top, long right, long bottom )
{
  html_rect r;
  r.hleft   = left;
  r.htop    = top;
  r.hright  = right;
  r.hbottom = bottom;
  return r;
};



/*
  return true (1) if src is equal to dst rect, otherwise it return false(0)
*/
static int html_rect_equals ( html_rect src, html_rect dst )
{
  if ( src.hleft == dst.hleft && src.hright == dst.hright &&
       src.htop == dst.htop && src.hbottom == dst.hbottom )
       return 1;
  return 0;
};



/*
  return origins in html_rect structure
*/
static html_rect html_rect_move ( html_rect r, long dx, long dy )
{
  html_rect d;
  d.hleft   = r.hleft+dx;
  d.htop    = r.htop+dy;
  d.hright  = r.hright+dx;
  d.hbottom = r.hbottom+dy;
  return d;
};



/*
  return sizex
*/
static long html_rect_sizex ( html_rect r )
{
  return (r.hright-r.hleft);
};


/*
  return sizey
*/
static long html_rect_sizey ( html_rect r )
{
  return (r.hbottom-r.htop);
};



/*
  from "c" return red color
  F.e. "c" == 0xFF0015
  Red color is FF
  Green color is 00
  Blue color is 15
*/
unsigned char  html_color_get_red_from ( unsigned long c )
{
  unsigned long red = (c>>16);
  return ((unsigned char)red);
};



unsigned char  html_color_get_green_from ( unsigned long c )
{
  unsigned long green = ((c<<16)>>24);
  return ((unsigned char)green);
};



unsigned char  html_color_get_blue_from ( unsigned long c )
{
  unsigned long blue = ((c<<24)>>24);
  return ((unsigned char)blue);
};



/*
  return main type of font fnt. F.e. HTML_ITALIC
*/
int   ret_maintype_of_font ( int fnt ) {
  return (fnt/max(1, HTML_FONTS));
};



/*
  return changed weight of font fnt. F.e. font is HTML_FONT_H3_ITALIC
  and weight is 6, it return HTML_FONT_H1_ITALIC
  else return -1;
*/
int   change_weight_of_font ( int fnt, int weight ) {

  int t;

  fnt = max(0, min(HTML_FONT_MAX, fnt)); /* set font to range of fonts */

  t = (ret_maintype_of_font(fnt)*HTML_FONTS); /* go to start of type */

  if ( weight-1 < HTML_FONTS )
    return t+max(0, weight-1);
  else return fnt;  /* else return max weight font */

  return -1; /* not possible */
};



/*
  return weight of font fnt. F.e. font is HTML_FONT_H3_ITALIC
  returned value will be 3
*/
int   ret_weight_of_font ( int fnt ) {

  return ((fnt%HTML_FONTS)+1);

};

/*
  return type of font in id, we want to get from fnt. Type can be one of
  FONT_NORMAL, FONT_ITALIC, FONT_BOLD, FONT_BI.
  F.e. : fnt is HTML_FONT_H1 and type is FONT_BOLD we now get
  HTML_FONT_H1_BOLD...
*/
int  change_type_of_font ( int fnt, char type ) {

  int main;
  int w;

  fnt = max(0, min(HTML_FONT_MAX, fnt)); /* set font to range of fonts */

  main = ret_maintype_of_font(fnt);
  w    = ret_weight_of_font(fnt)-1;

  switch ( type ) {

    case HTML_FONT_BOLD : {

      if ( main == HTML_FONT_FIXED ) return HTML_FONT_1_FIXEDBOLD+w;
      if ( main == HTML_FONT_FIXEDBOLD ) return HTML_FONT_1_FIXEDBOLD+w;
      if ( main == HTML_FONT_FIXEDBI ) return HTML_FONT_1_FIXEDBI+w;
      if ( main == HTML_FONT_FIXEDITALIC ) return HTML_FONT_1_FIXEDBI+w;

      if ( main == HTML_FONT_ITALIC ) return HTML_FONT_1_BI+w;
      if ( main == HTML_FONT_BI ) return HTML_FONT_1_BI+w;

      return HTML_FONT_1_BOLD+w;

    }; break;

    case HTML_FONT_ITALIC : {

      if ( main == HTML_FONT_FIXED ) return HTML_FONT_1_FIXEDITALIC+w;
      if ( main == HTML_FONT_FIXEDBOLD ) return HTML_FONT_1_FIXEDBI+w;
      if ( main == HTML_FONT_FIXEDBI ) return HTML_FONT_1_FIXEDBI+w;
      if ( main == HTML_FONT_FIXEDITALIC ) return HTML_FONT_1_FIXEDITALIC+w;

      if ( main == HTML_FONT_NORMAL ) return HTML_FONT_1_ITALIC+w;
      if ( main == HTML_FONT_BOLD ) return HTML_FONT_1_BI+w;
      if ( main == HTML_FONT_BI ) return HTML_FONT_1_BI+w;

      return HTML_FONT_1_ITALIC+w;

    }; break;

    case HTML_FONT_FIXEDBOLD : {

      if ( main == HTML_FONT_FIXEDITALIC ) return HTML_FONT_1_FIXEDBI+w;
      if ( main == HTML_FONT_FIXEDBI ) return HTML_FONT_1_FIXEDBI+w;

      if ( main == HTML_FONT_ITALIC ) return HTML_FONT_1_FIXEDBI+w;
      if ( main == HTML_FONT_BI ) return HTML_FONT_1_FIXEDBI+w;

      return HTML_FONT_1_FIXEDBOLD+w;

    }; break;

    case HTML_FONT_FIXEDITALIC : {

      if ( main == HTML_FONT_FIXEDBOLD ) return HTML_FONT_1_FIXEDBI+w;
      if ( main == HTML_FONT_FIXEDBI ) return HTML_FONT_1_FIXEDBI+w;

      if ( main == HTML_FONT_BOLD ) return HTML_FONT_1_FIXEDBI+w;
      if ( main == HTML_FONT_BI ) return HTML_FONT_1_FIXEDBI+w;

      return HTML_FONT_1_FIXEDITALIC+w;

    }; break;

    case HTML_FONT_NORMAL : {

      return HTML_FONT_1+w;

    }; break;

    case HTML_FONT_FIXED : {

      if ( main == HTML_FONT_ITALIC ) return HTML_FONT_1_FIXEDITALIC+w;
      if ( main == HTML_FONT_BOLD ) return HTML_FONT_1_FIXEDBOLD+w;
      if ( main == HTML_FONT_BI ) return HTML_FONT_1_FIXEDBI+w;

      if ( main == HTML_FONT_FIXEDITALIC ) return HTML_FONT_1_FIXEDITALIC+w;
      if ( main == HTML_FONT_FIXEDBOLD ) return HTML_FONT_1_FIXEDBOLD+w;
      if ( main == HTML_FONT_FIXEDBI ) return HTML_FONT_1_FIXEDBI+w;

      return HTML_FONT_1_FIXED+w;

    }; break;


  };

  return fnt; /* it's not possible */
};



/*
  return alignmet in TX_ALIGN_... format form text.
  text can be "center", "left", "right", "bottom", "top"
  If nothing of format, it return standard 0 = TX_ALIGN_LEFT.
*/
int   ret_align_id_from_str ( char *str )
{
  if ( str ) {
    if ( !stricmp(str, "center") ) return HTML_ALIGN_CENTERX;
    if ( !stricmp(str, "left") ) return HTML_ALIGN_LEFT;
    if ( !stricmp(str, "right") ) return HTML_ALIGN_RIGHT;
    if ( !stricmp(str, "bottom") ) return HTML_ALIGN_BOTTOM;
    if ( !stricmp(str, "top") ) return HTML_ALIGN_TOP;
    if ( !stricmp(str, "middle") ) return HTML_ALIGN_MIDDLE;
  };
  return HTML_ALIGN_NOALIGN;
};


static int is_hexa_num ( char *str )
{
  if ( !strncmp(str, HTML_HEXANUM, strlen(HTML_HEXANUM)) ) return 1;

  _while ( *str ) {

    char tl = tolower(*str);

    if ( tl >= 'a' && tl <= 'f' ) return 2;

    str++;
  };

  return 0;

};


/*
  return size from string to integer
  isdelta indicates if string has +/- before number
*/
long   ret_size_id_from_str ( char *str, int *isdelta )
{
  #define whattype() do {                             \
      if ( (o = strchr(str, '-')) ) isminus = 1;      \
      else o = strchr(str, '+');                      \
      if ( o ) str = o+1;                             \
  } while (0)

  long  l = 0;
  int   isminus = 0;
  char *o = NULL;
  if ( str ) {
    int ht = is_hexa_num(str);
    int sz = (ht==1)?strlen(HTML_HEXANUM):0;
    if ( ht ) { /* found hex number */
      int size = strlen(str)+strlen(C_HEXANUM)-sz+1;
      char *v = (char *)_malloc(size);
      if ( v ) {
        whattype();
        bzero(v, size);
        strcat(v, C_HEXANUM);
        strcat(v, str+sz);
        l = strtol(v, NULL, 0);
        _free(v);
      };
    } else {
      whattype();
      l = strtol(str, NULL, 0);
    };
    if ( isdelta )
      if ( o ) *isdelta = 1; /* found +/- character */
      else *isdelta = 0;
    return (isminus?(-l):(l));
  };
  if ( isdelta ) *isdelta = 0;
  return 0;
};


/*

  return type of input in integer

*/
int  html_get_input_type ( char *type )
{

  if ( !type ) return HTML_INPUT_NONE;

      if ( !stricmp(type, "BUTTON") ) /* type is button */

        return HTML_INPUT_BUTTON;

      else
      if ( !stricmp(type, "CHECKBOX") ) /* type is checkbox */

        return HTML_INPUT_CHECKBOX;

      else
      if ( !stricmp(type, "FILE") ) /* type is file */

        return HTML_INPUT_FILE;

      else
      if ( !stricmp(type, "HIDDEN") ) /* type is hidden */

        return HTML_INPUT_HIDDEN;

      else
      if ( !stricmp(type, "IMAGE") ) /* type is image */

        return HTML_INPUT_IMAGE;

      else
      if ( !stricmp(type, "PASSWORD") ) /* type is password */

        return HTML_INPUT_PASSWORD;

      else
      if ( !stricmp(type, "RADIO") ) /* type is radio */

        return HTML_INPUT_RADIO;

      else
      if ( !stricmp(type, "RESET") ) /* type is reset */

        return HTML_INPUT_RESET;

      else
      if ( !stricmp(type, "SUBMIT") ) /* type is submit */

        return HTML_INPUT_SUBMIT;

      else
      if ( !stricmp(type, "TEXT") ) /* type is text */

        return HTML_INPUT_TEXT;

      else
      if ( !stricmp(type, "SUBMIT") ) /* type is submit */

        return HTML_INPUT_SUBMIT;

   return HTML_INPUT_NONE;
};



/*
  test if char is one of html banks chars. HTML banks chars means the chars
  which can be used for dividing words. < > space
*/
static int hsch ( char ch ) {
  long n = 0;
  _while ( n < html_bankspc_char_size ) {
    if ( ch == html_bankspc_char[n] ) return 1;
    n++;
  };
  return 0;
};



/*
  test if char is one of html space chars. HTML space chars means the chars
  which can be used for dividing words in normal text
*/
static int hsnch ( char ch ) {
  long n = html_space_char_size-1;
  _while ( n >= 0 ) {
    if ( ch == html_space_char[n] ) return 1;
    n--;
  };
  return 0;
};



/*
  calculate how many chars in words can be visible in width w and frame allx
*/
int  html_font_get_words_in_width ( HTML_DEF_FONT *f, char *str, int w, int allx ) {
  int words = 0;
  if ( str && w > 0 ) {
    int n = 0;
    int dw = w;
    _while ( w > 0 && *str ) {
      w -= HTML_FUNC_FONT_GETCHARWIDTH(f, *str);
      if ( w > 0 && hsnch2(*str) )
        words = n+1;
      str++;
      n++;
    };
    if ( !(*str) ) return max(1, n);
    if ( !words && allx > dw ) return 0;
    return words;
  };
  return 0;
};



/*

*/
int  html_get_first_word ( char *str ) {
  if ( str ) {
    int n = 0;
    _while ( *str ) {
      if ( hsnch2(*str) ) return n+1;
      str++;
      n++;
    };
    return max(1, n);
  };
  return 0;
};



char*  firstrefnchar ( char *str ) {

  if ( str ) {
    _while ( *str ) {
      if ( !hsnch(*str) ) return str;
      str++;
    };
  };
  return NULL;
};


static char *stristr ( char *s, char *d )
{
  if ( s && d && *d ) {

    _while ( *s ) {
      char *olds = s;
      char *oldd = d;
     _while ( *s && *d && tolower(*s) == tolower(*d) ) {
        s++;
        d++;
      };
      if ( !(*d) ) return olds;
      if ( olds == s ) s++;
      d = oldd;
    };

  };

  return NULL;
};


/*
  duplicate str by max length in num
*/
char *strnewn ( char *str, long num ) {
  if ( str && num > 0 ) {
    char *b = (char *)_malloc(num+1);
    if ( b ) {
      strncpy(b, str, num);
      b[num] = '\0';
    };
    return b;
  };
  return NULL;
};


/*
  return size from string to integer, when str contains percent number,
  it return sizex*percents.
  isdelta indicates if string has +/- before number
*/

long   ret_size_id_from_str_perc ( char *str, int sizex, int *isdelta )
{
  if ( str ) {

    char *is_perc = strchr(str, '%');
    char *t = NULL;
    long  x = 0;
    long  o = 0;

    if ( !is_perc ) return ret_size_id_from_str(str, isdelta);

    t = strnewn(str, strsize(str, is_perc));

    if ( t ) {

      x = ret_size_id_from_str(t, isdelta);

      o = (long)(((double)x/(double)100)*(double)sizex);

    };

    _free(t);

    return o;

  };

  return 0;
};


/*
  <p align="center">Uniforms</p>
  if this is declaration, com will be:  align="center"
  and by "align" in to variable this function return "center"
*/

static char *find_first_not_space ( char *p ) {
  if ( p )
    _while ( *p ) {
      if ( !hsnch(*p) ) return p;
      p++;
    };
  return NULL;
};

static char *find_first_is_bank ( char *p ) {
  if ( p )
    _while ( *p ) {
      if ( hsch(*p) ) return p;
      p++;
    };
  return NULL;
};


static char *strnschstr ( char *s, char *d )
{
  if ( s && d && *d ) {

    char *first = s;
    _while ( *s ) {
      char *oldd = d;
      char *olds = s;

      if ( s == first || !isalpha(*(s-1)) )
      _while ( *s == *d ) {
        s++;
        d++;
      };
      if ( !(*d) &&
         ( !(*s) || !isalpha(*s)) ) return olds;
      d = oldd;
      s++;
    };

  };

  return NULL;
};


static char *html_commander ( char *com, char *to ) {

  if ( com && to ) {
  char *tcom = strlwr(strdup(com));
  char *tto  = strlwr(strdup(to));
  char *p = strnschstr(tcom, tto);
  char *old = p;
  char *h = NULL;
  int   nu = 0;

  if ( p ) {
    p = strchr(p, '=');
    if ( p ) {
      old = p;
      p = strchr(p+1, '"');
      h = find_first_not_space(old+1);
      if ( !p || (h && h < p)) {
        p = h;
        nu = 1;
      };
      if ( p ) {
        char *n = nu?find_first_is_bank(p):strchr(p+1, '"');
        if ( !n ) {
          char *v = strdup(p);
          _free(tcom);
          _free(tto);
          return v;
        } else
        if ( n-p > 0 ) {
          char *v = nu?strnewn(com+(p-tcom), n-p):strnewn(com+((p+1)-tcom), (n-p)-1);
          _free(tcom);
          _free(tto);
          return v;
        };
      };
    };
  };
  _free(tcom);
  _free(tto);
  };
  return NULL;
};


/*
  find next occurence of character.
  if found it, it return position of pointer+1, else return NULL
*/
static char *fnext ( char *str1, char *str2 ) {
  char *p = strstr(str1, str2);
  return ((p)?p+1:NULL);
};




/*
  return commands from text. F.e. <body bgcolor="#FFFFFF">
    - return upper commands => BODY
*/
static int find_html_start ( html_com *c, char *text ) {
  long items = 0; /* how many commands we know */
  long maxsize = 0, t = 0;
  if ( text )
    _while ( items < HTML_COMMAND_MAX && c[items] ) { /* while c[item] is not NULL */
      long size = strlen(c[items]);
      char *i = strupr(strnewn(text, size));
      if ( !strncmp(i, c[items], size) ) /* if strings are same */
        if ( size > maxsize && (!text[size-1] || hsch(text[size])) ) {
          t = items;
          maxsize = size;
        };
      _free(i);
      items++;
    };
  /* if commad exist */
  if ( maxsize ) return t;
  return -1;
};



/* - HTML DOWNLOADED OBJECTS **************************************** */

/*
  find object by "id"-identification string in "main" structure and return
  pointer to obj. If not exist this "id" return NULL;
*/
void*   html_obj_get_downloads ( html_dwns *main, char *id )
{
  if ( id ) { /* if "id" is string */
    _while ( main ) {
      #ifdef __DOWNLOADED_DISCERN_LWUP_CHARS__
      if ( !strcmp(main->hid, id) ) /* discern lower & upper chars */
      #else
      if ( !stricmp(main->hid, id) ) /* same for lower & upper chars */
      #endif
        return main->hptr; /* found id object and return it */

      main = main->hnext;
    };
  };
  return NULL;
};



/*
  make new object by "id" string and "ptr"-pointer to object and insert it
  to main directory of objects. if main directory not exist it return
  new main directory, otherwise it return old main directory

  NOTE: object will be create only if not exist yet !!
*/

html_dwns   *html_obj_ins_downloads ( html_dwns **main, char *id, int idnum, void *ptr )
{
  html_dwns *newd = NULL;

  if ( main && html_obj_get_downloads(*main, id) ) /* test if was defined */
    return *main; /* this "id" is defined yet */

  newd = (html_dwns*)_malloc(sizeof(html_dwns));
  if ( newd ) {
    bzero(newd, sizeof(html_dwns));
    newd->hnext  = NULL;
    newd->hid    = strdup(id);
    newd->hidnum = idnum;
    newd->hptr   = ptr;
    if ( main ) { /* if ptr to main exist */
      if ( *main ) newd->hnext = *main; /* main exist */
      *main = newd;
    };
  };
  if ( !main ) return newd;
  return (*main);
}


#define HTML_BITMAP  1
#define HTML_TEXT    2

void  html_how_delete ( int id, void *p ) {
 if ( p )
   switch ( id ) {
     case HTML_BITMAP : HTML_FUNC_BITMAP_DESTROY((HTML_DEF_BITMAP *)p); break;
     default    : _free(p);
   };
};



/*
  delete all obj in main directory and return NULL
*/
html_dwns    *html_obj_del_downloads ( html_dwns **main )
{
  if ( main ) { /* ptr exist */
    _while ( *main ) {
      html_dwns *old = *main;
      html_how_delete(old->hidnum, old->hptr);
      _free(old->hid); /* free memory for text id */
      (*main) = (*main)->hnext;
      _free(old);
    };
    *main = NULL;
  };
  return NULL;
};



char *html_file2path ( char *path )
{
  if ( path ) {
    int sz = strlen(path);
    if ( path[sz-1] == '/' || path[sz-1] == '\\' ) /* end of path */
      return path;
    else {
      char *last_slash1 = strrchr(path, '/');
      char *last_slash2 = strrchr(path, '\\');
      if ( (last_slash1 && !last_slash2) || last_slash1 > last_slash2 ) /* last_slash1 WON !!! */
        return strnewn(path, (last_slash1-path)+1);
      else /* no last_slash1 was !last but one! and I was the SECOND !!! */
        if ( last_slash2 ) return strnewn(path, (last_slash2-path)+1);
    };
  };
  return NULL;
};



char *html_path2file ( char *path, char *id )
{
  if ( path && id ) {
    char *v = (char *)_malloc(strlen(path)+strlen(id)+1);
    if ( v ) { /* path & string exist */
      *v = '\0'; /* set 0 for concating of 2 strings */
      strcat(v, path);
      strcat(v, id);
      return v;
    };
    return NULL; /* not enough memory to allocate string */
  } else if ( id ) return strdup(id); /* path is null so read only a file if exist */
  return NULL;
};



char *html_filefrompath ( char *path )
{
  if ( path ) {
    int sz = strlen(path);
    if ( path[sz-1] == '/' || path[sz-1] == '\\' ) /* end of path */
      return path;
    else {
      char *last_slash1 = strrchr(path, '/');
      char *last_slash2 = strrchr(path, '\\');
      if ( last_slash1 && *(last_slash1+1) ) /* end char is not / */
        return strdup(last_slash1+1);
      else
        if ( last_slash2 && *(last_slash2+1) ) /* end char is not \ */
          return strdup(last_slash2+1);
    };
  };
  return NULL;
};



/*
  download file or load file from disk and return ptr
*/
void  *html_obj_load_downloads ( html_dwns *main, char *path, char *id, int idnum )
{
  #define ret(x) do {     \
    _free(id);             \
    return x;             \
  } while (0)

  void *p = NULL;

  id = html_path2file(path, id);

  p = html_obj_get_downloads(main, id);

  if ( p ) ret(p);

  switch  ( idnum ) {
    case IMG_S : { /* IMAGE */
      ret(HTML_FUNC_BITMAP_LOAD(id));
    }; break;
  };
};



/* - HTML OBJECTS ******************************************************** */
/*
  insert new object to "main" object directory. If "main" exist it copy
  last definition of object to new object and return new object.
  if "main" directory not exist, it make new object and return it.
*/
html_obs *html_objects_ins ( HTML_DEF_ENGINE, html_obs **main, int type, int in, void *from, int size )
{
  html_obs *newd = NULL;
  html_obs *last = NULL;
  if ( main ) {
    last = ENGlast;
  };


  newd = (html_obs*)_malloc(sizeof(html_obs));
  if ( newd ) {
    memset(newd, 0, sizeof(html_obs));
    if ( size > 0 && from ) {
      newd->hobjtype = (html_standard*)_malloc(size);
      if ( newd->hobjtype ) {
        memcpy(newd->hobjtype, from, size);
        newd->hobjtype->hident = type;
        newd->hobjtype->hsafe = ENGtype;
        newd->hobjtype->hsafe.hid = ENGid;
      };
    };
    ENGid = type;
    if ( last ) last->hnext = newd;
    newd->hnext  = NULL;
    newd->hprev  = last;
    newd->hview  = ENGview;
    newd->hid  = type;
    newd->hidin  = ENGidrealnow = in;
    newd->hline = ENGline;
    if ( main && !last ) *main = newd;
    ENGlast = newd;
  };
  return newd;
};



html_obs *html_objects_set_align ( html_obs *o, int align )
{
  if ( o && o->hobjtype )
    o->hobjtype->halign = align;

  return o;
};


HTML_ENGINE  *html_done_engine ( HTML_DEF_ENGINE );

/*
  delete all html objects structure
*/
html_obs *html_objects_del ( html_obs **main )
{
  if ( main && *main ) {

    html_obs *o = *main;

    _while ( o ) {

      html_obs *v = o->hnext;

      if ( o->hobjtype ) {

        _free(o->hobjtype->htext);
        _free(o->hobjtype->hreference);
        _free(o->hobjtype->hanchorname);

        if ( o->hidin == IMG_S ) {

           if ( ((html_image*)o->hobjtype)->hbitmap )

             HTML_FUNC_BITMAP_DESTROY(((html_image*)o->hobjtype)->hbitmap);

           if ( ((html_image*)o->hobjtype)->hlowbitmap )

             HTML_FUNC_BITMAP_DESTROY(((html_image*)o->hobjtype)->hlowbitmap);

           _free(((html_image*)o->hobjtype)->honabort);
           _free(((html_image*)o->hobjtype)->honerror);
           _free(((html_image*)o->hobjtype)->honload);

        } else

        if ( o->hidin == INPUT_S ) {

           _free(((html_input*)o->hobjtype)->hname);
           _free(((html_input*)o->hobjtype)->hvalue);

        } else
        if ( o->hidin == TABLE_S ) {

           html_done_engine(((html_table*)(o->hobjtype))->hengine);

        } else
        if ( o->hidin == TD_S ) {

           html_done_engine(((html_td*)(o->hobjtype))->hengine);

        };

        _free(o->hobjtype);

      };

      _free(o);

      o = v;

    };

    *main = NULL;

  };

  return NULL;
};


html_obs *html_objects_end ( html_obs *start, html_obs *p, int back )
{
  if ( p ) {
    p->hend = 1;
    if ( back ) {
      html_obs *old = p;
      _while ( start ) {
        if ( start == p ) return old;
        old = start;
        start = start->hnext;
      };
    };
  };
  return p;
};



/*
   safe table functions
*/

void html_safe_table_del ( html_safe_table* p )
{

  _while ( p ) {

    html_safe_table *o = p->next;

    _free(p->hcolmin);
    _free(p->hcolmax);
    _free(p->hcolwidth);

    _free(p);

    p = o;

  };

};



void html_safe_table_ins ( HTML_DEF_ENGINE, long id, long *cmin, long *cmax, long *cwidth )
{

  html_safe_table  *p = ENGsafetable;
  html_safe_table  *n = (html_safe_table *)_malloc(sizeof(html_safe_table));

  if ( n ) {

    n->hcolmin = cmin;
    n->hcolmax = cmax;
    n->hcolwidth = cwidth;
    n->hid = id;
    n->next = p;

    ENGsafetable = n;

  };


};



int  html_safe_table_get ( HTML_DEF_ENGINE, long id, long **cmin, long **cmax, long **cwidth )
{

  html_safe_table  *p = ENGsafetable;

  _while ( p ) {

    if ( p->hid == id ) {

        if ( cmin ) (*cmin) = p->hcolmin;
        if ( cmax ) (*cmax) = p->hcolmax;
        if ( cwidth ) (*cwidth) = p->hcolwidth;

        return 1;
    };

    p = p->next;

  };

  return 0;
};



/*
   safe td functions
*/

void html_safe_td_del ( html_safe_td* p )
{

  _while ( p ) {

    html_safe_td *o = p->next;

    _free(p);

    p = o;


  };

};



void html_safe_td_ins ( HTML_DEF_ENGINE, long id, int min, int max )
{

  html_safe_td  *p = ENGsafetd;
  html_safe_td  *n = (html_safe_td *)_malloc(sizeof(html_safe_td));

  if ( n ) {

    n->hmin = min;
    n->hmax = max;
    n->hid = id;
    n->next = ENGsafetd;

    ENGsafetd = n;

  };


};



int  html_safe_td_get ( HTML_DEF_ENGINE, long id, int *min, int *max )
{

  html_safe_td  *p = ENGsafetd;

  _while ( p ) {

    if ( p->hid == id ) {

        if ( min ) (*min) = p->hmin;
        if ( max ) (*max) = p->hmax;

        return 1;
    };

    p = p->next;

  };

  return 0;
};



/*

  floating structure functions

*/
int html_float_ins ( HTML_DEF_ENGINE, long fromy, long toy, long plusl, long minusr )
{
  html_floating *v = (html_floating *)_malloc(sizeof(html_floating));
  if ( v ) {
    v->hfromy = fromy;
    v->htoy = toy;
    v->hplusleft = plusl;
    v->hminusright = minusr;
    v->hnext = NULL;
    if ( !ENGfloats ) ENGfloats = v;
    else {
      v->hnext = ENGfloats;
      ENGfloats = v;
    };
    return 1;
  };
  return 0;
};


html_floating *html_float_del ( html_floating *f )
{
  html_floating *t = f;

  _while ( t ) {

    html_floating *x = t->hnext;

    _free(t);
    t = x;

  };

  return NULL;
};


void  html_float_get_xorigins ( HTML_DEF_ENGINE, html_rect *r ) {

  html_floating *v = ENGfloats;

  _while ( v ) {
    if ( (*r).htop >= v->hfromy && (*r).htop < v->htoy ) {
      (*r).hleft += v->hplusleft;
      (*r).hright += v->hminusright;
    };
    v = v->hnext;
  };

};


int  html_float_trans ( HTML_DEF_ENGINE, html_rect *r )
{
  html_floating *v = ENGfloats;
  html_floating *old = NULL;
  int            ok  = 0;

  _while ( v ) {
    if ( (*r).htop >= v->hfromy && (*r).htop < v->htoy ) {
      (*r).hleft += v->hplusleft;
      (*r).hright += v->hminusright;
      ok = 1;
    }
    v = v->hnext;
  };

  return ok;
};


int  html_get_float_rectx ( HTML_DEF_ENGINE, html_rect r )
{
  html_float_get_xorigins(engine, &r);
  return html_rect_sizex(r);
};



long  html_get_max_width ( HTML_DEF_ENGINE )
{
  html_obs *o = ENGmain;
  long      linemax = 0;

  _while ( o ) {

    if ( o->hobjtype && o->hline > -1 && o->hview )

      linemax = lmax(linemax, o->hobjtype->hrect.hright);


    o = o->hnext;

  };

  return linemax;
};


#define html_get_max_y_all(eng) html_get_max_y((*eng).hobjectsout)

long  html_get_max_y ( html_obs *from )
{
  html_obs *o = from;
  long      ymax = 0;

  _while ( o ) {

    if ( o->hobjtype && o->hline > -1 && o->hview ) {

       ymax = lmax(ymax, o->hobjtype->hrect.hbottom);


    };

    o = o->hnext;
  };

  return ymax;
};

#endif


