#include "ak/game/world/view.h"
#include "ak/core/mem/ptr.h"
#include "ak/debug.h"
#include "ak/game/comp.h"
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
uint32_t
ak_wv_ett_depth(ak_wv* wv, ak_ett e)
{
  return ak_world_ett_depth(wv->w, e);
}

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

ak_comp_tu
ak_wv_comp_tu(ak_wv* wv,
              ak_ett e,
              ak_comp_enum ce)
{
  void* comp =
    ak_world_comp_at(wv->w, e, ce);
  ak_comp_tu tu = { 0 };
  ak_comp_tu_make_ip(&tu, ce, comp);
  return tu;
}

#define ak_d_comp_x(name)                   \
  ak_as_comp_t(name)                        \
    ak_wv_comp_##name(ak_wv* wv, ak_ett e)  \
  {                                         \
    ak_assert(ak_world_comp_exist(          \
      wv->w, e, ak_as_comp_e(name)));       \
    return *(ak_as_comp_t(name)*)           \
      ak_world_comp_at(                     \
        wv->w, e, ak_as_comp_e(name));      \
  }
#include "ak/game/comp.inc"
#undef ak_d_comp_x

//===== ak_wv_itchild =====//
ak_wv_itchild
ak_wv_itchild_make(ak_wv* wv, ak_ett pt)
{
  return (ak_wv_itchild){
    .it = ak_fcnst_itchild_make(
      ak_world_tree(wv->w),
      ak_world_ett_to_id(wv->w, pt)),
    .wv = wv
  };
}

ak_ett
ak_wv_itchild_next(ak_wv_itchild* it)
{
  ak_fcnstid id =
    ak_fcnst_itchild_next(&it->it);
  return id ? *(ak_ett*)ak_fcnst_at(
                ak_world_tree(it->wv->w), id)
            : 0;
}

//===== ak_wv_itettcomp =====//
ak_wv_itettcomp
ak_wv_itettcomp_make(ak_wv* wv, ak_ett e)
{
  ak_wv_itettcomp it = { 0 };
  it.wv = wv;
  it.e = e;
  it.ce = 0;
  return it;
}

bool
ak_wv_itettcomp_next(ak_wv_itettcomp* it,
                     ak_comp_tu* o_ctu)
{
  for (; it->ce < ak_comp_count_e;
       it->ce++) {
    if (ak_world_comp_exist(
          it->wv->w, it->e, it->ce)) {
      void* comp = ak_world_comp_at(
        it->wv->w, it->e, it->ce);
      ak_comp_tu_make_ip(
        o_ctu, it->ce, comp);

      it->ce++;
      return true;
    }
  }
  return false;
}

//===== ak_wv_itancestor =====//
ak_wv_itancestor
ak_wv_itancestor_make(ak_wv* wv, ak_ett e)
{
  return (ak_wv_itancestor){
    .it = ak_fcnst_itancestor_make(
      ak_world_tree(wv->w),
      ak_world_ett_to_id(wv->w, e)),
    .wv = wv
  };
}

ak_ett
ak_wv_itancestor_next(ak_wv_itancestor* it)
{
  ak_fcnstid id =
    ak_fcnst_itancestor_next(&it->it);
  return id ? *(ak_ett*)ak_fcnst_at(
                ak_world_tree(it->wv->w), id)
            : 0;
}

//===== ak_wv_itdfspost =====//
ak_wv_itdfspost
ak_wv_itdfspost_make(ak_wv* wv, ak_ett root)
{
  return (ak_wv_itdfspost){
    .it = ak_fcnst_itdfspost_make(
      ak_world_tree(wv->w),
      ak_world_ett_to_id(wv->w, root)),
    .wv = wv
  };
}

ak_ett
ak_wv_itdfspost_next(ak_wv_itdfspost* it)
{
  ak_fcnstid id =
    ak_fcnst_itdfspost_next(&it->it);
  return id ? *(ak_ett*)ak_fcnst_at(
                ak_world_tree(it->wv->w), id)
            : 0;
}

