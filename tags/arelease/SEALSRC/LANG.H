/****************************************************************/
/*                                                              */
/*                           lang.h                             */
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



#ifndef __LANG_H_INCLUDED__
#define __LANG_H_INCLUDED__


#define   MAX_TABLE_SIZE  256

typedef struct t_languages *p_languages;

typedef struct t_languages {

  l_char       *name;
  l_int       (*trans_key) ( l_int key );
  l_bool        set;
  p_languages   next;
  l_char        reserved[24];

} t_languages;

t_languages *lang_find_language ( l_char *name );
void         lang_set_language ( l_char *name );
t_languages *lang_find_set_language ( void );
void         lang_remove_language ( l_char *name );
l_int        lang_add_language ( l_int (*trans_key)(l_int key), l_char *name );

#endif

