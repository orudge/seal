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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <io.h>

#include <registry.h>



////////////////////////////////////////////////////////////////////////////////

// Seal 2.00 Registry !

p_key rootkey;
////////////////////////////////////////////////////////////////////////////////
p_key found_key_in ( p_key o, char * name, char lnk ) {
  //DEBUG_printf("Look for '%s' in '%s'\n",name,o->name);
  if ( o && name ) {
    uint a = 0;
    if ( lnk && o->type == KEY_LINK ) o = (p_key)o->data;
    if ( name[0] == '/' || name[0] == '\\' ) name++;
    if ( !name[0] ) return o; // If name is "" : it the key, so return it
    if ( o->chl ) {
	  p_key f = o->chl;
      p_key	p = f;
      while ( name[a] ) {
        if ( name[a] == '/' || name[a] == '\\' ) {
          do {
	        if ( !strnicmp( name,p->name,a) ) {
		      if ( !p->name[a] )
			    return found_key_in(p,name+a+1,lnk);
	        };
	        p = p->nxt;
          } while ( p != f);
          return NULL; // Not found here ...
        };
	    a++;
      };
	  f = o->chl;
      p = f;
      do {
	    if ( !stricmp( name,p->name) ) {
		  if ( lnk && p->type == KEY_LINK ) p = (p_key)p->data;
	      return p;
	    };
	    p = p->nxt;
      } while ( p != f);
    };
  };
  return NULL; // Not found at all !
};
////////////////////////////////////////////////////////////////////////////////
p_key found_key_ex ( char * name, char lnk ) {
  if ( rootkey && name ) return found_key_in(rootkey,name,lnk);
  return NULL;
};
////////////////////////////////////////////////////////////////////////////////
p_key found_key ( char * name ) {
  return found_key_ex(name,1);
};
////////////////////////////////////////////////////////////////////////////////
char key_exists ( char * name ) {
  return found_key(name) ? 1 : 0;
};
////////////////////////////////////////////////////////////////////////////////
char create_key ( char * parent, char * name ) {
  if ( parent && name ) {
    p_key own = found_key(parent);
    p_key o;
    if ( !own ) return RE_PATH_NOT_FOUND; // Unknow parent
    if ( found_key_in(own,name,1) ) return RE_SUCCESS; // Exists so sucess
    o = (p_key)malloc(sizeof(t_key));
    if ( !o ) return RE_UNKOWN; // No memory for ..
    memset(o,0,sizeof(t_key));
    o->name = strdup(name);
    o->own = own;
    if ( !own->chl ) {
      own->chl = o;
      o->nxt = o;
      o->prv = o;
    } else {
	  o->prv = own->chl;
	  o->nxt = own->chl->nxt;
	  o->prv->nxt = o;
	  o->nxt->prv = o;
	  own->chl = o;
    };
    return RE_SUCCESS; // All right
  };
  return RE_UNKOWN; // ...
};
////////////////////////////////////////////////////////////////////////////////
void free_key ( p_key o ) {
  if ( o ) {
	if ( o->name ) free(o->name);
	if ( o->data && o->type != KEY_LINK ) free(o->data);
	free(o);
  };
};
////////////////////////////////////////////////////////////////////////////////
char delete_key_ex ( p_key o ) {
  if ( o && o != rootkey ) {
	while ( o->chl ) delete_key_ex(o->chl);
	if ( o->own ) {
      p_key m = o->own;
      if ( m->chl == o ) m->chl = o->prv;
      if ( m->chl == o ) m->chl = NULL;
    };
    if ( o->nxt ) {
      p_key x = o->prv;
      x->nxt = o->nxt;
      o->nxt->prv = x;
    };
    free_key(o);
    return RE_SUCCESS;
  };
  if ( o == rootkey ) return RE_ROOT_KEY_SPECIFIED;
  return RE_KEY_NOT_FOUND;
};
////////////////////////////////////////////////////////////////////////////////
//// Delete a key ///////////////
// return
//   0 -> Sucess
//   1 -> Unknow key
char delete_key ( char * name ) {
  return delete_key_ex(found_key_ex(name,0));
};
////////////////////////////////////////////////////////////////////////////////
void *copy_of_data ( void *data, uint size ) {
  if ( size && data ) {
    void *o = (void*)malloc(size);
    if ( !o ) return NULL;
    memcpy(o,data,size);
    return o;
  };
  return NULL;
};
////////////////////////////////////////////////////////////////////////////////
char set_key_data ( char *name, char type, uint size, void *data ) {
  if ( name ) {
	p_key o = found_key ( name );
	if ( !o ) return  RE_KEY_NOT_FOUND;
	if ( o->data && o->type != KEY_LINK ) free(o->data);
	o->type = type;
	o->size = size;
	if ( type == KEY_LINK )
	  o->data = data;
	else
	  o->data = copy_of_data(data,size);
	return RE_SUCCESS;
  };
  return RE_KEY_NOT_FOUND;
};
////////////////////////////////////////////////////////////////////////////////
//// Get key data  ///////////////
// return
//   0 -> Sucess
//   1 -> Unknow key
char get_key_data ( char *name, char *type, uint *size, void **data ) {
  if ( name ) {
	p_key o = found_key ( name );
    if ( !o ) return  RE_KEY_NOT_FOUND;
	if ( type ) (*type) = o->type;
	if ( size ) (*size) = o->size;
	if ( data ) (*data) = o->data;
	return RE_SUCCESS;
  };
  return RE_KEY_NOT_FOUND;
};
////////////////////////////////////////////////////////////////////////////////
char *key_in_path ( char *path, char *key ) {
  char *result = (char *) malloc(strlen(path)+strlen(key)+2);
  strcpy(result, path);
  strcat(result, "/");
  strcat(result, key);
  return result;
}
////////////////////////////////////////////////////////////////////////////////
uint make_key_id ( p_key o, uint id ) {
  o->id = id;
  id++;
  if ( o->chl ) {
    p_key f = o->chl;
    p_key	p = f;
    do {
	  id = make_key_id(p,id);
	  p = p->nxt;
    } while ( p != f);
  };
  return id;
};
////////////////////////////////////////////////////////////////////////////////
void write_key ( FILE *f, p_key o ) {
  uint nlen = o->name ? strlen(o->name)+1 : 0;
  uint none = 0;
  //fwrite(&o->id, sizeof(int), 1, f);
  fwrite(&nlen, sizeof(int), 1, f);
  if ( nlen ) {
    fwrite(o->name, nlen, 1, f);
  };
  fwrite(&o->type, sizeof(char), 1, f);
  if ( o->type == KEY_LINK ) {
	uint lnk = o->data ? ((p_key)(o->data))->id : 0;
	uint size = sizeof(uint);
	fwrite(&size, sizeof(uint), 1, f);
    fwrite(&lnk, size, 1, f);
  } else {
    fwrite(&o->size, sizeof(uint), 1, f);
    if ( o->size ) {
      fwrite(o->data, o->size, 1, f);
    };
  };

  fwrite(o->own?&o->own->id:&none,sizeof(uint),1,f);
  fwrite(o->nxt?&o->nxt->id:&none,sizeof(uint),1,f);
  fwrite(o->prv?&o->prv->id:&none,sizeof(uint),1,f);
  fwrite(o->chl?&o->chl->id:&none,sizeof(uint),1,f);
  if ( o->chl ) { // Write sub keys if exists
    p_key ff = o->chl;
    p_key	p = ff;
    do {
      write_key ( f, p );
	  p = p->nxt;
    } while ( p != ff);
  };
};
////////////////////////////////////////////////////////////////////////////////
void write_registry ( void ) {
  FILE *f = fopen("registry.dat","wb");
  uint numberkey, magic;
  if ( !f ) return;
  magic = REGISTRY_MAGIC;
  numberkey = make_key_id(rootkey,1);
  numberkey--;
  fwrite(&magic,sizeof(uint),1,f);
  fwrite(&numberkey,sizeof(uint),1,f); // For dynamic loading !
  write_key(f,rootkey);
  fclose(f);
};

