/* Screen pages 1.1 (c) 2000,2001 Kostas Michalopoulos aka Bad Sector */
#include <conio.h>
#include <stdio.h>
#include "scrpages.h"

void init_page(p_page p, const char *title)
{
  strcpy(p->status, "Use arrows to move ³ ESC to exit");
  p->items = 0;
  p->index = 0;
  p->__draw_item = -1;
  p->before_keypress = 0;
  p->after_keypress = 0;
  p->on_command = 0;
  title = (char *) strdup(title);
  if (title) strcpy(p->title, title); else strcpy(p->title, "");
};

char *get_list_item(int idx, char *list)
{
  char *s = (char *) malloc(256);
  char ss[2];
  int i, id = 0;

  ss[1] = 0;

  strcpy(s, "");
  i = 0;
  while (1)
  {
    if (list[i] == 0) {
      if (strcmp(s, "") == 0) {
        break;
      } else {
        if (idx == id) return s;
        id++;
        strcpy(s, "");
      };
    } else {
      ss[0] = list[i];
      if (strlen(s) < 255) strcat(s, ss);
    };
    i++;
  };

  return s;
};

char *get_next_item(int *pos, char *list)
{
  char *s = (char *) malloc(256);
  char ss[2];
  int id = 0, i;

  ss[1] = 0;

  strcpy(s, "");
  i = *pos;
  while (1)
  {
    if (list[i] == 0) {
      break;
    } else {
      ss[0] = list[i];
      if (strlen(s) < 255) strcat(s, ss);
    };
    i++;
  };

  (*pos) = i+1;

  return s;
};

int get_list_items(char *list)
{
  char s[2];
  int cnt = 0, i;

  strcpy(s, "");
  i = 0;
  while (1)
  {
    if (list[i] == 0) {
      if (strcmp(s, "") == 0) {
        break;
      } else {
        cnt++;
        strcpy(s, "");
      };
    } else {
      strcpy(s, " ");
    };
    i++;
  };

  return cnt;
};

