/****************************************************************/
/*                                                              */
/*                           newscr.c                           */
/*                                                              */
/*                Seal - free desktop environment               */
/*                                                              */
/*                    Copyright (c) 1999,2000                   */
/*                         Michal Stencl                        */
/*                     All Rights Reserved                      */
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


GFX_VTABLE *oldscreenvtable = NULL;
GFX_VTABLE  newscreenvtable;

int  sd_getpixel(struct BITMAP *bmp, int x, int y) {
  _drawing_in_screen++;
  oldscreenvtable->getpixel(bmp, x, y);
  _drawing_in_screen--;
};

void sd_putpixel(struct BITMAP *bmp, int x, int y, int color)
{
  _drawing_in_screen++;
  oldscreenvtable->putpixel(bmp, x, y, color);
  _drawing_in_screen--;
};

void sd_vline(struct BITMAP *bmp, int x, int y1, int y2, int color) {
  _drawing_in_screen++;
  oldscreenvtable->vline(bmp, x, y1, y2, color);
  _drawing_in_screen--;
};

void sd_hline(struct BITMAP *bmp, int x1, int y, int x2, int color) {
  _drawing_in_screen++;
  oldscreenvtable->hline(bmp, x1, y, x2, color);
  _drawing_in_screen--;
};

void sd_line(struct BITMAP *bmp, int x1, int y1, int x2, int y2, int color) {
  _drawing_in_screen++;
  oldscreenvtable->line(bmp, x1, y1, x2, y2, color);
  _drawing_in_screen--;
};

void sd_rectfill(struct BITMAP *bmp, int x1, int y1, int x2, int y2, int color) {
  _drawing_in_screen++;
  oldscreenvtable->rectfill(bmp, x1, y1, x2, y2, color);
  _drawing_in_screen--;
};

void sd_draw_sprite(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y) {
  _drawing_in_screen++;
  oldscreenvtable->draw_sprite(bmp, sprite, x, y);
  _drawing_in_screen--;
};

void sd_draw_256_sprite(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y) {
  _drawing_in_screen++;
  oldscreenvtable->draw_256_sprite(bmp, sprite, x, y);
  _drawing_in_screen--;
};

void sd_draw_sprite_v_flip(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y) {
  _drawing_in_screen++;
  oldscreenvtable->draw_sprite_v_flip(bmp, sprite, x, y);
  _drawing_in_screen--;
};

void sd_draw_sprite_h_flip(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y) {
  _drawing_in_screen++;
  oldscreenvtable->draw_sprite_h_flip(bmp, sprite, x, y);
  _drawing_in_screen--;
};

void sd_draw_sprite_vh_flip(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y) {
  _drawing_in_screen++;
  oldscreenvtable->draw_sprite_vh_flip(bmp, sprite, x, y);
  _drawing_in_screen--;
};

void sd_draw_trans_sprite(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y) {
  _drawing_in_screen++;
  oldscreenvtable->draw_trans_sprite(bmp, sprite, x, y);
  _drawing_in_screen--;
};

void sd_draw_lit_sprite(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y, int color) {
  _drawing_in_screen++;
  oldscreenvtable->draw_lit_sprite(bmp, sprite, x, y, color);
  _drawing_in_screen--;
};

void sd_draw_rle_sprite(struct BITMAP *bmp, struct RLE_SPRITE *sprite, int x, int y) {
  _drawing_in_screen++;
  oldscreenvtable->draw_rle_sprite(bmp, sprite, x, y);
  _drawing_in_screen--;
};

void sd_draw_trans_rle_sprite(struct BITMAP *bmp, struct RLE_SPRITE *sprite, int x, int y) {
  _drawing_in_screen++;
  oldscreenvtable->draw_trans_rle_sprite(bmp, sprite, x, y);
  _drawing_in_screen--;
};

void sd_draw_lit_rle_sprite(struct BITMAP *bmp, struct RLE_SPRITE *sprite, int x, int y, int color) {
  _drawing_in_screen++;
  oldscreenvtable->draw_lit_rle_sprite(bmp, sprite, x, y, color);
  _drawing_in_screen--;
};

void sd_draw_character(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y, int color) {
  _drawing_in_screen++;
  oldscreenvtable->draw_character(bmp, sprite, x, y, color);
  _drawing_in_screen--;
};

