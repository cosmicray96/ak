#ifndef ak_gfx_mtrl_mtrl_h
#define ak_gfx_mtrl_mtrl_h

#include "ak/gfx/mtrl_t.h"

#define ak_as_mtrl_e(name) ak_mtrl_##name##_e
#define ak_as_mtrl_t(name)                  \
  ak_##name##_t // same as
                // ak_as_comp_t(name)

typedef enum
{
#define ak_d_mtrl_x(name) ak_as_mtrl_e(name),
#include "ak/gfx/mtrl.inc"
#undef ak_d_mtrl_x
  ak_mtrl_count_e
} ak_mtrl_enum;

#endif
