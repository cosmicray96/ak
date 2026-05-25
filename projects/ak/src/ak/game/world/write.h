#ifndef ak_game_world_write_h
#define ak_game_world_write_h

#include "ak/core/io.h"
#include "ak/game/core.h"
#include "ak/game/stg/world.h"
#include "ak/system/idgen.h"
#include "ak/system/stream.h"

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

void
ak_write_to_cmd(ak_world* w,
                ak_wcb* wcb,
                ak_ett root,
                ak_alct alct);
ak_stmerr
ak_stream_write_world(ak_stm stm,
                      ak_world* w,
                      ak_ett root,
                      ak_alct alct);

ak_stmerr
ak_stream_read_world(ak_stm stm,
                     ak_world* o_w,
                     ak_idgen* ig,
                     ak_alct alct);

#endif
