/*

(c) Copyright 2001 Julien Etelain. All rights reserved.

*/

/* Revision History (as of 04/04/2002):
 *
 * 04/04/2002: Updated with new icon (orudge)
 */

#include<seal.h>
#include<app.h>
#include<dialogs.h>
#include<button.h>
#include<iodlg.h>
#include<inputbox.h>
#include<menus.h>
#include<grfx-f.H>
#include<seal/tabbook.h>

#include"simpdat.h"

static DATAFILE *dat = NULL;

#define Get_Bitmap(n) (BITMAP*)GET_DATA(dat, n)

#include"imgedit.c"



////////////////////////////////////////////////////////////////////////////////

#define MSG_TOOL_MIN             200000
#define MSG_TOOL_MAX             200010

#define MSG_HISUPDATE            200100

#define MSG_TOOL_OPEN            200101
#define MSG_TOOL_NEW             200102
#define MSG_TOOL_SAVE            200103
#define MSG_TOOL_BORDER_P        200104
#define MSG_TOOL_BORDER_M        200105
#define MSG_TOOL_SET_TRANS_VALUE 200106
#define MSG_xABOUT               200107
#define MSG_TOOL_UNDO            200108
#define MSG_TOOL_CFONT           200109
#define MSG_TOOL_COPY            200110
#define MSG_TOOL_PASTASS         200111
#define MSG_TOOL_PASTASP         200112
#define MSG_TOOL_SAVEAS          200113
#define MSG_xHELP                200114
#define MSG_TOOL_SET_TOLERANCE   200115
#define MSG_TOOL_SET_CENTER      200116
#define MSG_TOOLBOX              200117
#define MSG_TOOL_RESIZE          200118

#define MSG_FILTERS_MIN          200200
#define MSG_FILTERS_MAX          200250



////////////////////////////////////////////////////////////////////////////////

l_int oldset1x = 50;
l_int oldset1a = 90;

p_appwin xwin = NULL;
p_appwin xtoolbox = NULL;
p_appwin xcolors = NULL;
t_rect rtoolbox ;
t_rect rcolors ;
#define new_menu_item2(name,param,hk,msg,it,enable,img,next) new_menu_item_ex(name, param, hk, msg, enable, it, 0, NULL, img, NULL, 0, next)
#define new_sub_menu2(name,menu,enable,img,next) new_sub_menu_ex(name, enable, NULL, NULL, img, NULL, 0, menu, next)


////////////////////////////////////////////////////////////////////////////////

p_imgedit  editorx     = NULL;
p_colsel2  cco         = NULL;
p_stattext sto         = NULL;
p_tabbook  conf_tab    = NULL;

p_history  pen_forme     = NULL;
p_history  pen_trans     = NULL;

p_history forme_fill     = NULL;
p_history forme_forme    = NULL;

p_history ffill_gardient = NULL;

l_text txttool[] = { "MOVE", "ZOOM", "GET", "POINT", "FILL", "FORME" ,"SELECT","TEXT" };

l_int  txttoolint[] = { MOVE_CUR,ZOOM,GET,PEN,FLOOD,FORME,SELECT,TEXT};

