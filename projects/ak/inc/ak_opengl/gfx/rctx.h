#ifndef ak_opengl_rctx_h
#define ak_opengl_rctx_h

#include "ak/core/mem/allocator.h"
#include "ak/platform/plat_base.h"

ak_rctx*
ak_opengl_rctx_startup_begin(
  ak_plat_base* pb,
  ak_alct alct);

ak_rctx*
ak_gl_rctx_stutdown();

#endif
