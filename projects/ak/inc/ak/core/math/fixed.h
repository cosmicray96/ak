#ifndef ak_core_math_fixed_h
#define ak_core_math_fixed_h

#include "ak/core/io.h"
#include "ak/export.h"
#include <stdint.h>

typedef int32_t ak_fx32;
#define ak_s_fx32_fbits 12
#define ak_s_fx32_fm                        \
  ((int64_t)1 << ak_s_fx32_fbits)
#define ak_s_fx32_fmask ak_s_fx32_fm - 1

static ak_fx32
ak_fx32_f(float f)
{
  ak_fx32 fx = f * ak_s_fx32_fm;
  return fx;
}
static ak_fx32
ak_fx32_i(int32_t n)
{
  return (ak_fx32)((int64_t)n
                   << ak_s_fx32_fbits);
}

static float
ak_fx32_to_f(ak_fx32 fx)
{
  return (float)fx / (float)ak_s_fx32_fm;
}

static ak_fx32
ak_fx32_add(ak_fx32 fx1, ak_fx32 fx2)
{
  return fx1 + fx2;
}
static ak_fx32
ak_fx32_sub(ak_fx32 fx1, ak_fx32 fx2)
{
  return fx1 - fx2;
}
static ak_fx32
ak_fx32_mul(ak_fx32 fx1, ak_fx32 fx2)
{
  return ((int64_t)fx1) * fx2 / ak_s_fx32_fm;
}
static ak_fx32
ak_fx32_div(ak_fx32 fx1, ak_fx32 fx2)
{
  return ((int64_t)fx1 * ak_s_fx32_fm) / fx2;
}

ak_ex void
ak_iostream_print_fx32(ak_iostream io,
                       ak_fx32 fx);

#endif
