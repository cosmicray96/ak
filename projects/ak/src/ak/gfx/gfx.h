#ifndef ak_gfx_gfx_h
#define ak_gfx_gfx_h

#include "ak/core/async/dispatcher.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/platform/plat_base.h"
#include <stdint.h>

ak_gfx*
ak_gfx_startup(ak_plat_base* pr,
               ak_dispatcher* d,
               ak_alct alct);
void
ak_gfx_shutdown(ak_gfx* r);

ak_dispatcher*
ak_gfx_dispatcher(ak_gfx* r);

void
ak_gfx_resize(ak_gfx* r,
              uint32_t w,
              uint32_t h);

void
ak_gfx_frame_begin(ak_gfx* g);
void
ak_gfx_frame_end(ak_gfx* g);

void
ak_gfx_scissor_reset(ak_gfx* gfx);
void
ak_gfx_scissor_set(ak_gfx* gfx,
                   int32_t x,
                   int32_t y,
                   uint32_t w,
                   uint32_t h);

ak_mat3_f
ak_gfx_vp_make(const ak_mat3* cam,
               uint32_t w,
               uint32_t h,
               ak_fx pixelsize);

ak_mat3_f
ak_gfx_vp_ui_make(uint32_t w, uint32_t h);

const char*
ak_gfx_name();

#endif
