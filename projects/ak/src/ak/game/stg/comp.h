#ifndef ak_game_stg_comp_h
#define ak_game_stg_comp_h

#include "ak/coll/spa.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/comp.h"
#include "ak/game/stg/core.h"

typedef struct ak_compstg ak_compstg;

ak_compstg*
ak_compstg_make(ak_alct alct);
void
ak_compstg_destroy(ak_compstg* cs);

bool
ak_compstg_exist(ak_compstg* cs,
                 ak_ett e,
                 ak_comp_enum ce);
void*
ak_compstg_at(ak_compstg* cs,
              ak_ett e,
              ak_comp_enum ce);
void
ak_compstg_overwrite(ak_compstg* cs,
                     ak_ett e,
                     ak_comp_enum ce,
                     const void* comp);

void
ak_compstg_add(ak_compstg* cs,
               ak_ett e,
               ak_comp_enum ce,
               const void* comp);
void
ak_compstg_remove(ak_compstg* cs,
                  ak_ett e,
                  ak_comp_enum ce);

void
ak_compstg_remove_all(ak_compstg* cs,
                      ak_ett e);

#define X(name)                             \
  static ak_##name##_t*                     \
    ak_compstg_at_##name(ak_compstg* cs,    \
                         ak_ett ett)        \
  {                                         \
    return ak_compstg_at(                   \
      cs, ett, ak_##name##_e);              \
  }                                         \
  static bool ak_compstg_exist_##name(      \
    ak_compstg* cs, ak_ett ett)             \
  {                                         \
    return ak_compstg_exist(                \
      cs, ett, ak_##name##_e);              \
  }                                         \
  static void ak_compstg_add_##name(        \
    ak_compstg* cs,                         \
    ak_ett ett,                             \
    const ak_##name##_t* comp)              \
  {                                         \
    ak_compstg_add(                         \
      cs, ett, ak_##name##_e, comp);        \
  }                                         \
  static void ak_compstg_remove_##name(     \
    ak_compstg* cs, ak_ett ett)             \
  {                                         \
    ak_compstg_remove(                      \
      cs, ett, ak_##name##_e);              \
  }

#include "./ak/game/comp.inc"
#undef X

#endif
