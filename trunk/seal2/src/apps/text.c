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

#include <seal.h>
#include <app.h>
#include <seal/tabbook.h>
#include <button.h>

app_begin ( void ) {


  if ( ap_process == AP_INIT ) {
	p_button b;
    p_appwin win = appwin_init( _malloc(sizeof(t_appwin)),
                                 rect_assign(100,100,300,300),
                                 "Tab test",
                                 WF_MINIMIZE,
                                 NULL,
                                 NULL);
    p_tabbook tb = tabbook_make ( rect_assign(5,20,195,195) );

    tb->add_tab(tb,"TEST1",NULL);
    tb->add_tab(tb,"TEST2",NULL);
    tb->add_tab(tb,"TEST3",NULL);

    b = button_init(_malloc(sizeof(t_button)), rect_assign ( 5,5,100,20 ), "TEST1", MSG_OK, 0 );
    tb->insert(tb,0,OBJECT(b));
    b = button_init(_malloc(sizeof(t_button)), rect_assign ( 5,5,100,20 ), "TEST2", MSG_OK, 0 );
    tb->insert(tb,1,OBJECT(b));
     b = button_init(_malloc(sizeof(t_button)), rect_assign ( 5,5,100,20 ), "TEST3", MSG_OK, 0 );
    tb->insert(tb,2,OBJECT(b));
    OBJECT(desktop)->insert(OBJECT(desktop),OBJECT(win));
    OBJECT(win)->insert(OBJECT(win),OBJECT(tb));
  };

} app_end;