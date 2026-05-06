#ifndef ak_game_world_v_itn_h
#define ak_game_world_v_itn_h

#include "ak/game/stg/world.h"
#include "ak/game/world/view.h"

struct ak_world_v
{
  ak_world* w;
};

ak_world_v
ak_world_v_make(ak_world* w);
void
ak_world_v_destroy(ak_world_v* wv);

#endif
