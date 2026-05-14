ak_d_comp_x(tf2d)
ak_d_comp_x(gmat3)
ak_d_comp_x(camera)
ak_d_comp_x(screen)
ak_d_comp_x(rect)
ak_d_comp_x(mtrl)

#define ak_d_mtrl_x(name) ak_d_comp_x(name) 
#include "ak/gfx/mtrl.inc"
#undef ak_d_mtrl_x

#define ak_d_mtrl_x(name) ak_d_comp_x(name##_base) 
#include "ak/gfx/mtrl.inc"
#undef ak_d_mtrl_x
