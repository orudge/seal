/****************************************************************/
/*                                                              */
/*                           html4.c                            */
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


#include"html2.c"
#include"htmlref.c"
#include"htmlcol.c"
#include<pc.h>

#ifndef strexist
#define strexist(p)     ((p) && *(p))
#endif
#ifndef strsize
#define strsize(p,v)    ((v)?((v)-(p)):strlen(p))
#endif
#define strchar(s,c)    ((s) && *(s) == (c) && !(*((s)+1)) )

#define IS_ONE_SPACE_CHAR(s) (s && *(s) && !(*((s)+1)) && hsnch2(*s))

#define ALIGN_CLEARLINE() ENGlinealign = 0
#define ALIGN_GET()  (HTML_NO_ALIGNMENT(ENGlinealign)?TYPalign:ENGlinealign)
#define ALIGN_SET(a) ENGlinealign = TYPalign = a
#define ENTER_REAL()        do {                                      \
                              ENGendenter  = 0;                       \
                              html_nextline_bylock ( engine, 0 );     \
                            } while (0)

#define ENTER_OK()          ( !ENGendenter )
#define IS_ENTER_TEXT(tx)   (strexist(tx) && !ENGlockenter)
#define IS_ENTER_IMAGE(img) (img && !ENGlockenter)

  #define enter_clear()   do { \
      ENGenter_unlock;         \
      ENGendenter = 0;         \
    } while(0)
  #define enter_set()   do { \
      ENGenter_lock;         \
      ENGendenter = 1;         \
    } while(0)

#define html_set_alignment(e,l) html_set_alignment_ex(e, l, 1, TYPalign)

#define HTML_TRAN_IGNORE_SPACECHARS   -1000
#define HTML_TRAN_ACCEPT_SPACECHARS   -2000
#define HTML_TRAN_XMP                 -3000


FILE *TEST = NULL;


/*
 int   html_find_commander ( char *, char **, long * )

 - this function find html command from text and return id of command
 - param is set to command parameters
 - size is set to end of command and parameter

 Example:

  <P ALIGN="CENTER">Hello

  function return id of P command, "param" is set to ALING="CENTER" and
  size is set to strlen("P ALIGN="CENTER">")
*/
int   html_find_commander ( char *text, char **param, long *size );


/*

  char *html_continue ( HTML_DEF_ENGINE, char *text, long size )

  - write text defined by length "size" to engine by current settings
    and return text+size

*/
char *html_continue ( HTML_DEF_ENGINE, char *text, long size, int refok );



long  html_file_translate ( HTML_DEF_ENGINE, char *text, int st );
char *html_translate_commander ( HTML_DEF_ENGINE, char *text, int *i );
char *html_translate_end_commander ( HTML_DEF_ENGINE, char *text, int *i );
char *html_find_ptr_of_end ( char *text, char *st, char *en, int *i );
HTML_ENGINE  *html_init_engine ( long left, long top, long right, long bottom );
HTML_ENGINE  *html_init_engine_ex ( HTML_ENGINE *engfirst, char *op, long left, long top, long right, long bottom );



void html_afree ( void **a )
{
  if ( a ) {
    _free(*a);
    *a = NULL;
  };
};


int   html_how_many_dir_sub_commands ( HTML_DEF_ENGINE )
{
  html_obs *o = ENGmain;
  int       i = 0;
  _while ( o ) {
    if ( !o->hend && (o->hidin == DIR_S || o->hidin == OL_S || o->hidin == DL_S ||
                      o->hidin == UL_S  || o->hidin == MENU_S) ) i++;
    o = o->hnext;
  };
  return i;
};


int   html_how_many_sub_commands_to ( HTML_DEF_ENGINE, int comm, int to, int to2 )
{
  html_obs *o = ENGlast;
  int       i = 0;
  _while ( o ) {
    if ( o->hidin == comm )
      if ( !o->hend ) return 1; else return 0;
    else
    if ( o->hidin == to || o->hidin == to2 ) return 0;

    o = o->hprev;
  };
  return 0;
};


int   html_how_many_sub_commands ( HTML_DEF_ENGINE, int comm )
{
  html_obs *o = ENGmain;
  int       i = 0;
  _while ( o ) {
    if ( o->hidin == comm && !o->hend ) i++;
    o = o->hnext;
  };
  return i;
};



html_obs *html_find_line ( HTML_DEF_ENGINE, long line )
{
  html_obs *o = ENGlast;
  html_obs *ok = NULL;
  _while ( o ) {
    if ( o->hview && o->hline == line ) ok = o;
    if ( ok && o->hline != line ) return ok;
    o = o->hprev;
  };
  return ok;
};



