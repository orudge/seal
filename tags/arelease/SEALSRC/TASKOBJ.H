#ifndef __TASKOBJ_H_INCLUDED__
#define __TASKOBJ_H_INCLUDED__


typedef struct t_taskbar *p_taskbar;

typedef struct t_taskbar {


  struct t_view    obclass;

  void     (*draw_taskbar) ( p_taskbar o );


} t_taskbar;

#define taskbar(o)   ((p_taskbar)(o))

void taskbar_translate_event ( p_object o, t_event *event );
void taskbar_func_callback ( p_object o );
void taskbar_draw_taskbar ( p_taskbar o );
void taskbar_draw ( p_view o );
l_long get_num_of_windows();

p_taskbar _taskbar_init ( p_taskbar o, t_rect r );
extern p_taskbar (*taskbar_init) ( p_taskbar o, t_rect r );




#endif

