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
 *
 * 25/09/2001 - Created (Julien Etelain)
 * 30/03/2002 - Added About dialog (orudge)
 * 04/04/2002 - Updated with new icon (orudge)
 */

#include <seal.h>
#include <app.h>
#include <button.h>
#include <treeview.h>
#include <dialogs.h>
#include <iodlg.h>
#include <stattext.h>

////////////////////////////////////////////////////////////////////////////////
SetInfoAppName("Registry Editor");
SetInfoDesciption("Graphical registry editor based around a treeview.");
SetInfoCopyright("(c) Copyright 2001-2002 SealSystem. All rights reserved");
SetInfoManufacturer("SealSystem");
////////////////////////////////////////////////////////////////////////////////
#define MSG_NEWKEY 100001
#define MSG_DELKEY 100002
#define MSG_EDITKEY 100003
p_treeview tree = NULL;

p_appwin win = NULL;
p_stattext keyinfo = NULL;
////////////////////////////////////////////////////////////////////////////////
void get_key_dat ( l_text key, l_text *value, l_long *type ) {
  l_uchar typ;
  if ( !get_key_data(key,&typ,NULL,NULL) ) {
    if ( typ == KEY_BYTE ) {
      *(value) = set_format_text(NULL,"%d",get_key_byte(key));
      *(type) = 1;
    } else if ( typ == KEY_SMALL ) {
      *(value) = set_format_text(NULL,"%d",get_key_small(key));
      *(type) = 2;
    } else if ( typ == KEY_INTEGER ) {
      *(value) = set_format_text(NULL,"%d",get_key_integer(key));
      *(type) = 3;
    } else if ( typ == KEY_BIG ) {
      *(value) = set_format_text(NULL,"%d",get_key_big(key));
      *(type) = 4;
    } else if ( typ == KEY_FLOAT ) {
      *(value) = set_format_text(NULL,"%f",get_key_float(key));
      *(type) = 5;
    } else if ( typ == KEY_TEXT ) {
      *(value) = _strdup(get_key(key));
      *(type) = 6;
    } else {
	  *(type) = 0;
      *(value) = _strdup("");
    };
  } else {
    *(value)=_strdup("Error !");
    *(type) = 0;
  };
};
////////////////////////////////////////////////////////////////////////////////
l_text get_key_display_info ( l_text key ) {
  void *data; l_uchar type;
  if ( !get_key_data(key,&type,NULL,&data) ) {
    if ( type == KEY_TEXT )    return set_format_text(NULL,"Text:\n\"%s\"",data);
    if ( type == KEY_BYTE )    return set_format_text(NULL,"Byte:\n%d",get_key_byte(key));
    if ( type == KEY_SMALL )   return set_format_text(NULL,"Small:\n%d",get_key_small(key));
    if ( type == KEY_INTEGER ) return set_format_text(NULL,"Integer:\n%d",get_key_integer(key));
    if ( type == KEY_BIG )     return set_format_text(NULL,"Big:\n%d",get_key_big(key));
    if ( type == KEY_FLOAT )   return set_format_text(NULL,"Float:\n%f",get_key_float(key));
    if ( type == KEY_LINK )    return set_format_text(NULL,"Link");
    return _strdup("No data");
  } else {
    return _strdup("Unknown key");
  };
};

////////////////////////////////////////////////////////////////////////////////
static p_list type_list ( void ) {
  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);
  if ( p ) {
      p->insert(p, new_history_item("No data" ,NULL,0,NULL));
      p->insert(p, new_history_item("Byte (8 bits)" ,NULL,0,NULL));
      p->insert(p, new_history_item("Small (16 bits)" ,NULL,0,NULL));
      p->insert(p, new_history_item("Integer (32 bits)" ,NULL,0,NULL));
      p->insert(p, new_history_item("Big (64 bits)" ,NULL,0,NULL));
      p->insert(p, new_history_item("Float (32 bits real)" ,NULL,0,NULL));
      p->insert(p, new_history_item("String (up to 255 chars)" ,NULL,0,NULL));
  };
  return p;
};

