//----------------------------------------------------------------------------
//
// ALLEGTTF - TTF fonts for Allegro
//
// Douglas Eleveld (D.J.Eleveld@anest.azg.nl)
//
//----------------------------------------------------------------------------
#ifndef AATEXT_HEADER
#define AATEXT_HEADER

#include <allegro.h>

//----------------------------------------------------------------------------
// Version Info
#define ALLEGTTF_VERSION          2
#define ALLEGTTF_SUB_VERSION      0
#define ALLEGTTF_VERSION_STR      "2.0"
#define ALLEGTTF_DATE_STR         "June 2001"
#define ALLEGTTF_DATE             20010601

/*  Allow inclusion in C++ programs */
#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------*/
/* Startup and shutdown stuff */
void antialias_init (struct RGB* pal);
void antialias_palette (struct RGB* pal);
void antialias_exit (void);

/*-----------------------------------------------------------------------------*/
/* The basic antialiased textout stuff */
void aatextout (BITMAP *bmp, AL_CONST FONT *font, AL_CONST char *string, int x, int y, int colour);
void aatextout_center (BITMAP *bmp, AL_CONST FONT *font, AL_CONST char *string, int x, int y, int colour);
void aatextout_right (BITMAP *bmp, AL_CONST FONT *font, AL_CONST char *string, int x, int y, int colour);

/* These functions are almost directly from Allegro */
void aatextprintf(BITMAP *bmp, AL_CONST FONT *f, int x, int y, int color, AL_CONST char *format, ...);
void aatextprintf_center(BITMAP *bmp, AL_CONST FONT *f, int x, int y, int color, AL_CONST char *format, ...);
void aatextprintf_right(BITMAP *bmp, AL_CONST FONT *f, int x, int y, int color, AL_CONST char *format, ...);

/*-----------------------------------------------------------------------------*/
/* Font loading handling stuff */

/* Simply load a font */
//struct FONT* load_font (const char*);

/* Load a font and tell us if it is antialiased or not */
//struct FONT* load_font2 (const char*, int* antialiased);

/* Possible types of font smoothing */
#define ALLEGTTF_NOSMOOTH    0
#define ALLEGTTF_TTFSMOOTH   1
#define ALLEGTTF_REALSMOOTH  2

/* Load a font froma bitmap file */
struct FONT* load_font(char *filename);


/* Load a ttf font from a file */
struct FONT* load_ttf_font_ex (const char* filename,
                        const int points_w,
                        const int points_h,
                        const int smooth);

/* Load a ttf font from a file */
struct FONT* load_ttf_font_ex2 (const char* filename,
                         const int points_w,
                         const int points_h,
                         const int begin,
                         const int end,
                         const int smooth);
                        
/* Load a ttf font from a file */
struct FONT* load_ttf_font (const char* filename,
                     const int points,
                     const int smooth);


/* Load a ttf font from a file using TTF2PCX on the path, saves memory! */
struct FONT* load_ttf_font_ex2_mem (const char* filename,
                                    const int pw,
                                    const int ph,
                                    const int begin,
                                    const int end,
                                    const int smooth);
                                    
struct FONT* load_ttf_font_ex_mem (const char* filename,
                                   const int pw,
                                   const int ph,
                                   const int smooth);
                                   
struct FONT* load_ttf_font_mem (const char* filename,
                                const int points,
                                const int smooth);


/*-----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*-----------------------------------------------------------------------------*/

#endif /* ALLEGTTF_HEADER */

