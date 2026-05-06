#ifndef ak_game_sys_ren_h
#define ak_game_sys_ren_h

#include "ak/core/mem/allocator.h"
#include "ak/game/world/view.h"
#include "ak/gfx/core.h"
typedef struct
{
  ak_gfx* gf;
  ak_world_v* wv;
} ak_sys_ren;

ak_sys_ren
ak_sys_ren_make(ak_gfx* gf,
                ak_world_v* wv,
                ak_alct alct);
void
ak_sys_ren_destroy(ak_sys_ren* r);

#endif
