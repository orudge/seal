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
#include <skin.h>

l_bool use_skins = true;
l_text skin_file = "skin.ini";

void draw_skin ( BITMAP *out, p_skin o, t_rect r, t_rect *z ) {
if ( out && o ) {
  l_int bw = IMAGE_WIDTH(o->bmp);
  l_int bh = IMAGE_HEIGHT(o->bmp);
  t_rect d  = o->d;
  t_point p = point_assign(r.a.x,r.a.y);

  r = rect_move(r,-p.x,-p.y);

  r.b.x++;
  r.b.y++;
  /////////////////
  //*1 //*4 //*7 //
  /////////////////
  //*2 //*5 //*8 //
  /////////////////
  //*3 //*6 //*9 //
  /////////////////     *x => done

  if ( d.a.x && d.a.y )
    masked_blit( o->bmp, out, 0,        0,        p.x+r.a.x,       p.y+r.a.y,       d.a.x, d.a.y );  // 1
  if ( d.a.x && d.b.y )
    masked_blit( o->bmp, out, 0,        bh-d.b.y, p.x+r.a.x,       p.y+r.b.y-d.b.y, d.a.x, d.b.y );  // 3
  if ( d.b.x && d.a.y )
    masked_blit( o->bmp, out, bw-d.b.x, 0,        p.x+r.b.x-d.b.x, p.y+r.a.y,       d.b.x, d.a.y );  // 7
  if ( d.b.x && d.b.y )
    masked_blit( o->bmp, out, bw-d.b.x, bh-d.b.y, p.x+r.b.x-d.b.x, p.y+r.b.y-d.b.y, d.b.x, d.b.y );  // 9

  if ( d.a.x )
    masked_stretch_blit(o->bmp,out, 0,     d.a.y,       d.a.x, bh-d.a.y-d.b.y,
                             p.x+r.a.x, p.y+r.a.y+d.a.y, d.a.x, r.b.y-r.a.y-d.a.y-d.b.y);  //2
  if ( d.a.y )
    masked_stretch_blit(o->bmp,out, d.a.x,       0,     bw-d.a.x-d.b.x,          d.a.y,
                             p.x+r.a.x+d.a.x, p.y+r.a.y, r.b.x-d.a.x-d.b.x-r.a.x, d.a.y);  //4
  if ( d.b.y )
    masked_stretch_blit(o->bmp,out, d.a.x,       bh-d.b.y,    bw-d.a.x-d.b.x,          d.b.y,
                             p.x+r.a.x+d.a.x, p.y+r.b.y-d.b.y, r.b.x-d.a.x-d.b.x-r.a.x, d.b.y);  //6
  if ( d.b.x )
    masked_stretch_blit(o->bmp,out, bw-d.b.x,    d.a.y,       d.b.x, bh-d.a.y-d.b.y,
                             p.x+r.b.x-d.b.x, p.y+r.a.y+d.a.y, d.b.x, r.b.y-r.a.y-d.a.y-d.b.y);  //8

  masked_stretch_blit(o->bmp,out, d.a.x,       d.a.y,       bw-d.a.x-d.b.x,          bh-d.a.y-d.b.y,
                           p.x+r.a.x+d.a.x, p.y+r.a.y+d.a.y, r.b.x-d.a.x-d.b.x-r.a.x, r.b.y-r.a.y-d.a.y-d.b.y);  //5

  if ( z ) (*z) = rect_assign ( p.x+d.a.x, p.y+d.a.y, p.x+r.b.x-d.b.x, p.y+r.b.y-d.b.y );

};
};

t_point get_skin_space ( p_skin o, l_int w, l_int h ) {
  return point_assign(o->d.a.x+o->d.b.x+w,o->d.a.y+o->d.b.y+h);
};
l_int get_skin_height ( p_skin o, l_int h ) {
  return o->d.a.y+o->d.b.y+h;
};
l_int get_skin_width ( p_skin o, l_int w ) {
  return o->d.a.x+o->d.b.x+w;
};

p_skin get_skin ( l_text skinfile, l_text section ) {
  l_text bmp;
  ini_rgb *txt;
  p_skin o =_malloc(sizeof(t_skin));

  if ( !o ) return NULL;

  clear_type(o,sizeof(t_skin));
  bmp = getini_fromfile ( skinfile,  section , "bmp" );

  if ( !bmp ) { free(o); return NULL; };

  o->d.a.x = getininum_fromfile (skinfile,  section, "ax" );
  o->d.a.y = getininum_fromfile (skinfile,  section, "ay" );
  o->d.b.x = getininum_fromfile (skinfile,  section, "bx" );
  o->d.b.y = getininum_fromfile (skinfile,  section, "by" );

  bmp = getini_fromfile ( skinfile,  section , "bmp" );
  if ( bmp[0] == '&' )  // if call an internal bitmap (ex: "&IMG_FILE16")
    o->bmp = GETBMP(bmp+1);
  else
    o->bmp = load_image( bmp );

  txt = getini_color( skinfile, section, "text" );
  if ( txt ) o->text = makecol(txt->r,txt->g,txt->b);

  return o;
};

void init_skins ( void ) {
  if ( get_depth(screen) < 16 ) {
    use_skins = false;
  } else {
    if ( key_exists("current/seal/skin/file") ) skin_file = get_key("current/seal/skin/file");
    if ( key_exists("current/seal/skin/use") ) use_skins =  ( get_key_integer("current/seal/skin/use") ) ? 1 : 0;
  };
};
