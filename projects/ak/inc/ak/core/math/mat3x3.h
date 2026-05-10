#ifndef ak_core_math_mat3x3_h
#define ak_core_math_mat3x3_h

#include "ak/core/math/fixed.h"
#include "ak/core/math/trig.h"
#include "ak/core/math/vec2.h"

// column major to match opengl convention
// m[col][row]
typedef struct
{
  union
  {
    ak_fx32 m[3][3];
    ak_fx32 v[9];
  };
} ak_mat3x3;

static ak_mat3x3
ak_mat3x3_identity()
{
  ak_mat3x3 m = { 0 };
  m.m[0][0] = ak_fx32_i(1);
  m.m[1][1] = ak_fx32_i(1);
  m.m[2][2] = ak_fx32_i(1);
  return m;
}

static ak_mat3x3
ak_mat3x3_mul(ak_mat3x3 a, ak_mat3x3 b)
{
  ak_mat3x3 m = { 0 };
  for (int col = 0; col < 3; col++)
    for (int row = 0; row < 3; row++)
      for (int k = 0; k < 3; k++)
        m.m[col][row] = ak_fx32_add(
          m.m[col][row],
          ak_fx32_mul(a.m[k][row],
                      b.m[col][k]));
  return m;
}

static ak_mat3x3
ak_mat3x3_transpose(const ak_mat3x3* a)
{
  ak_mat3x3 m = { 0 };
  for (int col = 0; col < 3; col++)
    for (int row = 0; row < 3; row++)
      m.m[col][row] = a->m[row][col];
  return m;
}

// transform a point (applies translation)
static ak_vec2
ak_mat3x3_mul_pos(ak_mat3x3 m, ak_vec2 p)
{
  return ak_vec2_make(
    ak_fx32_add(
      ak_fx32_add(
        ak_fx32_mul(m.m[0][0], p.x),
        ak_fx32_mul(m.m[1][0], p.y)),
      m.m[2][0]),
    ak_fx32_add(
      ak_fx32_add(
        ak_fx32_mul(m.m[0][1], p.x),
        ak_fx32_mul(m.m[1][1], p.y)),
      m.m[2][1]));
}

// transform a direction (ignores
// translation)
static ak_vec2
ak_mat3x3_mul_dir(ak_mat3x3 m, ak_vec2 d)
{
  return ak_vec2_make(
    ak_fx32_add(ak_fx32_mul(m.m[0][0], d.x),
                ak_fx32_mul(m.m[1][0], d.y)),
    ak_fx32_add(
      ak_fx32_mul(m.m[0][1], d.x),
      ak_fx32_mul(m.m[1][1], d.y)));
}

static void
ak_mat3x3_to_f(const ak_mat3x3* m,
               float out[9])
{
  for (int i = 0; i < 9; i++)
    out[i] = ak_fx32_to_f(m->v[i]);
}
static ak_vec2
ak_mat3x3_get_pos(ak_mat3x3 m)
{
  return ak_vec2_make(m.m[2][0], m.m[2][1]);
}

static ak_vec2
ak_mat3x3_get_scale(ak_mat3x3 m)
{
  // column lengths

  ak_fx32 sx = ak_fx32_sqrt(ak_fx32_add(
    ak_fx32_mul(m.m[0][0], m.m[0][0]),
    ak_fx32_mul(m.m[0][1], m.m[0][1])));

  ak_fx32 sy = ak_fx32_sqrt(ak_fx32_add(
    ak_fx32_mul(m.m[1][0], m.m[1][0]),
    ak_fx32_mul(m.m[1][1], m.m[1][1])));

  return ak_vec2_make(sx, sy);
}

static ak_mat3x3
ak_mat3x3_inv_fast(const ak_mat3x3* m)
{
  // transpose the rotation block (top-left
  // 2x2)
  ak_mat3x3 r = { 0 };
  r.m[0][0] = m->m[0][0];
  r.m[0][1] = m->m[1][0];
  r.m[1][0] = m->m[0][1];
  r.m[1][1] = m->m[1][1];
  r.m[2][2] = ak_fx32_i(1);

  // negate the translation transformed by
  // transposed rotation
  ak_vec2 t =
    ak_vec2_make(m->m[2][0], m->m[2][1]);
  ak_vec2 it = ak_mat3x3_mul_dir(r, t);
  r.m[2][0] = -it.x;
  r.m[2][1] = -it.y;

  return r;
}

/*
static ak_angle
ak_mat3x3_get_rot(ak_mat3x3 m)
{
  // rotation is encoded in first column:
  //
  // [ sx*cos ]
  // [ sx*sin ]
  //
  // atan2(y, x)

  return ak_atan2(m.m[0][1], m.m[0][0]);
}
*/

// build matrix from TRS
// [ sx*cos  -sy*sin  tx ]
// [ sx*sin   sy*cos  ty ]
// [       0        0  1 ]
/*
static ak_mat3x3
ak_mat3x3_from_transform(ak_vec2 pos,
                         ak_angle rot,
                         ak_vec2 scale)
{
  ak_fx32 c = ak_cos(rot);
  ak_fx32 s = ak_sin(rot);

  ak_mat3x3 r = { 0 };
  r.m[0][0] = ak_fx32_mul(scale.x, c);
  r.m[0][1] = ak_fx32_mul(scale.x, s);
  r.m[1][0] = ak_fx32_mul(scale.y, -s);
  r.m[1][1] = ak_fx32_mul(scale.y, c);
  r.m[2][0] = pos.x;
  r.m[2][1] = pos.y;
  r.m[2][2] = ak_fx32_i(1);
  return r;
}*/

#endif
