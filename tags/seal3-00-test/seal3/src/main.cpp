/******************************************************************
 * Seal 3.0                                                       *
 * Copyright (c) 1999-2002 Seal Developers. All Rights Reserved.  *
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
#include <debug.h>
#include <platform/execfile.h>
#include <stdio.h>
#include <conio.h>

debug *debug_log;

int main(int argc, char *argv[])
{
   debug_log = new debug("seal.dbg", true);

   // The following will be in seal_sys_init() - system driver initialization
   debug_log->message("Initializing executable file routines...");
   seal_sys_execfile_init();   
   // End seal_sys_init() routines

   SEAL_KERNEL_EXPORT(debug_log); // there will probably be a function like export_routines() to
                                  // do this

   printf("Welcome to Seal 3.0!\n");
   getch();

   debug_log->message("Terminating executable file routines...");
   seal_sys_execfile_term();

   delete debug_log;

   return(0);
}