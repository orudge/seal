/*
  JPEG reading and writing by Eric Vannier.
  -------------------------------------------
*/

#include"allegro.h"
#include"jpeg.h"
#include"seal.h"


BITMAP *jpeg_load_ex(char *filename, RGB *pal,jpeg_decode_param *jpeg);
int     jpeg_save_ex(char *filename, BITMAP *bmp, RGB *pal,jpeg_code_param *jpeg);

BITMAP *jpeg_load(char *filename, RGB *pal);
int     jpeg_save(char *filename, BITMAP *bmp, RGB *pal);


BITMAP *jpeg_load_ex(char *filename, RGB *pal,jpeg_decode_param *jpeg) {
  return load_jpeg_ex(filename, pal, jpeg);
};


int     jpeg_save_ex(char *filename, BITMAP *bmp, RGB *pal,jpeg_code_param *jpeg) {
  return save_jpeg_ex(filename, bmp, pal, jpeg);
};


BITMAP *jpeg_load(char *filename, RGB *pal) {
  return load_jpeg(filename, pal);
};


int     jpeg_save(char *filename, BITMAP *bmp, RGB *pal) {
  return save_jpeg(filename, bmp, pal);
};



#include"jpegsup.exp"

lib_begin ( void )
{
        if ( ap_process == AP_ALLOC ) { /* first call of library */

          AP_EXPORTLIB(); /* export table in jpegsup.exp */

          register_bitmap_file_type("jpg", &jpeg_load, &jpeg_save);

        } else

        if ( ap_process == AP_INIT ) {

          AP_SETNUMOFCALLS(1); /* set max num of calls */

        } else

        if ( ap_process == AP_FREE ) { /* last call of library */

          register_bitmap_file_type("jpg", NULL, NULL);

        };

} lib_end;

