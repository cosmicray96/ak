#ifndef ak_game_world_write_h
#define ak_game_world_write_h

#include "ak/core/io.h"
#include "ak/game/stg/world.h"
#include "ak/system/idgen.h"

void
ak_write_world(ak_iostream io,
               ak_world* w,
               ak_ett root,
               ak_alct alct);

void
ak_read_into_world(ak_iostream io,
                   ak_world* w,
                   ak_ett parent,
                   ak_idgen* eg,
                   ak_alct alct);

#endif
