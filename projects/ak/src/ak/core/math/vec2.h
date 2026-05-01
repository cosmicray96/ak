#ifndef ak_core_math_vec2_h
#define ak_core_math_vec2_h

#include "ak/core/math/fixed.h"
#include "ak/debug.h"

typedef struct
{
  union
  {
    struct
    {
      ak_fx32 x, y;
    };
    ak_fx32 v[2];
  };
} ak_vec2;

static ak_vec2
ak_vec2_make(ak_fx32 x, ak_fx32 y)
{
  ak_vec2 r;
  r.x = x;
  r.y = y;
  return r;
}

static ak_vec2
ak_vec2_add(ak_vec2 a, ak_vec2 b)
{
  return ak_vec2_make(ak_fx32_add(a.x, b.x),
                      ak_fx32_add(a.y, b.y));
}

static ak_vec2
ak_vec2_sub(ak_vec2 a, ak_vec2 b)
{
  return ak_vec2_make(ak_fx32_sub(a.x, b.x),
                      ak_fx32_sub(a.y, b.y));
}

static ak_vec2
ak_vec2_scale(ak_vec2 a, ak_fx32 s)
{
  return ak_vec2_make(ak_fx32_mul(a.x, s),
                      ak_fx32_mul(a.y, s));
}

static ak_fx32
ak_vec2_dot(ak_vec2 a, ak_vec2 b)
{
  return ak_fx32_add(ak_fx32_mul(a.x, b.x),
                     ak_fx32_mul(a.y, b.y));
}

static ak_fx32
ak_vec2_len(ak_vec2 a)
{
  return ak_fx32_sqrt(ak_vec2_dot(a, a));
}

static ak_vec2
ak_vec2_norm(ak_vec2 a)
{
  ak_fx32 len = ak_vec2_len(a);
  ak_assert(len != 0);
  return ak_vec2_make(ak_fx32_div(a.x, len),
                      ak_fx32_div(a.y, len));
}

#endif
