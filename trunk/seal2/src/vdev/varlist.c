
#include <inputbox.h>

typedef struct t_varlist {

  struct t_view obclass;


  p_devobj devlink;

  p_var selected;

} t_varlist;

#define VARLIST(o) ((p_varlist)(o))

////////////////////////////////////////////////////////////////////////////////
void varlist_draw ( p_view o ) {

  t_rect   r  = o->size_limits(o);
  t_point  p;
  p_devobj dv = DEVOBJ(o);

  BITMAP *out = o->begin_paint(o, &p, r);

  if ( out ) {
    p_varlist vr = VARLIST(o);

    rectfill(out,r.a.x+p.x,r.a.y+p.y,r.b.x+p.x,r.b.y+p.y,color_flat_face);

    button3d(VIEW(o),out,r.a.x+p.x,r.a.y+p.y,r.b.x+p.x,r.b.y+p.y,1);

    if ( vr->devlink && vr->devlink->properties ) {
      p_item x = vr->devlink->properties->first(vr->devlink->properties);
      p_item f = x;
      l_int a = 0;
      if  ( f )
        do {
          p_var var = ((p_var)(x->rec));
          l_color tx = ( vr->selected == var ) ? color_selected_text : color_flat_text ;

          if ( vr->selected == var ) {
            rectfill(out, p.x+2, 2+p.y+(a*15), p.x+r.b.x-2, p.y+(a*15)+16, color_selected_face);
          };

          textout_draw_rect(out, VIEW(o)->font, var->name, -1, p.x+2, 2+p.y+(a*15),
                             p.x+(r.b.x/2)-2, p.y+(a*15)+16, TX_ALIGN_LEFT, tx, TX_NOCOLOR, 1);

          if ( var->val->type == VAL_TEXT ) {
            textout_draw_rect(out, VIEW(o)->font, (l_text)var->val->data, -1, p.x+(r.b.x/2)+1, 2+p.y+(a*15),
                             p.x+r.b.x-2, p.y+(a*15)+16, TX_ALIGN_LEFT, tx, TX_NOCOLOR, 1);
          } else if ( var->val->type == VAL_BOOL ) {
            textout_draw_rect(out, VIEW(o)->font, (*(l_bool*)var->val->data)?"True":"False", -1, p.x+(r.b.x/2)+1, 2+p.y+(a*15),
                             p.x+r.b.x-2, p.y+(a*15)+16, TX_ALIGN_LEFT, tx, TX_NOCOLOR, 1);
          } else {
            textout_draw_rect(out, VIEW(o)->font, "(unknow)", -1, p.x+(r.b.x/2)+1, 2+p.y+(a*15),
                             p.x+r.b.x-2, p.y+(a*15)+16, TX_ALIGN_LEFT, tx, TX_NOCOLOR, 1);
          };

          vline(out,p.x+(r.b.x/2)-1,2+p.y+(a*15), p.y+(a*15)+16,color_3d_face);

          hline(out,p.x+2, p.y+(a*15)+16,p.x+r.b.x-2,color_3d_face);

          x = x->next;
          a++;
        } while ( x != f );
    };


  };

  o->end_of_paint(o, r);
};
void varlist_set_obj ( p_varlist o, p_devobj devlink ) {

  o->devlink = devlink;
  o->selected = NULL;
  VIEW(o)->draw_me(VIEW(o));
};
////////////////////////////////////////////////////////////////////////////////
void varlist_translate_event ( p_object o, p_event event ) {
  view_translate_event(o, event);
  if ( event->type & EV_MOUSE ) {
    p_varlist vr = VARLIST(o);
    if (  OBJECT(mouse)->state & MO_SF_MOUSELDOWN && vr->devlink && vr->devlink->properties ) {
      t_point  p   = VIEW(o)->get_local_point(VIEW(o), mouse->where);
      p_var    var;
      p.y -=2;
      var =  vr->devlink->properties->at(vr->devlink->properties,p.y/15);
      if ( var != vr->selected ) {
        vr->selected = var;
        VIEW(o)->draw_me(VIEW(o));
      };
      clear_event(event);
    };
    if ( OBJECT(mouse)->state & MO_SF_MOUSELDOUBLE ) {
      if ( vr->selected && vr->devlink ) {
        if ( vr->selected->val->type == VAL_TEXT ) {
          l_text ret = inputbox("Set a propertie","Please enter the value. It's c souces code, so use \" for texts",(l_text)vr->selected->val->data);
          if ( ret ) vr->selected->val->data = ret;
        } else if ( vr->selected->val->type == VAL_BOOL ) {
          (*(l_bool*)vr->selected->val->data) = (*(l_bool*)vr->selected->val->data)?0:1;
        };
        VIEW(vr->devlink)->draw_me(VIEW(vr->devlink));
        VIEW(o)->draw_me(VIEW(o));
        clear_event(event);
      };
    };
  };
};

p_varlist varlist_init ( p_varlist o, t_rect r, p_devobj devlink ) {

  if ( !o ) return NULL;

  clear_type(o, sizeof(t_varlist));

  view_init(VIEW(o), r);


  VIEW(o)->draw              = &varlist_draw;
  OBJECT(o)->translate_event = &varlist_translate_event;


  o->devlink = devlink;
  o->selected = NULL;

  return o;
};
