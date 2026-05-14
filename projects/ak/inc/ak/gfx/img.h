#ifndef ak_gfx_img_h
#define ak_gfx_img_h

#include "ak/export.h"
#include "ak/system/resman.h"

#include <stdint.h>

typedef struct
{
  ak_img* img;
  uint32_t x;
  uint32_t y;
  uint32_t w;
  uint32_t h;
} ak_img_v;

ak_ex ak_img
ak_img_load(const char* path);

void
ak_img_unload(ak_img* img);

#endif
