#include "ak/game/sys/ren.h"
#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/comp.h"
#include "ak/game/comp_t.h"
#include "ak/game/stg/core.h"
#include "ak/game/world/view.h"
#include "ak/gfx/gfx.h"
#include "ak/gfx/mtrl.h"
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
  ak_fx pixelsize = ak_fx_f(1.0f);
  ak_ett root = ak_wv_ett_root(r->wv);
  ak_screen_t screen =
    ak_wv_comp_screen(r->wv, root);

  ak_ett e_cam = 0;
  {
    ak_camera_t cam = { 0 };
    ak_wv_itcomp it =
      ak_wv_itcomp_make(r->wv, ak_camera_e);
    if (!ak_wv_itcomp_next(
          &it, &e_cam, &cam)) {
      return;
    }
  }

  ak_mat3x3 vp = { 0 };
  {
    ak_mat3x3 cmat3x3 =
      ak_wv_comp_gmat3(r->wv, e_cam);
    ak_mat3x3 cimat3x3 =
      ak_mat3x3_inv_fast(&cmat3x3);
    ak_mat3x3 proj = ak_mat3x3_identity();
    {
      ak_fx sx = ak_fxdiv(
        ak_fxmul(pixelsize, ak_fx_f(2.0f)),
        ak_fx_f(screen.w));
      ak_fx sy = ak_fxdiv(
        ak_fxmul(pixelsize, ak_fx_f(-2.0f)),
        ak_fx_f(screen.h));
      proj.m[0][0] = sx;
      proj.m[1][1] = sy;
    }
    vp = ak_mat3x3_mul(proj, cimat3x3);
  }

  {
    ak_hmn_iter it =
      ak_hmn_iter_make(&r->mtrls);
    ak_da* da = 0;
    uint64_t me = 0;
    while (ak_hmn_iter_next_u64(
      &it, &me, (void**)&da)) {
      ak_da_clear(da);
    }
  }
  {
    ak_wv_itdfs_pt it = ak_wv_itdfs_pt_make(
      r->wv, ak_wv_ett_root(r->wv));
    ak_ett e = 0;
    while (ak_wv_itdfs_pt_next(&it, &e)) {
      if (!ak_wv_comp_rect_exist(r->wv, e)) {
        continue;
      }
      if (!ak_wv_comp_mtrl_exist(r->wv, e)) {
        continue;
      }
      ak_mtrl_t mtrl =
        ak_wv_comp_mtrl(r->wv, e);
      ak_da* da =
        ak_hmn_at_u64(&r->mtrls, mtrl.me);
      ak_da_pushback(da, &e);
    }
  }

  ak_gfx_frame_begin(r->gf);
  {
    ak_hmn_iter it =
      ak_hmn_iter_make(&r->mtrls);
    ak_da* da = 0;
    uint64_t me = 0;
    while (ak_hmn_iter_next_u64(
      &it, &me, (void**)&da)) {

      ak_mtrl m = ak_mtrlstg_at(
        r->ms, (ak_mtrl_enum)me);
      m.call_begin(m.ctx, &vp);

      uint32_t count = ak_da_count(da);
      for (uint32_t i = 0; i < count; i++) {
        ak_ett e =
          *(ak_ett*)ak_da_at_impl(da, i);

        ak_mat3x3 gmat3x3 =
          ak_wv_comp_gmat3(r->wv, e);
        ak_mtrl_t mat =
          ak_wv_comp_mtrl(r->wv, e);
        ak_comp_tu mat_comp = ak_wv_comp_tu(
          r->wv,
          e,
          ak_mtrl_to_comp_e(mat.me));

        m.push_quad(
          m.ctx,
          &gmat3x3,
          ak_comp_tu_comp(&mat_comp));
      }

      m.call_end(m.ctx);
    }
  }
  ak_gfx_frame_end(r->gf);
}
/*

  ak_gfx_frame_begin(r->gf);
  ak_mtrl m =
    ak_mtrlstg_at(r->ms, ak_mtrl_mtrl_col_e);
  m.call_begin(m.ctx, &vp);

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
*/
