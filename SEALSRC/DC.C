/****************************************************************/
/*                                                              */
/*                            dc.c                              */
/*                                                              */
/*                    disk commander for Seal                   */
/*                                                              */
/*                Seal - free desktop environment               */
/*                                                              */
/*                    Copyright (c) 1999,2000                   */
/*                         Michal Stencl                        */
/*                     All Rights Reserved                      */
/*                                                              */
/* mail : stenclpmd@ba.telecom.sk                               */
/* web  : http://www.home.sk/public/seal/                       */
/*                                                              */
/* This file is part of Seal.                                   */
/*                                                              */
/* Seal is free software; you can redistribute it and/or        */
/* modify it under the terms of the GNU General Public License  */
/* as published by the Free Software Foundation; either version */
/* 2, or (at your option) any later version.                    */
/*                                                              */
/* Seal is distributed in the hope that it will be useful, but  */
/* WITHOUT ANY WARRANTY; without even the implied warranty of   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See    */
/* the GNU General Public License for more details.             */
/*                                                              */
/* You should have received a copy of the GNU General Public    */
/* License along with Seal; see the file COPYING.  If not,      */
/* write to the Free Software Foundation, 675 Mass Ave,         */
/* Cambridge, MA 02139, USA.                                    */
/*                                                              */
/****************************************************************/



#include"allegro.h"
#include"seal.h"
#include"menus.h"
#include"app.h"
#include"button.h"
#include"dialogs.h"
#include"iodlg.h"

l_text dc_standard_path1 = "c:/";
l_text dc_standard_path2 = "c:/";

#define MSG_VIEWF   10001
#define MSG_EDITF   10002
#define MSG_COPYF   10003
#define MSG_RENAMEF 10004
#define MSG_MKDIRF  10005
#define MSG_DELETEF 10006

#define DC_SIZEX   640
#define DC_SIZEY   480

#ifndef TXT_CANTRUNFILE
#define TXT_CANTRUNFILE        INI_TEXT("Can't run file")
#endif


p_textline    text_line = NULL;
p_filelistbox box1 = NULL;
p_filelistbox box2 = NULL;


l_bool left = true;

static void trans_ev ( p_object o, t_event *e )
{
  RETVIEW(o, e);

  if ( o->phase == PH_PREPROCESS && e->type & EV_KEYBOARD ) {

       if ( keyb->code == TO_KEY(KB_ENTER) &&
            OBJECT(text_line)->is_state(OBJECT(text_line), OB_SF_FOCUSED) ) {

           if ( !run_file(text_line->text) )

               seal_error(ERR_INFO, "%s\n\n%s", TXT_CANTRUNFILE, text_line->text);

            clear_event(e);

       };

       if ( keyb->code == TO_KEY(KB_TAB) ) { /* select next view */

            clear_event(e);

            if ( left ) OBJECT(box2)->select(OBJECT(box2));
            else OBJECT(box1)->select(OBJECT(box1));

       };

       if ( keyb->code == TO_KEY(KB_F3) ) { // view

            clear_event(e);

            e->type = EV_MESSAGE;
            e->message = MSG_VIEWF;

       };

       if ( keyb->code == TO_KEY(KB_F4) ) { // edit


            clear_event(e);

            e->type = EV_MESSAGE;
            e->message = MSG_EDITF;
       };

       if ( keyb->code == TO_KEY(KB_F5) ) { // copy


            clear_event(e);

            e->type = EV_MESSAGE;
            e->message = MSG_COPYF;
       };

       if ( keyb->code == TO_KEY(KB_F6) ) { // rename


            clear_event(e);

            e->type = EV_MESSAGE;
            e->message = MSG_RENAMEF;
       };

       if ( keyb->code == TO_KEY(KB_F7) ) { // make directory


            clear_event(e);

            e->type = EV_MESSAGE;
            e->message = MSG_MKDIRF;
       };

       if ( keyb->code == TO_KEY(KB_F8) ) { // delete


            clear_event(e);

            e->type = EV_MESSAGE;
            e->message = MSG_DELETEF;
       };


  };



  if ( o->phase == PH_POSTPROCESS && e->type & EV_MESSAGE ) {

      switch ( e->message ) {

          case MSG_EDITF :
          case MSG_VIEWF : {

               p_listbox x = left?LISTBOX(box1):LISTBOX(box2);

               l_text file = FILELISTBOX_ITEM_FILENAME(x->list, x->current);
               l_text path = FILELISTBOX_ITEM_PATH(x->list, x->current);

               l_text runfile = io_realpath(path, file);
               l_text lf = io_sealfile(runfile);

               if ( !run_file_args("./programs/apps/smalldit.exe", lf) )

                   seal_error(ERR_INFO, "%s\n\n%s", TXT_CANTRUNFILE, lf);

               _free(runfile);
               _free(lf);

               clear_event(e);

          }; break;

          case MSG_COPYF : {

               if ( left ) {

                  message(OBJECT(box1), EV_MESSAGE, MSG_COPY, OBJECT(box1));
                  message(OBJECT(box2), EV_MESSAGE, MSG_PASTE, OBJECT(box2));

               } else {

                  message(OBJECT(box2), EV_MESSAGE, MSG_COPY, OBJECT(box2));
                  message(OBJECT(box1), EV_MESSAGE, MSG_PASTE, OBJECT(box1));

               };

               clear_event(e);

          }; break;

          case MSG_RENAMEF : {


               if ( left ) message(OBJECT(box1), EV_MESSAGE, MSG_CHANGENAME, OBJECT(box1));
               else message(OBJECT(box2), EV_MESSAGE, MSG_CHANGENAME, OBJECT(box2));

               clear_event(e);

          }; break;

          case MSG_MKDIRF : {


               if ( left ) message(OBJECT(box1), EV_MESSAGE, MSG_NEWDIRECTORY, OBJECT(box1));
               else message(OBJECT(box2), EV_MESSAGE, MSG_NEWDIRECTORY, OBJECT(box2));

               clear_event(e);

          }; break;

          case MSG_DELETEF : {

               if ( left ) message(OBJECT(box1), EV_MESSAGE, MSG_CUT, OBJECT(box1));
               else message(OBJECT(box2), EV_MESSAGE, MSG_CUT, OBJECT(box2));

               clear_event(e);

          }; break;


      };


  };


};


