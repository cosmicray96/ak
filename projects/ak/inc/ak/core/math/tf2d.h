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
  ak_vec2 pos = tf.pos;
  ak_angle rot = tf.rot;
  ak_vec2 scale = tf.scale;

  ak_fx32 c = ak_cos(rot);
  ak_fx32 s = ak_sin(rot);

  ak_mat3x3 m = { 0 };
  m.m[0][0] = ak_fx32_mul(scale.x, c);
  m.m[0][1] = ak_fx32_mul(scale.x, s);
  m.m[1][0] = ak_fx32_mul(scale.y, -s);
  m.m[1][1] = ak_fx32_mul(scale.y, c);
  m.m[2][0] = pos.x;
  m.m[2][1] = pos.y;
  m.m[2][2] = ak_fx32_i(1);
  return m;
}

ak_ex ak_tf2d
ak_mat3x3_to_tf2d(ak_mat3x3 m);

// tl, tr, br, bl
ak_ex void
ak_tf2d_to_4corner(ak_tf2d tf, ak_vec2 v[4]);

#endif