void draw_page_items(p_page p)
{
  int i, j, j1, ilen, si = 0, ei = p->items;

  if (p->__draw_item > -1) {
    si = p->__draw_item;
    ei = si+1;
    p->__draw_item = -1;
  };
  for (i=si;i<ei;i++)
  {
    gotoxy(p->item[i].x1, p->item[i].y1);
    textcolor(14); textbackground(1);
    switch (p->item[i].type) {
      case PI_TEXT: {
        cprintf("%s", (char *) p->item[i].data);
      } break;
      case PI_BUTTON: {
        if (i == p->index) {
          textcolor(0);
          textbackground(7);
        } else textcolor(15);
        cprintf("%s", (char *) p->item[i].data);
      } break;
      case PI_CHECKLIST:
      case PI_LIST: {
        if (p->item[i].y2 == 0) { /* drop down list */
          if (i == p->index) {
            textcolor(0);
            textbackground(7);
          } else textcolor(7);
          cprintf("%s", get_list_item(p->item[i].index, p->item[i].data));
          ilen = strlen(get_list_item(p->item[i].index, p->item[i].data));
          if ( ilen < p->item[i].x2)
          for (j=0;j<p->item[i].x2-ilen;j++) cprintf(" ");
        } else { /* list box */
          int items = get_list_items(p->item[i].data);
          int pos = 0;
          gotoxy(p->item[i].x1, p->item[i].y1);
          textcolor(0); textbackground(1);
          cprintf("Ú"); for (j=p->item[i].x1+1;j<p->item[i].x2;j++) {gotoxy(j, p->item[i].y1); cprintf("Ä");};
          textcolor(9); textbackground(1); cprintf("¿");
          gotoxy(p->item[i].x1, p->item[i].y2);
          textcolor(0); textbackground(1);
          cprintf("À");
          textcolor(9); textbackground(1);
          for (j=p->item[i].x1+1;j<p->item[i].x2;j++) {gotoxy(j, p->item[i].y2); cprintf("Ä");};
          cprintf("Ù");
          for (j=p->item[i].y1+1;j<p->item[i].y2;j++) {
            textcolor(0); textbackground(1);
            gotoxy(p->item[i].x1, j);cprintf("³");
            textcolor(9); textbackground(1);
            gotoxy(p->item[i].x2, j);cprintf("³");
          };
          for (j=0;j<p->item[i].scroll;j++) get_next_item(&pos, p->item[i].data);
          for (j=0;j<items;j++) {
            char *s = get_next_item(&pos, p->item[i].data);
            if (j < p->item[i].y2-p->item[i].y1-1) {
              textbackground(1);
              if (j == p->item[i].idx) {
                textcolor(15);
                if (i == p->index) textbackground(4);
              } else textcolor(7);
              gotoxy(p->item[i].x1+1, j+p->item[i].y1+1);
              cprintf("%s", s);
              ilen = strlen(s);
              if (ilen < p->item[i].x2-p->item[i].x1-2)
              for (j1=0;j1<p->item[i].x2-p->item[i].x1-1-ilen;j1++) cprintf(" ");
            } else break;
          };
        };
      } break;
      case PI_INPUT: {
        textcolor(0);
        textbackground(7);
        cprintf("%s", (char *) p->item[i].data);
        for (j=0;j<p->item[i].x2-strlen(p->item[i].data);j++) cprintf(" ");
        textbackground(1);
        cprintf(" ");
      } break;
      case PI_PROGRESS: {
        int ii, j;
        if (p->item[i].index)
          j = (float)(p->item[i].x2-p->item[i].x1+1)/p->item[i].y2*p->item[i].index;
        else
          j = -1;
        textbackground(0);
        gotoxy(p->item[i].x1, p->item[i].y1);
        textcolor(15);
        for (ii=0;ii<j;ii++) cprintf("Û");
        for (ii=p->item[i].x1+j;ii<p->item[i].x2;ii++) cprintf(" ");
      } break;
    };
    gotoxy(p->item[p->index].x1, p->item[p->index].y1);
    if (p->item[p->index].type == PI_INPUT) {
      textcolor(0);
      textbackground(7);
      cprintf("%sÛ%c", (char *) p->item[p->index].data, 8); /* using seal-like cursor */
    };
    if (p->item[p->index].type == PI_LIST && p->item[p->index].y2 != 0) {
      gotoxy(p->item[p->index].x1+1, p->item[p->index].y1+p->item[p->index].idx+1);
    };
  };
};

void draw_page_item(p_page p, int item)
{
  p->__draw_item = item;
  draw_page_items(p);
};

void draw_page(p_page p)
{
  textcolor(14); textbackground(1);
  clrscr();
  gotoxy(1, 1);
  textcolor(0); textbackground(7);
  clreol();
  cprintf(" %s", p->title);
  gotoxy(1, 25);
  cprintf(" %s", p->status);
  clreol();
  draw_page_items(p);
};

void next_item(p_page p)
{
  int c = p->index;
  do {
    p->index++;
    if (p->index >= p->items) p->index = 0;
    if (p->item[p->index].type == PI_TEXT || p->item[p->index].type == PI_PROGRESS) p->index++;
    if (p->index >= p->items) p->index = 0;
  } while (p->index != c && p->item[p->index].type == PI_TEXT);
};

void prev_item(p_page p)
{
  int c = p->index;
  do {
    p->index--;
    if (p->index < 0) p->index = p->items-1;
    if (p->item[p->index].type == PI_TEXT || p->item[p->index].type == PI_PROGRESS) p->index--;
    if (p->index < 0) p->index = p->items-1;
  } while (p->index != c && p->item[p->index].type == PI_TEXT);
};

