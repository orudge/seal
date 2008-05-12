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

typedef struct t_xb_module *p_xb_module;

typedef struct t_xb_app {

  p_list vars;

} t_xb_app, *p_xb_app;

typedef struct t_xb_module {
  p_xb_app app;
  p_list   vars;


} t_xb_module;

#define ERR_FORGOT_PARENTESE 0
#define ERR_FORGOT_GUILLEMET 1

void xb_error ( p_xb_module mod, l_uint errcode ) {



};

void xb_parse ( p_xb_module mod, l_text *txt, l_uint *a, l_uint *p, l_uint *g, l_char *c ) {
  if ( *c )(*a)++;
  while ( (*txt)[*a] ) {
    if ( (*txt)[*a] == '"' ) (*g)=(*g)?0:1;
    if ( (*txt)[*a] == '(' ) (*p)++;
    if (  !(*g) && !(*p)  ) {
      *c = (*txt)[*a];
      return;
    };
    if ( (*txt)[*a] == ')' ) (*p)--;
    (*a)++;
  };
  if ( (*p) ) xb_error(mod,ERR_FORGOT_PARENTESE);
  if ( (*g) ) xb_error(mod,ERR_FORGOT_GUILLEMET);

  *c=0;
  return;
};

l_long nbpower ( l_long nb, l_long a ) {
  l_long result = nb;
  while ( a > 1 ) {
	result *=nb;
	a--;
  };
  return result;
};

l_long xb_calculate ( p_xb_module mod, l_text txt );

l_long xb_calculate_ex ( p_xb_module mod, l_text txt, l_char ac, l_char bc ) {
  l_uint a = 0, p = 0, g = 0, st =0;
  l_char c = 0, oldc = NULL;
  l_long result = 0;

  do {
    xb_parse(mod,&txt,&a,&p,&g, &c);
    if ( c == ac || c == bc || !c ) {
	  l_uchar mg = txt[st] == '(' ? 1 : 0;
      l_text sec = stridup(txt+st+mg,a-st-(mg*2));
      l_long nb = xb_calculate(mod,sec);
      _free(sec);
      switch ( oldc ) {
        case  0  : result = nb; break;
        case '+' : result += nb; break;
        case '-' : result -= nb; break;
        case '*' : result *= nb; break;
        case '/' : result /= nb; break;
        case '^' : result = nbpower(result,nb); break;
      };
      if ( !c ) return result;
      st = a + 1;
      oldc = c;
    };
  } while ( c );

  return result;
};

l_uchar xb_calculate_type ( p_xb_module mod, l_text txt ) {
  l_uint a = 0, p = 0, g = 0, st =0;
  l_char c = 0;
  l_uint comp[3] = { 0, 0, 0 };

  do {
    xb_parse(mod,&txt,&a,&p,&g, &c);
    if ( c == '+' || c == '-' ) comp[0]++;
    if ( c == '*' || c == '/' ) comp[1]++;
    if ( c == '^' ) comp[2]++;
  } while ( c );

  if ( comp[0] ) return 1;
  if ( comp[1] ) return 2;
  if ( comp[2] ) return 3;
  return 0;
};

l_long xb_calculate ( p_xb_module mod, l_text txt ) {
  l_uchar t = xb_calculate_type(mod,txt);
  if ( t == 1 )
    return xb_calculate_ex(mod,txt,'+','-');
  else if ( t == 2 )
    return xb_calculate_ex(mod,txt,'/','*');
  else if ( t == 3 )
    return xb_calculate_ex(mod,txt,'^', 0 );
  else
    return atoi(txt);
};

void xb_tester ( void ) {

 DEBUG_printf("->%d\n",xb_calculate(NULL,"(10*10+200+300+4000)/10^2"));

};

app_begin(void)
{
   if (ap_process == AP_INIT)
   {
       xb_tester();

   }
} app_end;
