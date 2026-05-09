#include "ak/core/math/tf2d.h"

ak_tf2d
ak_mat3x3_to_tf2d(ak_mat3x3 m)
{
  ak_tf2d tf;

  tf.pos =
    ak_vec2_make(m.m[2][0], m.m[2][1]);

  // scale is the length of each rotation
  // column
  ak_vec2 col0 =
    ak_vec2_make(m.m[0][0], m.m[0][1]);
  ak_vec2 col1 =
    ak_vec2_make(m.m[1][0], m.m[1][1]);
  tf.scale = ak_vec2_make(ak_vec2_len(col0),
                          ak_vec2_len(col1));

  // atan2(sx*sin, sx*cos) — sx cancels,
  // leaving the angle NOTE: assumes positive
  // scale. negative scale (flipped sprites)
  // will produce wrong rotation. divide by
  // scale first if needed.
  tf.rot = ak_atan2(m.m[0][1], m.m[0][0]);

  return tf;
}

void
ak_tf2d_to_4corner(ak_tf2d tf, ak_vec2 v[4])
{
  ak_fx32 half_w =
    ak_fx32_div(tf.scale.x, ak_fx32_f(2));
  ak_fx32 half_h =
    ak_fx32_div(tf.scale.y, ak_fx32_f(2));

  // tl
  v[0] = ak_vec2_make(
    ak_fx32_sub(tf.pos.x, half_w),
    ak_fx32_add(tf.pos.y, half_h));

  // tr
  v[1] = ak_vec2_make(
    ak_fx32_add(tf.pos.x, half_w),
    ak_fx32_add(tf.pos.y, half_h));

  // br
  v[2] = ak_vec2_make(
    ak_fx32_add(tf.pos.x, half_w),
    ak_fx32_sub(tf.pos.y, half_h));

  // bl
  v[3] = ak_vec2_make(
    ak_fx32_sub(tf.pos.x, half_w),
    ak_fx32_sub(tf.pos.y, half_h));
}
