#include "ak/core/math/fixed.h"
#include "ak/core/io.h"

ak_fx
ak_fx_sqrt(ak_fx fx)
{
  if (fx <= 0)
    return 0;
  int64_t n = (int64_t)fx << ak_s_fx32_fbits;
  int64_t x = n;
  int64_t y = (x + 1) >> 1;
  while (y < x) {
    x = y;
    y = (x + n / x) >> 1;
  }
  return (ak_fx)x;
}

void
ak_iostream_print_fx(ak_iostream io,
                     ak_fx fx)
{
  int32_t raw = fx;
  uint32_t abs = 0;
  uint32_t whole = 0;
  uint32_t frac = 0;

  if (raw < 0) {
    ak_iostream_write(io, "-", 1);
    abs = (uint32_t)(-(int64_t)raw);
  } else {
    abs = raw;
  }
  whole = abs >> ak_s_fx32_fbits;
  frac = abs & ak_s_fx32_fmask;
  frac =
    ((uint64_t)frac * 10000) / ak_s_fx32_fm;

  ak_iostream_print_u64(io, whole);
  ak_iostream_write(io, ".", 1);

  // pad frac to 4 digits
  if (frac < 1000)
    ak_iostream_write(io, "0", 1);
  if (frac < 100)
    ak_iostream_write(io, "0", 1);
  if (frac < 10)
    ak_iostream_write(io, "0", 1);
  ak_iostream_print_u64(io, frac);
}
