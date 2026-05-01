#include "ak/core/math/trig.h"
#include "ak/core/math/itn.h"
#include <math.h>

ak_fx32 ak_s_sin_lut[4096];

void
ak_sin_lut_init()
{
  for (int i = 0; i < 4096; i++) {
    float a =
      (float)i / 4096.0f * (ak_tau_f / 4.0f);
    ak_s_sin_lut[i] = ak_fx32_f(sinf(a));
  }
}

ak_fx32
ak_sin(ak_angle a)
{
  uint16_t quad = a >> 14;
  uint16_t intra = a & 0x3FFF;
  uint16_t idx = intra >> 2;

  switch (quad) {
    case 0:
      return ak_s_sin_lut[idx];
    case 1:
      return ak_s_sin_lut[4095 - idx];
    case 2:
      return -ak_s_sin_lut[idx];
    case 3:
      return -ak_s_sin_lut[4095 - idx];
  }
  return 0; // unreachable
}

ak_fx32
ak_cos(ak_angle a)
{
  return ak_sin(
    a + 16384); // quarter circle offset
}

void
ak_iostream_print_angle(ak_iostream io,
                        ak_angle ag)
{
  double d = ((double)ag / UINT16_MAX) * 360;
  ak_iostream_print_d(io, d);
  ak_iostream_write(io, "Deg", 3);
}
