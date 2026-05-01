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
      ak_fx32 x, y, z;
    };
    ak_fx32 v[3];
  };
} ak_vec3;

static ak_vec3
ak_vec3_make(ak_fx32 x, ak_fx32 y, ak_fx32 z)
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
  return ak_vec3_make(ak_fx32_add(a.x, b.x),
                      ak_fx32_add(a.y, b.y),
                      ak_fx32_add(a.z, b.z));
}

static ak_vec3
ak_vec3_sub(ak_vec3 a, ak_vec3 b)
{
  return ak_vec3_make(ak_fx32_sub(a.x, b.x),
                      ak_fx32_sub(a.y, b.y),
                      ak_fx32_sub(a.z, b.z));
}

static ak_vec3
ak_vec3_scale(ak_vec3 a, ak_fx32 s)
{
  return ak_vec3_make(ak_fx32_mul(a.x, s),
                      ak_fx32_mul(a.y, s),
                      ak_fx32_mul(a.z, s));
}

static ak_fx32
ak_vec3_dot(ak_vec3 a, ak_vec3 b)
{
  return ak_fx32_add(
    ak_fx32_add(ak_fx32_mul(a.x, b.x),
                ak_fx32_mul(a.y, b.y)),
    ak_fx32_mul(a.z, b.z));
}

static ak_vec3
ak_vec3_cross(ak_vec3 a, ak_vec3 b)
{
  return ak_vec3_make(
    ak_fx32_sub(ak_fx32_mul(a.y, b.z),
                ak_fx32_mul(a.z, b.y)),
    ak_fx32_sub(ak_fx32_mul(a.z, b.x),
                ak_fx32_mul(a.x, b.z)),
    ak_fx32_sub(ak_fx32_mul(a.x, b.y),
                ak_fx32_mul(a.y, b.x)));
}

static ak_fx32
ak_vec3_len(ak_vec3 a)
{
  return ak_fx32_sqrt(ak_vec3_dot(a, a));
}

static ak_vec3
ak_vec3_norm(ak_vec3 a)
{
  ak_fx32 len = ak_vec3_len(a);
  ak_assert(len != 0);
  return ak_vec3_make(ak_fx32_div(a.x, len),
                      ak_fx32_div(a.y, len),
                      ak_fx32_div(a.z, len));
}

#endif
