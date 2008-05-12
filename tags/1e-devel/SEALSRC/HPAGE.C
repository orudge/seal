/****************************************************************/
/*                                                              */
/*                           hpage.c                            */
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


#include"allegro.h"
#include"seal.h"
#include"menus.h"
#include"app.h"
#include"button.h"
#include"dialogs.h"
#include"editor.h"
#include"iodlg.h"
#include"htmlwin.h"

app_begin ( void ) {


  if ( ap_process == AP_INIT ) {

      t_rect r = rect_assign(0, 20, 640, 400);
      p_htmlwin p = htmlwin_init(malloc(sizeof(t_htmlwin)),
                                 r,
                                 "HPAGE",
                                 ap_args);

      APPWIN(p)->app_id = ap_id;

      OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(p));

  };

} app_end;

