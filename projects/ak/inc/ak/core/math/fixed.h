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

static int32_t
ak_fx32_sign(ak_fx32 fx)
{
  int32_t raw = fx;
  if (raw > 0)
    return 1;
  if (raw < 0)
    return -1;
  return 0;
}
static ak_fx32
ak_fx32_abs(ak_fx32 fx)
{
  if (ak_fx32_sign(fx) < 0) {
    return (uint32_t)(-(int64_t)fx);
  } else {
    return fx;
  }
}

static int32_t
ak_fx32_whole(ak_fx32 fx)
{
  return fx >> ak_s_fx32_fbits;
}

static ak_fx32
ak_fx32_frac(ak_fx32 fx)
{
  fx = ak_fx32_abs(fx);
  return fx & ((1 << ak_s_fx32_fbits) - 1);
}

ak_ex ak_fx32
ak_fx32_sqrt(ak_fx32 fx);

ak_ex void
ak_iostream_print_fx32(ak_iostream io,
                       ak_fx32 fx);

#endif
