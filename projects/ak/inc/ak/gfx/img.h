#ifndef ak_gfx_img_h
#define ak_gfx_img_h

#include <stdint.h>

typedef enum
{
  ak_img_rgba8
} ak_imgtype;

typedef struct
{
  ak_imgtype type;
  void* pixels;
  uint32_t w;
  uint32_t h;
} ak_img;

typedef struct
{
  ak_img* img;
  uint32_t x;
  uint32_t y;
  uint32_t w;
  uint32_t h;
} ak_img_v;

#endif
