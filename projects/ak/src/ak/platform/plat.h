#ifndef ak_platform_plat_h
#define ak_platform_plat_h

#include "ak/app/eq.h"
#include "ak/core/mem/allocator.h"
#include "ak/platform/core.h"
#include "ak/platform/plat_ren.h"
#include <stdint.h>

ak_plat*
ak_plat_startup(ak_plat_ren* pr,
                ak_alct alct);
void
ak_plat_shutdown(ak_plat* p);

int32_t
ak_plat_width(ak_plat* p);
int32_t
ak_plat_height(ak_plat* p);

void
ak_plat_eventflush(ak_plat* p,
                   ak_app_eq* eq);

#endif