l_text actual_path = "/";
////////////////////////////////////////////////////////////////////////////////
static p_menu  filters_menu_init (  )
{
p_menuitem v = NULL;

          p_item x = imgfilters->first(imgfilters );
          p_item f = x;
          l_int  a = 0;

          do {

            v = new_menu_item2(imgfilter(x->rec)->name, "", 0, MSG_FILTERS_MIN + a,  NULL , true, NULL, v);
            a++;
            x = x->next;
          } while ( x != f );

return new_menu(v);

};
////////////////////////////////////////////////////////////////////////////////
static p_list pen_forme_list ( void ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);


      p->insert(p, new_history_item(INI_TEXT("Rect") ,NULL,0,NULL));
      p->insert(p, new_history_item(INI_TEXT("Circle") ,NULL,0,NULL));

  return p;
};
////////////////////////////////////////////////////////////////////////////////
static p_list pen_trans_list ( void ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);
      p->insert(p, new_history_item(INI_TEXT("Full") ,NULL,0,NULL));
      p->insert(p, new_history_item(INI_TEXT("Transparent") ,NULL,0,NULL));
      p->insert(p, new_history_item(INI_TEXT("Linear Transparent") ,NULL,0,NULL));
  return p;
};
////////////////////////////////////////////////////////////////////////////////
static p_list forme_fill_list ( void ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);
      p->insert(p, new_history_item(INI_TEXT("No fill") ,NULL,0,NULL));
      p->insert(p, new_history_item(INI_TEXT("Fill with color 1") ,NULL,0,NULL));
      p->insert(p, new_history_item(INI_TEXT("Fill with color 2") ,NULL,0,NULL));
  return p;
};
////////////////////////////////////////////////////////////////////////////////
static p_list forme_forme_list ( void ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);
      p->insert(p, new_history_item(INI_TEXT("Line") ,NULL,0,NULL));
      p->insert(p, new_history_item(INI_TEXT("Rect") ,NULL,0,NULL));
      p->insert(p, new_history_item(INI_TEXT("Circle") ,NULL,0,NULL));
      p->insert(p, new_history_item(INI_TEXT("Ellipse") ,NULL,0,NULL));
  return p;
};
////////////////////////////////////////////////////////////////////////////////
static p_list ffill_gardient_list ( void ) {

  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);
      p->insert(p, new_history_item(INI_TEXT("Full") ,NULL,0,NULL));
      p->insert(p, new_history_item(INI_TEXT("Linear Gradient") ,NULL,0,NULL));
      p->insert(p, new_history_item(INI_TEXT("Rect Gradient") ,NULL,0,NULL));
      p->insert(p, new_history_item(INI_TEXT("Circle Gradient") ,NULL,0,NULL));
      p->insert(p, new_history_item(INI_TEXT("Radial Gradient") ,NULL,0,NULL));
  return p;
};
////////////////////////////////////////////////////////////////////////////////
static p_menu  editimg_menu_init ( void )
{

  p_menu menu = new_menu(

                  new_sub_menu(INI_TEXT("File"), new_menu(

                     new_menu_item(INI_TEXT("New"),     "", 0, MSG_TOOL_NEW, NULL,
                     new_menu_item(INI_TEXT("Open"),    "", 0, MSG_TOOL_OPEN, NULL,
                     new_menu_item(INI_TEXT("Save"),    "", 0, MSG_TOOL_SAVE, NULL,
                     new_menu_item(INI_TEXT("Save As"), "", 0, MSG_TOOL_SAVEAS, NULL,
                     new_menu_line(
                     new_menu_item(INI_TEXT("Exit"), "", TO_ALT(KB_F4), MSG_CLOSE, NULL,

                     NULL))))))),

                  new_sub_menu(INI_TEXT("Edit"), new_menu(

                     new_menu_item2(INI_TEXT("Copy"), "", 0, MSG_TOOL_COPY , NULL, true ,Get_Bitmap(COPY),
                     new_menu_item2(INI_TEXT("Paste As a New Selection"), "", 0, MSG_TOOL_PASTASS , NULL,true,Get_Bitmap(PASTE),
                     new_menu_item2(INI_TEXT("Paste As a New Picture"), "", 0, MSG_TOOL_PASTASP , NULL,true,Get_Bitmap(PASTE),
                     new_menu_line(
                     new_menu_item2(INI_TEXT("Undo"), "", 0, MSG_TOOL_UNDO , NULL,true,Get_Bitmap(UNDO),

                     NULL)))))),

                  new_sub_menu(INI_TEXT("Picture"), new_menu(

                     new_sub_menu2(INI_TEXT("Filters"), filters_menu_init(),true,Get_Bitmap(FILTERS),
                     new_menu_item(INI_TEXT("Resize"), "", 0, MSG_TOOL_RESIZE, NULL,
                     new_menu_item(INI_TEXT("Correct RGB"), "", 0, MSG_NOTHING, NULL,

                     NULL)))),

                  new_sub_menu(INI_TEXT("Options"), new_menu(

                     new_menu_item2(INI_TEXT("Select Font"), "", 0, MSG_TOOL_CFONT, NULL,true,Get_Bitmap(TEXT) ,

                     NULL)),

                  new_sub_menu(INI_TEXT("?"), new_menu(

                     new_menu_item(INI_TEXT("Help"), "", 0, MSG_xHELP, NULL,
                     new_menu_item(INI_TEXT("About..."), "", 0, MSG_xABOUT, NULL,

                     NULL))),

               NULL))))));

   return menu;

};

