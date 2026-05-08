#include "ak/game/world/view.h"
#include "ak/coll/dq.h"
#include "ak/game/stg/core.h"
#include "ak/game/stg/world.h"
#include "ak/game/world/view_itn.h"

//===== ak_world_v =====//
//--- internal ---//
ak_wv
ak_wv_make(ak_world* w)
{
  ak_wv wv = { 0 };
  wv.w = w;
  return wv;
}
void
ak_wv_destroy(ak_wv* wv)
{
  wv->w = 0;
}

//--- export ---//
bool
ak_wv_ett_exist(ak_wv* wv, ak_ett e)
{
  return ak_world_ett_exist(wv->w, e);
}

ak_ett
ak_wv_ett_root(ak_wv* wv)
{
  return ak_world_ett_root(wv->w);
}

ak_ett
ak_wv_ett_parent(ak_wv* wv, ak_ett e)
{
  return ak_world_ett_parent(wv->w, e);
}

bool
ak_wv_comp_exist(ak_wv* wv,
                 ak_ett e,
                 ak_comp_enum ce)
{
  return ak_world_comp_exist(wv->w, e, ce);
}

#define X(name)                             \
  ak_##name##_t ak_wv_comp_##name(          \
    ak_wv* wv, ak_ett e)                    \
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
ak_wv_itdfs
ak_wv_itdfs_make(ak_wv* wv, ak_ett root)
{
  ak_wv_itdfs it = { 0 };
  it.wv = wv;
  it.root = root;
  it.last =
    ak_world_ett_leftmost(wv->w, root);
  it.started = false;
  return it;
}

bool
ak_wv_itdfs_next(ak_wv_itdfs* it,
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
ak_wv_itchild
ak_wv_itchild_make(ak_wv* wv, ak_ett root)
{
  ak_wv_itchild it = { 0 };
  it.wv = wv;
  it.pt = root;
  it.child =
    ak_world_ett_firstchild(wv->w, root);
  it.started = false;
  return it;
}

bool
ak_wv_itchild_next(ak_wv_itchild* it,
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

//===== ak_wv_itbfs =====//
//--- export ---//
ak_wv_itbfs
ak_wv_itbfs_make(ak_wv* wv,
                 ak_ett root,
                 ak_alct alct)
{
  ak_wv_itbfs it = { 0 };
  it.wv = wv;
  it.q =
    ak_dq_make(sizeof(ak_ett), 10, alct);
  ak_dq_push(&it.q, &root);
  return it;
}
void
ak_wv_itbfs_destroy(ak_wv_itbfs* it)
{
  ak_dq_destroy(&it->q);
}

void
ak_wv_itbfs_reset(ak_wv_itbfs* it,
                  ak_ett root)
{
  ak_dq_clear(&it->q);
  ak_dq_push(&it->q, &root);
}

bool
ak_wv_itbfs_next(ak_wv_itbfs* it,
                 ak_ett* o_e)
{
  if (!ak_dq_pop(&it->q, o_e)) {
    return false;
  }

  ak_wv_itchild cit =
    ak_wv_itchild_make(it->wv, *o_e);
  ak_ett c = 0;
  while (ak_wv_itchild_next(&cit, &c)) {
    ak_dq_push(&it->q, &c);
  }
  return true;
}
