#include "ak/game/sys/ren.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/comp.h"
#include "ak/game/comp_t.h"
#include "ak/game/stg/core.h"
#include "ak/game/world/view.h"
#include "ak/gfx/gfx.h"
#include "ak/gfx/mtrl.h"
#include "ak/gfx/mtrl_t.h"
#include "ak/gfx/stg/mtrl.h"

ak_sys_ren
ak_sys_ren_make(ak_gfx* gf,
                ak_wv* wv,
                ak_alct alct)
{
  ak_sys_ren r = { 0 };
  r.alct = alct;
  r.wv = wv;
  r.gf = gf;
  r.ms = ak_mtrlstg_make(gf, alct);
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
  ak_mtrl m =
    ak_mtrlstg_at(r->ms, ak_mtrl_mtrl_col_e);
  m.call_begin(m.ctx);

  ak_wv_itdfs_pt it = ak_wv_itdfs_pt_make(
    r->wv, ak_wv_ett_root(r->wv));
  ak_ett e = 0;
  while (ak_wv_itdfs_pt_next(&it, &e)) {
    ak_assert(
      ak_wv_comp_gmat3_exist(r->wv, e));

    if (!ak_wv_comp_rect_exist(r->wv, e)) {
      continue;
    }
    if (!ak_wv_comp_mtrl_col_exist(r->wv,
                                   e)) {
      continue;
    }

    ak_tf2d_t tf = ak_mat3x3_to_tf2d(
      ak_wv_comp_gmat3(r->wv, e));
    ak_mtrl_col_t m_t =
      ak_wv_comp_mtrl_col(r->wv, e);

    m.push_quad(m.ctx, tf, &m_t);
  }

  m.call_end(m.ctx);

  ak_gfx_frame_end(r->gf);
}