l_bool     dcfilelistbox_select ( p_object o ) {

    l_bool ok = obj_select(o);

    if ( ok ) if ( o == OBJECT(box1) ) left = true; else

              if ( o == OBJECT(box2) ) left = false;

    return ok;

};


void  dcfilelistbox_translate_event ( p_object o, p_event event )
{

   RETVIEW(o, event);

   /* call old function from iodlg.h */

   filelistbox_translate_event(o, event);


   if ( event->type & EV_MESSAGE ) { /* */

       switch ( event->message ) {

           case MSG_OK : { /* enter on file */

                 l_text file = FILELISTBOX_ITEM_FILENAME(LISTBOX(o)->list, LISTBOX(o)->current);
                 l_text path = FILELISTBOX_ITEM_PATH(LISTBOX(o)->list, LISTBOX(o)->current);

                 l_text runfile = io_realpath(path, file);
                 l_text lf = io_sealfile(runfile);

                 clear_event(event);

                 if ( !run_file (lf) )

                     seal_error(ERR_INFO, "%s\n\n%s", TXT_CANTRUNFILE, lf);

                 _free(runfile);
                 _free(lf);

           }; break;

       };

   };

};


static t_point  dc_size_minimum ( p_view o )
{
   return point_assign(DC_SIZEX/2+100, 300);
};