void execute_page(p_page p)
{
  int ch = 0;
  int k, e, i, j;
  char s[2];

  s[1] = 0;

  for (i=0;i<p->items;i++) if (p->item[i].type == PI_INPUT) p->item[i].index = strlen((char *)p->item[i].data);

  draw_page(p);
  next_item(p);
  draw_page_items(p);
  while (ch != 283) {  /* 283=ESC */
    p->terminate = 0;
    if (p->before_keypress) p->before_keypress(p);
    ch = bioskey(0);
    k = (char) ch;
    e = ch >> 8;
    if (p->after_keypress) p->after_keypress(p, k, e);
    switch (k) {
      case 0: switch (e) {
        case 80: {
          if (p->item[p->index].type == PI_LIST && p->item[p->index].y2 != 0) {
            if (p->item[p->index].index < get_list_items(p->item[p->index].data)-1) {
              p->item[p->index].index++;
              if (p->item[p->index].idx < p->item[p->index].y2-p->item[p->index].y1-2) p->item[p->index].idx++; else p->item[p->index].scroll++;
              draw_page_item(p, p->index);
              //draw_page_items(p);
            } else {
              int index = p->index;
              next_item(p);
              draw_page_item(p, index);
              draw_page_item(p, p->index);
              //draw_page_items(p);
            };
          } else {
            int index = p->index;
            next_item(p);
            draw_page_item(p, index);
            draw_page_item(p, p->index);
            //draw_page_items(p);
          };
        } break;
        case 72: {
          if (p->item[p->index].type == PI_LIST && p->item[p->index].y2 != 0) {
            if (p->item[p->index].index > 0) {
              p->item[p->index].index--;
              if (p->item[p->index].idx > 0) p->item[p->index].idx--; else p->item[p->index].scroll--;
              draw_page_item(p, p->index);
              //draw_page_items(p);
            } else {
              int index = p->index;
              prev_item(p);
              draw_page_item(p, index);
              draw_page_item(p, p->index);
              //draw_page_items(p);
            };
          } else {
            int index = p->index;
            prev_item(p);
            draw_page_item(p, index);
            draw_page_item(p, p->index);
            //draw_page_items(p);
          };
        } break;
      } break;
      case 13: {
        if (p->item[p->index].type == PI_BUTTON) {
          if (p->on_command) p->on_command(p, p->item[p->index].command);
          if (p->item[p->index].command == CMD_EXIT) ch = 283;
        };
        if (p->item[p->index].type == PI_LIST && p->item[p->index].y2 == 0) { /* drop down list only */
          textcolor(15);
          textbackground(7);
          gotoxy(p->item[p->index].x1-1, p->item[p->index].y1-1);
          cprintf("Ú");
          for (i=0;i<p->item[p->index].x2;i++) cprintf("Ä");
          textcolor(8);
          textbackground(7);
          cprintf("¿");
          gotoxy(p->item[p->index].x1-1, p->item[p->index].y1+get_list_items(p->item[p->index].data));
          textcolor(15);
          textbackground(7);
          cprintf("À");
          textcolor(8);
          textbackground(7);
          for (i=0;i<p->item[p->index].x2;i++) cprintf("Ä");
          cprintf("Ù");
          k = 0;
          gotoxy(1, 25);
          cprintf(" Use arrows to move the red bar and ENTER to select the item under the red bar", p->status);
          clreol();
          while (k != 13) {  /* 283=ESC */
            for (j=0;j<get_list_items(p->item[p->index].data);j++) {
              textcolor(15);
              textbackground(7);
              gotoxy(p->item[p->index].x1-1, p->item[p->index].y1+j);cprintf("³");
              textcolor(8);
              textbackground(7);
              gotoxy(p->item[p->index].x1+p->item[p->index].x2, p->item[p->index].y1+j);cprintf("³");
              gotoxy(p->item[p->index].x1, p->item[p->index].y1+j);
              if (j == p->item[p->index].index) {
                textcolor(15);
                textbackground(4);
              } else {
                textcolor(0);
                textbackground(7);
              };
              cprintf("%s", get_list_item(j, p->item[p->index].data));
              for (i=0;i<p->item[p->index].x2-strlen(get_list_item(j, p->item[p->index].data));i++) cprintf(" ");
            };
            ch = bioskey(0);
            k = (char) ch;
            e = ch >> 8;
            switch (k) {
              case 0: switch (e) {
                case 80: {
                  if (p->item[p->index].index < get_list_items(p->item[p->index].data)-1) p->item[p->index].index++;
                } break;
                case 72: {
                  if (p->item[p->index].index > 0) p->item[p->index].index--;
                } break;
              } break;
            };
          };
          draw_page(p);
        };
      } break;
      case 8: {
        if (p->item[p->index].type == PI_INPUT && strcmp((char *)p->item[p->index].data, "") != 0) {
          ((char *) p->item[p->index].data)[strlen((char *)p->item[p->index].data)-1] = 0;
          draw_page_item(p, p->index);
          //draw_page_items(p);
        };
      } break;
      case 9: {
        int index = p->index;
        next_item(p);
        draw_page_item(p, index);
        draw_page_item(p, p->index);
        //draw_page_items(p);
      } break;
      default: {
        if (p->item[p->index].type == PI_INPUT && strlen((char *)p->item[p->index].data) < p->item[p->index].x2) {
          s[0] = k;
          strcat((char *) p->item[p->index].data, s);
          draw_page_item(p, p->index);
          //draw_page_items(p);
        };
      };
    };
    if (p->terminate) ch = 283;
  };
};

