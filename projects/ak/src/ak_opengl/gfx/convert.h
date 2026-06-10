#ifndef ak_gfx_gfx_convert_h
#define ak_gfx_gfx_convert_h

#include "ak/core/math/vec2f.h"
#include "ak_opengl/gfx/gfx_impl.h"

static ak_opengl_quad_uv
ak_opengl_convert_uv(ak_vec2f min,
                     ak_vec2f max)
{
  ak_opengl_quad_uv uv = { 0 };
  uv.u0 = min.x;
  uv.u1 = max.x;
  uv.v0 = min.y;
  uv.v1 = max.y;

  return uv;
}

#endif
