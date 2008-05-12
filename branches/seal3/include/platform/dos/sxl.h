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

/*
 * SEAL 3 Dynamic Linking System version 1.0.1
 * Header file that will be used from the host and the libraries
 * Copyright (C) 2002 Kostas Michalopoulos aka Bad Sector
 *
 * Revision History:
 *
 * 09/06/2002: First release - 1.0.1 (Bad Sector)
*/

#ifndef __SXL_H_INCLUDED__
#define __SXL_H_INCLUDED__

#ifdef __cplusplus
   extern "C" {
#endif

/* SXL Error Codes (returned in last_sxl_error) */
#define SXLERROR_NOERROR            0
#define SXLERROR_FILENOTFOUND       1
#define SXLERROR_NOTENOUGHMEMORY    2
#define SXLERROR_UNRESOLVEDEXTERNAL 3
#define SXLERROR_LIBRARYNOTFOUND    4
#define SXLERROR_SYMBOLNOTFOUND     5
/* note: SXLERROR_UNRESOLVEDEXTERNAL fills last_sxl_error_data */

/* This defines an invalid SXL handle */
#define SXL_INVALIDHANDLE           0

/* Contains error value - filled after every SXL function call */
extern unsigned char            last_sxl_error;

/* Contains optional information about the last error */
extern void                     *last_sxl_error_data;

/* SXL Library Handle - after every library load, you get one of these */
typedef unsigned long long      h_sxl;

/* Guess what! This function initializes the SXL system. It MUST be called
  before anything related to SXL, and must be called only ONE TIME */
void initialize_sxl_system();

/* This function is called both by libraries and hosting programs in order
  to register a symbol (registering a symbol makes it available to all
  libraries). `handle' is the library handle, `name' is the symbol's name
  and `addr' is the symbol's address. */
void register_symbol(h_sxl handle, char *name, void *addr);

/* Use these macros to quick-register symbols */
#define SXL_REGISTER(x)             register_symbol(handle, "_"#x, (void *) &x)
#define SXL_REGISTER_AS(x,y)        register_symbol(handle, "_"#y, (void *) &x)

/* Instead of REGISTER and REGISTER_AS, hosting programs must use these
  macros: */
#define SXL_HOST_REGISTER(x)        register_symbol(0, "_"#x, (void *) &x)
#define SXL_HOST_REGISTER_AS(x,y)   register_symbol(0, "_"#y, (void *) &x)

/* This function loads a SXL library and executes it's main() function,
  if is defined. */
h_sxl sxl_load(char *fname, char *args);

/* This function is the same as sxl_load, but before executes 'main()' it
 fills 'handle' with the library's handle */
h_sxl sxl_load_ex(char *fname, char *args, h_sxl *handle);

/* This function is called only by libraries in order to export a symbol.
  The exported symbol is not available globaly, but must be retrieved using
  the `get_symbol' function. `handle' is the library's handle, `name' is the
  symbol's name and `addr' is the symbol's address */
int export_symbol(h_sxl handle, char *name, void *addr);

/* Use these macros to quick-export symbols */
#define SXL_EXPORT(x)       export_symbol(handle, "_"#x, (void *) &x)
#define SXL_EXPORT_AS(x,y)  export_symbol(handle, "_"#y, (void *) &x)

/* This function returns the address of given symbol that is exported by
  the given library, or NULL if the symbol was not found. `handle' is
  library's handle and `name' is the symbol's name. */
void *get_symbol(h_sxl handle, char *name);

/* This function will free the given library and un-register all symbols that
  have been registered by it. `handle' is library's handle. */
void sxl_free(h_sxl handle);

/* This function frees all loaded libraries. */
void free_all_libraries();

#ifndef ___SXL_HOST___
/* When sxl.h is included from a library, it will be changed to library's
  handle, after library loads and before the main() function is called. When
  sxl.h is included from the hosting program, is always 0. */
  
h_sxl           sxl_handle = 0;
#endif

#ifdef __cplusplus
   }
#endif


#endif

