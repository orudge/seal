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

/* Revision History:
 *
 * 30/03/2002 - Reformatted code a bit and added skin support (orudge)
 */

#include <seal.h>
#include <checkbox.h>

p_skin skin_checkbox_up             = NULL;
p_skin skin_checkbox_down           = NULL;
p_skin skin_checkbox_greyed         = NULL;
p_skin skin_checkbox_greyed_checked = NULL;
p_skin skin_checkbox_checked        = NULL;

p_checkbox (*checkbox_init) (p_checkbox o, t_rect r, l_text caption, l_dword state, l_dword message) = &_checkbox_init;
/////////////////////////////////////////////////////////////////////////////////
void checkbox_translate_event (p_object o, t_event *event)
{
   RETVIEW(o,event);
   view_translate_event(o,event);

   if (event->type & EV_MOUSE)
   {
      if (OBJECT(mouse)->state & MO_SF_MOUSELDOWN)
      {
         if (!(CHECKBOX(o)->state & CB_GREYED))
         {
            CHECKBOX(o)->state |= CB_GREYED;
            VIEW(o)->draw_me(VIEW(o));
         }
      }

      if (OBJECT(mouse)->state & MO_SF_MOUSELUP)
      {
         if (CHECKBOX(o)->state & CB_GREYED)
            CHECKBOX(o)->state &= ~CB_GREYED;

         if (CHECKBOX(o)->state & CB_CHECKED)
            CHECKBOX(o)->state &= ~CB_CHECKED;
         else
            CHECKBOX(o)->state |= CB_CHECKED;

         VIEW(o)->draw_me(VIEW(o));

         if (CHECKBOX(o)->message)
         {
            set_event(event, EV_MESSAGE, CHECKBOX(o)->message, o);
            o->put_event(o, event);
         }

         clear_event(event);
      }
   }
}
/////////////////////////////////////////////////////////////////////////////////
void checkbox_draw (p_view o)
{
   t_rect  r = o->size_limits(o);
   t_point p;

   BITMAP *out = o->begin_paint(o, &p, r);

   if (out)
   {
      p_checkbox cb = CHECKBOX(o);
      l_int      cy = ( r.b.y - 12 ) / 2;
      t_rect sr;

      o->background(o, out, rect_move(r, p.x, p.y));

      if (cb->skin_checkbox_up && cb->skin_checkbox_down && cb->skin_checkbox_greyed && use_skins)
      {
         sr.a.x = p.x;
         sr.a.y = p.y;
         sr.b.x = p.x + skin_checkbox_checked->d.b.x;
         sr.b.y = p.y + skin_checkbox_checked->d.b.y;

         if (cb->state & CB_CHECKED)
         {
            if (cb->state & CB_GREYED)
            {
//               masked_blit(cb->skin_checkbox_greyed_checked->bmp, out, 0, 0, p.x, p.y, 13, 13);
               draw_skin(out, cb->skin_checkbox_greyed_checked, sr, NULL);
            }
            else             
            {
//               masked_blit(cb->skin_checkbox_checked->bmp, out, 0, 0, p.x, p.y, 13, 13);
               draw_skin(out, cb->skin_checkbox_checked, sr, NULL);
            }
         }
         else
         {
            if (cb->state & CB_GREYED)
            {
//               masked_blit(cb->skin_checkbox_checked->bmp, out, 0, 0, p.x, p.y, 13, 13);
               draw_skin(out, cb->skin_checkbox_checked, sr, NULL);
            }
            else
            {
//               masked_blit(cb->skin_checkbox_up->bmp, out, 0, 0, p.x, p.y, 13, 13);
               draw_skin(out, cb->skin_checkbox_up, sr, NULL);
            }
         }
      }
      else
      {
         rectfill ( out, p.x, p.y+cy, p.x+12, p.y+cy+12, (cb->state & CB_GREYED) ? color_3d_face : color_flat_face);

         button3d ( o, out, p.x, p.y+cy, p.x+12, p.y+cy+12, 1 );

         if (cb->state & CB_CHECKED)
         {
            l_color c = (cb->state & CB_GREYED) ? color_3d_text : color_flat_text;

            line(out, p.x+3, cy+p.y+3, p.x+9, cy+p.y+9, c);
            line(out, p.x+9, cy+p.y+3, p.x+3, cy+p.y+9, c);
            line(out, p.x+4, cy+p.y+3, p.x+9, cy+p.y+8, c);
            line(out, p.x+8, cy+p.y+3, p.x+3, cy+p.y+8, c);
            line(out, p.x+3, cy+p.y+4, p.x+8, cy+p.y+9, c);
            line(out, p.x+9, cy+p.y+4, p.x+4, cy+p.y+9, c);
         }
      }

      textout_draw_rect(out, o->font, cb->caption, -1, r.a.x+p.x+18, r.a.y+p.y,
                           r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTERY, cb->fcolor, TX_NOCOLOR, 0);
   }

   o->end_of_paint(o, r);
}
/////////////////////////////////////////////////////////////////////////////////
l_bool checkbox_done (p_object o)
{
   if (CHECKBOX(o)->caption)
      _free(CHECKBOX(o)->caption);

   return view_done(o);
}
/////////////////////////////////////////////////////////////////////////////////
p_checkbox _checkbox_init (p_checkbox o, t_rect r, l_text caption, l_dword state, l_dword message)
{
   if (!o)
      return NULL;

   clear_type(o,sizeof(t_checkbox));

   view_init(VIEW(o),r);

   o->caption = _strdup(caption);
   o->state   = state;
   o->message = message;
   o->fcolor = color_3d_text;
   OBJECT(o)->done = &checkbox_done;
   OBJECT(o)->translate_event = &checkbox_translate_event;
   VIEW(o)->brush.color = color_3d_face;
   VIEW(o)->draw = &checkbox_draw;

   if (!skin_checkbox_up && !skin_checkbox_down && !skin_checkbox_checked && !skin_checkbox_greyed && use_skins)
   {
      skin_checkbox_up = get_skin(skin_file, "checkbox_up");
      skin_checkbox_down = get_skin(skin_file, "checkbox_down");
      skin_checkbox_greyed = get_skin(skin_file, "checkbox_greyed");
      skin_checkbox_greyed_checked = get_skin(skin_file, "checkbox_greyed_checked");
      skin_checkbox_checked = get_skin(skin_file, "checkbox_checked");
   }

   if (use_skins)
   {
      VIEW(o)->transparent = true;

      o->skin_checkbox_up = skin_checkbox_up;
      o->skin_checkbox_down = skin_checkbox_down;
      o->skin_checkbox_greyed = skin_checkbox_greyed;
      o->skin_checkbox_greyed_checked = skin_checkbox_greyed_checked;
      o->skin_checkbox_checked = skin_checkbox_checked;
   }

   return o;
}
