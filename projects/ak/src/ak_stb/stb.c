#include "ak/debug.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "ak/gfx/img.h"

ak_img
ak_img_load(const char* path)
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

void
ak_img_unload(ak_img* img)
{
  stbi_image_free(img->pixels);
}
