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


//#ifndef NEW_ALLEGRO
//#include"oldallegro.h"
//#else
//#include"allegro.h"
//#endif

#ifndef __ALLTOGRX_H_INCLUDED__
#define __ALLTOGRX_H_INCLUDED__

#ifndef  GRAPHICS_TYPES
#define  GRAPHICS_TYPES
#define  define_nothing
#define  ScreenContext    screen
#define  GrRGB            RGB
#define  GrCOLOR          int
#define  GrCONTEXT        BITMAP
#define  GrRLE_SPRITE     RLE_SPRITE
#define  GrIMAGE          GrCONTEXT
#define  GrFONT           void*
#define  GrFIXED          fixed
#define  GrV3D            V3D
#define  GrV3DF           V3D_f
#define  GrPALETTE        PALETTE

#define  GrRgb            GrRGB
#define  GrColor          GrCOLOR
#define  GrContext        GrCONTEXT
#define  GrRleSprite      GrRLE_SPRITE
#define  GrImage          GrContext
#define  GrFont           GrFONT
#define  GrFixed          GrFIXED
#define  GrV3d            GrV3D
#define  GrV3Df           GrV3DF
#define  GrPalette        GrPALETTE

#define  GrPALETTESIZE    PAL_SIZE

#endif

#ifndef  TX_NOCOLOR
#define  TX_NOCOLOR               -1
#endif

#ifndef  TX_ALIGNS
#define  TX_ALIGNS
#define  TX_ALIGN_LEFT     0x01
#define  TX_ALIGN_TOP      0x02
#define  TX_ALIGN_RIGHT    0x04
#define  TX_ALIGN_BOTTOM   0x08
#define  TX_ALIGN_CENTERX  0x10
#define  TX_ALIGN_CENTERY  0x20
#define  TX_ALIGN_CENTER   (TX_ALIGN_CENTERX|TX_ALIGN_CENTERY)
#define  TX_ALIGN_DEFAULT  (TX_ALIGN_TOP|TX_ALIGN_LEFT) // or 0
#define  TX_UNDERLINE      0x40
#define  TX_STRIKELINE     0x80
#define  TX_NOSMOOTHTEXT   0x100
#endif

#define  CONTEXT_WIDTH(d)  (d?d->w:0)
#define  CONTEXT_HEIGHT(d) (d?d->h:0)
#define  IMAGE_WIDTH(d)    CONTEXT_WIDTH(d)
#define  IMAGE_HEIGHT(d)   CONTEXT_HEIGHT(d)

#define  GET_DATA(d,x) get_datafile_object(d, x)

#define  FONT_DEFAULT_WIDTH   16
#define  FONT_DEFAULT_HEIGHT  16

#define  NOCOLOR_RED           255
#define  NOCOLOR_GREEN           0
#define  NOCOLOR_BLUE          255

#define  ICON_16               16
#define  ICON_32               32

#define  SF_FLIPX              0x01
#define  SF_FLIPY              0x02

extern int   (*textlen)(void *f, unsigned char *text, int len );
extern void  (*change_font_size)(void *f, int w, int h);
extern int   (*textheight)(void *f );
extern void  (*drawtext)(BITMAP *b, void *f, unsigned char *text, int x1, int y1, GrCOLOR color );


/* icon library */

typedef struct ICONLIBRARY {

  int         id;
  DATAFILE   *datfile;
  BITMAP     *icon16;
  BITMAP     *icon32;

  char        rserved[16];

  struct ICONLIBRARY *next;

} ICONLIBRARY;


#ifndef NEW_ALLEGRO
/* only for low-level graphics */

