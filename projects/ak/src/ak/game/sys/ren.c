#include "ak/game/sys/ren.h"
#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/game/comp.h"
#include "ak/game/comp_t.h"
#include "ak/game/stg/core.h"
#include "ak/game/world/view.h"
#include "ak/gfx/gfx.h"
#include "ak/gfx/mtrl.h"
#include "ak/gfx/mtrl_t.h"
#include "ak/gfx/stg/mtrl.h"
#include <stdint.h>

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
  r.mtrls = ak_hmn_make(sizeof(ak_da), alct);
  for (uint32_t i = 0; i < ak_mtrl_count_e;
       i++) {
    ak_da da =
      ak_da_make(sizeof(ak_ett), alct);
    ak_hmn_insert_u64(
      &r.mtrls, (ak_mtrl_enum)i, &da);
  }
  return r;
}

void
ak_sys_ren_destroy(ak_sys_ren* r)
{

  ak_hmn_iter it =
    ak_hmn_iter_make(&r->mtrls);
  ak_da* da = 0;
  uint64_t me = 0;
  while (ak_hmn_iter_next_u64(
    &it, &me, (void**)&da)) {
    ak_da_destroy(da);
  }
  ak_hmn_destroy(&r->mtrls);

  ak_alct_invalidate(&r->alct);
  r->gf = 0;
  r->wv = 0;
}

void
ak_sys_ren_render(ak_sys_ren* r)
{

  ak_log_assert(false,
                "link ak_screen_t, make "
                "comp iter for camera");
  ak_fx32 pixelsize = ak_fx32_f(1.0f);
  ak_ett root = ak_wv_ett_root(r->wv);
  ak_screen_t screen =
    ak_wv_comp_screen(r->wv, root);
  ak_mat3x3 cmat3x3 =
    ak_wv_comp_gmat3(r->wv, root);
  ak_mat3x3 cimat3x3 =
    ak_mat3x3_inv_fast(&cmat3x3);
  ak_mat3x3 proj = ak_mat3x3_identity();
  {
    ak_fx32 sx = ak_fx32_div(
      ak_fx32_mul(pixelsize,
                  ak_fx32_f(2.0f)),
      ak_fx32_f(screen.w));
    ak_fx32 sy = ak_fx32_div(
      ak_fx32_mul(pixelsize,
                  ak_fx32_f(-2.0f)),
      ak_fx32_f(screen.h));
    proj.m[0][0] = sx;
    proj.m[1][1] = sy;
  }
  ak_mat3x3 vp =
    ak_mat3x3_mul(proj, cimat3x3);

  ak_gfx_frame_begin(r->gf);
  ak_mtrl m =
    ak_mtrlstg_at(r->ms, ak_mtrl_mtrl_col_e);
  m.call_begin(m.ctx, &cmat3x3);

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

    ak_mat3x3 gmat3x3 =
      ak_wv_comp_gmat3(r->wv, e);
    ak_mtrl_col_t m_t =
      ak_wv_comp_mtrl_col(r->wv, e);

    m.push_quad(m.ctx, &gmat3x3, &m_t);
  }

  m.call_end(m.ctx);

  ak_gfx_frame_end(r->gf);
}
