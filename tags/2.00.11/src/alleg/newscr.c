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
#include <vfile.h>
#include <object.h>

void _change_font_size ( void *f, int w, int h )
{
};

unsigned long get_context_mem_size ( BITMAP *b ) {
  if ( !b ) b = ScreenContext;
  return b?((unsigned long)b->w * (unsigned long)b->h * (unsigned long)(((b->vtable->color_depth+7)>>3)+1)):0;
};

int get_depth ( BITMAP *b ) {
  if ( !b ) b = ScreenContext;
  return (b?bitmap_color_depth(b):0);
};

int get_mask_color ( BITMAP *b ) {
  if ( !b ) b = ScreenContext;
  return (b? bitmap_mask_color(b) :0);
};

unsigned char* get_addr_line ( BITMAP *b, int line ) {
  if ( !b ) b = ScreenContext;
  return (b&&(line<b->h))?b->line[line]:0;
};

int get_width ( BITMAP *b ) {
  if ( !b ) b = ScreenContext;
  if ( b == ScreenContext ) return SCREEN_W;
  return b?b->w:0;
};

int get_height ( BITMAP *b ) {
  if ( !b ) b = ScreenContext;
  if ( b == ScreenContext ) return SCREEN_H;
  return b?b->h:0;
};

int get_clip ( BITMAP *b, int *x1, int *y1, int *x2, int *y2 ) {
  if ( !b ) b = screen;
  if ( b && (b->clip) ) {
    *x1 = b->cl;
    *y1 = b->ct;
    *x2 = b->cr;
    *y2 = b->cb;
    return 1;
  } else {
    *x1 = 0;
    *y1 = 0;
    *x2 = 0;
    *y2 = 0;
    return 0;
  };
};

t_rect get_clip_rect ( BITMAP *b ) {
  if ( !b ) b = screen;
  if ( b ) {
	if ( b->clip )
	  return rect_assign(b->cl,b->ct,b->cr,b->cb);
	else
	  return rect_assign(0,0,b->w-1,b->h-1);
  } else
    return rect_empty;
};

void set_clip_rect ( BITMAP *b, t_rect r ) {
  set_clip(b,r.a.x,r.a.y,r.b.x,r.b.y);
};

void get_real_box ( BITMAP *b, int *x1, int *y1, int *x2, int *y2 ) {
  if ( !b ) b = ScreenContext;
  if ( b ) {
    *x1 = 0;
    *y1 = 0;
    *x2 = get_width(b)-1;
    *y2 = get_height(b)-1;
  } else {
    *x1 = 0;
    *y1 = 0;
    *x2 = 0;
    *y2 = 0;
  };
};


void get_size_of_ftext ( char *text, GrFONT *font, int *x, int *y )
{
  int xx = 0;
  int maxx = 0;
  int yy = 0;
  if ( text && font && (x || y) ) {
    yy = textheight(font);
    while ( *text ) {
      if ( *text == '\n' ) {
        yy += textheight(font);
        if ( maxx < xx ) maxx = xx;
        xx  = 0;
      } else
        xx += get_char_length ( font, *text);
      text++;
    };
  };
  if ( maxx < xx ) maxx = xx;
  if ( x ) *x = maxx;
  if ( y ) *y = yy;
};

/* how about using hardware acceleration for the lines? Happilly they are never used. */
void point_vline ( BITMAP *out, int x1, int y1, int y2, GrCOLOR color )
{
  while ( y2 > y1 ) {
    putpixel(out, x1, y2, color);
    putpixel(out, x1-1, y2-1, color);
    y2-=2;
  };
};


void point_hline ( BITMAP *out, int x1, int y1, int x2, GrCOLOR color )
{
  while ( x2 > x1 ) {
    putpixel(out, x2, y1, color);
    putpixel(out, x2-1, y1-1, color);
    x2-=2;
  };
};


void draw_char ( BITMAP *out, GrFONT *f, unsigned char chr, int x1, int y1,
                 GrCOLOR f1, GrCOLOR b1 )
{
  unsigned char s[2] = {0, '\0'};
  s[0] = chr;
  text_mode(-1);
  if ( !f ) return;
  if ( b1 >= -1 )
    rectfill(out, x1, y1, x1+textlen(f, s, 1), y1+textheight(f), b1);
  (*drawtext)(out,f,s,x1,y1,f1);
};


