#ifndef ak_core_math_tf2d_h
#define ak_core_math_tf2d_h

#include "ak/core/math/mat3x3.h"

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

static ak_tf2d
ak_tf2d_identity()
{
  return ak_tf2d_make(
    ak_vec2_make(ak_fx32_i(0), ak_fx32_i(0)),
    0,
    ak_vec2_make(ak_fx32_i(1),
                 ak_fx32_i(1)));
}

static ak_mat3x3
ak_tf2d_to_mat3x3(ak_tf2d tf)
{
  return ak_mat3x3_from_transform(
    tf.pos, tf.rot, tf.scale);
}

#endif
