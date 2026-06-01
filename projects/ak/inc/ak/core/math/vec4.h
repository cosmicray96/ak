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
    ak_fx v[4];
  };
} ak_vec4;

typedef struct
{
  union
  {
    struct
    {
      float x, y, z, w;
    };
    struct
    {
      float r, g, b, a;
    };
    struct
    {
      float _x, _y, width, height;
    };
    float v[4];
  };
} ak_vec4f;

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
