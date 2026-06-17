#ifndef ak_core_math_stm_h
#define ak_core_math_stm_h

#include "ak/core/math/vec4f.h"
#include "ak/system/stream.h"

ak_stmerr
ak_stm_write_vec4f(ak_stm stm,
                   ak_vec4f vec4f);
ak_stmerr
ak_stm_read_vec4f(ak_stm stm,
                  ak_vec4f* o_vec4f);

#endif
