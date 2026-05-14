#ifndef ak_gfx_mtrl_mtrl_h
#define ak_gfx_mtrl_mtrl_h

#include "ak/core/math/vec2.h"
#include "ak/core/math/vec4.h"
#include "ak/gfx/core.h"
#define ak_as_mtrl_e(name) ak_mtrl_##name##_e

typedef enum
{
#define ak_d_mtrl_x(name) ak_as_mtrl_e(name),
#include "ak/gfx/mtrl.inc"
#undef ak_d_mtrl_x
  ak_mtrl_count_e
} ak_mtrl_enum;

typedef struct
{
  ak_mtrl_enum me;
  ak_vec4 col;
  ak_vec2 uv_min;
  ak_vec2 uv_max;
} ak_mtrl_quaddata;

typedef struct
{
  ak_gresid rid;
} ak_mtrl_basedata;

#endif
