/****************************************************************/
/*                                                              */
/*                         dataini.h                            */
/*                                                              */
/*               Seal - free desktop environment                */
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


#ifndef __DATAINI_H_INCLUDED__
#define __DATAINI_H_INCLUDED__

#define INI_STRING      1
#define INI_HEXANUM     2
#define INI_DECANUM     3

/*
  from "c" return red color
  F.e. "c" == 0xFF0015
  Red color is FF
  Green color is 00
  Blue color is 15
*/

#define getini_red(c)   ((unsigned char)((c)>>16))
#define getini_green(c) ((unsigned char)(((c)<<16)>>24))
#define getini_blue(c)  ((unsigned char)(((c)<<24)>>24))

#define ini_data  char

/*
   used as an output for
   "ini_rgb getini_color ( char *filename, char *ininame, char *ixname );"
   function.
*/
typedef struct ini_rgb {

  int r; /* red */
  int g; /* green */
  int b; /* blue */
  int x; /* reserved */

} ini_rgb;

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
long      getininum_fromfile ( char *filename, char *ininame, char *ixname );


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
ini_rgb   getini_color ( char *filename, char *ininame, char *ixname );

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
void      setini_tofile ( char *filename, char *ininame, char *ixname, char *value, int type );

/*
 read line (ixname) from inidata (dat) you have got by previous funtion.
*/
char     *getini_value ( ini_data *dat, char *ixname );

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
char     *getini_fromfile ( char *filename, char *ininame, char *ixname );

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
char    **getini_function ( char **def, char *one );

/*
 read data from file (filename) and structure (ininame).
 This read all lines from structure [ininame] to next
 structure or to end of the file.
*/
ini_data *getinidata_fromfile ( char *filename, char *ininame );

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
char     *getini_line ( char **def, ini_data *dat, int line );

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
char    **getini_values ( char *strvalue );

/*
  free values you got by function "char **getini_values ( char *strvalue );"
*/
void      freeini_values ( char **values );

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
int       getini_linenum ( ini_data *dat );


/* end of dataini.h */
#endif

