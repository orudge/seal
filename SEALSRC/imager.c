/****************************************************************/
/*                                                              */
/*                           imager.c                           */
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

/*

12.01.2001 Florian Xaver

25.12.2000 Florian Xaver
+ support of PNG files
+ added "convert to ..." into the menu
+ added scrollbars to show all of the pictures
*/

#include"allegro.h"
#include"seal.h"
#include"app.h"
#include"button.h"
#include"dialogs.h"
#include"iodlg.h"
#include"scroller.h"
#include"iodlg.h"

#define MSG_CONVERT 10007

#ifndef TXT_CONVERTTOIMAGE
#define TXT_CONVERTTOIMAGE  INI_TEXT("Convert to image...")
#endif
#ifndef TXT_VIEWFILE
#define TXT_VIEWFILE       INI_TEXT("View/Open")
#endif
#ifndef TXT_INFOVIEWFILE
#define TXT_INFOVIEWFILE   INI_TEXT("Views the file in the right window or open a directory")
#endif

#ifndef TXT_INFOCONVERTTOIMAGE
#define TXT_INFOCONVERTTOIMAGE  INI_TEXT("Here you can convert this file format into another file format")
#endif
#ifndef TXT_CONVERTNOW
#define TXT_CONVERTNOW      INI_TEXT("Convert now !")
#endif
#ifndef TXT_CLOSEBYALTF4
#define TXT_CLOSEBYALTF4    INI_TEXT("Close By Alt+F4")
#endif
#ifndef TXT_IMAGEISCONVRTINGINTOFILE
#define TXT_IMAGEISCONVRTINGINTOFILE   INI_TEXT("Image is converting into file")
#endif
#ifndef TXT_PLEASEWAIT
#define TXT_PLEASEWAIT   INI_TEXT("Please wait...")
#endif

#define STANDARD_PATH  "c:/"
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

extern void  filelistbox_translate_event ( p_object o, p_event event );

p_filelistbox box1   = NULL;
p_view        imager = NULL;
p_scroller    sc     = NULL;

p_list get_imager_filelist ( void );


l_dword dcfilelistbox_file_menu ( p_filelistbox o )
{

  p_filelistbox_item item = NULL;

  item = LISTBOX_ITEM_AT(LISTBOX(o)->list, LISTBOX(o)->current);

  if ( item ) {

      l_int fatr = item->info.attrib;

      p_menu p = new_menu(

                 new_menu_item_ex(TXT_VIEWFILE, NULL, 0, MSG_OK, true,
                                  TXT_INFOVIEWFILE, MIF_NONE, font_system_bd, NULL, NULL, 0,
                 new_menu_item(TXT_CONVERTTOIMAGE, NULL, 0, MSG_CONVERT, TXT_INFOCONVERTTOIMAGE,
                 new_menu_line(
                 new_menu_item(TXT_CHANGENAME, NULL, 0, MSG_CHANGENAME, TXT_INFOCHANGENAME,
                 new_menu_line(
                 new_menu_item(TXT_COPY, NULL, 0, MSG_COPY, TXT_INFOCOPY,
                 new_menu_item(TXT_DELETE, NULL, 0, MSG_CUT, TXT_INFODEL,
                 new_menu_item(TXT_PASTE, NULL, 0, MSG_PASTE, TXT_INFOPASTE,
                 new_menu_line(
                 new_menu_item(TXT_PROPERTIES, NULL, 0, MSG_PROPERTIES, TXT_INFOPROPERTIES,
                 new_menu_line(
                 new_sub_menu(TXT_NEW, new_menu(

                     new_menu_item(TXT_DIRECTORY, NULL, 0, MSG_NEWDIRECTORY, TXT_INFONEWDIRECTORY,
                     new_menu_item(TXT_LINKTOFILE, NULL, 0, MSG_NEWLINKTOFILE, TXT_INFOLINKTOFILE,
                     new_menu_item(TXT_LINKTODIRECTORY, NULL, 0, MSG_NEWLINKTODIRECTORY, TXT_INFOLINKTODIRECTORY,
                     NULL)))),



                
                NULL))))))))))))
             );

      if ( p ) {

         t_rect   r = rect_assign(mouse->where.x-5, mouse->where.y-5, 0, 0);

         p_object menu = OBJECT(menuview_init(
                                _malloc(sizeof(t_menuview)),
                                r,
                                p)
                         );


         return desktop->execute_view(desktop, VIEW(menu));

      };
  };

  return MSG_CANCEL;
};





