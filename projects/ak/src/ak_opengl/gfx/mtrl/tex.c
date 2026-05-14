#include "ak/core/math/fixed.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"
#include "ak/gfx/mtrl_itn.h"
#include "ak/gfx/mtrl_t.h"
#include "ak_opengl/gfx/gfx_itn.h"

#include <stddef.h>

#include <glad/glad.h>

//===== ak_mtrl_col =====//
//--- private ---//

static const char* vs_src =
  "#version 330 core\n"
  "layout (location = 0) in vec2 vi_cor;\n"
  "layout (location = 1) in vec3 vi_m1;\n"
  "layout (location = 2) in vec3 vi_m2;\n"
  "layout (location = 3) in vec3 vi_m3;\n"
  "layout (location = 4) in vec4 vi_uv;\n"
  "uniform mat3 u_vp;\n"
  "out vec4 vo_col;\n"
  "out vec2 vo_uv;\n"
  "void main() {\n"
  "mat3 m = mat3(vi_m1, vi_m2, vi_m3);\n"
  "vec3 pos = u_vp * m * vec3(vi_cor,1.0);\n"
  "gl_Position = vec4(pos.xy, 0.0, 1.0);\n"
  "vo_uv = vec2(\n"
  "mix(vi_uv.x, vi_uv.z, vi_cor.x + 0.5),\n"
  "mix(vi_uv.y, vi_uv.w, vi_cor.y + 0.5)\n"
  ");\n"
  "}\n";

//"  gl_Position = vec4(pos.xy, 0.0, 1.0);\n"

static const char* fs_src =
  "#version 330 core\n"
  "in vec4 vo_col;"
  "in vec2 vo_uv;"
  "out vec4 fo_col;\n"
  "void main() {\n"
  "  fo_col = vec4(vo_uv, 0.0, 1.0);\n"
  "}\n";

typedef struct
{
  float m3x3[9];
  float u0, v0, u1, v1;
} quad;

struct ak_mtrl_tex
{
  ak_alct alct;
  ak_gfx* g;
  GLuint vao;
  GLuint program;
  GLuint vp_loc;
  bool call_begin;
};

//--- internal ---//
ak_mtrl_tex*
ak_mtrl_tex_make(ak_gfx* g, ak_alct alct)
{
  ak_mtrl_tex* m =
    ak_alct_alloc(alct, sizeof(ak_mtrl_tex));
  m->alct = alct;
  m->g = g;

  m->program = program_make(fs_src, vs_src);
  glUseProgram(m->program);
  m->vp_loc =
    glGetUniformLocation(m->program, "u_vp");
  // ak_assert(m->vp_loc != -1);
  glUseProgram(0);

  glGenVertexArrays(1, &m->vao);
  glBindVertexArray(m->vao);

  ak_gfx_buff_bind_ebo(g);

  ak_gfx_buff_bind_vbo(g);
  glVertexAttribPointer(0,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        2 * sizeof(float),
                        (void*)0);
  glEnableVertexAttribArray(0);

  ak_gfx_buff_bind_ivbo(g);
  glVertexAttribPointer(
    1,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(quad),
    (void*)offsetof(quad, m3x3[0]));
  glEnableVertexAttribArray(1);
  glVertexAttribDivisor(1, 1);

  glVertexAttribPointer(
    2,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(quad),
    (void*)offsetof(quad, m3x3[3]));
  glEnableVertexAttribArray(2);
  glVertexAttribDivisor(2, 1);

  glVertexAttribPointer(
    3,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(quad),
    (void*)offsetof(quad, m3x3[6]));
  glEnableVertexAttribArray(3);
  glVertexAttribDivisor(3, 1);

  glVertexAttribPointer(
    4,
    4,
    GL_FLOAT,
    GL_FALSE,
    sizeof(quad),
    (void*)offsetof(quad, u0));
  glEnableVertexAttribArray(4);
  glVertexAttribDivisor(4, 1);

  glBindVertexArray(0);

  ak_gfx_buff_unbind_vbo(g);
  ak_gfx_buff_unbind_ivbo(g);
  ak_gfx_buff_unbind_ebo(g);

  m->call_begin = false;

  return m;
}

void
ak_mtrl_tex_destroy(void* mtrl)
{
  ak_mtrl_tex* m = mtrl;
  glDeleteVertexArrays(1, &m->vao);
  glDeleteProgram(m->program);
  ak_alct_free(m->alct, m);
}

//--- export ---//
void
ak_mtrl_tex_call_begin(void* mtrl,
                       const ak_mat3x3* vp)
{
  ak_mtrl_tex* m = mtrl;
  ak_assert(!m->call_begin);

  glUseProgram(m->program);
  float vp_f[9] = { 0 };
  ak_mat3x3_to_f(vp, vp_f);
  glUniformMatrix3fv(
    m->vp_loc, 1, GL_FALSE, vp_f);

  glBindVertexArray(m->vao);

  ak_gfx_call_begin(m->g, sizeof(quad));

  m->call_begin = true;
}

void
ak_mtrl_tex_call_end(void* mtrl)
{
  ak_mtrl_tex* m = mtrl;
  ak_assert(m->call_begin);
  ak_gfx_call_end(m->g);
  glUseProgram(0);
  glBindVertexArray(0);
  m->call_begin = false;
}

void
ak_mtrl_tex_pushquad(void* mtrl,
                     const ak_mat3x3* gmat3,
                     const void* comp)
{
  ak_mtrl_tex* m = mtrl;
  const ak_mtrl_tex_t* c = comp;
  ak_assert(m->call_begin);

  quad q = { .u0 = ak_fx_to_f(c->uv_min.x),
             .v0 = ak_fx_to_f(c->uv_min.y),
             .u1 = ak_fx_to_f(c->uv_max.x),
             .v1 = ak_fx_to_f(c->uv_max.y) };

  ak_mat3x3_to_f(gmat3, q.m3x3);

  ak_gfx_pushquad(m->g, &q);
}
