#include "ak/core/img.h"
#include "ak/debug.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

ak_img
ak_img_make_from_path(const char* path)
{
  ak_img img = { 0 };

  int width;
  int height;
  int channels;

  img.type = ak_img_rgba8;
  img.pixels = stbi_load(
    path, &width, &height, &channels, 4);
  img.w = width;
  img.h = height;

  ak_assert(img.pixels);

  return img;
}

ak_img
ak_img_make_from_stm(ak_stm stm,
                     ak_alct alct)
{

  void* data;
  uint64_t size;
  ak_stm_read_all(stm, &data, &size, alct);

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

  ak_assert(img.pixels);

  ak_alct_free(alct, data);
  return img;
}

void
ak_img_destroy(ak_img* img)
{
  stbi_image_free(img->pixels);
}
