#include "ak_opengl/gfx/gcore.h"

#include "ak_opengl/gfx/gcore.h"

#include "ak/coll/da.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/math/vec4f.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"

#include "ak_opengl/gfx/gfx_impl.h"

#include <GLES3/gl3.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
  float m[9];
  float uv[4];
  float col[4];
} quad;

//===== ak_gfx =====//
#define s_max_quad_count 512
#define s_max_ivbo_size                     \
  (s_max_quad_count * sizeof(quad))

//--- private ---//

static void
break_call_ifneeded(ak_gcore* g)
{
  uint32_t count = ak_da_count(&g->quads);
  if (count < s_max_quad_count) {
    return;
  }

  ak_opengl_gcore_call_end(g);
  ak_opengl_gcore_call_begin(g);
}

void
ak_opengl_gcore_call_begin(ak_gcore* g)
{
  ak_assert(!g->call_began);
  ak_glerr_check;
  g->call_began = true;

  ak_da_clear(&g->quads);
}

void
ak_opengl_gcore_pushquad(
  ak_gcore* g,
  const ak_gfx_quaddata* qd,
  const ak_mat3_f* mat3)
{
  ak_assert(g->call_began);
  ak_glerr_check;

  break_call_ifneeded(g);

  quad q = { 0 };
  for (uint32_t i = 0; i < 9; i++) {
    q.m[i] = mat3->v[i];
  }
  for (uint32_t i = 0; i < 4; i++) {
    q.col[i] = qd->col.v[i];
  }
  q.uv[0] = qd->uv_min.v[0];
  q.uv[1] = qd->uv_min.v[1];
  q.uv[2] = qd->uv_max.v[0];
  q.uv[3] = qd->uv_max.v[1];

  ak_da_pushback(&g->quads, &q);
}

void
ak_opengl_gcore_call_end(ak_gcore* g)
{
  ak_assert(g->call_began);
  uint32_t count = ak_da_count(&g->quads);

  glBindBuffer(GL_ARRAY_BUFFER, g->ivbo);
  glBufferSubData(GL_ARRAY_BUFFER,
                  0,
                  sizeof(quad) * count,
                  ak_da_ptr(&g->quads));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  ak_glerr_check;

  glBindVertexArray(g->vao);
  glDrawElementsInstanced(
    GL_TRIANGLES,
    6,
    GL_UNSIGNED_SHORT,
    0,
    ak_da_count(&g->quads));
  glBindVertexArray(0);
  ak_glerr_check;

  g->call_began = false;
}

//--- public ---//
ak_gcore
ak_opengl_gcore_make(uint32_t width,
                     uint32_t height,
                     ak_alct alct)
{
  ak_gcore g = { 0 };
  ak_gcore* r = &g;
  r->alct = alct;

  r->screen_w = width;
  r->screen_h = height;

  r->quads = ak_da_make(sizeof(quad), alct);
  ak_da_reserve(&r->quads, s_max_quad_count);

  r->call_began = false;

  ak_glerr_check;

  {
    float vbo_buf[8] = { -0.5f, 0.5f, 0.5f,
                         0.5f,  0.5f, -0.5f,
                         -0.5f, -0.5f };
    glGenBuffers(1, &r->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 8 * sizeof(float),
                 vbo_buf,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    ak_glerr_check;
  }
  {
    uint16_t ebo_buf[6] = {
      0, 3, 2, 2, 1, 0
    };
    glGenBuffers(1, &r->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 r->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 6 * sizeof(uint16_t),
                 ebo_buf,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    ak_glerr_check;
  }
  {
    glGenBuffers(1, &r->ivbo);
    glBindBuffer(GL_ARRAY_BUFFER, r->ivbo);
    glBufferData(GL_ARRAY_BUFFER,
                 s_max_ivbo_size,
                 0,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    ak_glerr_check;
  }
  {
    glGenVertexArrays(1, &r->vao);
    glBindVertexArray(r->vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 r->ebo);
    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);

    glVertexAttribPointer(0,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          2 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, r->ivbo);
    glVertexAttribPointer(
      1,
      3,
      GL_FLOAT,
      GL_FALSE,
      sizeof(quad),
      (void*)offsetof(quad, m[0]));
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glVertexAttribPointer(
      2,
      3,
      GL_FLOAT,
      GL_FALSE,
      sizeof(quad),
      (void*)offsetof(quad, m[3]));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glVertexAttribPointer(
      3,
      3,
      GL_FLOAT,
      GL_FALSE,
      sizeof(quad),
      (void*)offsetof(quad, m[6]));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    glVertexAttribPointer(
      4,
      4,
      GL_FLOAT,
      GL_FALSE,
      sizeof(quad),
      (void*)offsetof(quad, uv));
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);

    glVertexAttribPointer(
      5,
      4,
      GL_FLOAT,
      GL_FALSE,
      sizeof(quad),
      (void*)offsetof(quad, col));
    glEnableVertexAttribArray(5);
    glVertexAttribDivisor(5, 1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    ak_glerr_check;
  }

  // glPolygonMode(GL_FRONT_AND_BACK,
  // GL_LINE);

  return g;
}

void
ak_opengl_gcore_destroy(ak_gcore* r)
{
  glDeleteBuffers(1, &r->vbo);
  glDeleteBuffers(1, &r->ebo);

  glDeleteBuffers(1, &r->ivbo);

  ak_da_destroy(&r->quads);
}

void
ak_opengl_gcore_sync(ak_gcore* g)
{
  glFinish();
}

void
ak_opengl_gcore_resize(ak_gcore* g,
                       uint32_t w,
                       uint32_t h)
{
  ak_assert(!g->call_began);
  g->screen_w = w;
  g->screen_h = h;
  glViewport(0, 0, w, h);
}

static float s_color = 0;
void
ak_opengl_gcore_frame_begin(ak_gcore* g)
{
  ak_assert(!g->call_began);
  s_color += 0.01f;
  if (s_color >= 1.0f)
    s_color -= 1.0f;

  glClearColor(s_color, 0.1f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  ak_glerr_check;
}

void
ak_opengl_gcore_frame_end(ak_gcore* g)
{
}

void
ak_opengl_gcore_scissor_reset(
  ak_gcore* gcore)
{
  (void)gcore;
  glDisable(GL_SCISSOR_TEST);
}
void
ak_opengl_gcore_scissor_set(ak_gcore* gcore,
                            int32_t x,
                            int32_t y,
                            uint32_t w,
                            uint32_t h)
{
  (void)gcore;
  glEnable(GL_SCISSOR_TEST);
  glScissor(x,
            (GLint)(gcore->screen_h - y - h),
            (GLsizei)w,
            (GLsizei)h);
}