void draw_selected_text ( BITMAP *out, GrFONT *f,
                          unsigned char *txt, int size, int sfrom, int sto,
                          int x1, int y1, int x2, int y2, int align,
                          GrCOLOR f1, GrCOLOR b1, GrCOLOR f2, GrCOLOR b2, int clip )
{
  if ( (sfrom != 0) || (sto == sfrom) ) {
    if ( (sto == sfrom) || (sfrom < 0) || (sto < -1) ) {
      textout_draw_rect(out, f, txt, size, x1, y1, x2, y2, align, f1, b1, clip);
      return;
    };
    textout_draw_rect(out, f, txt, sfrom, x1, y1, x2, y2, align, f1, b1, clip);
    x1 += textlen(f, txt, sfrom);
  };
  if ( sfrom >= 0 ) {
    if ( sto == -1 ) {
      textout_draw_rect(out, f, &txt[sfrom], size-sfrom, x1, y1, x2, y2, align, f2, b2, clip);
      return;
    };
    textout_draw_rect(out, f, &txt[sfrom], sto-sfrom, x1, y1, x2, y2, align, f2, b2, clip);
    if ( sto > sfrom ) {
      x1 += textlen(f, &txt[sfrom], sto-sfrom);
      if ( sto < strlen(txt) )
        textout_draw_rect(out, f, &txt[sto], size-sto, x1, y1, x2, y2, align, f1, b1, clip);
    };
  };
};


void draw_double_text ( BITMAP *out, GrFONT *f, unsigned char *txt, int size,
                        int x1, int y1, int x2, int y2, int align,
                        GrCOLOR f1, GrCOLOR f2, int clip ) {
  x1++;
  y1++;
  x2++;
  y2++;
  textout_draw_rect(out, f, txt, size, x1, y1, x2, y2, align, f1, TX_NOCOLOR, clip);
  x1--;
  y1--;
  x2--;
  y2--;
  textout_draw_rect(out, f, txt, size, x1, y1, x2, y2, align, f2, TX_NOCOLOR, clip);
};


void lined_rect ( BITMAP *b, int x1, int y1, int x2, int y2, GrCOLOR one, GrCOLOR two ) {
  while ( y1 < y2 ) {
    hline(b, x1, y1, x2-1, one);
    hline(b, x1+1, y1+1, x2, two);
    y1+=3;
  };
};


void fade_rect ( BITMAP *b, int x1, int y1, int x2, int y2, GrCOLOR one, GrCOLOR two, int direct ) {
  if ( !b ) return;
  if ( get_depth(b) > 8 && one != two ) {
	int r1 = getr(one);
    int g1 = getg(one);
    int b1 = getb(one);
    double rd = (double)(getr(two)-r1)/(double)256;
    double gd = (double)(getg(two)-g1)/(double)256;
    double bd = (double)(getb(two)-b1)/(double)256;
    if ( direct == 0x01 ) { /* horizontaly */
      double frame = (double)(x2-x1)/(double)256;
      int    x = 0;
      while ( x < 256 ) {
        int sx = x1 + (int)((double)x*frame);
        int ex = x1 + (int)((double)(x+1)*frame);
        int c = makecol(r1+(int)((double)x*rd),
                        g1+(int)((double)x*gd),
                        b1+(int)((double)x*bd));
        rectfill(b, sx, y1, ex, y2, c);
        x++;
      };
    } else { /* verticaly */
      double frame = (double)(y2-y1)/(double)256;
      int    y = 0;
      while ( y < 256 ) {
        int sy = y1 + (int)((double)y*frame);
        int ey = y1 + (int)((double)(y+1)*frame);
        int c = makecol(r1+(int)((double)y*rd),
                        g1+(int)((double)y*gd),
                        b1+(int)((double)y*bd));
        rectfill(b, x1, sy, x2, ey, c);
        y++;
      };
    };
  } else
    rectfill(b, x1, y1, x2, y2, one);
};


void light_image ( BITMAP *b, int x1, int y1, int x2, int y2, int dr, int dg, int db )
{
/* NB Should probably limit the colors max(min(rgb.r+dr,255),0) to prevent wrapping effects */
  int x = 0;
  int y = 0;
  while ( y < y2-y1 ) {
        while ( x < x2-x1 ) {
          int col = getpixel(b, x1+x, y1+y);
          RGB     rgb;
          rgb.r = getr(col);
          rgb.g = getg(col);
          rgb.b = getb(col);
          putpixel(b, x1+x, y1+y, makecol(rgb.r+dr, rgb.g+dg, rgb.b+db));
          x++;
        };
        x = 0;
        y++;
      };
};


