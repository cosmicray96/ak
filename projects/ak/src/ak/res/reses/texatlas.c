#include "ak/res/reses/texatlas.h"
#include "ak/core/math/stm.h"
#include "ak/core/math/vec4f.h"
#include "ak/system/stream.h"

ak_texatlas
ak_texatlas_make(const ak_tex* tex,
                 const ak_da* uv_rects,
                 ak_alct alct)
{
  ak_texatlas ta = { 0 };
  ta.tex = tex;
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
ak_stm_read_texatlas(ak_stm stm,
                     const ak_tex* tex,
                     ak_texatlas* o_ta,
                     ak_alct alct)
{
  ak_stmerr ak_stmerr_var = ak_stmerr_err;
  uint32_t count = 0;
  ak_stm_try1(ak_stm_read_u32(stm, &count));

  ak_texatlas ta = { 0 };
  ta.tex = tex;
  ta.uv_rects =
    ak_da_make(sizeof(ak_vec4f), alct);

  for (uint32_t i = 0; i < count; i++) {
    ak_vec4f uv_rect = { 0 };
    ak_stm_try2(
      ak_stm_read_vec4f(stm, &uv_rect));
    ak_da_pushback(&ta.uv_rects, &uv_rect);
  }

  return ak_stmerr_ok;
ak_stm_crash:
  ak_texatlas_destroy(&ta);
  return ak_stmerr_var;
}

void
ak_texatlas_destroy(ak_texatlas* ta)
{
  ak_da_destroy(&ta->uv_rects);
  ta->tex = 0;
}

ak_vec4f
ak_texatlas_at(const ak_texatlas* ta,
               uint32_t idx)
{
  return *(const ak_vec4f*)ak_da_at_const(
    &ta->uv_rects, idx);
}

const ak_tex*
ak_texatlas_tex(const ak_texatlas* ta)
{
  return ta->tex;
}
