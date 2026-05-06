#ifndef ak_app_layers_world_h
#define ak_app_layers_world_h

#include "ak/app/app.h"
#include "ak/core/mem/allocator.h"
typedef struct ak_lworld ak_lworld;

ak_lworld*
ak_lworld_make(ak_alct alct);
void
ak_lworld_destroy(ak_lworld* l);

ak_applayer
ak_lworld_to_applayer(ak_lworld* l);

#endif
