#ifndef ak_gfx_gfx_itn_h
#define ak_gfx_gfx_itn_h

#include "ak/gfx/core.h"

#include <glad/glad.h>

typedef struct
{
  float u0, v0, u1, v1;
} ak_opengl_quad_uv;

#define ak_glerr_check                      \
  do {                                      \
    GLenum err = glGetError();              \
    ak_log_assert(err == GL_NO_ERROR,       \
                  "Opengl error: %x",       \
                  err);                     \
  } while (0);

GLuint
program_make(const char* vs_src,
             const char* fs_src);

void
ak_gfx_call_begin(ak_gfx* g);

void
ak_gfx_call_end(ak_gfx* g);

void
ak_gfx_pushquad(ak_gfx* g,
                const ak_gfx_quaddata* q,
                const ak_mat3_f* mat3);

#endif
