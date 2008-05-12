//----------------------------------------------------------------------------
//
// ALLEGTTF - TTF fonts for Allegro
//
// Douglas Eleveld (D.J.Eleveld@anest.azg.nl)
//
//----------------------------------------------------------------------------
#ifndef AATEXT_HEADER
#define AATEXT_HEADER

//----------------------------------------------------------------------------
#include <allegro.h>

//----------------------------------------------------------------------------
// Version Info
#define ALLEGTTF_VERSION          1
#define ALLEGTTF_SUB_VERSION      5
#define ALLEGTTF_VERSION_STR      "1.5"
#define ALLEGTTF_DATE_STR         "Oct 1998"
#define ALLEGTTF_DATE		  19981010

/*  Allow inclusion in C++ programs */
#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------*/
/* Startup and shutdown stuff */
void antialias_init (PALETTE pal);
void antialias_palette (PALETTE pal);
void antialias_exit (void);


/*-----------------------------------------------------------------------------*/
/* The basic antialiased textout stuff */
void aatextout (BITMAP *bmp, FONT *font, char *string, int x, int y, int colour);
void aatextout_center (BITMAP *bmp, FONT *font, char *string, int x, int y, int colour);

/* These functions are almost directly from Allegro */
void aatextprintf(BITMAP *bmp, FONT *f, int x, int y, int color, char *format, ...);
void aatextprintf_center(BITMAP *bmp, FONT *f, int x, int y, int color, char *format, ...);


/*-----------------------------------------------------------------------------*/
/* Font loading handling stuff */

/* Simply load a font */
FONT* load_font1 (const char*);

/* Load a font and tell us if it is antialiased or not */
FONT* load_font2 (const char*, int* antialiased);

/* Possible types of font smoothing */
#define ALLEGTTF_NOSMOOTH    0
#define ALLEGTTF_TTFSMOOTH   1
#define ALLEGTTF_REALSMOOTH  2

/* Load a ttf font from a file */
FONT* load_ttf_font_ex (const char* filename,
                        const int points_w,
                        const int points_h,
                        const int smooth);

/* Load a ttf font from a file */
FONT* load_ttf_font_ex2 (const char* filename,
                         const int points_w,
                         const int points_h,
                         const int begin,
                         const int end,
                         const int smooth);
                        
/* Load a ttf font from a file */
FONT* load_ttf_font (const char* filename,
                     const int points,
                     const int smooth);
                     

/*-----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*-----------------------------------------------------------------------------*/

#endif /* ALLEGTTF_HEADER */



