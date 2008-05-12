/****************************************************************/
/*                                                              */
/*                          dataini.c                           */
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


#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<pc.h>
#include<io.h>
#include"dataini.h"

#ifndef _malloc
#define _malloc      malloc
#endif
#ifndef _free
#define _free        free
#endif
#ifndef _realloc
#define _realloc     realloc
#endif
#ifndef  _strdup
#define  _strdup     strdup
#endif
#ifndef  _calloc
#define  _calloc     calloc
#endif
#ifndef  _cfree
#define  _cfree      cfree
#endif

#define INI_ISENTER(d)  (*(d) == INI_ENTERLINE && !isspace(*((d)+1)))
#define INI_ENTERLINE   INI_ENDLINE
#define INI_FIRSTCHAR   '['
#define INI_LASTCHAR    ']'
#define INI_FUNCFIRST   '('
#define INI_FUNCLAST    ')'
#define INI_CEQUALS     '='
#define INI_ENDLINE     '\n'
#define INI_EQUALS      "="
#define INI_NUMBER      long
#define INI_NONE        "(none)"
#define INI_TEXTCHAR    '"'
#define INI_CFIRSTCHAR  "["
#define INI_CLASTCHAR   "]"

#ifndef strsize
#define strsize(p,v)    ((v)?((v)-(p)):strlen(p))
#endif

#ifndef lmin
#define lmin(x,y)   (((x)<(y))?(x):(y))
#endif
#ifndef lmax
#define lmax(x,y)   (((x)>(y))?(x):(y))
#endif

static INI_NUMBER  ini_number;


static char*  delfrommem ( char *_dest, long _pos, long _size )
{
  if ( _dest ) {
    long ttsize = strlen(_dest);
    _size = lmax(0, lmin((ttsize-_pos), _size));
    _pos  = lmax(0, lmin(ttsize-1, _pos));
    if ( _size > 0 ) {
      char *p = strdup(&_dest[_pos+_size]);
      char *n = (char*)_realloc(_dest, ttsize-_size);
      strcpy(&n[_pos], p);
      _free(p);
      return n;
    };
  };
  return _dest;
};


static char *addtomem ( char *mem, long where, char *what1, char *what2, char *what3, char *what4 ) {
  long s1 = strlen(what1);
  long s2 = strlen(what2);
  long s3 = strlen(what3);
  long s4 = strlen(what4);
  long size = s1+s2+s3+s4;
  if ( size )
    if ( !mem ) {
      mem = (char *)_malloc(size+1);
      if ( mem ) {
        bzero(mem, size+1);
        strcat(mem, what1);
        strcat(mem, what2);
        strcat(mem, what3);
        strcat(mem, what4);
      };
    } else {
      long max = strlen(mem);
      if ( where < 0 ) where = max; else where = lmax(0, lmin(max, where));
      mem = (char *)_realloc(mem, max+size+1);
      if ( mem ) {
        char *s;
        bzero(&mem[max], size+1);
        s = stridup(&mem[where], lmax(0, max-where));
        strcat(&mem[where], what1);
        strcat(&mem[where+s1], what2);
        strcat(&mem[where+s1+s2], what3);
        strcat(&mem[where+s1+s2+s3], what4);
        if ( s ) strcat(&mem[where+size], s);
      };
    };
  return mem;
};


static char *strtoend ( char *str, char endchar ) {
  if ( str ) {
    char *v = strchr(str, endchar);
    long size = 0;
    if ( v ) size = v-str; else size = strlen(str);
    if ( size ) {
      char *t = (char *)_malloc(size+1);
      if ( t ) {
        bzero(t, size+1);
        strncat(t, str, size);
      };
      return t;
    };
  };
  return NULL;
};


static char *transini_line ( char *line ) {
   if ( !line || strstr(line, INI_NONE) ) return NULL;
   else {
     char *endp;
     endp = strchr(line, INI_TEXTCHAR);
     if ( endp ) return strtoend(endp+1, INI_TEXTCHAR); /* string */
     ini_number = strtol(line, &endp, 0);
     if ( endp ) return ((char*)(&ini_number)); /* number */
   };
   return NULL; /* else it's string */
};


static char *copy_not_space ( char *dst, char *src, int sz )
{
  char *o = dst;
  if ( dst && src ) {
    while ( sz > 0 ) {
      if ( *src != ' ' && *src != 10 && *src != 13 && *src != 9 ) {
        *dst = *src;
        dst++;
      };
      src++;
      sz--;
    };
  };
  return o;
};


