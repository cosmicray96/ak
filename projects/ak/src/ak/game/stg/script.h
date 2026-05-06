#ifndef ak_game_stg_sys_h
#define ak_game_stg_sys_h

#include "ak/app/event.h"
#include "ak/coll/da.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/world/cb.h"
#include "ak/game/world/view.h"

typedef struct
{
  void (*startup)(void* ctx,
                  ak_world_v* wv,
                  ak_world_cb* wcb);
  void (*shutdown)(void* ctx);

  void (*event)(void* ctx, ak_evt e);
  void (*update)(void* ctx, ak_dur delta);
} ak_world_sys;

typedef struct
{
  ak_da sys;
} ak_sysstg;

ak_sysstg
ak_sysstg_make(ak_alct alct);
void
ak_sysstg_destroy(ak_sysstg* ss);

void
ak_sysstg_push(ak_sysstg* ss,
               ak_world_sys sys);

//===== ak_sysstg_it =====//
typedef struct
{
  ak_sysstg* ss;
  uint32_t idx;
} ak_sysstg_it;
ak_sysstg_it
ak_sysstg_it_make(ak_sysstg* ss);
bool
ak_sysstg_it_next(ak_sysstg_it* it,
                  ak_world_sys* o_sys);

#endif
