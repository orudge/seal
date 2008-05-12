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

/* Revision History:

   DATE              CHANGE                                         VERSION
   23/09/2001        Created application                            0.1
   23/09/2001        Fixed many bugs                                0.1
   29/03/2002        Updated slightly for SEAL 2.00.11 release      0.2

   All dates are in DD/MM/YYYY format.
*/

// screen_reload
#include <allegro.h>
#include <seal.h>
#include <screen.h>
#include <app.h>
#include <dialogs.h>
#include <iodlg.h>
#include <mouse.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dir.h>

l_text file;

p_list get_filelist (void)
{
                                                /* function for free-ing items */
   p_list p = list_init(malloc(sizeof(t_list)), &free_filehistory_item, 0);

   if (p)
   {
      p->insert(p, new_filehistory_item("Executables (*.exe)", "*.exe"));
      p->insert(p, new_filehistory_item("Commands (*.com)", "*.com"));
      p->insert(p, new_filehistory_item("Batch Files (*.bat)", "*.bat"));
      p->insert(p, new_filehistory_item("All Files (*.*)", "*.*"));
   }

   return p;
}

void app_init()
{
   char pathstr[MAXPATH];
   char oldpath[MAXPATH];
   char *pathloc;
   char *newfile;
   p_object o;

   if (!file)
      file = open_dialog("/", "*.exe;*.com;*.bat", get_filelist());

   if (file)
   {
      newfile = GetFile(file); // get the DOS name for it

      pathloc = strrchr(newfile, '/');
      memset(pathstr, 0, sizeof(pathstr));
      memcpy(pathstr, newfile, pathloc-newfile);  // Get the path so a chdir() can be done

      getcwd(oldpath, MAXPATH);
      put_backslash(pathstr);
      chdir(pathstr);

//      mouse_done(OBJECT(mouse));
      remove_mouse();    // remove mouse driver

// <UPDATE by Julien> //
      // ... set_gfx_mode(GFX_TEXT, 0, 0, 0, 0); -> BAD IDEA !, there is function for it
      screen_halt();  // Set text mode and save screen ....
// </ UPDATE by Julien> //

#ifdef DEBUG_INFO
      printf("File name: %s\n", newfile);
      printf("Path:      %s\n", pathstr);
      printf("Old Path:  %s\n", oldpath);
      printf("\n");
#endif
      // Drivers have to be removed and reinstalled in case of conflicts with DOS apps

      remove_keyboard(); // remove keyboard driver
      remove_sound();    // remove sound driver

      allegro_exit();
      system(newfile);
      allegro_init();

      install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, ""); // reinstall sound driver
      install_keyboard();                                  // reinstall keyboard driver

      put_backslash(oldpath);
      chdir(oldpath);
      printf("\nPress any key to return to SEAL...");

      do {
      } while (!keypressed());

      readkey();

      screen_reload();

      install_mouse();                                     // reinstall mouse driver
//      mouse_init(mouse);


// <UPDATE by Julien> //
      // ... desktop->draw_view(desktop); -> why ? screen_reload done it ....
// </ UPDATE by Julien> //

      set_mouse_speed(mouse->speed.x, mouse->speed.y);

      mouse_set_sys_cursor(CUR_ARROW);
      mouse_show(mouse);

      DLXUnload(ap_id);
   }
}

SetInfoAppName("SealDOS");
SetInfoDesciption("SEAL 2 DOS compatibility program");
SetInfoCopyright("Copyright (c) Owen Rudge 2001-2002. All Rights Reserved.");
SetInfoManufacturer("Owen Rudge");

app_begin(void)
{
   if (ap_process == AP_INIT)
   {
      if (ap_args)
         file = ap_args;

      app_init();
   }
} app_end;
