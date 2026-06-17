#include "ak/gfx/shader.h"
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

ak_errcode
ak_shader_from_shaderstr(
  ak_gfx* gfx,
  const ak_shaderstr* ss,
  ak_shader** o_shader,
  ak_alct alct)
{
  ak_shader* s =
    ak_alct_alloc(alct, sizeof(ak_shader));
  s->alct = alct;
  s->gfx = gfx;
  ak_errcode err = program_make_with_err(
    ak_str_ptr_const(&ss->vert),
    ak_str_ptr_const(&ss->frag),
    &s->program);

  if (err != ak_ok) {
    ak_alct_free(alct, s);
    ak_log("Opengl. Shader Comp Error, %s",
           ak_errcode_to_str(err));
    return err;
  }

  s->vp_loc =
    glGetUniformLocation(s->program, "u_vp");
  s->time_loc =
    glGetUniformLocation(s->program, "u_t");

  s->tex_loc = glGetUniformLocation(
    s->program, "u_tex");

  ak_glerr_check;
  *o_shader = s;
  return ak_ok;
}

void
ak_shader_destroy(ak_shader* s)
{
  glDeleteProgram(s->program);
}

void
ak_shader_begin(ak_shader* s,
                ak_gresreg* grr,
                const ak_gfx_batchdata* id,
                const ak_gfx_calldata* bd)
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
                   const ak_gfx_quaddata* q,
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