static char *strli ( char **p, int *type )
{
      #define __v  (*p)

      if ( __v ) {

        char *oldp = __v;
        int   size;
        char *fname;
        __v = strchr(__v, ',');
        size = strsize(oldp, __v);

        if ( __v ) __v++; else __v = NULL;

        fname = (char *)_malloc(size+1);

        if ( fname ) {

          int i = 1;

          bzero(fname, size+1);
          copy_not_space(fname, oldp, size);

          if ( *fname == '$' ) {/* first char is $, so it's string */

             char *t = stridup(&fname[1], strlen(fname)-1);
             (*type) = INI_STRING;
             _free(fname);
             fname = t;

          } else {

            char *t = transini_line(fname);

            _free(fname);
            fname = t;
            (*type) = INI_DECANUM;

          };


       };

       return fname;

     };

     return NULL;
};


/*
  get arguments or values form the definition strvalue and divide it to sections.

  example of "seal.ini" :

  ...
  [fonts]
  symbol="$hello, 1, $mr., $stencl"
  ...

  char *def;
  ini_data *d = getinidata_fromfile("seal.ini", "fonts");
  char *value = getini_line ( NULL, d, 0);
  char **f = getini_values(value);

  (*f) point to memory of array. Each item contains these information :

  0    byte  = type
  1... bytes = data

  In this case the f[0][0] = INI_STRING and &(f[0][1]) = pointer to memory,
  where string "hello" is stored.

                   f[1][0] = INI_DECANUM and *((long*)(f[1][1])) = 1.
                   f[2][0] = INI_STRING and &(f[2][1]) = pointer to memory, where
  string "mr" is stored.
                   f[3][0] = INI_STRING and &(f[3][1]) = pointer to memory, where
  string "stencl" is stored.

  This array is ended by ZERO. - if f[x] == NULL
*/
char **getini_values ( char *strvalue )
{
  #define ret7(x) do { \
     free(val);       \
     freeini_values(all); \
     return x;        \
  } while (0)
  char **all = NULL;
  int n = 0;
  while ( strvalue ) {
    int type = 0;
    char *val = strli(&strvalue, &type);
    if ( type ) {
      if ( !all ) {
        all = (char**)_malloc((n+2)*sizeof(char*));
        if ( !all ) ret7(NULL);
        memset(all, 0, (n+2)*sizeof(char*));
        n++;
      } else {
        all = (char**)_realloc(all, (n+2)*sizeof(char*));
        if ( !all ) ret7(NULL);
        all[n+1] = NULL;
        n++;
      };
      if ( all ) {
        char *t = NULL;
        if ( type == INI_STRING ) {
          int s = strlen(val);
          t = (char*)_malloc(s+2);
          if ( !t ) ret7(NULL);
          t[0] = (char)type;
          memcpy(&t[1], val, s+1);
        } else {
          t = (char*)_malloc(sizeof(long)+sizeof(char));
          if ( !t ) ret7(NULL);
          t[0] = (char)type;
          *((long*)(&t[1])) = *((long*)val);
        };
        all[n-1] = t;
        all[n] = NULL;
      };
    };
    if ( type == INI_STRING ) _free(val);
  };
  #undef ret7
  return all;
};


/*
  free values you got by function "char **getini_values ( char *strvalue );"
*/
void  freeini_values ( char **values )
{
  if ( values ) {

    char **v = values;

    while ( *v ) {

      char *t = (*v);
      v++;
      _free(t);

    };

    _free(values);
  };
};


/*
 get number of lines in structure (dat), you got by function

 "ini_data *getinidata_fromfile ( char *filename, char *ininame )" ...see above

 example:

 "seal.ini" file :
 ...
 [SEAL]
 info = "desktop environment"
 version = 1
 ...

 ini_data *d = getinidata_fromfile("seal.ini", "SEAL");
 int lines = getini_linenum(d);

 (lines) now equals to 2 = number of lines in [SEAL] structure.
*/
int   getini_linenum ( ini_data *dat )
{
  int lines = 0;
  if ( dat ) {
    while ( *dat ) {
      if ( INI_ISENTER(dat) ) /* enter */
        lines++;
      dat++;
    };
  };
  return lines;
};

static char *while_text ( char *text ) {

  char *oldt = NULL;
  if ( text )
    while ( *text ) {
      if ( *text == INI_CEQUALS ) return oldt;
      if ( !isspace(*text) ) oldt = text+1;
      text++;
    };
  return text;
};