////////////////////////////////////////////////////////////////////////////////
l_bool key_prompt ( l_text *ret_name, l_text *ret_value ,l_int *ret_type, l_bool newkey ) {
  t_rect   r = rect_assign(0, 0, 400, 120);
  l_bool retvalue    = false;
  p_stattext lbl;
  p_textline name, value;
  p_history  type;
  p_button   b;
  p_appwin   w       = appwin_init(_malloc(sizeof(t_appwin)),
                            r,
                            newkey ? "Registry Editor - New Key" : "Registry Editor - Edit Key",
                            NULL,
                            ap_id,
                            NULL);

  if ( w ) VIEW(w)->align |= TX_ALIGN_CENTER;

  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(w));

  r.a.y += 20;
  lbl = stattext_init(_malloc(sizeof(t_stattext)),
                      rect_assign(r.a.x + 5, r.a.y, r.a.x + 100, r.a.y + 20 ),
                      TX_ALIGN_BOTTOM,
                      "Key name :");
  OBJECT(w)->insert(OBJECT(w), OBJECT(lbl));
  name = textline_init(_malloc(sizeof(t_textline)),
                           rect_assign(r.a.x + 105, r.a.y, r.b.x - 5, r.a.y + 20 ),
                           IO_DIR_LIMIT,
                           newkey?0:TF_REWRITEUNABLE);
  if ( !newkey ) name->text = _strdup(tree->selected->name);


  r.a.y += 25;
  lbl = stattext_init(_malloc(sizeof(t_stattext)),
                      rect_assign(r.a.x + 5, r.a.y, r.a.x + 100, r.a.y + 20 ),
                      TX_ALIGN_BOTTOM,
                      "Key value :");
  OBJECT(w)->insert(OBJECT(w), OBJECT(lbl));
  value = textline_init(_malloc(sizeof(t_textline)),
                           rect_assign(r.a.x + 105, r.a.y, r.b.x - 5, r.a.y + 20 ),
                           IO_DIR_LIMIT,
                           0);

  r.a.y += 25;
  lbl = stattext_init(_malloc(sizeof(t_stattext)),
                      rect_assign(r.a.x + 5, r.a.y, r.a.x + 100, r.a.y + 20 ),
                      TX_ALIGN_BOTTOM,
                      "Key type :");
  OBJECT(w)->insert(OBJECT(w), OBJECT(lbl));
  type = history_init(_malloc(sizeof(t_history)),
                            rect_assign(r.a.x + 105, r.a.y, r.b.x - 20, r.a.y + 20 ),
                            type_list(),
                            150,
                            HF_REWRITEUNABLE|LF_SELFLIST);

  if ( !newkey ) get_key_dat ( tree->selected->name, &value->text, &type->current);

  OBJECT(w)->insert(OBJECT(w), OBJECT(name));
  OBJECT(w)->insert(OBJECT(w), OBJECT(type));
  OBJECT(w)->insert(OBJECT(w), OBJECT(value));

  r.a.y += 25;
  b = button_init(_malloc(sizeof(t_button)),
                      rect_assign(r.a.x + 5, r.a.y, r.a.x + 100, r.a.y + 20 ),
                      TXT_OK,
                      MSG_OK,
                      BF_DEFAULT);
  OBJECT(w)->insert(OBJECT(w), OBJECT(b));

  b = button_init( _malloc(sizeof(t_button)),
                      rect_assign(r.a.x + 105, r.a.y, r.a.x + 200, r.a.y + 20 ),
                      TXT_CANCEL,
                      MSG_CLOSE,
                      BF_DEFAULT);
  OBJECT(w)->insert(OBJECT(w), OBJECT(b));

  if ( desktop->execute_view(desktop, VIEW(w)) == MSG_OK ) {
    if ( ret_name )  *(ret_name)  = _strdup(name->text);
    if ( ret_value ) *(ret_value) = _strdup(value->text);
    if ( ret_type )  *(ret_type)  = type->current;
    retvalue = true;
  };
  dispose(OBJECT(w));
  return retvalue;
};
////////////////////////////////////////////////////////////////////////////////
static void trans_ev ( p_object o, p_event event ){

  if ( /*o->process == PH_PREPROCESS &&*/ event->type & EV_MESSAGE ) {
    if ( event->message == MSG_NEWKEY || event->message == MSG_EDITKEY ) {
      p_treeitem sel = tree->selected;
      l_bool newkey = event->message == MSG_NEWKEY;
      l_text name, value;l_int type;
      clear_event(event);
      if ( !sel ) {
        msgbox( MW_INFO, MB_OK, "Please select a key first");
      } else if ( key_prompt ( &name, &value, &type, newkey ) ) {
        l_text key = newkey ? key_in_path ( sel->name, name ) : _strdup(name);

        if ( newkey ) {
          tree->add_ex(tree,sel,_strdup(key),_strdup(name),IMG_DIR16,false,&load_subs_reg);
          create_key( sel->name, name );
        };

        if ( type == 1 ) set_key_byte(key,atoi(value));
        if ( type == 2 ) set_key_small(key,atoi(value));
        if ( type == 3 ) set_key_integer(key,atol(value));
        if ( type == 4 ) set_key_big(key,atol(value));
        if ( type == 5 ) set_key_float(key,atof(value));
        if ( type == 6 ) set_key(key,_strdup(value));

        _free(name);
        _free(key);
        _free(value);

        if ( newkey ) {
          update_tree(tree);
          VIEW(tree)->draw_me(VIEW(tree));
        } else {
          l_text txt = get_key_display_info ( tree->selected->name );
          keyinfo->set_text(keyinfo, txt );
          _free(txt);
        };
      };
    } else if ( event->message == MSG_DELKEY ) {
      p_treeitem sel = tree->selected;
      clear_event(event);
      if ( !sel ) {
        msgbox( MW_INFO, MB_OK, "Please select a parent key first");
      } else if ( msgbox(MW_QUESTION, MB_YES|MB_NO, "Do you want to delete key \"%s\" and all its subkeys ?", sel->name) == MSG_YES ){
        delete_key ( sel->name );
        delete_treeitem(tree,sel);
        tree->selected = NULL;
        update_tree(tree);
        VIEW(tree)->draw_me(VIEW(tree));
      };
    } else if ( event->message == MSG_UPDATE ) {
      if ( keyinfo && tree->selected ) {
        l_text txt = get_key_display_info ( tree->selected->name );
        keyinfo->set_text(keyinfo, txt );
        _free(txt);
      };
    }
    else if (event->message == MSG_ABOUT)
    {
       seal2_app_about("Registry Editor", load_image("/system/bmp/regedit.ico,32"));

       clear_event(event);
    }
  };
};
////////////////////////////////////////////////////////////////////////////////
void init_window ( void )
{
   p_treeitem it;
   p_button b;

   t_rect   r = rect_assign(0, 0, 600, 300);

   win = appwin_init(_malloc(sizeof(t_appwin)),
                            r,
                            "Registry Editor",
                            WF_MINIMIZE|WF_MAXIMIZE,
                            ap_id,
                            &trans_ev);



   if ( win ) VIEW(win)->align |= TX_ALIGN_CENTER;

   win->icon16 = load_image("/system/bmp/regedit.ico,16");

   win->menu = standard_system_menu_items(win,
                new_menu_line(
                new_menu_item("About Registry Editor...", 0, 0, MSG_ABOUT, NULL,
   NULL)));

   OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(win));

      if ( win ) {


   tree = treeview_init(_malloc(sizeof(t_treeview)), rect_assign(5, 20, (r.b.x / 2) - 20 , r.b.y - 5));

   VIEW(tree)->align |= TX_ALIGN_BOTTOM;

   OBJECT(win)->insert(OBJECT(win), OBJECT(tree));

   b = button_init(_malloc(sizeof(t_button)),
                      rect_assign((r.b.x/2)+5, 20, (r.b.x/2)+100, 40),
                      "New Key...",
                      MSG_NEWKEY,
                      BF_DEFAULT);
  OBJECT(win)->insert(OBJECT(win), OBJECT(b));

   b = button_init(_malloc(sizeof(t_button)),
                      rect_assign((r.b.x/2)+105, 20, (r.b.x/2)+200, 40),
                      "Delete Key",
                      MSG_DELKEY,
                      BF_DEFAULT);
  OBJECT(win)->insert(OBJECT(win), OBJECT(b));

   b = button_init(_malloc(sizeof(t_button)),
                      rect_assign((r.b.x/2)+205, 20, (r.b.x/2)+300, 40),
                      "Edit Key",
                      MSG_EDITKEY,
                      BF_DEFAULT);
  OBJECT(win)->insert(OBJECT(win), OBJECT(b));

  keyinfo = stattext_init(_malloc(sizeof(t_stattext)),
                      rect_assign((r.b.x/2)+5, 45, r.b.x-5, 100),
                      TX_ALIGN_CENTERY,
                      "");
  OBJECT(win)->insert(OBJECT(win), OBJECT(keyinfo));

   tree->add(tree,_strdup("root"),_strdup(""),_strdup("Registry"),IMG_DIR16,true,NULL);
   load_treeitem_from_reg(tree,_strdup(""),_strdup(""));

      };

};
////////////////////////////////////////////////////////////////////////////////
app_begin ( void ) {


  if ( ap_process == AP_INIT ) { /* When ap start */

      AP_SETNUMOFCALLS(1); /* Set MAX of Calls */

      init_window(); /* Run the init void */

  };

  if ( ap_process == AP_FREE ) {   /*   */

  };
  if ( ap_process == AP_DONE ) {  /* When ap done */

  };
} app_end;

