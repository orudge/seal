#ifndef _DEV_VAR_H_
#define _DEV_VAR_H_

typedef struct t_value {
  l_word  type;
  void   *data;
} t_value, *p_value;

typedef struct t_var {
  l_text  name;
  p_value val;
} t_var, *p_var;


#define VAL_TEXT   1
#define VAL_CHAR   2
#define VAL_INT    3
#define VAL_LONG   4
#define VAL_BIG    5
#define VAL_COLOR  6
#define VAL_BOOL   7
#define VAL_WORD   8
#define VAL_DWORD  9
#define VAL_UBIG   10
#define VAL_BITMAP 11
#define VAL_OBJECT 12
#define VAL_FONT   13

p_value value_set_data ( l_word type, void *data );

#define value_set_text(v)   value_set_data(VAL_TEXT,_strdup(v))
p_value value_set_char  ( l_char v );
p_value value_set_int   ( l_int v );
p_value value_set_long  ( l_long v );
p_value value_set_big   ( l_big v );
p_value value_set_color ( l_color v );
p_value value_set_bool  ( l_bool v );
p_value value_set_word  ( l_word v );
p_value value_set_ubig  ( l_ubig v );
#define value_set_bitmap(v) value_set_data(VAL_BITMAP,(v))
#define value_set_object(v) value_set_data(VAL_OBJECT,(v))
#define value_set_font(v)   value_set_data(VAL_FONT,(v))

p_var new_var ( l_text name, p_value val );

#define var_set_value(o,val) (o)->val=(val)
#define vat_get_value(o)     ((o)->val)

#define value_get_text(o) ((o)->data)
#define value_get_char(o) (*(l_char*)((o)->data))
#define value_get_bool(o) (*(l_bool*)((o)->data))

#define value_get_dword(o) (*(l_dword*)((o)->data))
#define value_set_dword(o) value_set_long((l_long)(l_dword)(o))

p_var list_get_var ( p_list o, l_text name );
void list_set_var ( p_list o, l_text name, p_value val );

#endif