void  html_set_alignment_ex ( HTML_DEF_ENGINE, long line, int engset, int al )
{

  if ( !ENGnowrap ) {

      html_obs *o = html_find_line(engine, line);
      html_obs *v = o;
      int align = al;
      int maxx  = 0;
      int maxy  = html_rect_sizey(ENGrect);
      int dx    = 0;
      int dy    = 0;

      html_rect r = TYPframe;

      _while ( o && o->hline == line ) {

            if ( o->hline > -1 && o->hview && o->hobjtype ) {

               r = o->hobjtype->hsafe.hframe;
               if (  !HAS_SELFALIGN(o) ) {
                  maxx += max(0, html_rect_sizex(o->hobjtype->hrect));
                  if ( !HTML_NO_ALIGNMENT(o->hobjtype->halign) ) align = o->hobjtype->halign;
               };
               if ( !(o->hobjtype->hstyle & HTML_STYLE_FLOAT) )
                     maxy  = max(maxy, html_rect_sizey(o->hobjtype->hrect));
            };
            o = o->hnext;
      };

      r.htop = ENGrect.htop;
      r.hbottom = ENGrect.hbottom;

      html_float_get_xorigins(engine, &r);

      if ( align & HTML_ALIGN_CENTERX ) {
         dx = max(0, ((r.hright-r.hleft)-maxx)/2);
      };

      if ( align & HTML_ALIGN_RIGHT ) {
         dx = max(0, (r.hright-r.hleft)-maxx);
      };

      _while ( v && v->hline == line ) {
            if ( v->hline > -1 && v->hview && v->hobjtype && !html_rect_equals(v->hobjtype->hrect, HTML_RECT_EMPTY) &&
                 !HAS_SELFALIGN(v) ) {

               v->hobjtype->hrect = html_rect_move(v->hobjtype->hrect, dx, 0);

               v->hobjtype->halign &= ~(HTML_ALIGN_RIGHT+HTML_ALIGN_CENTERX);

               if ( !(v->hobjtype->hstyle & HTML_STYLE_FLOAT) ) {
                  int sizey = html_rect_sizey(v->hobjtype->hrect);

                  if ( v->hobjtype->halign & HTML_ALIGN_TOP ) {

                     v->hobjtype->hrect.hbottom = v->hobjtype->hrect.htop + sizey;

                  } else

                  if ( v->hobjtype->halign & HTML_ALIGN_CENTERY ) {

                     v->hobjtype->hrect.htop += (maxy - sizey)/2;
                     v->hobjtype->hrect.hbottom = v->hobjtype->hrect.htop + sizey;

                  } else { /* default is bottom */

                     v->hobjtype->hrect.hbottom = v->hobjtype->hrect.htop + maxy;
                     v->hobjtype->hrect.htop = v->hobjtype->hrect.hbottom-sizey;
                  };
               };

               v->hobjtype->halign &= ~(HTML_ALIGN_TOP+HTML_ALIGN_BOTTOM|HTML_ALIGN_CENTERY);
            };
            v = v->hnext;
      };

      if ( engset ) ENGrect.hbottom = ENGrect.htop+maxy;

  };

};


int  html_set_linespace ( HTML_DEF_ENGINE, int spacesize )
{

  if ( HTML_STYLE != HTML_ST_BLOCKLEVEL || spacesize == HTML_SPECIAL_SPACE ) {

     ENGlinespace = spacesize;

     return 1;

  };

  return 0;
};



void  html_set_frame ( HTML_DEF_ENGINE )
{
  ENGrect.hleft = TYPframe.hleft+ENGlinemove;
  ENGrect.hright = TYPframe.hright;
};


int  html_nextline_bylock ( HTML_DEF_ENGINE, int lock )
{
    int ok = 0;

    html_set_frame(engine);

    if ( ENTER_OK() && !lock ) {

      html_set_alignment(engine, ENGline);

      ENGrect = html_rect_assign(TYPframe.hleft, ENGrect.hbottom+ENGlinespace,
                            TYPframe.hright, ENGrect.hbottom+ENGlinespace);

      ENGlastlinespace = ENGlinespace;

      ENGlinemove = 0;

      ENGline++;

      ok = 1;

      ENGenter_lock;
      ENGendenter = 1;

      ENGlastwasspace = 1;

    } else {

      if ( ENGlastlinespace < ENGlinespace ) {

        ENGrect = html_rect_move(ENGrect, 0, ENGlinespace-ENGlastlinespace);
        ENGlastlinespace = ENGlinespace;

      };


    };

    return ok;

};


int  html_moveline ( HTML_DEF_ENGINE )
{
    ENGline++;

    return 1;

};


int  html_nextline ( HTML_DEF_ENGINE )
{
  return html_nextline_bylock(engine, ENGlockenter);
};


