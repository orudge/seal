/*
::
::     ___/\/\/\/\/\__/\/\/\/\/\/\______/\/\______/\/\_______
::    _/\/\__________/\______________/\/\/\/\____/\/\_______
::   ___/\/\/\/\____/\/\/\/\/\____/\/\____/\/\__/\/\_______
::  _________/\/\__/\/\__________/\/\/\/\/\/\__/\/\_______
:: _/\/\/\/\/\____/\/\/\/\/\/\__/\/\____/\/\__/\/\/\/\/\_
::______________________________________________________
::
::
::
::    PNGSUP.C
::    driver for png support for SEAL
::    Look at www.allegro.cc for LOADPNG
::
::    (c) Florian Xaver 2000
::    mail: florianx@drdos.org
::    web: http://www.seal.de.vu
::
::    License: GPL
::
*/

#include <png.h>
#include"allegro.h"
#include"seal.h"
#include"loadpng.h"



#include"pngsup.exp"

lib_begin ( void )
{
        if ( ap_process == AP_ALLOC ) { /* first call of library */

          AP_EXPORTLIB(); /* export table in pngsup.exp */

          register_png_file_type();



        } else

        if ( ap_process == AP_INIT ) {


          AP_SETNUMOFCALLS(1); /* set max num of calls */

        } else

        if ( ap_process == AP_FREE ) { /* last call of library */


          register_png_file_type();
          
        };

} lib_end;

