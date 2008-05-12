/****************************************************************/
/*                                                              */
/*                          htmlwin.h                           */
/*                                                              */
/*                Seal - free desktop environment               */
/*                                                              */
/*                  Copyright (c) 1999,2000                     */
/*                       Michal Stencl                          */
/*                    All Rights Reserved                       */
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



#ifndef __HTMLWIN_H_INCLUDED__
#define __HTMLWIN_H_INCLUDED__

#include"app.h"
#include"dialogs.h"
#include"load_htm.h"

#define IO_URL_LIMIT   IO_FILE_LIMIT



/* text declarations */

#ifndef TXT_NONAME
#define TXT_NONAME             INI_TEXT("Noname")
#endif
#ifndef TXT_BACKTOPREVIUSPAGE
#define TXT_BACKTOPREVIUSPAGE  INI_TEXT("Back to previus page")
#endif
#ifndef TXT_FORWARDTONEXTPAGE
#define TXT_FORWARDTONEXTPAGE  INI_TEXT("Forward to next page")
#endif
#ifndef TXT_OPENNEWPAGE
#define TXT_OPENNEWPAGE        INI_TEXT("Open new page")
#endif
#ifndef TXT_RELOADCURRENTPAGE
#define TXT_RELOADCURRENTPAGE  INI_TEXT("Reload current page")
#endif
#ifndef TXT_STOPLOADINGPROCESS
#define TXT_STOPLOADINGPROCESS INI_TEXT("Stop loading process")
#endif
#ifndef TXT_ABOUTHTMLWIN
#define TXT_ABOUTHTMLWIN       INI_TEXT("About...")
#endif


/* htmlwin messages */

#define MSG_HTMLOPEN             131
#define MSG_HTMLSTOP             132
#define MSG_HTMLBACK             133
#define MSG_HTMLNEXT             134
#define MSG_HTMLRELOAD           135
#define MSG_HTMLQUEST            136


/* htmlwin */

typedef struct t_htmlwin *p_htmlwin;

typedef struct t_htmlwin {

   struct t_appwin obclass; /* inherited functions & variables */

   p_list          queue;
   l_long          index;

   l_text          filename;
   p_textline      urlline;
   p_htmloader     htmloader;

   void            (*push_link) ( p_htmlwin o, l_text link, l_bool reload );
   void            (*back_link) ( p_htmlwin o );
   void            (*next_link) ( p_htmlwin o );
   l_bool          (*reload) ( p_htmlwin o );
   void            (*redraw_title) ( p_htmlwin o );

} t_htmlwin;

#define HTMLWIN(o) ((p_htmlwin)(o))



/* htmlwin functions */

void  htmlwin_setup ( p_object o );

void   htmlwin_push_link ( p_htmlwin o, l_text link, l_bool reload );
void   htmlwin_back_link ( p_htmlwin o );
void   htmlwin_next_link ( p_htmlwin o );
l_bool htmlwin_reload ( p_htmlwin o );
void   htmlwin_redraw_title ( p_htmlwin o );


p_htmlwin  _htmlwin_init ( p_htmlwin o, t_rect r, l_text title, l_text fname );
extern p_htmlwin  (*htmlwin_init) ( p_htmlwin o, t_rect r, l_text title, l_text fname );


#endif

