#include "ak/gfx/gfx.h"
#include "ak/core/mem/allocator.h"
#include "ak/core/mem/ptr.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"

#include "ak_opengl/gfx/gfx_itn.h"

#include <glad/glad.h>

#include <stdint.h>

//===== ak_gfx =====//
#define s_max_ivbo_size 1024 * 16

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

  bool call_began;

  GLuint vbo;

  GLuint ebo;

  GLuint ivbo;
  uint8_t* ivbo_buf;

  uint32_t quad_count;
  uint32_t cur_quadsize;
};

static void
call_reset_ifneed(ak_gfx* g)
{
  ak_assert(g->call_began);

  uint32_t idx =
    g->quad_count * g->cur_quadsize;

  if (idx + g->cur_quadsize <=
      s_max_ivbo_size) {
    return;
  }

  uint32_t cur_quadsize = g->cur_quadsize;
  ak_gfx_call_end(g);
  ak_gfx_call_begin(g, cur_quadsize);
}

//--- internal ---//
void
ak_gfx_buff_bind_vbo(ak_gfx* g)
{
  glBindBuffer(GL_ARRAY_BUFFER, g->vbo);
}
void
ak_gfx_buff_unbind_vbo(ak_gfx* g)
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void
ak_gfx_buff_bind_ivbo(ak_gfx* g)
{
  glBindBuffer(GL_ARRAY_BUFFER, g->ivbo);
}
void
ak_gfx_buff_unbind_ivbo(ak_gfx* g)
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void
ak_gfx_buff_bind_ebo(ak_gfx* g)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
               g->ebo);
}
void
ak_gfx_buff_unbind_ebo(ak_gfx* g)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void
ak_gfx_call_begin(ak_gfx* g,
                  uint32_t quadsize)
{
  ak_assert(!g->call_began);
  g->call_began = true;

  g->quad_count = 0;
  g->cur_quadsize = quadsize;
}

void
ak_gfx_pushquad(ak_gfx* g, const void* q)
{
  ak_assert(g->call_began);

  check_err;
  call_reset_ifneed(g);
  uint32_t idx =
    g->quad_count * g->cur_quadsize;
  ak_p_cpy(
    &g->ivbo_buf[idx], q, g->cur_quadsize);
  g->quad_count++;
}

void
ak_gfx_call_end(ak_gfx* g)
{
  ak_assert(g->call_began);

  glBindBuffer(GL_ARRAY_BUFFER, g->ivbo);
  glBufferSubData(GL_ARRAY_BUFFER,
                  0,
                  g->quad_count *
                    g->cur_quadsize,
                  g->ivbo_buf);
  check_err;

  glDrawElementsInstanced(GL_TRIANGLES,
                          6,
                          GL_UNSIGNED_SHORT,
                          0,
                          g->quad_count);
  check_err;

  g->cur_quadsize = 0;
  g->quad_count = 0;
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
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    check_err;
  }
  {
    /*
uint16_t ebo_buf[6] = {
0, 1, 2, 2, 3, 0
};
            */
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
    r->ivbo_buf =
      ak_alct_alloc(alct, s_max_ivbo_size);
    glGenBuffers(1, &r->ivbo);
    glBindBuffer(GL_ARRAY_BUFFER, r->ivbo);
    glBufferData(GL_ARRAY_BUFFER,
                 s_max_ivbo_size,
                 0,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    check_err;
  }

  r->cur_quadsize = 0;
  r->quad_count = 0;

  // glPolygonMode(GL_FRONT_AND_BACK,
  // GL_LINE);

  return r;
}

void
ak_gfx_shutdown(ak_gfx* r)
{
  ak_alct_free(r->alct, r->ivbo_buf);

  glDeleteBuffers(1, &r->vbo);
  glDeleteBuffers(1, &r->ebo);

  glDeleteBuffers(1, &r->ivbo);

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
  check_err;
}

void
ak_gfx_frame_end(ak_gfx* g)
{
  // empty
}
