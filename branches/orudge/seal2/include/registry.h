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

#ifndef _REGISTRY2_H_
#define _REGISTRY2_H_

typedef unsigned int uint;

typedef struct t_key *p_key;

typedef struct t_key {

  char *name;
  char  type;
  uint  size;
  void *data;

  uint   id;

  p_key own; // Owner
  p_key nxt; // Next
  p_key prv; // Prevent
  p_key chl; // Child

} t_key;

// <Only for 0.x compatibility>
typedef struct t_registry_search *p_registry_search;
typedef struct t_registry_search
{
  p_key key;
  char *parent_name;
  char *name;
} t_registry_search;
// </>

#define RE_SUCCESS              0   // no error
#define RE_KEY_NOT_FOUND        1   // key not found
#define RE_PATH_NOT_FOUND       2   // path in registry not found
#define RE_READ_ONLY_KEY        3   // tried to write on a read-only key
#define RE_NO_KEY_SPECIFIED     4   // no key has specified
#define RE_ROOT_KEY_SPECIFIED   5   // tried to write/delete a root key
#define RE_UNKOWN               6   // unknown error

#define KEY_BYTE      1
#define KEY_SMALL     2
#define KEY_INTEGER   3
#define KEY_BIG       4
#define KEY_FLOAT     5
#define KEY_TEXT      6
#define KEY_DATA      7
#define KEY_LINK      8

#define REGISTRY_MAGIC 0x47455253 // "SREG"

#define registry_file "registry.dat"

extern p_key rootkey;

p_key found_key_in ( p_key o, char * name, char lnk );
p_key found_key_ex ( char * name, char lnk );
p_key found_key ( char * name );
char key_exists ( char * name );
char create_key ( char * parent, char * name );
void free_key ( p_key o );
char delete_key_ex ( p_key o );
char delete_key ( char * name );
char set_key_data ( char *name, char type, uint size, void *data );
char get_key_data ( char *name, char *type, uint *size, void **data );
char *key_in_path ( char *path, char *key );
uint make_key_id ( p_key o, uint id );
void write_key ( FILE *f, p_key o );
void write_registry ( void );
void read_key ( FILE *f, p_key o );
char read_registry ( void );
void make_registry ( void );
char free_registry ( void );

char set_key ( char *key, char *text );
char set_key_byte(char *key, unsigned char b);
char set_key_small(char *key, short s);
char set_key_integer(char *key, long i);
char set_key_big(char *key, long long big);
char set_key_float(char *key, float f);
char set_key_link(char*key,p_key lnk);

char *get_key ( char *key );
unsigned char get_key_byte ( char *key );
short get_key_small ( char *key );
long get_key_integer ( char *key );
long long get_key_big ( char *key );
float get_key_float ( char *key );

char reg_find_first(char *path, p_registry_search info);
char reg_find_next(p_registry_search info);

char *key_full_name ( p_key o );
char *key_owner_full_name ( char* key );
char read_reg_file ( char * file, char show );

void registry_init ( void );
void registry_done ( void );



#endif