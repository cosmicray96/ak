#include "ak/res/reses/img.h"

#include <stb_image.h>

ak_errcode
ak_stm_read_img(ak_stm stm,
                ak_img* o_img,
                ak_alct alct)
{
  void* data;
  uint64_t size;
  ak_stmresult err =
    ak_stm_read_all(stm, &data, &size, alct);
  if (err.err != ak_ok) {
    return err.err;
  }

  ak_img img = { 0 };
  int width;
  int height;
  int channels;

  img.type = ak_img_rgba8;
  img.pixels =
    stbi_load_from_memory(data,
                          size,
                          &width,
                          &height,
                          &channels,
                          4);
  img.w = width;
  img.h = height;
  ak_alct_free(alct, data);

  if (!img.pixels) {
    return ak_err_invalid_args;
  }

  *o_img = img;
  return ak_ok;
}

void
ak_img_destroy(ak_img* img)
{
  stbi_image_free(img->pixels);
}
