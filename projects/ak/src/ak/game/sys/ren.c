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
#include "ak/gfx/core.h"
#include "ak/gfx/gfx.h"
#include "ak/gfx/mtrl/stg.h"
#include "ak/os/time.h"
#include <stdint.h>

//===== ak_sys_ren =====//
typedef struct
{
  ak_da da;
  bool used;
} map_item;

//--- internal ---//
ak_sys_ren
ak_sys_ren_make(ak_gfx* gf,
                ak_gcb* gcb,
                ak_mtrlstg* ms,
                ak_wv* wv,
                ak_alct alct)
{
  ak_sys_ren r = { 0 };
  r.alct = alct;
  r.wv = wv;
  r.gf = gf;
  r.ms = ms;
  r.mtrls =
    ak_hmn_make(sizeof(map_item), alct);
  r.free_mtrls =
    ak_da_make(sizeof(ak_ett), alct);
  r.time = ak_fx_f(0);
  r.gcb = gcb;

  return r;
}

void
ak_sys_ren_destroy(ak_sys_ren* r)
{
  ak_da_destroy(&r->free_mtrls);

  ak_hmn_iter it =
    ak_hmn_iter_make(&r->mtrls);
  map_item* mi = 0;
  uint64_t key = 0;
  while (ak_hmn_iter_next(
    &it, &key, (void**)&mi)) {
    ak_da_destroy(&mi->da);
  }
  ak_hmn_destroy(&r->mtrls);

  ak_alct_invalidate(&r->alct);
  r->gf = 0;
  r->wv = 0;
}

void
ak_sys_ren_render(ak_sys_ren* r,
                  ak_dur delta)
{
  r->time = ak_fxadd(
    r->time, ak_dur_as_secs_fx(delta));
  ak_ett root = ak_wv_ett_root(r->wv);

  ak_mat3_f vp = { 0 };
  {
    ak_screen_t screen =
      ak_wv_comp_screen(r->wv, root);
    ak_ett e_cam = 0;
    {
      ak_camera_t cam = { 0 };
      ak_wv_itcomp it = ak_wv_itcomp_make(
        r->wv, ak_camera_e);
      e_cam = ak_wv_itcomp_next(&it, &cam);
      if (!e_cam) {
        return;
      }
    }
    ak_mat3 cmat =
      ak_wv_comp_gmat3(r->wv, e_cam);
    vp = ak_gfx_vp_make(&cmat,
                        screen.w,
                        screen.h,
                        ak_fx_f(1.0));
  }
  {
    ak_hmn_iter it =
      ak_hmn_iter_make(&r->mtrls);
    map_item* mi = 0;
    uint64_t key = 0;
    while (ak_hmn_iter_next(
      &it, &key, (void**)&mi)) {
      mi->used = false;
      ak_da_clear(&mi->da);
    }
  }

  {
    ak_wv_itcomp it =
      ak_wv_itcomp_make(r->wv, ak_mtrl_e);
    ak_mtrl_t mat = { 0 };
    ak_ett e = 0;
    while (1) {
      e = ak_wv_itcomp_next(&it, &mat);
      if (!e) {
        break;
      }

      if (!ak_hmn_exist(&r->mtrls,
                        mat.base_id)) {
        map_item mi = { 0 };
        mi.da = ak_da_make(sizeof(ak_ett),
                           r->alct);
        mi.used = false;
        ak_hmn_insert(
          &r->mtrls, mat.base_id, &mi);
      }

      map_item* mi =
        ak_hmn_at(&r->mtrls, mat.base_id);
      mi->used = true;
      ak_da_pushback(&mi->da, &e);
    }
  }

  {
    ak_hmn_iter it =
      ak_hmn_iter_make(&r->mtrls);
    map_item* mi = 0;
    uint64_t key = 0;
    ak_ett base_id = 0;
    while (ak_hmn_iter_next(
      &it, &key, (void**)&mi)) {
      base_id = key;

      ak_assert(ak_wv_comp_mtrl_base_exist(
        r->wv, base_id));
      ak_mtrl_base_t base_t =
        ak_wv_comp_mtrl_base(r->wv, base_id);

      ak_gcb_push_mtrl(
        r->gcb, &base_t.data, &vp, r->time);

      uint32_t count = ak_da_count(&mi->da);
      for (uint32_t i = 0; i < count; i++) {
        ak_ett e = *(ak_ett*)ak_da_at_impl(
          &mi->da, i);

        ak_mat3 gmat3x3 =
          ak_wv_comp_gmat3(r->wv, e);
        ak_mtrl_t mat =
          ak_wv_comp_mtrl(r->wv, e);

        ak_gcb_push_quad(
          r->gcb, &mat.data, &gmat3x3);
      }
    }
  }

  {
    ak_da_clear(&r->free_mtrls);
    ak_hmn_iter it =
      ak_hmn_iter_make(&r->mtrls);
    map_item* mi = 0;
    uint64_t key = 0;
    ak_ett base_id = 0;
    while (ak_hmn_iter_next(
      &it, &key, (void**)&mi)) {
      base_id = key;
      if (mi->used) {
        continue;
      }
      ak_da_pushback(&r->free_mtrls,
                     &base_id);
    }
  }
  {
    uint32_t count =
      ak_da_count(&r->free_mtrls);
    for (uint32_t i = 0; i < count; i++) {
      ak_ett base_id =
        *(ak_ett*)ak_da_at_impl(
          &r->free_mtrls, i);
      map_item* mi =
        ak_hmn_at(&r->mtrls, base_id);
      ak_da_destroy(&mi->da);
      ak_hmn_remove(&r->mtrls, base_id);
    }
  }
}
