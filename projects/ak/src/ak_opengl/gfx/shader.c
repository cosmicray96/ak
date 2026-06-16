#include "ak/gfx/shader.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gresreg.h"
#include "ak_opengl/gfx/gfx_impl.h"
#include "ak_opengl/gfx/tex_impl.h"

//===== ak_shader =====//
//--- private ---//

struct ak_shader
{
  ak_alct alct;
  ak_gfx* gfx;
  GLuint program;
  GLint vp_loc;
  GLint time_loc;
  GLint tex_loc;
};

//--- internal ---//
ak_shader*
ak_shader_from_shaderstr(
  ak_gfx* gfx,
  const ak_shaderstr* ss,
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

  s->vp_loc =
    glGetUniformLocation(s->program, "u_vp");
  s->time_loc =
    glGetUniformLocation(s->program, "u_t");

  s->tex_loc = glGetUniformLocation(
    s->program, "u_tex");

  ak_glerr_check;
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
  glUseProgram(s->program);

  if (s->vp_loc != -1) {
    glUniformMatrix3fv(
      s->vp_loc, 1, GL_FALSE, id->vp.v);
  }
  if (s->time_loc != -1) {
    glUniform1f(s->vp_loc, id->time);
  }

  if (s->tex_loc != -1) {
    ak_tex* t =
      ak_gresreg_get_tex(grr, bd->tex);
    GLuint id = ak_tex_get(t);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
    glUniform1i(s->tex_loc, 0);
  }
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
  glUseProgram(0);
}
