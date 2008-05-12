/****************************************************************/
/*                                                              */
/*                          filter.h                            */
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

#ifndef __FILTER_H_INCLUDED__
#define __FILTER_H_INCLUDED__


/*
 Seal can filter from one data to the other one by filter functions. You can add your
 own function to the list of filters. This allow better DRAG & DROP functions, because
 t_object.get_data and t_object_set_data functions support this filters. So whenever
 one object need number and other one has this number in text format, it finds the
 filter for converting and convert it to the right form.
*/

/*
  data types, you can convert one to another. Can use combination.
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


#ifdef 1
#define  DAT_NONE      0x00000000  /* none data type */
#define  DAT_TEXT      0x00000001  /* single text ended by zero */
#define  DAT_IMAGE     0x00000002  /* type of BITMAP */
#define  DAT_TFILE     0x00000004  /* file t_file structure */
#define  DAT_LIST      0x00000008  /* list ( p_list) */
#define  DAT_ALLKNOW   0x00000020  /* all data types are accepted */
#define  DAT_NUMBER    0x00000040  /* type is the number */
#define  DAT_FLOAT     0x00000080  /* type is the number with floating point */
#define  DAT_HTML      0x00000100  /* type is the HTML format */
#endif


typedef struct t_filter *p_filter;

typedef struct t_filter {

  l_tag    tag_to;    /* it can convert to this type */
  l_tag    tag_from;  /* it can convert from this type */

  /* function that convert (ptr) from (tag_from) to (tag_to) and return the result */
  l_ptr    (*filter) ( l_ptr ptr, l_tag tag_to, l_tag tag_from );

  l_char   reserved[32];

  /* next in the queue */
  p_filter next;

} t_filter;

/* contains information about all filters that use Seal */

extern p_filter filter;



/*
 this function is used, whenever some data from the object comes to the other one.
 it filters data form (ptr), (tag_from) to (tag_to). returns converted value or
 data.

 example :

 l_ptr x = filter(text, TAG_NUMBER, TAG_STRING);

 (x) contains the number ( in l_tag format ).
*/
l_ptr  dat_filter ( l_ptr ptr, l_tag tag_to, l_tag tag_from );


/* insert new filter function to (filter array). This filter can convert from types
   tag_from to tag_to.
*/
void   filter_insert ( l_tag tag_to, l_tag tag_from, l_ptr (*filter_func)(l_ptr ptr, l_tag tag_to, l_tag tag_from) );


/* remove inserted filter function from filter' array.
*/
void   filter_remove ( l_tag tag_to, l_tag tag_from );


/* remove all inserted filters from filter' array.
*/
void   filter_remove_all ( void );


/* end of filter.h file */
#endif

