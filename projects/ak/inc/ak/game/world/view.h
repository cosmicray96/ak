#ifndef ak_game_world_view_h
#define ak_game_world_view_h

#include "ak/coll/dq.h"
#include "ak/coll/ds.h"
#include "ak/core/mem/allocator.h"
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
ak_ex bool
ak_wv_itcomp_next(ak_wv_itcomp* it,
                  ak_ett* o_e,
                  void* o_comp);

//===== ak_world_v_itdfs_pt =====//
typedef struct
{
  ak_wv* wv;
  ak_ett root;
  ak_ett last;
  bool started;
} ak_wv_itdfs_pt;
ak_wv_itdfs_pt
ak_wv_itdfs_pt_make(ak_wv* v, ak_ett root);
bool
ak_wv_itdfs_pt_next(ak_wv_itdfs_pt* it,
                    ak_ett* o_e);

//===== ak_world_v_itdfs_pt =====//
typedef struct
{
  ak_wv* wv;
  ak_ds s;
} ak_wv_itdfs_pre;
ak_wv_itdfs_pre
ak_wv_itdfs_pre_make(ak_wv* v,
                     ak_ett root,
                     ak_alct alct);
void
ak_wv_itdfs_pre_destroy(ak_wv_itdfs_pre* it);
void
ak_wv_itdfs_pre_reset(ak_wv_itdfs_pre* it,
                      ak_ett root);

bool
ak_wv_itdfs_pre_next(ak_wv_itdfs_pre* it,
                     ak_ett* o_e);

//===== ak_world_v_itchild =====//
typedef struct
{
  ak_wv* wv;
  ak_dq q;
  ak_ett pt;
  ak_ett child;
  bool started;
} ak_wv_itchild;
ak_wv_itchild
ak_wv_itchild_make(ak_wv* wv, ak_ett pt);
bool
ak_wv_itchild_next(ak_wv_itchild* it,
                   ak_ett* o_e);

//===== ak_wv_itbfs =====//
typedef struct
{
  ak_wv* wv;
  ak_dq q;
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

bool
ak_wv_itbfs_next(ak_wv_itbfs* it,
                 ak_ett* o_e);

//===== ak_wv_itbfs =====//
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

#endif
