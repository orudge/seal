/* SEAL installer 2.0 (c) 2000,2001 Kostas Michalopoulos aka Bad Sector */
#include <conio.h>
#include <process.h>
#include <unistd.h>
#include <dir.h>
#include <dos.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include "scrpages.h"
#include "cmdparse.h"

#define CMD_NEXT 10
#define CMD_QUIT 20

char    *title_bar;
char    *line[5];
char    *binaries, *sources;
char    *binfiles, *srcfiles;
char    *success;

/* BAR commands (for UnBARring) */
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

char *path;
char *spath;
p_page first, pitems, ppath, punbar;
int topic, fin;

void unbar_file(FILE *f)
{
  t_barhead hd;
  if (!f) return;
  fread(&hd, 1, sizeof(hd), f);
  #define READ_PARAMETER {                                      \
    unsigned short len;                                         \
    fread(&len, 1, 2, f);                                       \
    parameter = (char *) malloc(len+1);                         \
    fread(parameter, 1, len, f);                                \
    parameter[len] = 0;                                         \
  };
  punbar->item[1].y2 = filelength(fileno(f));
  while (!feof(f)) {
    unsigned char cmd;
    char *parameter;
    fread(&cmd, 1, 1, f);

    if (cmd == CMD_FINISH) {
      printf("Finished.\n");
      break;
    };
    switch (cmd) {
      case CMD_INFO: {    // parameter contains a line of information.
        READ_PARAMETER;   // Current BARs says only information about the
        free(parameter);  // program that was used to make them.
      }; break;
      case CMD_COMMENT: { // parameter contains a line of comments. Each line
        READ_PARAMETER;   // can have sub-lines, separated with the character
        free(parameter);  // 0xD (13 - '\r'). Current BARs doesn't use this.
      }; break;
      case CMD_STATUS: {  // parameter contains the text that should be
        READ_PARAMETER;   // displayed in the status line (if any)
        free(punbar->item[3].data);
        punbar->item[3].data = (void *) strdup(parameter);
        draw_page_item(punbar, 3);
        free(parameter);
      }; break;
      case CMD_CREATEFILE: {
        unsigned char attrib;
        unsigned short time, date;
        unsigned long size, toread, read = 0;
        struct ftime stime;
        FILE *ouf;

        READ_PARAMETER;
        fread(&attrib, 1, 1, f);
        fread(&time, 1, 2, f);
        fread(&date, 1, 2, f);
        fread(&size, 1, 4, f);

        stime.ft_tsec  = (time & 0x1F);
        stime.ft_min   = (time >> 5) & 0x3F;
        stime.ft_hour  = (time >> 11) & 0x1F;
        stime.ft_day   = (date & 0x1F);
        stime.ft_month = (date >> 5) & 0x0F;
        stime.ft_year  = ((date >> 9) & 0x7F);

        free(punbar->item[2].data);
        punbar->item[2].data = malloc(strlen(parameter)+28);
        strcpy(punbar->item[2].data, "Extracting ");
        strcat(punbar->item[2].data, parameter);
        strcat(punbar->item[2].data, "...            ");
        draw_page_item(punbar, 2);
        ouf = fopen(parameter, "wb+");
        while (1) {
          char buff[16384];
          toread = 16384;
          if (read+toread > size) toread = size-read;
          fread(&buff, 1, toread, f);
          fwrite(&buff, 1, toread, ouf);
          read += toread;
          punbar->item[1].index = ftell(f);
          draw_page_item(punbar, 1);
          if (read == size) break;
        };
        fclose(ouf);
        ouf = fopen(parameter, "rb");
        setftime(fileno(ouf), &stime);
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
      }; break;
    };
    punbar->item[1].index = ftell(f);
    draw_page_item(punbar, 1);
  };
}

void install_seal()
{
  FILE *f1, *f2;

  mkdir(path, S_IWUSR);
  mkdir(spath, S_IWUSR);
  f1 = fopen(get_list_item(pitems->item[3].index, binfiles), "rb+");
  f2 = fopen(get_list_item(pitems->item[4].index, srcfiles), "rb+");
  chdir(path);
  unbar_file(f1);
  chdir(spath);
  unbar_file(f2);
  if (f1) fclose(f1);
  if (f2) fclose(f2);

  fin = 1;
  punbar->terminate = 1;

  chdir(path);
}