/* begin of my allegro functions */
#ifdef __cplusplus
extern "C" {
#endif
/* mouse */
extern BITMAP *_mouse_sprite;
extern int  _drawing_in_screen;
extern int mouse_mode;
extern int mouse_mode_rect_ax;
extern int mouse_mode_rect_ay;
extern int mouse_mode_rect_bx;
extern int mouse_mode_rect_by;
extern void (*run_mouse_mode)(struct BITMAP *msb, int mdx, int mdy, int erase, int show );
int  mouse_is_block();
int  mouse_is_visible();
int  mouse_block(struct BITMAP *bmp, int x1, int y1, int x2, int y2);
void mouse_unblock(int i);
void get_mouse_focus(int *fx, int *fy);
void get_mouse_range(int *x1, int *y1, int *x2, int *y2);
/* end of my allegro functions */
#ifdef __cplusplus
};
#endif
#endif

#define FR_HOR 0x01
#define FR_VER 0x02

void get_size_of_ftext(char *text, GrFONT *font, int *x, int *y);
void point_vline ( BITMAP *out, int x1, int y1, int y2, GrCOLOR color );
void point_hline ( BITMAP *out, int x1, int y1, int x2, GrCOLOR color );
void fade_rect ( BITMAP *b, int x1, int y1, int x2, int y2, GrCOLOR one, GrCOLOR two, int direct );
void lined_rect ( BITMAP *b, int x1, int y1, int x2, int y2, GrCOLOR one, GrCOLOR two );
void light_image ( BITMAP *b, int x1, int y1, int x2, int y2, int dr, int dg, int db );
void button ( BITMAP *b, int x1, int y1, int x2, int y2, GrCOLOR c1, GrCOLOR c2 );
int  get_depth ( BITMAP *b );
int  get_mask_color ( BITMAP *b );
int  get_width ( BITMAP *b );
int  get_height ( BITMAP *b );
int  get_clip ( BITMAP *b, int *x1, int *y1, int *x2, int *y2 );
void get_real_box ( BITMAP *b, int *x1, int *y1, int *x2, int *y2 );

int  get_char_length ( void *f, unsigned char ch );
void draw_char ( BITMAP *out, GrFONT *f, unsigned char chr, int x1, int y1,
                 GrCOLOR f1, GrCOLOR b1 );
void textout_draw(BITMAP *bmp, void *f, unsigned char *s, int len, int x1, int y1, int align, GrColor color, GrColor bcolor );
void textout_draw_rect_ex(BITMAP *bmp, void *f, unsigned char *s, int len, int x1, int y1, int x2, int y2, int align, GrColor color, GrColor bcolor, int clip, int marge );
void textout_draw_rect(BITMAP *bmp, void *f, unsigned char *s, int len, int x1, int y1, int x2, int y2, int align, GrColor color, GrColor bcolor, int clip );
void textout_printf(BITMAP *bmp, void *f, int len,
                  int x1, int y1, int x2, int y2, int align, GrColor color, GrColor bcolor,
                  int clip, char *s, ... );
void draw_double_text ( BITMAP *out, GrFONT *f, unsigned char *txt, int size,
                        int x1, int y1, int x2, int y2, int align,
                        GrCOLOR f1, GrCOLOR f2, int clip );
void draw_selected_text ( BITMAP *out, GrFONT *f,
                          unsigned char *txt, int size, int sfrom, int sto,
                          int x1, int y1, int x2, int y2, int align,
                          GrCOLOR f1, GrCOLOR b1, GrCOLOR f2, GrCOLOR b2, int clip );