/*
 return information about line (line) from structure (dat), you got by function

 "ini_data *getinidata_fromfile ( char *filename, char *ininame )" ...see above

 example:

 ...
 [SEAL]
 info = "desktop environment"
 version = 1
 ...

 ini_data *d = getinidata_fromfile("seal.ini", "SEAL");
 char *head = NULL;
 char *def = getini_line ( &head, d, 1);

 (def) now contains text "desktop environment" and (head) contains text "info".
*/
char *getini_line ( char **def, ini_data *dat, int line )
{

  int i = 0;
  if ( def ) *def = NULL;

  if ( dat ) {
    while ( *dat && i < line ) {

      if ( INI_ISENTER(dat) ) /* enter */
        i++;

      dat++;
    };

    if ( *dat ) {

      char *cline = NULL;
      char *out = NULL;
      char *e = strchr(dat, INI_ENTERLINE);

      if ( e && *e == INI_ENTERLINE ) e = strchr(e+1, INI_ENTERLINE);

      cline = stridup(dat, strsize(dat, e));

      if ( cline ) {

        char *to = while_text(cline);
        char *eq = strchr(cline, INI_CEQUALS);

        if ( def ) *def = stridup(cline, strsize(cline, to));

        if ( eq ) out = transini_line(eq+1);

      };

      _free(cline);

      return out;
    };
  };

  return NULL;
};


/*
  get arguments form the "function" (one). in (def) is returned pointer to memory, where

  the header is stored and function return pointer to array of values, that line

  contains.

  example of "seal.ini" :

  ...
  [fonts]
  symbol("Hello",-1, "Hi")= "Hello everybody !"
  ...

  char *def;
  ini_data *d = getinidata_fromfile("seal.ini", "fonts");
  char *value = getini_line ( &def, d, 0);
  char **f = getini_function ( &func, def );

  (*def) point to memory, where the text "Hello everybody !" is stored.
  (*f) point to memory of array. Each item contains these information :

  0    byte  = type
  1... bytes = data

  In this case the f[0][0] = INI_STRING and &(f[0][1]) = pointer to memory,
  where string "Hello" is stored.

                   f[1][0] = INI_DECANUM and *((long*)(f[1][1])) = -1.
                   f[2][0] = INI_STRING and &(f[2][1]) = pointer to memory, where
  string "Hi" is stored.

  This array is ended by ZERO. - if f[x] == NULL
*/
char **getini_function ( char **def, char *one )
{
  char *n;
  char *v;
  char *v1;

  if ( !def ) *def = NULL;
  if ( !one ) return NULL;

  n = strchr(one, INI_FUNCFIRST);
  v = NULL;
  v1 = NULL;
  if ( def ) *def = stridup(one, strsize(one, n));
  if ( n ) {
    v = strchr(n+1, INI_FUNCLAST);
    if ( v ) {
      char **x = NULL;
      v1 = stridup(n+1, strsize(n, v)-1);
      x = getini_values(v1);
      _free(v1);
      return x;
    };
  };
  return NULL;
};


/*
 read line (ixname) from inidata (dat) you have got by previous funtion.
*/
char  *getini_value ( ini_data *dat, char *ixname ) {
  if ( dat && ixname ) {
    char *p = dat;
    int ok = 0;
    while ( p && !ok ) {
      p = strstr(p, ixname);
      if ( p && *(p-1) == '\n' ) ok = 1; /* if found it and it's placed at first char in line */
      else if ( p ) p++;
    };
    if ( ok ) {
      char *v = strstr(p+1, INI_EQUALS);
      if ( v ) v = strtoend(v+strlen(INI_EQUALS), INI_ENDLINE);
      p = transini_line(v);
      _free(v);
      return p;
    };
  };
  return NULL;
};


static ini_data *searchini ( char *mem, char *ininame, int _alloc ) {
  if ( mem && ininame ) {
    int   s = strlen(ininame);
    char *p = mem;
    int   ok = 0;
    while ( p && !ok ) {
      p = strstr(p, ininame);
      if ( p && p != mem && *(p-1) == INI_FIRSTCHAR && *(p+s) == INI_LASTCHAR )
        ok = 1; /* I found ininame and first and last ini char */
      else if ( p ) p++;
    };
    if ( ok ) { /* if I found ininame */
      long size = 0;
      char *t = NULL;
      p = p+s+1; /* move to char after last ini char */
      return _alloc?(ini_data *)strtoend(p, INI_FIRSTCHAR):p;
    };
  };
  return NULL;
};


