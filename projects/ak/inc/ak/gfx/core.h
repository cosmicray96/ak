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
  ak_gresid gid;
  enum
  {
    ak_uv_clamp,
    ak_uv_repeat
  } uv_type;
  enum
  {
    ak_filter_nearest,
    ak_filter_linear
  } filter_type;
} ak_tex_old;

typedef struct
{
  ak_vec4f col;
  ak_vec2f uv_min;
  ak_vec2f uv_max;
} ak_mtrl_quaddata;

typedef struct
{
  ak_gresid shaderid;
  ak_tex_old tex;
} ak_mtrl_basedata;

typedef struct
{
  ak_mat3_f vp;
  float time;
} ak_mtrl_indata;

#endif