void  html_set_enter ( HTML_DEF_ENGINE )
{

  if ( ENGlasttype ) {

    html_rect r = ENGrect;

    HTML_FLOAT_TRANS(&r);

    ENGlast->hobjtype->hrect = r;
    ENGlast->hobjtype->hrect.hright = r.hleft;
    ENGlast->hline = ENGline;

    if ( ENGlast->hobjtype->hanchorname == TYPanchorname &&
         TYPanchorname ) TYPanchorname = NULL;

  };

};


int  html_enter ( HTML_DEF_ENGINE )
{
  if ( ENGlasttype ) {

    int ok = html_nextline(engine);

    html_set_enter(engine);

    return ok;
  };
  return 0;
};


int  html_unclosed_blocklevel_before ( HTML_DEF_ENGINE )
{
  int ok = 2;

  if ( HTML_STYLE & HTML_ST_BLOCKLEVEL ) ok = 1;

  if ( ok == 1 && ENTER_OK()) {

    html_nextline(engine);               /* make enter */

    ENGrect.hbottom = ENGrect.htop+HTML_FUNC_FONT_GETHEIGHT(HTMLFONTid(TYPfont));

    html_set_enter(engine);              /* + make enter */

    HTML_STYLE = 0;

  };

  return (ok==1)?1:0;
};


int  html_set_text ( HTML_DEF_ENGINE, char *text )
{
  int ok = 0;

  if ( strexist(text) ) {

    if ( ENGrealid == TITLE_S && !ENGtitle ) /* set title */

         ENGtitle = _strdup(text);

    if ( ENGlasttype && !ENGlast->hend && !ENGlasttype->htext ) {

        ENGlasttype->htext = _strdup(text);
        ((html_blocklevel*)ENGlasttype)->hfcolor = TYPfcolor;
        ((html_blocklevel*)ENGlasttype)->hfont = TYPfont;

        /* if first character is blocklevel style => make return */
        ok = 2;

    } else {

      if ( ID_BLOCKLEVEL(ENGid) ) {

        html_blocklevel tex;
        tex.hclass.hrect = HTML_RECT_EMPTY;
        tex.hclass.htext = _strdup(text);
        tex.hclass.halign = ALIGN_GET();
        tex.hclass.hstyle = TYPstyle;
        tex.hclass.hreference = _strdup(TYPreference);
        tex.hclass.hanchorname = _strdup(TYPanchorname);
        tex.hfont = TYPfont;
        tex.hfcolor = TYPfcolor;
        tex.hnumber = TYPnumber;
        tex.hlisttype = TYPlisttype;
        tex.hselected = 0;

        ENG_OBJECT_END(ENG_OBJECT_INS(ENGid, ENGid, &tex, sizeof(html_blocklevel)), END_OBJECT);

        ok = 2;

      } else {

        html_blocklevel tex;
        tex.hclass.hrect = HTML_RECT_EMPTY;
        tex.hclass.htext = _strdup(text);
        tex.hclass.halign = ALIGN_GET();
        tex.hclass.hstyle = TYPstyle;
        tex.hclass.hreference = _strdup(TYPreference);
        tex.hclass.hanchorname = _strdup(TYPanchorname);
        tex.hfont = TYPfont;
        tex.hfcolor = TYPfcolor;
        tex.hnumber = TYPnumber;
        tex.hlisttype = TYPlisttype;
        tex.hselected = 0;

        ENG_OBJECT_END(ENG_OBJECT_INS(P_S, ENGid, &tex, sizeof(html_blocklevel)), END_OBJECT);

        ok = 3;

      };
    };

    if ( ok && (HTML_STYLE & HTML_ST_BLOCKLEVEL) ) ok = 1;

    if ( ok ) {

         ENGid = P_S;

         if ( ok == 1 ) HTML_STYLE = 0; /* clear style of first character */

    };

  };

  return ok;
};


int   html_find_commander ( char *text, char **param, long *size )
{
  (*param) = NULL;
  (*size) = 0;
  if ( text ) {
    char *end = strstr(text, BEGIN_E);
    (*size) = strsize(text, end)+1;
    if ( end ) {
      int i = find_html_start(HTMLCOMMANDS, text);
      if ( i != -1 ) {
         int from = strlen(HTMLCOMMANDS[i])+strlen(BEGIN_S);
         (*param) = strnewn(text+from, strsize(text+from, end));
      };

      return i;
    };
  } else return -1;
};