void button ( BITMAP *b, int x1, int y1, int x2, int y2, GrCOLOR c1, GrCOLOR c2 ) {
  hline(b,x1,y2,x2,c2);
  vline(b,x2,y1,y2,c2);
  hline(b,x1,y1,x2,c1);
  vline(b,x1,y1,y2,c1);
};

int get_text_length ( void *f, unsigned char *str, int len ) {
  int l = 0;
  if (font) {
   if ( str && (len > 0) ) {
    /* its not a good idea to change someones data even temporarily -  it might even be in read-only memory for all you know.*/
    unsigned char *t=strdup(str);
    if(t){
     if(len<strlen(t))t[len]=0;
     l = text_length((FONT*)f, t);
     _free(t);
    }
   } else if ( str && (len < 0) ) {
     return text_length((FONT*)f, str);
   };
  }
  return l;
};


int get_char_length ( void *f, unsigned char ch ) {
 if(ch){
  unsigned char s[2] = {0, '\0'};
  s[0] = ch;
  return textlen((FONT*)f, (unsigned char*)s, 1);
 } else return 0;
};


int get_text_height ( void *f ) {
  return text_height((FONT*)f);
};

void draw_text_out ( BITMAP *b, void *f, unsigned char *text, int x1, int y1, GrCOLOR color )
{
  textout(b, (FONT*)f, text, x1, y1, color);
};

void textout_draw(BITMAP *bmp, void *f, unsigned char *s, int len,
                  int x1, int y1, int align, GrColor color, GrColor bcolor ) {
  if ( s && len && f ) {
    unsigned char t;
    len = (len<0)?strlen((char*)s):len;
    t = s[len];
    s[len] = '\0';
    if ( align & TX_ALIGN_CENTERX ) x1 -= textlen(f, s, -1)/2;
      else if ( align & TX_ALIGN_RIGHT ) x1 -= textlen(f, s, -1);
    if ( align & TX_ALIGN_CENTERY ) y1 -= textheight(f)/2;
      else if ( align & TX_ALIGN_BOTTOM ) y1 -= textheight(f);
    text_mode(-1);
    if ( bcolor >= -1 )
      rectfill(bmp, x1, y1, x1+textlen(f, s, len), y1+textheight(f), bcolor);
    (*drawtext)(bmp,f,s,x1,y1,color);
    if ( align & TX_UNDERLINE )
       hline(bmp, x1, y1+textheight(f)-1, x1+textlen(f, s, len), color);
    if ( align & TX_STRIKELINE )
      hline(bmp, x1, y1+(textheight(f)/2), x1+textlen(f, s, len), color);
    s[len] = t;
  };
};

void textout_draw_rect_ex(BITMAP *bmp, void *f, unsigned char *s, int len,
                  int x1, int y1, int x2, int y2, int align, GrColor color, GrColor bcolor,
                  int clip, int marge ) {
  if ( s && len && f ) {
    unsigned char t = 0;
    t_rect old = rect_empty;
    int sx1 = x1, sy1 = y1;
    //len = (len<0)?strlen((char*)s):len;
    if ( len >= 0 ) {
      t = s[len];
      s[len] = 0;
    };
    if ( clip ) {
	  t_rect clipr = rect_assign(x1,y1,x2,y2);
	  old = get_clip_rect(bmp);
      if ( bmp->clip )
        set_clip_rect(bmp, rect_cliped(old,clipr));
      else
        set_clip_rect(bmp, clipr);
    };
    if ( align & TX_ALIGN_CENTERX ) x1 += ((x2-sx1)-textlen(f, s, -1))>>1;
      else if ( align & TX_ALIGN_RIGHT ) x1 = x2-textlen(f, s, -1);
    if ( align & TX_ALIGN_CENTERY ) y1 += ((y2-sy1)-textheight(f))>>1;
      else if ( align & TX_ALIGN_BOTTOM ) y1 = y2-textheight(f);
    text_mode(-1);
    if ( bcolor >= 0 )
      rectfill(bmp, x1, y1, x1+textlen(f, s, len)+(marge*2), y1+textheight(f), bcolor);
    (*drawtext)(bmp,f,s,marge+x1,y1,color);
    if ( align & TX_UNDERLINE )
      hline(bmp, x1, y1+textheight(f)-1, x1+textlen(f, s, len), color);
    if ( align & TX_STRIKELINE )
      hline(bmp, x1, y1+(textheight(f)/2), x1+textlen(f, s, len), color);
    if ( clip ) set_clip_rect(bmp, old);
    if ( t ) s[len] = t;
  };
};


