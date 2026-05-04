#ifndef ak_gfx_gfx_h
#define ak_gfx_gfx_h

#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/platform/plat_ren.h"
#include <stdint.h>

ak_gfx*
ak_gfx_startup(ak_plat_ren* pr,
               ak_alct alct);
void
ak_gfx_shutdown(ak_gfx* r);

void
ak_gfx_resize(ak_gfx* r,
              uint32_t w,
              uint32_t h);

void
ak_gfx_frame_begin(ak_gfx* g);
void
ak_gfx_frame_end(ak_gfx* g);

void
ak_gfx_call_end(ak_gfx* r);

void
ak_gfx_col_set(ak_gfx* r, float col);

#endif
