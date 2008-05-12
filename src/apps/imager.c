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

/*
 * Revision History:
 *
 *
 * 25/12/2000 - Florian Xaver:
 *              + support of PNG files
 *              + added "convert to ..." into the menu
 *              + added scrollbars to show all of the pictures
 *
 * 30/03/2002 - Added About dialog (orudge)
 * 04/04/2002 - Updated with new icon (orudge)
 */

#include <allegro.h>
#include <seal.h>
#include <app.h>
#include <button.h>
#include <dialogs.h>
#include <iodlg.h>
#include <scroller.h>
#include <iodlg.h>

#define MSG_CONVERT 10007

#ifndef TXT_CONVERTTOIMAGE
#define TXT_CONVERTTOIMAGE  INI_TEXT("Convert to Image...")
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
#define TXT_CONVERTNOW      INI_TEXT("Convert Now")
#endif
#ifndef TXT_CLOSEBYALTF4
#define TXT_CLOSEBYALTF4    INI_TEXT("Close by Alt+F4")
#endif
#ifndef TXT_IMAGEISCONVRTINGINTOFILE
#define TXT_IMAGEISCONVRTINGINTOFILE   INI_TEXT("Image is converting into file")
#endif
#ifndef TXT_PLEASEWAIT
#define TXT_PLEASEWAIT   INI_TEXT("Please wait...")
#endif

#define STANDARD_PATH  "/"
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
p_appwin      win    = NULL;

p_list get_imager_filelist ( void );