/*

   This function is call from main translating function, when some text
   occurse. Size is size of text, that must be showed and okref may be
   -1 || 1
   1  : Text must be reformated
  -1  : Not reformated text. Typical for PRE function

*/
char *html_continue ( HTML_DEF_ENGINE, char *text, long size, int okref )
{

  #define real_rectx(r)  html_get_float_rectx(engine, r)
  char *v = strnewn(text, size);
  char *ref;
  int   proc;
  long  szref;
  int textsize;
  int lastspace = 0;
  int lastwasspace = ENGlastwasspace;

  if ( !v ) return text+size;

  if ( okref > 0 ) { /* reformat text */
     ref = html_reformat_text(v, !ENGlastwasspace);
  } else
  if ( okref == -1 || okref == -3 ) /* reformat only from special characters */
    ref = html_ref_from_special_characters(v);
  else
    ref = _strdup(v); /* not reformat text */


  _free(v);

  proc = html_set_text(engine, ref); /* set text to object */

  if ( !proc ) return text+size;

  if ( !ENTER_OK() && IS_ONE_SPACE_CHAR(ref) ) return text+size;

  if ( proc == 1 && ENTER_OK() && IS_ENTER_TEXT(ref) /*&& ID_BLOCKLEVEL(ENGrealid)*/ ) { /* make enter */

    html_nextline(engine);               /* make enter */

    ENGrect.hbottom = ENGrect.htop+HTML_FUNC_FONT_GETHEIGHT(HTMLFONTid(TYPfont));

    html_set_enter(engine);              /* + make enter */

    enter_clear(); /* unlock enter */

  } else if ( proc ) {

    if ( proc == 1 )

      html_nextline_bylock(engine, 1);

    ENGrect.hbottom = ENGrect.htop+HTML_FUNC_FONT_GETHEIGHT(HTMLFONTid(TYPfont));

    html_set_enter(engine); /* set only rectangle for text */

    enter_clear(); /* unlock enter */

  };

  szref = strlen(ref);

  if ( szref > 0 && ref[szref-1] == HTML_SPACE_CHAR )

    ENGlastwasspace = 1;

  else

    ENGlastwasspace = 0;



  if ( okref < 0 && proc ) { /* ACCEPT ENTER CHARS */

    char *h = strchr(ref, HTML_ENTER_CHAR); /* find enter char */

    if ( h && h == ref && okref == -3 ) { /* first enter ignore */

       html_afree((void**)(&(ENGlasttype->htext)));
       html_continue(engine, h+1, strlen(h+1), -2);

       return text+size;

    } else

    if ( okref == -2 && h && h == &(text[size-1]) ) /* only enter at the end */

       return text+size;

  };

  if ( proc && ENGlasttype && ENGlastview )
  {

      if ( okref < 0 ) { /* Not reformated text => for functions XMP, PRE */

         char *h = strchr(ref, HTML_ENTER_CHAR);

         if ( h ) { /* found enter */

           int old_line_space = ENGlinespace;

           _free(ENGlasttype->htext);

           ENGlinespace = HTML_LINE_SPACE;

           ENGlasttype->htext = strnewn(ref, strsize(ref, h));

           textsize = HTML_FUNC_FONT_GETSTRWIDTH(HTMLFONTid(TYPfont), ENGlasttype->htext, strlen(ENGlasttype->htext));

           ENGrect.hbottom = ENGrect.htop+HTML_FUNC_FONT_GETHEIGHT(HTMLFONTid(TYPfont));

           ENGlasttype->hrect.hright = ENGlasttype->hrect.hleft+textsize;
           ENGlasttype->hrect.hbottom = ENGrect.hbottom;

           html_nextline(engine);

           ENGlinespace = old_line_space;

           html_continue(engine, h+1, strlen(h+1), -2);

         } else { /* char of enter (#13) wasn't found so only move cursor */

           textsize = HTML_FUNC_FONT_GETSTRWIDTH(HTMLFONTid(TYPfont), ENGlasttype->htext, strlen(ENGlasttype->htext));

           ENGrect.hleft += textsize;

           ENGrect.hbottom = ENGrect.htop+HTML_FUNC_FONT_GETHEIGHT(HTMLFONTid(TYPfont));

           ENGlasttype->hrect.hright = ENGlasttype->hrect.hleft+textsize;
           ENGlasttype->hrect.hbottom = ENGrect.hbottom;

         };


      } else { /* text for reformating */

        int  refsz = strlen(ref); /* gets size of string */
                 /* return number of characters in object rectangle */
        int  chs = html_font_get_words_in_width(HTMLFONTid(TYPfont), ref, real_rectx(ENGrect), html_rect_sizex(TYPframe));
        int  ow  = html_get_first_word(ref); /* length of one word */
        int  entok = 0;

        /* minimum size */
        if ( chs <= 1 &&
           ( ENGnowrap & 0x02 ||
             /* word is longer than frame => so it's stupid to enter it */
             HTML_FUNC_FONT_GETSTRWIDTH(HTMLFONTid(TYPfont), ref, ow) >= html_rect_sizex(TYPframe)) ) {

            chs = ow;

            if ( ENGlastwasspace )  /* wasn't space char in last command */

               entok = 1;
        };

        if ( ENGnowrap & 0x01 ) { /* if no wrap */

           chs = refsz;
           entok = 0;

        };

        if ( IS_ONE_SPACE_CHAR(ref) ) chs = ow;

        if ( chs < refsz || entok ) { /* text size is longer than rectangle, so clip text */

          int old_line_space = ENGlinespace;
          html_obs *o = NULL;

          _free(ENGlasttype->htext);

          ENGlinespace = HTML_LINE_SPACE;

          ENGlasttype->htext = strnewn(ref, chs);

          textsize = HTML_FUNC_FONT_GETSTRWIDTH(HTMLFONTid(TYPfont), ENGlasttype->htext, strlen(ENGlasttype->htext));

          ENGrect.hbottom = ENGrect.htop+HTML_FUNC_FONT_GETHEIGHT(HTMLFONTid(TYPfont));

          ENGlasttype->hrect.hright = ENGlasttype->hrect.hleft+textsize;
          ENGlasttype->hrect.hbottom = ENGrect.hbottom;

          html_nextline(engine);

          ENGlinespace = old_line_space;

          html_continue(engine, &ref[chs], refsz-chs, 0);

        } else { /* text is not larger than rectangle X area, so only move cursor */

          textsize = HTML_FUNC_FONT_GETSTRWIDTH(HTMLFONTid(TYPfont), ENGlasttype->htext, strlen(ENGlasttype->htext));

          ENGrect.hleft += textsize;

          ENGrect.hbottom = ENGrect.htop+HTML_FUNC_FONT_GETHEIGHT(HTMLFONTid(TYPfont));

          ENGlasttype->hrect.hright = ENGlasttype->hrect.hleft+textsize;
          ENGlasttype->hrect.hbottom = ENGrect.hbottom;

        };
      };
    };


  if ( szref > 0 && ref[szref-1] == HTML_SPACE_CHAR )

    ENGlastwasspace = 1;

  else

    ENGlastwasspace = 0;


  _free(ref);
  return text+size;
};


