#ifndef ak_game_stg_world_h
#define ak_game_stg_world_h

#include "ak/coll/fcnst.h"
#include "ak/coll/hmn.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/comp.h"
#include "ak/game/core.h"
#include "ak/game/stg/comp.h"
#include "ak/system/idgen.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
  ak_fcnst tree;
  ak_hmn map;
  ak_compstg* cs;
} ak_world;

ak_world
ak_world_make(ak_alct alct);
void
ak_world_destroy(ak_world* w);

void
ak_world_graft(ak_world* dest,
               ak_world* src,
               ak_ett dest_pt,
               ak_idgen* ig,
               ak_alct alct);

ak_fcnst*
ak_world_tree(ak_world* w);

ak_ett
ak_world_id_to_ett(ak_world* w,
                   ak_fcnstid id);
ak_ett
ak_world_ett_to_id(ak_world* w, ak_ett e);

uint32_t
ak_world_ett_count(ak_world* w);
uint32_t
ak_world_ett_count_subtree(ak_world* w,
                           ak_ett root);

ak_ett
ak_world_ett_root(ak_world* w);

uint32_t
ak_world_ett_compcount(ak_world* w,
                       ak_ett e);

uint32_t
ak_world_ett_depth(ak_world* w, ak_ett e);
uint32_t
ak_world_ett_order(ak_world* w, ak_ett e);
bool
ak_world_ett_exist(ak_world* w, ak_ett e);
void
ak_world_ett_new(ak_world* w,
                 ak_ett e,
                 ak_ett pt);
void
ak_world_ett_new_last(ak_world* w,
                      ak_ett e,
                      ak_ett pt);
void
ak_world_ett_remove_cb(
  ak_world* w,
  ak_ett e,
  ak_fcnst_remove_fn remove_fn,
  void* remove_ctx);
ak_ett
ak_world_ett_parent(ak_world* w, ak_ett e);
ak_ett
ak_world_ett_firstchild(ak_world* w,
                        ak_ett e);
ak_ett
ak_world_ett_nextsib(ak_world* w, ak_ett e);
ak_ett
ak_world_ett_leftmost(ak_world* w, ak_ett e);

bool
ak_world_comp_exist(ak_world* w,
                    ak_ett e,
                    ak_comp_enum ce);
void*
ak_world_comp_at(ak_world* w,
                 ak_ett e,
                 ak_comp_enum ce);
void
ak_world_comp_overwrite(ak_world* w,
                        ak_ett e,
                        ak_comp_enum ce,
                        const void* comp);

void
ak_world_comp_add_tu(ak_world* w,
                     ak_ett e,
                     const ak_comp_tu* ctu);

void
ak_world_comp_add(ak_world* w,
                  ak_ett e,
                  ak_comp_enum ce,
                  const void* comp);
void
ak_world_comp_remove(ak_world* w,
                     ak_ett e,
                     ak_comp_enum ce);

uint32_t
ak_world_comp_count(ak_world* w,
                    ak_comp_enum ce);
void*
ak_world_comp_at_idx(ak_world* w,
                     ak_comp_enum ce,
                     uint32_t idx,
                     ak_ett* o_e);

#endif
