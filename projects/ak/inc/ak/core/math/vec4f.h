
#ifndef ak_core_math_vec4f_h
#define ak_core_math_vec4f_h

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
      float rectx, recty, rectw, recth;
    };
    float v[4];
  };
} ak_vec4f;

static ak_vec4f
ak_vec4f_make(float x,
              float y,
              float z,
              float w)
{
  return (ak_vec4f){
    .x = x, .y = y, .z = z, .w = w
  };
}

#endif
