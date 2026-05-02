#ifndef ak_platform_plat_ren_h
#define ak_platform_plat_ren_h

#include "ak/core/mem/allocator.h"
typedef struct ak_plat_ren ak_plat_ren;

ak_plat_ren*
ak_plat_ren_startup(ak_alct alct);
void
ak_plat_ren_shutdown(ak_plat_ren* pr);

void
ak_plat_ren_swapbuffer(ak_plat_ren* pr);

#endif
