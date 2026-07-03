#include "ak/gfx/rctx.h"
#include "ak/core/async/dispatcher.h"
#include "ak_opengl/gfx/gcore.h"

//--- internal ---//
ak_mat3_f
ak_rctx_vp_make(const ak_mat3* cam,
                uint32_t w,
                uint32_t h,
                ak_fx pixelsize)
{
  ak_mat3 cimat3x3 = ak_mat3_inv_fast(cam);

  if (w != 800) {
    int a = 10;
  }

  float ps = ak_fx_to_f(pixelsize);
  float sx = (ps * 2.0f) / (float)w;
  float sy = -(ps * 2.0f) / (float)h;

  // convert inverse camera matrix to float
  ak_mat3_f cam_f = { 0 };
  for (int i = 0; i < 9; i++) {
    cam_f.v[i] = ak_fx_to_f(cimat3x3.v[i]);
  }

  ak_mat3_f proj = { 0 };
  proj.m[0][0] = sx;
  proj.m[1][1] = sy;
  proj.m[2][2] = 1.0f;

  ak_mat3_f result = { 0 };
  // C = A * B, column-major m[col][row]
  for (int col = 0; col < 3; col++)
    for (int row = 0; row < 3; row++)
      for (int k = 0; k < 3; k++)
        result.m[col][row] +=
          proj.m[k][row] * cam_f.m[col][k];

  return result;
}
ak_mat3_f
ak_rctx_vp_ui_make(uint32_t w, uint32_t h)
{
  ak_mat3_f proj = { 0 };

  proj.m[0][0] = 2.0f / (float)w;
  proj.m[1][1] = -2.0f / (float)h;
  proj.m[2][0] = -1.0f;
  proj.m[2][1] = 1.0f;
  proj.m[2][2] = 1.0f;

  return proj;
}

const char*
ak_rctx_name()
{
  return "opengl";
}
