#ifndef ak_app_layers_core_h
#define ak_app_layers_core_h

#include "ak/app/app.h"
#include "ak/core/mem/allocator.h"
#include "ak/platform/plat_base.h"
typedef struct ak_lcore ak_lcore;

ak_lcore*
ak_lcore_make(ak_alct alct);
void
ak_lcore_destroy(ak_lcore* l);

ak_plat_base*
ak_lcore_plat_base(ak_lcore* l);

ak_applayer
ak_lcore_to_applayer(ak_lcore* l);

#endif
