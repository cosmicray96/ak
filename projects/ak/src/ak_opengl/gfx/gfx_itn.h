#ifndef ak_gfx_gfx_itn_h
#define ak_gfx_gfx_itn_h

#include "ak/gfx/core.h"

#include <glad/glad.h>

GLuint
program_make(const char* fs_src,
             const char* vs_src);

void
ak_gfx_push_f(ak_gfx* g, float f);

#endif
