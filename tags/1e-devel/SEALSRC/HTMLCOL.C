/****************************************************************/
/*                                                              */
/*                           htmlcol.c                          */
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


#define HTML_MAX_STR_COLORS    256


HTML_DEF_COLOR  HTML_COLORS[HTML_MAX_STR_COLORS] =
{
 0x000000, 0xC0C0C0, 0x808080, 0xFFFFFF, 0xFF0000, 0x800000, 0xFF00FF,
 0x008000, 0x00FF00, 0x808000, 0x808000, 0xFFFF00, 0x0000FF, 0x000080,
 0x00FFFF
};


char*  HTML_COLOR_NAMES[HTML_MAX_STR_COLORS] =
{
"black", "silver", "gray", "white", "maroon", "red", "purple", "fuchsia",
"green", "lime", "olive", "yellow", "navy", "blue", "teal", "aqua",
"antiquewhite", "aquamarine", "azure",
"beige", "bisque", "blanchedalmond", "blueviolet", "brown",
"burlywood", "cadetblue", "chartreuse", "chocolate", "coral", "cornflowerblue",
"cornsilk", "crimson", "cyan", "darkblue", "darkcyan", "darkgoldenrod",
"darkgrey", "darkgreen", "darkkhaki", "darkmagenta", "darkolivegreen", "darkorange",
"darkorchid", "darkred", "darksalmon", "darkseagreen", "darkslateblue", "darkslategray",
"darkturquoise", "darkviolet", "deeppink", "deepskyblue", "dimgray", "dodgerblue",
"firebrick", "floralwhite", "forestgreen","gainsboro", "ghostwhite",
"gold", "goldenrod", "greenyellow", "honeydew", "hotpink",
"indianred", "indigo", "ivory", "khaki", "lavender", "lavenderblush", "lawngreen",
"lemonchiffon", "lightblue", "lightcoral", "lightcyan", "lightgoldenrodyellow",
"lightgreen", "lightgray", "lightpink", "lightsalmon", "lightseagreen", "lightskyblue",
"lightslategray", "lightsteelblue", "lightyellow", "limegreen", "limen",
"magenta", "mediumaquamarine", "mediumblue", "mediumorchid", "mediumpurple",
"mediumseagreen", "mediumslateblue", "mediumspringgreen", "mediumturquoise",
"mediumvioletred", "midnightblue", "mintcream", "mistyrose", "moccasin", "navajowhite",
"oldlace", "olivedrag", "orange", "orangered", "orchid", "palegoldenrod",
"palegreen", "paleturquoise", "palevioletred", "papayawhip", "peachpuff", "peru",
"pink", "plum", "powderblue", "rosybrown", "royalblue", "saddlebrown",
"salmon", "sandybrown", "seagreen", "seashell", "sienna", "skyblue",
"slateblue", "slategray", "snow", "springgreen", "steelblue", "tan",
"thistle", "tomato", "turqouise", "violet", "wheat", "whitesmoke",
"yellowgreen",

 NULL
};


long  find_str_color ( char *str )
{
  int    i = 0;

  _while ( HTML_COLOR_NAMES[i] && i < HTML_MAX_STR_COLORS ) {

    if ( !stricmp(HTML_COLOR_NAMES[i], str) )

       return HTML_COLORS[i];

    i++;

  };

  return 0;
};


/*
  color from string
*/
HTML_DEF_COLOR   ret_color_from_str ( char *str )
{

  long color = ret_size_id_from_str(str, NULL);

  if ( !color )

    color = find_str_color(str);


  return (HTML_DEF_COLOR)HTML_COLOR_RGB_SET(color);

};

