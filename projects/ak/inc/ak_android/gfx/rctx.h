#ifndef ak_android_gfx_rctx_h
#define ak_android_gfx_rctx_h

#include "ak/core/async/dispatcher.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/platform/plat_base.h"
#include "ak/res/core.h"

ak_rctx*
ak_android_rctx_startup(ak_plat_base* pb,
                        ak_resreg* rr,
                        ak_alct alct);

void
ak_android_rctx_shutdown(ak_rctx* r);

ak_dispatcher*
ak_android_rctx_dispatcher_pre(ak_rctx* r);

void
ak_android_rctx_wait_inited(ak_rctx* r);

#endif
