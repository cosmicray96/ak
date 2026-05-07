#include "ak/game/sys/ren.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/vec2.h"
#include "ak/core/math/vec4.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/comp_t.h"
#include "ak/game/stg/core.h"
#include "ak/game/world/view.h"
#include "ak/gfx/gfx.h"
#include "ak/gfx/mtrl/col_itn.h"
#include "ak/gfx/mtrl/mtrl.h"

ak_sys_ren
ak_sys_ren_make(ak_gfx* gf,
                ak_world_v* wv,
                ak_alct alct)
{
  ak_sys_ren r = { 0 };
  r.alct = alct;
  r.wv = wv;
  r.gf = gf;
  r.mtrl = ak_mtrl_col_make(gf, alct);
  return r;
}

void
ak_sys_ren_destroy(ak_sys_ren* r)
{
  ak_alct_invalidate(&r->alct);
  r->gf = 0;
  r->wv = 0;
}

void
ak_sys_ren_render(ak_sys_ren* r)
{
  ak_gfx_frame_begin(r->gf);

  ak_mtrl_col_col_set(
    r->mtrl,
    ak_vec4_make(ak_fx32_f(1.0f),
                 ak_fx32_f(0.0f),
                 ak_fx32_f(0.0f),
                 ak_fx32_f(1.0f)));

  ak_mtrl_col_call_begin(r->mtrl);

  ak_world_v_itdfs it =
    ak_world_v_itdfs_make(
      r->wv, ak_world_v_ett_root(r->wv));
  ak_ett e = 0;
  while (ak_world_v_itdfs_next(&it, &e)) {
    ak_rect_t* rect =
      ak_world_v_comp_rect(r->wv, e);
    if (!rect) {
      continue;
    }

    ak_tf2d_t* tf2d =
      ak_world_v_comp_tf2d(r->wv, e);
    if (!tf2d) {
      continue;
    }
    ak_fx32 half_w = ak_fx32_div(
      tf2d->tf.scale.x, ak_fx32_f(2));
    ak_fx32 half_h = ak_fx32_div(
      tf2d->tf.scale.y, ak_fx32_f(2));

    ak_vec2 tl = ak_vec2_make(
      ak_fx32_sub(tf2d->tf.pos.x, half_w),
      ak_fx32_sub(tf2d->tf.pos.y, half_h));

    ak_vec2 tr = ak_vec2_make(
      ak_fx32_add(tf2d->tf.pos.x, half_w),
      ak_fx32_sub(tf2d->tf.pos.y, half_h));

    ak_vec2 bl = ak_vec2_make(
      ak_fx32_sub(tf2d->tf.pos.x, half_w),
      ak_fx32_add(tf2d->tf.pos.y, half_h));

    ak_vec2 br = ak_vec2_make(
      ak_fx32_add(tf2d->tf.pos.x, half_w),
      ak_fx32_add(tf2d->tf.pos.y, half_h));

    /* triangle 1 */
    ak_mtrl_col_pushvert(r->mtrl, tl);
    ak_mtrl_col_pushvert(r->mtrl, bl);
    ak_mtrl_col_pushvert(r->mtrl, tr);

    /* triangle 2 */
    ak_mtrl_col_pushvert(r->mtrl, tr);
    ak_mtrl_col_pushvert(r->mtrl, bl);
    ak_mtrl_col_pushvert(r->mtrl, br);
  }

  ak_mtrl_col_call_end(r->mtrl);

  ak_gfx_frame_end(r->gf);
}