static char *getini_ixname ( ini_data **mem, char *ininame, char *ixname ) {
  if ( mem && *mem && ininame && ixname ) {
    long siz = 0;
    ini_data *v = NULL;
    *mem = searchini(*mem, ininame, 0);
    v = (ini_data *)strtoend(*mem, INI_FIRSTCHAR);
    siz = strlen(v);
    if ( v ) { /* if I found ininame */
      char *p = v;
      int   ok = 0;
      while ( p && !ok ) {
        p = strstr(p, ixname);
        if ( p != v && p && *(p-1) == '\n' )
          ok = 1; /* I found ixname */
        else if ( p ) p++;
      };
      if ( ok ) { /* if I found ixname */
        *mem += p-v;
        p = strtoend(p, '\n');
        _free(v);
        return p;
      };
      p = v+siz-1;
      while ( p != v && *p == '\n' ) {
        p--;
        siz--;
      };
      if ( p != v ) siz++;
      _free(v);
      *mem += siz;
    };
  };
  return NULL;
};


/*
  get number from ini file (filename), from section [ininame] and in the colum (ixname)

  example of seal.ini :

  ...
  [mouse]
  speed = 1
  ....

  long x = getininum_fromfile ("seal.ini", "mouse", "speed");

  x contains 1
*/
long getininum_fromfile ( char *filename, char *ininame, char *ixname )
{

  long*x = ((long*)getini_fromfile(filename, ininame, ixname));

  if ( x ) return (*x);

  else return 0;

};


/*
 read data from file (filename) and structure (ininame).

 This read all lines from structure [ininame] to next

 structure or to end of the file.
*/
ini_data *getinidata_fromfile ( char *filename, char *ininame ) {
  FILE *f = fopen(filename, "rt");
  ini_data *d = NULL;
  if ( f ) {
    long size = filelength(fileno(f));
    char *mem = (char *)_malloc(size+1);
    if ( mem ) {
      bzero(mem, size+1);
      fread(mem, size, 1, f);
      d = searchini(mem, ininame, 1);
    };
    _free(mem);
    fclose(f);
  };
  return d;
};



/*
 return color from INI file (filename), from structure (ininame),

 from line (ixname).

 typedef struct ini_rgb {

   int r; - index of red color
   int g; - index of green color
   int b; - index of blue color
   int x; - reserved

 } ini_rgb;

 example:

 "seal.ini" file :
 ...
 [colors]
 desktop = "255, 0, 128"
 ...

 ini_rgb rgb = geini_color("seal.ini", "colors", "desktop");

 now rgb contains in r (255), in g (0) and in b (128).
*/
ini_rgb  getini_color ( char *filename, char *ininame, char *ixname )
{
  ini_rgb xcolor;

  char *scolor  = getini_fromfile (filename, ininame, ixname);

  xcolor.r = 0;
  xcolor.g = 0;
  xcolor.b = 0;

  if ( scolor ) {

    char **v = getini_values ( scolor );
    int     r = 0;
    int     g = 0;
    int     b = 0;

    if ( v && v[0] && *(v[0]) == INI_DECANUM )
      r = *((long*)&(v[0][1]));
    if ( v && v[1] && *(v[1]) == INI_DECANUM )
      g = *((long*)&(v[1][1]));
    if ( v && v[2] && *(v[2]) == INI_DECANUM )
      b = *((long*)&(v[2][1]));

    xcolor.r = r;
    xcolor.g = g;
    xcolor.b = b;

    freeini_values(v);

  };

  _free(scolor);

  return xcolor;
};


/*
 read value that is set in file (filename) in structure (ininame)

 and in line (ixname)

 example :

 seal.ini file :
 ...
 [SEAL]
 info = "desktop environment"
 version = 1
 ...

...to get, what is set in [SEAL], [info] line you simple call

char *i = getini_fromfile("seal.ini", "SEAL", "info");


...to get, what is set in [SEAL], [version] line you simple call

long  v = *((long *)getini_fromfile("seal.ini", "SEAL", "version"));

...(i) now contains text : "desktop environment" and (v) number 1.
*/
char *getini_fromfile ( char *filename, char *ininame, char *ixname ) {
  ini_data *vx = getinidata_fromfile(filename, ininame);
  char *p = NULL;
  if ( vx ) p = getini_value(vx, ixname);
  _free(vx);
  return p;
};



