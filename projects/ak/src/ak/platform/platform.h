#ifndef ak_platform_platform_h
#define ak_platform_platform_h

#include "ak/core/mem/allocator.h"
#include "ak/platform/core.h"

ak_platform*
ak_platform_startup(ak_alct alct);

void
ak_platform_shutdown(ak_platform* p);

#endif
