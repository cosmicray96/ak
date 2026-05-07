#include "ak/game/world/view.h"
#include "ak/game/stg/world.h"
#include "ak/game/world/view_itn.h"

//===== ak_world_v =====//
//--- internal ---//
ak_world_v
ak_world_v_make(ak_world* w)
{
  ak_world_v wv = { 0 };
  wv.w = w;
  return wv;
}
void
ak_world_v_destroy(ak_world_v* wv)
{
  wv->w = 0;
}

//--- export ---//
bool
ak_world_v_ett_exist(ak_world_v* wv,
                     ak_ett e)
{
  return ak_world_ett_exist(wv->w, e);
}

ak_ett
ak_world_v_ett_root(ak_world_v* wv)
{
  return ak_world_ett_root(wv->w);
}

ak_ett
ak_world_v_ett_parent(ak_world_v* wv,
                      ak_ett e)
{
  return ak_world_ett_parent(wv->w, e);
}

bool
ak_world_v_comp_exist(ak_world_v* wv,
                      ak_ett e,
                      ak_comp_enum ce)
{
  return ak_world_comp_exist(wv->w, e, ce);
}

#define X(name)                             \
  ak_##name##_t ak_world_v_comp_##name(     \
    ak_world_v* wv, ak_ett e)               \
  {                                         \
    ak_assert(ak_world_comp_exist(          \
      wv->w, e, ak_##name##_e));            \
    return *(ak_##name##_t*)                \
      ak_world_comp_at(                     \
        wv->w, e, ak_##name##_e);           \
  }
#include "ak/game/comp.inc"
#undef X

//===== ak_world_v_itdfs =====//
//--- export ---//
ak_world_v_itdfs
ak_world_v_itdfs_make(ak_world_v* wv,
                      ak_ett root)
{
  ak_world_v_itdfs it = { 0 };
  it.wv = wv;
  it.root = root;
  it.last =
    ak_world_ett_leftmost(wv->w, root);
  it.started = false;
  return it;
}

bool
ak_world_v_itdfs_next(ak_world_v_itdfs* it,
                      ak_ett* o_e)
{
  if (!it->started) {
    it->started = true;
    *o_e = it->last;
    return true;
  }

  if (it->last == it->root) {
    return false;
  }

  ak_ett ns = ak_world_ett_nextsib(it->wv->w,
                                   it->last);
  if (ns) {
    it->last =
      ak_world_ett_leftmost(it->wv->w, ns);
  } else {
    it->last = ak_world_ett_parent(it->wv->w,
                                   it->last);
  }

  *o_e = it->last;
  return true;
}

//===== ak_world_v_itchild =====//
//--- export ---//
ak_world_v_itchild
ak_world_v_itchild_make(ak_world_v* wv,
                        ak_ett root)
{
  ak_world_v_itchild it = { 0 };
  it.wv = wv;
  it.pt = root;
  it.child =
    ak_world_ett_firstchild(wv->w, root);
  it.started = false;
  return it;
}

bool
ak_world_v_itchild_next(
  ak_world_v_itchild* it,
  ak_ett* o_e)
{
  if (!it->started) {
    it->started = true;
    *o_e = it->child;
    return it->child != 0;
  }

  it->child = ak_world_ett_nextsib(
    it->wv->w, it->child);
  *o_e = it->child;
  return it->child != 0;
}
