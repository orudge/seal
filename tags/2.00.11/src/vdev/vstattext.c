l_text vstattext_gen_declare  ( p_devobj o ) {
  l_text name = (l_text)list_get_var(o->properties,"Name")->val->data;
  return set_format_text(NULL,"p_stattext %s = NULL;\n",name);
};

l_text vstattext_gen_loader  ( p_devobj o ) {
  t_rect r = VIEW(o)->bounds;
  l_text cap = (l_text)list_get_var(o->properties,"Caption")->val->data;
  l_text name = (l_text)list_get_var(o->properties,"Name")->val->data;
  p_devobj own = DEVOBJ(OBJECT(o)->owner);
  p_var    ownname = list_get_var(own->properties,"Name");
  l_text   owner = (ownname)?ownname->val->data:"desktop";
  return set_format_text( NULL, "%s = stattext_init(malloc(sizeof(t_stattext)),TX_ALIGN_CENTER,rect_assign(%d,%d,%d,%d),%s);
  OBJECT(%s)->insert(OBJECT(%s),OBJECT(%s));\n", name, r.a.x,r.a.y,r.b.x,r.b.y, cap ,owner,owner,name);
};


void vstattext_draw ( BITMAP *out, t_rect r, t_point p, p_devobj o ) {
  l_text caption = (l_text)list_get_var(o->properties,"Caption")->val->data;

  rectfill(out,r.a.x+p.x,r.a.y+p.y,r.b.x+p.x,r.b.y+p.y,color_3d_face);

  textout_draw_rect(out, VIEW(o)->font, caption, -1, r.a.x+p.x+2, r.a.y+p.y+2,
                             r.b.x+p.x-2, r.b.y+p.y-2, TX_ALIGN_CENTER, color_3d_text, TX_NOCOLOR, 0);
};

void vstattext_init ( p_devobj o ) {

  list_set_var( o->properties,
                "Caption",
                value_set_text("\"Caption ...\""));

  list_set_var( o->properties,
                "Name",
                value_set_text(set_format_text(NULL,"stattext%d",o->devcls->dvnb)));

};

void install_vstattext ( void ) {

  add_new_devcls("stattext",NULL,load_image("vdev/st.bmp"),&vstattext_gen_declare,&vstattext_gen_loader,NULL,NULL,&vstattext_draw,&vstattext_init);

};
