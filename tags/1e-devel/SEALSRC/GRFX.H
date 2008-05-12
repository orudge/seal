#ifndef __TASKOBJ_H_INCLUDED__
#define __TASKOBJ_H_INCLUDED__


typedef struct t_grfx *p_grfx;

typedef struct t_grfx {

  struct t_view    obclass;

} t_grfx;

#define grfx(o)   ((p_grfx)(o))

p_grfx _grfx_init ( p_grfx o, t_rect r );
extern p_grfx (*grfx_init) ( p_grfx o, t_rect r );


#endif

