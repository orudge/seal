#include <seal.h>
#include "var.h"
#include <colors.h>
#include <app.h>
#include <button.h>
#include <screen.h>
#include <menus.h>
#include <dialogs.h>
#include <stattext.h>
#include <iodlg.h>

#include <inputbox.h>

SetInfoAppName("VDEV");
SetInfoDesciption("Visual Seal Devellopement");
SetInfoCopyright("(c) Copyright 2001 Point Mad. All rights reserved");
SetInfoManufacturer("Point Mad");

l_dword free_message = 1450000;

/******/

typedef struct t_varlist *p_varlist;

p_varlist varlst;

typedef struct t_editcode *p_editcode;

/******/

typedef struct  t_devobj *p_devobj;

typedef struct  t_devcls {

  l_text name;

  /*p_list properties;
  p_list events;*/

  l_ubig dvnb;

  p_list includes;

  BITMAP *icon;

  l_text (*gen_declare)  ( p_devobj o );
  l_text (*gen_loader)   ( p_devobj o );
  l_text (*gen_tr_event) ( p_devobj o );
  l_text (*gen_others)   ( p_devobj o );
  void   (*draw)         ( BITMAP *out, t_rect r, t_point p, p_devobj o );
  void   (*init)         ( p_devobj o );

} t_devcls, *p_devcls;
#define DEVCLS(o) ((p_devcls)(o))

typedef struct  t_devobj {
  struct t_view objclass;

  p_list   properties;
  p_list   events;
  p_devcls devcls;

} t_devobj;

void varlist_set_obj ( p_varlist o, p_devobj devlink );
p_editcode get_edit_code ( p_devobj o );

#define DEVOBJ(o) ((p_devobj)(o))
#define m_devobj  _malloc(sizeof(t_devobj))

#include"var.c"
////////////////////////////////////////////////////////////////////////////////
p_list   devclass = NULL;

p_devobj wkdv     = NULL;
p_devobj base     = NULL;
////////////////////////////////////////////////////////////////////////////////
void add_new_devcls ( l_text name,
                      p_list includes,
                      BITMAP *icon,
                      l_text (*gen_declare)  ( p_devobj o ),
                      l_text (*gen_loader)   ( p_devobj o ),
                      l_text (*gen_tr_event) ( p_devobj o ),
                      l_text (*gen_others) ( p_devobj o ),
                      void   (*draw)         ( BITMAP *out, t_rect r, t_point p, p_devobj o),
                      void   (*init)         ( p_devobj o ) ) {

  p_devcls o = _malloc(sizeof(t_devcls));
  o->name = name;
  o->includes = includes;
  o->icon = icon;
  o->gen_declare = gen_declare;
  o->gen_loader = gen_loader;
  o->gen_tr_event = gen_tr_event;
  o->gen_others = gen_others;
  o->draw = draw;
  o->init = init;
  o->dvnb = 0;
  if ( !devclass )  devclass = list_init(malloc(sizeof(t_list)),NULL,NULL );
  devclass->insert(devclass,o);
};
////////////////////////////////////////////////////////////////////////////////
void devobj_draw ( p_view o ) {

  t_rect   r  = o->size_limits(o);
  t_point  p;
  p_devobj dv = DEVOBJ(o);

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {
    dv->devcls->draw(out,r,p,dv);
    if ( dv == wkdv ) {
      if ( dv != base ) rectfill(out,r.b.x+p.x-10,r.b.y+p.y-10,r.b.x+p.x,r.b.y+p.y,COLOR(8));
      rect(out,r.a.x+p.x+2,r.a.y+p.y+2,r.b.x+p.x-2,r.b.y+p.y-2,COLOR(8));
      rect(out,r.a.x+p.x+4,r.a.y+p.y+4,r.b.x+p.x-4,r.b.y+p.y-4,COLOR(8));
    };

  };

  o->end_of_paint(o, r);
};
////////////////////////////////////////////////////////////////////////////////
void devobj_translate_event ( p_object o, p_event event ) {

  view_translate_event(o, event);

  if ( event->type & EV_MOUSE ) {

    if ( wkdv == DEVOBJ(o) && base != DEVOBJ(o) && OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) {

        t_point m = VIEW(o)->get_local_point ( VIEW(o), mouse->where );
        t_rect  r = VIEW(o)->get_local_extent ( VIEW(o) );
        move_step = 5;
        if (  m.x > r.b.x - 10 && m.x < r.b.x && m.y > r.b.y - 10 && m.y < r.b.y )
          VIEW(o)->drag_view(VIEW(o), DM_DRAGGROW+DM_DRAGCONTEXT, event);
        else
          VIEW(o)->drag_view(VIEW(o), DM_DRAGMOVE+DM_DRAGCONTEXT, event);
        move_step = 0;
        clear_event(event);
     };

     if ( OBJECT(mouse)->state & MO_SF_MOUSELDOWN ) {
       p_view old = VIEW(wkdv);
       o->select(o);
       wkdv = DEVOBJ(o);
       if ( old ) VIEW(old)->draw_me(VIEW(old));
       VIEW(o)->draw_me(VIEW(o));
       if ( varlst ) varlist_set_obj(varlst,wkdv);
       clear_event(event);
     };

   };
  if ( event->type & EV_KEYBOARD && o->is_state(o, OB_SF_FOCUSED) ) {
    if ( KEYPRESSED(KB_DEL) && DEVOBJ(o) == wkdv ) {
      if ( wkdv != base ) {
        p_object own = OBJECT(wkdv)->owner;
        p_editcode ed = get_edit_code(wkdv);

        dispose(OBJECT(wkdv));
        if ( ed ) dispose(OBJECT(ed));
        if ( own->last )
          wkdv = DEVOBJ(own->last);
        else
          wkdv = DEVOBJ(own);
        OBJECT(wkdv)->select(OBJECT(wkdv));
        VIEW(wkdv)->draw_me(VIEW(wkdv));
        if ( varlst ) varlist_set_obj(varlst,wkdv);
      } else {
        seal_error(ET_WARNING,"You can't delete base object !");
      };
      clear_event(event);
    };
  };
};

