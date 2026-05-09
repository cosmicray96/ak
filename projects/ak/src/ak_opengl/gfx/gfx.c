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

// worst case: 1 byte per vert & rounding
#define s_max_quad_count                    \
  ((s_max_vbo_size + 3) / 4)

// 6 idx per quad
#define s_max_ebo_count                     \
  (s_max_quad_count * 6)
#define s_max_ebo_size                      \
  (s_max_ebo_count * sizeof(uint16_t))

struct ak_gfx
{
  ak_alct alct;

  bool call_began;

  GLuint vbo;
  uint8_t* vbo_buf;
  uint32_t vbo_idx;

  GLuint ebo;
  uint16_t* ebo_buf;

  uint32_t cur_vertsize;
};

static uint32_t
get_vertcount(ak_gfx* g)
{
  ak_assert(g->call_began);
  return g->vbo_idx / g->cur_vertsize;
}
static uint32_t
get_quadcount(ak_gfx* g)
{
  ak_assert(g->call_began);
  return g->vbo_idx / (4 * g->cur_vertsize);
}

static void
call_reset_ifneed(ak_gfx* g)
{
  ak_assert(g->call_began);

  uint32_t bytes_for_one_quad =
    g->cur_vertsize * 4;
  if (g->vbo_idx + bytes_for_one_quad <=
      s_max_vbo_size) {
    return;
  }

  uint32_t vertsize = g->cur_vertsize;
  ak_gfx_call_end(g);
  ak_gfx_call_begin(g, vertsize);
}

static bool
exact_quads(ak_gfx* g)
{
  ak_assert(g->call_began);
  return g->vbo_idx %
           (g->cur_vertsize * 4) ==
         0;
}

//--- internal ---//
void
ak_gfx_buff_bind(ak_gfx* g)
{
  glBindBuffer(GL_ARRAY_BUFFER, g->vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
               g->ebo);
}

void
ak_gfx_buff_unbind(ak_gfx* g)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

  // glBindBuffer(GL_ARRAY_BUFFER, g->vbo);
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
  ak_assert(exact_quads(g));

  glBindBuffer(GL_ARRAY_BUFFER, g->vbo);
  glBufferSubData(GL_ARRAY_BUFFER,
                  0,
                  g->vbo_idx,
                  g->vbo_buf);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glDrawElements(GL_TRIANGLES,
                 get_quadcount(g) * 6,
                 GL_UNSIGNED_SHORT,
                 0);

  g->vbo_idx = 0;

  g->cur_vertsize = 0;

  g->call_began = false;
}

//--- public ---//
ak_gfx*
ak_gfx_startup(ak_plat_base* pr,
               ak_alct alct)
{
  ak_gfx* r =
    ak_alct_alloc(alct, sizeof(ak_gfx));
  r->alct = alct;

  r->call_began = false;

  {
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
  }
  {
    r->ebo_buf =
      ak_alct_alloc(alct, s_max_ebo_size);
    for (uint32_t q = 0;
         q < s_max_quad_count;
         q++) {
      uint32_t v = q * 4; // vertex base
      uint32_t e = q * 6; // ebo base
      r->ebo_buf[e + 0] = v + 0;
      r->ebo_buf[e + 1] = v + 1;
      r->ebo_buf[e + 2] = v + 2;
      r->ebo_buf[e + 3] = v + 2;
      r->ebo_buf[e + 4] = v + 3;
      r->ebo_buf[e + 5] = v + 0;
    }
    glGenBuffers(1, &r->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 r->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 s_max_ebo_size,
                 r->ebo_buf,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

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
