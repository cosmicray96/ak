#ifndef ak_core_math_tf2d_h
#define ak_core_math_tf2d_h

#include "ak/core/math/mat3x3.h"
#include "ak/core/math/vec2.h"
#include "ak/export.h"

typedef struct
{
  ak_vec2 pos;
  ak_angle rot;
  ak_vec2 scale;
} ak_tf2d;

static ak_tf2d
ak_tf2d_make(ak_vec2 pos,
             ak_angle rot,
             ak_vec2 scale)
{
  ak_tf2d r;
  r.pos = pos;
  r.rot = rot;
  r.scale = scale;
  return r;
}

ak_ex ak_tf2d
ak_tf2d_identity();

ak_ex ak_mat3x3
ak_tf2d_to_mat3x3(const ak_tf2d* tf);

ak_ex ak_tf2d
ak_mat3x3_to_tf2d(ak_mat3x3 m);

#endif