p_key *dynamic = NULL; // For dynamic loading
////////////////////////////////////////////////////////////////////////////////
void read_key ( FILE *f, p_key o ) {
  uint nlen;
  uint id = 0;
  //fread(&o->id, sizeof(int), 1, f);
  fread(&nlen, sizeof(int), 1, f);
  if ( nlen ) {
    o->name = (char*)malloc(nlen);
    fread(o->name, nlen, 1, f);
  } else o->name = NULL;

  fread(&o->type, sizeof(char), 1, f);
  fread(&o->size, sizeof(uint), 1, f);
  if ( o->type == KEY_LINK ) {
	fread(&id,sizeof(uint),1,f);
    if ( id ) o->data = (void*)dynamic[id-1];
    else o->data = NULL;

  } else {
    if ( o->size ) {
      o->data = (void*)malloc(o->size);
      fread(o->data, o->size, 1, f);
    } else o->data = NULL;
  };
  fread(&id,sizeof(uint),1,f);
  if ( id ) o->own = dynamic[id-1];
  else o->own = NULL;
  fread(&id,sizeof(uint),1,f);
  if ( id ) o->nxt = dynamic[id-1];
  else o->nxt = NULL;
  fread(&id,sizeof(uint),1,f);
  if ( id ) o->prv = dynamic[id-1];
  else o->prv = NULL;
  fread(&id,sizeof(uint),1,f);
  if ( id ) o->chl = dynamic[id-1];
  else o->chl = NULL;

};
////////////////////////////////////////////////////////////////////////////////
char read_registry ( void ) {
  FILE *f = fopen("registry.dat","rb");
  uint numberkey,a,magic;
  if ( !f ) return 0;
  fread(&magic,sizeof(uint),1,f);
  if ( magic != REGISTRY_MAGIC ) {
	fclose(f);
	return 0;
  };
  fread(&numberkey,sizeof(uint),1,f);
  dynamic = (p_key*)malloc(sizeof(p_key)*numberkey);
  a=0;
  while ( a < numberkey ) {
	p_key key = (p_key)malloc(sizeof(t_key));
	memset(key,0,sizeof(t_key));
	dynamic[a] = key;
	a++;
  };
  a=0;
  while ( a < numberkey ) {
	read_key(f,dynamic[a]);
	a++;
  };
  rootkey = dynamic[0];
  free(dynamic);
  dynamic = NULL,
  fclose(f);
  return 1;
};
////////////////////////////////////////////////////////////////////////////////
void make_registry ( void ) {
  rootkey = (p_key)malloc(sizeof(t_key));
  memset(rootkey,0,sizeof(t_key));
};
////////////////////////////////////////////////////////////////////////////////
/*void keymap ( p_key o, int level ) {
  if ( o->chl ) {
    p_key ff = o->chl->nxt;
    p_key	p = ff;
    do {
	  int a = level;
	  while ( a ) {
		printf("  ");
		a--;
	  };
	  printf("%c %s\n",p->chl?'+':'-',p->name);
      keymap ( p, level + 1);
	  p = p->nxt;
    } while ( p != ff);
  };
};

void registry_map ( void ) {
  printf("+ ROOT (\"\")\n");
  keymap(rootkey,1);
};*/
////////////////////////////////////////////////////////////////////////////////
char free_registry ( void ) {
  if ( rootkey ) {
    while ( rootkey->chl ) delete_key_ex(rootkey->chl);
    free_key(rootkey);
    rootkey = NULL;
  };
};
////////////////////////////////////////////////////////////////////////////////
char set_key ( char *key, char *text ) {
  return set_key_data(key,KEY_TEXT,strlen(text)+1,text);
};
char set_key_byte(char *key, unsigned char b) {
  return set_key_data(key,KEY_BYTE,sizeof(unsigned char),&b);
};
char set_key_small(char *key, short s) {
  return set_key_data(key,KEY_SMALL,sizeof(short),&s);
};
char set_key_integer(char *key, long i) {
  return set_key_data(key,KEY_INTEGER,sizeof(long),&i);
};
char set_key_big(char *key, long long big) {
  return set_key_data(key,KEY_BIG,sizeof(long long),&big);
};
char set_key_float(char *key, float f) {
  return set_key_data(key,KEY_FLOAT,sizeof(float),&f);
};
char set_key_link(char*key,p_key lnk) {
  return set_key_data(key,KEY_LINK,sizeof(int),lnk);
};
////////////////////////////////////////////////////////////////////////////////
char *get_key ( char *key ) {
  void *data;
  char type;
  if ( get_key_data(key,&type,NULL,&data) != RE_SUCCESS ) return NULL;
  if ( type != KEY_TEXT ) return NULL;
  return data?strdup((char*)data):NULL;
};
unsigned char get_key_byte ( char *key ) {
  void *data;
  char type;
  if ( get_key_data(key,&type,NULL,&data) != RE_SUCCESS ) return NULL;
  if ( type != KEY_BYTE ) return NULL;
  return *((unsigned char*)data);
};
short get_key_small ( char *key ) {
  void *data;
  char type;
  if ( get_key_data(key,&type,NULL,&data) != RE_SUCCESS ) return NULL;
  if ( type != KEY_SMALL ) return NULL;
  return *((short*)data);
};
long get_key_integer ( char *key ) {
  void *data;
  char type;
  if ( get_key_data(key,&type,NULL,&data) != RE_SUCCESS ) return NULL;
  if ( type != KEY_INTEGER ) return NULL;
  return *((long*)data);
};
long long get_key_big ( char *key ) {
  void *data;
  char type;
  if ( get_key_data(key,&type,NULL,&data) != RE_SUCCESS ) return NULL;
  if ( type != KEY_BIG ) return NULL;
  return *((long long*)data);
};
float get_key_float ( char *key ) {
  void *data;
  char type;
  if ( get_key_data(key,&type,NULL,&data) != RE_SUCCESS ) return NULL;
  if ( type != KEY_FLOAT ) return NULL;
  return *((float*)data);
};
////////////////////////////////////////////////////////////////////////////////
char reg_find_first(char *path, p_registry_search info) {
  p_key mum = found_key(path);
  if ( !mum ) return 0;
  if ( !mum->chl ) return 0;
  info->key = mum->chl->nxt; // CHL is last, CHL->NXT is first

  info->parent_name = strdup(path);

  info->name = key_in_path(info->parent_name, info->key->name);

  return 1;
};
////////////////////////////////////////////////////////////////////////////////
char reg_find_next(p_registry_search info) {

  if ( info->name ) { free(info->name); info->name = NULL;};

  if ( info->key->own->chl == info->key ) {
	//if ( info->parent_name ) { free(info->parent_name);	info->parent_name = NULL;};
	return 0;
  };
  info->key = info->key->nxt;
  info->name = key_in_path(info->parent_name, info->key->name);
  return 1;
};
////////////////////////////////////////////////////////////////////////////////
char *key_full_name ( p_key o ) {
  char *r = NULL;
  while ( o ) {
	if ( o->name ) {
	  if ( r ) {
	    char *nr = key_in_path(o->name,r);
	    free(r);
	    r = nr;
      } else {
	    r = strdup(o->name);
      };
    };
	o = o->own;
  };
  return r;
};
////////////////////////////////////////////////////////////////////////////////
char *key_owner_full_name ( char* key ) {
  p_key o = found_key(key);
  //DEBUG_printf("Search for '%s' owner full name\n",key);
  if ( !o ) return 0;
  if ( !o->own ) return 0;
  return key_full_name(o->own);
};
////////////////////////////////////////////////////////////////////////////////
char *eliminate_extra ( char *t ) {
  int st=0,a=0,fn=0;
  char *m;
  while ( t[a] ) {
	if ( t[a] != ' ' ) {
	  st = a;
	  break;
    };
	a++;
  };
  a=strlen(t)-1;
  while ( a >= 0 ) {
	if ( t[a] != ' ' && t[a] != '\n' && t[a] != '\r' ) {
	  fn = a;
	  break;
    };
	a--;
  };
  m = malloc(fn-st+2);
  memset(m,0,fn-st+2);
  if ( fn-st+1 ) memcpy(m,t+st,fn-st+1);
  free(t);
  return m;
};
////////////////////////////////////////////////////////////////////////////////
char read_reg_file ( char * file, char show ) {

  FILE *f = fopen ( file, "rt" );

  char *curkey = NULL;
  char *subkey = NULL;
  char *buffer = NULL;
  uint  l=1,a=0,st=0,size=0;
  char doing = 0; // 1->curkey, 2->value

  if(show)printf("Read file %s ...\n",file);
  l++;
  if ( !f ) return 0;

  size = filelength(fileno(f));
  buffer = malloc(size+1);
  if ( !buffer ) return 0;
  memset(buffer,0, size+1);
  fread(buffer, size, 1, f);
  fclose(f);

  while ( 1 ) {
    if ( buffer[a] == '\n' || !buffer[a] ) {
	  if ( doing == 2 && curkey && subkey ) {
	    char *value = malloc(a-st);
	    memset(value,0,a-st);
	    if ( a-st-1 ) memcpy(value,buffer+st+1,a-st-1);
	    value = eliminate_extra(value);
	    subkey = eliminate_extra(subkey);
	    if ( !strcmp(subkey,"@" ) ) {
		  free(subkey);
		  subkey = strdup( curkey );
	    } else {
		  create_key(curkey,subkey);
		  if ( strcmp("",curkey ) ) {
		    char *old = subkey;
		    subkey = key_in_path(curkey,subkey);
		    free(old);
	      };
	    };
	    if(show)printf("    Set key %s to %s\n",subkey,value);
	    l++;
	    if ( value[0] == '"' ) {
		  uint len = strlen(value);
		  char *val = malloc(len-1);
          memset(val,0,len-1);
	      if ( len-2 ) memcpy(val,value+1,len-2);
	      set_key(subkey,val);
          free(val);
        } else if ( value[0] == '(' ) {
	      if ( !stricmp (value,"(nodata)") ) set_key_data(subkey,0,0,0);
        } else {
	      set_key_integer(subkey,atoi(value));
        };
        free(subkey);
        free(value);
        subkey = NULL;
      };
      st = a;
      doing = 0;
      if ( !buffer[a] ) break;
    } else if ( buffer[a] == '[' && !doing ) {
	  if ( curkey ) free(curkey);
	  curkey = NULL;
	  st = a;
	  doing = 1;
    } else if ( buffer[a] == ']' && doing == 1 ) {
	  curkey = malloc(a-st);
	  memset(curkey,0,a-st);
	  if ( a-st-1 ) memcpy(curkey,buffer+st+1,a-st-1);
	  if(show)printf("  In %s\n",curkey);
	  l++;
	  doing = 0;
    } else if ( buffer[a] == '=' && !doing ) {
	  subkey = malloc(a-st);
	  memset(subkey,0,a-st);
	  if ( a-st-1 ) memcpy(subkey,buffer+st+1,a-st-1);
	  doing = 2;
	  st=a;
    };
	a++;
  };
  return 1;
};
////////////////////////////////////////////////////////////////////////////////
void registry_init ( void ) {
  if ( !read_registry() ) {
    make_registry();
    create_key("","USERS");
    create_key("USERS","DEFAULT");
    create_key("","CURRENT");
    create_key("","GLOBAL");
    create_key("","CONFIG");
    create_key("","SOFTWARE");
    create_key("","SYSTEM");
    set_key_link("CURRENT",found_key("USERS/DEFAULT"));
  };
};
////////////////////////////////////////////////////////////////////////////////
void registry_done ( void ) {
  //fclose(debug);
  write_registry();
  free_registry();
};