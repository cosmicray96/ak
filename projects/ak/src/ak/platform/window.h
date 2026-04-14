#ifndef ak_platform_window_h
#define ak_platform_window_h

#include "ak/core/mem/allocator.h"
#include "ak/platform/core.h"

ak_window*
ak_window_make(ak_platform* p, ak_alct alct);
void
ak_window_destroy(ak_window* w);

#endif
