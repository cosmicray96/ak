#ifndef ak_gfx_shader_h
#define ak_gfx_shader_h

#include "ak/core/mem/allocator.h"
#include "ak/core/shaderstr.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gresreg.h"

ak_shader*
ak_shader_from_shaderstr(
  ak_gfx* gfx,
  const ak_shaderstr* ss,
  ak_alct alct);

ak_shader*
ak_shader_make_from_src(ak_gfx* gfx,
                        const char* vert_src,
                        const char* frag_src,
                        ak_alct alct);

ak_shader*
ak_shader_make_from_path(
  ak_gfx* gfx,
  const char* vert_path,
  const char* frag_path,
  ak_alct alct);

void
ak_shader_destroy(ak_shader* s);

void
ak_shader_begin(ak_shader* s,
                ak_gresreg* grr,
                const ak_gfx_batchdata* id,
                const ak_gfx_calldata* bd);
void
ak_shader_pushquad(ak_shader* s,
                   const ak_gfx_quaddata* q,
                   const ak_mat3_f* mat3);
void
ak_shader_end(ak_shader* s);

#endif