void  html_set_end ( html_obs *o, int n, int command )
{
  if (!o) return;

  o->hend = n;

  o = o->hnext;

  _while ( o ) {

      if ( ID_BLOCKLEVEL(o->hidin) ||
           ID_LISTS(o->hidin) ||
           ID_ANCHOR(o->hidin) )

               o->hend = n;

      o = o->hnext;

  };
};



html_type *html_find_last_type ( HTML_DEF_ENGINE, int command, int *safetype, html_obs **obj )
{
  if ( obj ) (*obj) = NULL;

  if ( command != -1 ) {

     html_obs *m = ENGlast;
     html_obs *o = NULL;
     html_type *safe = NULL;
     int     oldtype = HTML_S;

     _while ( m && !safe ) {

       if ( m->hidin == command && !m->hend ) {  /* it's same command */

         (*safetype) = oldtype;

         o = m;

         if ( obj ) (*obj) = o;

         if ( o->hobjtype )

           safe = &(m->hobjtype->hsafe); /* copy old variables to safe param */

       };

       m = m->hprev;

     };

     html_set_end(o, 1, command);

     return safe;

  } else return NULL;
};


void  html_end_settings ( HTML_DEF_ENGINE, int type, html_type *t, html_obs *obj )
{

  int alignset = 0;

  if ( type != -1 ) {

    if ( ID_BLOCKLEVEL(type) ) { /* end command is BLOCKLEVEL */

      /* BLOCKLEVEL COMMANDS */
      if ( t ) {

        if ( type >= H1_S && type <= H6_S ) { /* HEADING TAGS */

          TYPfont  = t->hfont;
          alignset = 1;

        };

        switch ( type ) {

          case BLOCKQUOTE_S : ENGframe = TYPframe = t->hframe; break;
          case P_S          : alignset = 1; break;
          case DIV_S        : alignset = 1; break;
          case PRE_S        : {

             TYPfont  = t->hfont;
             TYPignorespaces = t->hignorespaces;

          }; break;

          case XMP_S        : {

             TYPfont  = t->hfont;

          }; break;

          case ADDRESS_S        : {

             TYPfont  = t->hfont;

          }; break;

        };

      };

      if ( type != DIV_S && type != ADDRESS_S ) ENGlinespace = HTML_SPECIAL_SPACE;
      else ENGlinespace = HTML_LINE_SPACE;

      html_nextline(engine); /* enter after it */

    } else


    switch ( type ) {

    /* FORMATING TAGS */
      case CENTER_S : { /* <CENTER> */

        alignset = 1;

      }; break;

    /* TEXT CHARACTERS COMMANDS */
      case BASEFONT_S : {       /* <B> */

        if ( t ) TYPfont = STDfont = t->hfont;

      }; break;

      case FONT_S : {  /* <FONT> */

        if ( t ) {
          TYPfont = t->hfont;
          TYPfcolor = t->hfcolor;
        };

      }; break;

      case S_S :        /* <S> */
      case STRIKE_S :   /* <STRIKE> */
      case U_S      :   /* <U> */

        if ( t ) TYPstyle = t->hstyle;

      break;


    /* ANCHOR COMMANDS */

      case A_S : {       /* <B> */

        if ( t ) {

          TYPreference = t->hreference;
          TYPanchorname = t->hanchorname;
          TYPfont = t->hfont;
          TYPfcolor = t->hfcolor;

        };

      }; break;

    /* LISTS COMMANDS */

      case DL_S  :      /* <DL> */
      case OL_S  :      /* <OL> */
      case UL_S  :      /* <UL> */
      case MENU_S :     /* <MENU> */
      case DIR_S : {     /* <DIR> */

        int subs = html_how_many_dir_sub_commands(engine);

        if ( t ) {
          if ( type == OL_S ) TYPnumber = t->hnumber;

          TYPlisttype = t->hlisttype;
        };

        if ( !subs ) {

          ENGlinespace = HTML_SPECIAL_SPACE;

          html_nextline(engine); /* enter after it */

        } else {

          if ( t ) ENGframe = TYPframe = t->hframe;

          ENGenter_unlock;

          html_set_frame(engine);

        };

      };


      case DD_S :   /* <DD> */
      case DT_S :   /* <DT> */
      case LI_S : { /* <LI> */

        if ( t ) {

          if ( type != DT_S ) TYPframe = t->hframe;

        };

        ENGenter_unlock;

        html_set_frame(engine);

      }; break;

      case TH_S :    /* <TH> */
      case TD_S : {  /* <TD> */

        alignset = 1;

      }; break;

      case TABLE_S : {  /* <TABLE> */

        if ( t ) {

          TYPframe = t->hframe;

          html_set_frame(engine);

          alignset = 1;

          TYPtdstyle = t->htdstyle;
          TYPtdframe = t->htdframe;
          TYPtdnumber = t->htdnumber;
          TYPtrnumber = t->htrnumber;
          TYPcolmin = t->hcolmin;
          TYPcolmax = t->hcolmax;
          TYPcolwidth = t->hcolwidth;
          TYPbcolor = t->hbcolor;
          TYPtdcols = t->htdcols;
          TYPtdcellspacing = t->htdcellspacing;
          TYPtdcellpadding = t->htdcellpadding;
          TYPtdborder = t->htdborder;

          html_set_alignment(engine, ENGline);

          ENGrect.hbottom = ENGrect.htop = html_get_max_y(obj);

          ENGline++;
          ENGendenter = 0;

//          html_nextline_bylock(engine, 0);
        };

      }; break;

    };

    if ( t ) {

      TYPfont  = t->hfont;

      if ( alignset )

          TYPalign = t->halign;

          if ( alignset == 2 ) ENGlinealign = TYPalign;
          else ENGlinealign = 0;

    };

    ENGid = P_S;

  };


};


