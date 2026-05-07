#ifndef ak_game_world_view_h
#define ak_game_world_view_h

#include "ak/export.h"
#include "ak/game/comp.h"
#include "ak/game/stg/core.h"

#include <stdbool.h>

typedef struct ak_world_v ak_world_v;

ak_ex bool
ak_world_v_ett_exist(ak_world_v* wv,
                     ak_ett e);
ak_ex ak_ett
ak_world_v_ett_root(ak_world_v* w);
ak_ex ak_ett
ak_world_v_ett_parent(ak_world_v* w,
                      ak_ett e);
ak_ex bool
ak_world_v_comp_exist(ak_world_v* w,
                      ak_ett ett,
                      ak_comp_enum ce);

#define X(name)                             \
  ak_ex ak_##name##_t*                      \
    ak_world_v_comp_##name(ak_world_v* w,   \
                           ak_ett e);

#include "ak/game/comp.inc"
#undef X

//===== ak_world_v_itdfs =====//
typedef struct
{
  ak_world_v* wv;
  ak_ett root;
  ak_ett last;
  bool started;
} ak_world_v_itdfs;
ak_world_v_itdfs
ak_world_v_itdfs_make(ak_world_v* v,
                      ak_ett root);
bool
ak_world_v_itdfs_next(ak_world_v_itdfs* it,
                      ak_ett* o_e);

//===== ak_world_v_itchild =====//
typedef struct
{
  ak_world_v* wv;
  ak_ett pt;
  ak_ett child;
  bool started;
} ak_world_v_itchild;
ak_world_v_itchild
ak_world_v_itchild_make(ak_world_v* wv,
                        ak_ett pt);
bool
ak_world_v_itchild_next(
  ak_world_v_itchild* it,
  ak_ett* o_e);

#endif
