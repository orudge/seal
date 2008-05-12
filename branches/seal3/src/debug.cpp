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

#include <stdio.h>
#include <stdarg.h>

void debug::message(l_text format, ...)
{                                 
   va_list argptr;

   va_start(argptr, format);

   if (log)
   {
      fprintf(log, "Message: ");
      vfprintf(log, format, argptr);
      fprintf(log,"\n");

      if (force_flush)
      {
         fflush(log);
      }
   }

   va_end(argptr);
}

void debug::warning(l_text format, ...)
{                                 
   va_list argptr;

   va_start(argptr, format);

   if (log)
   {
      fprintf(log, "Warning: ");
      vfprintf(log, format, argptr);
      fprintf(log,"\n");

      if (force_flush)
      {
         fflush(log);
      }
   }

   va_end(argptr);
}

void debug::error(l_text format, ...)
{                                 
   va_list argptr;

   va_start(argptr, format);

   if (log)
   {
      fprintf(log, "ERROR: ");
      vfprintf(log, format, argptr);
      fprintf(log,"\n");

      if (force_flush)
      {
         fflush(log);
      }
   }

   va_end(argptr);
}

void debug::fatal(l_text format, ...)
{                                 
   va_list argptr;

   va_start(argptr, format);

   if (log)
   {
      fprintf(log, "FATAL ERROR: ")	;
      vfprintf(log, format, argptr);
      fprintf(log,"\n");

      if (force_flush)
      {
         fflush(log);
      }
   }

   va_end(argptr);

   fprintf(log, "\n\nA fatal error has occurred. Aborting program execution.\n");
   abort();
}

debug::debug(l_text filename, l_bool force_flush)
{
   log = NULL;

   this->force_flush = force_flush;  // Force the file to be written to disk after every message?

   if (filename == NULL)
      log = NULL; // No log
   else if (strcmp(filename, "stdout") == 0)
      log = stdout;
   else if (strcmp(filename, "stderr") == 0)
      log = stderr;
   else
   {
      log = fopen(filename, "wt");

	if (log == NULL)
         fprintf(stderr, "debug::debug: Can't open file '%s' for writing\n", filename);
   }

   message("Starting logging to %s", filename);
}

void debug::close()
{
   message("Logging stopped, closing log file");

   if (log)
   {
      fclose(log);
      log = NULL;
   }
}

debug::~debug()
{         
   if (log)
      close();
}
