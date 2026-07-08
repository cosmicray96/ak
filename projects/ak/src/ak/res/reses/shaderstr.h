#ifndef ak_core_shaderstr_h
#define ak_core_shaderstr_h

#include "ak/coll/str.h"
#include "ak/core/mem/allocator.h"
#include "ak/export.h"
#include "ak/system/stream.h"

typedef struct
{
  ak_str vert;
  ak_str frag;
} ak_shaderstr;

ak_ex ak_errcode
ak_stm_read_shaderstr(ak_stm stm,
                      ak_shaderstr* o_ss,
                      ak_alct alct);

ak_ex void
ak_shaderstr_destroy(ak_shaderstr* ss);

const char*
ak_shaderstr_vert(const ak_shaderstr* ss);
const char*
ak_shaderstr_frag(const ak_shaderstr* ss);

#endif