void draw_flip_sprite ( BITMAP *b, BITMAP *s, int x, int y, int flag );
void __blit_hline ( BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int dw );
void blit_ex ( BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int sh, int dw, int dh );
void blit_hline ( BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int dw );
void blit_circlefill(BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int radius);
void blit_ellipsefill(BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int rx, int ry);
BITMAP *gen_neg ( int depth, BITMAP *src );
BITMAP *gen_emboss ( int depth, BITMAP *src );
BITMAP *conv_to_skipcolor_bitmap ( BITMAP *ctx, int r, int g, int b );
BITMAP  *_fixup_loaded_bitmap ( BITMAP *bmp, PALETTE pal, int bpp );
DATAFILE *conv_to_skipcolor_data ( DATAFILE *f, int r, int g, int b );
DATAFILE *load_skip_datafile ( char *filename, int r, int g, int b );
void     *get_datafile_object ( DATAFILE *dat, int obj_num );
ICONLIBRARY  *convert_to_icon_library ( DATAFILE *dat );
ICONLIBRARY  *load_icon_library ( char *filename, int r, int g, int b );
void  unload_icon_library ( ICONLIBRARY *icl );
BITMAP  *get_icon_from_library ( ICONLIBRARY *icl, int id, int size );
GrIMAGE *gr_load_bitmap ( char *filename );
int  _gr_setmode ( int _s, int _w, int _h, int _v_w, int _v_h, char _depth );
unsigned long    get_context_mem_size ( BITMAP *b );
unsigned char*   get_addr_line ( BITMAP *b, int line );

//#ifndef NEW_ALLEGRO
//#include"newscr.h"
//#endif

#define  GrLoadBitmap(f)               gr_load_bitmap(f)
#define  load_image(f)                 GrLoadBitmap(f)
#define  GrLoadSkipDataFile(f,r,g,b)   load_skip_datafile(f, r, g, b)
#define  GrLoadDataFile(n)             load_datafile(n)
#define  GrUnloadDataFile(d)           unload_datafile(d)
#define  GrLoadDataFileObject(f,on)    load_datafile_object(f, on)
#define  GrUnloadDataFileObject(f,on)  unload_datafile_object(dat)
#define  GrGetDataFileProperty(d,t)    get_datafile_property(dat, t)
#define  GrRegisterDataFileObject(i,l,s) \
                                       register_datafile_object(i, l, s)
#define  GrFixupDataFile(d)            fixup_datafile(data)

#define  GrDepth(b)                              get_depth(b)
#define  GrMaskColor(b)                          get_mask_color(b)
#define  GrAddressLine(b,l)                      get_addr_line(b, l)
#define  GrDirectAddress(b)                      (b?b->dat:NULL)
#define  GrGetContextMemorySize(b)               get_context_mem_size(b)

#define  GrSetTextDrawingMode(i)                text_mode(i)
#define  GrSetMode(s,w,h,v_w,v_h,d)             _gr_setmode(s, w, h, v_w, v_h, d)
#define  GrSetTextMode()                        _gr_setmode(GFX_TEXT, 80, 25, 0, 0, 4)
#define  GrScreenW()                            SCREEN_W
#define  GrScreenH()                            SCREEN_H

#define  GrGenerateNegativ(d,s)                 gen_neg(d, s)
#define  GrGenerateEmboss(d,s)                  gen_emboss(d, s)
#define  GrConvertToSkipColorContext(c,r,g,b)   conv_to_skipcolor_bitmap(c, r, g, b)
#define  GrConvertToSkipColorData(d,n,r,g,b)    conv_to_skipcolor_data(d, n, r, g, b)
#define  GrCreateContext(w,h)                   create_bitmap(w, h)
#define  GrCreateContextEx(cd,w,h)              create_bitmap_ex(cd, w, h)
#define  GrCreateSubContext(p,x1,y1,w,h)        create_sub_bitmap(p, x1, y1, w, h)
#define  GrFixupLoadedBitmap(b,p,d)             ((b)?_fixup_loaded_bitmap(b,p,d):NULL)
#define  GrDestroyContext(c)                    destroy_bitmap(c)

#define  GrDoLine(b,x1,y1,x2,y2,d,p)            do_line(b,x1,y1,x2,y2,d,p)
#define  GrDoCircle(b,x1,y1,r,d,p)              do_circle(b,x1,y1,r,d,p)
#define  GrDoEllipse(b,x1,y1,rx1,ry1,d,p)       do_ellipse(b,x1,y1,rx1,ry1,d,p)
#define  GrCalcSpline(p,npts,px,py)             calc_spline(p, npts, px, py)

