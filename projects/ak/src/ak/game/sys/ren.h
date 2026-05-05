#ifndef ak_game_sys_ren_h
#define ak_game_sys_ren_h

#include "ak/core/mem/allocator.h"
#include "ak/game/stg/world.h"
typedef struct
{

} ak_gren;

ak_gren
ak_gren_make(ak_world* w, ak_alct alct);
void
ak_gren_destroy(ak_gren* gr);

void
ak_gren_update(ak_gren* gr);

#endif
