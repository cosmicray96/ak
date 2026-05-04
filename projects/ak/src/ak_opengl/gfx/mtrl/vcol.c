#ifndef ak_gfx_mtrl_vcol_h
#define ak_gfx_mtrl_vcol_h

#include "ak/gfx/mtrl/vcol.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/vec4.h"
#include "ak/core/mem/allocator.h"
#include "ak_opengl/gfx/gfx_itn.h"

#include <glad/glad.h>

//===== ak_mtrl_vcol =====//
//--- private ---//
struct ak_mtrl_vcol
{
  ak_alct alct;
  GLuint vao;
};

//--- public ---//
ak_mtrl_vcol*
ak_mtrl_vcol_make(ak_alct alct)
{
  ak_mtrl_vcol* m = ak_alct_alloc(
    alct, sizeof(ak_mtrl_vcol));
  m->alct = alct;

  return m;
}
void
ak_mtrl_vcol_destroy(ak_mtrl_vcol* m)
{
  ak_alct_free(m->alct, m);
}

void
ak_mtrl_vcol_pushvert(ak_mtrl_vcol* m,
                      ak_gfx* gf,
                      ak_vec2 vert_pos,
                      ak_vec4 vert_col)
{
  ak_gfx_push_f(gf,
                ak_fx32_to_f(vert_pos.x));
  ak_gfx_push_f(gf,
                ak_fx32_to_f(vert_pos.y));
  ak_gfx_push_f(gf,
                ak_fx32_to_f(vert_col.r));
  ak_gfx_push_f(gf,
                ak_fx32_to_f(vert_col.g));
  ak_gfx_push_f(gf,
                ak_fx32_to_f(vert_col.b));
  ak_gfx_push_f(gf,
                ak_fx32_to_f(vert_col.a));
}

#endif
