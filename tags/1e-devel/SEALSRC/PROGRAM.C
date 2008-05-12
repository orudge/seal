#include<string.h>
#include<go32.h>
#include<sys/exceptn.h>

#define  __DEBUG_MEMORY__
#undef   __DEBUG_MEMORY__

#define __SEAL__
#define __MAIN_INCLUDED__
//#define __CLOCK_INCLUDED__
#define i_love_br_font

#include"allegro.h"

#include"object.c"
#include"alltogrx.c"
#include"dataini.c"
#include"dlx.h"
#include"_iodir.c"

#include"list.c"
#include"exedlx.c"

#include"keyboard.c"
#include"mouse.c"
#include"screen.c"
#include"drivers.c"
#include"colors.c"

#include"view.c"
#include"window.c"

#include"program.h"

#include"seal.exp" /* export table */


/* the main program object */
l_int  task_tick        = 20;

/* the main program object */
t_object   program;
/* list of objects that use OB_OF_STILLPROCESS option flag */
t_list     stillprocess_list;

/* drivers, the main object for drivers, such as keyboard, mouse, ... */
p_drivers  drivers = NULL;

/* destkop, the main object of visible objects */
p_view     desktop = NULL;

/* define if clock-cursor must be included */
l_int      clock_including = 0;

/* contains queue of events ( if you use put_event function ) */
static     t_event event_panding;

/* saves the last time of caling program_translate_event function */
static     l_big   event_oldtimer;


/* insert event to event queue */
void program_put_event ( t_object *o, t_event *event )
{
  event_panding = (*event);
};


/*

   insert object (s) to stillprocess objects

   return 1, if all is OK
   return 0, on error

*/

int  program_put_into_stillprocess ( p_object o, p_object s )
{

  if ( stillprocess_list.insert(&stillprocess_list, s) != -1 )

    return 1;

  return 0;

};


/*

  clear object (s) from stillprocess objects

  return 1, if everything is OK
  return 0, on error

*/

int  program_clear_from_stillprocess ( p_object o, p_object s )
{

  p_item p = stillprocess_list.find_rec(&stillprocess_list, s);

  if ( p ) {

      stillprocess_list.remove_item(&stillprocess_list, p);

      return 1;

  };

  return 0;

};


/*
  main get_event function.

  - get last event from event's queue
*/

void program_get_event ( t_object *o, t_event *event )
{
  STOP_PROCESS();

  clear_event(event);

  /* save the last time of calling */

  event_oldtimer = time_get_mili();


  if ( event_panding.type != EV_NOTHING ) { /* something in queue */

    (*event) = event_panding; /* new event will contain last from queue */

    clear_event(&event_panding); /* clear queue */

  } else { /* nothing in queue */

    if ( drivers ) { /* drivers exists */


        /* call each object in drivers object */
        ((p_object)drivers)->translate_event((p_object)drivers, event);

    };
  };

  START_PROCESS();


  /* call each stillprocess object */
  program_call_each_stillprocess(&stillprocess_list);
};


/* main translate event function */
void program_translate_event ( t_object *o, t_event *event )
{
  /* save the last time of calling */
  event_timer = time_get_mili();

  /* event type is message */
  if ( event->type == EV_MESSAGE ) {

    /* event message is MSG_QUIT */
    if ( event->message == MSG_QUIT ) {

      clear_event(event);
      /* set end_state of program to MSG_QUIT

         this is used in function obj_execute
      */
      o->end_state = MSG_QUIT;

    };

  };

  /* call each object in program hierarchy */
  obj_translate_event(o, event);

  /* if keyboard was pressed and the keycode of the key is ALT+X */
  if ( event->type == EV_KEYBOARD && OBJECT(keyb)->state & KB_SF_KEYDOWN
       && KEYPRESSED(TO_ALT(KB_X)) ) {

    clear_event(event);

    /* set event by these arguments */
    set_event(event, EV_MESSAGE, MSG_QUIT, o);

    /* put event to a queue */
    o->put_event(o, event);

    clear_event(event);

  };

  /* if keyboard was pressed and the keycode of the key is ALT+D */
  if ( event->type == EV_KEYBOARD && OBJECT(keyb)->state & KB_SF_KEYDOWN
       && KEYPRESSED(TO_ALT(KB_D)) ) {

        p_object v = NULL;

        /* find the selected object in the desktop */
        if ( OBJECT(desktop)->prefer )

           /* search for the next visible, selectable, enable object in the desktop */
           v = OBJECT(desktop)->prefer->find_match_view(OBJECT(desktop)->prefer, OB_SF_VISIBLE, OB_OF_SELECTABLE+OB_OF_ENABLE, false);

  	     clear_event(event);

        /* if found some, select it */
        if ( v ) v->select(v);

  };


};


