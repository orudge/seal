#ifndef __GRXFONT_H_INCLUDED__
#define __GRXFONT_H_INCLUDED__

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

#define FONTMAGIC       0x19590214L

/* .FNT file header */
typedef struct {
   unsigned long  magic;
   unsigned long  bmpsize;
   unsigned short width;
   unsigned short height;
   unsigned short minchar;
   unsigned short maxchar;
   unsigned short isfixed;
   unsigned short reserved;
   unsigned short baseline;
   unsigned short undwidth;
   char           fname[16];
   char           family[16];
} FNTfile_header;


/* all we need is an array of bytes for each character */
typedef unsigned char * GRX_BITMAP;


#define GRX_TMP_SIZE    4096



void convert_grx_bitmap(int width, int height, GRX_BITMAP src, GRX_BITMAP dest);
GRX_BITMAP *load_grx_bmps(PACKFILE *f, FNTfile_header *hdr, int numchar, unsigned short *wtable);
static void stretch_character(BITMAP *bmp, BITMAP *sprite, int x, int y, int w, int h);
void *grx_loadfont(char *fname, int w, int h);

#endif

