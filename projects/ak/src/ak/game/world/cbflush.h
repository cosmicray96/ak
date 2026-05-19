#ifndef ak_game_world_cbflush_h
#define ak_game_world_cbflush_h

#include "ak/game/stg/world.h"
#include "ak/game/world/cb.h"
#include "ak/system/idgen.h"

void
ak_world_cb_flush(ak_world* w,
                  ak_wcb* wcb,
                  ak_idgen* ig);

#endif
