#ifndef ak_game_world_v_itn_h
#define ak_game_world_v_itn_h

#include "ak/game/stg/world.h"
#include "ak/game/world/view.h"

struct ak_wv
{
  ak_world* w;
};

ak_wv
ak_wv_make(ak_world* w);

#endif
