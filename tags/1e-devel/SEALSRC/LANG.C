/****************************************************************/
/*                                                              */
/*                            lang.c                            */
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


#include"allegro.h"
#include"seal.h"
#include"lang.h"

static l_int      (*old_readkey) ( p_keyboard o ) = NULL;
static t_languages *languages = NULL;


t_languages *lang_find_language ( l_char *name )
{
  t_languages *s = languages;

  while ( s ) {

    if ( !stricmp(name, s->name) ) return s;

    s = s->next;

  };

  return NULL;
};


void   lang_set_language ( l_char *name )
{
  t_languages *s = languages;

  while ( s ) {

    s->set = 0;

    if ( !stricmp(name, s->name) ) {

      s->set = 1;

    };

    s = s->next;

  };

};


t_languages  *lang_find_set_language ( void )
{
  t_languages *s = languages;

  while ( s ) {

    if ( s->set ) return s;

    s = s->next;

  };

  return NULL;
};


void   lang_remove_language ( l_char *name )
{
  t_languages *s = languages;
  t_languages *o = NULL;

  while ( s ) {

    if ( !stricmp(name, s->name) ) {

      if ( !o )

        languages = s->next;

      else

        o->next = s->next;

      if ( s->set && languages ) lang_set_language ( languages->name );

      _free(s->name);
      _free(s);

      return;
    };

    o = s;

    s = s->next;

  };

};


l_int  lang_add_language ( l_int (*trans_key)(l_int o), l_char *name )
{
  t_languages *n = NULL;
  t_languages *s = languages;

  if ( !name || !trans_key ) return 0;

  n = lang_find_language(name);

  if ( n ) return 2;

  n = (t_languages*)_malloc(sizeof(t_languages));

  if ( !n ) return 0;

  memset(n, NULL, sizeof(t_languages));

  n->trans_key = trans_key;
  n->name = _strdup(name);
  n->next = languages;

  return 1;

};


static void  lang_done ( void )
{
  t_languages *s = languages;

  if ( keyb ) keyb->readkey = old_readkey;

  while ( s ) {

    t_languages *o = s->next;

    _free(s->name);
    _free(s);

    s = o;

  };

  languages = NULL;
};


static l_int lang_readkey ( p_keyboard o )
{
  t_languages *s = lang_find_set_language();

  l_int t = readkey(); /* allegro function */

  if ( s && s->trans_key ) t = s->trans_key(t);

  return t;
};


static void  lang_init ( void )
{

  if ( keyb ) {

    old_readkey = keyb->readkey;
    keyb->readkey = &lang_readkey;

  };

};


#include"lang.exp"

lib_begin ( void ) {

  if ( ap_process == AP_INIT ) {

    AP_SETNUMOFCALLS(1);

    lang_init();

  } else

  if ( ap_process == AP_FREE ) {

    lang_done();

  };

} lib_end;

