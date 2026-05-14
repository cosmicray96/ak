#ifndef ak_gfx_mtrl_t_h
#define ak_gfx_mtrl_t_h

#include "ak/core/math/mat3x3.h"
#include "ak/core/math/vec2.h"
#include "ak/core/math/vec4.h"
#include "ak/gfx/core.h"

typedef struct
{
  ak_vec4 col;
} ak_mtrl_col_t;

typedef struct
{
  char ph;
} ak_mtrl_col_base_t;

typedef struct
{
  ak_vec2 uv_min;
  ak_vec2 uv_max;
} ak_mtrl_tex_t;

typedef struct
{
  ak_mat3x3 vp;
  ak_gresid gid;
} ak_mtrl_tex_base_t;

#endif
