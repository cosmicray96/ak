#ifndef ak_gfx_mtrl_h
#define ak_gfx_mtrl_h

#include "ak/core/math/fixed.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/math/vec2.h"
#include "ak/core/math/vec4f.h"
#include "ak/gfx/core.h"

#include <stdint.h>

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
} ak_tex;

typedef struct
{
  ak_mtrl_enum me;
  union
  {
    struct
    {
      ak_vec4f col;
    } ui;
    struct
    {
      ak_vec4f col;
    } col;
    struct
    {
      ak_vec2 uv_min;
      ak_vec2 uv_max;
    } tex;
  };
} ak_mtrl_quaddata;

typedef struct
{
  ak_mtrl_enum me;
  union
  {
    struct
    {
    } col;
    struct
    {
      ak_tex tex;
    } tex;
    struct
    {
    } ui;
  };
} ak_mtrl_basedata;

typedef struct
{
  ak_mat3_f vp;
  ak_fx time;
} ak_mtrl_indata;

#endif
