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
#include <filter.h>

/* contains information about all filters that use Seal */
p_filter filter = NULL;


/*
 this function is used, whenever some data from the object comes to the other one.
 it filters data form (ptr), (tag_from) to (tag_to). returns converted value or
 data.
 example :
 l_ptr x = filter(text, DAT_NUMBER, DAT_STRING);
 (x) contains the number ( in l_big format ).
*/
l_ptr  dat_filter ( l_ptr ptr, l_tag tag_to, l_tag tag_from )
{

  /* set filter to the first (filter item) */
  p_filter f = filter;

  /* for all items */
  while ( f )
  {
      /* test if we can convert from one format to another */
      if ( l_tag_cmp(f->tag_to, tag_to) && l_tag_cmp(f->tag_from, tag_from) )
	{
           /* filter */
           return f->filter(ptr, tag_to, tag_from);
	}

      f = f->next;
  };

  /* can't filter from the type ( tag_from ) to type ( tag_to ) */
  return NULL;
};

/* insert new filter function to (filter array). This filter can convert from types
   tag_from to tag_to.
*/
void   filter_insert ( l_tag tag_to, l_tag tag_from, l_ptr (*filter_func)(l_ptr ptr, l_tag tag_to, l_tag tag_from) )
{
  /* allocate memory for new filter item */
  p_filter f = (p_filter)_malloc(sizeof(t_filter));

  /* memory was allocated succesfully ? */
  if ( f )
  {
      /* all memory to ZERO */
      clear_type(f, sizeof(t_filter));
      l_tag_cpy(f->tag_to, tag_to);
      l_tag_cpy(f->tag_from, tag_from);
      f->filter = filter_func;
      f->next = filter;
      filter = f;
  };

};


/* remove inserted filter function from filter' array.
*/
void   filter_remove ( l_tag tag_to, l_tag tag_from )
{

  /* find first item */
  p_filter f = filter;

  p_filter last = NULL;

  if ( f ) /* item exists */
  {
   while ( f ) /* while there are items... */
   {
      /* saves the next filter item */
      p_filter s = f->next;

      /* we want to remove this filter ? */
      if ( (f->tag_to == tag_to) && (f->tag_from == tag_from) ) {

           /* safe the array */
           if ( !last ) /* it's first item */
	     {
                 /* move the first item to next (s = f->next) */
                 filter = s;
	     }
           else
           {
                 /* else next of previous item point to next of this */
                 last->next = f->next;
           }

          /* free memory of item */
          _free(f);

          /* break */
          return;
      };

      last = f;
      f = s;
   };
  };
};


/* remove all inserted filters from filter' array.
*/
void   filter_remove_all ( void )
{
  p_filter f = filter;
  while ( f ) {
     p_filter s = f->next;
     _free(f);
     f = s;
  };
  filter = NULL;
};

#ifndef BUFFER_FORMAT_TEXT
#define BUFFER_FORMAT_TEXT   1024
#endif

#ifndef TEXTOUT_BUFFER_SIZE
#define TEXTOUT_BUFFER_SIZE 1024
#endif

#ifndef FIFO_BUFFER_SIZE
#define FIFO_BUFFER_SIZE   512
#endif



