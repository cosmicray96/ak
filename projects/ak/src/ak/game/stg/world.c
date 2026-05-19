#include "ak/game/stg/world.h"
#include "ak/game/core.h"
#include "ak/game/stg/comp.h"
#include "ak/game/stg/ett.h"
#include <threads.h>

//===== ak_world =====//
//--- public ---//
ak_world
ak_world_make(ak_ett root, ak_alct alct)
{
  ak_world w = { 0 };
  w.es = ak_ettstg_make(root, alct);
  w.cs = ak_compstg_make(alct);
  return w;
}

void
ak_world_destroy(ak_world* w)
{
  ak_ettstg_destroy(&w->es);
  ak_compstg_destroy(w->cs);
}

uint32_t
ak_world_ett_count(ak_world* w)
{
  return ak_ettstg_count(&w->es);
}

ak_ett
ak_world_ett_root(ak_world* w)
{
  return ak_ettstg_root(&w->es);
}

uint32_t
ak_world_ett_compcount(ak_world* w, ak_ett e)
{
  return ak_compstg_compcount(w->cs, e);
}

uint32_t
ak_world_ett_depth(ak_world* w, ak_ett e)
{
  return ak_ettstg_depth(&w->es, e);
}

uint32_t
ak_world_ett_order(ak_world* w, ak_ett e)
{
  return ak_ettstg_order(&w->es, e);
}

bool
ak_world_ett_exist(ak_world* w, ak_ett e)
{
  return ak_ettstg_exist(&w->es, e);
}

void
ak_world_ett_new(ak_world* w,
                 ak_ett e,
                 ak_ett pt)
{
  ak_ettstg_newett(&w->es, e, pt);
}

void
ak_world_ett_new_last(ak_world* w,
                      ak_ett e,
                      ak_ett pt)
{
  ak_ettstg_newett_last(&w->es, e, pt);
}

void
ak_world_ett_remove_cb(
  ak_world* w,
  ak_ett e,
  ak_ettstg_remove_fn remove_fn,
  void* remove_ctx)
{
  ak_compstg_remove_all(w->cs, e);
  ak_ettstg_remove_cb(
    &w->es, e, remove_fn, remove_ctx);
}

ak_ett
ak_world_ett_parent(ak_world* w, ak_ett e)
{
  return ak_ettstg_parent(&w->es, e);
}
ak_ett
ak_world_ett_firstchild(ak_world* w,
                        ak_ett e)
{
  return ak_ettstg_firstchild(&w->es, e);
}
ak_ett
ak_world_ett_nextsib(ak_world* w, ak_ett e)
{
  return ak_ettstg_nextsib(&w->es, e);
}
ak_ett
ak_world_ett_leftmost(ak_world* w, ak_ett e)
{
  return ak_ettstg_leftmost(&w->es, e);
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
