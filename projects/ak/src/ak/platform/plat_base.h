#ifndef ak_platform_plat_base_h
#define ak_platform_plat_base_h

#include "ak/core/mem/allocator.h"

typedef struct ak_plat_base ak_plat_base;

ak_plat_base*
ak_plat_base_startup(ak_alct alct);
void
ak_plat_base_shutdown(ak_plat_base* pb);

uint32_t
ak_plat_base_init_width(ak_plat_base* pb);
uint32_t
ak_plat_base_init_height(ak_plat_base* pb);

void
ak_plat_base_swapbuffer(ak_plat_base* pb);

#endif
