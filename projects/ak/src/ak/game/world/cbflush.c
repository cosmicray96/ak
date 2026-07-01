#include "ak/game/world/cbflush.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/math/tf2d.h"
#include "ak/debug.h"
#include "ak/game/comp.h"
#include "ak/game/comp_t.h"
#include "ak/game/stg/world.h"
#include "ak/game/world/cb_itn.h"

static void
ett_remove_cb(void* ig, ak_ett e)
{
  ak_idgen_remove((ak_idgen*)ig, e);
}

void
ak_world_cb_flush(ak_world* w, ak_wcb* wcb)
{
  ak_idgen* ig = ak_wcb_idgen(wcb);
  ak_wcbitem item = { 0 };
  ak_tf2d_t tf2d = ak_tf2d_identity();
  ak_gmat3_t mat3 = ak_mat3_identity();
  while (ak_wcb_pop(wcb, &item)) {

    switch (item.cmd) {
      case ak_wcbtype_ett_new: {
        ak_world_ett_new(w, item.e, item.pt);
        ak_world_comp_add(
          w, item.e, ak_tf2d_e, &tf2d);
        ak_world_comp_add(
          w, item.e, ak_gmat3_e, &mat3);
        break;
      }
      case ak_wcbtype_ett_remove: {
        ak_world_ett_remove_cb(
          w, item.e, &ett_remove_cb, ig);
        break;
      }
      case ak_wcbtype_comp_add: {
        if (ak_world_comp_exist(
              w, item.e, item.ctu.ce)) {
          ak_world_comp_overwrite(
            w,
            item.e,
            item.ctu.ce,
            ak_comp_tu_comp(&item.ctu));
        } else {
          ak_world_comp_add(
            w,
            item.e,
            item.ctu.ce,
            ak_comp_tu_comp(&item.ctu));
        }
        break;
      }
      case ak_wcbtype_comp_remove: {
        ak_world_comp_remove(
          w, item.e, item.ctu.ce);
        break;
      }

      default: {
        ak_assert(false);
      }
    }
  }
}
