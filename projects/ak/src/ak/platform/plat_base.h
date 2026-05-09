#ifndef ak_platform_plat_base_h
#define ak_platform_plat_base_h

#include "ak/core/mem/allocator.h"
typedef struct ak_plat_ren ak_plat_base;

ak_plat_base*
ak_plat_base_startup(ak_alct alct);
void
ak_plat_base_shutdown(ak_plat_base* pr);

void
ak_plat_base_swapbuffer(ak_plat_base* pr);

#endif
