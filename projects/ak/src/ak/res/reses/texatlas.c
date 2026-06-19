#include "ak/res/reses/texatlas.h"
#include "ak/coll/stm.h"
#include "ak/core/math/stm.h"
#include "ak/core/math/vec4f.h"
#include "ak/res/core.h"
#include "ak/system/stream.h"

ak_texatlas
ak_texatlas_make(const ak_da* uv_rects,
                 ak_alct alct)
{
  ak_texatlas ta = { 0 };
  ta.uv_rects =
    ak_da_make(sizeof(ak_vec4f), alct);
  uint32_t count = ak_da_count(uv_rects);
  for (uint32_t i = 0; i < count; i++) {
    const ak_vec4f* uv_rect =
      ak_da_at_const(uv_rects, i);
    ak_da_pushback(&ta.uv_rects, uv_rect);
  }
  return ta;
}

ak_stmerr
ak_stm_write_texatlas(ak_stm stm,
                      const ak_texatlas* ta)
{
  return ak_stm_write_da(stm, &ta->uv_rects);
}
ak_stmerr
ak_stm_read_texatlas(ak_stm stm,
                     ak_texatlas* o_ta,
                     ak_alct alct)
{
  ak_texatlas ta = { 0 };
  ak_stm_try(
    ak_stm_read_da(stm, &ta.uv_rects, alct));

  *o_ta = ta;
  return ak_stmerr_ok;
}

void
ak_texatlas_destroy(ak_texatlas* ta)
{
  ak_da_destroy(&ta->uv_rects);
}

ak_vec4f
ak_texatlas_at(const ak_texatlas* ta,
               uint32_t idx)
{
  return *(const ak_vec4f*)ak_da_at_const(
    &ta->uv_rects, idx);
}
