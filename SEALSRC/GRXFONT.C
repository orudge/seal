/* GRX font file reader by Mark Wodrich.
 *
 * GRX FNT files consist of the header data (see struct below). If the font
 * is proportional, followed by a table of widths per character (unsigned 
 * shorts). Then, the data for each character follows. 1 bit/pixel is used,
 * with each line of the character stored in contiguous bytes. High bit of
 * first byte is leftmost pixel of line.
 *
 * Note : FNT files can have a variable number of characters, so we must
 *        check that the chars 32..127 exist.
 */

#include"grxfont.h"

void convert_grx_bitmap(int width, int height, GRX_BITMAP src, GRX_BITMAP dest) 
{
   unsigned short x, y, bytes_per_line, v = 0;
   unsigned char bitpos, bitset;
   bytes_per_line = (width+7) >> 3;

   for (y=0; y<height; y++) {
      for (x=0; x<width; x++) {
	 bitpos = 7-(x&7);
	 bitset = !!(src[(bytes_per_line*y) + (x>>3)] & (1<<bitpos));
    #ifdef i_love_br_font
	 dest[y*width+x] = bitset?255:0;
    #else
	 dest[y*width+x] = bitset;
    #endif
      }
   }
}



GRX_BITMAP *load_grx_bmps(PACKFILE *f, FNTfile_header *hdr, int numchar, unsigned short *wtable) 
{
   int t, width, bmp_size;
   GRX_BITMAP temp;
   GRX_BITMAP *bmp;

   /* alloc array of bitmap pointers */
   bmp = (GRX_BITMAP*)malloc(sizeof(GRX_BITMAP) * numchar);

   /* assume it's fixed width for now */
   width = hdr->width;

   /* temporary working area to store FNT bitmap */
   temp = (GRX_BITMAP)malloc(GRX_TMP_SIZE);

   for (t=0; t<numchar; t++) {
      /* if prop. get character width */
      if (!hdr->isfixed) 
	 width = wtable[t];

      /* work out how many bytes to read */
      bmp_size = ((width+7) >> 3) * hdr->height;

      /* oops, out of space! */
      if (bmp_size > GRX_TMP_SIZE) {
	 free(temp);
	 for (t--; t>=0; t--)
	    free(bmp[t]);
	 free(bmp);
	 return NULL;
      }

      /* alloc space for converted bitmap */
      bmp[t] = (GRX_BITMAP)malloc(width*hdr->height);

      /* read data */
      pack_fread(temp, bmp_size, f);

      /* convert to 1 byte/pixel */
      convert_grx_bitmap(width, hdr->height, temp, bmp[t]);
   }

   free(temp);
   return bmp;
}


static void stretch_character(BITMAP *bmp, BITMAP *sprite, int x, int y, int w, int h)
{
   if ( w > 0 && h > 0 )
      stretch_sprite(bmp, sprite, x, y, w, h);
   else
      blit(sprite, bmp, 0, 0, x, y, sprite->w, sprite->h);
}


void *grx_loadfont(char *fname, int w, int h)
{
   PACKFILE *f, *cf;
   FNTfile_header hdr;              /* GRX font header */
   int numchar;                     /* number of characters in the font */
   unsigned short *wtable = NULL;   /* table of widths for each character */
   GRX_BITMAP *bmp;                 /* array of font bitmaps */
   FONT *font = NULL;               /* the Allegro font */
   FONT_PROP *font_prop;
   int c, c2, start, width;
   char copyright[256];

   f = pack_fopen(fname, F_READ);
   if (!f)
      return NULL;

   pack_fread(&hdr, sizeof(hdr), f);      /* read the header structure */

   if (hdr.magic != FONTMAGIC) {          /* check magic number */
      pack_fclose(f);
      return NULL;
   }

   numchar = hdr.maxchar-hdr.minchar+1;

   if (!hdr.isfixed) {                    /* proportional font */
      wtable = (unsigned short*)malloc(sizeof(unsigned short) * numchar);
      pack_fread(wtable, sizeof(unsigned short) * numchar, f);
   }

   bmp = load_grx_bmps(f, &hdr, numchar, wtable);
   if (!bmp)
      goto get_out;

   if (pack_ferror(f))
      goto get_out;

   font = (FONT*)malloc(sizeof(FONT));
   memset(font, NULL, sizeof(FONT));
   font->height = -1;
   font->dat.dat_prop = font_prop = (FONT_PROP*)malloc(sizeof(FONT_PROP));
   memset(font_prop, NULL, sizeof(FONT_PROP));
   start = 32 - hdr.minchar;
   width = hdr.width;

   for (c=0; c<FONT_SIZE; c++) {
      c2 = c+start;

      if ((c2 >= 0) && (c2 < numchar)) {
	 if (!hdr.isfixed)
	    width = wtable[c2];

	 font_prop->dat[c] = create_bitmap_ex(8, width, hdr.height);
	 memcpy(font_prop->dat[c]->dat, bmp[c2], width*hdr.height);
      }
      else {
	 font_prop->dat[c] = create_bitmap_ex(8, 8, hdr.height);
	 clear(font_prop->dat[c]);
      }
   }

   get_out:

   pack_fclose(f);

   if (wtable)
      free(wtable);

   if (bmp) {
      for (c=0; c<numchar; c++)
	 free(bmp[c]);
      free(bmp);
   }

   return font;
}

