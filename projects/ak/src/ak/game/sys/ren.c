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
typedef struct
{
  ak_da da;
  bool used;
} map_item;

//--- internal ---//
ak_sys_ren
ak_sys_ren_make(ak_alct alct)
{
  ak_sys_ren r = { 0 };
  r.alct = alct;
  r.mtrls =
    ak_hmn_make(sizeof(map_item), alct);
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
  map_item* mi = 0;
  uint64_t key = 0;
  while (ak_hmn_iter_next(
    &it, &key, (void**)&mi)) {
    ak_da_destroy(&mi->da);
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

  ak_mtrl_indata mid = {

    .vp = vp, .time = ak_fx_to_f(r->time)
  };
  ak_gcb_push_mtrlin(gcb, &mid);

  {
    ak_hmn_iter it =
      ak_hmn_iter_make(&r->mtrls);
    uint64_t key = 0;
    void* value;
    while (
      ak_hmn_iter_next(&it, &key, &value)) {
      map_item* mi = value;
      mi->used = false;
      ak_da_clear(&mi->da);
    }
  }

  {
    ak_wv_itcomp it =
      ak_wv_itcomp_make(wv, ak_mtrl_e);
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
    uint64_t key = 0;
    void* value = 0;
    while (
      ak_hmn_iter_next(&it, &key, &value)) {
      ak_ett base_id = key;
      map_item* mi = value;

      ak_assert(ak_wv_comp_mtrl_base_exist(
        wv, base_id));
      ak_mtrl_base_t base_t =
        ak_wv_comp_mtrl_base(wv, base_id);
      ak_mtrl_basedata bd = base_t.data;

      ak_gcb_push_mtrl(gcb, &base_t.data);

      uint32_t count = ak_da_count(&mi->da);
      for (uint32_t i = 0; i < count; i++) {
        ak_ett e =
          *(ak_ett*)ak_da_at(&mi->da, i);

        ak_mat3 gmat3 =
          ak_wv_comp_gmat3(wv, e);
        ak_mtrl_t mat =
          ak_wv_comp_mtrl(wv, e);
        ak_mtrl_quaddata qd = mat.data;

        ak_mat3_f gmat3f = { 0 };
        ak_mat3_to_f(&gmat3, &gmat3f);

        ak_gcb_push_quad(
          gcb, &mat.data, &gmat3f);
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
      ak_ett base_id = *(ak_ett*)ak_da_at(
        &r->free_mtrls, i);
      map_item* mi =
        ak_hmn_at(&r->mtrls, base_id);
      ak_da_destroy(&mi->da);
      ak_hmn_remove(&r->mtrls, base_id);
    }
  }
}