#define  GrSetDrawMode(m,p,xa,ya)               drawing_mode(m, p, xa, ya)
#define  GrSetClipBox(b,x1,y1,x2,y2)            set_clip(b, x1, y1, x2, y2)
#define  GrSetContext(b)                        CurrentContext = b
#define  GrWidth(b)                             get_width(b)
#define  GrHeight(b)                            get_height(b)
#define  GrGetClipBox(b,x1,y1,x2,y2)            get_clip(b, x1, y1, x2, y2)
#define  GrGetRealBox(b,x1,y1,x2,y2)            get_real_box(b, x1, y1, x2, y2)

#define  GrClearClip(b)                         clear(b)
#define  GrClearClipColor(b,c)                  clear_to_color(b,c)

#define  GrDrawSprite(b,s,x,y)                  draw_sprite(b, s, x, y)
#define  GrDrawSpriteLit(b,s,x,y,c)             draw_lit_sprite(b, s, x, y, c)
#define  GrDrawSpriteTrans(b,s,x,y)             draw_trans_sprite(b, s, x, y)
#define  GrDrawSpriteFlip(b,s,x,y,f)            draw_flip_sprite(b, s, x, y, f)
#define  GrDrawRleSprite(b,s,x,y)               draw_rle_sprite(b, s, x, y)
#define  GrDrawRleSpriteLit(b,s,x,y,c)          draw_lit_rle_sprite(b, s, x, y, c)
#define  GrDrawRleSpriteTrans(b,s,x,y)          draw_trans_rle_sprite(b, s, x, y)

#define  GrPutPixel(b,x1,y1,c)                  putpixel(b, x1, y1, c)
#define  GrGetPixel(b,x1,y1)                    getpixel(b, x1, y1)
#define  GrSpline(b,p,c)                        spline(b, p, c)
#define  GrHLine(b,x1,y1,w,c)                   hline(b, x1, y1, x1+w, c)
#define  GrVLine(b,x1,y1,h,c)                   vline(b, x1, y1, y1+h, c)
#define  GrLine(b,x1,y1,x2,y2,c)                line(b, x1, y1, x2, y2, c)
#define  GrRectColor(b,x1,y1,x2,y2,c)           rect(b, x1, y1, x2, y2, c)
#define  GrRectFilled(b,x1,y1,x2,y2,c)          rectfill(b, x1, y1, x2, y2, c)
#define  GrPolygon(b,v,p,c)                     polygon(b, v, p, c)
#define  GrCircle(b,x1,y1,r,c)                  circle(b, x1, y1, r, c)
#define  GrCircleFilled(b,x1,y1,r,c)            circlefill(b, x1, y1, r, c)
#define  GrEllipse(b,x1,y1,rx1,ry1,c)           ellipse(b, x1, y1, rx1, ry1, c)
#define  GrEllipseFilled(b,x1,y1,rx1,ry1,c)     ellipsefill(b, x1, y1, rx1, ry1, c)
#define  GrFloodFill(b,x1,y1,c)                 floodfill(b, x1, y1, c)
#define  GrBitBlt(d,x1d,y1d,s,x1s,y1s,x2s,y2s)  blit(s, d, x1s, y1s, x1d, y1d, (x2s-x1s)+1, (y2s-y1s)+1)
#define  GrBitBltEx(d,x1d,y1d,x2d,y2d,s,x1s,y1s,x2s,y2s)  \
                                                blit_ex(s, d, x1s, y1s, x1d, y1d, (x2s-x1s)+1, (y2s-y1s)+1, (x2d-x1d)+1, (y2d-y1d)+1)
#define  GrStretchSprite(d,s,x,y,w,h)           stretch_sprite(d, s, x, y, w, h)
#define  GrHLineBlt(d,x1d,y1d,dw,s,x1s,y1s,sw)  \
                                                blit_hline(s, d, x1s, y1s, x1d, y1d, sw, dw)
