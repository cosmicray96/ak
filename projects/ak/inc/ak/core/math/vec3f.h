#ifndef ak_core_math_vec3f_h
#define ak_core_math_vec3f_h

typedef struct
{
  union
  {
    struct
    {
      float x, y, z;
    };
    float v[2];
  };
} ak_vec3f;

static ak_vec3f
ak_vec3f_make(float x, float y, float z)
{
  ak_vec3f v;
  v.x = x;
  v.y = y;
  v.z = z;
  return v;
}

static ak_vec3f
ak_vec3f_add(ak_vec3f a, ak_vec3f b)
{
  return ak_vec3f_make(
    a.x + b.x, a.y + b.y, a.z + b.z);
}

static ak_vec3f
ak_vec3f_sub(ak_vec3f a, ak_vec3f b)
{
  return ak_vec3f_make(
    a.x - b.x, a.y - b.y, a.z - b.z);
}

static ak_vec3f
ak_vec3f_scale(ak_vec3f a, float s)
{
  return ak_vec3f_make(
    a.x * s, a.y * s, a.z * s);
}

#endif
