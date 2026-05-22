#include "ak/core/math/tf2d.h"

ak_tf2
ak_tf2d_identity()
{
  return ak_tf2d_make(
    ak_vec2_make(ak_fx_i(0), ak_fx_i(0)),
    ak_angle_rad(0),
    ak_vec2_make(ak_fx_i(1), ak_fx_i(1)));
}

ak_mat3
ak_tf2d_to_mat3x3(const ak_tf2* tf)
{
  ak_vec2 pos = tf->pos;
  ak_angle rot = tf->rot;
  ak_vec2 scale = tf->scale;

  ak_fx c = ak_cos(rot);
  ak_fx s = ak_sin(rot);

  ak_mat3 m = { 0 };
  m.m[0][0] = ak_fxmul(scale.x, c);
  m.m[0][1] = ak_fxmul(scale.x, s);
  m.m[1][0] = ak_fxmul(scale.y, -s);
  m.m[1][1] = ak_fxmul(scale.y, c);
  m.m[2][0] = pos.x;
  m.m[2][1] = pos.y;
  m.m[2][2] = ak_fx_i(1);
  return m;
}

ak_tf2
ak_mat3x3_to_tf2d(const ak_mat3* m)
{
  ak_tf2 tf;

  tf.pos =
    ak_vec2_make(m->m[2][0], m->m[2][1]);

  // scale is the length of each rotation
  // column
  ak_vec2 col0 =
    ak_vec2_make(m->m[0][0], m->m[0][1]);
  ak_vec2 col1 =
    ak_vec2_make(m->m[1][0], m->m[1][1]);
  tf.scale = ak_vec2_make(ak_vec2_len(col0),
                          ak_vec2_len(col1));

  // atan2(sx*sin, sx*cos) — sx cancels,
  // leaving the angle NOTE: assumes positive
  // scale. negative scale (flipped sprites)
  // will produce wrong rotation. divide by
  // scale first if needed.
  tf.rot = ak_atan2(m->m[0][1], m->m[0][0]);

  return tf;
}

/*
void
ak_tf2d_to_4corner(ak_tf2 tf, ak_vec2 v[4])
{
  ak_fx half_w =
    ak_fxdiv(tf.scale.x, ak_fx_f(2));
  ak_fx half_h =
    ak_fxdiv(tf.scale.y, ak_fx_f(2));

  // tl
  v[0] =
    ak_vec2_make(ak_fxsub(tf.pos.x, half_w),
                 ak_fxadd(tf.pos.y, half_h));

  // tr
  v[1] =
    ak_vec2_make(ak_fxadd(tf.pos.x, half_w),
                 ak_fxadd(tf.pos.y, half_h));

  // br
  v[2] =
    ak_vec2_make(ak_fxadd(tf.pos.x, half_w),
                 ak_fxsub(tf.pos.y, half_h));

  // bl
  v[3] =
    ak_vec2_make(ak_fxsub(tf.pos.x, half_w),
                 ak_fxsub(tf.pos.y, half_h));
}
*/
