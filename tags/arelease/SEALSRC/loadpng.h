/* loadpng.h */
#ifndef _included_loadpng_h_
#define _included_loadpng_h_

#ifdef __cplusplus
extern "C" {
#endif 



/* Overkill :-) */    
#define LOADPNG_VERSION		0
#define LOADPNG_SUBVERSION	4
#define LOADPNG_VERSIONSTR	"0.4"

    
/* Screen gamma value.  Default is -1, which chooses either:
 *  1) Get environment variable SCREEN_GAMMA
 *  2) Use the default of 2.2
 */
extern double _png_screen_gamma;

    
/* Choose zlib compression level for saving file.
 * Default is Z_BEST_COMPRESSION.
 */
extern int _png_compression_level;

    
/* Adds `PNG' to Allegro's internal file type table.
 * You can then just use load_bitmap and save_bitmap as usual.
 */
void register_png_file_type(void);
    
    
/* What do you think? */
BITMAP *load_png(char *filename, RGB *pal);
int save_png(char *filename, BITMAP *bmp, RGB *pal);

   
#ifdef __cplusplus
}
#endif

#endif /* _included_loadpng_h */
