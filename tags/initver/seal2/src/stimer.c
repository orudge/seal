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
#include"stimer.h"

p_timer_def safe_timers;

void  safe_timer ( void (*stop)(), void (*reload)() )
{
   p_timer_def x = safe_timers;
   /* search for timer */
   while ( x && (x->stop != stop) ) x = x->next;
   if ( !x ) { /* not found, so insert */
       p_timer_def x = (p_timer_def)_malloc(sizeof(t_timer_def));
       if ( x ) /* enough memory */
          clear_type(x, sizeof(t_timer_def)); /* all to ZERO */
          x->stop = stop;
          x->reload = reload;
          x->active = 1;
          x->next = safe_timers;
          safe_timers = x;
   };
};


/* erase previous saved timer */
void  erase_safe_timer ( void (*stop)() )
{
   p_timer_def x = safe_timers;
   p_timer_def last = NULL;
   /* search for timer */
   while ( x ) {
      p_timer_def s = x->next;
      if ( !last ) { /* first item */
         safe_timers = x->next;
         _free(x);
         return;
      } else { /*  */
          last->next = x->next;
          _free(x);
          return;
      };
      last = x;
      x = s;
   };
};


void safe_timer_halt ( void )
{
   p_timer_def x = safe_timers;
   /* for all timers */
   while ( x ) {
      /* it's able to stop it */
      if ( x->active ) {
            x->stop(); /* stop timer */
            x->active = 0; /* set to passive timer */
      };
      /* go to the next one */
      x = x->next;
   };
};


void safe_timer_reload ( void )
{
   p_timer_def x = safe_timers;
   /* for all timers */
   while ( x ) {
      /* it's able to reload it */
      if ( !x->active ) {
            x->reload(); /* stop timer */
            x->active = 1; /* set to passive timer */
      };
      /* go to the next one */
      x = x->next;
   };
};


void safe_timer_free_all ( void )
{
   p_timer_def x = safe_timers;
   /* for all timers */
   while ( x ) {
      p_timer_def n = x->next;
      /* free memory */
      _free(x);
      /* go to the next one */
      x = n;
   };
   safe_timers = NULL;
};

