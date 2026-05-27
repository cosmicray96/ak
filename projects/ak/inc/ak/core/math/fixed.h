#ifndef ak_core_math_fixed_h
#define ak_core_math_fixed_h

#include "ak/core/io.h"
#include "ak/export.h"
#include "ak/system/stream.h"
#include <stdint.h>

typedef int32_t ak_fx;
#define ak_s_fx32_fbits 12
#define ak_s_fx32_fm                        \
  ((int64_t)1 << ak_s_fx32_fbits)
#define ak_s_fx32_fmask ak_s_fx32_fm - 1

static ak_fx
ak_fx_f(float f)
{
  ak_fx fx = f * ak_s_fx32_fm;
  return fx;
}
static ak_fx
ak_fx_i(int32_t n)
{
  return (ak_fx)((int64_t)n
                 << ak_s_fx32_fbits);
}

static float
ak_fx_to_f(ak_fx fx)
{
  return (float)fx / (float)ak_s_fx32_fm;
}

static int32_t
ak_fx_to_i(ak_fx fx)
{
  return fx >> ak_s_fx32_fbits;
}

static ak_fx
ak_fxadd(ak_fx fx1, ak_fx fx2)
{
  return fx1 + fx2;
}
static ak_fx
ak_fxsub(ak_fx fx1, ak_fx fx2)
{
  return fx1 - fx2;
}
static ak_fx
ak_fxmul(ak_fx fx1, ak_fx fx2)
{
  return ((int64_t)fx1) * fx2 / ak_s_fx32_fm;
}
static ak_fx
ak_fxdiv(ak_fx fx1, ak_fx fx2)
{
  return ((int64_t)fx1 * ak_s_fx32_fm) / fx2;
}

static int32_t
ak_fx_sign(ak_fx fx)
{
  int32_t raw = fx;
  if (raw > 0)
    return 1;
  if (raw < 0)
    return -1;
  return 0;
}
static ak_fx
ak_fx_abs(ak_fx fx)
{
  if (ak_fx_sign(fx) < 0) {
    return (uint32_t)(-(int64_t)fx);
  } else {
    return fx;
  }
}

static ak_fx
ak_fx_round(ak_fx fx)
{
  return ak_fx_i(ak_fx_to_i(fx));
}

static ak_fx
ak_fx_frac(ak_fx fx)
{
  fx = ak_fx_abs(fx);
  return fx & ((1 << ak_s_fx32_fbits) - 1);
}

static ak_fx
ak_fx_floor(ak_fx fx)
{
  return fx & ~((1 << ak_s_fx32_fbits) - 1);
}

static ak_fx
ak_fx_ceil(ak_fx fx)
{
  ak_fx frac = ak_fx_frac(fx);
  if (frac == 0)
    return fx;
  return ak_fx_floor(fx) + ak_fx_i(1);
}

static ak_fx
ak_fx_lerp(ak_fx a, ak_fx b, ak_fx t)
{
  // t is fx32 in [0, 1]
  return ak_fxadd(
    a, ak_fxmul(ak_fxsub(b, a), t));
}

ak_ex ak_fx
ak_fx_sqrt(ak_fx fx);

ak_ex void
ak_iostream_print_fx(ak_iostream io,
                     ak_fx fx);

ak_ex ak_stmerr
ak_stm_write_fx(ak_stm stm, ak_fx fx);
ak_ex ak_stmerr
ak_stm_read_fx(ak_stm stm, ak_fx* o_fx);

#endif
