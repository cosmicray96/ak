#ifndef ak_gfx_tex_h
#define ak_gfx_tex_h

#include "ak/core/async/dispatcher.h"
#include "ak/core/errcode.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/res/reses/img.h"

typedef enum
{
  ak_textype_rgba8
} ak_textype;

ak_errcode
ak_tex_from_img(ak_gfx* gfx,
                const ak_img* img,
                ak_textype type,
                ak_tex** o_tex,
                ak_alct alct);

void
ak_tex_destroy(ak_tex* tex);

ak_errcode
ak_tex_from_img_dispatch(ak_dispatcher* d,
                         ak_gfx* gfx,
                         const ak_img* img,
                         ak_textype type,
                         ak_tex** o_tex,
                         ak_alct alct);

void
ak_tex_destroy_dispatch(ak_dispatcher* d,
                        ak_tex* tex);

ak_textype
ak_tex_type(ak_tex* tex);

#endif
