#ifndef ak_game_comp_new_h
#define ak_game_comp_new_h

#include "ak/core/io.h"
#include "ak/game/comp.h"

#define ak_d_comp_x(name)                   \
  void ak_write_##name(ak_iostream io,      \
                       ak_as_comp_t(name)   \
                         name);             \
  ak_as_comp_t(name)                        \
    ak_read_##name(ak_iostream io);

#include "ak/game/comp.inc"
#undef ak_d_comp_x

void
ak_write_comp_tu(ak_iostream io,
                 ak_comp_tu ctu);
ak_comp_tu
ak_read_comp_tu(ak_iostream io);

#endif
