#ifndef ak_opengl_gcore_h
#define ak_opengl_gcore_h

#include "ak/coll/da.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak_opengl/gfx/gfx_impl.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct ak_gcore ak_gcore;
struct ak_gcore
{

  ak_alct alct;

  uint32_t screen_w;
  uint32_t screen_h;

  bool call_began;

  GLuint vbo;
  GLuint ebo;
  GLuint vao;
  GLuint ivbo;

  ak_da quads;
};

ak_gcore
ak_opengl_gcore_make(uint32_t width,
                     uint32_t height,
                     ak_alct alct);
void
ak_opengl_gcore_destroy(ak_gcore* g);

void
ak_opengl_gcore_resize(ak_gcore* g,
                       uint32_t w,
                       uint32_t h);

void
ak_opengl_gcore_frame_begin(ak_gcore* g);
void
ak_opengl_gcore_frame_end(ak_gcore* g);

void
ak_opengl_gcore_scissor_reset(ak_gcore* g);
void
ak_opengl_gcore_scissor_set(ak_gcore* g,
                            int32_t x,
                            int32_t y,
                            uint32_t w,
                            uint32_t h);

void
ak_opengl_gcore_call_begin(ak_gcore* g);
void
ak_opengl_gcore_pushquad(
  ak_gcore* g,
  const ak_gfx_quaddata* qd,
  const ak_mat3_f* mat3);
void
ak_opengl_gcore_call_end(ak_gcore* g);

#endif
