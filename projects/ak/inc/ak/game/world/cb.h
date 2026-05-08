#ifndef ak_game_world_cb_h
#define ak_game_world_cb_h

#include "ak/export.h"
#include "ak/game/comp.h"
#include "ak/game/stg/core.h"

#include <stdbool.h>

typedef struct ak_wcb ak_wcb;

ak_ex ak_ett
ak_wcb_ett_new(ak_wcb* wcb, ak_ett pt);
ak_ex void
ak_wcb_ett_remove(ak_wcb* wcb, ak_ett e);

#define X(name)                             \
  ak_ex void ak_wcb_comp_##name##_add(      \
    ak_wcb* wcb,                            \
    ak_ett e,                               \
    ak_##name##_t comp);                    \
  ak_ex void ak_wcb_comp_##name##_remove(   \
    ak_wcb* wcb, ak_ett e);

#include "./ak/game/comp.inc"
#undef X

#endif