l_bool devobj_done ( p_object p ) {
  if ( p ) {
    p_devobj o = DEVOBJ(p);
    if ( o->events )     dispose_list(&o->events,true);
    if ( o->properties ) dispose_list(&o->properties,true);
  };
  return view_done ( p );
};
////////////////////////////////////////////////////////////////////////////////
p_devobj devobj_init ( p_devobj o, t_rect r, p_devcls devcls ) {

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_devobj));

  view_init(VIEW(o), r);

  OBJECT(o)->done            = &devobj_done;
  VIEW(o)->draw              = &devobj_draw;
  OBJECT(o)->translate_event = &devobj_translate_event;

  devcls->dvnb++;

  o->events = list_init(malloc(sizeof(t_list)),&free_var,NULL );
  o->properties = list_init(malloc(sizeof(t_list)),&free_var,NULL );
  o->devcls = devcls;

  VIEW(o)->drag_mode |=  DM_DRAGMOVE+DM_DRAGCONTEXT;
  VIEW(o)->draw_mode &= ~DWM_CANTACCELMOVE;
  OBJECT(o)->set_options(OBJECT(o), OB_OF_TOPSELECT, true);
  devcls->init(o);

  return o;
};

l_text join_text ( l_text t1, l_text t2, l_bool f1, l_bool f2 ) {
  l_dword l1 = strlen(t1);
  l_dword l2 = strlen(t2);
  l_text txt = malloc(l1+l2);
  strcpy(txt, t1);
  strcat(txt, t2);

  if (f1){ DEBUG_printf("Try to free <<%s>>\n",t1); _free(t1);};
  if (f2){ DEBUG_printf("Try to free <<%s>>\n",t2); _free(t2);};
  return txt;
};


////////////////////////////////////////////////////////////////////////////////
#include"vstattext.c"
#include"vbutton.c"
#include"vappwin.c"
#include"venv.c"

#include"editcode.c"
#include"varlist.c"

#include"vhistory.c"
#include"vcheckbox.c"
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

#define MSG_ADDVOBJ       10000
#define MSG_ADDVOBJ_MAX   10100
#define MSG_GENERATE      10101
#define MSG_SET_EVENT     10102
#define MSG_SAVE          10103
#define MSG_LOAD          10104

///////////////////////////////////////////////////////////////////////////////
void devobj_generate ( p_devobj dv, FILE *f, l_char section );