/*

*/
char *html_translate_end_commander ( HTML_DEF_ENGINE, char *text, int *type )
{
  char *param = NULL;
  long  size  = 0;
  int command = html_find_commander(text, &param, &size);
  int lasttype = -1;
  html_obs *o = NULL;

  if ( type ) *(type) = command;

  if ( command != -1 ) {

    html_type *t = html_find_last_type(engine, command, &lasttype, &o);

    if ( command == -1 ) html_err_func("End Function is not supported\n");

    if ( lasttype != -1 ) {

       html_end_settings(engine, command, t, o);

    } else html_err_func("HTML tag %s hasn't start", HTMLCOMMANDS[command]);

  } else html_err_func("Function is not supported");

  _free(param);
  return (text+size);
};


#include"htmltran.c"


char *html_find_ptr_of_end ( char *text, char *st, char *en, int *i )
{
  _while ( (*i) >= 0 && text ) {

  char *begin = stristr(text, st); /* looking for start command */
  char *end   = stristr(text, en); /* looking for end command */
  char *notb  = strstr(text, "<!--"); /* looking for disable start command */
  char *h = NULL;

  if ( notb <= begin && notb ) h = notb; else h = begin;
  if ( end <= h && end ) h = end; else if ( !h ) h = end;

  if ( !h ) {

    (*i) = 0;
    return NULL;

  } else

  if ( h == notb ) { /* <!-- command found first */

    int v = 1;

    text = html_find_ptr_of_end(h+4, "<!--", "-->", &v);

    if ( text ) text = text+3;

  } else

  if ( h == begin ) {

    (*i)++;

    text = html_find_ptr_of_end(h+strlen(st), st, en, i);

    if ( !*(i) && text ) text = text+strlen(en);

    if ( !(*i) ) return text;

  } else

  if ( h == end ) {

    (*i)--;

    if ( *(i) > 0 ) text = h+strlen(en);

    else text = h;

    return text;

  };

  };

  return NULL;

};