void textout_draw_rect(BITMAP *bmp, void *f, unsigned char *s, int len,
                  int x1, int y1, int x2, int y2, int align, GrColor color, GrColor bcolor,
                  int clip ) {
  textout_draw_rect_ex(bmp,f,s,len,x1,y1,x2,y2,align,color,bcolor,clip,0);
};

void textout_printf(BITMAP *bmp, void *f, int len,
                  int x1, int y1, int x2, int y2, int align, GrColor color, GrColor bcolor,
                  int clip, char *s, ... ) {
  static char buf[TEXTOUT_BUFFER_SIZE];
  /* I'm willing to let you to put 100Mb up in one go. Just not this way. Try using textout directly. */
  if ( s && len && f ) {
    char *t = NULL;
    int oldy2 = y2;
    va_list ap;
    va_start(ap, s);
    vsprintf(buf, s, ap);
    va_end(ap);

if(strlen(buf)>TEXTOUT_BUFFER_SIZE) {
/*
The overflow will have destroyed who knows what.
At least its no longer the stack since I made the buffer static
it will overflow into the next chunk of data in the uninitialised data segment actually.
Whatever it destroyed the result has got to be really bad. So.. hope we can still say something.
*/
 fputs("textout_printf has overflowed\n",stderr);
 exit(1);
}
    t = buf;
    while ( t && *t ) {
      char *oldt = t;
      y2  = y1+textheight(f);
      t = strchr(t, '\n'); /* find end-char */
      textout_draw_rect(bmp, f, oldt, strsize(oldt, t), x1, y1, x2, y2, align, color, bcolor, clip);
      if ( t ) { /* enter was found */
        t++; /* move to next char after enter */
        y1 += (*textheight)(f);
        if ( y1 > oldy2 ) return;
      };
    };
  };
};

void  draw_flip_sprite ( BITMAP *b, BITMAP *s, int x, int y, int flag ) {
  if ( flag & SF_FLIPX ) {
    if ( flag & SF_FLIPY ) draw_sprite_vh_flip(b, s, x, y);
      else draw_sprite_h_flip(b, s, x, y);
  } else if ( flag & SF_FLIPY ) draw_sprite_v_flip(b, s, x, y);
           else draw_sprite(b, s, x, y);
};

void __blit_hline ( BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int dw ) {
  sx %= source->w;
  sy %= source->h;
  sw = min(source->w-sx, min(sw, dw));
  while ( dw ) {
    blit(source, dest, sx, sy, dx, dy, sw, 1);
    sx = 0;
    dx += sw;
    dw -= sw;
    sw = min(source->w, dw);
  };
};

void blit_ex ( BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int sh, int dw, int dh )
{
  int ssw, ssx, ddw, ddx;
  if ( !source ) source = ScreenContext;
  if ( !dest ) dest = ScreenContext;
  sx %= source->w;
  sy %= source->h;
  sw = min(source->w-sx, min(sw, dw));
  sh = min(source->h-sy, min(sh, dh));
  ssw = sw; ssx = sx; ddw = dw; ddx = dx;
  while ( dh ) {
    sw = ssw;
    sx = ssx;
    dw = ddw;
    dx = ddx;
    while ( dw ) {
      blit(source, dest, sx, sy, dx, dy, sw, sh);
      sx = 0;
      dx += sw;
      dw -= sw;
      sw = min(source->w, dw);
    };
    sy = 0;
    dy += sh;
    dh -= sh;
    sh = min(source->h, dh);
  };
};

void blit_hline ( BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int dw ) {
  if ( !source ) source = ScreenContext;
  if ( !dest ) dest = ScreenContext;
  if ( !source->w || !source->h ) return;
  sx %= source->w;
  sy %= source->h;
  sw = min(source->w-sx, min(sw, dw));
  while ( dw ) {
    blit(source, dest, sx, sy, dx, dy, sw, 1);
    sx = 0;
    dx += sw;
    dw -= sw;
    sw = min(source->w, dw);
  };
};

/* blit_ellipsefill:
 *  Draws a blit filled circle. Without trying to use any hardware feature on the card - pity.
 */
