#include "ak/game/world/cb.h"
#include "ak/coll/dq.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/comp.h"
#include "ak/game/stg/core.h"
#include "ak/game/stg/ettgen.h"
#include "ak/game/world/cb_itn.h"

//===== ak_gcmdbuff  =====//

//--- private ---//
struct ak_world_cb
{
  ak_alct alct;
  ak_ettgen* eg;
  ak_dq cmds;
};
//--- internal ---//
ak_world_cb
ak_world_cb_make(ak_ettgen* eg, ak_alct alct)
{
  ak_world_cb wcb;
  return wcb;
}

void
ak_world_cb_destroy(ak_world_cb* wcb)
{
  // empty
}

//--- export ---//
ak_ex ak_ett
ak_world_cb_ett_new(ak_world_cb* wcb,
                    ak_ett pt);
ak_ex void
ak_world_cb_ett_remove(ak_world_cb* wcb,
                       ak_ett e);

void
ak_world_cb_comp_add(ak_world_cb* wcb,
                     ak_ett ett,
                     ak_comp_enum ce,
                     const void* comp);
void
ak_world_cb_comp_remove(ak_world_cb* wcb,
                        ak_ett ett,
                        ak_comp_enum ce);
