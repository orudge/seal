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

#include <stdio.h>
#include <allegro.h>
#include <allegro/internal/aintern.h>

static void *load_file_object(PACKFILE *f, long size);
static void *load_object(PACKFILE *f, int type, long size);
static void *load_data_object(PACKFILE *f, long size);
static void *read_block(PACKFILE *f, int size, int alloc_size);

extern int _packfile_type;

#define N            4096           /* 4k buffers for LZ compression */
#define F            18             /* upper limit for LZ match length */
typedef struct UNPACK_DATA          /* for reading LZ files */
{
   int state;                       /* where have we got to? */
   int i, j, k, r, c;
   int flags;
   unsigned char text_buf[N+F-1];   /* ring buffer, with F-1 extra bytes for string comparison */
} UNPACK_DATA;

/* my_load_datafile:
 * Loads an entire data file into memory, and returns a pointer to it. 
 * On error, sets errno and returns NULL.
 * The following was divined by perusing the runes in the allegro library datafile.c and file.c 
 * It allows loading from an open FILE * rather than a filename.  FILE *f assumed to be positioned at the
 * appended data upon entry, it is still open on exit.
 * If future versions of allegro change the load_datafile routine, this will have to be modified to
 * account for it - good luck. 
 */
void *my_load_datafile(FILE *fp,long len)
{
   DATAFILE *dat=NULL;
   int type;
   PACKFILE *f;
   _packfile_type = DAT_MAGIC;
   errno = *allegro_errno = 0;
   if ((f = malloc(sizeof(PACKFILE))) == NULL) {
      *allegro_errno = ENOMEM;
      return NULL;
   }
   f->buf_pos = f->buf;
   f->flags = 0;
   f->flags &= ~PACKFILE_FLAG_WRITE;
   f->buf_size = 0;
   f->filename = NULL;
   f->passdata = NULL;
   f->passpos = NULL;
   f->parent = NULL;
   f->pack_data = NULL;
   f->passpos = f->passdata;
   f->todo=len;
   f->hndl=fileno(fp); // _al_open (somehow ends up) same as open() so we can use the file number
   /* that is essentially what pack_fopen does for a readonly file */

   lseek(f->hndl,ftell(fp),SEEK_SET); 
   /* ensures the unbuffered file position is set correctly so allegro internal buffering works */

   /* dlxinsd simply appends standard allegro data files onto the file after its marker,
      we have to figure out the first bit ourselves because this is what read_datafile does prior to 
      calling load_file_object()
   */
   type = pack_mgetl(f);

   if (type == F_PACK_MAGIC) {  /* read a packed file */
      PACKFILE *f1;
      UNPACK_DATA *dat1 = malloc(sizeof(UNPACK_DATA));
      int c;
      if (!dat1) {
            *allegro_errno = ENOMEM;
            free(f);
            return NULL;
      }
      if ((f1 = malloc(sizeof(PACKFILE))) == NULL) {
         *allegro_errno = ENOMEM;
         free(f);
         return NULL;
      }
      f1->buf_pos = f1->buf;
      f1->flags = 0;
      f1->flags &= ~PACKFILE_FLAG_WRITE;
      f1->buf_size = 0;
      f1->filename = NULL;
      f1->passdata = NULL;
      f1->passpos = NULL;
      f1->parent = f;
      f1->pack_data = NULL;
      f1->passpos = f->passdata;
      f1->hndl=fileno(fp); // _al_open (somehow ends up) same as open() so we can use the file number
      for (c=0; c < N - F; c++)
               dat1->text_buf[c] = 0; 
      dat1->state = 0;
      f1->todo = LONG_MAX;
      f1->pack_data = (char *)dat1;
      f1->flags |= PACKFILE_FLAG_PACK;
      type = pack_mgetl(f1);
      if ( type != DAT_MAGIC ) {
            if (*allegro_errno == 0)
               *allegro_errno = EDOM;
            free(f1);free(f);
            return NULL;
      }
      dat = load_file_object(f1, LONG_MAX); 
      if (f1->passdata)
	  free(f1->passdata);
      if (f1->pack_data)
	  free(f1->pack_data);
      free(f1);
   } else {
      type = pack_mgetl(f);
      if ( type != DAT_MAGIC ) {
            if (*allegro_errno == 0)
               *allegro_errno = EDOM;
            return NULL;
      } /* we expect this next */
      /* load_file_object might use a callback (its address is internal to file.c sadly).
       It should be null when we call it though because it gets reset after use. 
      */
      dat = load_file_object(f, len); 
   }
   // Now clean up the way pack_fclose does  - without closing the file though.
   if (f->passdata)
	 free(f->passdata);
   if (f->pack_data)
	 free(f->pack_data);
   free(f);
   return dat; 
}