/*
  done program

  return true, if succesfull, otherwise false
*/
l_bool  program_done ( t_object *o )
{
  /* done each object in the program hierarchy */
  if ( !obj_done(o) ) return false;

  /* free list of stillprocess objects */
  stillprocess_list.done(&stillprocess_list);

  if ( drivers )

        /* done drivers = mouse, keyboard, screen, fonts, etc... */

        OBJECT(drivers)->done(OBJECT(drivers));

  /* free list of extension runners */
  done_ext_runners();

  drv_done(); /* done drives */

  filter_remove_all(); /* remove all filters from the array */

  return true;
};



t_object *program_init ( t_object *o )
{
  t_rect r;

  if ( !o ) return NULL;

  obj_init(o);

  init_ext_runners(); /* define extension runners */

  drv_init(); /* init drives */

  drivers = drivers_init(_malloc(sizeof(t_drivers)));

  DEBUG_printf(" - program_init : window_ini_palette()\n");

  window_ini_palette(); /* set window palette */

  clear_event(&event_panding);

  DEBUG_printf(" - program_init : view_ini()\n");

  view_ini(); /* all view initalizations */

  DEBUG_printf(" - program_init : list_init(&stillprocess_list, NULL, 0)\n");

  list_init(&stillprocess_list, NULL, 0);

  o->get_event = &program_get_event;
  o->put_event = &program_put_event;
  o->done = &program_done;
  o->translate_event = &program_translate_event;
  o->put_into_stillprocess = &program_put_into_stillprocess;
  o->clear_from_stillprocess = &program_clear_from_stillprocess;

  o->set_state(o, OB_SF_FOCUSED, true);
  o->set_options(o, OB_OF_TOPSELECT+OB_OF_NOTACTIVATE, true);

  DEBUG_printf(" - program_init : read_set_file(SET_DESKTOP)\n");

  /* read desktop.set file and call all programs that contains this file */
  read_set_file(SET_DESKTOP);

  DEBUG_printf(" - program_init : clock_including = getininum_fromfile(...)\n");
  /* get info from INI file, if clock-cursor must be included */
  clock_including = getininum_fromfile(INI_MAINFILE, INI_OPTIMALIZATION, "clock_including");

  DEBUG_printf(" - program_init : desktop = view_init(...)\n");

  if ( !desktop ) /* install desktop */

     desktop = view_init(_malloc(sizeof(t_view)), rect_assign(0, 0, screen_width-1, screen_height-1));


  if ( desktop ) { /* set desktop */

    /* set dekstop background */
    l_text fname = getini_fromfile(INI_MAINFILE, INI_DESKTOP, "wall_paper");
    l_text style = getini_fromfile(INI_MAINFILE, INI_DESKTOP, "style");

    desktop->brush.color = color_get_from_ini("desktop_background");;

    /* exist name of wall_paper */
    if ( fname ) {

      /* load image for wall_paper */
      desktop->brush.background = load_image(fname);

      /* if wall_paper exist */
      if ( desktop->brush.background ) {

        desktop->brush.state     |= BRUSH_SELFIMG;

        if ( style && !stricmp(style, "stretch") ) /* style is stretch */

          desktop->brush.state     |= BRUSH_STRETCH;

        else
        if ( style && !stricmp(style, "center") ) /* style is center */

          desktop->brush.state     |= BRUSH_CENTER;
      };

    };

    _free(fname);
    _free(style);

    OBJECT(desktop)->set_options(OBJECT(desktop), OB_OF_TOPSELECT+OB_OF_NOTACTIVATE, true);

  };


  /* insert desktop to program */
  o->insert(o, OBJECT(desktop));

  DEBUG_printf(" - program_init : read_set_file(SET_APPLICATIONS)\n");
  /* call applications from app.set file */
  read_set_file(SET_APPLICATIONS);

  DEBUG_printf(" - program_init : mouse_set_cursor_id(CUR_ARROW)\n");
  /* set mouse cursor to standard arrow */
  mouse_set_cursor_id(CUR_ARROW);

  DEBUG_printf(" - program_init : return (o) \n");
  return o;
};


