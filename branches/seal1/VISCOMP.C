#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<pc.h>
#include<process.h>
#include<fcntl.h>
#include<sys/stat.h> /* for mode definitions */

#include <coff.h>
#include "dlx.h"

char exe_header[512] = {0xb9, 0x01, 0x00, 0x8c, 0xc8, 0x8e, 0xc0,
                        0xbe, 0x1f, 0x01, 0xac, 0x3c, 0xe8, 0x74,
                        0x08, 0xb4, 0x02, 0x88, 0xc2, 0xcd, 0x21,
                        0xeb, 0xf3, 0xb4, 0x08, 0x21, 0x46, 0xe2,
                        0xec, 0xcd, 0x20, 0x0d, 0x0a, 0x09, 0x54,
                        0x68, 0x69, 0x73, 0x20, 0x70, 0x72, 0x6f,
                        0x67, 0x72, 0x61, 0x6d, 0x20, 0x72, 0x75,
                        0x6e, 0x20, 0x6f, 0x6e, 0x6c, 0x79, 0x20,
                        0x75, 0x6e, 0x64, 0x65, 0x72, 0x20, 0x53,
                        0x45, 0x41, 0x4C, 0x20, 0x0d, 0x0a, 0xe8};

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



int dlxgen ( int argc, char **argv )
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
  long idposition=-1;
  int  exehead = 0;
  long long prid=  DLX_BUILD_DOS;

  imports.data = NULL;
  imports.length = 0;
  imports.write = &datastor_write;

  if (argc < 3)
  {
    printf("Usage: dlxgen output.dlx input.o [input2.o ... -lgcc -lc ...icons.dat]\n");
    return 1;
  }

  if ( strstr(argv[1], ".exe") ) exehead = 1;

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

  dh.magic = DLX2_MAGIC;
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
/*      printf("bss_start 0x%x\n",bss_start); */
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

        ids.MFID=0;
        ids.PRID=0;
        ids.VTBL=0;
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

  output_f = fopen(argv[1], "wb");
  if (!output_f)
  {
    perror(argv[1]);
    return 1;
  }

  for (i=0; i<sc.s_nreloc; i++)
    if(relocs[i].r_type == 0x14)	/* Don't do these, they are relative */
      dh.numrelocs--;

  if ( exehead ) fwrite(exe_header, 512, 1, output_f);

  fwrite(&dh, 1, sizeof(dh), output_f);
  fwrite(&prid,1,sizeof(long long),output_f);
  fwrite(&ids,1,sizeof(ids),output_f);
  fwrite(imports.data, imports.length, 1, output_f);
  fwrite(data, 1, sc.s_size, output_f);
  for (i=0; i<sc.s_nreloc; i++)
    if(relocs[i].r_type != 0x14)	/* Don't do these, they are relative */
      fwrite(&(relocs[i].r_vaddr), 1, sizeof(long), output_f);

  fclose(output_f);
  return 0;
}




#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h> /* for mode definitions */

#define STDOUT 1
#define STDERR 2

static char *errname = NULL;
static char *outname = NULL;
static int   h_out,h_outbak;
static int   h_err,h_errbak;


char *open_stderr(char *errname)
{
#ifdef O_BINARY
  h_err = open (errname,O_WRONLY | O_BINARY | O_CREAT | O_TRUNC,
                        S_IREAD | S_IWRITE);
#else
  h_err = open (errname,O_WRONLY | O_CREAT | O_TRUNC,
                        S_IREAD | S_IWRITE);
#endif
  h_errbak = dup (STDERR);
  fflush(stderr);  /* so any buffered chars will be written out */
  dup2 (h_err, STDERR);
  close(h_err);
  return errname;
}

void close_stderr(void)
{
  dup2 (h_errbak, STDERR);
  close (h_errbak);
}

char *open_stdout(char *outname)
{
#ifdef O_BINARY
  h_out = open (outname,O_WRONLY | O_BINARY | O_CREAT | O_TRUNC,
                        S_IREAD | S_IWRITE);
#else
  h_out = open (outname,O_WRONLY | O_CREAT | O_TRUNC,
                        S_IREAD | S_IWRITE);
#endif
  h_outbak = dup (STDOUT);
  fflush(stdout);  /* so any buffered chars will be written out */
  dup2 (h_out, STDOUT);
  close(h_out);
  return outname;
}

