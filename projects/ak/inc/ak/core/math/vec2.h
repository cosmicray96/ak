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
      ak_fx x, y;
    };
    ak_fx v[2];
  };
} ak_vec2;

static ak_vec2
ak_vec2_make(ak_fx x, ak_fx y)
{
  ak_vec2 v;
  v.x = x;
  v.y = y;
  return v;
}

static ak_vec2
ak_vec2_add(ak_vec2 a, ak_vec2 b)
{
  return ak_vec2_make(ak_fxadd(a.x, b.x),
                      ak_fxadd(a.y, b.y));
}

static ak_vec2
ak_vec2_sub(ak_vec2 a, ak_vec2 b)
{
  return ak_vec2_make(ak_fxsub(a.x, b.x),
                      ak_fxsub(a.y, b.y));
}

static ak_vec2
ak_vec2_scale(ak_vec2 a, ak_fx s)
{
  return ak_vec2_make(ak_fxmul(a.x, s),
                      ak_fxmul(a.y, s));
}

static ak_fx
ak_vec2_dot(ak_vec2 a, ak_vec2 b)
{
  return ak_fxadd(ak_fxmul(a.x, b.x),
                  ak_fxmul(a.y, b.y));
}

static ak_fx
ak_vec2_len(ak_vec2 a)
{
  return ak_fx_sqrt(ak_vec2_dot(a, a));
}

static ak_vec2
ak_vec2_norm(ak_vec2 a)
{
  ak_fx len = ak_vec2_len(a);
  ak_assert(len != 0);
  return ak_vec2_make(ak_fxdiv(a.x, len),
                      ak_fxdiv(a.y, len));
}

#endif