void blit_circlefill(BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int radius)
{
   int cx = 0;
   int cy = radius;
   int df = 1 - radius;
   int d_e = 3;
   int d_se = -2 * radius + 5;
   int vx = (dx-radius)-sx;
   int vy = (dy-radius)-sy;
   if ( !source ) source = ScreenContext;
   if ( !dest ) dest = ScreenContext;
   do {
      __blit_hline(source, dest, (dx-cy)-vx, (dy-cx)-vy, dx-cy, dy-cx, sw, dx+cy-(dx-cy));
      if (cx)
        __blit_hline(source, dest, (dx-cy)-vx, (dy+cx)-vy, dx-cy, dy+cx, sw, dx+cy-(dx-cy));
      if (df < 0)  {
	 df += d_e;
	 d_e += 2;
	 d_se += 2;
      }
      else {
	 if (cx != cy) {
           __blit_hline(source, dest, (dx-cx)-vx, (dy-cy)-vy, dx-cx, dy-cy, sw, dx+cx-(dx-cx));
	    if (cy)
              __blit_hline(source, dest, (dx-cx)-vx, (dy+cy)-vy, dx-cx, dy+cy, sw, dx+cx-(dx-cx));
	 }
	 df += d_se;
	 d_e += 2;
	 d_se += 4;
	 cy--;
      }
      cx++;
   } while (cx <= cy);
}

/* blit_ellipsefill:
 *  Draws a blit filled ellipse. without using hardware - shame.
 */
void blit_ellipsefill(BITMAP *source, BITMAP *dest, int sx, int sy, int dx, int dy, int sw, int rx, int ry)
{
   int x, y;
   int a, b, c, d;
   int da, db, dc, dd;
   int na, nb, nc, nd;
   int vx = (dx-rx)-sx, vy = (dy-ry)-sy;
   if ( !source ) source = ScreenContext;
   if ( !dest ) dest = ScreenContext;
   if (rx < 1)
      rx = 1;
   if (ry < 1)
      ry = 1;
   if (rx > ry) {
      dc = -1;
      dd = 0xFFFF;
      x = 0;
      y = rx * 64;
      na = 0;
      nb = (y + 32) >> 6;
      nc = 0;
      nd = (nb * ry) / rx;
      do {
       a = na;
       b = nb;
       c = nc;
       d = nd;
       x = x + (y / rx);
       y = y - (x / rx);
       na = (x + 32) >> 6;
       nb = (y + 32) >> 6;
       nc = (na * ry) / rx;
       nd = (nb * ry) / rx;
       if ((c > dc) && (c < dd)) {
         __blit_hline(source, dest, (dx-b)-vx, (dy+c)-vy, dx-b, dy+c, sw, dx+b-(dx-b));
         if (c)
         __blit_hline(source, dest, (dx-b)-vx, (dy-c)-vy, dx-b, dy-c, sw, dx+b-(dx-b));
         dc = c;
       }
       if ((d < dd) && (d > dc)) {
         __blit_hline(source, dest, (dx-a)-vx, (dy+d)-vy, dx-a, dy+d, sw, dx+a-(dx-a));
         __blit_hline(source, dest, (dx-a)-vx, (dy-d)-vy, dx-a, dy-d, sw, dx+a-(dx-a));
         dd = d;
       }
      } while(b > a);
   }
   else {
      da = -1;
      db = 0xFFFF;
      x = 0;
      y = ry * 64;
      na = 0;
      nb = (y + 32) >> 6;
      nc = 0;
      nd = (nb * rx) / ry;
      do {
       a = na;
       b = nb;
       c = nc;
       d = nd;
       x = x + (y / ry);
       y = y - (x / ry);
       na = (x + 32) >> 6;
       nb = (y + 32) >> 6;
       nc = (na * rx) / ry;
       nd = (nb * rx) / ry;
       if ((a > da) && (a < db)) {
         __blit_hline(source, dest, (dx-d)-vx, (dy+a)-vy, dx-d, dy+a, sw, dx+d-(dx-d));
         if (a)
          __blit_hline(source, dest, (dx-d)-vx, (dy-a)-vy, dx-d, dy-a, sw, dx+d-(dx-d));
          da = a;
       }
       if ((b < db) && (b > da)) {
         __blit_hline(source, dest, (dx-c)-vx, (dy+b)-vy, dx-c, dy+b, sw, dx+c-(dx-c));
         __blit_hline(source, dest, (dx-c)-vx, (dy-b)-vy, dx-c, dy-b, sw, dx+c-(dx-c));
         db = b;
       }
      } while(b > a);
   }
}

