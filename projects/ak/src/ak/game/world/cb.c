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
  wcb.alct = alct;
  wcb.eg = eg;
  wcb.cmds = ak_dq_make(
    sizeof(ak_world_cmditem), 10, alct);
  return wcb;
}

void
ak_world_cb_destroy(ak_world_cb* wcb)
{
  ak_dq_destroy(&wcb->cmds);
  wcb->eg = 0;
  ak_alct_invalidate(&wcb->alct);
}

uint32_t
ak_world_cb_count(ak_world_cb* wcb)
{
  return ak_dq_count(&wcb->cmds);
}

bool
ak_world_cb_peek(ak_world_cb* wcb,
                 uint32_t idx,
                 ak_world_cmditem* o_item)
{
  return ak_dq_peek(&wcb->cmds, idx, o_item);
}

bool
ak_world_cb_pop(ak_world_cb* wcb,
                ak_world_cmditem* o_item)
{
  return ak_dq_pop(&wcb->cmds, o_item);
}

//--- export ---//
ak_ett
ak_world_cb_ett_new(ak_world_cb* wcb,
                    ak_ett pt)
{
  ak_ett e = ak_ettgen_new(wcb->eg);
  ak_world_cmditem item = { 0 };
  item.cmd = ak_world_cmd_ett_new;
  item.e = e;
  item.pt = pt;

  ak_dq_push(&wcb->cmds, &item);

  return e;
}

void
ak_world_cb_ett_remove(ak_world_cb* wcb,
                       ak_ett e)
{
  ak_world_cmditem item = { 0 };
  item.cmd = ak_world_cmd_ett_remove;
  item.e = e;

  ak_dq_push(&wcb->cmds, &item);
}

void
ak_world_cb_comp_modify(ak_world_cb* wcb,
                        ak_ett e,
                        ak_comp_enum ce,
                        const void* comp)
{
  ak_world_cmditem item = { 0 };
  item.cmd = ak_world_cmd_comp_modify;
  item.e = e;
  ak_comp_tu_make_ip(&item.ctu, ce, comp);

  ak_dq_push(&wcb->cmds, &item);
}

void
ak_world_cb_comp_add(ak_world_cb* wcb,
                     ak_ett e,
                     ak_comp_enum ce,
                     const void* comp)
{
  ak_world_cmditem item = { 0 };
  item.cmd = ak_world_cmd_comp_add;
  item.e = e;
  ak_comp_tu_make_ip(&item.ctu, ce, comp);

  ak_dq_push(&wcb->cmds, &item);
}

void
ak_world_cb_comp_remove(ak_world_cb* wcb,
                        ak_ett e,
                        ak_comp_enum ce)
{
  ak_world_cmditem item = { 0 };
  item.cmd = ak_world_cmd_comp_remove;
  item.e = e;
  item.ctu.ce = ce;

  ak_dq_push(&wcb->cmds, &item);
}
