#ifndef ak_gfx_core_h
#define ak_gfx_core_h

#include "ak/core/math/mat3x3.h"
#include "ak/core/math/vec2f.h"
#include "ak/core/math/vec4f.h"
#include <stdint.h>

// typedef uint32_t ak_gresid;
typedef struct ak_gfx ak_gfx;

typedef uint32_t ak_gresid;

typedef struct ak_shader ak_shader;

typedef struct
{
  ak_vec4f col;
  ak_vec2f uv_min;
  ak_vec2f uv_max;
} ak_mtrl_quaddata;

typedef struct
{
  ak_gresid shaderid;
  ak_gresid tex;
} ak_mtrl_basedata;

typedef struct
{
  ak_mat3_f vp;
  float time;
} ak_mtrl_indata;

#endif
