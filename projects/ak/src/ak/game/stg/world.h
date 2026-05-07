#ifndef ak_game_stg_world_h
#define ak_game_stg_world_h

#include "ak/core/mem/allocator.h"
#include "ak/game/stg/comp.h"
#include "ak/game/stg/core.h"
#include "ak/game/stg/ett.h"
#include <stdbool.h>
typedef struct
{
  ak_ettstg es;
  ak_compstg* cs;
} ak_world;

ak_world
ak_world_make(ak_ett root, ak_alct alct);
void
ak_world_destroy(ak_world* w);

ak_ett
ak_world_ett_root(ak_world* w);

bool
ak_world_ett_exist(ak_world* w, ak_ett e);
void
ak_world_ett_new(ak_world* w,
                 ak_ett e,
                 ak_ett pt);
void
ak_world_ett_remove_cb(
  ak_world* w,
  ak_ett e,
  ak_ettstg_remove_fn remove_fn,
  void* remove_ctx);
ak_ett
ak_world_ett_parent(ak_world* w, ak_ett e);
ak_ett
ak_world_ett_firstchild(ak_world* w,
                        ak_ett e);
ak_ett
ak_world_ett_nextsib(ak_world* w, ak_ett e);
ak_ett
ak_world_ett_leftmost(ak_world* w, ak_ett e);

bool
ak_world_comp_exist(ak_world* w,
                    ak_ett e,
                    ak_comp_enum ce);
void*
ak_world_comp_at(ak_world* w,
                 ak_ett e,
                 ak_comp_enum ce);
void
ak_world_comp_overwrite(ak_world* w,
                        ak_ett e,
                        ak_comp_enum ce,
                        const void* comp);

void
ak_world_comp_add(ak_world* w,
                  ak_ett e,
                  ak_comp_enum ce,
                  const void* comp);
void
ak_world_comp_remove(ak_world* w,
                     ak_ett e,
                     ak_comp_enum ce);

#define X(name)                             \
  static ak_##name##_t*                     \
    ak_world_comp_at_##name(ak_world* w,    \
                            ak_ett ett)     \
  {                                         \
    return ak_world_comp_at(                \
      w, ett, ak_##name##_e);               \
  }                                         \
  static bool ak_world_comp_exist_##name(   \
    ak_world* w, ak_ett ett)                \
  {                                         \
    return ak_world_comp_exist(             \
      w, ett, ak_##name##_e);               \
  }                                         \
  static void ak_world_comp_add_##name(     \
    ak_world* w,                            \
    ak_ett ett,                             \
    const ak_##name##_t* comp)              \
  {                                         \
    ak_world_comp_add(                      \
      w, ett, ak_##name##_e, comp);         \
  }                                         \
  static void ak_world_comp_remove_##name(  \
    ak_world* w, ak_ett ett)                \
  {                                         \
    ak_world_comp_remove(                   \
      w, ett, ak_##name##_e);               \
  }

#include "./ak/game/comp.inc"
#undef X

#endif