void free_page(p_page p)
{
  /* UNFINISHED */
  free(p);
}

char *load_list_from_file(const char *filename)
{
  FILE *f = fopen(filename, "rt");
  char *s = (char *) malloc(256);
  char *r;
  int need_mem = 0, i, j;
  if (!f) return "\0";

  while (fgets(s, 255, f)) {
    s[strlen(s)-1]=0;
    if (strcmp(s, "") == 0) strcpy(s, " ");
    for (i=0;i<strlen(s);i++) {
      need_mem++;
      if (s[i] == '\t') need_mem += 8;
    };
    need_mem++;
  };

  freopen(filename, "rt", f);

  r = (char *) malloc(need_mem+1);

  j = 0;
  while (fgets(s, 255, f)) {
    s[strlen(s)-1] = 0;
    if (strcmp(s, "") == 0) strcpy(s, " ");
    for (i=0;i<strlen(s);i++) {
      if (s[i] == '\t') {
        r[j] = ' '; j++;
        r[j] = ' '; j++;
        r[j] = ' '; j++;
        r[j] = ' '; j++;
        r[j] = ' '; j++;
        r[j] = ' '; j++;
        r[j] = ' '; j++;
        r[j] = ' '; j++;
      } else {
        r[j] = s[i];
        j++;
      };
    };
    r[j] = 0;
    j++;
  };

  r[j] = 0;

  fclose(f);

  free(s);

  return r;
};

char *new_list()
{
  char *list = (char *) malloc(2);
  list[0] = 0;
  list[1] = 0;

  return list;
};

char *add_list_item(char *list, char *item)
{
  int old = 1, start = 0, i = 0;
  list = (char *) realloc(list, sizeof(*list)+strlen(item)+1);
  if (!list[0] && !list[1]) start = 0; else
  do {
    if (!list[i] && !old) {
      start = i;
      break;
    };
    old = list[i];
    i++;
  } while (1);
  for (i=0;item[i];i++) {
    list[start+i] = item[i];
  };
  list[start+i] = 0;
  list[start+i+1] = 0;

  return list;
}

void add_page_item(p_page p, int type, int x1, int y1, int x2, int y2, int command, void *data)
{
  t_page_item i;
  i.type = type;
  i.x1 = x1;
  i.y1 = y1;
  i.x2 = x2;
  i.y2 = y2;
  i.command = command;
  i.index = 0;
  i.idx = 0;
  i.scroll = 0;
  i.data = data;
  if (type == PI_CHECKLIST)
    i.idata = (int *) malloc(4*get_list_items(p->item[p->index].data));
  else
    i.idata = 0;

  p->items++;
  p->item[p->items-1] = i;
};