/*
  call each object's func_callback, that is placed in stillprocess list.
*/
void program_call_each_stillprocess ( p_list o )
{

  if ( o ) {

    p_item v = o->first(o); /* find first object */
    p_item f = v;

    if ( v )

      do {

        if ( v->rec &&
             /* difference between last calling and current is GE to process_tick of object */
             time_diff_mili(OBJECT(v->rec)->process_time) >= OBJECT(v->rec)->process_tick ) {

          /* set current time */
          OBJECT(v->rec)->process_time = time_get_mili();
          /* call function of the object */
          OBJECT(v->rec)->func_callback(OBJECT(v->rec));

        };

        v = v->next;

      } while ( v != f );

  };

};

static void  aclock2 ( void )
{

  if ( !mouse->is_block(mouse) )

  if ( _time_diff_mili(event_oldtimer) >= 500 )

      mouse_set_cursor_id(CUR_CLOCK);

  else if ( mouse->get_cursor(mouse) == CURSOR_GETSYSTEM(CUR_CLOCK) )

      mouse_set_cursor_id(CUR_ARROW);

};


/* main process interrupt */

int program_int ( void ) {

  /* if not pause in multitasking */
  if ( go_process ) {

    /* difference is greater then task_tick */
    if ( _time_diff_mili(event_timer) >= task_tick ) {

      /* get current time */
      event_timer = time_get_mili();

      /* get events */
      program.get_event(&program, &event_main);


      /* one loop program_translate_event */
      program.translate_event(&program, &event_main);

    };

  };

  return 1;

};

/* end of interrupt */


/* first screen you ever see */
static void  xx_screen_shot ( void )
{

    /* load image */
    BITMAP *b = load_image("startup.bmp");

    if ( b ) { /* image exist */

        /* calculate center of screen  for image */
        l_rect x = (screen_width  - IMAGE_WIDTH(b))/2;
        l_rect y = (screen_height - IMAGE_HEIGHT(b))/2;

        mouse->hide(mouse);

        /* draw image */
        clear_to_color(screen, makecol(51, 51, 31));

        draw_sprite(screen, b, x, y);

        mouse->show(mouse);

        mouse_set_cursor_id(CUR_CLOCK);

    /* destroy image */
    destroy_bitmap(b);

   };

};



//#define __DEBUG_FILE__ /* debug version */

/* main of seal */

int main ( int argc, char **argv )
{

  ini_mainfile = strdup("seal.ini");

  screen_shot = &xx_screen_shot;

  /* test if SEAL run in safe mode */
  if ( argc > 1 && argv[1] && !stricmp(argv[1], "-safe") )

      /* SEAL now run in safe mode */
      safe_mode = 1;


  #ifdef __TEST_FILE__
    seal_test_file = fopen("seal.tst", "wt");
  #endif
  #ifdef __DEBUG_FILE__
    seal_debug_file = fopen("seal.dbg", "wt");
  #endif

  program_init(&program);

  #ifdef __TEST_FILE__
    fclose(seal_test_file);
  #endif

  #ifdef __CLOCK_INCLUDED__
  if ( clock_including )

       install_int(&aclock2, 500);
  #endif

  program.execute(&program);


  program.done(&program);

  #ifdef __DEBUG_FILE__
    fclose(seal_debug_file);
  #endif

};

