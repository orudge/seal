/* -------------------------------------------------
     grfx  object ! (the supidest object !)
           by Julien Etelain <julien@pmad.mail22.com>

     grfx is only to draw in with allegro functions.
     See help_grfx.txt to get help about busing grfx


   This is a part of SEAL ...
       -> Have a look to .txt

   ------------------------------------------------- */

#include"allegro.h"
#include"seal.h"
#include"grfx.h"

p_grfx   (*grfx_init) ( p_grfx o, t_rect r ) = &_grfx_init;

p_grfx   _grfx_init ( p_grfx o, t_rect r )
{
  if ( !o ) return NULL;

  clear_type(o, sizeof(t_grfx));

  view_init(VIEW(o), r);

  OBJECT(o)->set_options(OBJECT(o), OB_OF_SELECTABLE+OB_OF_ENABLE, false);
  OBJECT(o)->set_options(OBJECT(o), OB_OF_STILLPROCESS, true);


  return o;

};

 #include"grfx.exp"


lib_begin ( void ) {

  if ( ap_process == AP_ALLOC ) {

          AP_EXPORTLIB(); /* export table in app.exp */

  } else

  if ( ap_process == AP_INIT ) {

          AP_SETNUMOFCALLS(1); /* set max num of calls */

  } else

  if ( ap_process == AP_FREE ) {


  };


} lib_end;

