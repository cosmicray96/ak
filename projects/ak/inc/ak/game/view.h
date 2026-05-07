#ifndef ak_game_view_h
#define ak_game_view_h

#include "ak/game/world/cb.h"
#include "ak/game/world/view.h"
#include "ak/gfx/mtrl/view.h"
typedef struct
{
  ak_world_v* wv;
  ak_world_cb* wcb;
  ak_mtrlstg_v* mv;
} ak_game_v;

#endif
