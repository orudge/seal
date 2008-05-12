/****************************************************************/
/*                                                              */
/*                           htmltran.c                         */
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


  #define BODY_START do {                                              \
      BLOCKLEVEL_SETTINGS;                                             \
      ENGenter_lock;                                                   \
      ENGview = 1;                                                     \
      ENGline = 0;                                                     \
      ENG_OBJECT_INS(P_S, BODY_S, &b, sizeof(html_blocklevel));        \
   } while (0)


  #define CHARACTER_SETTINGS                      \
      html_blocklevel chr;                        \
      chr.hfont = TYPfont;                        \
      chr.hfcolor = TYPfcolor;                    \
      chr.hclass.halign = ALIGN_GET();            \
      chr.hclass.hstyle = TYPstyle;               \
      chr.hclass.htext = NULL;                    \
      chr.hclass.hrect = HTML_RECT_EMPTY;         \
      chr.hclass.hframe = TYPframe;         \
      chr.hclass.hreference = _strdup(TYPreference); \
      chr.hclass.hanchorname = _strdup(TYPanchorname); \
      chr.hnumber = TYPnumber;                      \
      chr.hlisttype = TYPlisttype;                  \
      chr.hselected = 0;                            \

  #define BLOCKLEVEL_SETTINGS                     \
      html_blocklevel b;                          \
      ALIGN_CLEARLINE();                          \
      b.hfont = TYPfont;                          \
      b.hfcolor = TYPfcolor;                      \
      b.hclass.halign = ALIGN_GET();              \
      b.hclass.hstyle = TYPstyle;                 \
      b.hclass.htext = NULL;                      \
      b.hclass.hrect = HTML_RECT_EMPTY;           \
      b.hclass.hframe = TYPframe;         \
      b.hclass.hreference = _strdup(TYPreference); \
      b.hclass.hanchorname = _strdup(TYPanchorname); \
      b.hnumber = TYPnumber;                      \
      b.hlisttype = TYPlisttype;                  \
      b.hselected = 0;                            \

  #define LIST_SETTINGS       BLOCKLEVEL_SETTINGS


HTML_ENGINE *html_call_sub_engine ( HTML_DEF_ENGINE, long sizex, char *text,
                                    long ptr, html_type type, int align,
                                    HTML_DEF_BITMAP *pattern )
{

            HTML_ENGINE *engnew = html_init_engine_ex(ENGfirstengine, (char*)ptr,
                                                      0, 0, sizex, 1);
            long x = 0;

            (*engnew).hpattern = pattern;
            (*engnew).htablestyle = ENGtablestyle;
            (*engnew).hnowrap = 0;
            (*engnew).hstyle = ENGstyle;
            (*engnew).hbcolor = type.hbcolor;
            (*engnew).hfcolor = type.hfcolor;
            (*engnew).halcolor = ENGalcolor;
            (*engnew).hlicolor = ENGlicolor;
            (*engnew).hvlcolor = ENGvlcolor;

            (*engnew).htype = type;

            engine = engnew;

            ALIGN_SET(align);

            ENGrect = TYPtdframe = ENGframe = TYPframe = STDframe = html_rect_assign(0, 0, sizex, 1);

            BODY_START;

            ENGlinespace = HTML_SPECIAL_SPACE;

            enter_set();

            html_file_translate(engine, text, NULL);

            /* initialize the second engine */

            return engine;
};


long  html_wheretr_inrowspan ( HTML_DEF_ENGINE, long row )
{

   return html_get_max_y_all(engine);
};


void  html_insert_int_max ( int where, long **dest, long what, int colspan )
{
  long i = 0;
  long *v = *dest;

  if ( what < 0 || colspan <= 0 ) return;

  if ( v )
    _while ( *v >= 0 ) {
      if ( where == i ) {
        if ( *v < what ) {
           *v = max(what/colspan, *v);
           what -= *v;
           colspan--;
           if ( colspan > 0 ) where++; else return;
           return;
        } else return;
      };
      i++;
      v++;
    };

  if ( !(*dest) )
    (*dest) = (long *)_malloc((i+colspan+1)*sizeof(long));
  else
    (*dest) = (long *)_realloc((*dest), (i+colspan+1)*sizeof(long));

  if ( (*dest) ) {

    int t;

    for ( t = i; t < i+colspan; t++ )
       (*dest)[t] = what/colspan;

    (*dest)[t] = -1;

  };
};


long*  html_alloc_widths ( long *arr )
{
  if ( arr ) {

    long x = 0;
    long *v = arr;
    long *col = NULL;

    _while ( *v >= 0 ) {

      x++;
      v++;

    };

    col = (long*)_malloc((x+1)*sizeof(long));

    if ( col ) {

      int x = 0;

      _while ( *arr >= 0 ) {

        col[x] = 0;
        x++;
        arr++;

      };

      col[x] = -1;

    };

    return col;
  };

  return NULL;
};


long*  html_alloc_widths_cpy ( long *arr )
{
  if ( arr ) {

    long x = 0;
    long *v = arr;
    long *col = NULL;

    _while ( *v >= 0 ) {

      x++;
      v++;

    };

    col = (long*)_malloc((x+1)*sizeof(long));

    if ( col ) {

      int x = 0;

      _while ( *arr >= 0 ) {

        col[x] = *arr;
        x++;
        arr++;

      };

      col[x] = -1;

    };

    return col;
  };

  return NULL;
};


int   html_get_all ( long *arr )
{
  int all = 0;
  if ( arr )
    _while ( *arr >= 0 ) {
      all+=*arr;
      arr++;
    };
  return all;
};


int   html_get_num_cols ( long *arr )
{
  int all = 0;
  if ( arr )
    _while ( *arr >= 0 ) {
      all++;
      arr++;
    };
  return all;
};


int   html_get_col ( long *arr, int col, int colspan )
{
  int all = 0;
  if ( arr ) {
    arr = &arr[col];
    _while ( *arr >= 0 && colspan > 0 ) {
      all += *arr;
      arr++;
      colspan--;
    };
  };

  return all;
};


void   html_set_width ( long *arr, int col, int w, int colspan )
{
  if ( arr ) {
    int safespan = colspan;
    arr = &arr[col];
    _while ( *arr >= 0 && colspan > 0 ) {
      if ( safespan > 1 ) {
        if ( *arr == 0 ) *arr = w/safespan;
      } else if ( *arr < w ) *arr = w;
      arr++;
      colspan--;
    };
  };
};


void   html_set_widths_to ( long *arr, int w, int allw )
{
  if ( arr ) {

    _while ( *arr >= 0 ) {

      int sx = *arr+(((double)(*arr)/(double)max(1, allw))*(double)w);

      if ( sx > *arr ) *arr = sx;

      arr++;
    };
  };
};


int   html_is_last_col ( long *arr, int col )
{
  if ( arr ) {
    _while ( *arr >= 0 ) {
      arr++;
      col--;
    };
    if ( col >= 0 ) return 1;
  };
  return 0;
};


int   html_get_to_col ( long *arr, int col )
{
  long all = 0;
  if ( arr )
    _while ( *arr >= 0 && col > 0 ) {
      all+=*arr;
      arr++;
      col--;
    };
  return all;
};


