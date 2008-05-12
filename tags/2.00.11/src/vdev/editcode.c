
#include <editor.h>
#include <dialogs.h>

#include"syntax.c"

typedef struct t_editcode {

  struct t_appwin window;

  p_history select;
  p_editor  editor;
  p_devobj  linkdev;
  p_var     curev;

} t_editcode;

#define EDITCODE(o) ((p_editcode)(o))

#define MSG_ED_UNDO    10000
#define MSG_ED_SAVE    10001
#define MSG_HISSELECT  10002

p_list editcodes = NULL;

p_editcode get_edit_code ( p_devobj o ) {
  if ( editcodes && editcodes->last ) {
    p_item x = editcodes->first(editcodes);
    p_item f = x;
    if  ( f )
    do {
       if ( o == EDITCODE(x->rec)->linkdev ) return EDITCODE(x->rec);
       x = x->next;
    } while ( x != f );
   };
  return NULL;
};

void add_editcode ( p_editcode o ) {
  if ( !editcodes ) editcodes = list_init(_malloc(sizeof(t_list)),0,0);
  editcodes->insert(editcodes,o);
};

void del_editcode ( p_editcode o ) {
  l_int id = editcodes->index_of(editcodes,o);
  editcodes->remove_index(editcodes,id);
};

void editcode_trans_ev ( p_object o, t_event *event ) {
  if ( o->process == PH_PREPROCESS && event->type & EV_MESSAGE ) {
    l_dword msg = event->message;
    if ( msg == MSG_CLOSE || msg == MSG_HISSELECT || msg == MSG_ED_SAVE ) {
      p_editcode ed = EDITCODE(o);
      if ( ed->curev && ed->editor && ed->curev->val ) {
        if ( ed->curev->val->data ) _free ( ed->curev->val->data );
          ed->curev->val->data = (void*)_strdup(ed->editor->text);

      };
      if ( msg == MSG_ED_SAVE )
        clear_event(event);
      if ( msg == MSG_HISSELECT ) {
        if ( ed->linkdev && ed->linkdev->events && ed->select ) {
          ed->curev = ((p_var)(ed->linkdev->events->at(ed->linkdev->events,ed->select->current)));
          ed->editor->set_text(ed->editor, (l_text)ed->curev->val->data);
        };
        clear_event(event);
      };
    };
    if ( msg == MSG_ED_UNDO ) event->message = MSG_CLOSE;
  };
};

static p_list get_events_items ( p_devobj o ) {

  if ( o && o->events && o->events->last ) {

    p_list p = list_init(_malloc(sizeof(t_list)), &free_history_item, 0);

    if ( p ) {
      p_item x = o->events->first(o->events);
      p_item f = x;

      if  ( f )
        do {
          p_var var = ((p_var)(x->rec));

           p->insert(p, new_history_item(var->name, NULL, 0, NULL));

           x = x->next;

        } while ( x != f );
      return p;
    };
  };
  return NULL;
};

l_bool editcode_done ( p_object o ) {

  del_editcode(EDITCODE(o));

  return win_done(o);

};

void make_editcode ( p_editcode o, t_rect r, p_devobj linkdev ) {
  p_button b;
  l_text cap;
  p_var var_n = list_get_var(linkdev->properties,"Name");

  if ( !o ) return ;
  if ( var_n )
  cap = set_format_text(NULL,"Code Editor - %s",var_n->val->data);
  else
  cap = _strdup("Code editor");
  appwin_init( APPWIN(o), r, cap, WF_MINIMIZE, NULL, &editcode_trans_ev );
  _free(cap);
  o->linkdev = linkdev;
  OBJECT(o)->done = &editcode_done;
  VIEW(o)->align |= TX_ALIGN_CENTER;

  OBJECT(desktop)->insert(OBJECT(desktop),OBJECT(o));

  r = rect_move(r,-r.a.x,-r.a.y);

  o->editor = EDITOR( syntax_C_init( _malloc(sizeof(t_syntax)),
                           rect_assign(5,45,r.b.x-20,r.b.y-20),
                           0 ) );

  OBJECT(o)->insert(OBJECT(o),OBJECT(o->editor));

  o->curev = ((p_var)(o->linkdev->events->at(o->linkdev->events,0)));

  if ( o->curev ) {
    o->editor->set_text(o->editor, (l_text)o->curev->val->data);
  };

  o->select = history_init( _malloc(sizeof(t_history)),
                         rect_assign( 5, 20, 185, 40 ),
                         get_events_items(linkdev) ,
                         150,
                         HF_REWRITEUNABLE|LF_SELFLIST);

  o->select->message = MSG_HISSELECT;

  OBJECT(o)->insert(OBJECT(o),OBJECT(o->select));

  b = button_init(_malloc(sizeof(t_appwin)),rect_assign(205,20,300,40),TXT_CANCEL,MSG_ED_UNDO,BF_DEFAULT);
  OBJECT(o)->insert(OBJECT(o), OBJECT(b));

  add_editcode(o);
};
