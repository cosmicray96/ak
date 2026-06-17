#include "ak/game/sys/ren.h"
#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/game/comp.h"
#include "ak/game/comp_t.h"
#include "ak/game/core.h"
#include "ak/game/world/view.h"
#include "ak/gfx/gfx.h"
#include "ak/os/time.h"
#include <stdint.h>

//===== ak_sys_ren =====//
//--- internal ---//
ak_sys_ren
ak_sys_ren_make(ak_alct alct)
{
  ak_sys_ren r = { 0 };
  r.alct = alct;
  r.mtrls = ak_hmn_make(sizeof(ak_da), alct);
  r.free_mtrls =
    ak_da_make(sizeof(ak_ett), alct);
  r.time = ak_fx_f(0);

  return r;
}

void
ak_sys_ren_destroy(ak_sys_ren* r)
{
  ak_da_destroy(&r->free_mtrls);

  ak_hmn_iter it =
    ak_hmn_iter_make(&r->mtrls);
  ak_da* da = 0;
  uint64_t key = 0;
  while (ak_hmn_iter_next(
    &it, &key, (void**)&da)) {
    ak_da_destroy(da);
  }
  ak_hmn_destroy(&r->mtrls);

  ak_alct_invalidate(&r->alct);
}

void
ak_sys_ren_render(ak_sys_ren* r,
                  ak_wv* wv,
                  ak_gcb* gcb,
                  ak_dur delta)
{
  r->time = ak_fxadd(
    r->time, ak_dur_as_secs_fx(delta));
  ak_ett root = ak_wv_ett_root(wv);
  if (!root) {
    return;
  }

  ak_mat3_f vp = { 0 };
  {
    ak_screen_t screen =
      ak_wv_comp_screen(wv, root);
    ak_ett e_cam = 0;
    {
      ak_camera_t cam = { 0 };
      ak_wv_itcomp it =
        ak_wv_itcomp_make(wv, ak_camera_e);
      e_cam = ak_wv_itcomp_next(&it, &cam);
      if (!e_cam) {
        return;
      }
    }
    ak_mat3 cmat =
      ak_wv_comp_gmat3(wv, e_cam);
    vp = ak_gfx_vp_make(&cmat,
                        screen.w,
                        screen.h,
                        ak_fx_f(1.0));
  }

  ak_gfx_batchdata mid = {

    .vp = vp, .time = ak_fx_to_f(r->time)
  };
  ak_gcb_push_batch(gcb, &mid);

  {
    ak_hmn_iter it =
      ak_hmn_iter_make(&r->mtrls);
    uint64_t key = 0;
    void* value;
    while (
      ak_hmn_iter_next(&it, &key, &value)) {
      ak_da* da = value;
      ak_da_clear(da);
    }
  }

  {
    ak_wv_itcomp it =
      ak_wv_itcomp_make(wv, ak_quadsimple_e);
    ak_quadsimple_t qs = { 0 };
    ak_ett e = 0;
    while (true) {
      e = ak_wv_itcomp_next(&it, &qs);
      if (!e) {
        break;
      }

      if (!ak_hmn_exist(&r->mtrls,
                        qs.mtrlid)) {
        ak_da da = ak_da_make(sizeof(ak_ett),
                              r->alct);
        ak_hmn_insert(
          &r->mtrls, qs.mtrlid, &da);
      }

      ak_da* da =
        ak_hmn_at(&r->mtrls, qs.mtrlid);
      ak_da_pushback(da, &e);
    }
  }

  {
    ak_hmn_iter it =
      ak_hmn_iter_make(&r->mtrls);
    uint64_t key = 0;
    void* value = 0;
    while (
      ak_hmn_iter_next(&it, &key, &value)) {
      ak_ett base_id = key;
      ak_da* da = value;

      ak_assert(
        ak_wv_comp_mtrl_exist(wv, base_id));
      ak_mtrl_t mtrl =
        ak_wv_comp_mtrl(wv, base_id);

      ak_gcb_push_call(gcb, &mtrl);

      uint32_t count = ak_da_count(da);
      for (uint32_t i = 0; i < count; i++) {
        ak_ett e = *(ak_ett*)ak_da_at(da, i);

        ak_mat3 gmat3 =
          ak_wv_comp_gmat3(wv, e);
        ak_quadsimple_t qs =
          ak_wv_comp_quadsimple(wv, e);
        ak_gfx_quaddata qd = qs.data;

        ak_mat3_f gmat3f = { 0 };
        ak_mat3_to_f(&gmat3, &gmat3f);

        ak_gcb_push_quad(
          gcb, &qs.data, &gmat3f);
      }
    }
  }

  {
    ak_da_clear(&r->free_mtrls);
    ak_hmn_iter it =
      ak_hmn_iter_make(&r->mtrls);
    uint64_t key = 0;
    void* value = 0;
    while (
      ak_hmn_iter_next(&it, &key, &value)) {
      ak_ett base_id = key;
      ak_da* da = value;
      if (ak_da_count(da) == 0) {
        ak_da_pushback(&r->free_mtrls,
                       &base_id);
      }
    }
  }
  {
    uint32_t count =
      ak_da_count(&r->free_mtrls);
    for (uint32_t i = 0; i < count; i++) {
      ak_ett base_id = *(ak_ett*)ak_da_at(
        &r->free_mtrls, i);
      ak_da* da =
        ak_hmn_at(&r->mtrls, base_id);
      ak_da_destroy(da);
      ak_hmn_remove(&r->mtrls, base_id);
    }
  }
}