char *html_find_ptr_of_end_td ( char *text )
{
  char *h1 = stristr(text, "<TABLE");
  char *h2 = stristr(text, "<TR");
  char *h3 = stristr(text, "</TD>");
  char *h4 = stristr(text, "</TABLE>");
  char *h5 = stristr(text, "<TD");
  char *h6 = stristr(text, "<TH");
  char *h = NULL;

  /* h != NULL */
  if ( h1 < h2 && h1 ) h = h1; else h = h2;
  if ( (h3 < h && h3) || !h ) h = h3;
  if ( (h4 < h && h4) || !h  ) h = h4;
  if ( (h5 < h && h5) || !h  ) h = h5;
  if ( (h6 < h && h6) || !h  ) h = h6;


  if ( h6 == h || h5 == h || h4 == h || h3 == h || h2 == h ) {

    return h;

  } else if ( h1 == h ) {

    int n = 0;

    return html_find_ptr_of_end_td(html_find_ptr_of_end(h1, "<TABLE", "</TABLE>", &n));

  };

  return NULL;
};


char *html_find_ptr_of_end_th ( char *text )
{
  char *h1 = stristr(text, "<TABLE");
  char *h2 = stristr(text, "<TR");
  char *h3 = stristr(text, "</TH>");
  char *h4 = stristr(text, "</TABLE>");
  char *h5 = stristr(text, "<TH");
  char *h6 = stristr(text, "<TD");
  char *h = NULL;

  /* h != NULL */
  if ( h1 < h2 && h1 ) h = h1; else h = h2;
  if ( (h3 < h && h3) || !h ) h = h3;
  if ( (h4 < h && h4) || !h  ) h = h4;
  if ( (h5 < h && h5) || !h  ) h = h5;
  if ( (h6 < h && h6) || !h  ) h = h6;


  if ( h6 == h || h5 == h || h4 == h || h3 == h || h2 == h ) {

    return h;

  } else if ( h1 == h ) {

    int n = 0;

    return html_find_ptr_of_end_th(html_find_ptr_of_end(h1, "<TABLE", "</TABLE>", &n));

  };

  return NULL;
};


void html_frame_movex ( HTML_DEF_ENGINE, int left, int right )
{
  TYPframe.hleft += left;
  TYPframe.hright -= right;
  ENGrect.hleft = TYPframe.hleft;
  ENGrect.hright = TYPframe.hright;
};



int html_last_lists_without_text ( HTML_DEF_ENGINE )
{

     if ( HTML_STYLE & HTML_ST_LI ) return LI_S;
     if ( HTML_STYLE & HTML_ST_DD ) return DD_S;

     return 0;

};


/*
  char *html_translate_commander ( HTML_DEF_ENGINE, char *text )

  - function for command translating

    by command we means <P> or <PRE> f.e.

  Arguments :

  HTML_DEF_ENGINE is engine you have got from html_init_engine function
  text is string from <....

  Return :

  - pointer to the end of the command f.e. ..>"&...."

*/