/*
  long html_file_translate ( HTML_DEF_ENGINE, char *text )

  - main function for html translating.

  Argumets:

  HTML_DEF_ENGINE is engine you have got from html_init_engine function
  text - is text you have got from html_file_open function
*/

long  html_file_translate ( HTML_DEF_ENGINE, char *text, int st )
{

  int id = 1;
  int o  = 0;
  int idstart = 1;
  char *v = text;
  char *orig = NULL;

  if ( !ENGoriginalpos ) ENGoriginalpos = text;

  orig = ENGoriginalpos;

  if ( v ) { /* if text exist */

  do {

    char *old = v;

    if ( id == HTML_TRAN_ACCEPT_SPACECHARS ) {

      if ( o ) id = -3; else id = -1;

    }
    else
    if ( id == HTML_TRAN_IGNORE_SPACECHARS ) id = 1;

    if ( id == HTML_TRAN_XMP ) {

      int i = 1;

      if ( o ) id = -3; else id = -1;

      v = html_find_ptr_of_end(v, "<XMP>", "</XMP>", &i);

    } else {

      v = strstr(v, BEGIN_S); /* find '<' character */

    };

    if ( v != old )

      v = html_continue(engine, old, strsize(old, v), id); /* continue by previous
                            specification to position of this character */

    if ( !v || !(*v) ) {

       ENTER_REAL();

       return strsize(text, v);

    };

    o  = 0;
    id = TYPignorespaces;

    if ( strstr(v, END_S) == v ) { /* it's "</" end symbol */

      int what = -1;

      v  = html_translate_end_commander(engine, v+2, &what);
      id = TYPignorespaces;

      if ( what == st && what != -1 ) idstart--;

      if ( idstart <= 0 ) return strsize(text, v);

    } else if ( v ) { /* NO, it's start "<" symbol */

      ENGoriginalpos = orig+strsize(text, v+1);

      v = html_translate_commander(engine, v+1, &id);

      if ( id == st && id != -1 ) idstart++;

      o = 1;

    };

  } _while ( v && *v );

    ENTER_REAL();

  };

  return strsize(text, v);

};



/*
  html_init_engine

  - init engine of html and return pointer to html_engine strcuture

  Arguments : left, top, right, bottom are used as corners of window, where
              you want to output html file.
*/

int i = 0;

HTML_ENGINE  *html_init_engine_ex ( HTML_ENGINE *engfirst, char *op, long left, long top, long right, long bottom )
{

  HTML_ENGINE *engine = (HTML_ENGINE*)_malloc(sizeof(HTML_ENGINE));

  if ( engine ) {

    memset(engine, 0, sizeof(HTML_ENGINE));

    TYPignorespaces = HTML_TRAN_IGNORE_SPACECHARS;
    TYPfont = STDfont = HTML_FONT_3;
    TYPfcolor = STDfcolor = HTML_FUNC_COLOR_MAKERGB(0, 0, 0);
    TYPbcolor = STDbcolor = HTML_FUNC_COLOR_MAKERGB(255, 255, 255);
    TYPacolor = STDacolor = HTML_FUNC_COLOR_MAKERGB(0, 0, 128);
    TYPalign = STDalign = ENGlinealign = HTML_ALIGN_LEFT;

    ENGframe = TYPframe = STDframe = html_rect_assign(left, top, right, bottom);

    ENGenter_lock;
    ENGendenter = 1;
    ENGrect = html_rect_assign(left, top, right, top);
    ENGlinespace = HTML_LINE_SPACE;
    ENGoriginalpos = op;

    if ( !engfirst ) ((*engine).hfirstengine) = engine;
    else ((*engine).hfirstengine) = engfirst;

    ENGtab_blockquote = ENGfirstengine->htab_blockquote;
    ENGtab_list = ENGfirstengine->htab_list;

    ENGtitle = NULL;

  };

  return engine;

};


HTML_ENGINE  *html_init_engine ( long left, long top, long right, long bottom )
{

  HTML_ENGINE *engine = html_init_engine_ex ( NULL, NULL, left, top, right, bottom );

  if ( engine ) {

    ENGtab_blockquote = 5;
    ENGtab_list = 4;

  };

  return engine;

};


/*
  html_done_engine

  - done engine and free memory

*/

HTML_ENGINE  *html_done_engine ( HTML_DEF_ENGINE )
{

  if ( engine ) {

     html_objects_del(&ENGmain);
     html_float_del(ENGfloats);

     if ( engine == ENGfirstengine ) {

        html_obj_del_downloads(&ENGdownloads);
        html_safe_table_del(ENGsafetable);
        html_safe_td_del(ENGsafetd);

        _free(ENGpath);
        _free(ENGtitle);

     };

     _free(TYPreference);
     _free(TYPanchorname);

     if ( ENGpattern )

       HTML_FUNC_BITMAP_DESTROY(ENGpattern);

     _free(engine);

  };

  return NULL;

};


