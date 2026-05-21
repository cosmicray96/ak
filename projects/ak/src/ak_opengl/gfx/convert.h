#ifndef ak_gfx_gfx_convert_h
#define ak_gfx_gfx_convert_h

#include "ak/core/math/fixed.h"
#include "ak/core/math/vec2.h"
#include "ak_opengl/gfx/gfx_impl.h"

static ak_opengl_quad_uv
ak_opengl_convert_uv(ak_vec2 min,
                     ak_vec2 max)
{
  ak_opengl_quad_uv uv = { 0 };
  uv.u0 = ak_fx_to_f(min.x);
  uv.u1 = ak_fx_to_f(max.x);
  uv.v0 = ak_fx_to_f(min.y);
  uv.v1 = ak_fx_to_f(max.y);

  return uv;
}

#endif