void close_stdout(void)
{
  dup2 (h_outbak, STDOUT);
  close (h_outbak);
}


l_text ch_ext ( l_text f, l_text next )
{

  l_text v = (l_text)_malloc(250);

  l_text x = v;

  while ( *f ) {

    if ( *f == '.' ) break;

    *x = *f;

    f++;

    x++;

  };

  *x = '.';

  while ( *next ) {

    x++;

    *x = *next;

    next++;

  };

  *(x+1) = '\0';

  return v;
};



p_error_listbox_item  new_error_listbox_item ( l_text buf, l_long line )
{

   p_error_listbox_item o = _malloc(sizeof(t_error_listbox_item));

   p_listbox_item  p = new_listbox_item(buf, NULL, 0, 0);

   if ( o ) clear_type(o, sizeof(t_error_listbox_item));

   if ( o && p ) {

      memcpy(&(o->obclass), p, sizeof(t_listbox_item));

      o->line = line;

      _free(p);
   };

   return o;
};


#define __MS_DOS__

p_list get_error_list_from ( l_text filename )
{


  FILE  *f = fopen(filename, "rt");

  l_char buf[255];
  l_long line = 0;

  p_list list = list_init(_malloc(sizeof(t_list)), &free_error_listbox_item, 0);

  /* not end of file */
  while ( !feof(f) ) {

      l_text p;
      l_text last = NULL;
      l_text w = NULL;

      *buf = '\0';

      /* read 1 line */
      fgets(buf, 255, f);

      w = p = strchr(buf, ':');

      #ifdef __MSDOS__
        if ( p ) p = strchr(p+1, ':');
      #endif

      if ( p ) last = strchr(p+1, ':');

      if ( p && last ) {

          l_text x = stridup(p, strsize(p, last));

          if ( x ) line = atol(x);

          _free(x);

      } else line = -1;

      if ( w ) list->insert(list, new_error_listbox_item(buf, line));

  };

  fclose(f);

  return list;

};


/* compile file and return the result */

l_text compile ( l_text file, l_text libs, p_list *o )
{

  l_int ret = 0;
  l_int ok = 0;
  l_text ofile = NULL;
  l_text cfile = NULL;
  l_text exefile = NULL;
  l_text ext   = "c";
  l_text curdir = NULL;
  l_text newdir = NULL;
  l_text i = NULL;
  l_text txt = NULL;
  l_text errfile = NULL;
  l_text outfile = NULL;

  if ( o ) *o = NULL;

  if ( !file ) return NULL;

  if ( io_isas(file, "cc") ) ext = "cc";

  ofile = ch_ext(file, "o");
  cfile = ch_ext(file, ext);
  exefile = ch_ext(file, "exe");

  errfile = ch_ext(file, "err");
  outfile = ch_ext(file, "out");

  newdir = io_sealpath(cfile);

  open_stderr(errfile);

  do {

     l_text args[] = {
       "gcc",
       "-g",
       "-c",
       cfile,
       "-o",
       ofile,
       0
     };


     curdir = getcwd(NULL, IO_DIR_LIMIT);

     chdir(newdir);

     _io_removefile(ofile);
     _io_removefile(exefile);

     i = show_info(400, MW_INFO, "Compiling file %s to %s", cfile, ofile);

     ok = spawnvp(P_WAIT, "gcc", args);

     hide_info(i);

     if ( ok == -1 )

        seal_error(ERR_INFO, "%CGCC not found");

     else {

         char *args3[] = {
              "dlxgen",
              exefile,
              ofile,
              libs,
              0
         };

         i = show_info(400, MW_INFO, "Making Seal EXE file from %s to %s", ofile, exefile);

         if ( dlxgen(3, args3) != -1 )  ret = 1;

         hide_info(i);

     };


     if ( o ) *o = get_error_list_from(errfile);


     chdir(curdir);

  } while (0);

  close_stderr();

  _free(curdir);
  _free(ofile);
  _free(cfile);
  _free(errfile);

  if ( ret ) return exefile;

  _free(exefile);

  return NULL;

};


