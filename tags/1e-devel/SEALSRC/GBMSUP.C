/*
*/

#include"allegro.h"
#include"algbm.h"
#include"seal.h"


lib_exportable

lib_begin ( void )
{
        if ( ap_process == AP_ALLOC ) { /* first call of library */

          AP_EXPORTLIB(); /* export table in gifsup.exp */

          register_bitmap_file_type("gif", &load_gbm, &save_gbm);
          register_bitmap_file_type("tif", &load_gbm, &save_gbm);
          register_bitmap_file_type("vid", &load_gbm, &save_gbm);
          register_bitmap_file_type("pgm", &load_gbm, &save_gbm);
          register_bitmap_file_type("ppm", &load_gbm, &save_gbm);
          register_bitmap_file_type("iax", &load_gbm, &save_gbm);
          register_bitmap_file_type("xbm", &load_gbm, &save_gbm);
          register_bitmap_file_type("pse", &load_gbm, &save_gbm);

        } else

        if ( ap_process == AP_INIT ) {

          AP_SETNUMOFCALLS(1); /* set max num of calls */

        } else

        if ( ap_process == AP_FREE ) { /* last call of library */

          register_bitmap_file_type("gif", NULL, NULL);
          register_bitmap_file_type("tif", NULL, NULL);
          register_bitmap_file_type("vid", NULL, NULL);
          register_bitmap_file_type("pgm", NULL, NULL);
          register_bitmap_file_type("ppm", NULL, NULL);
          register_bitmap_file_type("iax", NULL, NULL);
          register_bitmap_file_type("xbm", NULL, NULL);
          register_bitmap_file_type("pse", NULL, NULL);

        };

} lib_end;

