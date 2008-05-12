l_text vhistory_gen_declare  ( p_devobj o ) {
  l_text name = (l_text)list_get_var(o->properties,"Name")->val->data;
  l_text msg = (l_text)list_get_var(o->properties,"Message")->val->data;
  if ( !stricmp(msg,"&own") ) {
    free_message++;
    return set_format_text(NULL,"#define MSG_%s %d\np_history %s = NULL;\n",name,free_message,name);
  } else
    return set_format_text(NULL,"p_history %s = NULL;\n",name);
};
l_text vhistory_gen_loader  ( p_devobj o ) {
  t_rect r = VIEW(o)->bounds;
  l_text msg = (l_text)list_get_var(o->properties,"Message")->val->data;
  l_text name = (l_text)list_get_var(o->properties,"Name")->val->data;
  p_devobj own = DEVOBJ(OBJECT(o)->owner);
  p_var    ownname = list_get_var(own->properties,"Name");
  l_text   owner = (ownname)?ownname->val->data:"desktop";
  l_text ret;
  if ( !stricmp(msg,"&own") ) msg = set_format_text(NULL,"MSG_%s",name);
  else msg = _strdup(msg);
  ret = set_format_text( NULL, " %s  = history_init( _malloc(sizeof(t_history)),rect_assign(%d,%d,%d,%d),&%s_list_generation ,150,HF_REWRITEUNABLE|LF_SELFLIST);
  OBJECT(%s)->insert(OBJECT(%s),OBJECT(%s));\n", name, r.a.x,r.a.y,r.b.x-15,r.b.y, name,owner,owner,name);
  _free(msg);
  return ret;
};
l_text vhistory_gen_tr_event  ( p_devobj o ) {
  l_text c = (l_text)list_get_var(o->events,"On Select")->val->data;
  l_text msg = (l_text)list_get_var(o->properties,"Message")->val->data;
  l_text ret;
  if ( !stricmp(msg,"&own") ) msg = set_format_text(NULL,"MSG_%s",(l_text)list_get_var(o->properties,"Name")->val->data);
  else msg = _strdup(msg);
  return join_text( "if ( event->type & EV_MESSAGE && event->message == ", join_text( msg, join_text( " ) {\n  ", join_text( c, "\n};", 0, 0), 0, 1), 1, 1), 0, 1 );
};
l_text vhistory_gen_others  ( p_devobj o ) {
  l_text c = (l_text)list_get_var(o->events,"List Generation")->val->data;
  l_text name = (l_text)list_get_var(o->properties,"Name")->val->data;
  return join_text( "l_list ", join_text( name, join_text( "_list_generation {\n", join_text( c, "\n};", 0, 0), 0, 1), 0, 1), 0, 1 );
};

void vhistory_draw ( BITMAP *out, t_rect r, t_point p, p_devobj o ) {


  rectfill(out,r.a.x+p.x,r.a.y+p.y,r.b.x+p.x-15,r.b.y+p.y,color_flat_face);
  button3d(VIEW(o),out,r.a.x+p.x,r.a.y+p.y,r.b.x+p.x-15,r.b.y+p.y,1);

  rectfill(out,r.b.x+p.x-15,r.a.y+p.y,r.b.x+p.x,r.b.y+p.y,color_3d_face);
  button3d(VIEW(o),out,r.b.x+p.x-15,r.a.y+p.y,r.b.x+p.x,r.b.y+p.y,0);

  textout_draw_rect( out, get_font_in_size("Symbols", 8, 8), "g", -1,
                     r.b.x+p.x-13,r.a.y+p.y,r.b.x+p.x,r.b.y+p.y,
                     TX_ALIGN_CENTER, color_3d_text, TX_NOCOLOR, 0);

};

void vhistory_init ( p_devobj o ) {

  list_set_var( o->events,
                "On Select",
                value_set_text("/* things to do when something is select in history */\n  clear_event(event); // Never forgot it ! (here)"));

  list_set_var( o->events,
                "List Generation",
                value_set_text("  p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);
  if ( p ) {
     p->insert(p, new_history_item(\"item 1\", NULL, 0, NULL));
  };
  return p; "));


  list_set_var( o->properties,
                "Name",
                value_set_text(set_format_text(NULL,"history%d",o->devcls->dvnb)));

  list_set_var( o->properties,
                "Message",
                value_set_text("&own"));

};

void install_vhistory ( void ) {

  add_new_devcls("history",NULL,load_image("vdev/his.bmp"),vhistory_gen_declare,vhistory_gen_loader,vhistory_gen_tr_event,vhistory_gen_others,vhistory_draw,vhistory_init);

};
