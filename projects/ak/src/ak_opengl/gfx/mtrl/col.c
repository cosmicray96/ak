#include "ak/core/math/fixed.h"
#include "ak/core/math/vec4.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"
#include "ak/gfx/mtrl/col_itn.h"
#include "ak/gfx/mtrl/mtrl.h"
#include "ak_opengl/gfx/gfx_itn.h"

#include <glad/glad.h>

//===== ak_mtrl_col =====//
//--- private ---//

static const char* vs_src =
  "#version 330 core\n"
  "layout (location = 0) in vec2 vi_pos;\n"
  "void main() {\n"
  "  gl_Position = vec4(vi_pos, 0.0, 1.0);\n"
  "}\n";

static const char* fs_src =
  "#version 330 core\n"
  "uniform vec4 u_col;"
  "in vec4 vo_col;"
  "out vec4 fo_col;\n"
  "void main() {\n"
  "  fo_col = u_col;\n"
  "}\n";

typedef struct
{
  float x;
  float y;
} vert;

struct ak_mtrl_col
{
  ak_alct alct;
  ak_gfx* g;
  GLuint vao;
  GLuint program;
  GLuint col_loc;
  float col_r;
  float col_g;
  float col_b;
  float col_a;
  bool call_begin;
};

//--- public ---//
ak_mtrl_col*
ak_mtrl_col_make(ak_gfx* g, ak_alct alct)
{
  ak_mtrl_col* m =
    ak_alct_alloc(alct, sizeof(ak_mtrl_col));
  m->alct = alct;
  m->g = g;

  m->program = program_make(fs_src, vs_src);

  glUseProgram(m->program);
  m->col_loc = glGetUniformLocation(
    m->program, "u_col");
  glUseProgram(0);

  glGenVertexArrays(1, &m->vao);
  glBindVertexArray(m->vao);

  ak_gfx_buff_bind(g);

  glVertexAttribPointer(0,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(vert),
                        (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  ak_gfx_buff_unbind(g);

  m->call_begin = false;

  return m;
}

void
ak_mtrl_col_destroy(ak_mtrl_col* m)
{
  glDeleteVertexArrays(1, &m->vao);
  glDeleteProgram(m->program);
  ak_alct_free(m->alct, m);
}

void
ak_mtrl_col_col_set(ak_mtrl_col* m,
                    ak_vec4 col)
{
  ak_assert(!m->call_begin);
  m->col_r = ak_fx32_to_f(col.r);
  m->col_g = ak_fx32_to_f(col.g);
  m->col_b = ak_fx32_to_f(col.b);
  m->col_a = ak_fx32_to_f(col.a);
}

void
ak_mtrl_col_call_begin(ak_mtrl_col* m)
{
  ak_assert(!m->call_begin);

  glUseProgram(m->program);
  glBindVertexArray(m->vao);

  glUniform4f(m->col_loc,
              m->col_r,
              m->col_g,
              m->col_b,
              m->col_a);
  ak_gfx_call_begin(m->g, sizeof(vert));

  m->call_begin = true;
}

void
ak_mtrl_col_call_end(ak_mtrl_col* m)
{
  ak_assert(m->call_begin);
  ak_gfx_call_end(m->g);
  glUseProgram(0);
  glBindVertexArray(0);
  m->call_begin = false;
}

void
ak_mtrl_col_pushvert(ak_mtrl_col* m,
                     ak_vec2 vert_pos)
{
  ak_assert(m->call_begin);
  vert v = {
    .x = ak_fx32_to_f(vert_pos.x),
    .y = ak_fx32_to_f(vert_pos.y),
  };

  ak_gfx_vertpush(m->g, &v);
}
