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

/* Bad Seal Registry Editor for DOS */
/* (C) 2001 Kostas Michalopoulos aka Bad Sector */

#include <conio.h>
#include "registry.c"

char curr[1024];
char cparr[1024];
char (list[256])[1024];
char (value[256])[1024];
char (full[1024])[1024];
int ivalue[1024];
int type[1024];
int lc;
int sel, prg;
int ch;


void win3d_ex(int x1, int y1, int x2, int y2, int c1, int c2, int b)
{
  int i;

  window(x1, y1, x2, y2);
  textcolor(0);
  textbackground(b);
  clrscr();
  window(1, 1, 80, 25);
  gotoxy(x1, y1);
  textcolor(c1);
  cprintf("Ú");
  for (i=x1+1;i<x2;i++) cprintf("Ä");
  textcolor(c2);
  cprintf("¿");
  gotoxy(x1, y2);
  textcolor(c1);
  cprintf("À");
  textcolor(c2);
  for (i=x1+1;i<x2;i++) cprintf("Ä");
  cprintf("Ù");
  for (i=y1+1;i<y2;i++) {
    gotoxy(x1, i);textcolor(c1);cprintf("³");
    gotoxy(x2, i);textcolor(c2);cprintf("³");
  };
}

void win3d(int x1, int y1, int x2, int y2)
{
  win3d_ex(x1, y1, x2, y2, 15, 8, 7);
}

void draw_main()
{
  textcolor(14);
  textbackground(1);
  clrscr();
  gotoxy(1, 25);
  textbackground(4);clreol();cprintf(" Enter-Open key  F4-Edit value  F7-New key  F8-Delete key  F9-File    ESC-Exit");
  win3d(1, 1, 80, 3);
  win3d(1, 21, 80, 24);
  textcolor(0);
  gotoxy(3, 2);cprintf("Registry editor 1.0 (by BadSector) for Seal Registry 1.0");
  win3d_ex(1, 5, 30, 20, 0, 9, 1);
  win3d_ex(31, 5, 80, 20, 0, 9, 1);
  textcolor(14);
  gotoxy(2, 4);cprintf("Key name");
  gotoxy(32, 4);cprintf("Key value");
}

void load_path(char *path)
{
  t_registry_search p;
  char *own;
  int i;

  path = (char *) strdup(path);

  lc = 0;
  if (strcmp(path, "")) {

    lc = 1;
    strcpy(list[0], "[] Open parent");
    strcpy(value[0], " ");
    type[lc] = KEY_TEXT;
  };
  if (reg_find_first(path, &p)) do {
    char *s = get_key(p.name);
    strcpy(list[lc], p.key->name);
    if ( s ) strcpy(value[lc], s);
    strcpy(full[lc], p.name);
    type[lc] = p.key->type;
    if (type[lc] == KEY_INTEGER) {
      ivalue[lc] = get_key_integer(p.name);
      itoa(ivalue[lc], value[lc], 10);
    };
    free(s);
    //free(p->name);
    lc++;
    //if (p.parent_name) strcpy(cparr, p.parent_name);
  } while (reg_find_next(&p));

  strcpy(curr, path);

  own = key_owner_full_name(path);
  if ( own ) {
	strcpy(cparr, own);
	free(own);
  } else
  /*i = get_path_pos(cparr, 1);
  if (i > -1) {
    char *parent;
    rec = get_rec(i);
    if (rec.parent > -1) {
      parent = get_path_from_pos(rec.parent);
      strcpy(cparr, parent);
      free(parent);
    } else*/ strcpy(cparr, "");
  //};

  free(path);

/*  i = get_path_pos(path, 1);
  if (i > -1) {
    rec = get_rec(i);
    strcpy(curr, rec.name);
  } else */
}

void draw_path()
{
  int c = 0, i;

  if (lc > 0) do {
    char *s = (char *) malloc(50);
    if (type[c+prg] == 0) strcpy(s, "(no data)"); else
    if (type[c+prg] == KEY_TEXT) strncpy(s, value[c+prg], 46); else
    if (type[c+prg] == KEY_INTEGER) itoa(ivalue[c+prg], s, 10); else
    if (type[c+prg] == KEY_DATA) strcpy(s, "(data key)"); else
    if (type[c+prg] == KEY_LINK) strcpy(s, "(link)");
    gotoxy(2, 6+c);
    if (sel == c+1) {
      textcolor(15);textbackground(4);
    } else {
      textcolor(7);textbackground(1);
    };
    cprintf(" %s", list[c+prg]);
    for (i=strlen(list[c+prg]);i<27;i++) cprintf(" ");
    gotoxy(32, 6+c);
    cprintf(" %s", s);
    for (i=strlen(s);i<47;i++) cprintf(" ");
    free(s);
    c++;
  } while (c != 14 && c+prg != lc);
}

