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

#include <io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DLX_ID(a,b,c,d)     ((a<<24) | (b<<16) | (c<<8) | d)

#define ID_DLXDATAFILE DLX_ID('0','T','A','D')

int main(int argc, char **argv)
{
   FILE *input;
   FILE *output;
   long  ilen;
/* UNUSED
   long  olen;
*/
   long  id = ID_DLXDATAFILE;
   char *data;

   if (argc < 3)
   {
      printf("\nUsage: dlxinsd output.xdl input.dat\n");
      return 1;
   }

   output = fopen(argv[1], "ab");
   input = fopen(argv[2], "rb");

   ilen = filelength(fileno(input));
   data = (char*)malloc(ilen);

   if (!data)
   {
      printf("\nError: Not enough memory\n");
      fclose(input);
      fclose(output);
      return 2;
   }

   fread(data, ilen, 1, input);
   fclose(input);
   fwrite(data, ilen, 1, output);
   fwrite(&ilen, sizeof(long), 1, output);
   fwrite(&id, sizeof(long), 1, output);
   fclose(output);

   free(data);
   return 0;
}