static char *setini_toline ( char *ixname, char *value ) {
   return addtomem(NULL, 0, ixname, "=", value, "\n");
};


static char *setini_todata ( char *ixname, char *value, int type ) {
  if ( ixname ) {
    switch ( type ) {
      case INI_STRING: { /* string */
        int size = strlen(value);
        if ( size ) { /* string exist */
          char *v = _malloc(size+3);
          if ( v ) {
            char *ok = NULL;
            bzero(v, size+3);
            v[0] = INI_TEXTCHAR;
            strcat(v, value);
            v[size+1] = INI_TEXTCHAR;
            ok = setini_toline(ixname, v);
            _free(v);
            return ok;
          };
        } else { /* string not exist */
          return setini_toline(ixname, INI_NONE);
        };
      }; break;
      case INI_HEXANUM: { /* hexa number */
        INI_NUMBER x = *((INI_NUMBER*)(value));
        char cx1[33];
        char cx2[33+2] = {'\0'};
        strcat(cx2, "0x");
        return setini_toline(ixname, strcat(cx2, itoa(x, cx1, 16)));
      }; break;
      case INI_DECANUM: { /* deca number */
        INI_NUMBER x = *((INI_NUMBER*)(value));
        char cx1[33];
        return setini_toline(ixname, itoa(x, cx1, 10));
      }; break;
    };
  };
  return NULL;
};


/*
 set or write "value" into file (filename) to structure

 (ininame) and to line (ixname). Value is pointer to value ( text or number ),

 type is type of pointer. ( INI_DECANUM, INI_HEXANUM, INI_STRING ).

 example :

 l_int i = 1;

 setini_tofile("seal.ini", "SEAL", "info", "FREE desktop environment", INI_STRING);
 setini_tofile("seal.ini", "SEAL", "version", &i, INI_DECANUM);
 setini_tofile("seal.ini", "SEAL", "hexa version", &i, INI_HEXANUM);

 Output of "seal.ini" file :
 ...
 [SEAL]
 info = "FREE desktop environment"
 version = 1
 hexa version = 0x1
 ...
*/
void  setini_tofile ( char *filename, char *ininame, char *ixname, char *value, int type )
{
  if ( ininame && ixname ) {
    FILE *f = fopen(filename, "rt");
    if ( !f ) { /* if file not exist then create new one */
      f = fopen(filename, "wt");
      fclose(f);
      f = fopen(filename, "rt");
    };
    if ( f ) {
      long size = filelength(fileno(f));
      long six  = 0;
      char *mem = (char *)_malloc(size+1);
      if ( mem ) {
        char *p = mem;
        char *ixptr;
        bzero(mem, size+1);
        fread(mem, size, 1, f);
        ixptr = getini_ixname(&p, ininame, ixname);
        fclose(f);
        f = fopen(filename, "wt"); /* reopen file in write mode */
        if ( f ) {
          if ( p ) { /* if iname was found */
            fwrite(mem, (p-mem), 1, f); /* write to ixname */
            if ( ixptr ) { /* if ixname was found */
              six = strlen(ixptr);
              _free(ixptr);
              ixptr = setini_todata(ixname, value, type);
              fwrite(ixptr, strlen(ixptr)-1, 1, f);
              fwrite(p+six, size-(((long)p+six)-(long)mem), 1, f);
            } else { /* if ixname wasn't found */
              ixptr = setini_todata(ixname, value, type);
              fwrite(ixptr, strlen(ixptr), 1, f);
              fwrite(p, size-((long)p-(long)mem), 1, f);
            };
          } else { /* if iname wasn't found */
            long sz = strlen(mem);
            fwrite(mem, sz, 1, f); /* write to end of file */
            if ( mem[sz-1] != 10 )
              fprintf(f, "\n"); /* enter line */
            fprintf(f, "\n"); /* enter line */
            ixptr = addtomem(NULL, 0, INI_CFIRSTCHAR, ininame, INI_CLASTCHAR, "\n");
            fwrite(ixptr, strlen(ixptr), 1, f);
            _free(ixptr);
            ixptr = setini_todata(ixname, value, type);
            fwrite(ixptr, strlen(ixptr), 1, f);
          };
        };
        fclose(f);
        _free(ixptr);
        _free(mem);
      };
    };
    fclose(f);
  };
};

