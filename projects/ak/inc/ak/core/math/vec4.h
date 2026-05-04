#ifndef ak_core_math_vec3_h
#define ak_core_math_vec3_h

#include "ak/core/math/fixed.h"

typedef struct
{
  union
  {
    struct
    {
      ak_fx32 x, y, z, w;
    };
    struct
    {
      ak_fx32 r, g, b, a;
    };
    ak_fx32 v[4];
  };
} ak_vec4;

static ak_vec4
ak_vec4_make(ak_fx32 x,
             ak_fx32 y,
             ak_fx32 z,
             ak_fx32 w)
{
  ak_vec4 v;
  v.x = x;
  v.y = y;
  v.z = z;
  v.w = w;
  return v;
}

#endif
