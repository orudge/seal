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
#include <mouse.h>
#include <grfx-f.h>

////////////////////////////////////////////////////////////////////////////////
// Define windows types for ico reading ...

typedef unsigned short WORD;
typedef unsigned char  BYTE;
typedef long           LONG;
typedef unsigned long  DWORD;

typedef struct tagRGBQUAD {
    BYTE    rgbBlue;
    BYTE    rgbGreen;
    BYTE    rgbRed;
    BYTE    rgbReserved; // Must be 0
} RGBQUAD;

typedef struct tagBITMAPINFOHEADER {
    DWORD   biSize;
    LONG    biWidth;
    LONG    biHeight;
    WORD    biPlanes;
    WORD    biBitCount;
    DWORD   biCompression;
    DWORD   biSizeImage;
    LONG    biXPelsPerMeter;
    LONG    biYPelsPerMeter;
    DWORD   biClrUsed;
    DWORD   biClrImportant;
} BITMAPINFOHEADER;

typedef struct
{
    BYTE        bWidth;          // Width, in pixels, of the image
    BYTE        bHeight;         // Height, in pixels, of the image
    BYTE        bColorCount;     // Number of colors in image (0 if >=8bpp)
    BYTE        bReserved;       // Reserved ( must be 0)
    WORD        wPlanes;         // Color Planes
    WORD        wBitCount;       // Bits per pixel
    DWORD       dwBytesInRes;    // How many bytes in this resource?
    DWORD       dwImageOffset;   // Where in the file is this image?
} ICONDIRENTRY, *LPICONDIRENTRY;

typedef struct
{
    WORD           idReserved;   // Reserved (must be 0)
    WORD           idType;       // Resource Type (1 for icons)
    WORD           idCount;      // How many images?
    ICONDIRENTRY   idEntries[1]; // An entry for each image (idCount of 'em)
} ICONDIR, *LPICONDIR;

/*typedef struct
{
   BITMAPINFOHEADER   icHeader;      // DIB header
   RGBQUAD         icColors[1];   // Color table
   BYTE            icXOR[1];      // DIB bits for XOR mask
   BYTE            icAND[1];      // DIB bits for AND mask
} ICONIMAGE, *LPICONIMAGE; */

typedef struct _CURSORDIRENTRY {
    BYTE  bWidth;
    BYTE  bHeight;
    BYTE  bColorCount;
    BYTE  bReserved;
    WORD  wXHotspot;
    WORD  wYHotspot;
    DWORD lBytesInRes;
    DWORD dwImageOffset;
} CURSORDIRENTRY;

typedef struct _CURSORDIR {
    WORD           cdReserved;
    WORD           cdType;
    WORD           cdCount;
    CURSORDIRENTRY cdEntries[1];
} CURSORDIR;


long mgetl( void *data)
{
   int b1=0, b2=0, b3=0, b4=0;

   memcpy(&b1,data,1);
   memcpy(&b2,data+1,1);
   memcpy(&b3,data+2,1);
   memcpy(&b4,data+3,1);


	 return (((long)b1 << 24) | ((long)b2 << 16) |
		       ((long)b3 << 8) | (long)b4);
}

