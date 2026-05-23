#include "ak/game/stg/world.h"
#include "ak/coll/fcnst.h"
#include "ak/coll/hmn.h"
#include "ak/game/core.h"
#include "ak/game/stg/comp.h"
#include <threads.h>

//===== ak_world =====//
//--- private ---//
static ak_fcnstid
id_from_ett(ak_world* w, ak_ett e)
{
  ak_fcnstid* id = ak_hmn_at(&w->map, e);
  return *id;
}

//--- public ---//
ak_world
ak_world_make(ak_ett root, ak_alct alct)
{
  ak_world w = { 0 };
  w.tree =
    ak_fcnst_make(sizeof(ak_ett), alct);
  w.map =
    ak_hmn_make(sizeof(ak_fcnstid), alct);
  w.cs = ak_compstg_make(alct);

  ak_fcnstid roott =
    ak_fcnst_add(&w.tree, 0, &root);
  ak_hmn_insert(&w.map, root, &roott);

  return w;
}

void
ak_world_destroy(ak_world* w)
{
  ak_hmn_destroy(&w->map);
  ak_fcnst_destroy(&w->tree);
  ak_compstg_destroy(w->cs);
}

ak_fcnst*
ak_world_tree(ak_world* w)
{
  return &w->tree;
}

ak_ett
ak_world_id_to_ett(ak_world* w,
                   ak_fcnstid id)
{
  return *(ak_ett*)ak_fcnst_at(&w->tree, id);
}
ak_ett
ak_world_ett_to_id(ak_world* w, ak_ett e)
{
  return id_from_ett(w, e);
}

uint32_t
ak_world_ett_count(ak_world* w)
{
  return ak_fcnst_count(&w->tree);
}

ak_ett
ak_world_ett_root(ak_world* w)
{
  return ak_fcnst_root(&w->tree);
}

uint32_t
ak_world_ett_compcount(ak_world* w, ak_ett e)
{
  return ak_compstg_compcount(w->cs, e);
}

uint32_t
ak_world_ett_depth(ak_world* w, ak_ett e)
{
  return ak_fcnst_depth(&w->tree,
                        id_from_ett(w, e));
}

uint32_t
ak_world_ett_order(ak_world* w, ak_ett e)
{
  return ak_fcnst_order(&w->tree,
                        id_from_ett(w, e));
}

bool
ak_world_ett_exist(ak_world* w, ak_ett e)
{
  return ak_hmn_exist(&w->map, e);
}

void
ak_world_ett_new(ak_world* w,
                 ak_ett e,
                 ak_ett pt)
{
  ak_fcnstid id = ak_fcnst_add(
    &w->tree, id_from_ett(w, pt), &e);
  ak_hmn_insert(&w->map, e, &id);
}

void
ak_world_ett_new_last(ak_world* w,
                      ak_ett e,
                      ak_ett pt)
{
  ak_fcnstid id = ak_fcnst_add_last(
    &w->tree, id_from_ett(w, pt), &e);
  ak_hmn_insert(&w->map, e, &id);
}

void
ak_world_ett_remove_cb(
  ak_world* w,
  ak_ett e,
  ak_fcnst_remove_fn remove_fn,
  void* remove_ctx)
{
  ak_compstg_remove_all(w->cs, e);
  ak_fcnst_remove(&w->tree,
                  id_from_ett(w, e),
                  remove_fn,
                  remove_ctx);
}

ak_ett
ak_world_ett_parent(ak_world* w, ak_ett e)
{
  return ak_fcnst_pt(&w->tree,
                     id_from_ett(w, e));
}
ak_ett
ak_world_ett_firstchild(ak_world* w,
                        ak_ett e)
{
  return ak_fcnst_fc(&w->tree,
                     id_from_ett(w, e));
}
ak_ett
ak_world_ett_nextsib(ak_world* w, ak_ett e)
{
  return ak_fcnst_ns(&w->tree,
                     id_from_ett(w, e));
}
ak_ett
ak_world_ett_leftmost(ak_world* w, ak_ett e)
{
  return ak_fcnst_leftmost(
    &w->tree, id_from_ett(w, e));
}

bool
ak_world_comp_exist(ak_world* w,
                    ak_ett e,
                    ak_comp_enum ce)
{
  return ak_compstg_exist(w->cs, e, ce);
}

void*
ak_world_comp_at(ak_world* w,
                 ak_ett e,
                 ak_comp_enum ce)
{
  return ak_compstg_at(w->cs, e, ce);
}

void
ak_world_comp_overwrite(ak_world* w,
                        ak_ett e,
                        ak_comp_enum ce,
                        const void* comp)
{
  ak_compstg_overwrite(w->cs, e, ce, comp);
}

void
ak_world_comp_add_tu(ak_world* w,
                     ak_ett e,
                     const ak_comp_tu* ctu)
{
  ak_compstg_add_tu(w->cs, e, ctu);
}

void
ak_world_comp_add(ak_world* w,
                  ak_ett e,
                  ak_comp_enum ce,
                  const void* comp)
{
  ak_compstg_add(w->cs, e, ce, comp);
}

void
ak_world_comp_remove(ak_world* w,
                     ak_ett e,
                     ak_comp_enum ce)
{
  ak_compstg_remove(w->cs, e, ce);
}

uint32_t
ak_world_comp_count(ak_world* w,
                    ak_comp_enum ce)
{
  return ak_compstg_count(w->cs, ce);
}

void*
ak_world_comp_at_idx(ak_world* w,
                     ak_comp_enum ce,
                     uint32_t idx,
                     ak_ett* o_e)
{
  return ak_compstg_at_idx(
    w->cs, ce, idx, o_e);
}
