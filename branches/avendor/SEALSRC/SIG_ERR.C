/****************************************************************/
/*                                                              */
/*                          sig_err.c                           */
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

#include<signal.h>

/* previous signal handlers */
static void *old_sig_abrt = NULL;
static void *old_sig_fpe = NULL;
static void *old_sig_ill = NULL;
static void *old_sig_segv = NULL;
static void *old_sig_term = NULL;
static void *old_sig_int = NULL;
static void *old_sig_kill = NULL;
static void *old_sig_quit = NULL;


static void signal_error ( int num )
{
   l_int err = seal_error(ERR_NONREGULAR, "%s", "System Error !

If you haven't saved some data,
you can continue, but on your own
risk, Otherwise, please press Exit.

Would you like to continue
or exit system now ?\n");

    /* may crash system */
    if ( err ) {
       /* but I have a chance to close all programs */
       program.execute(&program);

       return;
    };

    /* else shutdown */

    switch (num) {

      case SIGABRT:
	 signal(num, old_sig_abrt);
	 break;

      case SIGFPE:
	 signal(num, old_sig_fpe);
	 break;

      case SIGILL:
	 signal(num, old_sig_ill);
	 break;

      case SIGSEGV:
	 signal(num, old_sig_segv);
	 break;

      case SIGTERM:
	 signal(num, old_sig_term);
	 break;

      case SIGINT:
	 signal(num, old_sig_int);
	 break;

      case SIGKILL:
	 signal(num, old_sig_kill);
	 break;

      case SIGQUIT:
	 signal(num, old_sig_quit);
	 break;

      default:
	 signal(num, SIG_DFL);
	 break;
   }

   raise(num);
};

END_OF_FUNCTION(signal_error)


void signal_init ( void )
{
   /* lock memory of error signal function */
   LOCK_FUNCTION(signal_error);

   /* set signals function */
   old_sig_abrt = signal(SIGABRT, signal_error);
   old_sig_fpe  = signal(SIGFPE,  signal_error);
   old_sig_ill  = signal(SIGILL,  signal_error);
   old_sig_segv = signal(SIGSEGV, signal_error);
   old_sig_term = signal(SIGTERM, signal_error);
   old_sig_int  = signal(SIGINT,  signal_error);
   old_sig_kill = signal(SIGKILL, signal_error);
   old_sig_quit = signal(SIGQUIT, signal_error);
};


void signal_done ( void )
{

};