void sd_textout_fixed(struct BITMAP *bmp, void *f, int h, unsigned char *str, int x, int y, int color) {
  _drawing_in_screen++;
  oldscreenvtable->textout_fixed(bmp, f, h, str, x, y, color);
  _drawing_in_screen--;
};

void sd_blit_from_memory(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height){
  _drawing_in_screen++;
  oldscreenvtable->blit_from_memory(source, dest, source_x, source_y, dest_x, dest_y, width, height);
  _drawing_in_screen--;
};

void sd_blit_to_memory(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height) {
  _drawing_in_screen++;
  oldscreenvtable->blit_to_memory(source, dest, source_x, source_y, dest_x, dest_y, width, height);
  _drawing_in_screen--;
};

void sd_blit_to_self(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height) {
  _drawing_in_screen++;
  oldscreenvtable->blit_to_self(source, dest, source_x, source_y, dest_x, dest_y, width, height);
  _drawing_in_screen--;
};

void sd_blit_to_self_forward(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height) {
  _drawing_in_screen++;
  oldscreenvtable->blit_to_self_forward(source, dest, source_x, source_y, dest_x, dest_y, width, height);
  _drawing_in_screen--;
};

void sd_blit_to_self_backward(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height) {
  _drawing_in_screen++;
  oldscreenvtable->blit_to_self_backward(source, dest, source_x, source_y, dest_x, dest_y, width, height);
  _drawing_in_screen--;
};

void sd_masked_blit(struct BITMAP *source, struct BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height) {
  _drawing_in_screen++;
  oldscreenvtable->masked_blit(source, dest, source_x, source_y, dest_x, dest_y, width, height);
  _drawing_in_screen--;
};

void sd_clear_to_color(struct BITMAP *bitmap, int color) {
  _drawing_in_screen++;
  oldscreenvtable->clear_to_color(bitmap, color);
  _drawing_in_screen--;
};

void init_newscreenvtable() {
  if ( screen && !oldscreenvtable ) {
    memcpy(&newscreenvtable, screen->vtable, sizeof(GFX_VTABLE));
    newscreenvtable.getpixel = &sd_getpixel;
    newscreenvtable.putpixel = &sd_putpixel;
    newscreenvtable.vline = &sd_vline;
    newscreenvtable.hline = &sd_hline;
    newscreenvtable.line = &sd_line;
    newscreenvtable.rectfill = &sd_rectfill;
    newscreenvtable.draw_sprite = &sd_draw_sprite;
    newscreenvtable.draw_256_sprite = &sd_draw_256_sprite;
    newscreenvtable.draw_sprite_v_flip = &sd_draw_sprite_v_flip;
    newscreenvtable.draw_sprite_h_flip = &sd_draw_sprite_h_flip;
    newscreenvtable.draw_sprite_vh_flip = &sd_draw_sprite_vh_flip;
    newscreenvtable.draw_trans_sprite = &sd_draw_trans_sprite;
    newscreenvtable.draw_lit_sprite = &sd_draw_lit_sprite;
    newscreenvtable.draw_rle_sprite = &sd_draw_rle_sprite;
    newscreenvtable.draw_trans_rle_sprite = &sd_draw_trans_rle_sprite;
    newscreenvtable.draw_lit_rle_sprite = &sd_draw_lit_rle_sprite;
    newscreenvtable.draw_character = &sd_draw_character;
    newscreenvtable.textout_fixed = &sd_textout_fixed;
    newscreenvtable.blit_from_memory = &sd_blit_from_memory;
    newscreenvtable.blit_to_memory = &sd_blit_to_memory;
    newscreenvtable.blit_to_self =  &sd_blit_to_self;
    newscreenvtable.blit_to_self_forward = &sd_blit_to_self_forward;
    newscreenvtable.blit_to_self_backward = &sd_blit_to_self_backward;
    newscreenvtable.masked_blit = &sd_masked_blit;
    newscreenvtable.clear_to_color = &sd_clear_to_color;
    oldscreenvtable = screen->vtable;
    screen->vtable = &newscreenvtable;
  } else if ( oldscreenvtable && screen ) screen->vtable = &newscreenvtable;
};

void done_newscreenvtable() {
  if ( screen && screen->vtable == &newscreenvtable && oldscreenvtable ) screen->vtable = oldscreenvtable;
};