////////////////////////////////////////////////////////////////////////////////
// Used by load_ico and load_cur
// Load image in 1bpp,4pbb,8bpp,16bpp,24bpp colors
BITMAP *ico2bmp ( FILE *f, WORD Width, WORD Height, DWORD Offset, DWORD Bytes ) {
  BITMAP *bmp;
  BITMAPINFOHEADER *icHeader;
  RGBQUAD *cols;
  void *data;
  l_int pos=0,l, c, bpp;
  BYTE sr,sg,sb, sbr,sbg,sbb;
  if ( !f ) return NULL;

  fsetpos( f, &Offset );

  data = malloc(Bytes);

  fread( data, Bytes,1, f );
  icHeader = data+pos;
  pos+=sizeof(BITMAPINFOHEADER);

  if ( !Width || !Height ) {
    Width  = icHeader->biWidth;
    Height = icHeader->biHeight;
  };

  if ( !Width || !Height ) return NULL;


  bpp  = icHeader->biBitCount;
  bmp  = create_bitmap(Width,Height);
  sr=getr(bmp->vtable->mask_color);
  sg=getg(bmp->vtable->mask_color);
  sb=getb(bmp->vtable->mask_color);

    if ( bpp == 24 ) { // ---- 24 Bits Per Pixel ----
      l=0;
      while ( l < Height ) {
        c=0;
        while ( c < Width ) {
          unsigned char r,g,b;
          memcpy(&r,data+pos,1);
          pos+=1;
          memcpy(&g,data+pos,1);
          pos+=1;
          memcpy(&b,data+pos,1);
          pos+=1;
          if ( r ==sr && g ==sg && b==sb )
            g+=36;
          putpixel(bmp,c,Height-1-l,makecol(r,g,b));
          c++;
        };
        l++;
      };
    } else { // Not 24 bpp, so there is a palette
      cols=data+pos;
      pos+=sizeof( RGBQUAD ) * (1<<bpp);
      l=0;
      if ( cols ) {
	    int a = 0;
	    while ( a < 1<<bpp ) {
		   if ( cols[a].rgbRed==sr && cols[a].rgbGreen==sg && cols[a].rgbBlue==sb ) {
             cols[a].rgbGreen+=36;
           };
	      a++;
        };
      };
      while ( l < Height ) {
        unsigned char b[32];
        c=0;
        while ( c < Width ) {
          if ( bpp == 8 ) { // ---- 8 Bits Per Pixel ----
            unsigned char nc = 0;
            memcpy(&nc,data+pos,1);
            pos+=1;
            putpixel(bmp,c,Height-1-l,makecol(cols[nc].rgbRed,cols[nc].rgbGreen,cols[nc].rgbBlue));
          } else if ( bpp == 16 ) { // ---- 16 Bits Per Pixel ----
            unsigned short nc;
            memcpy(&nc,data+pos,2);
            pos+=2;
            putpixel(bmp,c,Height-1-l,makecol(cols[nc].rgbRed,cols[nc].rgbGreen,cols[nc].rgbBlue));
          } else if ( bpp == 4 ) { // ---- 4 Bits Per Pixel ----
            unsigned long n;
            int j, k;
            int temp;
            int nc;
            j = c % 8;
            if (j == 0) {
              memcpy(&n,data+pos,4);
              pos+=4;
	             for (k=0; k<4; k++) {
	               temp = n & 255;
	               b[k*2+1] = temp & 15;
	               temp = temp >> 4;
	               b[k*2] = temp & 15;
	               n = n >> 8;
	             }
            }
            nc = b[j];
            putpixel(bmp,c,Height-1-l,makecol(cols[nc].rgbRed,cols[nc].rgbGreen,cols[nc].rgbBlue));
          } else if ( bpp == 1 ) { // ---- 1 Bits Per Pixel ----
            unsigned long n;
            int  j, k;
            int nc;

            j = c % 32;
            if (j == 0) {
              n = mgetl(data+pos);
              pos+=4;
              for (k=0; k<32; k++) {
	            b[31-k] = (char)(n & 1);
	            n = n >> 1;
	          }
            }

            nc = b[j];
            putpixel(bmp,c,Height-1-l,makecol(cols[nc].rgbRed,cols[nc].rgbGreen,cols[nc].rgbBlue));
          };
          c++;
        };
        l++;
      };
    };

    l=0;
    while ( l < Height ) { // This part applies the 'AND' mask (transparence) : 1 Bit Per Pixel
      unsigned char b[32];
      unsigned long n;
      int i, j, k;
      int pix;

      for (i=0; i<Width; i++) {
        j = i % 32;
        if (j == 0) {
        n = mgetl(data+pos);
        pos+=4;
	      for (k=0; k<32; k++) {
	        b[31-k] = (char)(n & 1);
	        n = n >> 1;
	      }
         }
        pix = b[j];
        if ( pix ) putpixel(bmp,i,Height-1-l,bmp->vtable->mask_color);
      }

      l++;
    };

  free(data);

  return bmp;
};
////////////////////////////////////////////////////////////////////////////////
BITMAP *_load_ico ( FILE *f, l_int wsize, l_int dep ) {

  ICONDIR *pIconDir = malloc(sizeof(ICONDIR)); // Allocate memory for ico directorie

  l_int i = 0;

  fread( &(pIconDir->idReserved), sizeof(WORD),1, f );

  fread( &(pIconDir->idType), sizeof(WORD),1, f );

  fread( &(pIconDir->idCount), sizeof(WORD),1, f );

  pIconDir = realloc( pIconDir, ( sizeof( WORD ) * 3 ) +
                                ( sizeof( ICONDIRENTRY ) * pIconDir->idCount ) );

  fread( &(pIconDir->idEntries), pIconDir->idCount * sizeof(ICONDIRENTRY),1, f );

  for ( i=0; i<pIconDir->idCount; i++ ) { // Search icon by size and depth

    if ( pIconDir->idEntries[i].bWidth == wsize && pIconDir->idEntries[i].bColorCount == dep ) {
      return ico2bmp( f , pIconDir->idEntries[i].bWidth,
                          pIconDir->idEntries[i].bHeight,
                          pIconDir->idEntries[i].dwImageOffset,
                          pIconDir->idEntries[i].dwBytesInRes );

    };
  }
  // Not found !
  for ( i=0; i<pIconDir->idCount; i++ ) { // Search icon by size

    if ( pIconDir->idEntries[i].bWidth == wsize ) {

      return ico2bmp ( f, pIconDir->idEntries[i].bWidth,
                          pIconDir->idEntries[i].bHeight,
                          pIconDir->idEntries[i].dwImageOffset,
                          pIconDir->idEntries[i].dwBytesInRes );

    };
  }

  for ( i=0; i<pIconDir->idCount; i++ ) { // Search icon by depth

    if ( pIconDir->idEntries[i].bColorCount == dep ) {

      BITMAP *bmp = ico2bmp ( f, pIconDir->idEntries[i].bWidth,
                          pIconDir->idEntries[i].bHeight,
                          pIconDir->idEntries[i].dwImageOffset,
                          pIconDir->idEntries[i].dwBytesInRes );
      return resizebmp(bmp,wsize,wsize);
    };
  }
  if ( pIconDir->idCount ) {
	BITMAP *bmp = ico2bmp ( f, pIconDir->idEntries[0].bWidth,
                          pIconDir->idEntries[0].bHeight,
                          pIconDir->idEntries[0].dwImageOffset,
                          pIconDir->idEntries[0].dwBytesInRes );
    return resizebmp(bmp,wsize,wsize);
  };
  // Not found !
  free( pIconDir );

  return NULL;

};

