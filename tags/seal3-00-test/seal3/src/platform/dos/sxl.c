/******************************************************************
 * Seal 3.0                                                       *
 * Copyright (c) 1999-2002 Seal Developers. All Rights Reserved.  *
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

/*
 * SEAL 3 Dynamic Linking System version 1.0.1
 * SXA/SXL Loader
 * Copyright (C) 2002 Kostas Michalopoulos aka Bad Sector
 *
 * Revision History:
 *
 * 09/06/2002: First release - 1.0.1 (Bad Sector)
 * 15/06/2002: Small changes (orudge)
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define ___SXL_HOST___
#include <platform/dos/sxl.h>

#define SXLFLAG_NOFLAGS 		0x0000
#define SXLFLAG_NOMAIN  		0x0001
#define SXLFLAG_NOEND                   0x0002

unsigned char	last_sxl_error = SXLERROR_NOERROR;
void            *last_sxl_error_data = 0;

typedef struct t_export *p_export;
typedef struct t_export
{
   char  	*name;
   void  	*addr;

   p_export 	next;
} t_export;

typedef struct t_sxl *p_sxl;
typedef struct t_sxl
{
  h_sxl         handle;
  void  	*code;
  int		(*main)(char *args);
  int		(*end)();
  p_export 	exp, last;

  p_sxl 	prev;
  p_sxl 	next;
} t_sxl;

p_sxl   list = 0, last = 0;
h_sxl   next_handle = 1;

typedef struct t_sym *p_sym;
typedef struct t_sym
{
  h_sxl handle;
  char  *name;
  void  *addr;

  p_sym prev;
  p_sym next;
} t_sym;

p_sym   sym_list = 0, sym_last = 0;

char *read_str(FILE *f)
{
   unsigned short length;
   char *s;

   fread(&length, 1, 2, f);
   s = (char *) malloc(length+1);
   fread(s, 1, length, f);
   s[length] = 0;

   return s;
}

long get_sym_addr(char *name)
{
   p_sym sym = sym_list;

   if (!sym)
      return -1;

   while (sym)
   {
      if (!strcmp(sym->name, name))
         return (long) sym->addr;

      sym = sym->next;
   };

   return -1;
}

void register_symbol(h_sxl handle, char *name, void *addr)
{
   p_sym sym;

   if (!name || !addr)
      return;

   if (get_sym_addr(name) == -1)
   {
      sym = (p_sym) malloc(sizeof(t_sym));
      sym->handle = handle;
      sym->name = strdup(name);
      sym->addr = addr;
      sym->prev = sym_last;
      sym->next = 0;

      if (sym_list)
         sym_last->next = sym; else sym_list = sym;

      sym_last = sym;
   }
   else
   {
      sym = sym_list;

      while (sym)
      {
         if (!strcmp(sym->name, name))
         {
            sym->addr = addr;
            return;
         }

         sym = sym->next;
      }
   }
}

h_sxl sxl_load(char *fname, char *args)
{
   sxl_load_ex(fname, args, NULL);
}

h_sxl sxl_load_ex(char *fname, char *args, h_sxl *handle)
{
   typedef struct t_ext {
      char *name;
      int symbol;
   } t_ext;
   struct {
     unsigned long magic;
     unsigned long timestamp;
     unsigned long size;
     unsigned long relocs;
     unsigned long externals;
     unsigned short flags;
     unsigned long main_pos;
     unsigned long end_pos;
     unsigned long handle_pos;
   } header;
   struct {
     unsigned char type;
     unsigned long addr;
     unsigned long symbol;
   } reloc;

   p_sxl sxl;
   t_ext *ext;
   FILE *f;
   int i;

   if (!fname || !fname[0])
      return 0;

   f = fopen(fname, "rb");

   last_sxl_error = SXLERROR_NOERROR;

   if (!f)
   {
      last_sxl_error = SXLERROR_FILENOTFOUND;
      return 0;
   }

   sxl = (p_sxl) malloc(sizeof(t_sxl));

   if (!sxl)
   {
      last_sxl_error = SXLERROR_NOTENOUGHMEMORY;
      return 0;
   }

   memset(sxl, 0, sizeof(t_sxl));
   sxl->handle = next_handle;

   fread(&header, 1, sizeof(header), f);
   ext = (t_ext *) malloc(sizeof(t_ext)*header.externals);

   if (!ext)
   {
      fclose(f);
      free(sxl);
      last_sxl_error = SXLERROR_NOTENOUGHMEMORY;

      return 0;
   }

   sxl->code = (void *) malloc(header.size);
 
   if (!sxl->code)
   {
      fclose(f);
      free(ext);
      free(sxl);
      last_sxl_error = SXLERROR_NOTENOUGHMEMORY;

      return(0);
   }

   fread(sxl->code, 1, header.size, f);

   for (i = 0; i < header.externals; i++)
   {
      ext[i].name = read_str(f);
      fread(&ext[i].symbol, 1, 4, f);
   }

   for (i = 0; i < header.relocs; i++)
   {
      fread(&reloc, 1, sizeof(reloc), f);
      if (reloc.type == 6)
      {
         int j, addr = (long)sxl->code;

         for (j=0;j<header.externals;j++)
         {
            if (ext[j].symbol == reloc.symbol)
            {
               addr = get_sym_addr(ext[j].name);
               break;
            }
         }

         *(long*)((sxl->code)+reloc.addr) += addr;
      }
      else
      {
         int j, addr = -2;

         for (j = 0; j < header.externals; j++)
         {
            if (ext[j].symbol == reloc.symbol)
            {
               addr = get_sym_addr(ext[j].name);
               break;
            }
         }

         if (addr == -1 || addr == -2)
         {
            int ii;

            last_sxl_error = SXLERROR_UNRESOLVEDEXTERNAL;

            if (addr == -1)
               last_sxl_error_data = (void *) strdup(ext[j].name);
            else
               last_sxl_error_data = 0;

            for (ii=0;ii<header.externals;ii++)
               free(ext[ii].name);

            free(ext);
            free(sxl->code);

            return 0;
         }

         *(long*)((sxl->code)+reloc.addr) += (long)addr-(long)(sxl->code);
      }
   }

   for (i = 0;i < header.externals; i++)
      free(ext[i].name);

   free(ext);
   fclose(f);

   if (header.flags & SXLFLAG_NOMAIN)
      sxl->main = 0;
   else
      sxl->main = (sxl->code)+(header.main_pos);

   if (header.flags & SXLFLAG_NOEND)
      sxl->end = 0;
   else
      sxl->end = (sxl->code)+(header.end_pos);

   next_handle++;

   if (!next_handle)
      next_handle++; /* next_handle must never be 0 */

   memcpy(sxl->code+header.handle_pos, &sxl->handle, sizeof(sxl->handle));

   sxl->prev = last;

   if (list)
      last->next = sxl; else list = sxl;

   last = sxl;

   if (handle)
      *handle = sxl->handle;


   if (sxl->main)
      sxl->main(args);

   return sxl->handle;
}