#ifndef uchar
#define uchar        unsigned char
#endif
#ifndef ulong
#define ulong        unsigned long
#endif
#define lrgb         long

#define sizeuchar    1
#define sizeint      2
#define sizelrgb     3
#define sizelong     4

BITMAP *gen_neg ( int depth, BITMAP *src ) { // if depth == 0 create standard bitmap
  #define loop_neg(d,s,rgb1,rgb2)                           \
    while ( w-- ) {                                         \
      ssdat = *((s*)sdat);                                  \
        rcol  = getr##rgb2(ssdat);                          \
        gcol  = getg##rgb2(ssdat);                          \
        bcol  = getb##rgb2(ssdat);                          \
                                                            \
        rcol = 255-rcol;                                    \
        gcol = 255-gcol;                                    \
        bcol = 255-bcol;                                    \
                                                            \
        *((d*)bdat) = (d)makecol##rgb1(rcol, gcol, bcol);   \
      sdat += size##s;                                      \
      bdat += size##d;                                      \
    }
  int y = 0;
  int w = src->w, h = src->h;
  int ssdat = 0;
  unsigned char *sdat, *bdat;
  unsigned char rcol, gcol, bcol;
  BITMAP *bmp = NULL;
  if ( depth ) bmp = create_bitmap_ex(depth, src->w, src->h);
    else bmp = create_bitmap(src->w, src->h);
  if ( !bmp ) return NULL;
  h = min(h, bmp->h);
  while ( h-- ) {
    sdat = src->line[y];
    bdat = bmp->line[y];
    switch ( bmp->vtable->color_depth ) {
      case 8: switch ( src->vtable->color_depth ) {
             case 8 : loop_neg(uchar, uchar, 8, 8); break;
             case 15: loop_neg(uchar, int, 8, 15); break;
             case 16: loop_neg(uchar, int, 8, 16); break;
             case 24: loop_neg(uchar, lrgb, 8, 24); break;
             case 32: loop_neg(uchar, long, 8, 32); break;
           }; break;
      case 15: switch ( src->vtable->color_depth ) {
             case 8 : loop_neg(int, uchar, 15, 8); break;
             case 15: loop_neg(int, int, 15, 15); break;
             case 16: loop_neg(int, int, 15, 16); break;
             case 24: loop_neg(int, lrgb, 15, 24); break;
             case 32: loop_neg(int, long, 15, 32); break;
           }; break;
      case 16: switch ( src->vtable->color_depth ) {
             case 8 : loop_neg(int, uchar, 16, 8); break;
             case 15: loop_neg(int, int, 16, 15); break;
             case 16: loop_neg(int, int, 16, 16); break;
             case 24: loop_neg(int, lrgb, 16, 24); break;
             case 32: loop_neg(int, long, 16, 32); break;
           }; break;
      case 24: switch ( src->vtable->color_depth ) {
             case 8 : loop_neg(lrgb, uchar, 24, 8); break;
             case 15: loop_neg(lrgb, int, 24, 15); break;
             case 16: loop_neg(lrgb, int, 24, 16); break;
             case 24: loop_neg(lrgb, lrgb, 24, 24); break;
             case 32: loop_neg(lrgb, long, 24, 32); break;
           }; break;
      case 32: switch ( src->vtable->color_depth ) {
             case 8 : loop_neg(long, uchar, 32, 8); break;
             case 15: loop_neg(long, int, 32, 15); break;
             case 16: loop_neg(long, int, 32, 16); break;
             case 24: loop_neg(long, lrgb, 32, 24); break;
             case 32: loop_neg(long, long, 32, 32); break;
           }; break;
    };
    y++;
    w = min(src->w, bmp->w);
  };
  return bmp;
};