void  init_dc ( void )
{

   t_rect r = rect_assign(0, 0, DC_SIZEX, DC_SIZEY);
   t_rect t = r;
   t_rect s = r;

   p_button b = NULL;

   p_dirhistory  dir1;
   p_dirhistory  dir2;

   p_appwin o = appwin_init(_malloc(sizeof(t_appwin)), /* make window */
                            r,
                            "Disk Commander 1.0",
                            WF_MINIMIZE,
                            ap_id, /* application id */
                            &trans_ev);

   VIEW(o)->drag_mode |= DM_DRAGGROW; /* able to grow view */

   VIEW(o)->align |= TX_ALIGN_CENTER; /* set alignment */

   VIEW(o)->size_minimum = &dc_size_minimum;

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(o)); /* insert it on the desktop */

   r = VIEW(o)->size_limits(VIEW(o));

   s = r = rect_assign(r.a.x+10, r.a.y+10, t.b.x-10, r.b.y-110);

   r = rect_assign(r.a.x, r.a.y, r.a.x+(DC_SIZEX/2)-20, r.b.y);

   t = r;

   r.a.y += 30;

   box1 = filelistbox_init(_malloc(sizeof(t_filelistbox)), r, 2, LF_SELECTABLE|FL_OPEN, dc_standard_path1, "*.*", FA_ALL, NULL);

   if ( box1 ) {
        /* redeclare translate event function */
        OBJECT(box1)->translate_event = &dcfilelistbox_translate_event;

        OBJECT(box1)->select = &dcfilelistbox_select;

        /* redeclare alignment of filelistbox 1 */
        VIEW(box1)->align |= TX_ALIGN_BOTTOM;
   };

   r = rect_move(r, (DC_SIZEX/2)-10, 0);

   box2 = filelistbox_init(_malloc(sizeof(t_filelistbox)), r, 2, LF_SELECTABLE|FL_OPEN, dc_standard_path2, "*.*", FA_ALL, NULL);

   if ( box2 ) {
        /* redeclare translate event function */
        OBJECT(box2)->translate_event = &dcfilelistbox_translate_event;

        OBJECT(box2)->select = &dcfilelistbox_select;

        /* redeclare alignment of filelistbox 1 */
        VIEW(box2)->align |= TX_ALIGN_RIGHT+TX_ALIGN_BOTTOM;
   };

   r = rect_assign(t.a.x, t.a.y, t.a.x+200, t.a.y+20);

   /* make link for box1 */
   dir1 = dirhistory_init(_malloc(sizeof(t_dirhistory)), r, IO_TEXT_LIMIT, HF_REWRITEUNABLE, box1);

   r = rect_assign(t.a.x+(DC_SIZEX/2)-10, r.a.y, t.a.x+(DC_SIZEX/2)+190, r.b.y);

   /* make link for box2 */
   dir2 = dirhistory_init(_malloc(sizeof(t_dirhistory)), r, IO_TEXT_LIMIT, HF_REWRITEUNABLE, box2);

   if ( dir2 )
        /* redeclare alignment of filelistbox 1 */
        VIEW(dir2)->align |= TX_ALIGN_RIGHT;

   OBJECT(o)->insert(OBJECT(o), OBJECT(box1)); /* insert it on the desktop */

   OBJECT(o)->insert(OBJECT(o), OBJECT(dir1)); /* insert it on the desktop */

   OBJECT(o)->insert(OBJECT(o), OBJECT(box2)); /* insert it on the desktop */

   OBJECT(o)->insert(OBJECT(o), OBJECT(dir2)); /* insert it on the desktop */


   s = rect_assign(s.a.x, s.b.y+30, s.b.x-5, s.b.y+50);

   text_line = textline_init(_malloc(sizeof(t_textline)), s, IO_FILE_LIMIT, 0);

   VIEW(text_line)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_FIXEDY|TX_ALIGN_RIGHT;

   OBJECT(o)->insert(OBJECT(o), OBJECT(text_line));


   r = rect_assign(s.a.x, s.b.y+20, s.a.x+110, s.b.y+40);

   b = button_init(_malloc(sizeof(t_button)), r, "F3 View", MSG_VIEWF, BF_NORMAL );

   VIEW(b)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_FIXEDY;

   OBJECT(o)->insert(OBJECT(o), OBJECT(b));


   r = rect_assign(r.b.x+10, r.a.y, r.b.x+100, r.b.y);

   b = button_init(_malloc(sizeof(t_button)), r, "F4 Edit", MSG_EDITF, BF_NORMAL );

   VIEW(b)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_FIXEDY;

   OBJECT(o)->insert(OBJECT(o), OBJECT(b));


   r = rect_assign(r.b.x+10, r.a.y, r.b.x+100, r.b.y);

   b = button_init(_malloc(sizeof(t_button)), r, "F5 Copy", MSG_COPYF, BF_NORMAL );

   VIEW(b)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_FIXEDY;

   OBJECT(o)->insert(OBJECT(o), OBJECT(b));


   r = rect_assign(r.b.x+10, r.a.y, r.b.x+100, r.b.y);

   b = button_init(_malloc(sizeof(t_button)), r, "F6 Rename", MSG_RENAMEF, BF_NORMAL );

   VIEW(b)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_FIXEDY;

   OBJECT(o)->insert(OBJECT(o), OBJECT(b));


   r = rect_assign(r.b.x+10, r.a.y, r.b.x+100, r.b.y);

   b = button_init(_malloc(sizeof(t_button)), r, "F7 MkDir", MSG_MKDIRF, BF_NORMAL );

   VIEW(b)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_FIXEDY;

   OBJECT(o)->insert(OBJECT(o), OBJECT(b));


   r = rect_assign(r.b.x+10, r.a.y, r.b.x+100, r.b.y);

   b = button_init(_malloc(sizeof(t_button)), r, "F8 Delete", MSG_DELETEF, BF_NORMAL );

   VIEW(b)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_FIXEDY;

   OBJECT(o)->insert(OBJECT(o), OBJECT(b));

};


app_begin ( void ) {


  if ( ap_process == AP_INIT ) {

      init_dc();

  };

  if ( ap_process == AP_FREE ) {

  };

} app_end;

