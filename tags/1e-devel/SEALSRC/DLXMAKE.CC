/*
   DLX Generator.  Based heavily on the DXE generator.
*/
#define DEF_START "    LIBEXPORT("
#define DEF_END   ")"
#define DEF_CSTART "    LIBEXPORTCLASS("
#define DEF_CEND   ")"
#define DEF_USEBEGIN "DLXUSE_BEGIN"
#define DEF_EXPBEGIN "  LIBEXPORT_BEGIN"
#define DEF_EXPEND   "  LIBEXPORT_END"
#define DEF_USEEND   "DLXUSE_END"
#define DEF_LIBBEGIN "  LIBLOADS_BEGIN"
#define DEF_LIBEND   "  LIBLOADS_END"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <coff.h>
#include <pc.h>
#include "dlx.h"
void exit_cleanup(void)
{
  remove("dlx__tmp.o");
}

#define GLOBALMAX 9

char *GLOBALSYM[GLOBALMAX] = {
  "_LibMain",
  "__LIBEXPORTTABLE",
  "__LIBTOLOAD",
  "dlx_last_dtor",
  "dlx_first_dtor",
  "dlx_first_ctor",
  "dlx_last_ctor",
  "_main",
  "__DLXVERSIONTABL"
};

int ThisIsGLOBAL ( char *name ) {
  int i = 0;
  while ( i < GLOBALMAX ) {
    if ( !strcmp(name, GLOBALSYM[i]) ) return 1;
    i++;
  };
  return 0;
};

char *strrstr ( char *p1, char *p2 )
{
  char *v = NULL;
  while ( p1 ) {
   p1 = strstr(p1, p2);
   if ( p1 ) {
     v = p1;
     p1++;
   };
  };
  return v;
};


int main(int argc, char **argv)
{
  int errors = 0;
  int write_external = 0;
  unsigned bss_start = 0;
  FILHDR fh;
  FILE *input_f, *output_f;
  SCNHDR sc;
  char *data, *strings;
  SYMENT *sym;
  RELOC *relocs;
  int strsz, i;
  dlxheader dh;
  dlxiddesc ids;
  long idposition=-1;
  if (argc < 4)
  {
    printf("Usage: dlxmake -[command] out.exp input.o [input2.o ... -lgcc -lc]\n");
    printf("command: x  'to out.exp write all function which were in input.o... exported' \n");
    printf("command: i  'to out.exp write all function which were in input.o... imported' \n");
    return 1;
  }
  output_f = fopen(argv[2], "w+");
  write_external = strcmp(argv[1],"-x");
  input_f = fopen(argv[3], "rb");
  if (!input_f)
  {
    perror(argv[3]);
    return 1;
  }

  fread(&fh, 1, FILHSZ, input_f);
  if (fh.f_nscns != 1 || argc > 4)
  {
    char command[1024];
    fclose(input_f);

    strcpy(command,"ld -X -S -r -o dlx__tmp.o -L");
    strcat(command,getenv("DJDIR"));
    strcat(command,"/lib ");
    for(i=3;argv[i];i++) {
      strcat(command,argv[i]);
      strcat(command," ");
    }
    strcat(command," -T dlx.ld ");
      
    printf("%s\n",command);
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
//      printf("Error: input file has more than one section; use -M for map\n");
//      return 1;
    }
  }

  fseek(input_f, fh.f_opthdr, 1);
  fread(&sc, 1, SCNHSZ, input_f);

  dh.magic = DLX2_MAGIC;
  dh.libmainpos = -1;
  dh.extablepos = -1;
  dh.libloadpos = -1;
  dh.prgsize = sc.s_size;
  dh.numrelocs = sc.s_nreloc;
  dh.numimports = 0;

  sym = (SYMENT *)malloc(sizeof(SYMENT)*fh.f_nsyms);
  fseek(input_f, fh.f_symptr, 0);
  fread(sym, fh.f_nsyms, SYMESZ, input_f);
  fread(&strsz, 1, 4, input_f);
  strings = (char *)malloc(strsz);
  fread(strings+4, 1, strsz-4, input_f);
  strings[0] = 0;

  if (!output_f) {
      perror("dlx__tmp.o");
      return 1;
  };

  fprintf(output_f, "%s\n%s\n%s\n%s\n", DEF_USEBEGIN, DEF_LIBBEGIN, DEF_LIBEND,
                                        DEF_EXPBEGIN);

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

    if ( write_external ) {
      if (sym[i].e_scnum == 0) { // it's external function
        char *k, *l;
        k=strrstr(&name[1], "__");
        l=strrchr(&name[1], '$');
        if ( k && k[2] >= '0' && k[2] <= '9' )
          fprintf(output_f,"%s%s%s \n",DEF_CSTART,&name[1],DEF_CEND);
        else
        if ( l && l[1] >= '0' && l[1] <= '9' )
          fprintf(output_f,"%s%s%s \n",DEF_CSTART,&name[1],DEF_CEND);
        else
        fprintf(output_f,"%s%s%s \n",DEF_START,&name[1],DEF_END);
      };
    } else {
      if (sym[i].e_scnum != 0 && sym[i].e_sclass == 2  ) { // it's internal function
        if ( !ThisIsGLOBAL(name) ) {
          char *k=strrstr(&name[1], "__");
          char *l=strrchr(&name[1], '$');
            if ( k && k[2] >= '0' && k[2] <= '9' )
              fprintf(output_f,"%s%s%s \n",DEF_CSTART,&name[1],DEF_CEND);
            else
            if ( l && l[1] >= '0' && l[1] <= '9' )
              fprintf(output_f,"%s%s%s \n",DEF_CSTART,&name[1],DEF_CEND);
            else
            fprintf(output_f,"%s%s%s \n",DEF_START,&name[1],DEF_END);
        };
      };
    };
    i += sym[i].e_numaux;
  }
  fprintf(output_f, "%s\n%s\n", DEF_EXPEND, DEF_USEEND);
  fclose(input_f);
  return 0;
}