//===== ak_wv_itdfspre =====//
ak_wv_itdfspre
ak_wv_itdfspre_make_empty(ak_alct alct)
{
  return (ak_wv_itdfspre){
    .it = ak_fcnst_itdfspre_make_empty(alct),
    .wv = 0
  };
}
ak_wv_itdfspre
ak_wv_itdfspre_make(ak_wv* wv,
                    ak_ett root,
                    ak_alct alct)
{
  return (ak_wv_itdfspre){
    .it = ak_fcnst_itdfspre_make(
      ak_world_tree(wv->w),
      ak_world_ett_to_id(wv->w, root),
      alct),
    .wv = wv
  };
}

void
ak_wv_itdfspre_destroy(ak_wv_itdfspre* it)
{
  ak_fcnst_itdfspre_destroy(&it->it);
}

void
ak_wv_itdfspre_reset(ak_wv_itdfspre* it,
                     ak_wv* wv,
                     ak_ett root)
{
  it->wv = wv;
  ak_fcnst_itdfspre_reset(
    &it->it,
    ak_world_tree(it->wv->w),
    ak_world_ett_to_id(it->wv->w, root));
}

ak_ett
ak_wv_itdfspre_next(ak_wv_itdfspre* it)
{
  ak_fcnstid id =
    ak_fcnst_itdfspre_next(&it->it);
  return id ? *(ak_ett*)ak_fcnst_at(
                ak_world_tree(it->wv->w), id)
            : 0;
}

//===== ak_wv_itcomp =====//
//--- export ---//
ak_wv_itcomp
ak_wv_itcomp_make(ak_wv* wv, ak_comp_enum ce)
{
  ak_wv_itcomp it = { 0 };
  it.wv = wv;
  it.ce = ce;
  it.idx = 0;
  return it;
}

ak_ett
ak_wv_itcomp_next(ak_wv_itcomp* it,
                  void* o_comp)
{
  if (it->idx >= ak_world_comp_count(
                   it->wv->w, it->ce)) {
    return 0;
  }
  ak_ett e = 0;
  void* comp = ak_world_comp_at_idx(
    it->wv->w, it->ce, it->idx, &e);
  ak_p_cpy(
    o_comp, comp, ak_comp_sizes[it->ce]);
  it->idx++;
  return e;
}

//===== ak_wv_itbfs =====//
ak_wv_itbfs
ak_wv_itbfs_make(ak_wv* wv,
                 ak_ett root,
                 ak_alct alct)
{
  return (ak_wv_itbfs){
    .it = ak_fcnst_itbfs_make(
      ak_world_tree(wv->w),
      ak_world_ett_to_id(wv->w, root),
      alct),
    .wv = wv
  };
}

void
ak_wv_itbfs_destroy(ak_wv_itbfs* it)
{
  ak_fcnst_itbfs_destroy(&it->it);
}

void
ak_wv_itbfs_reset(ak_wv_itbfs* it,
                  ak_ett root)
{
  ak_fcnst_itbfs_reset(
    &it->it,
    ak_world_ett_to_id(it->wv->w, root));
}

ak_ett
ak_wv_itbfs_next(ak_wv_itbfs* it)
{
  ak_fcnstid id =
    ak_fcnst_itbfs_next(&it->it);
  return id ? *(ak_ett*)ak_fcnst_at(
                ak_world_tree(it->wv->w), id)
            : 0;
}

//===== ak_wv_itleaf =====//
ak_wv_itleaf
ak_wv_itleaf_make(ak_wv* wv, ak_ett root)
{
  return (ak_wv_itleaf){
    .it = ak_fcnst_itleaf_make(
      ak_world_tree(wv->w),
      ak_world_ett_to_id(wv->w, root)),
    .wv = wv
  };
}

ak_ett
ak_wv_itleaf_next(ak_wv_itleaf* it)
{
  ak_fcnstid id =
    ak_fcnst_itleaf_next(&it->it);
  return id ? *(ak_ett*)ak_fcnst_at(
                ak_world_tree(it->wv->w), id)
            : 0;
}
