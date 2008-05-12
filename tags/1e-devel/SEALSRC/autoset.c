/*

      Copyright (c) Florian Xaver 2000

      florianx@drdos.org

      autoset.c - setup program for seal

      License: GPL

*/

#include"allegro.h"
#include"seal.h"
#include"string.h"
#include"dialogs.h"


app_begin ( void ) {

     if ( ap_process == AP_INIT ) {

     l_text path;
     l_text path1;
     int installed =0;
     l_text temp;
     

     AP_SETNUMOFCALLS(1);

     temp = getini_fromfile (INI_MAINFILE, "setup", "installed");
     if (!stricmp(temp, "yes"))
        {
        return;
        }

     msgbox(MW_INFO, MB_OK, "Wellcome to SEAL/FreeDOS beta 1!\n(c) Florian Xaver (florianx@drdos.org)\n"
                            "\n"
                            "SEAL/FreeDOS ist going to be installed!");


     path = (l_text) getcwd(NULL, 260);
     strcat(path, "/");
     path1 = _strdup(path);
     setini_tofile(INI_MAINFILE, "paths", "main", path, INI_STRING);
     msgbox(MW_INFO, MB_OK, "Installing SEAL to current directory (%s)", path);
     strcat(path, "desktop/");
     setini_tofile(INI_MAINFILE, "drives", "desktop", path, INI_STRING);
     strcat(path1, "astart/");
     setini_tofile(INI_MAINFILE, "paths", "autostart", path1, INI_STRING);
//     msgbox(MW_QUESTION, MB_OK, "Updated to %s", path);
     _free(path);
     _free(path1);
     _free(temp);


     setini_tofile ("seal.ini", "setup", "installed", "yes", INI_STRING);

     msgbox(MW_INFO, MB_OK, "Please choose the settings and then restart SEAL!\n\n"
                            "If you will have problems with the sound, you will have to edit seal.cfg!");

     run_file ("./settings/setscr.exe");


     
     }

} app_end;