BITMAP *gen_emboss ( int depth, BITMAP *src ) { // if depth == 0 create standard bitmap
  #define loop_print(d,s,rgb1,rgb2)                         \
    while ( w-- ) {                                         \
      ssdat = *((s*)sdat);                                  \
      dddat = *((s*)ddat);                                  \
        rcol  = getr##rgb2(ssdat);                          \
        gcol  = getg##rgb2(ssdat);                          \
        bcol  = getb##rgb2(ssdat);                          \
                                                            \
        drcol  = getr##rgb2(dddat);                         \
        dgcol  = getg##rgb2(dddat);                         \
        dbcol  = getb##rgb2(dddat);                         \
                                                            \
        rcol = (rcol+(255-drcol))>>1;                       \
        gcol = (gcol+(255-dgcol))>>1;                       \
        bcol = (bcol+(255-dbcol))>>1;                       \
                                                            \
        *((d*)bdat) = (d)makecol##rgb1(rcol, gcol, bcol);   \
      sdat += size##s;                                      \
      ddat += size##s;                                      \
      bdat += size##d;                                      \
    }
  int y = 0;
  int w = src->w-1, h = src->h-1;
  int ssdat = 0;
  int dddat = 0;
  unsigned char *sdat, *ddat, *bdat;
  unsigned char rcol, gcol, bcol, drcol, dgcol, dbcol;
  BITMAP *bmp = NULL;
  if ( depth ) bmp = create_bitmap_ex(depth, src->w, src->h);
    else bmp = create_bitmap(src->w, src->h);
  if ( !bmp ) return NULL;
  clear(bmp);
  h = min(h, bmp->h-1);
  if ( (w > 1) && (h > 1) )
  while ( h-- ) {
    sdat = src->line[y];
    ddat = src->line[y+1]+2;
    bdat = bmp->line[y];
    switch ( bmp->vtable->color_depth ) {
      case 8: switch ( src->vtable->color_depth ) {
             case 8 : loop_print(uchar, uchar, 8, 8); break;
             case 15: loop_print(uchar, int, 8, 15); break;
             case 16: loop_print(uchar, int, 8, 16); break;
             case 24: loop_print(uchar, lrgb, 8, 24); break;
             case 32: loop_print(uchar, long, 8, 32); break;
           }; break;
      case 15: switch ( src->vtable->color_depth ) {
             case 8 : loop_print(int, uchar, 15, 8); break;
             case 15: loop_print(int, int, 15, 15); break;
             case 16: loop_print(int, int, 15, 16); break;
             case 24: loop_print(int, lrgb, 15, 24); break;
             case 32: loop_print(int, long, 15, 32); break;
           }; break;
      case 16: switch ( src->vtable->color_depth ) {
             case 8 : loop_print(int, uchar, 16, 8); break;
             case 15: loop_print(int, int, 16, 15); break;
             case 16: loop_print(int, int, 16, 16); break;
             case 24: loop_print(int, lrgb, 16, 24); break;
             case 32: loop_print(int, long, 16, 32); break;
           }; break;
      case 24: switch ( src->vtable->color_depth ) {
             case 8 : loop_print(lrgb, uchar, 24, 8); break;
             case 15: loop_print(lrgb, int, 24, 15); break;
             case 16: loop_print(lrgb, int, 24, 16); break;
             case 24: loop_print(lrgb, lrgb, 24, 24); break;
             case 32: loop_print(lrgb, long, 24, 32); break;
           }; break;
      case 32: switch ( src->vtable->color_depth ) {
             case 8 : loop_print(long, uchar, 32, 8); break;
             case 15: loop_print(long, int, 32, 15); break;
             case 16: loop_print(long, int, 32, 16); break;
             case 24: loop_print(long, lrgb, 32, 24); break;
             case 32: loop_print(long, long, 32, 32); break;
           }; break;
    };
    y++;
    w = min(src->w-1, bmp->w-1);
  };
  return bmp;
};

BITMAP *conv_to_skipcolor_bitmap ( BITMAP *ctx, int r, int g, int b ) {
  int x, y = 0;
  l_color skip = makecol(r,g,b);
  if ( !ctx || (ctx == screen) ) return NULL;
  if ( skip == ctx->vtable->mask_color ) return ctx;
  for ( y = 0; y < ctx->h; y++ )
    for ( x = 0; x < ctx->w; x++ ) {
      l_color c = getpixel(ctx, x, y);
      if ( skip == c )
        putpixel(ctx, x, y, ctx->vtable->mask_color);
      if ( c == ctx->vtable->mask_color )
        if ( !ctx->vtable->mask_color )
          putpixel(ctx, x, y, makecol(0,36,0));
        else
          putpixel(ctx, x, y, makecol(255,36,255));
    };
  return ctx;
};

DATAFILE *conv_to_skipcolor_data ( DATAFILE *f, int r, int g, int b ) {
  int c = 0;
  if ( f )
   for (c=0; f[c].type != DAT_END; c++) {
      if ( f[c].type == DAT_BITMAP )
         f[c].dat = (BITMAP*)conv_to_skipcolor_bitmap((BITMAP*)(f[c].dat), r, g, b);
      else
      if ( f[c].type == DAT_FILE )
         conv_to_skipcolor_data((DATAFILE*)(f[c].dat), r, g, b);
   };
  return f;
};


