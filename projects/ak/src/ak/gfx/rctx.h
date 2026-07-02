#ifndef ak_gfx_rctx_h
#define ak_gfx_rctx_h

#include "ak/gfx/gcb.h"
typedef struct ak_rctx ak_rctx;

void
ak_rctx_render(ak_rctx* rctx, ak_gcb* gcb);

ak_mat3_f
ak_rctx_vp_make(const ak_mat3* cam,
                uint32_t w,
                uint32_t h,
                ak_fx pixelsize);

ak_mat3_f
ak_rctx_vp_ui_make(uint32_t w, uint32_t h);

const char*
ak_rctx_name();

#endif
