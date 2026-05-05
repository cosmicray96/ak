#ifndef ak_game_stg_comp_h
#define ak_game_stg_comp_h

#include "ak/coll/hmn.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/stg/core.h"

typedef enum
{
  ak_comptype_none,
  ak_comptype_comp1,
  ak_comptype_comp2
} ak_comptype;

typedef struct
{
  ak_hmn comps;
} ak_compstg;

// use x comp here, make it a array instead
// of map

ak_compstg
ak_compstg_make(ak_alct alct);
void
ak_compstg_destroy(ak_compstg* cs);

void
ak_compstg_add(ak_compstg* cs,
               ak_ett ett,
               ak_comptype ct,
               const void* comp);
void
ak_compstg_remove(ak_compstg* cs,
                  ak_ett ett,
                  ak_comptype ct);
void*
ak_compstg_at(ak_compstg* cs,
              ak_ett ett,
              ak_comptype ct);

void*
ak_compstg_comp1(ak_compstg* cs, ak_ett ett);
void*
ak_compstg_comp2(ak_compstg* cs, ak_ett ett);

#endif
