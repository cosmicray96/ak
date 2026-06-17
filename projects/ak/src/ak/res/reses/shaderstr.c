#include "ak/res/reses/shaderstr.h"

void
ak_shaderstr_destroy(ak_shaderstr* ss)
{
  ak_str_destroy(&ss->vert);
  ak_str_destroy(&ss->frag);
}

const char*
ak_shaderstr_vert(const ak_shaderstr* ss)
{
  return ak_str_ptr_const(&ss->vert);
}
const char*
ak_shaderstr_frag(const ak_shaderstr* ss)
{
  return ak_str_ptr_const(&ss->frag);
}
