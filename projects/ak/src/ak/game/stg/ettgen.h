#ifndef ak_game_stf_ettgen_h
#define ak_game_stf_ettgen_h

#include "ak/coll/dq.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/core.h"

typedef struct
{
  ak_dq freelist;
  uint32_t counter;
} ak_ettgen;

ak_ettgen
ak_ettgen_make(ak_alct alct);
void
ak_ettgen_destroy(ak_ettgen* eg);

ak_ett
ak_ettgen_new(ak_ettgen* eg);
void
ak_ettgen_remove(ak_ettgen* eg, ak_ett e);
bool
ak_ettgen_exist(ak_ettgen* eg, ak_ett e);

#endif