#define  GrCircleBlt(d,x1d,y1d,s,x1s,y1s,sw,r)  blit_circlefill(s, d, x1s, y1s, x1d, y1d, sw, r)
#define  GrEllipseBlt(d,x1d,y1d,s,x1s,y1s,sw,rx,ry)  \
                                                blit_ellipsefill(s, d, x1s, y1s, x1d, y1d, sw, rx, ry)
#define  GrMaskedBlt(d,x1d,y1d,s,x1s,y1s,x2s,y2s) \
                                                masked_blit(s, d, x1s, y1s, x1d, y1d, (x2s-x1s)+1, (y2s-y1s)+1)
#define  GrStretchBlt(d,x1d,y1d,x2d,y2d,s,x1s,y1s,x2s,y2s)  \
                                                stretch_blit(s, d, x1s, y1s, (x2s-x1s)+1, (y2s-y1s)+1, x1d, y1d, (x2d-x1d)+1, (y2d-y1d)+1)
#define  GrRotateSprite(d,x1d,y1d,s,a)          rotate_sprite(d, s, x1d, y1d, a)
#define  GrRotateStretchedSprite(d,x1d,y1d,s,a,sc) \
                                                rotate_scaled_sprite(d, s, x1d, y1d, a, sc)
#define  GrDrawText(b,f,str,l,x1,y1,a,c)        textout_draw(b, f, str, l, x1, y1, a, c)
#define  GrDrawTextRect(b,f,str,l,x1,y1,x2,y2,a,c,ic) \
                                                textout_draw_rect(b, f, str, l, x1, y1, x2, y2, a, c, ic)

#define  GrFontTextLength(f,str,l)              (*textlen)(f,str,l)
#define  GrFontCharLength(f,ch)			         get_char_length(f,ch)
#define  GrFontTextHeight(f)                    (*textheight)(f)


#define  GrPolygon3d(b,t,s,vc,vtx)              polygon3d(b, t, s, vc, vtx)
#define  GrPolygon3df(b,t,s,vc,vtx)             polygon3d_f(b, t, s, vc, vtx)
#define  GrTriangle3d(b,t,s,v1,v2,v3)           triangle3d(b, t, s, v1, v2, v3)
#define  GrTriangle3df(b,t,s,v1,v2,v3)          triangle3d_f(b, t, s, v1, v2, v3)
#define  GrQuad3d(b,t,s,v1,v2,v3,v4)            quad3d(b, t, s, v1, v2, v3, v4)
#define  GrQuad3df(b,t,s,v1,v2,v3,v4)           quad3d_f(b, t, s, v1, v2, v3, v4)

#define  __putpixel(b,x1,y1,c)                  _putpixel(b, x1, y1, c)
#define  __getpixel(b,x1,y1)                    _getpixel(b, x1, y1)

/* end of low-level graphics */

// colors

#define  CoSetColor(i,p)                       set_color(i, p)
#define  CoSetPalette(p)                       set_pallete(p)
#define  CoSetPaletteRange(p,f,t,vs)           set_pallete_range(p, f, t, vs)
#define  CoGetColor(i,p)                       get_color(i, p)
#define  CoGetPalette(p)                       get_pallete(p)
#define  CoGetPaletteRange(p,f,t)              get_pallete_range(p, f, t)

#define  CoFadeInterpolate(s,d,o,p,f,t)        fade_interpolate(s, d, o, p, f, t)
#define  CoFadeFromRange(s,d,sp,f,t)           fade_from_range(s, d, sp, f, t)
#define  CoFadeInRange(p,s,f,t)                fade_in_range(p, sp, f, t)
#define  CoFadeOutRange(sp, f, t)              fade_out_range(sp, f, t)
#define  CoFadeFrom(s,d,sp)                    fade_from(s, d, sp)
#define  CoFadeIn(p,sp)                        fade_in(p, sp)
#define  CoFadeOut(sp)                         fade_out(sp)

#define  CoSelectPalette(p)                    select_pallete(p)
#define  CoUnselectPalette()                   unselect_pallete()

