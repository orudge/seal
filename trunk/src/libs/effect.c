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
#include <cp.h>
#include <button.h>
#include <dialogs.h>
#include <trackbar.h>
#include <grfx.h>
#include <skin.h>

#define MSG_TRANS      100001
#define MSG_HISUPDATE  100003

p_grfx preview1 = NULL;
p_button preview2 = NULL;
p_trackbar transl = NULL;

p_history  skin = NULL;

BITMAP *beffect = NULL;

l_bool skin_changed;

SetInfoAppName("Graphics settings");
SetInfoDesciption("Garphics effects settings");
SetInfoCopyright("(c) Copyright 2001 Point Mad. All rights rserved.");
SetInfoManufacturer("Point Mad");

void free_skin ( p_skin o ) {
  destroy_bitmap(o->bmp);
  _free(o);
};

void release_skin ( p_skin o ) {
  if ( !o || o == skin_btnotov || o == skin_btup || o == skin_btdown ) return;
  free_skin(o);
};

void   preview1_draw ( p_view o )
{
 t_rect  r = o->get_local_extent(o);
 t_point p;

 BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {
    l_int val = 255;

    rectfill(out,p.x,p.y,p.x+r.b.x,p.y+r.b.y,color_3d_face);

    if ( transl && beffect ) {
      switch ( transl->value ) {
        case 1 : val = 235; break;
        case 2 : val = 215; break;
        case 3 : val = 195; break;
        case 4 : val = 175; break;
        case 5 : val = 155; break;
      };

      masked_blit(beffect, out, 0, 0, p.x+10, p.y+10, 80, 100);

      if ( val == 255 )
        masked_blit(beffect, out, 0, 0, p.x, p.y, 80, 100);
      else
        trans_blit(beffect, out, 0, 0, p.x, p.y, 80, 100,val,255);
    };

  };

  o->end_of_paint(o, r);
};

void set_save ( void ) {
  if ( transl ) {
    translucide = transl->value;
    if ( skin_changed ) {
    l_text skinf = "skin/";
    set_key_integer("current/seal/optical/translucide", translucide);
    if ( skin->current == 0 ) {
      set_key_integer("current/seal/skin/use", 0);
      skin_btup    = NULL;
      skin_btdown  = NULL;
      skin_btnotov = NULL;
      use_skins = false;
    } else {
      l_text file = io_realpath("skin/",TEXTLINE(skin)->text);
      set_key_integer("current/seal/skin/use", 1);
      set_key("current/seal/skin/file", file);
      skin_btup    = get_skin(file,"btup");
      skin_btdown  = get_skin(file,"btdown");
      skin_btnotov = get_skin(file,"btnotover");
      use_skins = true;
      skin_file = file;
    };
    release_skin(preview2->skin_btup);
    release_skin(preview2->skin_btdown);
    release_skin(preview2->skin_btnotov);
    preview2->skin_btup = NULL;
    preview2->skin_btdown = NULL;
    preview2->skin_btnotov = NULL;
    };
  };
};

