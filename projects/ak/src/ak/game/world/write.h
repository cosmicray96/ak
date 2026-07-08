#ifndef ak_game_world_write_h
#define ak_game_world_write_h

#include "ak/core/io.h"
#include "ak/game/core.h"
#include "ak/game/stg/world.h"
#include "ak/system/idgen.h"
#include "ak/system/stream.h"

ak_errcode
ak_stream_write_world(ak_stm stm,
                      ak_world* w,
                      ak_ett root,
                      ak_alct alct);

ak_errcode
ak_stream_read_world(ak_stm stm,
                     ak_world* o_w,
                     ak_alct alct);

void
ak_stream_print_world(ak_world* o_w);

#endif
