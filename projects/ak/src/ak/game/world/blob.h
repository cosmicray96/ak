#ifndef ak_game_world_blob_h
#define ak_game_world_blob_h

#include "ak/core/mem/allocator.h"
#include "ak/game/stg/core.h"
#include "ak/game/stg/world.h"
#include <stdint.h>

typedef struct
{
  ak_ett e;
  ak_ett pt;
  uint32_t order;
  uint32_t comp_count;
  uint32_t comp_offset;
} ak_ett_blob;

typedef struct
{
  uint32_t ett_count;
  uint8_t data[]; // ett_blob followed by tu
} ak_world_blob;

ak_world_blob
ak_world_blob_make(ak_world* w,
                   ak_ett root,
                   ak_alct alct);
void
ak_world_blob_destroy(ak_world_blob* wb);

#endif
