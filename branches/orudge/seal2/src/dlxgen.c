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

/*
   DLX Generator.  Based heavily on the DXE generator.
*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <coff.h>

#define NO_LIST_SUPPORT_FOR_ME

#include "dlx.h"
#include"zlib.h"


void exit_cleanup(void)
{
  remove("dlx__tmp.o");
}


typedef struct dlxheader dlxheader;
typedef struct dlxiddesc dlxiddesc;
typedef struct datastor *p_datastor;
typedef struct datastor {

        void* data;
        long length;

        void (*write)(p_datastor o, void* d2, long l2);

} datastor;

        void datastor_write(p_datastor o, void* d2, long l2)
        {
                o->data=realloc(o->data, o->length+l2);
                memcpy(o->data+o->length, d2, l2);
                o->length+=l2;
        }



int main(int argc, char **argv)
{
  int errors = 0;
  unsigned bss_start = 0;
  FILHDR fh;
  FILE *input_f, *output_f;
  SCNHDR sc;
  char *data, *strings;
  SYMENT *sym;
  RELOC *relocs;
  int strsz, i;
  dlxheader dh;
  datastor imports;
  dlxiddesc ids;
  char compressed = 0;
  long idposition=-1;
  unsigned int filesize = 0;
  long long prid=  DLX_BUILD_DOS;

  t_xdlinfo info;


  imports.data = 0;
  imports.length = 0;
  imports.write = &datastor_write;

  if (argc < 3)
  {
    printf("XDL Generator - Version 1.1 - Support XDL and CXDL
  dlxgen [-C]output.xdl input.o [input2.o ... -lgcc -lc ...icons.dat]
    [-C]        : Activate Compression
    output.xdl  : Output file
    [ ... ]     : See LD help\n");
    return 1;
  }



  input_f = fopen(argv[2], "rb");
  if (!input_f)
  {
    perror(argv[2]);
    return 1;
  }

  fread(&fh, 1, FILHSZ, input_f);
  if (fh.f_nscns != 1 || argc > 3)
  {
    char command[1024];
    fclose(input_f);

    strcpy(command,"ld -X -S -r -o dlx__tmp.o -L");
    strcat(command,getenv("DJDIR"));
    strcat(command,"/lib ");
    for(i=2;argv[i];i++) {
      strcat(command,argv[i]);
      strcat(command," ");
    }
    strcat(command," -T dlx.ld ");

    // printf("%s\n",command);
    i = system(command);
    if(i)
      return i;

    input_f = fopen("dlx__tmp.o", "rb");
    if (!input_f)
    {
      perror("dlx__tmp.o");
      return 1;
    } else
      atexit(exit_cleanup);

    fread(&fh, 1, FILHSZ, input_f);
    if (fh.f_nscns != 1) {
      printf("Error: input file has more than one section; use -M for map\n");
      return 1;
    }
  }

  fseek(input_f, fh.f_opthdr, 1);
  fread(&sc, 1, SCNHSZ, input_f);

  info.need_seal_version = 200010;
  info.version = time(NULL);
  info.appname = -1;
  info.description = -1;
  info.copyright = -1;
  info.manufacturer = -1;


  dh.magic = XDL_MAGIC;
  dh.libmainpos = -1;
  dh.extablepos = -1;
  dh.libloadpos = -1;
  dh.prgsize = sc.s_size;
  dh.numrelocs = sc.s_nreloc;
  dh.numimports = 0;

  data = (char *)malloc(sc.s_size);
  fseek(input_f, sc.s_scnptr, 0);
  fread(data, 1, sc.s_size, input_f);

  sym = (SYMENT *)malloc(sizeof(SYMENT)*fh.f_nsyms);
  fseek(input_f, fh.f_symptr, 0);
  fread(sym, fh.f_nsyms, SYMESZ, input_f);
  fread(&strsz, 1, 4, input_f);
  strings = (char *)malloc(strsz);
  fread(strings+4, 1, strsz-4, input_f);
  strings[0] = 0;

  relocs = (RELOC *)malloc(sizeof(RELOC)*sc.s_nreloc);
  fseek(input_f, sc.s_relptr, 0);
  fread(relocs, sc.s_nreloc, RELSZ, input_f);

  for (i=0; i<fh.f_nsyms; i++)
  {
    char tmp[9], *name;
    if (sym[i].e.e.e_zeroes)
    {
      memcpy(tmp, sym[i].e.e_name, 8);
      tmp[8] = 0;
      name = tmp;
    }
    else
      name = strings + sym[i].e.e.e_offset;
#if 0
    printf("[%3d] 0x%08x 0x%04x 0x%04x %d %s\n",
	   i,
           sym[i].e_value,
	   sym[i].e_scnum & 0xffff,
	   sym[i].e_sclass,
	   sym[i].e_numaux,
	   name
	   );
#endif
    if (sym[i].e_scnum == 0) // fix me 2
    {
      //printf("Error: object contains unresolved external symbols (%s)\n", name);
      //errors ++;
      long numiuse=0;
      long sl=strlen(name)+1;
      long i2=0;
      imports.write(&imports, &sl,4);
      imports.write(&imports, name,sl);
      dh.numimports++;
      for (; i2<sc.s_nreloc; i2++)
       if(relocs[i2].r_symndx==i) {
                numiuse++;
      }
      imports.write(&imports, &numiuse,4);
      for (i2=0; i2<sc.s_nreloc; i2++)
        if(relocs[i2].r_symndx==i)
              imports.write(&imports, &relocs[i2].r_vaddr,4);
    } else if (strncmp(name, "_LibMain", 8) == 0)
    {
      if (dh.libmainpos != -1)
      {
        printf("Error: multiple definitions of _LibMain!\n");
	errors++;
      }
      dh.libmainpos = sym[i].e_value;
    } else if (strncmp(name, "__LIBTOLOAD", 12) == 0) {
      if (dh.libloadpos != -1)
      {
        printf("Error: multiple definitions of library load table!\n");
	errors++;
      }
      dh.libloadpos = sym[i].e_value;
    } else if (strncmp(name, "_InfoAppName", 13) == 0) {

      info.appname = sym[i].e_value;

    } else if (strncmp(name, "_InfoDesciption", 16) == 0) {

      info.description = sym[i].e_value;

    } else if (strncmp(name, "_InfoCopyright", 15) == 0) {

      info.copyright = sym[i].e_value;

    } else if (strncmp(name, "_InfoManufacturer", 18) == 0) {

      info.manufacturer = sym[i].e_value;

    } else if (strncmp(name, "__LIBEXPORTTABLE", 16) == 0) {
      if (dh.extablepos != -1)
      {
        printf("Error: multiple definitions of library load table!\n");
	errors++;
      }
      dh.extablepos = sym[i].e_value;
    } else if (strncmp(name, "__DLXVERSIONTABL", 16) == 0) {
      if (idposition != -1)
      {
        printf("Error: multiple definitions of version table!\n");
	errors++;
      }
      idposition = sym[i].e_value;
    }else if (strcmp(name, ".bss") == 0 && !bss_start) {
      bss_start = sym[i].e_value;
//      printf("bss_start 0x%x\n",bss_start);
      memset(data+bss_start, 0, sc.s_size - bss_start);
    }
    i += sym[i].e_numaux;
  }

  if (dh.extablepos == -1)
  {
    printf("Error: export table not found!\n");
    errors++;
  }
  if (dh.libmainpos == -1)
  {
    printf("Error: _LibMain not found!\n");
    errors++;
  }
  if (dh.libloadpos == -1)
  {
    printf("Error: library table not found!\n");
    errors++;
  }
  {
        long long val;
        long long UID;
        memset(&ids,0,sizeof(ids));
        ids.MFID=10;
        ids.PRID=110;
        ids.VTBL=1110;

        if(idposition!=-1)
        {
                ids.MFID=((long long*)(data+idposition))[0];
                ids.PRID=((long long*)(data+idposition))[1];
                ids.VTBL=((long long*)(data+idposition))[2];
        }
        UID=uclock();srandom( time(NULL) );UID=random();
        UID*=time(NULL);UID+=uclock();UID+=val;UID*=uclock();
        UID*=uclock();UID*=random();
        ids.UNID=UID+ids.VTBL+ids.PRID+ids.MFID;
        if(idposition!=-1)
        {
                ((long long*)(data+idposition))[3]=ids.UNID;
        }
  }
#if 0
  for (i=0; i<sc.s_nreloc; i++)
    printf("0x%08x %3d 0x%04x - 0x%08x\n",
	   relocs[i].r_vaddr,
	   relocs[i].r_symndx,
	   relocs[i].r_type,
	   *(long *)(data + relocs[i].r_vaddr)
	   );
#endif

  fclose(input_f);
  if (errors)
    return errors;

  if ( argv[1][0] == '-' && ( argv[1][1] =='C'||argv[1][1] =='c') ) compressed = 1;

  if ( compressed )
    output_f = fopen(argv[1]+2, "wb");
  else
    output_f = fopen(argv[1], "wb");

  if (!output_f)
  {
    perror(argv[1]);
    return 1;
  }

  for (i=0; i<sc.s_nreloc; i++)
    if(relocs[i].r_type == 0x14)	// Don't do these, they are relative *
      dh.numrelocs--;

  if ( compressed ) dh.magic = CXDL_MAGIC;

  fwrite(&dh, 1, sizeof(dh), output_f);
  fwrite(&prid,1,sizeof(long long),output_f);
  fwrite(&ids,1,sizeof(ids),output_f);

  fwrite(&info,1,sizeof(t_xdlinfo),output_f);

  fwrite(imports.data, imports.length, 1, output_f);

  filesize = sizeof(dh)+sizeof(long long)+sizeof(ids)+sizeof(t_xdlinfo)+imports.length+(dh.numrelocs*sizeof(long));

  if ( compressed ) {
    uLong compsize = sc.s_size+16+(sc.s_size/100);
    void *comp = malloc(compsize); // Must be 1% +16 bits larger than original data
    if ( compress((Bytef*)comp,&compsize,(Bytef*)data,sc.s_size) == Z_OK ) {
      fwrite(&compsize, 1, sizeof(uLong), output_f);
      fwrite(comp, 1, compsize, output_f);
      printf("  Compression   : %d%c\n  Old file size : %d Kb\n",(filesize+compsize+sizeof(uLong))*100/(filesize+sc.s_size),'%',((filesize+sc.s_size)/1024));
      filesize += compsize;
    } else {
      printf("  Compression : ERROR\n - STOPED - \n");
      exit(1);
    };
  } else {
    fwrite(data, 1, sc.s_size, output_f);
    filesize += sc.s_size;
  };

  for (i=0; i<sc.s_nreloc; i++)
    if(relocs[i].r_type != 0x14)	// Don't do these, they are relative
      fwrite(&(relocs[i].r_vaddr), 1, sizeof(long), output_f);

  fclose(output_f);

  printf("  File size     : %d Kb%s\n",(filesize/1024), ( ( filesize > 1024*50 ) && !compressed ) ? " (You should use compression)":"");
  return 0;
}