p_sxl sxl_find(h_sxl handle)
{
   p_sxl sxl = list;

   while (sxl)
   {
      if (sxl->handle == handle)
         return sxl;
      else
         sxl = sxl->next;
   }

   last_sxl_error = SXLERROR_LIBRARYNOTFOUND;
   return 0;
}

int export_symbol(h_sxl handle, char *name, void *addr)
{
   p_sxl sxl = sxl_find(handle);
   p_export exp;

   if (!sxl)
      return 0;
  
   exp = (p_export) malloc(sizeof(t_export));
   exp->name = strdup(name);
   exp->addr = addr;
   exp->next = 0;

   if (sxl->exp)
      sxl->last->next = exp;
   else
      sxl->exp = exp;

   sxl->last = exp;
}

void *get_symbol(h_sxl handle, char *name)
{
   p_sxl sxl = sxl_find(handle);
   p_export exp;

   if (!sxl || !name)
      return 0;

   exp = sxl->exp;

   while (exp)
   {
      if (!strcmp(name, exp->name))
         return exp->addr;

      exp = exp->next;
   };

   return 0;
}

void sxl_free(h_sxl handle)
{
   p_sxl sxl = sxl_find(handle);
   p_sym sym = sym_list;

   if (!sxl)
      return;

   /* call end() */
   if (sxl->end)
      sxl->end();

   /* un-register and free symbols that are registered from this library */
   while (sym)
   {
      p_sym next = sym->next;

      if (sym->handle == sxl->handle)
      {
         if (sym->prev)
            sym->prev->next = sym->next;
         else
            sym_list = sym->next;

         if (sym->next)
            sym->next->prev = sym->prev;
         else
            sym_last = sym->prev;

         free(sym->name);
         free(sym);
      }

      sym = next;
   };

   /* free exported symbols */
   while (sxl->exp)
   {
      sxl->last = sxl->exp->next;
      free(sxl->exp->name);
      free(sxl->exp);
      sxl->exp = sxl->last;
   }
  
   if (sxl->prev)
      sxl->prev->next = sxl->next;
   else
      list = sxl->next;

   if (sxl->next)
      sxl->next->prev = sxl->prev;
   else
      last = sxl->prev;

   if (sxl->code)
      free(sxl->code);

   free(sxl);
}

void free_all_libraries()
{
   while (list)
   {
      last = list->next;
      sxl_free(list->handle);
      list = last;
   }
}

void initialize_sxl_system()
{
//   atexit(free_all_libraries);
   SXL_HOST_REGISTER(register_symbol);
   SXL_HOST_REGISTER(export_symbol);
   SXL_HOST_REGISTER(sxl_load);
   SXL_HOST_REGISTER(get_symbol);
   SXL_HOST_REGISTER(sxl_free);
}

