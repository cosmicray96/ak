#ifndef ak_core_math_vec3_h
#define ak_core_math_vec3_h

#include "ak/core/math/fixed.h"
#include "ak/debug.h"

typedef struct
{
  union
  {
    struct
    {
      ak_fx x, y, z;
    };
    ak_fx v[3];
  };
} ak_vec3;

static ak_vec3
ak_vec3_make(ak_fx x, ak_fx y, ak_fx z)
{
  ak_vec3 v;
  v.x = x;
  v.y = y;
  v.z = z;
  return v;
}

static ak_vec3
ak_vec3_add(ak_vec3 a, ak_vec3 b)
{
  return ak_vec3_make(ak_fxadd(a.x, b.x),
                      ak_fxadd(a.y, b.y),
                      ak_fxadd(a.z, b.z));
}

static ak_vec3
ak_vec3_sub(ak_vec3 a, ak_vec3 b)
{
  return ak_vec3_make(ak_fxsub(a.x, b.x),
                      ak_fxsub(a.y, b.y),
                      ak_fxsub(a.z, b.z));
}

static ak_vec3
ak_vec3_scale(ak_vec3 a, ak_fx s)
{
  return ak_vec3_make(ak_fxmul(a.x, s),
                      ak_fxmul(a.y, s),
                      ak_fxmul(a.z, s));
}

static ak_fx
ak_vec3_dot(ak_vec3 a, ak_vec3 b)
{
  return ak_fxadd(
    ak_fxadd(ak_fxmul(a.x, b.x),
             ak_fxmul(a.y, b.y)),
    ak_fxmul(a.z, b.z));
}

static ak_vec3
ak_vec3_cross(ak_vec3 a, ak_vec3 b)
{
  return ak_vec3_make(
    ak_fxsub(ak_fxmul(a.y, b.z),
             ak_fxmul(a.z, b.y)),
    ak_fxsub(ak_fxmul(a.z, b.x),
             ak_fxmul(a.x, b.z)),
    ak_fxsub(ak_fxmul(a.x, b.y),
             ak_fxmul(a.y, b.x)));
}

static ak_fx
ak_vec3_len(ak_vec3 a)
{
  return ak_fx_sqrt(ak_vec3_dot(a, a));
}

static ak_vec3
ak_vec3_norm(ak_vec3 a)
{
  ak_fx len = ak_vec3_len(a);
  ak_assert(len != 0);
  return ak_vec3_make(ak_fxdiv(a.x, len),
                      ak_fxdiv(a.y, len),
                      ak_fxdiv(a.z, len));
}

#endif
