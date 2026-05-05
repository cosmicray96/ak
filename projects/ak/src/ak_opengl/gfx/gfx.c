#include "ak/gfx/gfx.h"
#include "ak/core/mem/allocator.h"
#include "ak/core/mem/ptr.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"

#include "ak_opengl/gfx/gfx_itn.h"

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
};

static uint32_t
get_vertcount(ak_gfx* g)
{
  ak_assert(g->call_began);
  return g->vbo_idx / g->cur_vertsize;
}

static uint32_t
get_max_trigcount(ak_gfx* g)
{
  ak_assert(g->call_began);
  return s_max_vbo_size /
         (g->cur_vertsize * 3);
}

static uint32_t
get_max_trigsize(ak_gfx* g)
{
  ak_assert(g->call_began);
  return get_max_trigcount(g) *
         g->cur_vertsize;
}

static void
call_reset_ifneed(ak_gfx* g)
{
  ak_assert(g->call_began);

  uint32_t max_trigsize =
    get_max_trigsize(g);

  if (g->vbo_idx < max_trigsize) {
    return;
  }
  if (g->vbo_idx > max_trigsize) {
    ak_assert(false);
  }

  uint32_t vertsize = g->cur_vertsize;
  ak_gfx_call_end(g);
  ak_gfx_call_begin(g, vertsize);
}

static bool
exact_triangles(ak_gfx* g)
{
  ak_assert(g->call_began);
  return g->vbo_idx %
           (g->cur_vertsize * 3) ==
         0;
}

//--- internal ---//
void
ak_gfx_buff_bind(ak_gfx* g)
{
  glBindBuffer(GL_ARRAY_BUFFER, g->vbo);
}

void
ak_gfx_buff_unbind(ak_gfx* g)
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
ak_gfx_call_begin(ak_gfx* g,
                  uint32_t vertsize)
{
  ak_assert(!g->call_began);
  g->call_began = true;

  g->vbo_idx = 0;

  g->cur_vertsize = vertsize;

  glBindBuffer(GL_ARRAY_BUFFER, g->vbo);
}

void
ak_gfx_vertpush(ak_gfx* g, const void* v)
{
  ak_assert(g->call_began);

  call_reset_ifneed(g);

  ak_p_cpy(g->vbo_buf + g->vbo_idx,
           v,
           g->cur_vertsize);
  g->vbo_idx += g->cur_vertsize;
}

void
ak_gfx_call_end(ak_gfx* g)
{
  ak_assert(g->call_began);
  ak_assert(exact_triangles(g));

  glBufferSubData(GL_ARRAY_BUFFER,
                  0,
                  g->vbo_idx,
                  g->vbo_buf);

  glDrawArrays(
    GL_TRIANGLES, 0, get_vertcount(g));

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);

  g->vbo_idx = 0;

  g->cur_vertsize = 0;

  g->call_began = false;
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
