#ifndef ak_opengl_rctx_h
#define ak_opengl_rctx_h

#include "ak/core/async/dispatcher.h"
#include "ak/core/math/mat3x3.h"
#include "ak/gfx/core.h"

ak_dispatcher*
ak_opengl_rctx_dispatcher(ak_rctx* r);

void
ak_opengl_rctx_call_begin(ak_rctx* r);
void
ak_opengl_rctx_call_end(ak_rctx* r);
void
ak_opengl_rctx_pushquad(
  ak_rctx* r,
  const ak_gfx_quaddata* qd,
  const ak_mat3_f* gmat3);

#endif
