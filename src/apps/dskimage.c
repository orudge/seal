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

/* SEAL Disk Image Utility
   Copyright(c) Alexandr Levitsky 2001.
   All Rights Reserved Worldwide.
*/

/* Revision History (as of 03/04/2002):
 *
 * 03/04/2002: Updated slightly for SEAL 2.00.11 (orudge)
 *
 */

#include <stdio.h>
#include <bios.h>
#include <fcntl.h>
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <string.h>
#include <ctype.h>

#include <allegro.h>
#include <seal.h>
#include <app.h>
#include <button.h>
#include <menus.h>
#include <editor.h>
#include <dialogs.h>
#include <iodlg.h>
#include <keyboard.h>
#include <window.h>

// function declaration
     p_list get_list ( void );
     p_list get_list2 ( void );
     int Disk_To_File ();
     int File_To_Disk ();
     int _biso_disk_error_message(unsigned bd_errnumb);
     p_list get_file_items();

// Global objects
     p_list list = NULL;
     p_list list2 = NULL;
     p_history his, his2;  // combo boxes
     p_appwin hello = NULL;
     p_process process = NULL;
     p_stattext stext1, stext2, stext3;
     p_textline stext4;
     p_textline fileimage = NULL;
     p_button butt_di, butt_id, butt_exit, butt_browse, butt_about;

// variables
     long id_progress = 20;
     long id_max_progress = 80;
     struct diskinfo_t di;
     char diskbuf[512];
     char *file = NULL;

p_list get_file_items()
{
        p_list p = list_init (malloc(sizeof (t_list)), &free_filehistory_item, 0);
        if (p)
        {
                p->insert (p, new_filehistory_item ("Disk Images (*.dsk)", "*.dsk"));
                p->insert (p, new_filehistory_item ("Disk Images (*.img)", "*.img"));
                p->insert (p, new_filehistory_item ("All Files (*.*)", "*.*"));
        }
        return (p);
}
void trans_ev ( p_object o, p_event event )
{

      if ( event->type & EV_MOUSE ) {
           if (OBJECT(mouse)->state & MO_SF_MOUSELUP)
           {
           }
      }  

  if ( o->process & PH_PREPROCESS ) {


		// EXIT
            if ( event->message == 950003 ) {
                // clear_event(event);
		event->message = MSG_CLOSE;
            };

		// DISK TO IMAGE
            if ( event->message == 950001 ) {
			clear_event(event);
                   	Disk_To_File();


            };

		// IMAGE TO DISK
            if ( event->message == 950002 ) {
                        clear_event(event);
			File_To_Disk();

            };
                // About ...
            if (event->message == 950004)
            {
               clear_event(event);
               seal2_app_about("Disk Imager", load_image("/system/bmp/35floppy.ico"));
//               msgbox(MW_INFO, 1, " Disk Image Utilite\n version 1.0\n Created by Alex Levitsky\n (C) 2001\n sealru@mailru.com");
            }
                // BROWSE
            if ( event->message == 950005 ) {
                        clear_event(event);
                        file = open_dialog ("/", "*.dsk", get_file_items());
                        file = io_sealfile(file);
                        TEXTLINE(fileimage)->set_text(TEXTLINE(fileimage), file);

            };
        
  };

};


p_list get_list ( void )
{
        list = list_init(malloc(sizeof(t_list)),
                                &free_listbox_item, 
                                DAT_LIST); 
        if ( list ) {
           list->insert(list, new_listbox_item("1.44mb", 
                                               NULL,
                                               true,
                                               LIF_NONE));
        };
        return list;
};

p_list get_list2 ( void )
{
        list2 = list_init(malloc(sizeof(t_list)),
                                &free_listbox_item,
                                DAT_LIST); 
        if ( list2 ) {
           list2->insert(list2, new_listbox_item("A:", 
                                               NULL,
                                               true,
                                               LIF_NONE));
           list2->insert(list2, new_listbox_item("B:", 
                                               NULL,
                                               true,
                                               LIF_NONE));
        };
        return list2;
};