void help(p_page p)
{
  window(22, 9, 80, 25);
  textcolor(0);
  textbackground(7);
  cprintf("ษอออออออออออออออออออออออออออออออออออป%c%c", 10, 13);
  if (topic == 0) {
    cprintf("บ  If you agree with Using Licence  บ%c%c", 10, 13);
    cprintf("บ  just press Tab and ENTER to      บ%c%c", 10, 13);
    cprintf("บ  continue. To see more text use   บ%c%c", 10, 13);
    cprintf("บ  arrows up and down.              บ%c%c", 10, 13);
  };
  if (topic == 1) {
    cprintf("บ   Press ENTER here to continue    บ%c%c", 10, 13);
    cprintf("บ  with installation process.       บ%c%c", 10, 13);
  };
  if (topic == 2) {
    cprintf("บ   Press ENTER here to cancel the  บ%c%c", 10, 13);
    cprintf("บ  install process and exit to DOS  บ%c%c", 10, 13);
  };
  if (topic == 3) {
    cprintf("บ  Choose one a binary package      บ%c%c", 10, 13);
    cprintf("บ  to be installed (you can choose  บ%c%c", 10, 13);
    cprintf("บ  only one package).               บ%c%c", 10, 13);
  };
  if (topic == 4) {
    cprintf("บ  Choose one a sources package     บ%c%c", 10, 13);
    cprintf("บ  to be installed (you can choose  บ%c%c", 10, 13);
    cprintf("บ  only one package).               บ%c%c", 10, 13);
  };
  if (topic == 5) {
    cprintf("บ  Just choose Yes or No!           บ%c%c", 10, 13);
  };
  if (topic == 6) {
    cprintf("บ  Type here the path where binary  บ%c%c", 10, 13);
    cprintf("บ  files will be installed.         บ%c%c", 10, 13);
  };
  if (topic == 7) {
    cprintf("บ  Type here the path where source  บ%c%c", 10, 13);
    cprintf("บ  files will be installed.         บ%c%c", 10, 13);
  };
  if (topic == 8) {
    cprintf("บ   Press ENTER here to go to the   บ%c%c", 10, 13);
    cprintf("บ  previus screen.                  บ%c%c", 10, 13);
  };
  cprintf("ศอออออออออออออออออออออออออออออออออออผ%c%c", 10, 13);
  window(1, 1, 80, 25);
  while (getch() != 13);
  draw_page(p);
}

void after_keypress(p_page p, int key, int scancode)
{
  if (p == first) topic = 0;
  if (p == pitems) {
    if (p->index == 3) topic = 3;
    if (p->index == 4) topic = 4;
    if (p->index == 5) topic = 1;
    if (p->index == 6) topic = 2;
  };
  if (p == ppath) {
    if (p->index == 2) topic = 6;
    if (p->index == 4) topic = 7;
    if (p->index == 5) topic = 8;
    if (p->index == 6) topic = 1;
    if (p->index == 7) topic = 2;
  };
  if (key == 0 && scancode == 59) help(p);
};

void on_command(p_page p, int cmd)
{
  if (cmd == CMD_NEXT) {
    if (p == first) {
      first->terminate = 1; /* if control returns to "first" do not run it */
      execute_page(pitems);
      return;
    };
    if (p == pitems) {
      execute_page(ppath);
      draw_page(p);
      return;
    };
    if (p == ppath) {
      draw_page(punbar);
      install_seal();
      p->terminate = 1;
      pitems->terminate = 1;
      return;
    };
  };
  if (cmd == CMD_QUIT) {
    if (p == ppath) pitems->terminate = 1;
    p->terminate = 1;
  };
};

char *convert_to_list(char *text)
{
  int i, j = strlen(text);
  text = realloc(text, j+2);
  text[j+1] = 0;
  for (i=0;i<j;i++) if (text[i] == '|') text[i] = 0;

  return text;
}

