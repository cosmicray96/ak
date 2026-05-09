#include "ak/core/math/fixed.h"
#include "ak/core/math/tf2d.h"
#include "ak/core/math/vec2.h"
#include "ak/core/math/vec4.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"
#include "ak/gfx/mtrl_itn.h"
#include "ak/gfx/mtrl_t.h"
#include "ak_opengl/gfx/gfx_itn.h"

#include <glad/glad.h>

//===== ak_mtrl_col =====//
//--- private ---//

static const char* vs_src =
  "#version 330 core\n"
  "layout (location = 0) in vec2 vi_pos;\n"
  "layout (location = 1) in vec4 vi_col;\n"
  "out vec4 vo_col;\n"
  "void main() {\n"
  "  gl_Position = vec4(vi_pos, 0.0, 1.0);\n"
  "  vo_col = vi_col;\n"
  "}\n";

static const char* fs_src =
  "#version 330 core\n"
  "in vec4 vo_col;"
  "out vec4 fo_col;\n"
  "void main() {\n"
  "  fo_col = vo_col;\n"
  "}\n";

typedef struct
{
  float x;
  float y;
  float r;
  float g;
  float b;
  float a;
} vert;

struct ak_mtrl_col
{
  ak_alct alct;
  ak_gfx* g;
  GLuint vao;
  GLuint program;
  GLuint col_loc;
  bool call_begin;
};

//--- internal ---//
ak_mtrl_col*
ak_mtrl_col_make(ak_gfx* g, ak_alct alct)
{
  ak_mtrl_col* m =
    ak_alct_alloc(alct, sizeof(ak_mtrl_col));
  m->alct = alct;
  m->g = g;

  m->program = program_make(fs_src, vs_src);

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

  glVertexAttribPointer(1,
                        4,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(vert),
                        (void*)8);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  ak_gfx_buff_unbind(g);

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
ak_mtrl_col_call_begin(void* mtrl)
{
  ak_mtrl_col* m = mtrl;
  ak_assert(!m->call_begin);

  glUseProgram(m->program);
  glBindVertexArray(m->vao);

  ak_gfx_call_begin(m->g, sizeof(vert));

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
ak_mtrl_col_pushquad(void* mtrl,
                     ak_tf2d tf,
                     const void* comp)
{
  ak_mtrl_col* m = mtrl;
  const ak_mtrl_col_t* c = comp;
  ak_assert(m->call_begin);
  ak_vec2 vs[4];
  ak_tf2d_to_4corner(tf, vs);

  for (uint32_t i = 0; i < 4; i++) {
    vert v = { 0 };
    v.x = ak_fx32_to_f(vs[i].x);
    v.y = ak_fx32_to_f(vs[i].y);
    v.r = ak_fx32_to_f(c->col[i].r);
    v.g = ak_fx32_to_f(c->col[i].g);
    v.b = ak_fx32_to_f(c->col[i].b);
    v.a = ak_fx32_to_f(c->col[i].a);

    ak_gfx_vertpush(m->g, &v);
  }
}