void init_hello ( void )
{

   t_rect r = rect_assign(200, 150, 500, 350);

   hello = appwin_init(_malloc(sizeof(t_appwin)), r,
                       "Disk Imager",
                       2,
                       ap_id, 
                       &trans_ev); 

   hello->icon16 = load_image("bmp/35floppy.ico,16");

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(hello));

     r = rect_assign(10, 45, 150, 65);
     his = history_init(malloc(sizeof(t_history)),
                                  r, 
                                  get_list(), 
                                  15,                /* Max number of chars. */
                                  HF_REWRITEUNABLE);  /* Read-only. */

     OBJECT(hello)->insert(OBJECT(hello), OBJECT(his));

     r = rect_assign(10, 90, 150, 110);
     his2 = history_init(malloc(sizeof(t_history)),
                        r, 
                        get_list2(),
                        5,
                        HF_REWRITEUNABLE);

     OBJECT(hello)->insert(OBJECT(hello), OBJECT(his2));

	// text line for name of fileimage
     r = rect_assign(10, 135, 145, 155);
     fileimage = textline_init ( malloc(sizeof(t_textline)), 
                                    r, 
                                    65535,
                                    0);
     OBJECT(hello)->insert(OBJECT(hello), OBJECT(fileimage));
     TEXTLINE(fileimage)->set_text(TEXTLINE(fileimage), "[NONE]");



	// progress bar
     r = rect_assign(10, 170, 250, 185);	
     process = process_init(malloc(sizeof(t_process)),
                                r,
                                id_max_progress,
                                &id_progress);
     OBJECT(hello)->insert(OBJECT(hello), OBJECT(process));

     id_progress = 80;


	// static text lines
     r = rect_assign(10, 25, 150, 45);
     stext1 = stattext_init ( malloc(sizeof(t_stattext)), 
                                    r, 
                                    TX_ALIGN_LEFT,
                                    "Disk Size:", 
                                    20); 
     OBJECT(hello)->insert(OBJECT(hello), OBJECT(stext1));
	// 
     r = rect_assign(10, 70, 150, 90);
     stext2 = stattext_init ( malloc(sizeof(t_stattext)), 
                                    r, 
                                    TX_ALIGN_LEFT,
                                    "Floppy Drive:", 
                                    20); 
     OBJECT(hello)->insert(OBJECT(hello), OBJECT(stext2));
     r = rect_assign(10, 115, 150, 133);
     stext3 = stattext_init ( malloc(sizeof(t_stattext)), 
                                    r, 
                                    TX_ALIGN_LEFT,
                                    "Image File:",
                                    20); 
     OBJECT(hello)->insert(OBJECT(hello), OBJECT(stext3));

     r = rect_assign(253, 168, 290, 187);
     stext4 = textline_init ( malloc(sizeof(t_textline)), 
                                    r, 
                                    20,
                                    1);
     OBJECT(hello)->insert(OBJECT(hello), OBJECT(stext4));
     OBJECT (stext4)->set_options (OBJECT (stext4), OB_OF_SELECTABLE, false);
     TEXTLINE(stext4)->set_text(TEXTLINE(stext4), " ");

	// buttons
     r = rect_assign(148, 135, 167, 155);
     butt_browse = button_init(_malloc(sizeof(t_button)), r, "...",
                     950005, 
                     BF_NORMAL);
     OBJECT(hello)->insert(OBJECT(hello), OBJECT(butt_browse));

     r = rect_assign(175, 30, 290, 55);
     butt_di = button_init(_malloc(sizeof(t_button)), r, "Disk to Image",
                     950001, 
                     BF_NORMAL);
     OBJECT(hello)->insert(OBJECT(hello), OBJECT(butt_di));
     r = rect_assign(175, 65, 290, 90);
     butt_id = button_init(_malloc(sizeof(t_button)), r, "Image to Disk",
                     950002, 
                     BF_NORMAL);
     OBJECT(hello)->insert(OBJECT(hello), OBJECT(butt_id));
     r = rect_assign(175, 100, 290, 125);
     butt_exit = button_init(_malloc(sizeof(t_button)), r, "Exit",
                     950003, 
                     BF_NORMAL);
     OBJECT(hello)->insert(OBJECT(hello), OBJECT(butt_exit));
 
     r = rect_assign(175, 135, 290, 160);
     butt_about = button_init(_malloc(sizeof(t_button)), r, "About...",
                     950004, 
                     BF_NORMAL);
     OBJECT(hello)->insert(OBJECT(hello), OBJECT(butt_about));
}

