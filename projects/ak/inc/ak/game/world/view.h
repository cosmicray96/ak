#ifndef ak_game_world_view_h
#define ak_game_world_view_h

#include "ak/coll/fcnst.h"
#include "ak/export.h"
#include "ak/game/comp.h"
#include "ak/game/core.h"

#include <stdbool.h>

ak_ex uint32_t
ak_wv_ett_depth(ak_wv* wv, ak_ett e);
ak_ex bool
ak_wv_ett_exist(ak_wv* wv, ak_ett e);
ak_ex ak_ett
ak_wv_ett_root(ak_wv* w);
ak_ex ak_ett
ak_wv_ett_parent(ak_wv* w, ak_ett e);
ak_ex bool
ak_wv_comp_exist(ak_wv* w,
                 ak_ett ett,
                 ak_comp_enum ce);
ak_ex ak_comp_tu
ak_wv_comp_tu(ak_wv* w,
              ak_ett ett,
              ak_comp_enum ce);

#define ak_d_comp_x(name)                   \
  ak_ex ak_as_comp_t(name)                  \
    ak_wv_comp_##name(ak_wv* w, ak_ett e);  \
  static bool ak_wv_comp_##name##_exist(    \
    ak_wv* w, ak_ett e)                     \
  {                                         \
    return ak_wv_comp_exist(                \
      w, e, ak_as_comp_e(name));            \
  }
#include "ak/game/comp.inc"
#undef ak_d_comp_x

//===== ak_wv_itchild =====//
typedef struct
{
  ak_fcnst_itchild it;
  ak_wv* wv;
} ak_wv_itchild;
ak_wv_itchild
ak_wv_itchild_make(ak_wv* wv, ak_ett pt);
ak_ett
ak_wv_itchild_next(ak_wv_itchild* it);

//===== ak_wv_itancestor =====//
typedef struct
{
  ak_fcnst_itancestor it;
  ak_wv* wv;
} ak_wv_itancestor;
ak_wv_itancestor
ak_wv_itancestor_make(ak_wv* wv, ak_ett e);
ak_ett
ak_wv_itancestor_next(ak_wv_itancestor* it);

//===== ak_wv_itdfspost =====//
typedef struct
{
  ak_fcnst_itdfspost it;
  ak_wv* wv;
} ak_wv_itdfspost;
ak_wv_itdfspost
ak_wv_itdfspost_make(ak_wv* wv, ak_ett root);
ak_ett
ak_wv_itdfspost_next(ak_wv_itdfspost* it);

//===== ak_wv_itdfspre =====//
typedef struct
{
  ak_fcnst_itdfspre it;
  ak_wv* wv;
} ak_wv_itdfspre;
ak_wv_itdfspre
ak_wv_itdfspre_make(ak_wv* wv,
                    ak_ett root,
                    ak_alct alct);
void
ak_wv_itdfspre_destroy(ak_wv_itdfspre* it);
void
ak_wv_itdfspre_reset(ak_wv_itdfspre* it,
                     ak_ett root);
ak_ett
ak_wv_itdfspre_next(ak_wv_itdfspre* it);

//===== ak_wv_itcomp =====//
typedef struct
{
  ak_wv* wv;
  ak_comp_enum ce;
  uint32_t idx;
} ak_wv_itcomp;
ak_ex ak_wv_itcomp
ak_wv_itcomp_make(ak_wv* wv,
                  ak_comp_enum ce);
ak_ex ak_ett
ak_wv_itcomp_next(ak_wv_itcomp* it,
                  void* o_comp);

//===== ak_wv_itettcomp =====//
typedef struct
{
  ak_wv* wv;
  ak_ett e;
  ak_comp_enum ce;
} ak_wv_itettcomp;

ak_wv_itettcomp
ak_wv_itettcomp_make(ak_wv* wv, ak_ett e);
bool
ak_wv_itettcomp_next(ak_wv_itettcomp* it,
                     ak_comp_tu* o_ctu);

//===== ak_wv_itbfs =====//
typedef struct
{
  ak_fcnst_itbfs it;
  ak_wv* wv;
} ak_wv_itbfs;
ak_wv_itbfs
ak_wv_itbfs_make(ak_wv* wv,
                 ak_ett root,
                 ak_alct alct);
void
ak_wv_itbfs_destroy(ak_wv_itbfs* it);
void
ak_wv_itbfs_reset(ak_wv_itbfs* it,
                  ak_ett root);
ak_ett
ak_wv_itbfs_next(ak_wv_itbfs* it);

//===== ak_wv_itleaf =====//
typedef struct
{
  ak_fcnst_itleaf it;
  ak_wv* wv;
} ak_wv_itleaf;

ak_wv_itleaf
ak_wv_itleaf_make(ak_wv* wv, ak_ett root);
ak_ett
ak_wv_itleaf_next(ak_wv_itleaf* it);

#endif
