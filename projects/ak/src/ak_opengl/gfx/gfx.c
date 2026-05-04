#include "ak/gfx/gfx.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"

#include <glad/glad.h>

#include <stdint.h>
#include <stdlib.h>

//===== ak_gfx =====//
//--- private ---//

#define s_max_vbo_size 1024 * 16 // for tests

struct ak_gfx
{
  ak_alct alct;

  bool call_began;

  GLuint vbo;
  uint8_t* vbo_buf;
  uint32_t vbo_idx;

  uint32_t cur_vertsize;
  uint32_t cur_max_trigcount;

  uint32_t cur_vao;
  GLuint cur_program;
};

static uint32_t
get_vertcount(ak_gfx* g)
{
  ak_assert(g->call_began);
  return g->vbo_idx / g->cur_vertsize;
}

static bool
max_trigcount_reached(ak_gfx* g)
{
  ak_assert(g->call_began);
  // fix?

  float trigcount = (float)g->vbo_idx /
                    (3 * g->cur_vertsize);
  if (trigcount >= g->cur_max_trigcount) {
    return true;
  }
  return false;
}

//--- public ---//
ak_gfx*
ak_gfx_startup(ak_plat_ren* pr, ak_alct alct)
{
  ak_gfx* r =
    ak_alct_alloc(alct, sizeof(ak_gfx));
  r->alct = alct;

  r->call_began = false;

  r->vbo_buf =
    ak_alct_alloc(alct, s_max_vbo_size);
  glGenBuffers(1, &r->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
  glBufferData(GL_ARRAY_BUFFER,
               s_max_vbo_size,
               NULL,
               GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  r->vbo_idx = 0;

  r->cur_vertsize = 0;
  r->cur_max_trigcount = 0;

  r->cur_vao = 0;
  r->cur_program = 0;

  // glPolygonMode(GL_FRONT_AND_BACK,
  // GL_LINE);

  return r;
}

void
ak_gfx_shutdown(ak_gfx* r)
{
  glDeleteBuffers(1, &r->vbo);

  ak_alct_free(r->alct, r->vbo_buf);
  ak_alct_free(r->alct, r);
}

void
ak_gfx_resize(ak_gfx* g,
              uint32_t w,
              uint32_t h)
{
  ak_assert(!g->call_began);
  glViewport(0, 0, w, h);
}

void
ak_gfx_call_begin(ak_gfx* g,
                  GLuint program,
                  GLuint vao,
                  uint32_t vertsize)
{
  ak_assert(!g->call_began);
  g->call_began = true;

  g->vbo_idx = 0;

  g->cur_program = program;
  g->cur_vao = vao;
  g->cur_vertsize = vertsize;
  g->cur_max_trigcount =
    s_max_vbo_size / (vertsize * 3);

  glUseProgram(g->cur_program);
  glBindVertexArray(g->cur_vao);
}

// add push_vec2, vec3, etc
void
ak_gfx_push_f(ak_gfx* g, float f)
{
  ak_assert(g->call_began);

  if (max_trigcount_reached(g)) {
    GLuint program = g->cur_program;
    GLuint vao = g->cur_vao;
    uint32_t vertsize = g->cur_vertsize;

    ak_gfx_call_end(g);
    ak_gfx_call_begin(
      g, program, vao, vertsize);
  }

  *(float*)(g->vbo_buf + g->vbo_idx) = f;
  g->vbo_idx += sizeof(float);
}

void
ak_gfx_call_end(ak_gfx* g)
{
  ak_assert(g->call_began);

  glBindBuffer(GL_ARRAY_BUFFER, g->vbo);
  glBufferSubData(GL_ARRAY_BUFFER,
                  0,
                  g->vbo_idx,
                  g->vbo_buf);

  glDrawArrays(
    GL_TRIANGLES, 0, get_vertcount(g));

  glBindVertexArray(0);
  glUseProgram(0);

  g->vbo_idx = 0;
  g->cur_program = 0;
  g->cur_vao = 0;

  g->cur_vertsize = 0;
  g->cur_max_trigcount = 0;

  g->call_began = false;
}

void
ak_gfx_frame_begin(ak_gfx* g)
{
  ak_assert(!g->call_began);

  glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void
ak_gfx_frame_end(ak_gfx* g)
{
  // empty
}
