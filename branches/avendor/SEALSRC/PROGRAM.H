/****************************************************************/
/*                                                              */
/*                          program.h                           */
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



#ifndef __PROGRAM_H_INCLUDED__
#define __PROGRAM_H_INCLUDED__

#define INI_DESKTOP         "desktop"
#define INI_OPTIMALIZATION  "optimalization"

#define SET_DESKTOP      "desktop.set"
#define SET_APPLICATIONS "app.set"

/* the difference between ticks */
extern l_int  task_tick;
/* the main program object */
extern t_object   program;
/* list of objects that use OB_OF_STILLPROCESS option flag */
extern t_list     stillprocess_list;
/* drivers, the main object for drivers, such as keyboard, mouse, ... */
extern p_drivers  drivers;
/* destkop, the main object of visible objects */
extern p_view     desktop;
/* define if clock-cursor must be included */
extern l_int      clock_including;

int  program_put_into_stillprocess ( p_object o, p_object s );
int  program_clear_from_stillprocess ( p_object o, p_object s );
void program_put_event ( t_object *o, t_event *event );
void program_get_event ( t_object *o, t_event *event );

void   program_translate_event ( t_object *o, t_event *event );
l_bool program_done ( t_object *o );

t_object *program_init ( t_object *o );
void      program_call_each_stillprocess ( p_list o );

#define   program_shutdown()   do {                                            \
                set_event(&event_main, EV_MESSAGE, MSG_QUIT, OBJECT(&program));\
                OBJECT(&program)->put_event(OBJECT(&program), &event_main);    \
          } while (0)

#ifdef    ENABLE_MULTITASKER
l_bool    __multi_process = true;
#define   HALT_MULTITASKER() __multi_process=false
#define   while(t1)      _while((__multi_process)&&(##t1))
#endif

#endif

