#ifndef ak_gfx_mtrl_vcol_h
#define ak_gfx_mtrl_vcol_h

#include "ak/gfx/mtrl/vcol.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/vec4.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gfx.h"
#include "ak_opengl/gfx/gfx_itn.h"

#include <glad/glad.h>

//===== ak_mtrl_vcol =====//
//--- private ---//

static const char* vs_src =
  "#version 330 core\n"
  "layout (location = 0) in vec2 vi_pos;\n"
  "layout (location = 1) in vec4 vi_col;\n"
  "out vec4 vo_col;"
  "void main() {\n"
  "  vo_col = vi_col;"
  "  gl_Position = vec4(vi_pos, 0.0, 1.0);\n"
  "}\n";

static const char* fs_src =
  "#version 330 core\n"
  "in vec4 vo_col;"
  "out vec4 fo_col;\n"
  "void main() {\n"
  "  fo_col = vo_col;\n"
  "}\n";

struct ak_mtrl_vcol
{
  ak_alct alct;
  ak_gfx* g;
  GLuint vao;
  GLuint program;
};

//--- public ---//
ak_mtrl_vcol*
ak_mtrl_vcol_make(ak_gfx* g, ak_alct alct)
{
  ak_mtrl_vcol* m = ak_alct_alloc(
    alct, sizeof(ak_mtrl_vcol));
  m->alct = alct;
  m->g = g;

  m->program = program_make(fs_src, vs_src);

  GLuint vbo = ak_gfx_vbo(g);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glGenVertexArrays(1, &m->vao);
  glBindVertexArray(m->vao);

  glVertexAttribPointer(
    0, 2, GL_FLOAT, GL_FALSE, 24, (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    1, 4, GL_FLOAT, GL_FALSE, 24, (void*)8);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return m;
}

void
ak_mtrl_vcol_destroy(ak_mtrl_vcol* m)
{
  glDeleteVertexArrays(1, &m->vao);
  glDeleteProgram(m->program);
  ak_alct_free(m->alct, m);
}

void
ak_mtrl_vcol_call_begin(ak_mtrl_vcol* m)
{
  ak_gfx_call_begin(
    m->g, m->program, m->vao, 24);
}

void
ak_mtrl_vcol_call_end(ak_mtrl_vcol* m)
{
  ak_gfx_call_end(m->g);
}

void
ak_mtrl_vcol_pushvert(ak_mtrl_vcol* m,
                      ak_vec2 vert_pos,
                      ak_vec4 vert_col)
{
  ak_gfx_push_f(m->g,
                ak_fx32_to_f(vert_pos.x));
  ak_gfx_push_f(m->g,
                ak_fx32_to_f(vert_pos.y));
  ak_gfx_push_f(m->g,
                ak_fx32_to_f(vert_col.r));
  ak_gfx_push_f(m->g,
                ak_fx32_to_f(vert_col.g));
  ak_gfx_push_f(m->g,
                ak_fx32_to_f(vert_col.b));
  ak_gfx_push_f(m->g,
                ak_fx32_to_f(vert_col.a));
}

#endif
