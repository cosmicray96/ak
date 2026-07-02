#ifndef ak_platform_plat_base_h
#define ak_platform_plat_base_h

#include "ak/app/eq.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"

typedef struct ak_plat_base ak_plat_base;

ak_plat_base*
ak_plat_base_startup(ak_app_eq* eq,
                     ak_resreg* rr,
                     ak_alct alct);
void
ak_plat_base_shutdown(ak_plat_base* pb);

ak_rctx*
ak_plat_base_rctx(ak_plat_base* pb);

void
ak_plat_base_eventflush(ak_plat_base* pb);

uint32_t
ak_plat_base_width(ak_plat_base* pb);
uint32_t
ak_plat_base_height(ak_plat_base* pb);

#endif