char *html_translate_commander ( HTML_DEF_ENGINE, char *text, int *id )
{

  char *param = NULL;
  long  size  = 0;
  int   list_before = -1;
  int command = html_find_commander(text, &param, &size);

  (*id) = TYPignorespaces; /* ignore space chars */

  if ( command != -1 && text ) {

  /* if command is blocklevel type but before is not ended
     LI without text and parameter of command is not selected, command is set to LI */
  if ( ID_BLOCKLEVEL(command) ) {

    int v = html_last_lists_without_text(engine);
    if ( v ) {
      list_before = v;
    };
  };

  /* BLOCKLEVEL COMMANDS - HEADINGS */

  if ( ID_HEADINGS(command) ) { /* command is one of H1 to H6 */

      char *align = html_commander(param, "align"); /* find alignment */

      BLOCKLEVEL_SETTINGS;

      b.hfont = change_type_of_font(TYPfont, HTML_FONT_BOLD);
      b.hfont = change_weight_of_font(b.hfont, (HTML_FONTS-(command-H1_S))-1);
      b.hclass.halign = align?ret_align_id_from_str(align):TYPalign;

      html_nextline(engine);

      if ( list_before == -1 ) {

        ENG_OBJECT_INS(command, command, &b, sizeof(html_blocklevel));

        ENGlinespace = HTML_SPECIAL_SPACE;

      } else {

        ENG_OBJECT_INS(list_before, command, &b, sizeof(html_lists));

      };

      TYPfont = b.hfont;

      ALIGN_SET(b.hclass.halign);

      _free(align);

  } else


  switch ( command ) {


    /* HTML COMMANDS */

    case DISABLE_S : { /* "<!-- start of disabled text */

      char *v = strstr(text, "-->");

      if ( v ) {
        _free(param);
        return (v+3);
      };

    }; break;

    case BODY_S : { /* BODY */

      char *fcolor;
      char *bcolor;
      char *alcolor;
      char *licolor;
      char *vlcolor;
      char *pattern;

      BODY_START;

      fcolor = html_commander(param, "text");
      bcolor = html_commander(param, "bgcolor");
      licolor = html_commander(param, "link");
      alcolor = html_commander(param, "alink");
      vlcolor = html_commander(param, "vlink");
      pattern = html_commander(param, "background");

      if ( pattern )

        ENGpattern = ENG_DOWNLOADS_LOAD(ENGpath, pattern, IMG_S);

      ENGfcolor = fcolor?ret_color_from_str(fcolor):TYPfcolor;
      ENGbcolor = bcolor?ret_color_from_str(bcolor):TYPbcolor;
      ENGlicolor = licolor?ret_color_from_str(licolor):TYPacolor;
      ENGvlcolor = vlcolor?ret_color_from_str(vlcolor):HTML_COLOR_VLINK;
      ENGalcolor = alcolor?ret_color_from_str(alcolor):HTML_COLOR_ALINK;

      TYPfcolor = ENGfcolor;
      TYPacolor = ENGlicolor;
      TYPbcolor = ENGbcolor;

      _free(fcolor);
      _free(bcolor);
      _free(alcolor);
      _free(vlcolor);
      _free(licolor);
      _free(pattern);

    }; break;

    case TITLE_S : { /* <TITLE> */

      html_title title;
      title.htext = NULL;
      title.halign = TYPalign;
      title.hstyle = TYPstyle;
      title.hreference = NULL;
      title.hanchorname = NULL;
      title.hrect = HTML_RECT_EMPTY;

      ENGenter_lock;

      ENG_OBJECT_INS(TITLE_S, TITLE_S, &title, sizeof(html_title));

    }; break;


    /* BLOCKLEVEL COMMANDS */

    case DIV_S : { /* <DIV> */

      char *align = html_commander(param, "align"); /* find alignment */

      BLOCKLEVEL_SETTINGS;

      html_nextline(engine);

      b.hclass.halign = align?ret_align_id_from_str(align):TYPalign;

      ENG_OBJECT_INS(DIV_S, DIV_S, &b, sizeof(html_standard));

      ALIGN_SET(b.hclass.halign);

      _free(align);

    }; break;

    case ADDRESS_S : { /* <ADDRESS> */

      BLOCKLEVEL_SETTINGS;

      b.hfont = change_type_of_font(TYPfont, HTML_FONT_ITALIC);

      if ( list_before == -1 ) {

        ENG_OBJECT_INS(ADDRESS_S, ADDRESS_S, &b, sizeof(html_blocklevel));

      } else {

        ENG_OBJECT_INS(list_before, ADDRESS_S, &b, sizeof(html_lists));

      };

      TYPfont = b.hfont;

    }; break;

    case PRE_S : {  /* <PRE> */

      BLOCKLEVEL_SETTINGS;

      (*id) = HTML_TRAN_ACCEPT_SPACECHARS; /* not ignore space chars */

      b.hfont = HTML_FONT_3_FIXED;

      if ( list_before == -1 ) {

        ENG_OBJECT_INS(PRE_S, PRE_S, &b, sizeof(html_blocklevel));

        ENGlinespace = HTML_SPECIAL_SPACE;

      } else {

        ENG_OBJECT_INS(list_before, PRE_S, &b, sizeof(html_lists));

      };

      TYPfont = b.hfont;
      TYPignorespaces = HTML_TRAN_ACCEPT_SPACECHARS; /* not ignore space chars */

    }; break;

    case XMP_S : {  /* <XMP> */

      BLOCKLEVEL_SETTINGS;

      (*id) = HTML_TRAN_XMP; /* not ignore space chars & ignore <> chars */

      b.hfont = HTML_FONT_3_FIXED;

      if ( list_before == -1 ) {

        ENG_OBJECT_INS(XMP_S, XMP_S, &b, sizeof(html_blocklevel));

        ENGlinespace = HTML_SPECIAL_SPACE;

      } else {

        ENG_OBJECT_INS(list_before, XMP_S, &b, sizeof(html_lists));

      };

      TYPfont = b.hfont;

    }; break;

    case P_S : {  /* <P> */

      char *align = html_commander(param, "align"); /* find alignment */

      BLOCKLEVEL_SETTINGS;

      html_nextline(engine);

      b.hclass.halign = align?ret_align_id_from_str(align):TYPalign;

      if ( list_before == -1 ) {

        ENG_OBJECT_INS(P_S, P_S, &b, sizeof(html_blocklevel));

        ENGlinespace = HTML_SPECIAL_SPACE;

      } else {

        ENG_OBJECT_INS(list_before, P_S, &b, sizeof(html_lists));

      };

      _free(align);

      ALIGN_SET(b.hclass.halign);

    }; break;

    case BLOCKQUOTE_S : {  /* <BLOCKQUOTE> */

      BLOCKLEVEL_SETTINGS;

      if ( list_before == -1 ) {

        ENG_OBJECT_INS(BLOCKQUOTE_S, BLOCKQUOTE_S, &b, sizeof(html_blocklevel));

        ENGlinespace = HTML_SPECIAL_SPACE;

      } else {

        ENG_OBJECT_INS(list_before, BLOCKQUOTE_S, &b, sizeof(html_lists));

      };

      html_frame_movex(engine, ENGtab_blockquote, 0); /* move left origin of frame */

    }; break;


    /* FORMATING TAGS */

    case BR_S : { /* <BR> */

      int lnspace = ENGlinespace;
      int end     = ENGendenter;
      int oldl    = ENGline;

      ENGlinespace = HTML_LINE_SPACE;

      ENTER_REAL();

      ENGenter_unlock;
      ENGendenter = end;
      ENGlinespace = lnspace;

    }; break;


    case CENTER_S : { /* <CENTER> */

      CHARACTER_SETTINGS;

      html_nextline(engine);

      chr.hclass.halign = HTML_ALIGN_CENTERX;

      ENG_OBJECT_INS(ENGid, command, &chr, sizeof(html_blocklevel));

      ALIGN_SET(chr.hclass.halign);

    }; break;


    case HR_S : { /* <HR> */

      char *align = html_commander(param, "align"); /* find alignment */
      char *noshade = stristr(param, "noshade"); /* find argument noshade */
      char *size = html_commander(param, "size"); /* find thickness */
      char *width = html_commander(param, "width"); /* find width of line */

      int lsize    = size?ret_size_id_from_str(size, NULL):2;
      int lwidth   = width?ret_size_id_from_str_perc(width, html_rect_sizex(TYPframe), NULL):html_rect_sizex(TYPframe);
      int lalign   = align?ret_align_id_from_str(align):0;

      html_horline hr;

      html_nextline_bylock(engine, ENGlockenter);

      hr.hclass.halign = lalign;
      hr.hclass.hstyle = 0;
      hr.hclass.htext = NULL;
      hr.hclass.hreference = NULL;
      hr.hclass.hanchorname = NULL;

      hr.hnoshade = noshade?1:0;

      hr.hclass.hrect = html_rect_assign(TYPframe.hleft, ENGrect.htop, TYPframe.hleft+lwidth, ENGrect.htop+lsize);


      if ( lalign & HTML_ALIGN_RIGHT ) {

        if ( lwidth < html_rect_sizex(TYPframe) ) {
          hr.hclass.hrect.hright = TYPframe.hright;
          hr.hclass.hrect.hleft  = TYPframe.hright-lwidth;
        };

      } else if ( !(lalign & HTML_ALIGN_LEFT) ) {

          hr.hclass.hrect = html_rect_move(hr.hclass.hrect, max(0, (html_rect_sizex(TYPframe)-lwidth)/2), 0);

      };

      hr.hclass.halign = HTML_ALIGN_SELFALIGN;

      ENG_OBJECT_END(ENG_OBJECT_INS(HR_S, HR_S, &hr, sizeof(html_horline)), END_OBJECT);

      HTML_FLOAT_TRANS(&(ENGlasttype)->hrect); /* must reload rect, when image is floating */

//      ENG_OBJECT_END(html_make_no_text(engine), END_OBJECT);

      ENGrect.hbottom = hr.hclass.hrect.hbottom;

      ENGlinespace = HTML_LINE_SPACE;

      ENTER_REAL();

      enter_clear();

      _free(align);
      _free(size);
      _free(width);

    }; break;


    /* TEXT CHARACTERS COMMANDS */

    case B_S :        /* <B> */
    case STRONG_S : { /* <STRONG> */

      CHARACTER_SETTINGS;

      chr.hfont = change_type_of_font(TYPfont, HTML_FONT_BOLD);

      ENG_OBJECT_INS(ENGid, command, &chr, sizeof(html_blocklevel));

      TYPfont = chr.hfont;

    }; break;


    case BASEFONT_S : { /* <BASEFONT> */

      char *size = html_commander(param, "size"); /* find size of font */

      if ( size ) {

        int   lplus  = 0;
        int   lsize  = size?ret_size_id_from_str(size, &lplus):TYPfont;

        CHARACTER_SETTINGS;

        chr.hfont = change_weight_of_font(TYPfont, max(0, ret_weight_of_font(STDfont)));

        ENG_OBJECT_INS(ENGid, command, &chr, sizeof(html_blocklevel));

        STDfont = chr.hfont;
        TYPfont = chr.hfont;
      };

      _free(size);

    }; break;


    case BIG_S : { /* <BIG> */

      CHARACTER_SETTINGS;

      chr.hfont = change_weight_of_font(TYPfont, ret_weight_of_font(STDfont)+1);

      ENG_OBJECT_INS(ENGid, command, &chr, sizeof(html_blocklevel));

      STDfont = chr.hfont;
      TYPfont = chr.hfont;

    }; break;

    case BLINK_S : { /* <BLINK> */

      /* make nothing */

    }; break;

    case CODE_S :   /* <CODE> */
    case KBD_S  :   /* <KBD> */
    case TT_S   : { /* <TT> */

      CHARACTER_SETTINGS;

      chr.hfont = change_type_of_font(TYPfont, HTML_FONT_FIXED);

      ENG_OBJECT_INS(ENGid, command, &chr, sizeof(html_blocklevel));

      TYPfont = chr.hfont;

    }; break;


    case EM_S :       /* <EM> */
    case I_S :        /* <I> */
    case CITE_S :     /* <I> */
    case VAR_S    : { /* <VAR> */

      CHARACTER_SETTINGS;

      chr.hfont = change_type_of_font(TYPfont, HTML_FONT_ITALIC);

      ENG_OBJECT_INS(ENGid, command, &chr, sizeof(html_blocklevel));

      TYPfont = chr.hfont;

    }; break;


    case S_S :        /* <S> */
    case STRIKE_S :   /* <STRIKE> */
    case U_S      : { /* <U> */

      CHARACTER_SETTINGS;

      chr.hclass.hstyle |= HTML_STYLE_UNDERLINED;

      ENG_OBJECT_INS(ENGid, command, &chr, sizeof(html_blocklevel));

      TYPstyle = chr.hclass.hstyle;

    }; break;


    case SMALL_S : { /* <SMALL> */

      CHARACTER_SETTINGS;

      chr.hfont = change_weight_of_font(TYPfont, ret_weight_of_font(STDfont)-1);

      ENG_OBJECT_INS(ENGid, command, &chr, sizeof(html_blocklevel));

      STDfont = chr.hfont;
      TYPfont = chr.hfont;

    }; break;

    case FONT_S : {  /* <FONT> */

      char *color = html_commander(param, "color"); /* find alignment */
      char *size = html_commander(param, "size"); /* find size of font */

      long  lcolor = color?ret_color_from_str(color):TYPfcolor;
      int   lplus  = 0;
      int   lsize  = size?ret_size_id_from_str(size, &lplus):TYPfont;

      CHARACTER_SETTINGS;

      if ( size )
        chr.hfont = change_weight_of_font(TYPfont, lplus?ret_weight_of_font(STDfont)+lsize:lsize);

      chr.hfcolor = lcolor;

      ENG_OBJECT_INS(ENGid, command, &chr, sizeof(html_blocklevel));

      if ( size ) TYPfont = chr.hfont;
      if ( color ) TYPfcolor = chr.hfcolor;

      _free(color);
      _free(size);

    }; break;


    /* LISTS COMMANDS */

    case DL_S : {  /* <DL> */

      int subs = html_how_many_sub_commands(engine, DL_S);

      LIST_SETTINGS;

      b.hfcolor = STDfcolor;
      b.hfont = STDfont;

      ENG_OBJECT_INS(command, command, &b, sizeof(html_lists));

      b.hclass.hreference = _strdup(TYPreference); //
      b.hclass.hanchorname = _strdup(TYPanchorname); //

      ENG_OBJECT_INS(P_S, P_S, &b, sizeof(html_blocklevel));

      if ( !subs ) /* if it's not sub command */

        html_set_linespace(engine, HTML_SPECIAL_SPACE);

      else /* if it's not the first UL in the UL's */

        html_set_linespace(engine, HTML_LINE_SPACE);

      html_nextline(engine);

    }; break;


    case DT_S : {  /* <DT> */

      int subs = html_how_many_sub_commands_to(engine, DD_S, DT_S, DL_S);

      LIST_SETTINGS;

      b.hfcolor = STDfcolor;
      b.hfont = STDfont;

      ENG_OBJECT_INS(command, command, &b, sizeof(html_lists));

      html_frame_movex(engine, -(subs?ENGtab_list:0), 0);

      html_set_linespace(engine, HTML_LINE_SPACE);

      html_nextline(engine);

    }; break;


    case DD_S : {  /* <DD> */

      LIST_SETTINGS;

      b.hfcolor = STDfcolor;
      b.hfont = STDfont;

      ENG_OBJECT_INS(command, command, &b, sizeof(html_lists));

      html_set_linespace(engine, HTML_LINE_SPACE);

      if ( !html_how_many_sub_commands(engine, DT_S) ) {

        html_nextline(engine);

        ENGlinemove = ENGtab_list;

      } else {

        html_frame_movex ( engine, ENGtab_list, 0);

        html_nextline(engine);

     };

    }; break;


    case OL_S  :   /* <OL> */
    case UL_S  :   /* <UL> */
    case DIR_S :   /* <DIR> */
    case MENU_S : {  /* <MENU> */

      int subs = html_how_many_dir_sub_commands(engine);

      char *type = html_commander(param, "type"); /* find alignment */
      char *start = html_commander(param, "start"); /* find alignment */

      int   value = start?ret_size_id_from_str(start, NULL):-1;

      LIST_SETTINGS;

      b.hfcolor = STDfcolor;
      b.hfont = STDfont;

      if ( start ) b.hnumber = value; else b.hnumber = 1;

      if ( command == OL_S ) b.hlisttype = HTML_LISTTYPE_1; /* default settings */
      else
        if ( subs >= 2 ) b.hlisttype = HTML_LISTTYPE_SQUARE; /* default settings */
        else
        if ( subs >= 1 ) b.hlisttype = HTML_LISTTYPE_CIRCLE; /* default settings */
        else
        if ( subs >= 0 ) b.hlisttype = HTML_LISTTYPE_DISC; /* default settings */

      if ( type ) {
        if ( !stricmp(type, "disc") ) /* type is DISC */
          b.hlisttype = HTML_LISTTYPE_DISC;
        else
        if ( !stricmp(type, "circle") ) /* type is CIRCLE */
          b.hlisttype = HTML_LISTTYPE_CIRCLE;
        else
        if ( !stricmp(type, "square") ) /* type is SQUARE */
          b.hlisttype = HTML_LISTTYPE_SQUARE;
        else
        if ( !strcmp(type, "A") )       /* type is A */
          b.hlisttype = HTML_LISTTYPE_A;
        else
        if ( !strcmp(type, "a") )       /* type is a */
          b.hlisttype = HTML_LISTTYPE_a;
        else
        if ( !strcmp(type, "I") )       /* type is I */
          b.hlisttype = HTML_LISTTYPE_I;
        else
        if ( !strcmp(type, "i") )       /* type is i */
          b.hlisttype = HTML_LISTTYPE_i;
        else
        if ( !strcmp(type, "1") )       /* type is 1 */
          b.hlisttype = HTML_LISTTYPE_1;
      };

      ENG_OBJECT_INS(command, command, &b, sizeof(html_lists));

      html_frame_movex(engine, ENGtab_list, 0); /* move left origin of frame */

      b.hclass.hreference = _strdup(TYPreference);
      b.hclass.hanchorname = _strdup(TYPanchorname);

      ENG_OBJECT_INS(P_S, P_S, &b, sizeof(html_blocklevel));

      if ( !subs ) /* if it's not sub command */

        html_set_linespace(engine, HTML_SPECIAL_SPACE);

      else /* if it's not the first UL in the UL's */

         html_set_linespace(engine, HTML_LINE_SPACE);

      html_nextline(engine);

      TYPnumber = b.hnumber;
      TYPlisttype = b.hlisttype;

      _free(type);

    }; break;

    case LI_S : {  /* <LI> */

      char *type = html_commander(param, "type"); /* find alignment */
      char *start = html_commander(param, "start"); /* find alignment */

      int   value = start?ret_size_id_from_str(start, NULL):-1;

      LIST_SETTINGS;

      b.hfcolor = STDfcolor;
      b.hfont = STDfont;

      if ( start ) b.hnumber = value; else b.hnumber = TYPnumber;

      ENG_OBJECT_INS(LI_S, LI_S, &b, sizeof(html_lists));

      html_set_linespace(engine, HTML_LINE_SPACE);

      html_nextline(engine);

      _free(type);
      _free(start);

      TYPnumber = b.hnumber+1;

    }; break;


    /* ANCHORS */
    case A_S : {

      html_anchor a;

      CHARACTER_SETTINGS;

      a = chr;

      a.hclass.hreference = html_commander(param, "href");
      a.hclass.hanchorname = html_commander(param, "name");
      a.hfcolor = a.hclass.hreference?HTML_COLOR_ANCHOR:TYPfcolor;

      ENG_OBJECT_INS(ENGid, A_S, &a, sizeof(html_anchor));

      TYPreference = a.hclass.hreference;
      TYPanchorname = a.hclass.hanchorname;
      TYPfcolor = a.hfcolor;

    }; break;


    /* AREA COMMANDS */

    case IMG_S : {  /* <IMG> */

      html_image img;

      html_rect f = TYPframe;
      html_rect r = ENGrect;

      char *src = html_commander(param, "src"); /* image location */
      char *lowsrc = html_commander(param, "lowsrc"); /* low image location */
      char *alt = html_commander(param, "alt"); /* alternative text */
      char *align = html_commander(param, "align"); /* alignment */
      char *border = html_commander(param, "border"); /* border width */
      char *height = html_commander(param, "height"); /* height of image */
      char *width = html_commander(param, "width"); /* width of image */

      int lborder = border?ret_size_id_from_str(border, NULL):-1;
      int lalign = align?ret_align_id_from_str(align):0;
      int ialign = align?ret_align_id_from_str(align):TYPalign;
      int lheight = height?ret_size_id_from_str(height, NULL):0;
      int lwidth = width?ret_size_id_from_str(width, NULL):0;

      CHARACTER_SETTINGS;

      html_unclosed_blocklevel_before(engine);

      r = ENGrect;

      img.hclass = chr;
      img.hclass.hclass.halign = 0;

      img.honabort = html_commander(param, "onabort"); /* image Load JAVA code */
      img.honerror = html_commander(param, "onerror"); /* error JAVA code */
      img.honload  = html_commander(param, "onload"); /* image Load JAVA code */

      img.hclass.hclass.htext = alt;
      img.hborderwidth = lborder;
      img.hlowbitmap = NULL;


      /* download image from path and source src */
      img.hbitmap = ENG_DOWNLOADS_LOAD(ENGpath, src, IMG_S);

      if ( img.hbitmap && (lwidth != 0 || lheight != 0) ) { /* stretch image to correct size */

        HTML_DEF_BITMAP *strimg;

        /* correct size of image */
        if ( lwidth  == 0 ) lwidth  = HTML_FUNC_BITMAP_GETWIDTH(img.hbitmap);
        if ( lheight == 0 ) lheight = HTML_FUNC_BITMAP_GETHEIGHT(img.hbitmap);

        strimg = HTML_FUNC_BITMAP_STRETCH(img.hbitmap, lwidth, lheight);

        if ( strimg ) { /* make stretched image */

          HTML_FUNC_BITMAP_DESTROY(img.hbitmap);

          img.hbitmap = strimg;

        };

      };

      if ( img.hbitmap ) { /* insert to download objects */

        html_obj_ins_downloads(&ENGdownloads, _strdup(src), IMG_S, img.hbitmap);

      };

      HTML_FLOAT_TRANS(&f);
      HTML_FLOAT_TRANS(&r);

      if ( ialign == HTML_ALIGN_CENTERX ) {
        img.hclass.hclass.halign = HTML_ALIGN_CENTERX;
      } else
      if ( lalign == HTML_ALIGN_LEFT ) {
        img.hclass.hclass.halign = HTML_ALIGN_SELFALIGN;
        img.hclass.hclass.hstyle = HTML_STYLE_FLOAT;
      } else
      if ( lalign == HTML_ALIGN_RIGHT ) {
        img.hclass.hclass.halign = HTML_ALIGN_SELFALIGN;
        img.hclass.hclass.hstyle = HTML_STYLE_FLOAT;
      } else
        img.hclass.hclass.halign = 0;


      ENG_OBJECT_END(ENG_OBJECT_INS(ENGid, IMG_S, &img, sizeof(html_image)), END_OBJECT);

      if ( ENGnowrap != 1 )

        if ( r.hleft + lwidth > f.hright && r.hleft > f.hleft ) { /* out of range */

          enter_clear();

          lalign = 0;

          ENGlinespace = HTML_LINE_SPACE;

          html_nextline(engine); /* make enter */

          r = ENGrect;

          enter_clear();

        };

      if ( img.hbitmap ) {

        lwidth  = HTML_FUNC_BITMAP_GETWIDTH(img.hbitmap);
        lheight = HTML_FUNC_BITMAP_GETHEIGHT(img.hbitmap);

      };

      ENGlast->hline = ENGline;

      if ( f.hright - lwidth < f.hleft && lalign == HTML_ALIGN_RIGHT ) /* */

        lalign = HTML_ALIGN_LEFT;


      if ( lalign == HTML_ALIGN_LEFT ) {

        ENGlasttype->hrect = html_rect_assign(r.hleft, r.htop, r.hleft+lwidth, r.htop+lheight);

        HTML_FLOAT_INS(r.htop, r.htop+lheight, lwidth, 0);

      } else
      if ( lalign == HTML_ALIGN_RIGHT ) {

        ENGlasttype->hrect = html_rect_assign(f.hright-lwidth, r.htop, f.hright, r.htop+lheight);

        HTML_FLOAT_INS(r.htop, r.htop+lheight, 0, -lwidth);

      } else {

        ENGrect.hleft = r.hleft+lwidth;
        ENGrect.hbottom = ENGrect.htop+lheight;

        ENGlasttype->hrect = html_rect_assign(r.hleft, r.htop, r.hleft+lwidth, r.htop+lheight);

        HTML_STYLE = 0;

        enter_clear();

      };

//      ENGlastwasspace = 0;  /* after image may be space */

      _free(src);
      _free(lowsrc);
      _free(align);
      _free(border);
      _free(height);
      _free(width);

    }; break;


    /* FORMS COMMANDS */

    #ifdef 1
    case INPUT_S : {  /* <INPUT> */

      html_input inp;

      html_rect f = TYPframe;
      html_rect r = ENGrect;

      int lwidth  = 0;
      int lheight = 0;
      int lalign = 0;
      int lvalue = 0;

      char *type = html_commander(param, "type"); /* type of input */
      char *name = html_commander(param, "name"); /* name of input */
      char *value = html_commander(param, "value"); /* text or value */

      CHARACTER_SETTINGS;

      html_unclosed_blocklevel_before(engine);

      f = TYPframe;
      r = ENGrect;

      inp.hclass = chr;
      inp.htype = html_get_input_type(type);
      inp.hname = name;
      inp.hvalue = NULL;
      inp.hinserted = 0;
      inp.hselfcom = 0;
      inp.hivalue = 0;

      switch ( inp.htype ) { /* set width & height of input */

        case HTML_INPUT_BUTTON : {

          inp.hvalue = value;
          lwidth = HTML_INPUT_BUTTON_WIDTH(value);
          lheight = HTML_INPUT_BUTTON_HEIGHT(value);

        }; break;

        case HTML_INPUT_SUBMIT : {

          inp.hvalue = value;
          lwidth = HTML_INPUT_BUTTON_WIDTH(value);
          lheight = HTML_INPUT_BUTTON_HEIGHT(value);

        }; break;

        case HTML_INPUT_CHECKBOX : {

          lwidth = HTML_INPUT_CHECKBOX_WIDTH(value);
          lheight = HTML_INPUT_CHECKBOX_HEIGHT(value);

          _free(value);

        }; break;

        case HTML_INPUT_FILE : {

          inp.hvalue = value;
          lwidth = HTML_INPUT_FILE_WIDTH(value);
          lheight = HTML_INPUT_FILE_HEIGHT(value);

        }; break;

        case HTML_INPUT_HIDDEN : {

          inp.hvalue = value;
          lwidth = HTML_INPUT_HIDDEN_WIDTH(lvalue);
          lheight = HTML_INPUT_HIDDEN_HEIGHT(lvalue);

        }; break;

        case HTML_INPUT_IMAGE : {

          _free(value);

        }; break;

        case HTML_INPUT_PASSWORD : {

          lwidth = HTML_INPUT_PASSWORD_WIDTH(lvalue);
          lheight = HTML_INPUT_PASSWORD_HEIGHT(lvalue);

          _free(value);
        }; break;

        case HTML_INPUT_RADIO : {

          lwidth = HTML_INPUT_RADIO_WIDTH(value);
          lheight = HTML_INPUT_RADIO_HEIGHT(value);

          _free(value);
        }; break;

        case HTML_INPUT_RESET : {

          inp.hvalue = value;
          lwidth = HTML_INPUT_RESET_WIDTH(value);
          lheight = HTML_INPUT_RESET_HEIGHT(value);

        }; break;

        case HTML_INPUT_TEXT : {

          char *size = html_commander(param, "size"); /* size of input */

          inp.hivalue = size?ret_size_id_from_str(size, NULL):20;
          inp.hvalue = value;

          lwidth = HTML_INPUT_TEXT_WIDTH(inp.hivalue);
          lheight = HTML_INPUT_TEXT_HEIGHT(inp.hivalue);

          _free(size);

        }; break;

      };


      ENG_OBJECT_END(ENG_OBJECT_INS(ENGid, INPUT_S, &inp, sizeof(html_input)), END_OBJECT);

      if ( !ENGnowrap )

        if ( r.hleft + lwidth > f.hright && r.hleft > f.hleft ) { /* out of range */

          enter_clear();

          lalign = 0;

          html_nextline(engine); /* make enter */

          r = ENGrect;

          enter_clear();

        };

      ENGlast->hline = ENGline;

      ENGrect.hleft = r.hleft+lwidth;
      ENGrect.hbottom = ENGrect.htop+lheight;

      ENGlasttype->hrect = html_rect_assign(r.hleft, r.htop, r.hleft+lwidth, r.htop+lheight);

      enter_clear();

      HTML_STYLE = 0;

      _free(type);

    }; break;
    #endif


    /* TABLE COMMANDS */

    case TABLE_S : { /* TABLE */

      html_table table;

      HTML_ENGINE *engnew = NULL;
      HTML_ENGINE *engold = engine;

      int   minx = 0;
      int   maxx = 0;

      char *endptr = NULL;
      char *textnew = NULL;

      char *align = html_commander(param, "align"); /* alignment */
      char *border = html_commander(param, "border"); /* border width */
      char *height = html_commander(param, "height"); /* height of table */
      char *width = html_commander(param, "width"); /* width of table */
      char *bgcolor = html_commander(param, "bgcolor"); /* back color of table */
      char *cols = html_commander(param, "cols"); /* number of cols in table */
      char *cellpadding = html_commander(param, "cellpadding"); /* cellpading of table */
      char *cellspacing = html_commander(param, "cellspacing"); /* space between cells in table */
      html_type type;
      html_type oldtype = ENGtype;
      int   i = 1;

      BLOCKLEVEL_SETTINGS;

      if ( html_nextline_bylock(engine, 0) )

            html_moveline(engine);

      b.hclass.halign = align?ret_align_id_from_str(align):0;
//      b.hclass.halign |= HTML_ALIGN_TOP;

      table.hclass = b;

      type = ENGtype;

      table.hborder = border?ret_size_id_from_str(border, NULL):0;
      table.hheight = height?ret_size_id_from_str(height, NULL):0;
      table.hwidth =  width?ret_size_id_from_str_perc(width, html_rect_sizex(TYPframe), NULL):html_rect_sizex(TYPframe);
      table.hcols = cols?ret_size_id_from_str(cols, NULL):0;
      table.hcellpadding = /*cellpadding?ret_size_id_from_str(cellpadding, NULL):*/3;
      table.hcellspacing = cellspacing?ret_size_id_from_str(cellspacing, NULL):3;
      table.hcolwidth = NULL;
      table.hbgcolor = bgcolor?ret_color_from_str(bgcolor):TYPbcolor;
      table.hengine = NULL;

      ENGrect.hleft  = TYPframe.hleft;

      if ( table.hwidth )

        ENGrect.hright = ENGrect.hleft+table.hwidth;

      else

        ENGrect.hright = TYPframe.hright;

      /* insert object into array */
      ENG_OBJECT_INS(ENGid, TABLE_S, &table, sizeof(html_table));

      /* get pointer of </TABLE> */
      endptr = html_find_ptr_of_end(text, "<TABLE", "</TABLE>", &i);

      /* get new text */
      textnew = strnewn(text+size, strsize(text+size, endptr));

      /* exists this process ? */
      if ( !html_safe_table_get(engine, (long)(ENGoriginalpos+size), &TYPcolmin, &TYPcolmax, &TYPcolwidth) )
      {

         /* initialize new engine */

         engnew = html_init_engine_ex(ENGfirstengine, ENGoriginalpos+size,
                                   0, 0, html_rect_sizex(ENGrect), 1);

         engine = engnew; /* set new engine */

         BODY_START;

         /* table will be rendered */
         ENGtablestyle = HTML_TABLE_RENDER;

         TYPtdframe = TYPframe = ENGrect;

         /* translate function */
         html_file_translate(engnew, textnew, -1);

         engine = engold; /* reset old engine */

         /* copy widths from engnew->htype.hcolmin to current TYPxxx */
         TYPcolmin = html_alloc_widths_cpy(engnew->htype.hcolmin);
         /* copy widths from engnew->htype.hcolmax to current TYPxxx */
         TYPcolmax = html_alloc_widths_cpy(engnew->htype.hcolmax);

         /* get max number of cols ( td ) */
         table.hcols = html_get_num_cols(TYPcolmin);

         /* table was defined as relative */
         if ( width && strchr(width, '%')) {

            /* get width of all cols */
            int all = html_get_all(TYPcolmax);
            /* get the free space */
            int dx = html_rect_sizex(ENGrect) - all;

            /* there is some free space */
            if ( dx > 0 ) {

               /* divide free space between TYPcolmin widths */
               html_set_widths_to(TYPcolmin, dx, all);
               /* divide free space between TYPcolmax widths */
               html_set_widths_to(TYPcolmax, dx, all);

            };

         };

         /* set table.hcolmin to TYPcolmin */
         ((html_table*)ENGlasttype)->hcolmin = TYPcolmin;
         /* set table.hcolmax to TYPcolmax */
         ((html_table*)ENGlasttype)->hcolmax = TYPcolmax;
         /* copy TYPcolmin to table.hcolwidth and TYPcolwidth */
         ((html_table*)ENGlasttype)->hcolwidth = TYPcolwidth = html_alloc_widths(TYPcolmin);

         /* insert this process to array of processes... it saves the time */
         html_safe_table_ins(engine, (long)(ENGoriginalpos+size), TYPcolmin, TYPcolmax, TYPcolwidth);

         /* destroy engine */
         html_done_engine(engnew);

      };


      /* get max number of cols ( td ) */
      table.hcols = html_get_num_cols(TYPcolmin);

      /* set all important rects to ENGrect defined prvious */
      TYPtdframe = TYPframe = ENGrect;

      /* get minimal width of all colls */
      minx = html_get_all(TYPcolmin);
      /* get maximal width of all colls */
      maxx = html_get_all(TYPcolmax);

      /* set left+right origins of TABLE.rect to ENGrect.hleft */
      (ENGlasttype)->hrect.hleft = (ENGlasttype)->hrect.hright = ENGrect.hleft;
      /* set top+bottom origins of TABLE.rect to ENGrect.htop */
      (ENGlasttype)->hrect.htop = (ENGlasttype)->hrect.hbottom = ENGrect.htop;

      /* clear TD numerator */
      TYPtdnumber = 0;
      /* clear TR numerator to standard value if no TR will occured */
      TYPtrnumber = 1;

      /* set TABLE render style */
      TYPtdstyle = HTML_STYLE_TABLE_NONE+HTML_STYLE_TABLE_NOW;

      /* if minimal width of TABLE is lower or equals to the rect where can be visibled */
      if ( minx >= html_rect_sizex(TYPtdframe) )

        TYPtdstyle |= HTML_STYLE_TABLE_MIN;

      else
      /* if maximal width of TABLE is lower than the rect where can be visibled */
      if ( maxx < html_rect_sizex(TYPtdframe) )

        TYPtdstyle |= HTML_STYLE_TABLE_MAX;
      else
      /* if maximal width of TABLE is >= and minimal width of TABLE is lower
         than the rect where can be visibled */
      if ( maxx >= html_rect_sizex(TYPtdframe) && minx < html_rect_sizex(TYPtdframe) )

        TYPtdstyle |= HTML_STYLE_TABLE_RENDER;


      /* set types before rendering */
      TYPtdborder = table.hborder;
      TYPtdcols = table.hcols;
      TYPbcolor = table.hbgcolor;
      TYPtdcellspacing = table.hcellspacing;
      TYPtdcellpadding = table.hcellpadding;

      type = ENGtype;

      /* render table */

      ((html_table*)ENGlasttype)->hengine = html_call_sub_engine(engine,
                                                   html_rect_sizex(ENGrect), textnew,
                                                   (long)(ENGoriginalpos+size), type,
                                                   HTML_ALIGN_NOALIGN, NULL);

      /* set the right origin of table to max width of table + last space between cells */
      (ENGlasttype)->hrect.hright = (ENGlasttype)->hrect.hleft + html_get_max_width(((html_table*)ENGlasttype)->hengine)+table.hcellspacing*2;
      /* set the bottom origin of table to max width of table + last space between cells */
      (ENGlasttype)->hrect.hbottom = (ENGlasttype)->hrect.htop + html_get_max_y_all(((html_table*)ENGlasttype)->hengine)+table.hcellspacing*2;

      HTML_FLOAT_TRANS(&(ENGlasttype)->hrect); /* must reload rect, when image is floating */

      /* set current rect.top origin to bottom of table */
      ENGrect.htop = (ENGlasttype)->hrect.hbottom;
      /* set current rect.bottom origin to bottom of table */
      ENGrect.hbottom = ENGrect.htop;

      /* make alignment of table */
      html_set_alignment_ex(engine, ENGline, 1, 0/*b.hclass.halign*/);

      /* get oldtype set before table tag occured */
      ENGtype = oldtype;

      /* move line */
      html_moveline(engine);

      size = strsize(text, endptr);

      /* make enter */
      ENTER_REAL();

      _free(align);
      _free(border);
      _free(height);
      _free(width);
      _free(bgcolor);
      _free(cols);
      _free(cellpadding);
      _free(cellspacing);
      _free(textnew);


    }; break;


    case CAPTION_S : {  /* CAPTION */

         html_rect r;

         html_tr tr;

         BLOCKLEVEL_SETTINGS;

         tr.hclass = b;
         tr.hcellpadding = TYPtdcellpadding;
         tr.hborder = TYPtdborder;

         ENG_OBJECT_END(ENG_OBJECT_INS(ENGid, TR_S, &tr, sizeof(html_tr)), END_OBJECT);

         if ( ENGtablestyle != HTML_TABLE_RENDER ) { /* NON-RENDER SIZES */

                   html_set_alignment(engine, ENGline);

                   html_moveline ( engine );

                   ENGrect.htop = html_wheretr_inrowspan(engine, TYPtrnumber);
                   ENGrect.htop += TYPtdcellspacing+TYPtdcellpadding;

         } else {

            html_nextline_bylock(engine, 0);

         };

         TYPframe = TYPtdframe;
         TYPframe.hbottom = TYPframe.htop = ENGrect.htop;
         ENGrect = TYPframe;

    }; break;

    case TR_S      : { /* TR */

         html_rect r;
         html_tr   tr;

         BLOCKLEVEL_SETTINGS;

         tr.hclass       = b;
         tr.hcellpadding = TYPtdcellpadding;
         tr.hborder      = TYPtdborder;

         /* insert tag TR_S to array of objects and close it */
         ENG_OBJECT_END(ENG_OBJECT_INS(ENGid, TR_S, &tr, sizeof(html_tr)), END_OBJECT);

         /* table is in REAL mode */
         if ( ENGtablestyle != HTML_TABLE_RENDER ) { /* NON-RENDER SIZES */

                   /* move to next line */
                   html_moveline ( engine );

                   /* get bottom of last TR */
                   ENGrect.htop = html_wheretr_inrowspan(engine, TYPtrnumber);

                   ENGrect.htop += TYPtdcellspacing;

         } else
            /* table only renders sizes */
            html_nextline_bylock(engine, 0);


         TYPtrnumber++;// = html_lasttr_inrowspan(engine, TYPtrnumber, NULL);

         /* reset old frame */
         TYPframe = TYPtdframe;
         /* set frames top+bottom origins to last TR origin */
         TYPframe.hbottom = TYPframe.htop = ENGrect.htop;
         /* set rect to frame */
         ENGrect = TYPframe;

         /* reset TD numerator */
         TYPtdnumber = 0;

    }; break;


    case TH_S :   /* TH */
    case TD_S : { /* TD */


      char *colspan = html_commander(param, "colspan"); /* colspan of td */
      char *rowspan = html_commander(param, "rowspan"); /* rowspan */
      char *nowrap  = stristr(param, "nowrap"); /* wrap words ? */
      int   lcolspan = colspan?ret_size_id_from_str(colspan, NULL):1;
      int   lrowspan = rowspan?ret_size_id_from_str(rowspan, NULL):1;


      char *endptr = NULL;
      char *textnew = NULL;

      html_td td;

      BLOCKLEVEL_SETTINGS;

      if ( lrowspan == 0 ) lrowspan = 9999;
      if ( lcolspan == 0 ) lcolspan = 9999;

      td.hclass = b;
      td.hrowspan = lrowspan;
      td.hrow = TYPtrnumber;
      td.hvalign = 0;
      td.hcolspan = lcolspan;
      td.hheight = 0;
      td.hwidth = 0;
      td.hbgcolor = 0;
      td.hengine = NULL;
      td.hcellpadding = TYPtdcellpadding;
      td.hborder = TYPtdborder;

      if ( command == TH_S )

          TYPfont = change_type_of_font(TYPfont, HTML_FONT_BOLD);

      else

          TYPfont = change_type_of_font(TYPfont, HTML_FONT_NORMAL);


      /* get text string */

      if ( command == TH_S )

         endptr = html_find_ptr_of_end_th(text+size);

      else

         endptr = html_find_ptr_of_end_td(text+size);


      textnew = strnewn(text+size, strsize(text+size, endptr));

//      TYPtdnumber = html_wheretd_inrowspan(engine, TYPtrnumber, TYPtdnumber);

      html_moveline(engine);

      if ( ENGtablestyle == HTML_TABLE_RENDER ) { /* RENDER SIZES */

         HTML_ENGINE *engnew;
         HTML_ENGINE *engold = engine;
         char *swidth = html_commander(param, "width"); /* width of table */
         char *istable = stristr(text, "<TABLE");
         int   lwidth =  swidth?ret_size_id_from_str(swidth, NULL):0;
         int   owidth = 0;
         int   maxx = 0;
         int   minx = 0;
         int   ominx = 0;
         int   lc = 0;
         int   i = 1;

         td.hclass.hclass.hreference = _strdup(TYPreference);
         td.hclass.hclass.hanchorname = _strdup(TYPanchorname);

         ENG_OBJECT_END(ENG_OBJECT_INS(ENGid, command, &td, sizeof(html_td)), END_OBJECT);

         if ( swidth ) /* if in %, so ignore */

             lwidth = strchr(swidth, '%')?0:lwidth;

         /* check if this td wasn't translate */

         if ( !html_safe_td_get(engine, (long)(ENGoriginalpos+size), &minx, &maxx) )
         {

            /* initialize new engine */

            engnew = html_init_engine_ex(ENGfirstengine, (ENGoriginalpos+size),
                                         0, 0, 1, 1);

            engine = engnew;

            ENGnowrap = nowrap?1:2;

            BODY_START;


            html_file_translate(engine, textnew, -1);

            minx = html_get_max_width(engine);

            owidth = lwidth;
            ominx  = minx;

            lwidth = max(lwidth, minx);

            minx = lwidth;


            /* initialize the second engine */

            engine = engold; /* reset old engine */

            html_done_engine(engnew);

            if ( !nowrap ) {

               engnew = html_init_engine_ex(ENGfirstengine, (ENGoriginalpos+size),
                                            0, 0, 1, 1);

               engine = engnew;

               BODY_START;

               ENGnowrap = 1;

               if ( !owidth || owidth < minx ) {

                  html_file_translate(engine, textnew, -1);

                  maxx = html_get_max_width(engnew);

                  maxx = max(maxx, lwidth);

               } else maxx = minx;

               engine = engold; /* reset old engine */

               html_done_engine(engnew);

            } else maxx = minx;


           /* safe minx and maxx for either use */
            html_safe_td_ins(engine, (long)(ENGoriginalpos+size), minx, maxx);

         };

         html_insert_int_max(TYPtdnumber, &TYPcolmin, minx, lcolspan);

         html_insert_int_max(TYPtdnumber, &TYPcolmax, maxx, lcolspan);

         _free(swidth);

      } else { /* DRAW INTO ENGINE */

         /* get minimal width of the table */
         int minx = html_get_all(TYPcolmin);
         /* get maximal width of the table */
         int maxx = html_get_all(TYPcolmax);
         /* get maximal width of current cell */
         int my_maxx = html_get_col(TYPcolmax, TYPtdnumber, lcolspan);
         /* get minimal width of current cell */
         int my_minx = html_get_col(TYPcolmin, TYPtdnumber, lcolspan);
         /* set width to minimal width by default */
         int width = my_minx;

         /* get width of the cell */
         char *swidth = html_commander(param, "width");
         /* get alignment of the cell */
         char *align = html_commander(param, "align");
         /* get vertical alignment of the cell */
         char *valign = html_commander(param, "valign");
         /* get backcolor of the cell */
         char *bgcolor = html_commander(param, "bgcolor");

         int lalign = align?ret_align_id_from_str(align):0;
         int lwidth = 0;
         int lplus  = 0;

         html_rect r = TYPframe;
         html_rect f = TYPtdframe;
         html_type type = ENGtype;

         td.hvalign = valign?ret_align_id_from_str(valign):HTML_ALIGN_NOALIGN;
         td.hcolspan = lcolspan;

         if ( command == TH_S && !lalign )

             lalign = HTML_ALIGN_CENTERX;

/*         if ( td.hvalign & HTML_ALIGN_TOP ) {

             lalign |= HTML_ALIGN_TOP;

         };*/

         /* set alignment of the cell */
         b.hclass.halign = lalign;

         if ( TYPtdstyle & HTML_STYLE_TABLE_MAX ) {

            int dx = 0;

            lplus = html_get_to_col(TYPcolmax, TYPtdnumber);

            lwidth = swidth?ret_size_id_from_str_perc(swidth, html_rect_sizex(f)-lplus, NULL):0;

            width = max(lwidth, my_maxx);

            html_set_width(TYPcolmax, TYPtdnumber, width, lcolspan);

         }

         else if ( TYPtdstyle & HTML_STYLE_TABLE_MIN ) {

            lplus = html_get_to_col(TYPcolmin, TYPtdnumber);

            width = my_minx;

            width = max(width, html_rect_sizex(f)-lplus);

         }
         else
         if ( TYPtdstyle & HTML_STYLE_TABLE_RENDER )
         {

            int  lwidth;
            int _W_;
            int _D_;
            int _d_;
            int _w_;

            lplus = html_get_to_col(TYPcolwidth, TYPtdnumber);

            _W_ = (html_rect_sizex(f)-lplus) - minx;
            _D_ = maxx-minx;
            _d_ = my_maxx-my_minx;
            _w_ = my_minx + (int)((double)_d_*((double)_W_/(double)_D_));

            lwidth = swidth?ret_size_id_from_str_perc(swidth, html_rect_sizex(f)-lplus, NULL):0;

            if ( lwidth ) width = max(my_minx, lwidth);

            else width = max(my_minx, _w_);

            html_set_width(TYPcolwidth, TYPtdnumber, width, lcolspan);

         };

         lplus += TYPtdcellspacing*(TYPtdnumber+1);
         lplus += TYPtdcellpadding*(TYPtdnumber+1);

         td.hwidth = width+TYPtdcellpadding;

         if ( lcolspan > 1 )

            td.hwidth += (TYPtdcellspacing + TYPtdcellpadding)*(lcolspan-1);

         r.hleft = TYPtdframe.hleft+lplus;
         r.hright = r.hleft+td.hwidth;

         td.hbgcolor = bgcolor?ret_color_from_str(bgcolor):TYPbcolor;

         type = ENGtype;
         type.hbcolor = td.hbgcolor;

         td.hengine = html_call_sub_engine(engine, width, textnew, (long)(ENGoriginalpos+size), type, lalign, NULL);

         td.hheight = 0;

         td.hclass.hclass.hreference = _strdup(TYPreference);
         td.hclass.hclass.hanchorname = _strdup(TYPanchorname);

         ENG_OBJECT_END(ENG_OBJECT_INS(ENGid, command, &td, sizeof(html_td)), END_OBJECT);

         (ENGlasttype)->hrect.htop = (ENGlasttype)->hrect.hbottom = r.htop+td.hcellpadding;
         (ENGlasttype)->hrect.hbottom += html_get_max_y_all(td.hengine)+td.hcellpadding;
         (ENGlasttype)->hrect.hleft = r.hleft;
         (ENGlasttype)->hrect.hright = r.hright;

//         html_set_alignment_ex(engine, ENGline, 1, b.hclass.halign);

         html_moveline(engine);

         _free(swidth);
         _free(align);
         _free(bgcolor);

       };

       TYPtdnumber += lcolspan;

       _free(textnew);
       _free(rowspan);
       _free(colspan);

       size = strsize(text, endptr);

    }; break;

//    default : ENG_OBJECT_INS(command, command, NULL, 0); break;


  };

  if ( ID_BLOCKLEVEL(command) ) {

    HTML_STYLE |= HTML_ST_BLOCKLEVEL;

  } else
  if ( ID_LISTS(command) ) {

    switch ( command ) {
      case LI_S : HTML_STYLE = HTML_ST_LI+HTML_ST_BLOCKLEVEL; break;
      case DD_S : HTML_STYLE = HTML_ST_DD+HTML_ST_BLOCKLEVEL; break;
      case DL_S : HTML_STYLE = HTML_ST_DL+HTML_ST_BLOCKLEVEL; break;
      case DT_S : HTML_STYLE = HTML_ST_DT+HTML_ST_BLOCKLEVEL; break;
      default: HTML_STYLE |= HTML_ST_BLOCKLEVEL;
    };

  };

  } else html_err_func("Function is not supported\n");

  _free(param);
  return text?(text+size):NULL;
};