////////////////////////////////////////////////////////////////////////////////
// Load an image from an .ico file by size (wsize) and colors (wdepth)
// if exists else return an image in size, if not exists in size return NULL
// wsize can be 16, 32 or 64 ; wdepth can be  16, 256 or more
BITMAP *load_ico ( l_text file, l_int wsize, l_int wdepth ) {
  if ( file && wsize > 0 && wdepth > 0 ) { // Check if parameters are good
    FILE* f = fopen(file,"rb"); // Open the file for binary reading
    BITMAP *bmp;
    if ( !f ) return NULL; // If file not opened, return NULL
    bmp = _load_ico(f,wsize,( wdepth >= 1<<8 ) ? 0 : wdepth); // Use the sub functions for reading
    fclose(f); // Close file
    return bmp; // Return the bitmap
  };
  return NULL; // Wrong parameters, so return NULL;
};
////////////////////////////////////////////////////////////////////////////////
// Load an image from a .cur file (1st found in file)
p_cursor load_cur ( l_text file, l_int wdepth ) {
  if ( file && wdepth > 0 ) {
    p_cursor cur;
    FILE* f = fopen(file,"rb"); // Open the file for binary reading
    CURSORDIR *pCursorDir = malloc(sizeof(CURSORDIR)); // Allocate memory for ico directorie
    l_int i = 0;
    if ( !f ) { free(pCursorDir); return NULL; };
    cur = malloc(sizeof(t_cursor));
    clear_type(cur,sizeof(t_cursor));
    fread( &(pCursorDir->cdReserved), sizeof(WORD),1, f );
    fread( &(pCursorDir->cdType), sizeof(WORD),1, f );
    fread( &(pCursorDir->cdCount), sizeof(WORD),1, f );
    pCursorDir = realloc( pCursorDir, ( sizeof( WORD ) * 3 ) +
                                    ( sizeof( CURSORDIRENTRY ) * pCursorDir->cdCount ) );
    fread( &(pCursorDir->cdEntries), pCursorDir->cdCount * sizeof(CURSORDIRENTRY),1, f );
    cur->bmp =  ico2bmp( f , pCursorDir->cdEntries[0].bWidth,
                             pCursorDir->cdEntries[0].bHeight,
                             pCursorDir->cdEntries[0].dwImageOffset,
                             pCursorDir->cdEntries[0].lBytesInRes);
    cur->focus.x = pCursorDir->cdEntries[0].wXHotspot;
    cur->focus.y = pCursorDir->cdEntries[0].wYHotspot;
    free( pCursorDir );
    fclose(f);
    return cur;
  };
return NULL;
};

