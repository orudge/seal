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

/* Filename: execfile.c
 * Purpose:  Executable file routines for DOS. Calls SXL routines
 *           where appropriate.
 * Author:   Owen Rudge
 * Date:     15th June 2002
 */

/* Revision History:
 *
 * 15/06/2002: Created (orudge)
 */

#define ___SXL_HOST___

#include <seal.h>
#include <platform/dos/execfile.h>
#include <platform/dos/libc.h>

void* __builtin_new(size_t mysize);
void __builtin_delete(void* myptr);
void __throw(void*myptr);
void __terminate(void*myptr);
void terminate__Fv();

/* FUNCTION:     seal_sys_execfile_init
 * Parameters:   None
 * Return Value: 1 if successful, 0 otherwise
 * Purpose:      Initializes the executable file routines
 */

l_int seal_sys_execfile_init()
{
   initialize_sxl_system(); // Initialize the SXL system

   #include "libc.exp"  // libc exports
}

/* FUNCTION:     seal_sys_execfile_term
 * Parameters:   None
 * Return Value: 1 if successful, 0 otherwise
 * Purpose:      Terminates the executable file routines
 */

l_int seal_sys_execfile_term()
{
   free_all_libraries(); // Free all loaded libraries
}

p_execfile_hndl seal_sys_execfile_load(l_text filename, l_text arguments)
{
   return((p_execfile_hndl) sxl_load(filename, arguments));
}

l_ptr seal_sys_execfile_find_symbol(p_execfile_hndl handle, l_text name)
{
   return(get_symbol(handle, name));
}

void seal_sys_execfile_unload(p_execfile_hndl handle)
{
   sxl_free(handle);
}