main(int args, char **argv)
{
  char *licence = load_list_from_file("copying");

  /* load initialization script */
  set_var_value("__Version", "2.0");
  set_var_value("__Copyright", "(c) 2000,2001 Kostas Michalopoulos");
  set_var_value("__HiVer", "2");
  set_var_value("__LoVer", "0");
  parse_file("install.inf");
  title_bar = get_var_value("Title");
  line[0] = get_var_value("Line1");
  line[1] = get_var_value("Line2");
  line[2] = get_var_value("Line3");
  line[3] = get_var_value("Line4");
  line[4] = get_var_value("BottomLine");
  binaries = get_var_value("BinariesList");
  sources = get_var_value("SourcesList");
  binfiles = get_var_value("BinariesFiles");
  srcfiles = get_var_value("SourcesFiles");
  success = get_var_value("SuccessMessage");
  path = get_var_value("DestinationPath");
  spath = get_var_value("DestinationPathForSources");

  path = realloc(path, 61);
  spath = realloc(spath, 61);
  binaries = convert_to_list(binaries);
  sources = convert_to_list(sources);
  binfiles = convert_to_list(binfiles);
  srcfiles = convert_to_list(srcfiles);

  first = (p_page) malloc(sizeof(t_page));
  pitems = (p_page) malloc(sizeof(t_page));
  ppath = (p_page) malloc(sizeof(t_page));
  punbar = (p_page) malloc(sizeof(t_page));

  topic = 0;
  fin = 0;

  /* first page */
  init_page(first, title_bar);
  strcpy(first->status, "F1=Help    TAB & ENTER=Continue");
  first->after_keypress = &after_keypress;
  first->on_command = &on_command;

  add_text(first, 1, 3, line[0]);
  add_text(first, 1, 4, line[1]);
  add_text(first, 1, 5, line[2]);
  add_text(first, 1, 6, line[3]);
  add_list(first, 2, 8, 78, 18, licence);
  add_text(first, 1, 20, line[4]);
  add_button(first, 50, 23, "Press TAB & ENTER to continue", CMD_NEXT);

  /* pitems page */
  init_page(pitems, title_bar);
  strcpy(pitems->status, "F1=Help    Up arrow=Prev item   Down arrow=Next item    ENTER=Select");
  pitems->after_keypress = &after_keypress;
  pitems->on_command = &on_command;

  add_text(pitems, 3, 4, "Select items to install");
  add_text(pitems, 10, 10, "Binaries:");
  add_text(pitems, 10, 12, "Sources:");
  add_drop_list(pitems, 25, 10, 30, binaries);
  add_drop_list(pitems, 25, 12, 30, sources);
  add_button(pitems, 10, 22, "-> Next screen", CMD_NEXT);
  add_button(pitems, 10, 23, "-> Exit to DOS", CMD_EXIT);

  pitems->item[3].index = 0;
  pitems->item[4].index = 0;

  /* ppath page */
  init_page(ppath, title_bar);
  strcpy(ppath->status, "F1=Help    Up arrow=Prev item   Down arrow=Next item    ENTER=Select");
  ppath->after_keypress = &after_keypress;
  ppath->on_command = &on_command;

  add_text(ppath, 3, 4, "Destination paths:");
  add_text(ppath, 10, 10, "Path for binaries:");
  add_input(ppath, 10, 11, 60, path);
  add_text(ppath, 10, 14, "Path for sources:");
  add_input(ppath, 10, 15, 60, spath);
  add_button(ppath, 10, 21, "-> Previus page", CMD_EXIT);
  add_button(ppath, 10, 22, "-> Next screen ", CMD_NEXT);
  add_button(ppath, 10, 23, "-> Exit to DOS ", CMD_QUIT);

  /* punbar page */
  init_page(punbar, title_bar);
  strcpy(punbar->status, "Please wait while installing...");
  add_text(punbar, 3, 4, "Install in progress...");
  add_progress(punbar, 15, 12, 65, 100);
  add_text(punbar, 15, 11, "Copying ...");
  add_text(punbar, 15, 13, "");

  execute_page(first);

  free_page(first);
  free_page(pitems);
  free_page(ppath);
  free_page(punbar);

  textcolor(7);
  textbackground(0);
  clrscr();

  if (fin) {
    printf("%s\n", success);
  };

  free(title_bar);
  free(line[0]);
  free(line[1]);
  free(line[2]);
  free(line[3]);
  free(line[4]);
  free(binaries);
  free(sources);
  free(binfiles);
  free(srcfiles);
  free(success);
  free(path);
  free(spath);

  free_vars();

  return 0;
}

