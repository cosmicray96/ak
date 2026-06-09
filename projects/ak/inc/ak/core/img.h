#ifndef ak_core_img_h
#define ak_core_img_h

#include "ak/export.h"
#include "ak/system/stream.h"
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

ak_ex ak_img
ak_img_make_from_path(const char* path);

ak_ex ak_img
ak_img_make_from_stm(ak_stm stm);

void
ak_img_destroy(ak_img* img);

#endif
