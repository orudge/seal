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

#ifndef _SEAL_SAFE_TIMER_H_
#define _SEAL_SAFE_TIMER_H_

typedef struct t_timer_def *p_timer_def;

typedef struct t_timer_def {

  l_int   active; /* flag if the timer is active */

  void    (*stop) (); /* stop the timer */
  void    (*reload) (); /* reload timer */

  l_char  reserved[24];

  p_timer_def next;

} t_timer_def;

extern p_timer_def safe_timers;

/* safe timer */
void  safe_timer ( void (*stop)(), void (*reload)() );
/* erase previous saved timer */
void  erase_safe_timer ( void (*stop)() );
/* erase all saved timers */
void safe_timer_free_all ( void );
/* reload timers */
void safe_timer_reload ( void );
/* stop timers */
void safe_timer_halt ( void );

#endif
