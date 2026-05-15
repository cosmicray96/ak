#include "ak/core/math/fixed.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/math/vec3.h"
#include "ak/core/math/vec4.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gresman.h"
#include "ak/gfx/mtrl.h"
#include "ak/gfx/mtrl_itn.h"
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
  "layout (location = 4) in vec4 vi_col;\n"
  "uniform mat3 u_vp;\n"
  "out vec4 vo_col;\n"
  "void main() {\n"
  "mat3 m = mat3(vi_m1, vi_m2, vi_m3);\n"
  "vec3 pos = u_vp * m * vec3(vi_cor,1.0);\n"
  "gl_Position = vec4(pos.xy, 0.0, 1.0);\n"
  "vo_col = vi_col;\n"
  "}\n";

//"  gl_Position = vec4(pos.xy, 0.0, 1.0);\n"

static const char* fs_src =
  "#version 330 core\n"
  "in vec4 vo_col;"
  "out vec4 fo_col;\n"
  "void main() {\n"
  "  fo_col = vo_col;\n"
  "}\n";

typedef struct
{
  float m3x3[9];
  float r, g, b, a;
} quad;

typedef struct ak_mtrl_col ak_mtrl_col;
struct ak_mtrl_col
{
  ak_alct alct;
  ak_gfx* g;
  GLuint vao;
  GLuint program;
  GLuint vp_loc;
  bool call_begin;
};

//--- internal ---//
ak_mtrl_col*
ak_mtrl_col_make(ak_gfx* g,
                 ak_gresman* grm,
                 ak_alct alct)
{
  ak_mtrl_col* m =
    ak_alct_alloc(alct, sizeof(ak_mtrl_col));
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
    (void*)offsetof(quad, r));
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
ak_mtrl_col_destroy(void* mtrl)
{
  ak_mtrl_col* m = mtrl;
  glDeleteVertexArrays(1, &m->vao);
  glDeleteProgram(m->program);
  ak_alct_free(m->alct, m);
}

//--- export ---//
void
ak_mtrl_col_call_begin(
  void* mtrl,
  const ak_mtrl_basedata* bd,
  const ak_mat3* vp,
  ak_fx time)
{
  ak_mtrl_col* m = mtrl;
  ak_assert(!m->call_begin);

  glUseProgram(m->program);
  float vp_f[9] = { 0 };
  ak_mat3_to_f(vp, vp_f);
  glUniformMatrix3fv(
    m->vp_loc, 1, GL_FALSE, vp_f);

  glBindVertexArray(m->vao);

  ak_gfx_call_begin(m->g, sizeof(quad));

  m->call_begin = true;
}

void
ak_mtrl_col_call_end(void* mtrl)
{
  ak_mtrl_col* m = mtrl;
  ak_assert(m->call_begin);
  ak_gfx_call_end(m->g);
  glUseProgram(0);
  glBindVertexArray(0);
  m->call_begin = false;
}

void
ak_mtrl_col_pushquad(
  void* mtrl,
  const ak_mtrl_quaddata* qd,
  const ak_mat3* gmat3)
{
  ak_mtrl_col* m = mtrl;
  ak_assert(m->call_begin);

  quad q = { .r = ak_fx_to_f(qd->col.r),
             .g = ak_fx_to_f(qd->col.g),
             .b = ak_fx_to_f(qd->col.b),
             .a = ak_fx_to_f(qd->col.a) };

  ak_mat3_to_f(gmat3, q.m3x3);

  ak_gfx_pushquad(m->g, &q);
}
