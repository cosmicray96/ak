#ifndef ak_res_reses_img_h
#define ak_res_reses_img_h

#include "ak/export.h"
#include "ak/res/core.h"
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

/*
ak_ex ak_img
ak_img_make_from_path(const char* path);

ak_ex ak_img
ak_img_make_from_stm(ak_stm stm,
                     ak_alct alct);
*/

ak_ex ak_stmerr
ak_stm_read_img(ak_stm stm,
                ak_img* o_img,
                ak_alct alct);
ak_ex void
ak_img_destroy(ak_img* img);

ak_ex ak_res
ak_img_into_res(const ak_img* img);

#endif
