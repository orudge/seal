/****************************************************************/
/*                                                              */
/*                          newscr.h                            */
/*                                                              */
/*                Seal - free desktop environment               */
/*                                                              */
/*                  Copyright (c) 1999,2000                     */
/*                       Michal Stencl                          */
/*                    All Rights Reserved                       */
/*                                                              */
/* mail : stenclpmd@ba.telecom.sk                               */
/* web  : http://www.home.sk/public/seal/                       */
/*                                                              */
/* This file is part of Seal.                                   */
/*                                                              */
/* Seal is free software; you can redistribute it and/or        */
/* modify it under the terms of the GNU General Public License  */
/* as published by the Free Software Foundation; either version */
/* 2, or (at your option) any later version.                    */
/*                                                              */
/* Seal is distributed in the hope that it will be useful, but  */
/* WITHOUT ANY WARRANTY; without even the implied warranty of   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See    */
/* the GNU General Public License for more details.             */
/*                                                              */
/* You should have received a copy of the GNU General Public    */
/* License along with Seal; see the file COPYING.  If not,      */
/* write to the Free Software Foundation, 675 Mass Ave,         */
/* Cambridge, MA 02139, USA.                                    */
/*                                                              */
/****************************************************************/



#ifndef __NEWSCR_H_INCLUDED__
#define __NEWSCR_H_INCLUDED__

extern GFX_VTABLE *oldscreenvtable;
extern GFX_VTABLE  newscreenvtable;

int  sd_getpixel(struct BITMAP *bmp, int x, int y);
void sd_putpixel(struct BITMAP *bmp, int x, int y, int color);
void sd_vline(struct BITMAP *bmp, int x, int y1, int y2, int color);
void sd_hline(struct BITMAP *bmp, int x1, int y, int x2, int color);
void sd_line(struct BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
void sd_rectfill(struct BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
void sd_draw_sprite(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y);
void sd_draw_256_sprite(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y);
void sd_draw_sprite_v_flip(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y);
void sd_draw_sprite_h_flip(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y);
void sd_draw_sprite_vh_flip(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y);
void sd_draw_trans_sprite(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y );
void sd_draw_lit_sprite(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y, int color);
void sd_draw_rle_sprite(struct BITMAP *bmp, struct RLE_SPRITE *sprite, int x, int y);
void sd_draw_trans_rle_sprite(struct BITMAP *bmp, struct RLE_SPRITE *sprite, int x, int y);
void sd_draw_lit_rle_sprite(struct BITMAP *bmp, struct RLE_SPRITE *sprite, int x, int y, int color);
void sd_draw_character(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y, int color);
void sd_textout_fixed(struct BITMAP *bmp, void *f, int h, unsigned char *str, int x, int y, int color);
void sd_blit_from_memory(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void sd_blit_to_memory(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void sd_blit_to_self(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void sd_blit_to_self_forward(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void sd_blit_to_self_backward(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void sd_masked_blit(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
void sd_clear_to_color(struct BITMAP *bitmap, int color);

void init_newscreenvtable();
void done_newscreenvtable();

#endif

