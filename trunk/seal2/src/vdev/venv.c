l_text venv_gen_declare  ( p_devobj o ) {
l_text   apn   = (l_text)list_get_var(o->properties,"InfoAppName")->val->data;
l_text   apd   = (l_text)list_get_var(o->properties,"InfoDesciption")->val->data;
l_text   apc   = (l_text)list_get_var(o->properties,"InfoCopyright")->val->data;
l_text   apm   = (l_text)list_get_var(o->properties,"InfoManufacturer")->val->data;

return set_format_text(NULL,"SetInfoAppName(%s);
SetInfoDesciption(%s);
SetInfoCopyright(%s);
SetInfoManufacturer(%s);\n",apn,apd,apc,apm);
};

void venv_draw ( BITMAP *out, t_rect r, t_point p, p_devobj o ) {

  l_text   apn   = (l_text)list_get_var(o->properties,"InfoAppName")->val->data;

  rectfill(out,r.a.x+p.x,r.a.y+p.y,r.b.x+p.x,r.b.y+p.y,color_desktop_face);

  button3d(VIEW(o),out,r.a.x+p.x,r.a.y+p.y,r.b.x+p.x,r.b.y+p.y,1);

  textout_draw_rect(out, VIEW(o)->font, set_format_text(NULL,"%s - VDEV 1.00",apn), -1, r.a.x+p.x+6, r.a.y+p.y+6,
                             r.b.x+p.x-2, r.b.y+p.y-2, TX_ALIGN_LEFT, color_desktop_text, TX_NOCOLOR, 0);
};

void venv_init ( p_devobj o ) {

  list_set_var( o->properties,
                "InfoAppName",
                value_set_text("\"My Seal App\""));
  list_set_var( o->properties,
                "InfoDesciption",
                value_set_text("\"My Seal App\""));
  list_set_var( o->properties,
                "InfoCopyright",
                value_set_text("\"(c) Copyright YYYY Me. All rights rserved.\""));
  list_set_var( o->properties,
                "InfoManufacturer",
                value_set_text("\"Me\""));
  /*list_set_var( o->properties,
                "Name",
                value_set_text("desktop"));*/
};

void install_venv ( void ) {

  add_new_devcls("Application",NULL,NULL,&venv_gen_declare,NULL,NULL,NULL,&venv_draw,&venv_init);

};