void generate_code ( l_text file ) {
  l_text loader = NULL;
  l_text declare = NULL;
  l_text tr_event = NULL;
  l_text others = NULL;
  FILE *f = fopen(file,"wt");

  if ( !f ) return;

  fprintf(f," /* My Seal Program
(c) Copyright 2001 [you]. All rights reserved.
web : [your web site]
email : [your email]
*/
#include\"seal.h\"
#include\"app.h\"
#include\"button.h\"
#include\"stattext.h\"
#include\"dialogs.h\"
#include\"checkbox.h\"

// Declarations ////////////////////////////////////////////////////////////////\n");

devobj_generate(base,f,1);

fprintf(f,"\n// Others functions/////////////////////////////////////////////////////////////\n");

devobj_generate(base,f,2);

fprintf(f,"\n// Events handler //////////////////////////////////////////////////////////////

void global_tr_event ( p_object o, p_event event ) {\n");

devobj_generate(base,f,3);

fprintf(f,"\n};

// App main handler (load, unload ... ) ////////////////////////////////////////
app_begin ( void ) {
  if ( ap_process == AP_INIT ) { // Load ...\n");

devobj_generate(base,f,4);

fprintf(f,"\n  };
} app_end;");



fclose(f);

};
////////////////////////////////////////////////////////////////////////////////
void devobj_generate ( p_devobj dv, FILE *f, l_char section ) {
  p_devcls dc = dv->devcls;
  l_text tmp = NULL;

  if ( section == 1 && dc->gen_declare ) {
	tmp = dc->gen_declare(dv);
    fprintf(f,"\n");
    fprintf(f,tmp);
    _free(tmp);
  };
  if ( section == 2 && dc->gen_others ) {
	tmp = dc->gen_others(dv);
    fprintf(f,"\n");
    fprintf(f,tmp);
    _free(tmp);
  };
  if ( section == 4 && dc->gen_loader ) {
	tmp = dc->gen_loader(dv);
    fprintf(f,"\n");
    fprintf(f,tmp);
    _free(tmp);
  };
  if ( section == 3 && dc->gen_tr_event ) {
	tmp = dc->gen_tr_event(dv);
    fprintf(f,"\n");
    fprintf(f,tmp);
    _free(tmp);
  };

  if ( OBJECT(dv)->last ) {
     p_object p = OBJECT(dv)->first_view(OBJECT(dv));
     p_object ff = p;
     do {
       devobj_generate ( DEVOBJ(p), f, section );
       p = OBJECT(p)->next_view(OBJECT(p));
     } while(ff != p);
  };

};
////////////////////////////////////////////////////////////////////////////////
static p_list get_file_items ( void ) {
  p_list p = list_init(malloc(sizeof(t_list)), &free_filehistory_item, 0);
  if ( p ) {
     p->insert(p, new_filehistory_item("C files (*.c)", "*.c"));
     p->insert(p, new_filehistory_item("All files (*.*)", "*.*"));
  };
  return p;
};
////////////////////////////////////////////////////////////////////////////////
void trans_ev ( p_object o, p_event event ) {
   if ( event->type == EV_MESSAGE ) {
      if ( event->message == MSG_CLOSE && o->phase == PH_PREPROCESS ) {
        if ( base ) dispose(OBJECT(base));
      };

      if ( event->message >= MSG_ADDVOBJ && event->message < MSG_ADDVOBJ_MAX ) {
        p_devcls dc = DEVCLS(devclass->at(devclass, event->message - MSG_ADDVOBJ));
        p_devobj dv = devobj_init(_malloc(sizeof(t_devobj)),rect_assign(50,50,150,150),dc);

        if ( wkdv )
          OBJECT(wkdv)->insert(OBJECT(wkdv),OBJECT(dv));
        else
          OBJECT(desktop)->insert(OBJECT(desktop),OBJECT(dv));
        clear_event(event);
      };
      if ( event->message == MSG_GENERATE ) {
        l_text file = save_dialog("/", "*.*", get_file_items());
        if ( file ) {
          generate_code(file);
          _free(file);
        };
        clear_event(event);
      };
      if ( event->message == MSG_ABOUT ) {
        l_text txt = set_format_text(NULL,"%s\n  %s\n  by %s\n\n%s",&InfoAppName,&InfoDesciption,&InfoManufacturer,&InfoCopyright);
        if ( txt ) {
          msgbox(MW_INFO, MB_OK, txt);
          _free(txt);
        };
        clear_event(event);
      };

      if ( event->message == MSG_SET_EVENT ) {
        p_editcode ed = get_edit_code(wkdv);
        if ( ed ) {
          OBJECT(ed)->select(OBJECT(ed));
          VIEW(ed)->show(VIEW(ed));
          VIEW(ed)->draw_me(VIEW(ed));
        } else
          make_editcode(_malloc(sizeof(t_editcode)),rect_assign(0,0,400,300),wkdv);
        clear_event(event);
      };
   };
};


////////////////////////////////////////////////////////////////////////////////
l_bool init_vdev_env ( void ) {
   p_button b;
   p_icobutton bi;
   p_item x = devclass->first(devclass);
   p_item f = x;
   l_int  i = 1;
   t_rect rr;
   p_appwin win = appwin_init(_malloc(sizeof(t_appwin)),  /* make window */
                               rect_assign(0,40,screen_width-1,screen_height-1),
                               "Visual DEVelopement 1.01",
                               WF_MINIMIZE,
                               ap_id,
                               &trans_ev);

   VIEW(win)->size_minimum = &view_size_minimum;

   /*b = button_init(_malloc(sizeof(t_appwin)),rect_assign(5,20,145,40),"Save",MSG_SAVE,BF_DEFAULT);
   OBJECT(win)->insert(OBJECT(win), OBJECT(b));  */

   b = button_init(_malloc(sizeof(t_appwin)),rect_assign(5,45,160,65),"Events",MSG_SET_EVENT,BF_DEFAULT);
   OBJECT(win)->insert(OBJECT(win), OBJECT(b));

   b = button_init(_malloc(sizeof(t_appwin)),rect_assign(5,70,160,90),"Generate",MSG_GENERATE,BF_DEFAULT);
   OBJECT(win)->insert(OBJECT(win), OBJECT(b));

   b = button_init(_malloc(sizeof(t_appwin)),rect_assign(5,95,160,115),TXT_ABOUT,MSG_ABOUT,BF_DEFAULT);
   OBJECT(win)->insert(OBJECT(win), OBJECT(b));

   rr = rect_assign(5,120,25,140);

   do {
      if ( devclass->first(devclass) != x ) {
         p_devcls dc = DEVCLS(x->rec);
         bi = icobutton_init( _malloc(sizeof(t_icobutton)),
                              rr,
                              dc->icon,
                              NULL,
                              MSG_ADDVOBJ+i,
                              BF_PUSH+BF_UNDERSEL);
         VIEW(bi)->info_text = set_format_text(NULL,"Insert a %s",dc->name);
         OBJECT(win)->insert(OBJECT(win), OBJECT(bi));
         rr = rect_move(rr,25,0);
         if ( rr.b.y >= 160 ) {
           rr.a.x = 5;
           rr.b.x = 25;
           rr = rect_move(rr,25,0);
         };
         i++;
       };
       x = x->next;
    } while ( x != f );

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(win));

   if ( win ) {

     p_devcls dc = DEVCLS(devclass->at(devclass,0));

     p_devobj dv = devobj_init(_malloc(sizeof(t_devobj)),rect_assign(165,25,screen_width-5,screen_height-45),dc);

     wkdv = dv;
     base = dv;

     OBJECT(win)->insert(OBJECT(win),OBJECT(dv));
   };

   varlst = varlist_init (_malloc(sizeof(t_varlist)),rect_assign(5,rr.b.y+5,160,screen_height-45),wkdv);
   OBJECT(win)->insert(OBJECT(win),OBJECT(varlst));
  return true;
};
////////////////////////////////////////////////////////////////////////////////
l_bool install_vdev ( void ) {
  install_venv();
  install_vappwin();
  install_vbutton();
  install_vstattext();
  install_vhistory();
  install_vcheckbox();
  return true;
};
////////////////////////////////////////////////////////////////////////////////
app_begin ( void ) {

  if ( ap_process == AP_INIT ) {

     install_vdev  ();
     init_vdev_env();
  } else if ( ap_process == AP_DONE ) {

  };

} app_end;

