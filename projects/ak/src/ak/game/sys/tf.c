#include "ak/game/sys/tf.h"
#include "ak/coll/spa.h"
#include "ak/core/math/mat3x3.h"
#include "ak/debug.h"
#include "ak/game/comp_t.h"
#include "ak/game/world/cb.h"
#include "ak/game/world/view.h"

ak_sys_tf
ak_sys_tf_make(ak_wv* wv,
               ak_wcb* wcb,
               ak_alct alct)
{
  ak_sys_tf stf = { 0 };
  stf.alct = alct;
  stf.wv = wv;
  stf.wcb = wcb;
  stf.it = ak_wv_itdfspre_make(
    wv, ak_wv_ett_root(wv), alct);
  stf.gmat3s =
    ak_spa_make(sizeof(ak_gmat3_t), alct);
  return stf;
}

void
ak_sys_tf_destroy(ak_sys_tf* stf)
{
  ak_spa_destroy(&stf->gmat3s);
  ak_wv_itdfspre_destroy(&stf->it);
  stf->wv = 0;
  ak_alct_invalidate(&stf->alct);
}

void
ak_sys_tf_update(ak_sys_tf* stf)
{
  ak_wv_itdfspre_reset(
    &stf->it, ak_wv_ett_root(stf->wv));
  ak_spa_clear(&stf->gmat3s);

  ak_ett e = ak_wv_itdfspre_next(&stf->it);
  {
    ak_assert(e);
    ak_assert(!ak_wv_ett_parent(stf->wv, e));

    ak_tf2d_t tf =
      ak_wv_comp_tf2d(stf->wv, e);
    ak_mat3 mat = ak_tf2d_to_mat3x3(&tf);
    ak_spa_insert(&stf->gmat3s, e, &mat);
    ak_wcb_comp_gmat3_add(stf->wcb, e, mat);
  }

  while (1) {
    e = ak_wv_itdfspre_next(&stf->it);
    if (!e) {
      break;
    }

    ak_assert(
      ak_wv_comp_tf2d_exist(stf->wv, e));
    ak_assert(
      ak_wv_comp_gmat3_exist(stf->wv, e));

    ak_ett pt = ak_wv_ett_parent(stf->wv, e);
    ak_assert(pt);

    ak_mat3 mat_pt =
      *(ak_mat3*)ak_spa_at(&stf->gmat3s, pt);

    ak_tf2d_t tf2d =
      (ak_wv_comp_tf2d(stf->wv, e));
    ak_mat3 mat = ak_tf2d_to_mat3x3(&tf2d);

    mat = ak_mat3_mul(&mat_pt, &mat);

    ak_spa_insert(&stf->gmat3s, e, &mat);
    ak_wcb_comp_gmat3_add(stf->wcb, e, mat);
  }
}