ICONLIBRARY  *load_icon_library ( char *filename, int r, int g, int b )
{
    DATAFILE *dat = load_skip_datafile ( filename, r, g, b );
    return convert_to_icon_library ( dat );
};


void  unload_icon_library ( ICONLIBRARY *icl )
{
   while ( icl ) {
     ICONLIBRARY *i = icl->next;
     if ( icl->datfile ) unload_datafile(icl->datfile);
     _free(icl);
     icl = i;
   };

};


BITMAP  *get_icon_from_library ( ICONLIBRARY *icl, int id, int size )
{
   while ( icl ) {
      if ( icl->id == id ) {
          if ( size == ICON_32 ) {
             if ( icl->icon32 ) return icl->icon32;
             else return icl->icon16;
          } else
             if ( icl->icon16 ) return icl->icon16;
             else return icl->icon32;
          return icl->icon32;
      };
      icl = icl->next;
   };
   return NULL;
};


static ICONLIBRARY  *_convert_to_icon_library ( DATAFILE *f, ICONLIBRARY *icl, int id )
{
  int c = 0;
  int h = 0;
  if ( f ) {
     ICONLIBRARY *i = NULL;
     if ( id ) {
       i = (ICONLIBRARY*)_malloc(sizeof(ICONLIBRARY));
       memset(i, 0, sizeof(ICONLIBRARY));
       if ( !icl ) i->datfile = f;
       i->id = id-1;
     };
     for (c=0; f[c].type != DAT_END; c++) {
          if ( i && (f[c].type == DAT_BITMAP) && (h < 2) ) {
            if ( h == 1 ) i->icon16 = (BITMAP*)(f[c].dat);
            else
            if ( h == 0 ) i->icon32 = (BITMAP*)(f[c].dat);
            h++;
          } else
          if ( f[c].type == DAT_FILE ) {
             ICONLIBRARY *t;
             id++;
             t = _convert_to_icon_library ( (DATAFILE*)(f[c].dat), i, id );
             t->next = i;
             i = t;
          };
     };
     return i;
  };
  return NULL;
};


ICONLIBRARY  *convert_to_icon_library ( DATAFILE *f )
{
  return _convert_to_icon_library(f, NULL, 0);
};



void *get_datafile_object ( DATAFILE *dat, int obj_num )
{
  int c = 0;
  if ( dat )
    for (c=0; dat[c].type != DAT_END; c++) {
      if ( c == obj_num ) return dat[c].dat;
    };
  return NULL;
};


DATAFILE *load_skip_datafile ( char *filename, int r, int g, int b ) {
  DATAFILE *f = load_datafile(filename);
  f = conv_to_skipcolor_data(f, r, g, b);
  return f;
};

int  _gr_setmode ( int _s, int _w, int _h, int _v_w, int _v_h, char _depth )
{
/* new allegro insists on 8 15 16 24 or 32 (in debug mode) */
  set_color_depth((_depth<8)?8:_depth);
  if ( set_gfx_mode(_s, _w, _h, _v_w, _v_h) ) {
    return 0;
  };
  if ( (_depth < 15) && (_s >= 0) ) {
     PALLETE pal;
     generate_332_palette(pal);
     pal[0].r = 0;
     pal[0].g = 0;
     pal[0].b = 0;
     set_pallete(pal);
  };
  return 1;
};



extern FONT* load_ttf_font_ex (const char* filename, const int points_w, const int points_h, const int smooth);
extern l_int font_smooth;
extern FONT *load_font(char *f);


static void *_seal_load_font ( char *_filename, char *xxname, int w, int h, int from, int to ) {
  /* you shouldnt make an 8 point font anything but 8 point anyhow */
  void *f=NULL;
  if(_filename){
   /* f=load_font(_filename);
   if ( !f )*/ f = (void*)load_ttf_font_ex2(GetFile(_filename), w, h, 32, 255, font_smooth);
  }
  return f;
};

static void  _destroy_font ( void *f ) {
  destroy_font((FONT*)f);
};

void   *(*seal_load_font)(char* _filename, char *xxname, int w, int h, int from, int to) = &_seal_load_font;

void    (*unload_font)(void *f) = &_destroy_font;