#define  CoGenerate332Palette(p)               generate_332_palette(p)
#define  CoGenerateOptimizedPalette(b,p,c)     generate_optimized_palette(b, p, c)
#define  CoCreateRgbTable(t,p,callback)        create_rgb_table(t, p, callback)
#define  CoCreateLightTable(t,p,r,g,b,callback) \
                                               create_light_table(t, p, r, g, b, callback)
#define  CoCreateTransTable(t,p,r,g,b,callback) \
                                               create_trans_table(t, p, r, g, b, callback)
#define  CoCreateColorTable(t,p,blendfunc,callback) \
                                               create_color_table(t, p, blendfunc, callback)
#define  CoSetBlenderMode(b15,b16,b24,r,g,b,a) \
                                               set_blender_mode(b15, b16, b24, r, g, b, a)
#define  CoSetTransBlender(r,g,b,a)            set_trans_blender(r, g, b, a)
#define  CoHsvToRgb(h,s,v,r,g,b)               hsv_to_rgb(h, s, v, r, g, b)
#define  CoRgbToHsv(r,g,b,h,s,v)               rgb_to_hsv(r, g, b, h, s, v)
#define  CoBestFitColor(p,r,g,b)               bestfit_color(p, r, g, b)

#define  CoMakeColor(r,g,b)                    makecol(r, g, b)
#define  CoMakeColor8(r,g,b)                   makecol8(r, g, b)
#define  CoMakeColorDepth(d,r,g,b)             makecol_depth(d, r, g, b)

#define  CoGetRed(c)                           getr(c)
#define  CoGetGreen(c)                         getg(c)
#define  CoGetBlue(c)                          getb(c)
#define  CoGetRedDepth(d,c)                    getr_depth(d, c)
#define  CoGetGreenDepth(d,c)                  getg_depth(d, c)
#define  CoGetBlueDepth(d,c)                   getb_depth(d, c)

#define  CoMakeColor15(r,g,b)                  makecol15(r, g, b)
#define  CoMakeColor16(r,g,b)                  makecol16(r, g, b)
#define  CoMakeColor24(r,g,b)                  makecol24(r, g, b)
#define  CoMakeColor32(r,g,b)                  makecol32(r, g, b)

#define  CoGetRed8(c)                          getr8(c)
#define  CoGetGreen8(c)                        getg8(c)
#define  CoGetBlue8(c)                         getb8(c)
#define  CoGetRed15(c)                         getr15(c)
#define  CoGetGreen15(c)                       getg15(c)
#define  CoGetBlue15(c)                        getb15(c)
#define  CoGetRed16(c)                         getr16(c)
#define  CoGetGreen16(c)                       getg16(c)
#define  CoGetBlue16(c)                        getb16(c)
#define  CoGetRed24(c)                         getr24(c)
#define  CoGetGreen24(c)                       getg24(c)
#define  CoGetBlue24(c)                        getb24(c)
#define  CoGetRed32(c)                         getr32(c)
#define  CoGetGreen32(c)                       getg32(c)
#define  CoGetBlue32(c)                        getb32(c)


//#ifndef NEW_ALLEGRO
//#include"grxfont.h"
//extern void   *(*load_font)(char* _filename, char *xxname, int w, int h, int from, int to);
//#define  GrLoadFont(f,x,w,h,fr,t)              (load_font?(*load_font)(f,x,w,h,fr,t):NULL)
//#else
/* we want to use the load_font from libalttf so have to rename this
 some old code might call it directly - it should still work though the extra args are unused
 will fail to load ttf fonts though. Serves em right - they should use the ini file.
*/
extern void   *(*seal_load_font)(char* _filename, char *xxname, int w, int h, int from, int to);
#define  GrLoadFont(f,x,w,h,fr,t)              (seal_load_font?(*seal_load_font)(f,x,w,h,fr,t):NULL)
//#endif

extern void    (*unload_font)(void *f);
#define  GrUnloadFont(f)                       (unload_font?(*unload_font)(f):NULL)
#endif

