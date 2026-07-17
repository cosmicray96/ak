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
  ak_wcbitem item = { 0 };
  item.cmd = ak_wcbtype_comp_add;
  item.e = e;
  ak_comp_tu_make_ip(&item.ctu, ce, comp);

  ak_dq_push(&wcb->cmds, &item);
}

static void
comp_remove(ak_wcb* wcb,
            ak_ett e,
            ak_comp_enum ce)
{
  ak_wcbitem item = { 0 };
  item.cmd = ak_wcbtype_comp_remove;
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
  wcb.cmds =
    ak_dq_make(sizeof(ak_wcbitem), alct);
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
            ak_wcbitem* o_item)
{
  return ak_dq_peek(&wcb->cmds, idx, o_item);
}

bool
ak_wcb_pop(ak_wcb* wcb, ak_wcbitem* o_item)
{
  return ak_dq_pop(&wcb->cmds, o_item);
}

void
ak_wcb_joinback(ak_wcb* dest, ak_wcb* src)
{
  ak_wcbitem cmd = { 0 };
  while (ak_dq_pop(&src->cmds, &cmd)) {
    ak_dq_push(&dest->cmds, &cmd);
  }
}

/*

void
ak_world_graft(ak_world* dest,
               ak_world* src,
               ak_ett dest_pt,
               ak_idgen* ig,
               ak_alct alct)
{
  ak_hmn map =
    ak_hmn_make(sizeof(ak_ett), alct);

  ak_wv wv = ak_wv_make(src);
  ak_wv_itdfspre it = ak_wv_itdfspre_make(
    &wv, ak_wv_ett_root(&wv), alct);

  bool first = true;

  while (true) {
    ak_ett e = ak_wv_itdfspre_next(&it);
    if (!e) {
      break;
    }
    ak_ett pt = ak_wv_ett_parent(&wv, e);
    if (first) {
      ak_hmn_insert(&map, pt, &dest_pt);
      first = false;
    }

    ak_ett new_e = ak_idgen_new(ig);
    ak_hmn_insert(&map, e, &new_e);

    ak_ett new_pt =
      *(ak_ett*)ak_hmn_at(&map, pt);

    ak_world_ett_new(dest, new_e, new_pt);

    ak_wv_itettcomp itec =
      ak_wv_itettcomp_make(&wv, e);
    ak_comp_tu ctu = { 0 };
    while (
      ak_wv_itettcomp_next(&itec, &ctu)) {
      ak_world_comp_add_tu(
        dest, new_e, &ctu);
    }
  }

  ak_wv_itdfspre_destroy(&it);
  ak_hmn_destroy(&map);
}
*/
void
ak_wcb_wv(ak_wcb* wcb,
          ak_wv* wv,
          ak_ett new_pt)
{
}

//--- export ---//
ak_ett
ak_wcb_ett_new(ak_wcb* wcb, ak_ett pt)
{
  ak_ett e = ak_idgen_new(wcb->ig);
  ak_wcbitem item = { 0 };
  item.cmd = ak_wcbtype_ett_new;
  item.e = e;
  item.pt = pt;

  ak_dq_push(&wcb->cmds, &item);

  return e;
}

void
ak_wcb_ett_remove(ak_wcb* wcb, ak_ett e)
{
  ak_wcbitem item = { 0 };
  item.cmd = ak_wcbtype_ett_remove;
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
