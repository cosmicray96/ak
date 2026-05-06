#include "ak/game/world/cbflush.h"
#include "ak/debug.h"
#include "ak/game/comp.h"
#include "ak/game/stg/core.h"
#include "ak/game/stg/ettgen.h"
#include "ak/game/stg/world.h"
#include "ak/game/world/cb_itn.h"

static void
ett_remove_cb(void* eg, ak_ett e)
{
  ak_ettgen_remove((ak_ettgen*)eg, e);
}

void
ak_world_cb_flush(ak_world* w,
                  ak_world_cb* wcb,
                  ak_ettgen* eg)
{
  ak_world_cmditem item = { 0 };
  while (ak_world_cb_pop(wcb, &item)) {

    switch (item.cmd) {
      case ak_world_cmd_ett_new: {
        ak_world_ett_new(w, item.e, item.pt);
        break;
      }
      case ak_world_cmd_ett_remove: {
        ak_world_ett_remove_cb(
          w, item.e, &ett_remove_cb, eg);
        break;
      }
      case ak_world_cmd_comp_modify: {
        ak_world_comp_overwrite(
          w,
          item.e,
          item.ctu.ce,
          ak_comp_tu_comp(&item.ctu));
        break;
      }
      case ak_world_cmd_comp_add: {
        ak_world_comp_add(
          w,
          item.e,
          item.ctu.ce,
          ak_comp_tu_comp(&item.ctu));
        break;
      }
      case ak_world_cmd_comp_remove: {
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
