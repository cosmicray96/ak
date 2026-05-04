#ifndef ak_gfx_gfx_h
#define ak_gfx_gfx_h

#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include <stdint.h>

ak_gfx*
ak_gfx_startup(ak_alct alct);
void
ak_gfx_shutdown(ak_gfx* r);

void
ak_gfx_flush(ak_gfx* r);

void
ak_gfx_col_set(ak_gfx* r, float col);

void
ak_gfx_resize(ak_gfx* r,
              uint32_t w,
              uint32_t h);

#endif
