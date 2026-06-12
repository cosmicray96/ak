#include "ak/gfx/gfx.h"
#include "ak/coll/da.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/math/vec4f.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"

#include "ak/platform/plat.h"
#include "ak/platform/plat_base.h"
#include "ak_opengl/gfx/gfx_impl.h"

#include <glad/glad.h>

#include <stddef.h>
#include <stdint.h>

void
ak_plat_base_glmakecurrent(ak_plat_base* pb);

typedef struct
{
  float m[9];
  float uv[4];
  float col[4];
} quad;

//===== ak_gfx =====//
#define s_max_quad_count 512
#define s_max_ivbo_size                     \
  s_max_quad_count * sizeof(quad)

#define check_err                           \
  do {                                      \
    GLenum err = glGetError();              \
    if (err != GL_NO_ERROR) {               \
      ak_log("opengl error:");              \
      ak_logv(err, x);                      \
      ak_assert(false);                     \
    }                                       \
  } while (0);

//--- private ---//

struct ak_gfx
{
  ak_alct alct;
  ak_plat_base* pb;

  uint32_t screen_w;
  uint32_t screen_h;

  bool call_began;

  GLuint vbo;
  GLuint ebo;
  GLuint vao;
  GLuint ivbo;

  ak_da quads;
};

void
break_call_ifneeded(ak_gfx* g)
{
  uint32_t count = ak_da_count(&g->quads);
  if (count < s_max_quad_count) {
    return;
  }

  ak_gfx_call_end(g);
  ak_gfx_call_begin(g);
}

void
ak_gfx_call_begin(ak_gfx* g)
{
  ak_assert(!g->call_began);
  check_err;
  g->call_began = true;

  ak_da_clear(&g->quads);
}

void
ak_gfx_pushquad(ak_gfx* g,
                const ak_mtrl_quaddata* qd,
                const ak_mat3_f* mat3)
{
  ak_assert(g->call_began);
  check_err;

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
ak_gfx_call_end(ak_gfx* g)
{
  ak_assert(g->call_began);
  uint32_t count = ak_da_count(&g->quads);

  glBindVertexArray(g->vao);
  glBufferSubData(GL_ARRAY_BUFFER,
                  0,
                  sizeof(quad) * count,
                  ak_da_ptr(&g->quads));
  check_err;

  glDrawElementsInstanced(
    GL_TRIANGLES,
    6,
    GL_UNSIGNED_SHORT,
    0,
    ak_da_count(&g->quads));
  check_err;

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
  r->pb = pr;
  r->screen_w = ak_plat_base_init_width(pr);
  r->screen_h = ak_plat_base_init_height(pr);

  r->quads = ak_da_make(sizeof(quad), alct);
  ak_da_reserve(&r->quads, s_max_quad_count);

  ak_plat_base_glmakecurrent(pr);

  r->call_began = false;

  check_err;

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
    check_err;
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
    check_err;
  }
  {
    glGenBuffers(1, &r->ivbo);
    glBindBuffer(GL_ARRAY_BUFFER, r->ivbo);
    glBufferData(GL_ARRAY_BUFFER,
                 s_max_ivbo_size,
                 0,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    check_err;
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
    check_err;
  }

  // glPolygonMode(GL_FRONT_AND_BACK,
  // GL_LINE);

  return r;
}

void
ak_gfx_shutdown(ak_gfx* r)
{
  glDeleteBuffers(1, &r->vbo);
  glDeleteBuffers(1, &r->ebo);

  glDeleteBuffers(1, &r->ivbo);

  ak_da_destroy(&r->quads);

  ak_alct_free(r->alct, r);
}

void
ak_gfx_resize(ak_gfx* g,
              uint32_t w,
              uint32_t h)
{
  ak_assert(!g->call_began);
  glViewport(0, 0, w, h);
  g->screen_w = w;
  g->screen_h = h;
}

void
ak_gfx_frame_begin(ak_gfx* g)
{
  ak_assert(!g->call_began);

  glClearColor(1.0f, 0.1f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  check_err;
}

void
ak_gfx_frame_end(ak_gfx* g)
{
  ak_plat_base_swapbuffer(g->pb);
}

ak_mat3_f
ak_gfx_vp_make(const ak_mat3* cam,
               uint32_t w,
               uint32_t h,
               ak_fx pixelsize)
{
  ak_mat3 cimat3x3 = ak_mat3_inv_fast(cam);

  if (w != 800) {
    int a = 10;
  }

  float ps = ak_fx_to_f(pixelsize);
  float sx = (ps * 2.0f) / (float)w;
  float sy = -(ps * 2.0f) / (float)h;

  // convert inverse camera matrix to float
  ak_mat3_f cam_f = { 0 };
  for (int i = 0; i < 9; i++) {
    cam_f.v[i] = ak_fx_to_f(cimat3x3.v[i]);
  }

  ak_mat3_f proj = { 0 };
  proj.m[0][0] = sx;
  proj.m[1][1] = sy;
  proj.m[2][2] = 1.0f;

  ak_mat3_f result = { 0 };
  // C = A * B, column-major m[col][row]
  for (int col = 0; col < 3; col++)
    for (int row = 0; row < 3; row++)
      for (int k = 0; k < 3; k++)
        result.m[col][row] +=
          proj.m[k][row] * cam_f.m[col][k];

  return result;
}

/*
ak_mat3_f
ak_gfx_vp_ui_make(uint32_t w, uint32_t h)
{
  ak_mat3_f proj = { 0 };
  proj.m[0][0] = 2.0f / (float)w;
  proj.m[1][1] = -2.0f / (float)h;
  proj.m[0][2] = -1.0f;
  proj.m[1][2] = 1.0f;
  proj.m[2][2] = 1.0f;
  return proj;
}*/
ak_mat3_f
ak_gfx_vp_ui_make(uint32_t w, uint32_t h)
{
  ak_mat3_f proj = { 0 };

  proj.m[0][0] = 2.0f / (float)w;
  proj.m[1][1] = -2.0f / (float)h;
  proj.m[2][0] = -1.0f;
  proj.m[2][1] = 1.0f;
  proj.m[2][2] = 1.0f;

  return proj;
}

void
ak_gfx_scissor_reset(ak_gfx* gfx)
{
  (void)gfx;
  glDisable(GL_SCISSOR_TEST);
}
void
ak_gfx_scissor_set(ak_gfx* gfx,
                   int32_t x,
                   int32_t y,
                   uint32_t w,
                   uint32_t h)
{
  (void)gfx;
  glEnable(GL_SCISSOR_TEST);
  glScissor(x,
            (GLint)(gfx->screen_h - y - h),
            (GLsizei)w,
            (GLsizei)h);
}

const char*
ak_gfx_name()
{
  return "opengl";
}
