/* Screen pages 1.1 (c) 2000,2001 Kostas Michalopoulos aka Bad Sector */

/* Page items */
#define PI_TEXT         0
#define PI_INPUT        1
#define PI_LIST         2
#define PI_BUTTON       3
#define PI_CHECKLIST    4
#define PI_PROGRESS     5

// note: PI_CHECKLIST is unfinished, please don't touch it!

/* Commands */
#define CMD_NOTHING     0
#define CMD_EXIT        1

typedef struct t_page_item
{
  int type;
  int x1, y1, x2, y2;
  int command;
  int index;
  int idx, scroll;
  void *data;
  int *idata;
} t_page_item;

typedef struct t_page *p_page;

typedef struct t_page
{
  t_page_item item[100];
  int items, index, __draw_item;
  int terminate;
  char title[79];
  char status[79];
  void (*before_keypress)(p_page p);
  void (*after_keypress)(p_page p, int key, int scancode);
  void (*on_command)(p_page p, int cmd);
} t_page;

#define NEW_PAGE (p_page) malloc(sizeof(t_page))

/* Initializes a screen page */
void init_page(p_page p, const char *title);

/* Destroys the page and frees the allocated memory */
void free_page(p_page p);

/* Draws all items a page */
void draw_page_items(p_page p);

/* Draws the specified item */
void draw_page_item(p_page p, int item);

/* Draws a page and it's items */
void draw_page(p_page p);

/* Executes a screen page */
void execute_page(p_page p);

/* Adds an item to a page (used from add_xxx defines) */
/* NOTE: does not check if there is room for the item */
void add_page_item(p_page p, int type, int x1, int y1, int x2, int y2, int command, void *data);

char *load_list_from_file(const char *filename);

char *new_list();
char *add_list_item(char *list, char *item);
char *get_list_item(int idx, char *list);

/* adds text "t" to page "p" in pos (x, y) */
#define add_text(p, x, y, t) add_page_item(p, PI_TEXT, x, y, 0, 0, 0, (void *)strdup(t))

/* adds an input box to page "p" in pos (x, y) with length "len". buffer
is used to store the text */
#define add_input(p, x, y, len, buffer) add_page_item(p, PI_INPUT, x, y, len, 0, 0, buffer)

/* adds a drop-down list box to page "p" in pos (x, y) with length "len". List
is in buffer "list" and every item is separated from the other with a \0. Last
item must have two \0. f.e.

   "Item1\0Item2\0Item3\0"

   in this example we're using a string, so the second \0 in is string's end
   (remember that C uses ASCIIZ - strings terminated with \0);

*/
#define add_drop_list(p, x, y, len, list) add_page_item(p, PI_LIST, x, y, len, 0, 0, list)

/* adds a list box to page "p" in rectangle (x1, y1, x2, y2). List is defined
as in add_drop_list. */
#define add_list(p, x1, y1, x2, y2, list) add_page_item(p, PI_LIST, x1, y1, x2, y2, 0, list)

/* adds a button to page "p" in pos (x, y), command "cmd" and caption "cap". */
#define add_button(p, x, y, cap, cmd) add_page_item(p, PI_BUTTON, x, y, 0, 0, cmd, &cap)

/* similar to "add_list", but the items can be (un)checked using the Enter or
 the space bar. The data are in item's idata pointer (access them by writing
 ->idata[item] where item is the item you want to read/alter */
#define add_checklist(p, x1, y1, x2, y2, list) add_page_item(p, PI_CHECKLIST, x1, y1, x2, y2, 0, list)

/* adds a progress bar to page "p" in area (x1,y1)-(x2,y1). Max is the
 maximum value that the progress bar can take. p->index is the current
 position */
#define add_progress(p, x1, y1, x2, max) add_page_item(p, PI_PROGRESS, x1, y1, x2, max, 0, NULL)

