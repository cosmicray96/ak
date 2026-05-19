#ifndef ak_game_world_cb_h
#define ak_game_world_cb_h

#include "ak/export.h"
#include "ak/game/comp.h"
#include "ak/game/core.h"

#include <stdbool.h>

ak_ex ak_ett
ak_wcb_ett_new(ak_wcb* wcb, ak_ett pt);
ak_ex void
ak_wcb_ett_remove(ak_wcb* wcb, ak_ett e);

#define ak_d_comp_x(name)                   \
  ak_ex void ak_wcb_comp_##name##_add(      \
    ak_wcb* wcb,                            \
    ak_ett e,                               \
    ak_as_comp_t(name) comp);               \
  ak_ex void ak_wcb_comp_##name##_remove(   \
    ak_wcb* wcb, ak_ett e);

#include "./ak/game/comp.inc"
#undef ak_d_comp_x

#endif
