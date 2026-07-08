#ifndef ak_res_reses_texatlas_h
#define ak_res_reses_texatlas_h

#include "ak/core/math/vec4f.h"
#include "ak/core/mem/allocator.h"
#include "ak/export.h"
#include "ak/gfx/core.h"
#include "ak/system/stream.h"

typedef struct
{
  ak_da uv_rects;
} ak_texatlas;

ak_ex ak_texatlas
ak_texatlas_make(const ak_da* uv_rect,
                 ak_alct alct);
ak_ex ak_errcode
ak_stm_write_texatlas(ak_stm stm,
                      const ak_texatlas* ta);
ak_ex ak_errcode
ak_stm_read_texatlas(ak_stm stm,
                     ak_texatlas* o_ta,
                     ak_alct alct);
ak_ex void
ak_texatlas_destroy(ak_texatlas* ta);

ak_ex ak_vec4f
ak_texatlas_at(const ak_texatlas* ta,
               uint32_t idx);

#endif