/*
  - open file "filename" and return string of text of this file
  - if error occurs it return NULL
*/
char *html_file_open ( HTML_DEF_ENGINE, char *filename )
{
   FILE *f = fopen(filename, "rt"); /* open file "filename" */
   if ( f ) {
     long size = filelength(fileno(f)); /* get size of file */
     ENGpath = html_file2path(filename);
     if ( size > 0 ) {
       char *buf = (char *)_malloc(size+1); /* alloc memory by specific size */
       if ( buf ) {
         memset(buf, 0, size+1);
         fread(buf, size, 1, f); /* read file to memory */
       };
       fclose(f);
       return buf; /* return ptr to string of file "filename" */
     };
   };
   fclose(f);
   return NULL;
};


void print_engine ( HTML_DEF_ENGINE ) {

  int oldy = 0;
  int oldx = 0;
  html_obs *o = ENGmain;

  _while ( o ) {
    if ( o->hview && o->hobjtype && !html_rect_equals(o->hobjtype->hrect, HTML_RECT_EMPTY))
    {
      int dy = o->hobjtype->hrect.htop-oldy;
      oldy = o->hobjtype->hrect.htop;
      _while ( dy > 0 ) {
        fprintf(TEST, "\n");
        oldx = 0;
        dy--;
      };
      if ( (ID_BLOCKLEVEL(o->hid) || ID_LISTS(o->hid) || o->hid == TEXT_S) && o->hobjtype->htext ) {
        int dx = o->hobjtype->hrect.hleft-oldx;
        oldx = o->hobjtype->hrect.hright;
        fprintf(TEST, "[%i]", o->hobjtype->hrect.hright);

        if ( o->hid == LI_S ) dx -= 2;
        _while ( dx > 0 ) {
          fwrite(" ", 1, 1, TEST);
          dx--;
        };
        if ( o->hid == LI_S ) {
          if ( ((html_blocklevel*)o->hobjtype)->hlisttype == HTML_LISTTYPE_DISC )
            fprintf(TEST, "o ");
          else
          if ( ((html_blocklevel*)o->hobjtype)->hlisttype == HTML_LISTTYPE_CIRCLE )
            fprintf(TEST, "o ");
          else
          if ( ((html_blocklevel*)o->hobjtype)->hlisttype == HTML_LISTTYPE_SQUARE )
            fprintf(TEST, "o ");
          else
          if ( ((html_blocklevel*)o->hobjtype)->hlisttype == HTML_LISTTYPE_1 )
            fprintf(TEST, "%i ", ((html_blocklevel*)o->hobjtype)->hnumber);
          else
          if ( ((html_blocklevel*)o->hobjtype)->hlisttype == HTML_LISTTYPE_A )
            fprintf(TEST, "%c ", 'A'+((html_blocklevel*)o->hobjtype)->hnumber);
          else
          if ( ((html_blocklevel*)o->hobjtype)->hlisttype == HTML_LISTTYPE_a )
            fprintf(TEST, "%c ", 'a'+((html_blocklevel*)o->hobjtype)->hnumber);
          else
          if ( ((html_blocklevel*)o->hobjtype)->hlisttype == HTML_LISTTYPE_I )
            fprintf(TEST, "%c ", 'I'+((html_blocklevel*)o->hobjtype)->hnumber);
          else
          if ( ((html_blocklevel*)o->hobjtype)->hlisttype == HTML_LISTTYPE_i )
            fprintf(TEST, "%c ", 'i'+((html_blocklevel*)o->hobjtype)->hnumber);
        };

        fprintf(TEST, "%s", o->hobjtype->htext);
      };
    };

    o = o->hnext;
  };

  fprintf(TEST, "\n::%i::\n", html_get_max_width(engine));
};


#ifdef 1

int main ( void ) {

  int oldy = 0;
  int oldx = 0;
  html_obs *o;
  HTML_ENGINE *engine = html_init_engine(0, 0, 80, 24);
  char *txt = html_file_open(engine, "hello.htm");
  char *t = txt;
  int h = 0;
  TEST = fopen("html.tst", "wt");
  clrscr();


  _while ( t ) {
    t = stristr(t, "<TABLE");
    if ( t ) {
      h++;
      t++;
    };
  };

//  html_file_translate(engine, txt);
  o = ENGmain;
  _while ( o ) {

    if ( o->hobjtype ) {

       fprintf(TEST, "\n%s : %i, %i, %i, %i", HTMLid(o->hidin),
                     o->hobjtype->hrect.hleft, o->hobjtype->hrect.htop,
                     o->hobjtype->hrect.hright, o->hobjtype->hrect.hbottom);
    };

    o = o->hnext;
  };

  print_engine ( engine );

  html_done_engine(engine);

  printf("\n\nMax tables = %i", h);

  fclose(TEST);
  _free(txt);
  return 0;
};

#endif

