/* AllegroFont - a wrapper for FreeType 2 */
/* to render TTF and other format fonts with Allegro */

            
/* FreeType 2 is copyright (c) 1996-2000 */
/* David Turner, Robert Wilhelm, and Werner Lemberg */
/* AllegroFont is copyright (c) 2001 Javier Gonz lez */

/* See COPYING.txt (GNU Lesser General Public License 2.1) for license */


#ifndef ALFONT_H
#define ALFONT_H

#ifdef __cplusplus
extern "C" {
#endif


/* common define */

#define ALFONT_VERSION              1
#define ALFONT_SUB_VERSION          1
#define ALFONT_VERSION_STR          "1.1"
#define ALFONT_DATE_STR             "24/08/2001"
#define ALFONT_DATE                 20010824    /* yyyymmdd */

/* error codes */

#define ALFONT_OK                   0


/* includes */

#include <allegro.h>


/* structs */
struct _ALFONT_FONT;
typedef struct _ALFONT_FONT ALFONT_FONT;


/* API */

int alfont_init(void);
int alfont_init_unusing_atexit(void);
void alfont_exit(void);

ALFONT_FONT *alfont_load_font(const char *filepathname);
ALFONT_FONT *alfont_load_font_from_mem(const char *data, int data_len);
void alfont_destroy_font(ALFONT_FONT *f);

int alfont_set_font_size(ALFONT_FONT *f, int w, int h);

int alfont_text_mode(int mode);

void alfont_textout_aa(BITMAP *bmp, const ALFONT_FONT *f, const char *s, int x, int y, int color);
void alfont_textout(BITMAP *bmp, const ALFONT_FONT *f, const char *s, int x, int y, int color);

void alfont_textout_centre_aa(BITMAP *bmp, const ALFONT_FONT *f, const char *s, int x, int y, int color);
void alfont_textout_centre(BITMAP *bmp, const ALFONT_FONT *f, const char *s, int x, int y, int color);

void alfont_textout_right_aa(BITMAP *bmp, const ALFONT_FONT *f, const char *s, int x, int y, int color);
void alfont_textout_right(BITMAP *bmp, const ALFONT_FONT *f, const char *s, int x, int y, int color);

void alfont_textprintf(BITMAP *bmp, const ALFONT_FONT *f, int x, int y, int color, const char *format, ...);
void alfont_textprintf_aa(BITMAP *bmp, const ALFONT_FONT *f, int x, int y, int color, const char *format, ...);

void alfont_textprintf_centre(BITMAP *bmp, const ALFONT_FONT *f, int x, int y, int color, const char *format, ...);
void alfont_textprintf_centre_aa(BITMAP *bmp, const ALFONT_FONT *f, int x, int y, int color, const char *format, ...);

void alfont_textprintf_right(BITMAP *bmp, const ALFONT_FONT *f, int x, int y, int color, const char *format, ...);
void alfont_textprintf_right_aa(BITMAP *bmp, const ALFONT_FONT *f, int x, int y, int color, const char *format, ...);

int alfont_text_height(const ALFONT_FONT *f);
int alfont_text_length(const ALFONT_FONT *f, const char *str);


#ifdef __cplusplus
}
#endif

#endif