int main(int argc, char **argv)
{
  registry_init();
  if ( argc == 1 ) {
  _setcursortype(0);
  draw_main();
  sel = 1;
  prg = 0;
  load_path("");
  draw_path();
  do {
    int key;
    textcolor(0);
    textbackground(7);
    window(2, 22, 79, 23);clrscr();
    cprintf(" Key:");
    cprintf(" %s", curr);
    window(1, 1, 80, 25);
    key = bioskey(0);
    ch = key & 255;
    switch (key >> 8) {
      case 80: if (sel+prg < lc) {
        if (sel < 14) sel++; else prg++;
        draw_path();
      } break;
      case 72: if (sel+prg > 1) {
        if (sel > 1) sel--; else prg--;
        draw_path();
      } break;
      case 62: if (!(strcmp(curr, "") && sel+prg == 1)) {
        char val[256];
        win3d(1, 8, 80, 17);
        textcolor(0);
        gotoxy(3, 9);printf("Old key value:");
        window(3, 10, 78, 12);
        printf("%s", (value)[sel+prg-1]);
        if (type[sel+prg-1] != KEY_TEXT)
        if (type[sel+prg-1] == KEY_INTEGER) printf(" (integer)");
        else printf(" (error! cannot edit key of that type)");
        window(1, 1, 80, 25);
        gotoxy(3, 13);printf("New key value:");
        window(3, 14, 78, 16);
        textcolor(14);textbackground(1);clrscr();
        window(1, 1, 80, 25);
        gotoxy(3, 14);
        _setcursortype(2);
        gets(val);
        _setcursortype(0);
        if (type[sel+prg-1] == KEY_INTEGER || type[sel+prg-1] == KEY_TEXT) {
          if (type[sel+prg-1] == KEY_INTEGER) set_key_integer(full[sel+prg-1], atoi(val));
          else set_key(full[sel+prg-1], val);
        };
        win3d_ex(1, 5, 30, 20, 0, 9, 1);
        win3d_ex(31, 5, 80, 20, 0, 9, 1);
        load_path(curr);
        draw_path();
      } break;
      case 65: if (strcmp(curr, "")) {
        char val[256];
        int ntype;
        win3d(1, 8, 80, 12);
        textcolor(0);
        gotoxy(3, 9);printf("New key's name (nothing = cancel):");
        gotoxy(3, 11);printf("(S)tring or (I)integer ?");
        textcolor(14);textbackground(1);
        window(3, 10, 78, 10);
        clrscr();
        window(1, 1, 80, 25);
        gotoxy(3, 10);
        _setcursortype(2);
        gets(val);
        _setcursortype(0);
        if (strcmp(val, "")) {
          char *newkey = key_in_path(curr, val);
          gotoxy(27, 11);
          do {
            ntype = getch();
          } while (ntype != 's' && ntype != 'i' && ntype != 'S' && ntype != 'I');
          create_key(curr, val);
          if (ntype == 's' || ntype == 'S') set_key(newkey, "unassigned value"); else set_key_integer(newkey, 0);
          free(newkey);
        };
        win3d_ex(1, 5, 30, 20, 0, 9, 1);
        win3d_ex(31, 5, 80, 20, 0, 9, 1);
        load_path(curr);
        draw_path();
      } break;
      case 66: if (strcmp(curr, "") && (!(strcmp(curr, "") && sel+prg == 1))) {
        int sl;
        win3d(1, 10, 80, 12);
        textcolor(0);
        gotoxy(3, 11);printf("Do you want to delete the selected key? (Y)es/(N)o ");
        do {
          sl = getch();
        } while (sl != 'y' && sl != 'n' && sl != 'Y' && sl != 'N');
        if (sl == 'y' || sl == 'Y') {
          delete_key(full[sel+prg-1]);
        };
        sel = 1;
        prg = 0;
        win3d_ex(1, 5, 30, 20, 0, 9, 1);
        win3d_ex(31, 5, 80, 20, 0, 9, 1);
        load_path(curr);
        draw_path();
      } break;
      case 67: {
        char file[256];
        int ntype;
        win3d(1, 8, 80, 11);
        textcolor(0);
        gotoxy(3, 9);printf("File name to integrate : ");
        textcolor(14);textbackground(1);
        window(3, 10, 78, 10);
        clrscr();
        window(1, 1, 80, 25);
        gotoxy(3, 10);
        _setcursortype(2);
        gets(file);
        _setcursortype(0);

        if (strcmp(file, "")) {
	      window(1, 1, 80, 25);
          clrscr();
          read_reg_file(file,1);
          printf("\n - Finished - Press any key -");
          getch();
        };
        draw_main();
        win3d_ex(1, 5, 30, 20, 0, 9, 1);
        win3d_ex(31, 5, 80, 20, 0, 9, 1);
        load_path(curr);
        draw_path();
      } break;
    };
    if (ch == 13) {
      win3d_ex(1, 5, 30, 20, 0, 9, 1);
      win3d_ex(31, 5, 80, 20, 0, 9, 1);
      if (strcmp(curr, "") && sel+prg == 1) {
        load_path(cparr);
//        pcurr--;
      } else {
//        pcurr++;
//        strcpy(parr[pcurr], cparr);
        if (strcmp(curr, "")) load_path(key_in_path(curr, list[sel+prg-1])); else load_path(list[sel+prg-1]);
      };
      sel = 1;
      prg = 0;
      draw_path("");
    };
    draw_path();
  } while (ch != 27);
  textcolor(7);
  textbackground(0);
  clrscr();
  //registry_map();
  _setcursortype(2);
  } else {
	read_reg_file(argv[1],1);
  };
  registry_done();
  return 0;
}