int Disk_To_File ()
{
     int a,b,c, tar;
     unsigned status = 0;
     char *drv;

    drv = TEXTLINE(his2)->text;
    di.drive = *drv - 'A';

    if (strlen (TEXTLINE(fileimage)->text) < 1)
    {
             msgbox (MW_ERROR,1,"No filename specified.");
             return (-1);
    }

    tar = open (GetFile(TEXTLINE(fileimage)->text), O_APPEND | O_CREAT | O_BINARY | O_WRONLY );

    if (!tar)
    {
             msgbox (MW_ERROR,1,"Can't open file.");
             return (-1);
    }
    else
    {
        // delete old file in exists
        remove (TEXTLINE(fileimage)->text);
    }
	id_progress = 0;
        TEXTLINE(stext4)->set_text(TEXTLINE(stext4), "0%");
        _while ( id_progress < id_max_progress) 
	{
                b = 1;
              _while ( b < 19 )
              {
                      c = 0;
                      _while ( c < 2 )
		      {
			di.head     = c;
			di.track    = id_progress;
			di.sector   = b;
			di.nsectors = 1;
			di.buffer   = diskbuf;
                        status = _bios_disk( _DISK_READ, &di ) >> 8;
                        if (!status) break;
                        c++;
                      }

                 if (status)
                 {
                     a = _biso_disk_error_message(status);
                     if (a == -1)
                     {
                         close (tar);
                         return (-1);
                     }
                 }
                 write (tar, diskbuf , 512);
                 b++;

              }
                TEXTLINE(stext4)->set_text(TEXTLINE(stext4),  set_format_text (NULL,"%3.0f%%", (float)((float)id_progress / (float)id_max_progress) * (float)100));
                id_progress++;
	}
	close (tar);
        id_progress = 80;
        TEXTLINE(stext4)->set_text(TEXTLINE(stext4), "100%");
        
        msgbox (MW_INFO,1,"Image is done!");
}
int File_To_Disk ()
{
     int a,b,c, tar;
     unsigned status = 0;
	char *drv;

    drv = TEXTLINE(his2)->text;
    di.drive = *drv - 'A';

    if (strlen (TEXTLINE(fileimage)->text) < 1)
    {
             msgbox (MW_ERROR,1,"No filename specified.");
             return (-1);        
    }

    tar = open (GetFile(TEXTLINE(fileimage)->text), O_RDONLY | O_BINARY );

    if (!tar)
    {
             msgbox (MW_ERROR,1,"Can't open file.");
             return (-1);
    }
	id_progress = 0;
        TEXTLINE(stext4)->set_text(TEXTLINE(stext4), "0%");
        // view_draw (VIEW(stext4));
        _while ( id_progress < 80)
	{
                b = 1;
                _while ( b < 19 )
		{
                      c = 0;  
                      _while ( c < 2 )
		      {
			read (tar, diskbuf , 512);

			di.head     = c;
			di.track    = id_progress;
			di.sector   = b;
			di.nsectors = 1;
			di.buffer   = diskbuf;

			status = _bios_disk( _DISK_WRITE , &di ) >> 8;
                        if (!status) break;

                        c++;
		      }
                      if (status)
                      {
                            a = _biso_disk_error_message(status);
                            if (a == -1)
                            {
                                close (tar);
                                return (-1);
                            }
                      }

                      b++;
		}
                TEXTLINE(stext4)->set_text(TEXTLINE(stext4),  set_format_text (NULL,"%3.0f%%", (float)((float)id_progress / (float)id_max_progress) * (float)100));
                // view_draw (VIEW(stext4));
                id_progress++;
	}
	close (tar);
	id_progress = 80;
        TEXTLINE(stext4)->set_text(TEXTLINE(stext4), "100%");
        // view_draw (VIEW(stext4));
        msgbox (MW_INFO,1,"Disk is done!");
}
int _biso_disk_error_message(unsigned bd_errnumb)
{
     switch (bd_errnumb)
     {
        case 0x00:
             return (0);
        case 0x01:
             msgbox (MW_ERROR,1,"Invalid request or a bad command");
             return (0);
             break;
        case 0x02:
             msgbox (MW_ERROR,1,"Address mark not found");
             break;
        case 0x03:
             msgbox (MW_ERROR,1,"Disk write protected");
             break;
        case 0x04:
             msgbox (MW_ERROR,1,"Sector not found");
             break;
        case 0x05:
             msgbox (MW_ERROR,1,"Reset failed");
             break;
        case 0x06:
             msgbox (MW_ERROR,1,"Floppy disk removed");
             break;
        case 0x07:
             msgbox (MW_ERROR,1,"Drive parameter activity failed");
             break;
        case 0x08:
             msgbox (MW_ERROR,1,"Direct Memory Access (DMA) overrun");
             break;
        case 0x09:
             msgbox (MW_ERROR,1,"DMA crossed 64K boundary");
             break;
        case 0x0A:
             msgbox (MW_ERROR,1,"Bad sector flag detected");
             break;
        case 0x0B:
             msgbox (MW_ERROR,1,"Bad track flag detected");
             break;
        case 0x0C:
             msgbox (MW_ERROR,1,"Media type not found");
             break;
        case 0x0D:
             msgbox (MW_ERROR,1,"Invalid number of sectors on format");
             break;
        case 0x0E:
             msgbox (MW_ERROR,1,"Control data access mark detected");
             break;
        case 0x0F:
             msgbox (MW_ERROR,1,"DMA arbitration level out of range");
             break;
        case 0x10:
             msgbox (MW_ERROR,1,"Data read (CRC or ECC) error");
             break;
        case 0x11:
             msgbox (MW_ERROR,1,"Corrected data read (ECC) error");
             break;
        case 0x20:
             msgbox (MW_ERROR,1,"Controller failure");
             break;
        case 0x40:
             msgbox (MW_ERROR,1,"Seek error");
             break;
        case 0x80:
             msgbox (MW_ERROR,1,"Disk timed out or failed to respond");
             break;
        case 0xAA:
             msgbox (MW_ERROR,1,"Drive not ready");
             break;
        case 0xBB:
             msgbox (MW_ERROR,1,"Undefined error");
             break;
        case 0xCC:
             msgbox (MW_ERROR,1,"Write fault on drive");
             break;
        case 0xE0:
             msgbox (MW_ERROR,1,"Status error");
             break;
        case 0xFF:
             msgbox (MW_ERROR,1,"Sense operation failed");
             break;

        default:
             msgbox (MW_ERROR,1,"Unknown error");
             return (0);
             break;
     }
     return (-1);

}

app_begin ( void ) {

   if ( ap_process == AP_INIT ) {
                init_hello();
   };

} app_end;
