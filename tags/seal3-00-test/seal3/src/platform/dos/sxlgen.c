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
 * SXA/SXL Generator
 * Copyright (C) 2002 Kostas Michalopoulos aka Bad Sector
 *
 * Revision History:
 *
 * 09/06/2002: First release - 1.0.1 (Bad Sector)
*/

#include <stdlib.h>
#include <stdio.h>
#include <coff.h>
#include <time.h>

#define SXL_MAGIC       0x55771169

#define SXLFLAG_NOFLAGS 0x0000
#define SXLFLAG_NOMAIN  0x0001
#define SXLFLAG_NOEND   0x0002

typedef struct t_sxl_head
{
  unsigned long magic;
  unsigned long timestamp;
  unsigned long size;
  unsigned long relocs;
  unsigned long externals;
  unsigned short flags;
  unsigned long main_pos;
  unsigned long end_pos;
  unsigned long handle_pos;
} t_sxl_head;

typedef struct t_sxl_reloc
{
  unsigned char type;
  unsigned long addr;
  unsigned long symbol;
} t_sxl_reloc;

t_sxl_head header;

void delete_tmp_file()
{
  remove("_sxl_tmp.o");
}

void write_str(FILE *f, char *s)
{
  unsigned short length = strlen(s);
  fwrite(&length, 1, 2, f);
  fwrite(s, 1, length, f);
}

int convert(FILE *o, char *fname)
{
  FILE          *f = fopen(fname, "rb");
  FILHDR        head;
  SCNHDR        *sections;
  RELOC         reloc;
  SYMENT        *symbols;
  char          *strings;
  int           data_size;
  void          *data;
  int           i;
  int           main_pos = -1, end_pos = -1, handle_pos = -1;
  t_sxl_reloc   rlc;
  int          (*mp)();

  if (!f) return 0;

  fread(&head, 1, FILHSZ, f);
  if (head.f_nscns != 1) {  /* more than one sections */
    char cmd[1024];
    fclose(f);

    strcpy(cmd, "ld -X -S -r -o _sxl_tmp.o ");
    strcat(cmd, fname);
    strcat(cmd, " -T sxl.ld");
    if (system(cmd)) {
      printf("Error: an error occured while calling LD.EXE\n");

      return 1;
    };
    f = fopen("_sxl_tmp.o", "rb");
    if (!f) {
      printf("Error: cannot open _sxl_tmp.o file\n");

      return 1;
    } else
      atexit(delete_tmp_file);
    fread(&head, 1, FILHSZ, f);
  };
  
  fseek(f, head.f_opthdr, SEEK_CUR);
  sections = (SCNHDR *) malloc(SCNHSZ*head.f_nscns);
  fread(sections, head.f_nscns, SCNHSZ, f);

  data_size = sections[0].s_paddr+sections[0].s_size;
  data = (void *) malloc(data_size);
  memset(data, 0, data_size);

  header.size = data_size;

  fseek(f, sections[0].s_scnptr, SEEK_SET);
  fread(data, 1, sections[0].s_size, f);

  fwrite(data, 1, data_size, o);

  fseek(f, head.f_symptr, SEEK_SET);
  symbols = (SYMENT *) malloc(SYMESZ*head.f_nsyms);
  fread(symbols, head.f_nsyms, SYMESZ, f);
  fread(&i, 1, 4, f);
  strings = (char *) malloc(i-4);
  memset(strings, 0, 4);
  fread(strings+4, 1, i-4, f);

  for (i=0;i<head.f_nsyms;i++) {
    char *name, nam8[9];
    if (symbols[i].e.e.e_zeroes) {
      memcpy(nam8, symbols[i].e.e_name, 8);
      nam8[8] = 0;
      name = nam8;
    } else name = strings+symbols[i].e.e.e_offset;

    if (!stricmp(name, "_main")) {
      main_pos = symbols[i].e_value;
    } else
    if (!stricmp(name, "_end")) {
      end_pos = symbols[i].e_value;
    } else
    if (!stricmp(name, "_sxl_handle")) {
      handle_pos = symbols[i].e_value;
    } else
    if (name[0] && name[0] != '.' && !symbols[i].e_scnum && !symbols[i].e_value) { // external symbol
      write_str(o, name);
      fwrite(&i, 1, 4, o);
      header.externals++;
    };

    i += symbols[i].e_numaux;
  };

  {
    int r;

    fseek(f, sections[0].s_relptr, SEEK_SET);
    for (r=0;r<sections[0].s_nreloc;r++) {
      fread(&reloc, 1, RELSZ, f);
      header.relocs++;
      rlc.type = reloc.r_type;
      rlc.addr = reloc.r_vaddr;
      rlc.symbol = reloc.r_symndx;
      fwrite(&rlc, 1, sizeof(rlc), o);
    };
  };

  fclose(f);

  free(strings);
  free(symbols);
  free(data);
  free(sections);

  if (main_pos == -1)
    header.flags |= SXLFLAG_NOMAIN;
  else
    header.main_pos = main_pos;
  if (end_pos == -1)
    header.flags |= SXLFLAG_NOEND;
  else
    header.end_pos = end_pos;
  if (handle_pos == -1) {
    printf("Error: `handle' not defined!\n");

    return 0;
  };
  header.handle_pos = handle_pos;

  return 1;
}

main(int args, char **argv)
{
  FILE *f;
  char fname[256];
  int i, j = -1;
  int make_x = 0;

  printf("SXL Generator version 1.0.1\n");
  printf("Copyright (C) 2002 Kostas Michalopoulos aka Bad Sector\n");
  printf("this program is distributed under the terms of the GNU GPL\n");
  if (args==1) {
    printf("Usage: sxlgen filename[.o] [-x]\n\n");
    printf("  -x     Makes an executable instead of a library\n");

    return 0;
  };
  if ((args==3) && (strcmp(argv[2], "-x")))
  {
    printf("Invalid option: %s\n", argv[2]);

    return 0;
  };
  if ((args==3) && (!strcmp(argv[2], "-x")))
    make_x = 1;

  strcpy(fname, argv[1]);
  for (i=strlen(fname);i>0;i--) if (fname[i] == '.') {
    j = i;

    break;
  };
  if (j == -1) strcat(fname, ".o");

  f = fopen(fname, "rb");
  if (!f) {
    printf("cannot open %s\n", fname);

    return 0;
  };
  strcpy(fname, argv[1]);
  if (make_x)
  {
    if (j == -1) strcat(fname, ".sxa"); else strcpy(fname+j, ".sxa");
  }
  else
  {
    if (j == -1) strcat(fname, ".sxl"); else strcpy(fname+j, ".sxl");
  }
  fclose(f);
  f = fopen(fname, "wb+");
  if (!f) {
    printf("cannot make %s\n", fname);

    return 0;
  };
  memset(&header, 0, sizeof(header));
  header.magic = SXL_MAGIC;
  header.timestamp = time(0);
  header.flags = SXLFLAG_NOFLAGS;
  fwrite(&header, 1, sizeof(header), f);

  if (!convert(f, argv[1])) {
    fclose(f);
    remove(fname);

    return 1;
  };

  fseek(f, 0, SEEK_SET);
  fwrite(&header, 1, sizeof(header), f);
  printf("Converted. Filename = %s  Length = ", fname);
  i = filelength(fileno(f));
  if (i < 1024)
    printf("%i Bytes.\n", i);
  else
    if (i < 1024*1024)
      printf("%i KBytes.\n", i / 1024);
    else
      printf("%i MBytes.\n", i / 1024 / 1024);
  fclose(f);

  return 0;
}

