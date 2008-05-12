/*

      Copyright (c) Florian Xaver 2000

      florianx@drdos.org

      Starts files in ASTART automtic.

      License: GPL



*/
#include"allegro.h"
#include"seal.h"
#include "dialogs.h"

int  ff_walker(const char* path, const struct ffblk *ff)
     {
       l_text lf = 0;

       if (!stricmp("ldv", get_extension(path)) || !stricmp("lnk", get_extension(path)))
       
       {

//       msgbox(MW_ERROR, MB_OK, "%s", path);

       lf = io_sealpath(path);
       
       if ( !run_file (lf) )

                     seal_error(ERR_INFO, "%s\n\n%s", "Cannot run file %s :(", lf);


       DEBUG_printf("Autostart:\n");
       DEBUG_printf("Loaded file:");
       DEBUG_printf(path);
       DEBUG_printf("\n");

       if (lf) _free(lf);
        
     
       if (strcmp(ff->ff_name, "XXXXX") == 0)
         return 42;

       }

       return 0;
}





app_begin ( void ) {

     if ( ap_process == AP_INIT ) {

     l_text path = 0;

     AP_SETNUMOFCALLS(1);

     path = getini_fromfile (INI_MAINFILE, "paths", "autostart");

     if (path == 0 ) return;

//     msgbox(MW_ERROR, MB_OK, "%s", path);

     __file_tree_walk(path, ff_walker);


     _free(path);


     }

} app_end;
