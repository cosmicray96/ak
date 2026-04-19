#ifndef ak_platform_plat_h
#define ak_platform_plat_h

#include "ak/app/eq.h"
#include "ak/core/mem/allocator.h"
#include "ak/platform/core.h"

ak_plat*
ak_plat_startup(ak_alct alct);
void
ak_plat_shutdown(ak_plat* p);

void
ak_plat_eventflush(ak_plat* p,
                   ak_app_eq* eq);

ak_window
ak_plat_win_make(ak_plat* p);
void
ak_plat_win_destroy(ak_plat* p, ak_window w);

#endif
