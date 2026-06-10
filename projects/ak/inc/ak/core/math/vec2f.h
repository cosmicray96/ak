#ifndef ak_core_math_vec2f_h
#define ak_core_math_vec2f_h

#include "ak/debug.h"
#include <math.h>

typedef struct
{
  union
  {
    struct
    {
      float x, y;
    };
    float v[2];
  };
} ak_vec2f;

static ak_vec2f
ak_vec2f_make(float x, float y)
{
  ak_vec2f v;
  v.x = x;
  v.y = y;
  return v;
}

static ak_vec2f
ak_vec2f_add(ak_vec2f a, ak_vec2f b)
{
  return ak_vec2f_make(a.x + b.x, a.y + b.y);
}

static ak_vec2f
ak_vec2f_sub(ak_vec2f a, ak_vec2f b)
{
  return ak_vec2f_make(a.x - b.x, a.y - b.y);
}

static ak_vec2f
ak_vec2f_scale(ak_vec2f a, float s)
{
  return ak_vec2f_make(a.x * s, a.y * s);
}

static float
ak_vec2f_dot(ak_vec2f a, ak_vec2f b)
{
  return (a.x * b.x) + (a.y * b.y);
}

static float
ak_vec2f_len(ak_vec2f a)
{
  return sqrtf(ak_vec2f_dot(a, a));
}

static ak_vec2f
ak_vec2f_norm(ak_vec2f a)
{
  float len = ak_vec2f_len(a);
  ak_assert(len != 0);
  return ak_vec2f_make(a.x / len, a.y / len);
}

#endif
