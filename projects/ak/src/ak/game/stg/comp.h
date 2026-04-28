#ifndef ak_game_stg_comp_h
#define ak_game_stg_comp_h

#include "ak/coll/hmn.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/stg/core.h"

typedef struct
{
  ak_hmn comps;
} ak_compstg;

ak_compstg
ak_compstg_make(ak_alct alct);
void
ak_compstg_destroy(ak_compstg* cs);

void*
ak_compstg_comp1(ak_compstg* cs, ak_ett ett);
void*
ak_compstg_comp2(ak_compstg* cs, ak_ett ett);

#endif
