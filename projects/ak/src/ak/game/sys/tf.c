#include "ak/game/sys/tf.h"
#include "ak/core/math/mat3x3.h"
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
  return stf;
}

void
ak_sys_tf_destroy(ak_sys_tf* stf)
{
  stf->wv = 0;
  ak_alct_invalidate(&stf->alct);
}

void
ak_sys_tf_update(ak_sys_tf* stf)
{
  // store changes locally as well
  ak_wv_itbfs it =
    ak_wv_itbfs_make(stf->wv,
                     ak_wv_ett_root(stf->wv),
                     stf->alct);

  ak_ett e = 0;
  while (ak_wv_itbfs_next(&it, &e)) {
    ak_assert(
      ak_wv_comp_tf2d_exist(stf->wv, e));
    ak_assert(
      ak_wv_comp_tfmat3g_exist(stf->wv, e));

    ak_ett pt = ak_wv_ett_parent(stf->wv, e);
    if (!pt) {
      ak_tf2d_t tf =
        ak_wv_comp_tf2d(stf->wv, e);
      ak_mat3x3 mat3 =
        ak_mat3x3_from_transform(
          tf.pos, tf.rot, tf.scale);
      ak_wcb_comp_tfmat3g_add(
        stf->wcb, e, mat3);
      continue;
    }

    ak_assert(
      ak_wv_comp_tfmat3g_exist(stf->wv, pt));

    ak_tfmat3g_t mat3g_pt =
      ak_wv_comp_tfmat3g(stf->wv, pt);

    ak_tfmat3g_t mat3g =
      ak_wv_comp_tfmat3g(stf->wv, e);

    mat3g = ak_mat3x3_mul(mat3g_pt, mat3g);

    ak_wcb_comp_tfmat3g_add(
      stf->wcb, e, mat3g);
  }
}