// OCR - 29/03/2002 - From here to end of file taken from Allegro 4.0.1 packfile.c

/* load_file_object:
 *  Loads a datafile object.
 */
static void *load_file_object(PACKFILE *f, long size)
{
   #define MAX_PROPERTIES  64

   DATAFILE_PROPERTY prop[MAX_PROPERTIES];
   DATAFILE *dat;
   PACKFILE *ff;
   int prop_count, count, type, c, d;
   char *p;

   count = pack_mgetl(f);

   dat = malloc(sizeof(DATAFILE)*(count+1));
   if (!dat) {
      *allegro_errno = ENOMEM;
      return NULL;
   }

   for (c=0; c<=count; c++) {
      dat[c].type = DAT_END;
      dat[c].dat = NULL;
      dat[c].size = 0;
      dat[c].prop = NULL;
   }

   for (c=0; c<MAX_PROPERTIES; c++)
      prop[c].dat = NULL;

   c = 0;
   prop_count = 0;

   while (c < count) {
      type = pack_mgetl(f);

      if (type == DAT_PROPERTY) {
	 /* load an object property */
	 type = pack_mgetl(f);
	 d = pack_mgetl(f);
	 if (prop_count < MAX_PROPERTIES) {
	    prop[prop_count].type = type;
	    prop[prop_count].dat = malloc(d+1);
	    if (prop[prop_count].dat != NULL) {
	       pack_fread(prop[prop_count].dat, d, f);
	       prop[prop_count].dat[d] = 0;
	       if (need_uconvert(prop[prop_count].dat, U_UTF8, U_CURRENT)) {
		  p = malloc(uconvert_size(prop[prop_count].dat, U_UTF8, U_CURRENT));
		  if (p)
		     do_uconvert(prop[prop_count].dat, U_UTF8, p, U_CURRENT, -1);
		  free(prop[prop_count].dat);
		  prop[prop_count].dat = p;
	       }
	       prop_count++;
	       d = 0;
	    }
	 }
	 while (d-- > 0)
	    pack_getc(f);
      }
      else {
	 /* load actual data */
	 ff = pack_fopen_chunk(f, FALSE);

	 if (ff) {
	    d = ff->todo;

	    dat[c].dat = load_object(ff, type, d);

	    if (dat[c].dat) {
	       dat[c].type = type;
	       dat[c].size = d;

	       if (prop_count > 0) {
		  dat[c].prop = malloc(sizeof(DATAFILE_PROPERTY)*(prop_count+1));
		  if (dat[c].prop != NULL) {
		     for (d=0; d<prop_count; d++) {
			dat[c].prop[d].dat = prop[d].dat;
			dat[c].prop[d].type = prop[d].type;
			prop[d].dat = NULL;
		     }
		     dat[c].prop[d].dat = NULL;
		     dat[c].prop[d].type = DAT_END;
		  }
		  else {
		     for (d=0; d<prop_count; d++) {
			free(prop[d].dat);
			prop[d].dat = NULL;
		     }
		  }
		  prop_count = 0;
	       }
	       else
		  dat[c].prop = NULL;
	    }

	    f = pack_fclose_chunk(ff);

/*	    if (datafile_callback)
	       datafile_callback(dat+c);*/

	    c++;
	 }
      }

      if (*allegro_errno) {
	 unload_datafile(dat);

	 for (c=0; c<MAX_PROPERTIES; c++)
	    if (prop[c].dat)
	       free(prop[c].dat);

	 return NULL;
      }
   }

   for (c=0; c<MAX_PROPERTIES; c++)
      if (prop[c].dat)
	 free(prop[c].dat);

   return dat;
}

/* load_object:
 *  Helper to load an object from a datafile.
 */
static void *load_object(PACKFILE *f, int type, long size)
{
   int i;

   /* look for a load function */
   for (i=0; i<MAX_DATAFILE_TYPES; i++)
      if (_datafile_type[i].type == type)
	 return _datafile_type[i].load(f, size);

   /* if not found, load binary data */
   return load_data_object(f, size);
}

/* load_data_object:
 *  Loads a binary data object from a datafile.
 */
static void *load_data_object(PACKFILE *f, long size)
{
   return read_block(f, size, 0);
}

/* read_block:
 *  Reads a block of size bytes from a file, allocating memory to store it.
 */
static void *read_block(PACKFILE *f, int size, int alloc_size)
{
   void *p;

   p = malloc(MAX(size, alloc_size));
   if (!p) {
      *allegro_errno = ENOMEM;
      return NULL;
   }

   pack_fread(p, size, f);

   if (pack_ferror(f)) {
      free(p);
      return NULL;
   }

   return p;
}