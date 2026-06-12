#include "ak/gfx/shader.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gresreg.h"
#include "ak_opengl/gfx/gfx_impl.h"

//===== ak_shader =====//
//--- private ---//

struct ak_shader
{
  ak_alct alct;
  ak_gfx* gfx;
  GLuint program;
};

//--- internal ---//
ak_shader*
ak_shader_from_shaderstr(ak_gfx* gfx,
                         ak_shaderstr* ss,
                         ak_alct alct)
{
  return ak_shader_make_from_src(
    gfx,
    ak_shaderstr_vert(ss),
    ak_shaderstr_frag(ss),
    alct);
}
ak_shader*
ak_shader_make_from_src(ak_gfx* gfx,
                        const char* vert_src,
                        const char* frag_src,
                        ak_alct alct)
{
  ak_shader* s =
    ak_alct_alloc(alct, sizeof(ak_shader));
  s->alct = alct;
  s->gfx = gfx;
  s->program =
    program_make(vert_src, frag_src);
  return s;
}

ak_shader*
ak_shader_make_from_path(
  ak_gfx* gfx,
  const char* vert_path,
  const char* frag_path,
  ak_alct alct);

void
ak_shader_destroy(ak_shader* s)
{
  glDeleteProgram(s->program);
}

void
ak_shader_begin(ak_shader* s,
                ak_gresreg* grr,
                const ak_mtrl_indata* id,
                const ak_mtrl_basedata* bd)
{
  ak_gfx_call_begin(s->gfx);
}

void
ak_shader_pushquad(ak_shader* s,
                   const ak_mtrl_quaddata* q,
                   const ak_mat3_f* mat3)
{
  ak_gfx_pushquad(s->gfx, q, mat3);
}

void
ak_shader_end(ak_shader* s)
{
  ak_gfx_call_end(s->gfx);
}
