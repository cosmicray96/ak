#ifndef ak_gfx_gfx_itn_h
#define ak_gfx_gfx_itn_h

#include "ak/gfx/core.h"

#include <glad/glad.h>

GLuint
program_make(const char* fs_src,
             const char* vs_src);

void
ak_gfx_call_begin(ak_gfx* g,
                  uint32_t quadsize);
void
ak_gfx_call_end(ak_gfx* g);

void
ak_gfx_pushquad(ak_gfx* g, const void* q);

void
ak_gfx_buff_bind_vbo(ak_gfx* g);
void
ak_gfx_buff_unbind_vbo(ak_gfx* g);
void
ak_gfx_buff_bind_ivbo(ak_gfx* g);
void
ak_gfx_buff_unbind_ivbo(ak_gfx* g);
void
ak_gfx_buff_bind_ebo(ak_gfx* g);
void
ak_gfx_buff_unbind_ebo(ak_gfx* g);

#endif
