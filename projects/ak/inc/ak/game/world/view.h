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
  ak_ex ak_##name##_t                       \
    ak_world_v_comp_##name(                 \
      ak_world_v* w,                        \
      ak_ett e,                             \
      ak_comp_enum ce);

#include "ak/game/comp.inc"
#undef X

#endif
