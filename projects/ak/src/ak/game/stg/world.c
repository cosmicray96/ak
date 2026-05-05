#include "ak/game/stg/world.h"
#include "ak/game/stg/comp.h"
#include "ak/game/stg/core.h"
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

ak_ett
ak_world_ett_root(ak_world* w)
{
  return ak_ettstg_root(&w->es);
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
ak_world_ett_remove(ak_world* w, ak_ett e)
{
  ak_ettstg_remove(&w->es, e);
}
ak_ett
ak_world_ett_parent(ak_world* w, ak_ett e)
{
  return ak_ettstg_parent(&w->es, e);
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

//===== ak_world_itdfs =====//
//--- public ---//
ak_world_itdfs
ak_world_itdfs_make(ak_world* w, ak_ett root)
{
  ak_world_itdfs it = { 0 };
  it.w = w;
  it.ettit =
    ak_ettstg_itdfs_make(&w->es, root);
  return it;
}
bool
ak_world_itdfs_next(ak_world_itdfs* it,
                    ak_ett* o_e)
{
  return ak_ettstg_itdfs_next(&it->ettit,
                              o_e);
}

//===== ak_world_itchild =====//
//--- public ---//
ak_world_itchild
ak_world_itchild_make(ak_world* w, ak_ett pt)
{
  ak_world_itchild it = { 0 };
  it.w = w;
  it.child =
    ak_ettstg_firstchild(&w->es, pt);
  it.started = false;
  return it;
}
bool
ak_world_itchild_next(ak_world_itchild* it,
                      ak_ett* o_e)
{
  if (!it->started) {
    it->started = true;
    *o_e = it->child;
    return it->child != 0;
  }

  it->child =
    ak_ettstg_nextsib(&it->w->es, it->child);
  *o_e = it->child;
  return it->child != 0;
}
