#include <bmps.h>
////////////////////////////////////////////////////////////////////////////////
// This function generate the c source code to declare the appwin with the 'o' properties
l_text vappwin_gen_declare  ( p_devobj o ) {
  l_text name = (l_text)list_get_var(o->properties,"Name")->val->data;
  return set_format_text(NULL,"p_appwin %s = NULL;\n",name);
};
////////////////////////////////////////////////////////////////////////////////
// This function generate the c source code to load the appwin with the 'o' properties
l_text vappwin_gen_loader  ( p_devobj o ) {
  l_text   txt   = NULL;
  t_rect   r     = VIEW(o)->bounds;
  l_text   cap   = (l_text)list_get_var(o->properties,"CAPTION")->val->data;
  l_text   name  = (l_text)list_get_var(o->properties,"Name")->val->data;
  l_bool   maxbt = value_get_bool(list_get_var(o->properties,"Max button")->val);
  l_bool   minbt = value_get_bool(list_get_var(o->properties,"Min button")->val);
  l_bool   abtbt = value_get_bool(list_get_var(o->properties,"About button")->val);
  l_bool   sysm  = value_get_bool(list_get_var(o->properties,"Sys menu button")->val);
  l_text   flags = set_format_text(NULL,"0%s%s%s%s",(maxbt)?"|WF_MAXIMIZE":"",(minbt)?"|WF_MINIMIZE":"",(abtbt)?"|WF_ABOUT":"",(sysm)?"|WF_SYSMENU":"");
  l_bool   centr  = value_get_bool(list_get_var(o->properties,"Center on load")->val);
  p_devobj own = DEVOBJ(OBJECT(o)->owner);
  p_var    ownname = list_get_var(own->properties,"Name");
  l_text   owner = (ownname)?ownname->val->data:"desktop";
  txt= set_format_text( NULL, "%s = appwin_init(_malloc(sizeof(t_appwin)),rect_assign(%d,%d,%d,%d),%s,%s,ap_id,&global_tr_event);
  %s
  OBJECT(%s)->insert(OBJECT(%s),OBJECT(%s));\n", name, r.a.x,r.a.y,r.b.x,r.b.y, cap,flags,(centr)?set_format_text(NULL,"VIEW(%s)->align |= TX_ALIGN_CENTER;",name):"",owner,owner,name);
  _free(flags);
  return txt;
};
////////////////////////////////////////////////////////////////////////////////
void vappwin_draw ( BITMAP *out, t_rect r, t_point p, p_devobj o ) {
  l_text caption = (l_text)list_get_var(o->properties,"Caption")->val->data;
  l_bool maxbt   = value_get_bool(list_get_var(o->properties,"Max button")->val);
  l_bool minbt   = value_get_bool(list_get_var(o->properties,"Min button")->val);
  l_bool abtbt   = value_get_bool(list_get_var(o->properties,"About button")->val);
  l_bool sysm    = value_get_bool(list_get_var(o->properties,"Sys menu button")->val);
  l_int  i       = 2;


  rectfill(out,r.a.x+p.x,r.a.y+p.y,r.b.x+p.x,r.b.y+p.y,color_3d_face);
  button3d(VIEW(o),out,r.a.x+p.x,r.a.y+p.y,r.b.x+p.x,r.b.y+p.y,0);

  r.b.y = r.a.y + FONT_GETHEIGHT(VIEW(o)->font)+4;
  r.a.x+=2;
  r.a.y+=2;
  r.b.x-=2;
  fade_rect(out, r.a.x+p.x, r.a.y+p.y, r.b.x+p.x, r.b.y+p.y, color_window_act_title_face, color_window_act_title_face_gardient, FR_HOR);

  textout_draw_rect(out, VIEW(o)->font, caption, -1, r.a.x+p.x+2, r.a.y+p.y+2,
                             r.b.x+p.x-2, r.b.y+p.y-2, TX_ALIGN_CENTER, color_window_act_title_text, TX_NOCOLOR, 0);

  if ( sysm ) draw_sprite(out,IMG_APP16,p.x+2,p.y+1);

  draw_sprite(out,IMG_WCLOSE,p.x+r.b.x-18,p.y+2);

  if ( maxbt ) { draw_sprite(out,IMG_WMAX,p.x+r.b.x-(18*i),p.y+2); i++; };
  if ( minbt ) { draw_sprite(out,IMG_WMIN,p.x+r.b.x-(18*i),p.y+2); i++; };
  if ( abtbt ) { draw_sprite(out,IMG_ABOUT,p.x+r.b.x-(18*i),p.y+2); i++; };

};
////////////////////////////////////////////////////////////////////////////////
void vappwin_init ( p_devobj o ) {

  list_set_var( o->properties,
                "Caption",
                value_set_text("\"--- Window ----\""));

  list_set_var( o->properties,
                "Name",
                value_set_text(set_format_text(NULL,"appwin%d",o->devcls->dvnb)));

  list_set_var( o->properties,
                "Max button",
                value_set_bool(true));

  list_set_var( o->properties,
                "Min button",
                value_set_bool(true));

  list_set_var( o->properties,
                "About button",
                value_set_bool(true));

  list_set_var( o->properties,
                "Sys menu button",
                value_set_bool(true));

  list_set_var( o->properties,
                "Center on load",
                value_set_bool(true));
};
////////////////////////////////////////////////////////////////////////////////
void install_vappwin ( void ) {

  add_new_devcls("appwin",NULL,load_image("vdev/win.bmp"),&vappwin_gen_declare,&vappwin_gen_loader,NULL,NULL,vappwin_draw,vappwin_init);

};
