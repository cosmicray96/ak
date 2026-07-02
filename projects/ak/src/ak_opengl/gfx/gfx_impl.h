#ifndef ak_gfx_gfx_itn_h
#define ak_gfx_gfx_itn_h

#include "ak/core/errcode.h"

#if defined(__ANDROID__)
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

typedef struct
{
  float u0, v0, u1, v1;
} ak_opengl_quad_uv;

#define ak_glerr_check                      \
  do {                                      \
    break;                                  \
    GLenum err = glGetError();              \
    ak_log_assert(err == GL_NO_ERROR,       \
                  "Opengl error: %x",       \
                  err);                     \
  } while (0);

GLuint
program_make(const char* vs_src,
             const char* fs_src);

ak_errcode
program_make_with_err(const char* vs_src,
                      const char* fs_src,
                      GLuint* o_program);

#endif
