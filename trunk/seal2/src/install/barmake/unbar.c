/* Bad ARchive Unarchiver */

#include <stdio.h>
#include <dir.h>
#include <sys/stat.h>

#define CMD_INFO        0
#define CMD_COMMENT     1
#define CMD_STATUS      2
#define CMD_CREATEFILE  100
#define CMD_CREATEDIR   101
#define CMD_CLOSEDIR    102
#define CMD_FINISH      255

typedef struct t_barhead
{
  unsigned char ID[3];
  unsigned char version;
} t_barhead;

FILE            *f = NULL;
t_barhead       hd;

main()
{
  printf("UNBAR version 1.0 (C) 2001 Kostas Michalopoulos aka Bad Sector\n");
  f = fopen("output.bar", "rb+");
  if (fread(&hd, 1, sizeof(hd), f) != sizeof(hd)) {
    printf("Error opening output.bar\n");

    return 1;
  };

  #define READ_PARAMETER {                                      \
    unsigned short len;                                         \
    fread(&len, 1, 2, f);                                       \
    parameter = (char *) malloc(len+1);                         \
    fread(parameter, 1, len, f);                                \
    parameter[len] = 0;                                         \
  };

  mkdir("output", S_IWUSR);
  chdir("output");
  while (!feof(f)) {
    unsigned char cmd;
    char *parameter;
    fread(&cmd, 1, 1, f);

    if (cmd == CMD_FINISH) {
      printf("Finished.\n");
      break;
    };
    switch (cmd) {
      case CMD_INFO: {
        READ_PARAMETER;
        printf("INFO: %s\n", parameter);
        free(parameter);
      }; break;
      case CMD_COMMENT: {
        READ_PARAMETER;
        printf("-: %s\n", parameter);
        free(parameter);
      }; break;
      case CMD_STATUS: {
        READ_PARAMETER;
        printf("STATUS: %s\n", parameter);
        free(parameter);
      }; break;
      case CMD_CREATEFILE: {
        unsigned char attrib;
        unsigned short time, date;
        unsigned long size, toread, read = 0;
        FILE *ouf;

        READ_PARAMETER;
        fread(&attrib, 1, 1, f);
        fread(&time, 1, 2, f);
        fread(&date, 1, 2, f);
        fread(&size, 1, 4, f);

        ouf = fopen(parameter, "wb+");
        while (1) {
          char buff[16384];
          toread = 16384;
          if (read+toread > size) toread = size-read;
          fread(&buff, 1, toread, f);
          fwrite(&buff, 1, toread, ouf);
          read += toread;
          if (read == size) break;
        };
        fclose(ouf);

        free(parameter);
      }; break;
      case CMD_CREATEDIR: {
        READ_PARAMETER;
        mkdir(parameter, S_IWUSR);
        chdir(parameter);
        free(parameter);
      }; break;
      case CMD_CLOSEDIR: {
        chdir("..");
      }; break;
      default: {
        printf("WARNING: Unknown command in the file: %i\n", cmd);
      }; break;
    };
  };
  chdir("..");

  fclose(f);
  return 0;
}

