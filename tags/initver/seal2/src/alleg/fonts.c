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
#include <fonts.h>
#include <vfile.h>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
t_fonts_loaded *system_fonts = NULL;
void   *font_system = NULL;
l_text  system_font_name = "ARIAL";

/*l_text  file_getbin ( l_text filename ) {
  FILE *f = SealFileOpen(filename, "rb");
  if ( f ) {
      l_long size = filelength(fileno(f));
      l_text text = _malloc(size+1);
      if ( text ) {
         clear_type(text, size+1);
         fread(text, size, 1, f);
      };
      fclose(f);
      return text;
  };
  return NULL;
};*/

////////////////////////////////////////////////////////////////////////////////
int    add_font_to_system ( void *font, char *name, int weight )
{
  t_fonts_loaded *s = system_fonts;
  t_fonts_loaded *n;
  t_fonts_loaded *last = NULL;
  int             fh = 0;
  if ( !font || !name ) return 0;
  while ( s ) {
    last = s;
    if ( (fh == s->weight) && !stricmp(s->name, name) ) return 2;
    s = s->next;
  };
  n = (t_fonts_loaded *)_malloc(sizeof(t_fonts_loaded));
  if ( !n ) return 0;
  memset(n, 0, sizeof(t_fonts_loaded));
  if ( last ) last->next = n;
  else system_fonts = n;
  n->font = font;
  n->weight = weight;
  n->name = _strdup(name);
  return 1;
};

////////////////////////////////////////////////////////////////////////////////
// Standart sizes of font
l_int font_sizes[] = { 8, /*9,*/ 10, /*11, 12, */ 14, 16, /* 18,*/ 20, /*22, 24, 26, 28, 36, 48, 72,*/ 0};

////////////////////////////////////////////////////////////////////////////////
// Load fonts from path
void    load_supported_fonts ( l_text path ) {
  struct t_ffblk f; // Delcare the struct to list directory

  l_int done = 0;

  l_text fi = io_realpath(path, "*.*"); // Prepare wilcard

  DEBUG_printf ("  - Load TTF and FNT fonts from path '%s'\n", path );

  done = io_findfirst(fi, &f, FA_ARCH); // Start path listing

  while ( !done ) {
    l_text file = io_realpath(path, f.ff_filename); // Generate the full filename
    l_text ext  = get_extension(file);
    l_text name = mid(f.ff_filename,0,strlen(f.ff_filename)-((ext)?strlen(ext)+1:0)); // Keep only filename without the expension
    DEBUG_printf ("   - Load ''%s'', named ''%s''\n",file,name);

    if ( ext && !stricmp(ext,"TTF") ) { // Check if it's a ttf file
      l_int a = 0;
      while ( font_sizes[a] ) {  // Will repeat the loading font for all sizes
        void *fnt = seal_load_font(file,NULL,font_sizes[a],font_sizes[a],-1,-1); // Load font with size N°a in table font_sizes
          if ( fnt ) // If load font succeed
            add_font_to_system ( fnt, name, font_sizes[a] ); // Add font in size in the system list (systems_fonts)
          else // If load don't succeed
            DEBUG_printf("    - Error :: Unable to load font\n");
        a++;
      };
    } else if ( ext && !stricmp(ext,"FNT") ) {
      FONT *f = load_font(GetFile(file));
      if ( f )
        add_font_to_system ( f, name, text_height(f) );
      else
        DEBUG_printf("    - Error :: Unable to load font\n");
    } else { // If it's not a ttf file
      DEBUG_printf("    - Error :: Non-TTF/FNT, not supported\n");
    };
    if ( name ) _free(name);
    if ( file ) _free(file);
    done = io_findnext(&f); // Find next file in path
  };
  font_system = get_font_in_size ( system_font_name, 8, 8 );
  _free(fi); // Free the wilcard ("path/*.*")
};

////////////////////////////////////////////////////////////////////////////////
// Unload loaded system fonts
void    unload_system_fonts ( void )
{
  /* did nothing - what a pity */
  t_fonts_loaded *s = system_fonts;
  t_fonts_loaded *last = NULL;
  while ( s ) {
    last = s;
    _free(s->name);
    s = s->next;
    _free(last);
  };
};
////////////////////////////////////////////////////////////////////////////////
void fonts_done ( void ) {
  unload_system_fonts( );
};
////////////////////////////////////////////////////////////////////////////////
// Return the font fontname in the standart size thz most close of w
void *get_font_in_size ( char *fontname, int w, int h )
{
  t_fonts_loaded *s = system_fonts;
  l_int a = 1;
  l_int stw = w;/*8; // Standart size
  while ( font_sizes[a] ) { /
    if ( w == font_sizes[a] )
      stw = font_sizes[a];
    else if ( w > font_sizes[a-1] && w < font_sizes[a] )
      stw = font_sizes[a];
    a++;
  };
  if ( w > font_sizes[a-1] ) stw = font_sizes[a-1];*/
  while ( s ) {
    if ( s->weight == stw && fontname && s->name && !stricmp(fontname, s->name) ) {
      return s->font;
    };
    s = s->next;
  };
  return get_font_in_size(system_font_name,10,10);
};
