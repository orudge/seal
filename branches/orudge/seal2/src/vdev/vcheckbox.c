l_text vcheckbox_gen_declare  ( p_devobj o ) {
  l_text name = (l_text)list_get_var(o->properties,"Name")->val->data;
  l_text msg = (l_text)list_get_var(o->properties,"Message")->val->data;
  if ( !stricmp(msg,"&own") ) {
    free_message++;
    return set_format_text(NULL,"#define MSG_%s %d\np_checkbox %s = NULL;\n",name,free_message,name);
  } else
    return set_format_text(NULL,"p_checkbox %s = NULL;\n",name);
};
l_text vcheckbox_gen_loader  ( p_devobj o ) {
  t_rect r = VIEW(o)->bounds;
  l_text cap = (l_text)list_get_var(o->properties,"Caption")->val->data;
  l_text msg = (l_text)list_get_var(o->properties,"Message")->val->data;
  l_text name = (l_text)list_get_var(o->properties,"Name")->val->data;
  l_bool   checked = value_get_bool(list_get_var(o->properties,"Checked")->val);
  p_devobj own = DEVOBJ(OBJECT(o)->owner);
  p_var    ownname = list_get_var(own->properties,"Name");
  l_text   owner = (ownname)?ownname->val->data:"desktop";
  l_text   ret;
  if ( !stricmp(msg,"&own") ) msg = set_format_text(NULL,"MSG_%s",name);
  else msg = _strdup(msg);
  ret = set_format_text( NULL, "%s = checkbox_init(malloc(sizeof(t_checkbox)),rect_assign(%d,%d,%d,%d),%s,%s,%s);
  OBJECT(%s)->insert(OBJECT(%s),OBJECT(%s));\n", name, r.a.x,r.a.y,r.b.x,r.b.y, cap ,checked?"CB_CHECKED":"",msg,owner,owner,name);
  _free(msg);
  return ret;
};
l_text vcheckbox_gen_tr_event  ( p_devobj o ) {
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
void vcheckbox_draw ( BITMAP *out, t_rect r, t_point p, p_devobj o ) {
  l_text caption = (l_text)list_get_var(o->properties,"Caption")->val->data;
  l_bool   checked = value_get_bool(list_get_var(o->properties,"Checked")->val);
  l_int      cy = ( r.b.y - 12 ) / 2;
  rectfill(out,r.a.x+p.x,r.a.y+p.y,r.b.x+p.x,r.b.y+p.y,color_3d_face);
  rectfill ( out, p.x, p.y+cy, p.x+12, p.y+cy+12, color_flat_face );
  button3d ( o, out, p.x, p.y+cy, p.x+12, p.y+cy+12, 1 );
  if ( checked ) {
    l_color c =  color_flat_text;
    line(out, p.x+3, cy+p.y+3, p.x+9, cy+p.y+9, c);
    line(out, p.x+9, cy+p.y+3, p.x+3, cy+p.y+9, c);
    line(out, p.x+4, cy+p.y+3, p.x+9, cy+p.y+8, c);
    line(out, p.x+8, cy+p.y+3, p.x+3, cy+p.y+8, c);
    line(out, p.x+3, cy+p.y+4, p.x+8, cy+p.y+9, c);
    line(out, p.x+9, cy+p.y+4, p.x+4, cy+p.y+9, c);
  };
  textout_draw_rect(out, VIEW(o)->font, caption, -1, r.a.x+p.x+18, r.a.y+p.y,
                           r.b.x+p.x, r.b.y+p.y, TX_ALIGN_CENTERY, color_3d_text, TX_NOCOLOR, 0);
};

void vcheckbox_init ( p_devobj o ) {

  list_set_var( o->events,
                "On Click",
                value_set_text("/* things to do when checkbox is cliked */\n  clear_event(event); // Never forgot it ! (here)"));

  list_set_var( o->properties,
                "Caption",
                value_set_text("\"Checkbox\""));

  list_set_var( o->properties,
                "Message",
                value_set_text("&own"));

  list_set_var( o->properties,
                "Name",
                value_set_text(set_format_text(NULL,"checkbox%d",o->devcls->dvnb)));

  list_set_var( o->properties,
                "Checked",
                value_set_bool(true));
};

void install_vcheckbox ( void ) {

  add_new_devcls("checkbox",NULL,load_image("vdev/cb.bmp"),vcheckbox_gen_declare,vcheckbox_gen_loader,vcheckbox_gen_tr_event,NULL,vcheckbox_draw,vcheckbox_init);

};
