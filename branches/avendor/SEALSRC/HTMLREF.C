/****************************************************************/
/*                                                              */
/*                           htmlref.c                          */
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


#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#ifndef strexist
#define strexist(p)     ((p) && *(p))
#endif
#ifndef strsize
#define strsize(p,v)    ((v)?((v)-(p)):strlen(p))
#endif

#define HTML_NON_BREAKING_SPACE 160
#define HTML_QUOT                34
#define HTML_AMP                 38
#define HTML_LT                  60
#define HTML_GT                  62

#define HTML_SPACE_CHAR        32
#define HTML_ENTER_CHAR        10
#define HTML_TAB_CHAR           9


#define HTML_ONE_CHAR(c)       ((c) == HTML_SPACE_CHAR || (c) == HTML_ENTER_CHAR || (c) == HTML_TAB_CHAR)
#define HTML_ONE_CHAR_PLUS(c)  ((c) == HTML_SPACE_CHAR || (c) == HTML_TAB_CHAR)

/*
  if redeclare special characters,
  you must also remake html_find_first_occ function
*/
char*   html_special_characters[256] =
   {
    NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, "quot", NULL, NULL, NULL, "amp", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "lt", NULL, "gt", NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "nbsp", "iexcl", "cent", "pound", "curren", "yen", "brvbar", "sect", "uml", "copy", "ordf", "laquo", "not", "shy", "reg", "macr",
    "deg", "plusmn", "sup2", "sup3", "acute", "micro", "para", "middot", "cedil", "supl", "ordm", "raquo", "frac14", "frac12", "frac34", "iquest",
    "Agrave", "Aacute", "circ", "Atilde", "Auml", "ring", "AElig", "Ccedil", "Egrave", "Eacute", "Ecirc", "Euml", "Igrave", "Iacute", "Icirc", "Iuml",
    "ETH", "Ntilde", "Ograve", "Oacute", "Ocirc", "Otilde", "Ouml", "times", "Oslash", "Ugrave", "Uacute", "Ucirc", "Uuml", "Yacute", "THORN", "szlig",
    "agrave", "aacute", "acirc", "atilde", "auml", "aring", "aelig", "ccedil", "egrave", "eacute", "ecirc", "euml", "igrave", "iacute", "icirc", "iuml",
    "ieth", "ntilde", "ograve", "oacute", "ocirc", "otilde", "ouml", "divide", "oslash", "ugrave", "uacute", "ucirc", "uuml", "yacute", "thorn", "yuml"
  };


int html_find_first_occ ( char *text )
{
  int n = 0;
  int all = 0;
  int i   = 0;

  all = 160; /* from nbsp */

  /* first make most occured texts */
  if ( !strcmp(html_special_characters[HTML_NON_BREAKING_SPACE], text) )

        return HTML_NON_BREAKING_SPACE;

  if ( !strcmp(html_special_characters[HTML_QUOT], text) )

        return HTML_QUOT;

  if ( !strcmp(html_special_characters[HTML_AMP], text) )

        return HTML_AMP;

  if ( !strcmp(html_special_characters[HTML_LT], text) )

        return HTML_LT;

  if ( !strcmp(html_special_characters[HTML_GT], text) )

        return HTML_GT;


  _while ( all < 256 ) {

    if ( html_special_characters[all] &&
         !strcmp(html_special_characters[all], text) ) return all;

    all++;
  };

  return 0;
};



char *html_ref_from_special_characters ( char *text )
{
  long  sz;
  char *p;
  char *sf;

  if ( !text ) return NULL;

  sz = strlen(text)+1;
  p  = (char *)_malloc(sz);
  sf = p;

  if ( p ) {

    memset(p, 0, sz);

    _while ( text && *text ) {

      char   *old = text;
      long  strz;

      text = strchr(text, '&'); /* find type of special characters */

      strz = strsize(old, text);

      strncat(p, old, strz);

      p += strz; /* move p */

      if ( text && *p || strz <= 0 ) {
        char *e = strchr(text+1, ';'); /* find type of end of special character */
        if ( !e ) e = text+(sz-2);
        if ( e ) {
          char *v = strnewn(text+1, strsize(text+1, e));
          if ( strexist(v) ) {
            if ( *v == '#' ) { /* it's in decimal code */
              int chr = atoi(v+1);
              *p = (char)chr;
              p++;
            } else { /* it's in special code */
              *p = (char)html_find_first_occ(v);
              p++;
            };
          };
          text = e+1;
        } else text++;
      } else return sf;
    };

  } else html_err_func("Not enough memory for translating");

  return sf;
};



char *html_ref_from_spaces ( char *text, int firstspace )
{
  long  sz;
  char *p;
  char *sf;

  if ( !text ) return NULL;

  sz = strlen(text)+1;
  p  = (char*)_malloc(sz);
  sf = p;

  if ( p ) {

    char old = firstspace?'a':HTML_SPACE_CHAR;

    memset(p, 0, sz);

    _while ( *text ) {

      if ( !HTML_ONE_CHAR(*text) ) {
        *p = *text;
         p++;
      } else
        if ( !HTML_ONE_CHAR(old) ) {
          *p = HTML_SPACE_CHAR;
           p++;
        };

      old = *text;

      text++;

    };

    if ( !(*sf) ) {
      _free(sf);
      sf = NULL;
    };

  } else html_err_func("Not enough memory for translating");

  return sf;
};



char *html_reformat_text ( char *text, int firstspace )
{
  if ( strexist(text) ) {
    char *v = html_ref_from_spaces(text, firstspace);
    char *p = html_ref_from_special_characters(v);
    _free(v);
    return p;
  };
  return NULL;
};


void  html_reformat_text_from_nbsp ( char *text )
{
  if ( strexist(text) ) {

     _while ( *text ) {

        if ( (unsigned char)(*text) == HTML_NON_BREAKING_SPACE )

           *text = HTML_SPACE_CHAR;

        text++;

     };

  };

};


