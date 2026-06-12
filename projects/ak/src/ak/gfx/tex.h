#ifndef ak_gfx_tex_h
#define ak_gfx_tex_h

#include "ak/core/img.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"

typedef struct ak_tex ak_tex;

typedef enum
{
  ak_textype_rgba8
} ak_textype;

ak_tex*
ak_tex_make_from_img(ak_gfx* gfx,
                     const ak_img* img,
                     ak_textype type,
                     ak_alct alct);

void
ak_tex_destroy(ak_tex* tex);

ak_textype
ak_tex_type(ak_tex* tex);

#endif
