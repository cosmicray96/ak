#ifndef ak_game_stg_world_h
#define ak_game_stg_world_h

#include "ak/core/mem/allocator.h"
#include "ak/game/stg/comp.h"
#include "ak/game/stg/core.h"
#include "ak/game/stg/ett.h"
#include <stdbool.h>
typedef struct
{
  ak_ettstg es;
  ak_compstg cs;
} ak_world;

ak_world
ak_world_make(ak_alct alct);
void
ak_world_destroy(ak_world* w);

ak_ett
ak_world_ett_root(ak_world* w);

void
ak_world_ett_new(ak_world* w,
                 ak_ett e,
                 ak_ett pt);
void
ak_world_ett_remove(ak_world* w, ak_ett e);
bool
ak_world_ett_exist(ak_world* w, ak_ett e);

void
ak_world_comp_add(ak_world* w,
                  ak_ett e,
                  ak_comptype ct,
                  const void* comp);
void
ak_world_comp_remove(ak_world* w,
                     ak_ett e,
                     ak_comptype ct);
void*
ak_world_comp_at(ak_world* w,
                 ak_ett e,
                 ak_comptype ct);

typedef struct
{
  ak_world* w;
  ak_ettstg_itdfs ettit;
} ak_world_itdfs;
ak_world_itdfs
ak_world_itdfs_make(ak_world* w,
                    ak_ett root);
bool
ak_world_itdfs_next(ak_world_itdfs* it,
                    ak_ett* o_e);

#endif