////////////////////////////////////////////////////////////////////////////////

static p_list get_file_items ( void ) {
  p_list p = list_init(malloc(sizeof(t_list)), &free_filehistory_item, 0);
  if ( p ) {
     p->insert(p, new_filehistory_item("BMP (*.bmp)", "*.bmp"));
     p->insert(p, new_filehistory_item("JPEG (*.jpg)", "*.jpg"));
     p->insert(p, new_filehistory_item("GIF (*.gif)", "*.gif"));

     p->insert(p, new_filehistory_item("All Files (*.*)", "*.*"));
  };
  return p;
};

////////////////////////////////////////////////////////////////////////////////
void resizebox ( ) {
if ( !editorx->select ) {
     t_rect   r = rect_assign(0, 0, 400, 120);
     p_stattext lbl     = NULL;
     p_textline wx = NULL;
     p_textline hx = NULL;
     p_button   b       = NULL;

     l_dword    msg     = NULL;
     p_appwin   w       = appwin_init(_malloc(sizeof(t_appwin)),
                            r,
                            INI_TEXT("Resize Picture"),
                            NULL,
                            ap_id,
                            NULL);


   if ( w ) VIEW(w)->align |= TX_ALIGN_CENTER;


   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(w));


r = rect_assign(3, 20, 397, 40);
lbl = stattext_init(_malloc(sizeof(t_stattext)),
                      r,
                      TX_ALIGN_BOTTOM,
                      INI_TEXT ("New size ( width x height )"));

OBJECT(w)->insert(OBJECT(w), OBJECT(lbl));


r = rect_assign(3, 45, 195, 65);
wx = textline_init(_malloc(sizeof(t_textline)),
                           r,
                           IO_DIR_LIMIT,
                           0);
OBJECT(w)->insert(OBJECT(w), OBJECT(wx));
TEXTLINE(wx)->set_text(TEXTLINE(wx), set_format_text(NULL,"%d",IMAGE_WIDTH(editorx->picture)));

r = rect_assign(200, 45, 397, 65);
hx = textline_init(_malloc(sizeof(t_textline)),
                           r,
                           IO_DIR_LIMIT,
                           0);
OBJECT(w)->insert(OBJECT(w), OBJECT(hx));
TEXTLINE(hx)->set_text(TEXTLINE(hx), set_format_text(NULL,"%d",IMAGE_HEIGHT(editorx->picture)));

r = rect_assign(3, 101, 100, 117);
b = button_init(_malloc(sizeof(t_button)),
                      r,
                      INI_TEXT("Ok"),
                      MSG_OK, // Msg
                      BF_DEFAULT);
OBJECT(w)->insert(OBJECT(w), OBJECT(b));

r = rect_assign(103, 101, 203, 117);
b = button_init(_malloc(sizeof(t_button)),
                      r,
                      INI_TEXT("Undo"),
                      MSG_CLOSE, // Msg
                      BF_DEFAULT);
OBJECT(w)->insert(OBJECT(w), OBJECT(b));

msg = desktop->execute_view(desktop, VIEW(w));

  if (msg == MSG_OK) {
    t_point t = point_assign(atoi(wx->text),atoi(hx->text));
    editorx->picture=resizebmp(editorx->picture,t.x,t.y);
    view_draw_me(VIEW(editorx));
    imgedit_undo_add(editorx);
  };

dispose(OBJECT(w));
} else {
        msgbox( MW_INFO, MB_OK, INI_TEXT("You can just resize a picture ... not a selection !"));

};
};
////////////////////////////////////////////////////////////////////////////////
void    redraw_title ( void )
{

  if ( xwin && editorx ) {

      if ( WINDOW(xwin)->caption ) _free(WINDOW(xwin)->caption);

      WINDOW(xwin)->caption = set_format_text(NULL, "Seal Image Manipulation Program - %s", editorx->filename);

      TEST_SUB_VIEWS(VIEW(xwin), WINDOW(xwin)->draw_title(WINDOW(xwin)));

  };

};
////////////////////////////////////////////////////////////////////////////////

  static void trans_ev ( p_object o, p_event event )
{

  if ( o->process == PH_PREPROCESS && event->type & EV_MESSAGE ) { /* if an event */
  l_dword msg = event->message;

  t_event saved;

  memcpy(&saved,event,sizeof(t_event));

  clear_event (event);

  if ( MSG_TOOL_MIN <= msg &&  msg <= MSG_TOOL_MAX ) {
    l_long id = msg - MSG_TOOL_MIN;
    editorx->tool = id;

  } else if ( MSG_FILTERS_MIN <= msg && msg <= MSG_FILTERS_MAX ) {
    l_int a = msg - MSG_FILTERS_MIN;

    getimgfilter(a)->filterbox(editorx->picture);
    view_draw_me(VIEW(editorx));
    imgedit_undo_add(editorx);
  } else
  switch ( msg ) {
    case MSG_TOOL_OPEN : {
      l_text file = open_dialog(actual_path, "*.*", get_file_items());
      if ( file ) {
        editorx->load(editorx, file);
        actual_path = io_path(file);
        _free(file);
        redraw_title();
      };
    }; break;
    case MSG_TOOL_SAVEAS : {
      l_text file = save_dialog(actual_path, "*.*", get_file_items());
      if ( file ) {
        editorx->load(editorx, file);
        actual_path = io_path(file);
        _free(file);
        redraw_title();
      }
    }; break;
    case MSG_TOOL_SAVE : {
      l_text file = editorx->filename;
      if ( file ) {
        if ( !stricmp(file,INI_TEXT("New Image")) ) file = save_dialog(actual_path, "*.*", get_file_items());
        editorx->save(editorx, file, 0);
        actual_path = io_path(file);
        _free(file);
        redraw_title();
      };
    }; break;
    case MSG_TOOL_NEW : {
      l_int w = NULL;
      l_int h = NULL;
      l_text t = inputbox (INI_TEXT("New picture ..."),INI_TEXT("Width ?"),"300");
      if (!t) return;
      w = atoi(t);
      _free(t);
      t = inputbox (INI_TEXT("New picture ..."),INI_TEXT("Height ?"),"300");
      if (!t) return;
      h = atoi(t);
      editorx->newbmp(editorx,w,h);
      _free(t);
      redraw_title();
    }; break;
    case MSG_TOOL_BORDER_P : {
      editorx->border+=2;
      imgedit_update_status(editorx);
    }; break;
    case MSG_TOOL_BORDER_M : {
      if ( editorx->border > 1 ) {
        editorx->border-=2;
        imgedit_update_status(editorx);
      };
    }; break;
    case MSG_TOOL_SET_TRANS_VALUE : {
      l_int a = NULL;
      l_text t = inputbox (INI_TEXT("Transparent"),INI_TEXT("Value ? (0-100) "),set_format_text(NULL,"%d",editorx->floodlevel));
      if (!t) return;
      a = atoi(t);
      editorx->floodlevel = a;
       _free(t);
    }; break;
    case MSG_TOOL_SET_TOLERANCE : {
      l_int b = 0;
      l_text t = inputbox (INI_TEXT("Flood Fill"),INI_TEXT("Flood tolerance (0-255) "),set_format_text(NULL,"%d",editorx->floodtol));
      if (!t) return;
      b = atoi(t);
      if ((b < 0) || (b > 255)) return;
      editorx->floodtol= b;
      _free(t);
    }; break;
    case MSG_TOOL_SET_CENTER : {
      if ( editorx->floodstyle == 1 ) {
        l_int b = 0;
        l_text t = inputbox (INI_TEXT("Flood Fill"),INI_TEXT("Angle (degree) (0-360) "),set_format_text(NULL,"%d",editorx->set1));
        if (!t) return;
        b = atoi(t);
        if ((b < 0) || (b > 360)) return;
        editorx->set1= b;
        _free(t);
      } else if ( editorx->floodstyle != 0 ){
        l_int b = 0;
        l_text t = inputbox (INI_TEXT("Flood Fill"),INI_TEXT("Center x (0-100) "),set_format_text(NULL,"%d",editorx->set1));
        if (!t) return;
        b = atoi(t);
        if ((b < 0) || (b > 100)) return;
        editorx->set1= b;
        t = inputbox (INI_TEXT("Flood Fill"),INI_TEXT("Center y (0-100) "),set_format_text(NULL,"%d",editorx->set2));
        if (!t) return;
        b = atoi(t);
        if ((b < 0) || (b > 100)) return;
        editorx->set2= b;
        _free(t);
      } else {
        msgbox( MW_INFO, MB_OK, INI_TEXT("No options"));
      };
    }; break;
    case MSG_TOOL_UNDO  : imgedit_undo( editorx ); break;
    case MSG_TOOL_CFONT : msgbox( MW_INFO, MB_OK, "Not yet supported ..."); break;
    case MSG_TOOL_COPY  : imgedit_pcopy_act ( editorx , PCOPY_COPY ); break;
    case MSG_TOOL_PASTASS : imgedit_pcopy_act ( editorx , PCOPY_PASTASNEWSEL ); break;
    case MSG_TOOL_PASTASP : imgedit_pcopy_act ( editorx , PCOPY_PASTASNEWPIC ); break;
    case MSG_xABOUT :
      msgbox( MW_INFO, MB_OK, "Seal Image Manipulation Program 0.1\n    by Julien Etelain <julien@pmad.net>\n\n(c) copyright 2001 Julien Etelain . All rights reserved");
    break;
    case MSG_xHELP : msgbox( MW_INFO, MB_OK, "Not yet supported ..."); break;
    case MSG_TOOL_RESIZE : resizebox(); break;
    case MSG_HISUPDATE : {
      editorx->penstyle   = pen_forme->current;
      editorx->floodmode  = pen_trans->current;
      editorx->fillmode   = forme_fill->current;
      editorx->forme      = forme_forme->current;
      editorx->floodstyle = ffill_gardient->current;
    }; break;
    default : memcpy(event,&saved,sizeof(t_event)); break;
    };
  };
};
////////////////////////////////////////////////////////////////////////////////
static t_point  xcolors_minimum ( p_view o )
{
   return point_assign(20, 20);
};
////////////////////////////////////////////////////////////////////////////////
void loadtoolbox( t_rect r, l_int w ) {

  p_icobutton bb = NULL;

  p_button b = NULL;

conf_tab = tabbook_make(rect_assign(r.b.x-w-5, r.a.y+45, r.b.x-5, r.a.y+160));
conf_tab->add_tab(conf_tab,"pen",NULL);
conf_tab->add_tab(conf_tab,"forme",NULL);
conf_tab->add_tab(conf_tab,"flood",NULL);
OBJECT(xwin)->insert(OBJECT(xwin), OBJECT(conf_tab ));
r = rect_assign(3, 3, w-20, 23);
pen_forme = history_init(_malloc(sizeof(t_history)),
                            r,
                            pen_forme_list() ,
                            150,
                            HF_REWRITEUNABLE|LF_SELFLIST);
pen_forme->message = MSG_HISUPDATE;
conf_tab->insert(conf_tab,0, OBJECT(pen_forme));

r = rect_assign(3, 24, w-20, 44);
pen_trans = history_init(_malloc(sizeof(t_history)),
                            r,
                            pen_trans_list() ,
                            150,
                            HF_REWRITEUNABLE|LF_SELFLIST);
pen_trans->message = MSG_HISUPDATE;
conf_tab->insert(conf_tab,0, OBJECT(pen_trans));
r = rect_assign(3, 46, w-3, 66);
b = button_init(_malloc(sizeof(t_button)),
                            r,
                            INI_TEXT("Set trans value"),
                            MSG_TOOL_SET_TRANS_VALUE,
                            BF_DEFAULT);
conf_tab->insert(conf_tab,0, OBJECT(b));
r = rect_assign(3, 68, 50, 88);
bb = icobutton_init( _malloc(sizeof(t_icobutton)),
                            r,
                            Get_Bitmap(BORDER_P),
                            INI_TEXT("Border +"),
                            MSG_TOOL_BORDER_P,
                            BF_PUSH);
conf_tab->insert(conf_tab,0, OBJECT(bb));
r = rect_assign(53, 68, 103, 88);
bb = icobutton_init( _malloc(sizeof(t_icobutton)),
                            r,
                            Get_Bitmap(BORDER_M),
                            INI_TEXT("Border -"),
                            MSG_TOOL_BORDER_M,
                            BF_PUSH);
conf_tab->insert(conf_tab,0,OBJECT(bb));

r = rect_assign(3, 3, w-20, 23);
forme_forme = history_init(_malloc(sizeof(t_history)),
                            r,
                            forme_forme_list() ,
                            150,
                            HF_REWRITEUNABLE|LF_SELFLIST);
forme_forme->message = MSG_HISUPDATE;
conf_tab->insert(conf_tab,1, OBJECT(forme_forme));
/////
r = rect_assign(3, 24, w-20, 44);
forme_fill = history_init(_malloc(sizeof(t_history)),
                            r,
                            forme_fill_list() ,
                            150,
                            HF_REWRITEUNABLE|LF_SELFLIST);
forme_fill->message = MSG_HISUPDATE;
conf_tab->insert(conf_tab,1, OBJECT(forme_fill));
r = rect_assign(3, 45, 50, 65);
bb = icobutton_init( _malloc(sizeof(t_icobutton)),
                            r,
                            Get_Bitmap(BORDER_P),
                            INI_TEXT("Border +"),
                            MSG_TOOL_BORDER_P,
                            BF_PUSH);
conf_tab->insert(conf_tab,1, OBJECT(bb));
r = rect_assign(53, 45, 103, 65);
bb = icobutton_init( _malloc(sizeof(t_icobutton)),
                            r,
                            Get_Bitmap(BORDER_M),
                            INI_TEXT("Border -"),
                            MSG_TOOL_BORDER_M,
                            BF_PUSH);
conf_tab->insert(conf_tab,1, OBJECT(bb));
/////
r = rect_assign(3, 3, w-20, 23);
ffill_gardient = history_init(_malloc(sizeof(t_history)),
                            r,
                            ffill_gardient_list() ,
                            150,
                            HF_REWRITEUNABLE|LF_SELFLIST);
ffill_gardient->message = MSG_HISUPDATE;
conf_tab->insert(conf_tab,2, OBJECT(ffill_gardient));
r = rect_assign(3, 24, w-5, 44);
b = button_init(_malloc(sizeof(t_button)),
                            r,
                            INI_TEXT("Set tolerance"),
                            MSG_TOOL_SET_TOLERANCE,
                            BF_DEFAULT);
conf_tab->insert(conf_tab,2, OBJECT(b));
r = rect_assign(3, 46, w-5, 66);
b = button_init(_malloc(sizeof(t_button)),
                            r,
                            INI_TEXT("Gardient options"),
                            MSG_TOOL_SET_CENTER,
                            BF_DEFAULT);
conf_tab->insert(conf_tab,2, OBJECT(b));



};
////////////////////////////////////////////////////////////////////////////////
void init_window ( void ) {
  t_rect   r = rect_assign(0, 0, 755, 525);
  xwin = appwin_init(_malloc(sizeof(t_appwin)),
                            r,
                            _strdup("SIMP - Seal Image Manupilation Program"),
                            WF_MINIMIZE,
                            ap_id,
                            &trans_ev);

  if ( xwin ) VIEW(xwin)->align |= TX_ALIGN_CENTER;

  xwin->icon16 = load_image("/system/bmp/simp.ico,16");

  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(xwin));

  if ( xwin ) {
    p_menuview menu = NULL;
    l_long   a = 0;
    p_icobutton bb =NULL;
    l_int    w = 110;
    menu = hormenu_init(_malloc(sizeof(t_menuview)),
                       rect_assign(r.a.x+5,r.a.y+20,r.b.x-5,r.a.y+40),
                       editimg_menu_init());

    OBJECT(xwin)->insert(OBJECT(xwin), OBJECT(menu));

    sto = stattext_init(_malloc(sizeof(t_stattext)),rect_assign(r.a.x+5, r.b.y-20, r.b.x-5, r.b.y-5),TX_ALIGN_LEFT,"Loading... Please Wait...");
    OBJECT(xwin)->insert(OBJECT(xwin), OBJECT(sto));

    while ( a < 8 ) {

       bb = icobutton_init( _malloc(sizeof(t_icobutton)), /* memory for class t_button */
                            rect_assign(r.a.x+5,r.a.y+45+(a*20),r.a.x+20,r.a.y+65+(a*20)), /* button area */
                            Get_Bitmap(txttoolint[a]),
                            txttool[a],
                            MSG_TOOL_MIN + a,    /* message of the button */
                            BF_PUSH);

      OBJECT(xwin)->insert(OBJECT(xwin), OBJECT(bb));

      a++;
    };

    loadtoolbox(r,w);

    cco = colsel2_init( _malloc(sizeof(t_colsel2)), rect_assign(r.b.x-w-5, r.a.y+165 , r.b.x-5, r.a.y+230) );
    OBJECT(xwin)->insert(OBJECT(xwin), OBJECT(cco));

    editorx = imgedit_init(_malloc(sizeof(t_imgedit)), rect_assign( r.a.x+25, r.a.y+45, r.b.x-w-10, r.b.y-25) , cco, sto);
    OBJECT(xwin)->insert(OBJECT(xwin), OBJECT(editorx));
  };
};

////////////////////////////////////////////////////////////////////////////////

SetInfoAppName("SIMP");
SetInfoDesciption("Seal Image Manipulation Image");
SetInfoCopyright("(c) Copyright 2001 Point Mad. All rights reserved");
SetInfoManufacturer("Point Mad");

app_begin ( void ) {

  if ( ap_process == AP_INIT ) {

      AP_SETNUMOFCALLS(1);

      dat = conv_to_skipcolor_data(DLXGetFileData(ap_id), CO_SKIP_RED, CO_SKIP_GREEN, CO_SKIP_BLUE);

      imgedit_xdl_init();

      init_window();

      if (ap_args != NULL ) if ( io_exist(ap_args) ) editorx->load(editorx,ap_args);

      redraw_title();

  };

  if ( ap_process == AP_DONE ) {

  };

} lib_end;