l_bool  make_image ( l_text filename )
{
    if ( VIEW(imager)->brush.background )
         destroy_bitmap(VIEW(imager)->brush.background);

    VIEW(imager)->brush.background = load_image(filename);

    if ( VIEW(imager)->brush.background ) {

      VIEW(imager)->grow_view(VIEW(imager),rect_assign(0, 0, (VIEW(imager)->brush.background)->w, (VIEW(imager)->brush.background)->h));

      reset_scroller(SCROLLER(sc));
      TEST_SUB_VIEWS(VIEW(sc), VIEW(sc)->draw_view(VIEW(sc)););
      draw_scroller_bars(SCROLLER(sc));


    } else
    {

    return false;

    }

    return true;
};


void  dcfilelistbox_translate_event ( p_object o, p_event event )
{

   RETVIEW(o, event);

   /* call old function from iodlg.h */

   filelistbox_translate_event(o, event);

   if ( event->type & EV_MESSAGE ) { /* message event */

      switch ( event->message ) {

         case MSG_CONVERT: {

         l_text convert_to_file = NULL;

         l_text file = FILELISTBOX_ITEM_FILENAME(LISTBOX(o)->list, LISTBOX(o)->current);
         l_text path = FILELISTBOX_ITEM_PATH(LISTBOX(o)->list, LISTBOX(o)->current);

         l_text runfile = io_realpath(path, file);
         l_text lf = io_sealfile(runfile);
         

         clear_event(event);

         

         convert_to_file = file_dialog(TXT_CONVERTTOIMAGE, TXT_CONVERTNOW, "c:/", "*.gif", get_imager_filelist(), FA_ALL, FL_SAVE, NULL);

         if ( convert_to_file ) { /* no canceled dialog */


             l_text path = io_path(convert_to_file);

             void *p = show_info(0, MW_INFO, "%s...\n\n%s %s...", TXT_PLEASEWAIT, TXT_IMAGEISCONVRTINGINTOFILE, convert_to_file );

             save_bitmap(convert_to_file, load_image(lf), NULL);

             hide_info(p);

             RETYPE_FILES_IN_PATH(path); /* retype files is directory path,
                                            it's for redrawing listbox with
                                            files from dir path */

             _free(path);
             _free(runfile);
             _free(lf);
             _free(convert_to_file);
             

             break;
             };

             };

         };

   };
   


   if ( event->type & EV_MESSAGE ) { /* */

       switch ( event->message ) {

           case MSG_OK : { /* enter on file */

                 l_text file = FILELISTBOX_ITEM_FILENAME(LISTBOX(o)->list, LISTBOX(o)->current);
                 l_text path = FILELISTBOX_ITEM_PATH(LISTBOX(o)->list, LISTBOX(o)->current);

                 l_text runfile = io_realpath(path, file);
                 l_text lf = io_sealfile(runfile);

                 clear_event(event);



                 if ( !make_image (lf) )

                     seal_error(ERR_INFO, "%s\n\n%s", INI_TEXT("Can't show image !"), lf);

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


void  imager_scroll_size ( p_scroller o, l_long *x, l_long *y )
{

      t_rect r;

      if (imager)

        {

         r = VIEW(imager)->get_local_extent(VIEW(imager));
         if ( x ) *x = r.b.x;
         if ( y ) *y = r.b.y;


        }

};








void  init_imager ( void )
{

   t_rect r = rect_assign(0, 0, DC_SIZEX, DC_SIZEY);
   t_rect s = r;
   t_rect t = r;



   p_button b = NULL;


   p_dirhistory  dir1;


   p_appwin o = appwin_init(_malloc(sizeof(t_appwin)), /* make window */
                            r,
                            "Imager 1.0",
                            WF_MINIMIZE,
                            ap_id, /* application id */
                            NULL);

   VIEW(o)->drag_mode |= DM_DRAGGROW; /* able to grow view */

   VIEW(o)->align |= TX_ALIGN_CENTER; /* set alignment */

   VIEW(o)->size_minimum = &dc_size_minimum;

   r = VIEW(o)->size_limits(VIEW(o));

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(o)); /* insert it on the desktop */      

   s = r = rect_assign(r.a.x+10, r.a.y+10, t.b.x-10, r.b.y-40);

   r = rect_assign(r.a.x, r.a.y, r.a.x+(DC_SIZEX/2)-40, r.b.y);

   t = r;

   r.a.y += 30;

   box1 = filelistbox_init(_malloc(sizeof(t_filelistbox)), r, 1, FL_OPEN, STANDARD_PATH, "*.*", FA_ALL, NULL);

   if ( box1 ) {
        /* redeclare translate event function */
        OBJECT(box1)->translate_event = &dcfilelistbox_translate_event;

        /* redeclare alignment of filelistbox 1 */
//        VIEW(box1)->align |= TX_ALIGN_BOTTOM;

        FILELISTBOX(box1)->file_menu = &dcfilelistbox_file_menu;

   };

   OBJECT(o)->insert(OBJECT(o), OBJECT(box1)); /* insert it on the desktop */      


    r = rect_move(r, (DC_SIZEX/2)-10, 0);

    sc = scroller_init(_malloc(sizeof(t_scroller)), r, SF_VERSCROLLBAR|SF_HORSCROLLBAR);

    SCROLLER(sc)->scroll_size = &imager_scroll_size;

    SCROLLER(sc)->deltax = 20;
    SCROLLER(sc)->deltay = 20;

    OBJECT(o)->insert(OBJECT(o), OBJECT(sc)); /* insert it on the desktop */

    r = rect_assign(0, 0, 10, 10);


    imager = view_init(_malloc(sizeof(t_view)), r);

    OBJECT(sc)->insert(OBJECT(sc), OBJECT(imager)); /* insert it on the desktop */

    VIEW(imager)->brush.state &= ~BRUSH_SELFIMG;    
    
    make_image ("start.jpg");


   r = rect_assign(t.a.x, t.a.y, t.a.x+200, t.a.y+20);

   /* make link for box1 */
   dir1 = dirhistory_init(_malloc(sizeof(t_dirhistory)), r, IO_TEXT_LIMIT, HF_REWRITEUNABLE, box1);

   OBJECT(o)->insert(OBJECT(o), OBJECT(dir1)); /* insert it on the desktop */







   


};


p_list get_imager_filelist ( void ) {

                                                /* function for free-ing items */
  p_list p = list_init(malloc(sizeof(t_list)), &free_filehistory_item, 0);

  if ( p ) {

     p->insert(p, new_filehistory_item("Gif files (*.gif)", "*.gif"));
     p->insert(p, new_filehistory_item("PNG files (*.png)", "*.png"));
     p->insert(p, new_filehistory_item("Jpeg files (*.jpg)", "*.jpg"));
     p->insert(p, new_filehistory_item("Bmp files (*.bmp)", "*.bmp"));
     p->insert(p, new_filehistory_item("Pcx files (*.pcx)", "*.pcx"));
     p->insert(p, new_filehistory_item("Tiff files (*.tif)", "*.tif"));
     p->insert(p, new_filehistory_item("Targa files (*.tga)", "*.tga"));
     p->insert(p, new_filehistory_item("Lbm files (*.lbm)", "*.lbm"));
     p->insert(p, new_filehistory_item("X Window Bitmap files (*.xbm)", "*.xbm"));
     p->insert(p, new_filehistory_item("Iax files (*.iax)", "*.iax"));
     p->insert(p, new_filehistory_item("Pse files (*.pse)", "*.pse"));
     p->insert(p, new_filehistory_item("Pixmap files (*.ppm)", "*.ppm"));
     p->insert(p, new_filehistory_item("Portable Greyscale-map files (*.pgm)", "*.pgm"));
     p->insert(p, new_filehistory_item("Vid files (*.vid)", "*.vid"));
     p->insert(p, new_filehistory_item("All files (*.*)", "*.*"));

  };


  return p;

};


app_begin ( void ) {

  if ( ap_process == AP_INIT ) {

    init_imager();

  } else

  if ( ap_process == AP_FREE ) {

/*    if ( image ) destroy_bitmap(image);

    image = NULL;*/

  };

} app_end;

