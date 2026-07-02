#ifndef ak_gfx_core_h
#define ak_gfx_core_h

#include "ak/core/math/mat3x3.h"
#include "ak/core/math/vec2f.h"
#include "ak/core/math/vec4f.h"
#include "ak/res/core.h"

#include <stdint.h>

typedef struct ak_rctx ak_rctx;

typedef struct ak_shader ak_shader;
typedef struct ak_tex ak_tex;

typedef struct
{
  ak_vec4f col;
  ak_vec2f uv_min;
  ak_vec2f uv_max;
} ak_gfx_quaddata;

typedef struct
{
  ak_resid shaderid;
  ak_resid tex;
} ak_gfx_calldata;

typedef struct
{
  ak_mat3_f vp;
  float time;
} ak_gfx_batchdata;

#endif
