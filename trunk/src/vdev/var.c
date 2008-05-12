#include"var.h"

p_value value_set_data ( l_word type, void *data ) {
  p_value o = malloc(sizeof(t_value));
  o->type = type;
  o->data = data;
  return o;
};



void free_value ( p_value o ) {
  if ( o ) {
    if ( o->type < VAL_UBIG ) if ( o->data ) _free(o->data);
    _free(o);
  };
};

p_value value_set_char ( l_char v ) {
  l_char *o = _malloc(sizeof(l_char));
  memcpy(o,&v,sizeof(l_char));
  return value_set_data(VAL_CHAR,o);
};
p_value value_set_int ( l_int v ) {
  l_int *o = _malloc(sizeof(l_int));
  memcpy(o,&v,sizeof(l_int));
  return value_set_data(VAL_INT,o);
};
p_value value_set_long ( l_long v ) {
  l_long *o = _malloc(sizeof(l_long));
  memcpy(o,&v,sizeof(l_long));
  return value_set_data(VAL_LONG,o);
};
p_value value_set_big ( l_big v ) {
  l_big *o = _malloc(sizeof(l_big));
  memcpy(o,&v,sizeof(l_big));
  return value_set_data(VAL_BIG,o);
};
p_value value_set_color ( l_color v ) {
  l_color *o = _malloc(sizeof(l_color));
  memcpy(o,&v,sizeof(l_color));
  return value_set_data(VAL_COLOR,o);
};
p_value value_set_bool ( l_bool v ) {
  l_bool *o = _malloc(sizeof(l_bool));
  memcpy(o,&v,sizeof(l_bool));
  return value_set_data(VAL_BOOL,o);
};
p_value value_set_word ( l_word v ) {
  l_long *o = _malloc(sizeof(l_word));
  memcpy(o,&v,sizeof(l_word));
  return value_set_data(VAL_WORD,o);
};
p_value value_set_ubig ( l_ubig v ) {
  l_ubig *o = _malloc(sizeof(l_ubig));
  memcpy(o,&v,sizeof(l_ubig));
  return value_set_data(VAL_UBIG,o);
};

p_var new_var ( l_text name, p_value val ) {
  p_var o = _malloc(sizeof(t_var));
  o->name = _strdup(name);
  o->val  = val;
  return o;
};

void free_var ( void *p ) {
  if ( p ) {
    p_var o = ((p_var)(p));
    if ( o->val ) free_value(o->val);
    if ( o->name ) _free(o->name);
    _free(o);
  };
};

p_var list_get_var ( p_list o, l_text name ) {

  if ( o && o->last && name ) {

    p_item x = o->first(o);
    p_item f = x;

    if  ( f )
    do {
       p_var var = ((p_var)(x->rec));

       if ( !stricmp(var->name,name) ) return var;

       x = x->next;

    } while ( x != f );
  };

  return NULL;
};

void list_set_var ( p_list o, l_text name, p_value val ) {
  p_var var;
  if ( !o || !name || !val ) return;
  var = list_get_var(o,name);
  if ( !var ) {
    var = new_var(name,val);
    o->insert(o,var);
  } else {
    if ( var->val ) free_value ( var->val );
    var->val = val;
  };
};
