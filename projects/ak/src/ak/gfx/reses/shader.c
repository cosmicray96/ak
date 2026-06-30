#include "ak/gfx/reses/shader.h"

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