l_dword dcfilelistbox_file_menu ( p_filelistbox o )
{

  p_filelistbox_item item = NULL;

  item = LISTBOX_ITEM_AT(LISTBOX(o)->list, LISTBOX(o)->current);

  if ( item ) {

      l_int fatr = item->info.attrib;

      p_menu p = new_menu(

                 new_menu_item_ex(TXT_VIEWFILE, NULL, 0, MSG_OK, true,
                                  TXT_INFOVIEWFILE, MIF_NONE, font_system, NULL, NULL, 0,
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
                     NULL)),




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

{   BITMAP *bmp = NULL;

    bmp = load_image(filename);

    if ( bmp ) {
      if ( VIEW(imager)->brush.background ) destroy_bitmap(VIEW(imager)->brush.background);

      VIEW(imager)->brush.background = bmp;
      VIEW(imager)->grow_view(VIEW(imager),rect_assign(0, 0, bmp->w-1, bmp->h-1));

      reset_scroller(SCROLLER(sc));
      TEST_SUB_VIEWS(VIEW(sc), VIEW(sc)->draw_view(VIEW(sc)););
      draw_scroller_bars(SCROLLER(sc));

      if ( WINDOW(win)->caption ) _free(WINDOW(win)->caption);
      WINDOW(win)->caption = set_format_text(NULL, "Image Viewer - %s", filename);
      TEST_SUB_VIEWS(VIEW(win), WINDOW(win)->draw_title(WINDOW(win)));

      return true;
    };

    return false;
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



         convert_to_file = file_dialog(TXT_CONVERTTOIMAGE, TXT_CONVERTNOW, STANDARD_PATH, "*.bmp", get_imager_filelist(), FA_ALL, FL_SAVE, NULL);

         if ( convert_to_file ) { /* no canceled dialog */


             l_text path = io_path(convert_to_file);

             void *p = show_info(0, MW_INFO, "%s...\n\n%s %s...", TXT_PLEASEWAIT, TXT_IMAGEISCONVRTINGINTOFILE, convert_to_file );

             save_image(convert_to_file, load_image(lf), NULL);

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

                     seal_error(ERR_INFO, "%s\n\n%s", INI_TEXT("Can't show image!"), lf);

                 _free(runfile);
                 _free(lf);

           }; break;
           case MSG_ABOUT:
             seal2_app_about("Imager", load_image("/system/bmp/viewer.ico,32"));

             clear_event(event);
             break;

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


   p_filelistctrl  dir1;


   p_appwin o = appwin_init(_malloc(sizeof(t_appwin)), /* make window */
                            r,
                            _strdup("Image Viewer"),
                            WF_MINIMIZE|WF_MAXIMIZE,
                            ap_id, /* application id */
                            NULL);

   o->icon16 = load_image("/system/bmp/viewer.ico,16");

   VIEW(o)->drag_mode |= DM_DRAGGROW; /* able to grow view */

   VIEW(o)->align |= TX_ALIGN_CENTER; /* set alignment */

   VIEW(o)->size_minimum = &dc_size_minimum;

   r = VIEW(o)->size_limits(VIEW(o));

   o->menu = standard_system_menu_items(o,
                new_menu_line(
                new_menu_item("About Imager...", 0, 0, MSG_ABOUT, NULL,
   NULL)));

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(o)); /* insert it on the desktop */

   win = o;

   s = r = rect_assign(r.a.x+5, r.a.y+10, t.b.x-10, r.b.y-5);

   r = rect_assign(r.a.x, r.a.y, r.a.x+(DC_SIZEX/2)-15, r.b.y);

   t = r;

   r.a.y += 30;

   box1 = filelistbox_init(_malloc(sizeof(t_filelistbox)), r, 1, FL_OPEN, STANDARD_PATH, "*.*", FA_ALL, NULL);

   if ( box1 ) {
        /* redeclare translate event function */
        OBJECT(box1)->translate_event = &dcfilelistbox_translate_event;

        /* redeclare alignment of filelistbox 1 */
        VIEW(box1)->align |= TX_ALIGN_BOTTOM;

        FILELISTBOX(box1)->file_menu = &dcfilelistbox_file_menu;

   };

   OBJECT(o)->insert(OBJECT(o), OBJECT(box1)); /* insert it on the desktop */


    r = rect_move(r, (DC_SIZEX/2)+5, 0);

    r.b.y-=15;
    r.b.x-=20;

    sc = scroller_init(_malloc(sizeof(t_scroller)), r, SF_VERSCROLLBAR|SF_HORSCROLLBAR);

    VIEW(sc)->align |= TX_ALIGN_BOTTOM|TX_ALIGN_RIGHT;

    SCROLLER(sc)->scroll_size = &imager_scroll_size;

    SCROLLER(sc)->deltax = 20;
    SCROLLER(sc)->deltay = 20;

    OBJECT(o)->insert(OBJECT(o), OBJECT(sc)); /* insert it on the desktop */

    r = rect_assign(0, 0, 10, 10);


    imager = view_init(_malloc(sizeof(t_view)), r);

    OBJECT(sc)->insert(OBJECT(sc), OBJECT(imager)); /* insert it on the desktop */

    VIEW(imager)->brush.state &= ~BRUSH_SELFIMG;

    make_image ("start.jpg");


   r = rect_assign(t.a.x, t.a.y, t.a.x+300, t.a.y+20);

   /* make link for box1 */
   dir1 = filelistctrl_init(_malloc(sizeof(t_filelistctrl)), r, box1);

   OBJECT(o)->insert(OBJECT(o), OBJECT(dir1)); /* insert it on the desktop */


};


p_list get_imager_filelist (void)
{
                                                /* function for free-ing items */
   p_list p = list_init(malloc(sizeof(t_list)), &free_filehistory_item, 0);

   if (p)
   {
      p->insert(p, new_filehistory_item("BMP files (*.bmp)", "*.bmp"));
      p->insert(p, new_filehistory_item("PCX files (*.pcx)", "*.pcx"));
      p->insert(p, new_filehistory_item("GIF files (*.gif)", "*.gif"));
      p->insert(p, new_filehistory_item("JPEG files (*.jpg)", "*.jpg"));
      p->insert(p, new_filehistory_item("PNG files (*.png)", "*.png"));
      p->insert(p, new_filehistory_item("TIFF files (*.tif)", "*.tif"));
      p->insert(p, new_filehistory_item("TARGA files (*.tga)", "*.tga"));
      p->insert(p, new_filehistory_item("LBM files (*.lbm)", "*.lbm"));
      p->insert(p, new_filehistory_item("VID files (*.vid)", "*.vid"));
      p->insert(p, new_filehistory_item("Windows Icons (*.ico)", "*.ico"));
      p->insert(p, new_filehistory_item("Portable Greyscale-map files (*.pgm)", "*.pgm"));
      p->insert(p, new_filehistory_item("Pixmap files (*.ppm)", "*.ppm"));
      p->insert(p, new_filehistory_item("IAX files (*.iax)", "*.iax"));
      p->insert(p, new_filehistory_item("PSE files (*.pse)", "*.pse"));
      p->insert(p, new_filehistory_item("X Window Bitmap files (*.xbm)", "*.xbm"));

      p->insert(p, new_filehistory_item("All Files (*.*)", "*.*"));
   }


   return p;
}


app_begin ( void ) {

  if ( ap_process == AP_INIT ) {

    init_imager();

  } else

  if ( ap_process == AP_FREE ) {

/*    if ( image ) destroy_bitmap(image);

    image = NULL;*/

  };

} app_end;

