#ifndef ak_game_stg_comp_h
#define ak_game_stg_comp_h

#include "ak/coll/spa.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/comp.h"
#include "ak/game/stg/core.h"
#include <stdint.h>

typedef struct ak_compstg ak_compstg;

ak_compstg*
ak_compstg_make(ak_alct alct);
void
ak_compstg_destroy(ak_compstg* cs);

uint32_t
ak_compstg_compcount(ak_compstg* cs,
                     ak_ett e);

bool
ak_compstg_exist(ak_compstg* cs,
                 ak_ett e,
                 ak_comp_enum ce);
void*
ak_compstg_at(ak_compstg* cs,
              ak_ett e,
              ak_comp_enum ce);
void
ak_compstg_overwrite(ak_compstg* cs,
                     ak_ett e,
                     ak_comp_enum ce,
                     const void* comp);

void
ak_compstg_add(ak_compstg* cs,
               ak_ett e,
               ak_comp_enum ce,
               const void* comp);
void
ak_compstg_remove(ak_compstg* cs,
                  ak_ett e,
                  ak_comp_enum ce);

void
ak_compstg_remove_all(ak_compstg* cs,
                      ak_ett e);

uint32_t
ak_compstg_count(ak_compstg* cs,
                 ak_comp_enum ce);

void*
ak_compstg_at_idx(ak_compstg* cs,
                  ak_comp_enum ce,
                  uint32_t idx,
                  ak_ett* o_e);

#endif
