#ifndef ak_gfx_gfx_itn_h
#define ak_gfx_gfx_itn_h

#include "ak/gfx/core.h"

#include <glad/glad.h>

void
ak_gfx_vertsize_set(ak_gfx* g,
                    uint32_t size);
void
ak_gfx_bind_vao(ak_gfx* g, GLuint vao);
void
ak_gfx_push_f(ak_gfx* g, float f);

#endif