void set_trans_ev(p_object o, t_event *e)
{
   RETVIEW(o, e);

   if ((o->phase == PH_POSTPROCESS) && (e->type == EV_MESSAGE))
   {
      switch (e->message)
      {
         case MSG_TRANS:
            VIEW(preview1)->draw_view(VIEW(preview1));
            clear_event(e);
            break;
         case MSG_HISUPDATE : {
            skin_changed = true;
            if ( skin->current == 0 ) {
              preview2->skin_btup = NULL;
              preview2->skin_btdown = NULL;
              preview2->skin_btnotov = NULL;
              VIEW(preview2)->transparent = false;
              if ( preview2->flags & BF_UNDERSEL ) preview2->flags &= ~BF_UNDERSEL;
            } else {
              l_text file = io_realpath("skin/",TEXTLINE(skin)->text);
              if ( file ) {
                release_skin(preview2->skin_btup);
                release_skin(preview2->skin_btdown);
                release_skin(preview2->skin_btnotov);
                VIEW(preview2)->transparent = true;
                preview2->skin_btup = get_skin(file,"btup");
                preview2->skin_btdown = get_skin(file,"btdown");
                preview2->skin_btnotov = get_skin(file,"btnotover");
                if ( preview2->skin_btnotov && !(preview2->flags & BF_UNDERSEL)) preview2->flags |= BF_UNDERSEL;
                _free(file);
              };
            };
            VIEW(preview2)->draw_view(VIEW(preview2));
           clear_event(e);
         }; break;
      }
   }
}
////////////////////////////////////////////////////////////////////////////////
static p_list skin_list ( l_text sel, l_int *nsel ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);

  if ( p ) {
    struct t_ffblk f;
    l_int done = 0;
    l_int id = 1;
    l_text fi = io_realpath("skin/", "*.skn");

    p->insert(p, new_history_item(TXT__NONE_, NULL, 0, NULL));

    done = io_findfirst(fi, &f, FA_ARCH);
    while ( !done ) {
      l_text realfile = io_realpath("skin/", f.ff_filename);
      if ( io_isfilename (f.ff_filename) ) {
        p->insert(p, new_history_item(f.ff_filename, NULL, 0, NULL));
        if ( sel ) if ( !stricmp(realfile,sel) ) (*nsel) = id;
      };
      id++;
      _free(realfile);
      done = io_findnext(&f);
    };

     _free(fi);

  };

  return p;
};

void set_load (p_object o)
{
  t_rect r = VIEW(o)->get_local_extent(VIEW(o));

  if ( get_depth(screen) < 16 ) {
   p_stattext st1 = stattext_init(_malloc(sizeof(t_stattext)), r, TX_ALIGN_CENTER, "Your color depth don't allowed you to use graphic effects.");
   if ( st1 ) OBJECT(o)->insert(OBJECT(o), OBJECT(st1));
  } else {
   p_stattext st1 = NULL;
   l_text file = NULL;
   l_int sel = 0;

   st1 = stattext_init(_malloc(sizeof(t_stattext)), rect_assign(0, 5, (r.b.x/2)-10, 20), TX_ALIGN_CENTER, TXT_TRANSLUCIDE_LEVEL);
   OBJECT(o)->insert(OBJECT(o), OBJECT(st1));

   st1 = stattext_init(_malloc(sizeof(t_stattext)), rect_assign((r.b.x/2)+5, 5, r.b.x, 20), TX_ALIGN_CENTER, TXT_SKIN_SELECTROR);

   OBJECT(o)->insert(OBJECT(o), OBJECT(st1));

   if ( get_key_integer("current/seal/skin/use") == 1 ) {
     file = get_key("current/seal/skin/file");
   };

   skin  = history_init( _malloc(sizeof(t_history)),
                         rect_assign( (r.b.x/2)+5, 25, r.b.x-15, 45 ),
                         skin_list(file, &sel) ,
                         150,
                         HF_REWRITEUNABLE|LF_SELFLIST);

   if ( file ) skin->current = sel;

   skin->message = MSG_HISUPDATE;
   OBJECT(o)->insert(OBJECT(o), OBJECT(skin));

   transl = trackbar_init(_malloc(sizeof(t_trackbar)), rect_assign(0, 40, (r.b.x/2)-105, 150),
   1,0,5,translucide, MSG_TRANS);
   OBJECT(o)->insert(OBJECT(o), OBJECT(transl));

   preview1  =    grfx_init( _malloc(sizeof(t_grfx)),
                         rect_assign( (r.b.x/2)-100, 40, (r.b.x/2)- 10, 150 ));

   VIEW(preview1)->draw = &preview1_draw;
   OBJECT(o)->insert(OBJECT(o), OBJECT(preview1));

   preview2  =    button_init( _malloc(sizeof(t_button)),
                         rect_assign( (r.b.x/2)+5, 50, r.b.x, 75 ),TXT_PREVIEW,NULL,NULL);

   OBJECT(o)->insert(OBJECT(o), OBJECT(preview2));

   };
}

app_begin (void)
{
  if (ap_process == AP_INIT) {
    beffect = load_image("bmp/effect.bmp");
    add_cp_loader(TXT_GRAPHICS_EFFECTS_AND_SKINS,NULL,cps_display,NULL,NULL, &set_load, &set_trans_ev, &set_save);
  };
} app_end;
