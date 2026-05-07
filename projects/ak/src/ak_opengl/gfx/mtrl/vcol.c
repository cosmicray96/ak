#include "ak/core/math/vec4.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gfx.h"
#include "ak/gfx/mtrl/mtrl.h"
#include "ak/gfx/mtrl/vcol_itn.h"
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

typedef struct
{
  float x;
  float y;
  float r;
  float g;
  float b;
  float a;
} vert;

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
  ak_gfx_call_begin(m->g, sizeof(vert));
  glUseProgram(m->program);
  glBindVertexArray(m->vao);
}

void
ak_mtrl_vcol_call_end(ak_mtrl_vcol* m)
{
  ak_gfx_call_end(m->g);
  glUseProgram(0);
  glBindVertexArray(0);
}

void
ak_mtrl_vcol_pushvert(ak_mtrl_vcol* m,
                      ak_vec2 vert_pos,
                      ak_vec4 vert_col)
{
  vert v = {
    .x = ak_fx32_to_f(vert_pos.x),
    .y = ak_fx32_to_f(vert_pos.y),
    .r = ak_fx32_to_f(vert_col.r),
    .g = ak_fx32_to_f(vert_col.g),
    .b = ak_fx32_to_f(vert_col.b),
    .a = ak_fx32_to_f(vert_col.a),
  };

  ak_gfx_vertpush(m->g, &v);
}