BITMAP* LoadIco32 ( AL_CONST char * tfile, RGB *pal ) {
  l_text file = mid ( (l_text)tfile, 0, strlen((l_text)tfile) - 3);
  BITMAP *b = load_ico ( (l_text)file, 32,  ( get_depth(screen) < 16 ) ? 16 : 256 );
  _free(file);
  return b;
};

BITMAP* LoadIco16 ( AL_CONST char * tfile, RGB *pal ) {
  l_text file = mid ( (l_text)tfile, 0, strlen((l_text)tfile) - 3);
  BITMAP *b= load_ico ( file, 16, ( get_depth(screen) < 16 ) ? 16 : 256 );
  _free(file);
  return b;
};

BITMAP* LoadIco ( AL_CONST char * file, RGB *pal ) {
  return load_ico ( (l_text)file, 32, ( get_depth(screen) < 16 ) ? 16 : 256 );
};

#include"algbm.h"
#include"jpeg.h"
#include"loadpng.h"

BITMAP *_load_gbm ( AL_CONST char *filename, RGB *pal) {
  return load_gbm((char *)filename, pal);
};

int     _save_gbm ( AL_CONST char *filename, BITMAP *bmp, AL_CONST RGB *pal) {
  return save_gbm((char *)filename, bmp, (RGB*) pal);
};

BITMAP *jpeg_load ( AL_CONST char *filename, RGB *pal) {
  return load_jpeg(((char *)filename), pal);
};

int     jpeg_save ( AL_CONST char *filename, BITMAP *bmp, AL_CONST RGB *pal) {
  return save_jpeg(((char *)filename), bmp,(RGB*) pal);
};

void init_ico ( void ) {

  register_bitmap_file_type("ico,32", &LoadIco32, NULL);
  register_bitmap_file_type("ico,16", &LoadIco16, NULL);
  register_bitmap_file_type("ico",    &LoadIco,   NULL);

  register_bitmap_file_type("gif",    &_load_gbm, &_save_gbm);
  register_bitmap_file_type("tif",    &_load_gbm, &_save_gbm);
  register_bitmap_file_type("vid",    &_load_gbm, &_save_gbm);
  register_bitmap_file_type("pgm",    &_load_gbm, &_save_gbm);
  register_bitmap_file_type("ppm",    &_load_gbm, &_save_gbm);
  register_bitmap_file_type("iax",    &_load_gbm, &_save_gbm);
  register_bitmap_file_type("xbm",    &_load_gbm, &_save_gbm);
  register_bitmap_file_type("pse",    &_load_gbm, &_save_gbm);
  register_bitmap_file_type("jpg",    &jpeg_load, &jpeg_save);

  register_png_file_type();
};
