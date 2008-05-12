l_text vbutton_gen_declare  ( p_devobj o ) {
  l_text name = (l_text)list_get_var(o->properties,"Name")->val->data;
  l_text msg = (l_text)list_get_var(o->properties,"Message")->val->data;
  if ( !stricmp(msg,"&own") ) {
    free_message++;
    return set_format_text(NULL,"#define MSG_%s %d\np_button %s = NULL;\n",name,free_message,name);
  } else
    return set_format_text(NULL,"p_button %s = NULL;\n",name);
};
l_text vbutton_gen_loader  ( p_devobj o ) {
  t_rect r = VIEW(o)->bounds;
  l_text cap = (l_text)list_get_var(o->properties,"Caption")->val->data;
  l_text msg = (l_text)list_get_var(o->properties,"Message")->val->data;
  l_text name = (l_text)list_get_var(o->properties,"Name")->val->data;
  p_devobj own = DEVOBJ(OBJECT(o)->owner);
  p_var    ownname = list_get_var(own->properties,"Name");
  l_text   owner = (ownname)?ownname->val->data:"desktop";
  l_text   ret;
  if ( !stricmp(msg,"&own") ) msg = set_format_text(NULL,"MSG_%s",name);
  else msg = _strdup(msg);
  ret = set_format_text( NULL, "%s = button_init(malloc(sizeof(t_button)),rect_assign(%d,%d,%d,%d),%s,%s,BF_DEFAULT);
  OBJECT(%s)->insert(OBJECT(%s),OBJECT(%s));\n", name, r.a.x,r.a.y,r.b.x,r.b.y, cap ,msg,owner,owner,name);
  _free(msg);
  return ret;
};
l_text vbutton_gen_tr_event  ( p_devobj o ) {
  l_text c = (l_text)list_get_var(o->events,"On Click")->val->data;
  l_text msg = (l_text)list_get_var(o->properties,"Message")->val->data;
  //l_text ret;
  if ( !stricmp(msg,"&own") ) msg = set_format_text(NULL,"MSG_%s",(l_text)list_get_var(o->properties,"Name")->val->data);
  else msg = _strdup(msg);
  //ret = set_format_text( NULL, "if ( event->type & EV_MESSAGE && event->message == %s ) {\n  %s\n};" ,msg,c);
  //_free(msg);
  //return ret;
  return join_text( "if ( event->type & EV_MESSAGE && event->message == ", join_text( msg, join_text( " ) {\n  ", join_text( c, "\n};", 0, 0), 0, 1), 1, 1), 0, 1 );

};
void vbutton_draw ( BITMAP *out, t_rect r, t_point p, p_devobj o ) {
  l_text caption = (l_text)list_get_var(o->properties,"Caption")->val->data;

  rectfill(out,r.a.x+p.x,r.a.y+p.y,r.b.x+p.x,r.b.y+p.y,color_3d_face);
  button3d(VIEW(o),out,r.a.x+p.x,r.a.y+p.y,r.b.x+p.x,r.b.y+p.y,0);

  textout_draw_rect(out, VIEW(o)->font, caption, -1, r.a.x+p.x+2, r.a.y+p.y+2,
                             r.b.x+p.x-2, r.b.y+p.y-2, TX_ALIGN_CENTER, color_3d_text, TX_NOCOLOR, 0);
};

void vbutton_init ( p_devobj o ) {

  list_set_var( o->events,
                "On Click",
                value_set_text("/* things to do when button is cliked */\n  clear_event(event); // Never forgot it ! (here)"));

  list_set_var( o->properties,
                "Caption",
                value_set_text("\"Button\""));

  list_set_var( o->properties,
                "Message",
                value_set_text("&own"));

  list_set_var( o->properties,
                "Name",
                value_set_text(set_format_text(NULL,"button%d",o->devcls->dvnb)));

};

void install_vbutton ( void ) {

  add_new_devcls("button",NULL,load_image("vdev/bt.bmp"),vbutton_gen_declare,vbutton_gen_loader,vbutton_gen_tr_event,NULL,vbutton_draw,vbutton_init);

};
