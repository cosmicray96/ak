#ifndef ak_core_math_vec4_h
#define ak_core_math_vec4_h

#include "ak/core/math/fixed.h"

typedef struct
{
  union
  {
    struct
    {
      ak_fx x, y, z, w;
    };
    struct
    {
      ak_fx r, g, b, a;
    };
    ak_fx v[4];
  };
} ak_vec4;

static ak_vec4
ak_vec4_make(ak_fx x,
             ak_fx y,
             ak_fx z,
             ak_fx w)
{
  ak_vec4 v;
  v.x = x;
  v.y = y;
  v.z = z;
  v.w = w;
  return v;
}

#endif
