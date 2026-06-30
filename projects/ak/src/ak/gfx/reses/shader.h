#ifndef ak_gfx_shader_h
#define ak_gfx_shader_h

#include "ak/core/async/dispatcher.h"
#include "ak/core/errcode.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/res/reses/shaderstr.h"

ak_errcode
ak_shader_from_shaderstr(
  ak_gfx* gfx,
  const ak_shaderstr* ss,
  ak_shader** o_shader,
  ak_alct alct);

void
ak_shader_destroy(ak_shader* s);

ak_errcode
ak_shader_from_shaderstr_dispatch(
  ak_dispatcher* d,
  ak_gfx* gfx,
  const ak_shaderstr* ss,
  ak_shader** o_shader,
  ak_alct alct);

void
ak_shader_destroy_dispatch(ak_dispatcher* d,
                           ak_shader* s);

void
ak_shader_begin(ak_shader* s,
                ak_resreg* rr,
                const ak_gfx_batchdata* id,
                const ak_gfx_calldata* bd);
void
ak_shader_pushquad(ak_shader* s,
                   const ak_gfx_quaddata* q,
                   const ak_mat3_f* mat3);
void
ak_shader_end(ak_shader* s);

#endif
