#ifndef ak_game_world_cb_h
#define ak_game_world_cb_h

#include "ak/export.h"
#include "ak/game/comp.h"
#include "ak/game/stg/core.h"

#include <stdbool.h>

typedef struct ak_world_cb ak_world_cb;

ak_ex ak_ett
ak_world_cb_ett_new(ak_world_cb* wcb,
                    ak_ett pt);
ak_ex void
ak_world_cb_ett_remove(ak_world_cb* wcb,
                       ak_ett e);

void
ak_world_cb_comp_modify(ak_world_cb* wcb,
                        ak_ett ett,
                        ak_comp_enum ce,
                        const void* comp);
void
ak_world_cb_comp_add(ak_world_cb* wcb,
                     ak_ett ett,
                     ak_comp_enum ce,
                     const void* comp);
void
ak_world_cb_comp_remove(ak_world_cb* wcb,
                        ak_ett ett,
                        ak_comp_enum ce);

#endif
