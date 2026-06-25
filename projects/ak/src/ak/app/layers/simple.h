#ifndef ak_app_layers_simple_h
#define ak_app_layers_simple_h

#include "ak/app/app.h"
#include "ak/app/layers/core.h"
#include "ak/core/mem/allocator.h"
typedef struct ak_lsimple ak_lsimple;

ak_lsimple*
ak_lsimple_make(ak_lcore* lcore,
                ak_alct alct);
void
ak_lsimple_destroy(ak_lsimple* l);

ak_applayer
ak_lsimple_to_applayer(ak_lsimple* l);

#endif
