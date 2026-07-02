#ifndef ak_opengl_x11_gfx_rctx_h
#define ak_opengl_x11_gfx_rctx_h

#include "ak/core/async/dispatcher.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/platform/plat_base.h"
#include "ak/res/core.h"

ak_rctx*
ak_opengl_x11_rctx_startup(ak_plat_base* pb,
                           ak_resreg* rr,
                           ak_alct alct);

void
ak_opengl_x11_rctx_shutdown(ak_rctx* r);

#endif
