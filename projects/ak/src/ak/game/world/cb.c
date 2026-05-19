#include "ak/game/world/cb.h"
#include "ak/coll/dq.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/comp.h"
#include "ak/game/world/cb_itn.h"

//===== ak_gcmdbuff  =====//
//--- private ---//
static void
comp_add(ak_wcb* wcb,
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

static void
comp_remove(ak_wcb* wcb,
            ak_ett e,
            ak_comp_enum ce)
{
  ak_world_cmditem item = { 0 };
  item.cmd = ak_world_cmd_comp_remove;
  item.e = e;
  item.ctu.ce = ce;

  ak_dq_push(&wcb->cmds, &item);
}

//--- internal ---//
ak_wcb
ak_wcb_make(ak_idgen* ig, ak_alct alct)
{
  ak_wcb wcb;
  wcb.alct = alct;
  wcb.ig = ig;
  wcb.cmds = ak_dq_make(
    sizeof(ak_world_cmditem), alct);
  return wcb;
}

void
ak_wcb_destroy(ak_wcb* wcb)
{
  ak_dq_destroy(&wcb->cmds);
  wcb->ig = 0;
  ak_alct_invalidate(&wcb->alct);
}

uint32_t
ak_wcb_count(ak_wcb* wcb)
{
  return ak_dq_count(&wcb->cmds);
}

bool
ak_wcb_peek(ak_wcb* wcb,
            uint32_t idx,
            ak_world_cmditem* o_item)
{
  return ak_dq_peek(&wcb->cmds, idx, o_item);
}

bool
ak_wcb_pop(ak_wcb* wcb,
           ak_world_cmditem* o_item)
{
  return ak_dq_pop(&wcb->cmds, o_item);
}

void
ak_wcb_joinback(ak_wcb* dest, ak_wcb* src)
{
  ak_world_cmditem cmd = { 0 };
  while (ak_dq_pop(&src->cmds, &cmd)) {
    ak_dq_push(&dest->cmds, &cmd);
  }
}

//--- export ---//
ak_ett
ak_wcb_ett_new(ak_wcb* wcb, ak_ett pt)
{
  ak_ett e = ak_idgen_new(wcb->ig);
  ak_world_cmditem item = { 0 };
  item.cmd = ak_world_cmd_ett_new;
  item.e = e;
  item.pt = pt;

  ak_dq_push(&wcb->cmds, &item);

  return e;
}

void
ak_wcb_ett_remove(ak_wcb* wcb, ak_ett e)
{
  ak_world_cmditem item = { 0 };
  item.cmd = ak_world_cmd_ett_remove;
  item.e = e;

  ak_dq_push(&wcb->cmds, &item);
}

#define ak_d_comp_x(name)                   \
  void ak_wcb_comp_##name##_add(            \
    ak_wcb* wcb,                            \
    ak_ett e,                               \
    ak_as_comp_t(name) comp)                \
  {                                         \
    comp_add(                               \
      wcb, e, ak_as_comp_e(name), &comp);   \
  }                                         \
  void ak_wcb_comp_##name##_remove(         \
    ak_wcb* wcb, ak_ett e)                  \
  {                                         \
    comp_remove(                            \
      wcb, e, ak_as_comp_e(name));          \
  }
#include "ak/game/comp.inc"
#undef ak_d_comp_x
