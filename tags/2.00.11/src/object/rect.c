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
#include <object.h>

// postion and origin functions ////////////////////////////////////////////////

t_rect rect_empty = {{-0xFFFF, -0xFFFF},{ -0xFFFF, -0xFFFF}};

inline t_point point_assign ( l_rect x, l_rect y )
{
  t_point p;
  p.x = x;
  p.y = y;
  return p;
};

inline t_rect  rect_assign ( l_rect ax, l_rect ay, l_rect bx, l_rect by )
{
  t_rect r;
  r.a.x = ax;
  r.a.y = ay;
  r.b.x = bx;
  r.b.y = by;
  return r;
};

inline t_rect  rect_move ( t_rect r, l_rect mx, l_rect my )
{
  r.a.x += mx;
  r.a.y += my;
  r.b.x += mx;
  r.b.y += my;
  return r;
};

inline t_point rect_size ( t_rect r )
{
  t_point p;
  p.x = r.b.x - r.a.x;
  p.y = r.b.y - r.a.y;
  return p;
};


inline l_bool  rect_overlay ( t_rect r, t_rect d )
{
  if ( r.a.x > d.b.x || r.a.y > d.b.y ||
       r.b.x < d.a.x || r.b.y < d.a.y ) return false;
  return true;
};

inline t_rect  rect_cliped ( t_rect r, t_rect d )
{
  if  ( !rect_overlay(r, d) ) return rect_empty;
  r.b.x = max(r.a.x, min(r.b.x, d.b.x));
  r.b.y = max(r.a.y, min(r.b.y, d.b.y));
  r.a.x = min(r.b.x, max(r.a.x, d.a.x));
  r.a.y = min(r.b.y, max(r.a.y, d.a.y));
  return r;
};



inline l_rect rect_sizex ( t_rect r ) {
  return r.b.x-r.a.x;
};


inline l_rect rect_sizey ( t_rect r ) {
  return r.b.y-r.a.y;
};


inline l_bool rect_check_empty ( t_rect r )
{
  if ( (r.a.x == rect_empty.a.x) && (r.b.x == rect_empty.b.x) &&
       (r.a.y == rect_empty.a.y) && (r.b.y == rect_empty.b.y) ) return true;
  return false;
};


inline l_bool rect_contains ( t_rect r, t_point p )
{
  if ( (r.a.x <= p.x) && (r.a.y <= p.y) && (r.b.x >= p.x) && (r.b.y >= p.y) )
    return true;
  return false;
};


inline l_bool  rect_equals ( t_rect r, t_rect nr )
{
  if ( (r.a.x != nr.a.x) || (r.a.y != nr.a.y) || (r.b.x != nr.b.x) ||
       (r.b.y != nr.b.y) ) return false;
  return true;
};


void  rect_double_overlay ( t_rect* fr, t_rect *lr )
{
  t_rect r   = *fr;
  t_rect xfr = *fr;
  t_rect xlr = *lr;
  if ( !rect_overlay(*fr, *lr) ) {
    *(fr) = rect_empty;
    *(lr) = rect_empty;
    return;
  };
  r = rect_cliped(r, *lr);
  if ( r.a.x > (*lr).a.x ) (*lr).b.x = r.a.x - 1;
  if ( r.b.x < (*lr).b.x ) (*lr).a.x = r.b.x + 1;
  if ( r.b.y < (*lr).b.y ) *fr = rect_assign(r.a.x, r.b.y + 1, r.b.x, (*lr).b.y);
  if ( r.a.y > (*lr).a.y ) *fr = rect_assign(r.a.x, (*lr).a.y, r.b.x, r.a.y - 1);
  if ( rect_equals(xfr, *fr) ) *fr = rect_empty;
  if ( rect_equals(xlr, *lr) ) *lr = rect_empty;
};
////////////////////////////////////////////////////////////////////////////////
