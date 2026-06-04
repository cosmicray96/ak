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
    ak_fx m[3][3];
    ak_fx v[9];
  };
} ak_mat3;

static ak_mat3
ak_mat3_identity()
{
  ak_mat3 m = { 0 };
  m.m[0][0] = ak_fx_i(1);
  m.m[1][1] = ak_fx_i(1);
  m.m[2][2] = ak_fx_i(1);
  return m;
}

static ak_mat3
ak_mat3_mul(const ak_mat3* a,
            const ak_mat3* b)
{
  ak_mat3 m = { 0 };
  for (int col = 0; col < 3; col++)
    for (int row = 0; row < 3; row++)
      for (int k = 0; k < 3; k++)
        m.m[col][row] =
          ak_fxadd(m.m[col][row],
                   ak_fxmul(a->m[k][row],
                            b->m[col][k]));
  return m;
}

static ak_mat3
ak_mat3_transpose(const ak_mat3* a)
{
  ak_mat3 m = { 0 };
  for (int col = 0; col < 3; col++)
    for (int row = 0; row < 3; row++)
      m.m[col][row] = a->m[row][col];
  return m;
}

// transform a point (applies translation)
static ak_vec2
ak_mat3_mul_pos(const ak_mat3* m, ak_vec2 p)
{
  return ak_vec2_make(
    ak_fxadd(
      ak_fxadd(ak_fxmul(m->m[0][0], p.x),
               ak_fxmul(m->m[1][0], p.y)),
      m->m[2][0]),
    ak_fxadd(
      ak_fxadd(ak_fxmul(m->m[0][1], p.x),
               ak_fxmul(m->m[1][1], p.y)),
      m->m[2][1]));
}

// transform a direction (ignores
// translation)
static ak_vec2
ak_mat3_mul_dir(const ak_mat3* m, ak_vec2 d)
{
  return ak_vec2_make(
    ak_fxadd(ak_fxmul(m->m[0][0], d.x),
             ak_fxmul(m->m[1][0], d.y)),
    ak_fxadd(ak_fxmul(m->m[0][1], d.x),
             ak_fxmul(m->m[1][1], d.y)));
}

static ak_vec2
ak_mat3_get_pos(const ak_mat3* m)
{
  return ak_vec2_make(m->m[2][0],
                      m->m[2][1]);
}

static ak_vec2
ak_mat3_get_scale(const ak_mat3* m)
{
  // column lengths

  ak_fx sx = ak_fx_sqrt(ak_fxadd(
    ak_fxmul(m->m[0][0], m->m[0][0]),
    ak_fxmul(m->m[0][1], m->m[0][1])));

  ak_fx sy = ak_fx_sqrt(ak_fxadd(
    ak_fxmul(m->m[1][0], m->m[1][0]),
    ak_fxmul(m->m[1][1], m->m[1][1])));

  return ak_vec2_make(sx, sy);
}

static ak_mat3
ak_mat3_inv_fast(const ak_mat3* m)
{
  // transpose the rotation block (top-left
  // 2x2)
  ak_mat3 r = { 0 };
  r.m[0][0] = m->m[0][0];
  r.m[0][1] = m->m[1][0];
  r.m[1][0] = m->m[0][1];
  r.m[1][1] = m->m[1][1];
  r.m[2][2] = ak_fx_i(1);

  // negate the translation transformed by
  // transposed rotation
  ak_vec2 t =
    ak_vec2_make(m->m[2][0], m->m[2][1]);
  ak_vec2 it = ak_mat3_mul_dir(&r, t);
  r.m[2][0] = -it.x;
  r.m[2][1] = -it.y;

  return r;
}

static ak_angle
ak_mat3_get_rot(const ak_mat3* m)
{
  // rotation is encoded in first column:
  //
  // [ sx*cos ]
  // [ sx*sin ]
  //
  // atan2(y, x)

  return ak_atan2(m->m[0][1], m->m[0][0]);
}

// build matrix from TRS
// [ sx*cos  -sy*sin  tx ]
// [ sx*sin   sy*cos  ty ]
// [       0        0  1 ]

typedef struct
{
  union
  {
    float m[3][3];
    float v[9];
  };
} ak_mat3_f;

static void
ak_mat3_to_f(const ak_mat3* m, ak_mat3_f* mf)
{
  for (int i = 0; i < 9; i++)
    mf->v[i] = ak_fx_to_f(m->v[i]);
}

/*
static ak_mat3_f
ak_mat3_from_rect(float x,
                  float y,
                  float w,
                  float h)
{
  ak_mat3_f m = { 0 };
  m.m[0][0] = w;
  m.m[0][1] = 0.0f;
  m.m[0][2] = 0.0f;
  m.m[1][0] = 0.0f;
  m.m[1][1] = h;
  m.m[1][2] = 0.0f;
  m.m[2][0] = x;
  m.m[2][1] = y;
  m.m[2][2] = 1.0f;
  return m;
}
*/

static ak_mat3_f
ak_mat3_from_rect(float x,
                  float y,
                  float w,
                  float h)
{
  ak_mat3_f m = { 0 };

  m.m[0][0] = w;
  m.m[1][1] = h;

  m.m[2][0] = x + w * 0.5f;
  m.m[2][1] = y + h * 0.5f;
  m.m[2][2] = 1.0f;

  return m;
}
#endif
