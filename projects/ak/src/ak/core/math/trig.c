#include "ak/core/math/trig.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/itn.h"
#include "ak/debug.h"
#include <math.h>

ak_fx32 ak_s_sin_lut[4096 + 1];

// precomputed arctan(2^-i) as ak_angle
// values arctan(2^-i) in radians * (65536 /
// tau)
#define ak_s_atan2_iters 14
static const ak_angle
  ak_s_atan2_lut[ak_s_atan2_iters] = {
    8192, // arctan(2^0)  = 45.000°
    4836, // arctan(2^-1) = 26.565°
    2555, // arctan(2^-2) = 14.036°
    1297, // arctan(2^-3) =  7.125°
    651,  // arctan(2^-4) =  3.576°
    326,  // arctan(2^-5) =  1.790°
    163,  // arctan(2^-6) =  0.895°
    81,   // arctan(2^-7) =  0.448°
    41,   // arctan(2^-8) =  0.224°
    20,   // arctan(2^-9) =  0.112°
    10,   // arctan(2^-10) = 0.056°
    5,    // arctan(2^-11) = 0.028°
    3,    // arctan(2^-12) = 0.014°
    1,    // arctan(2^-13) = 0.007°
  };

void
ak_sin_lut_init()
{
  for (int i = 0; i < 4096; i++) {
    float a =
      (float)i / 4096.0f * (ak_tau_f / 4.0f);
    ak_s_sin_lut[i] = ak_fx32_f(sinf(a));
  }
  ak_s_sin_lut[4096] = ak_fx32_f(sinf(1));
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
      // mirrored - next sample goes backward
      // in lut
      return ak_s_sin_lut[4095 - idx];
    case 2:
      return -ak_s_sin_lut[idx];
    case 3:
      return -ak_s_sin_lut[4095 - idx];
    default:
      ak_assert(false);
  }
  return 0;
}

/*
ak_fx32
ak_sin_old(ak_angle a)
{
  uint16_t quad = a >> 14;
  uint16_t intra = a & 0x3FFF;
  uint16_t idx = intra >> 2;

  ak_fx32 t = (ak_fx32)(a & 0b11)
              << (ak_s_fx32_fbits - 2);
  ak_fx32 s0 = ak_fx32_f(0);
  ak_fx32 s1 = ak_fx32_f(0);

  switch (quad) {
    case 0:
      s0 = ak_s_sin_lut[idx];
      s1 = ak_s_sin_lut[idx + 1];
      break;
    case 1:
      // mirrored - next sample goes backward
      // in lut
      s0 = ak_s_sin_lut[4095 - idx];
      s1 =
        ak_s_sin_lut[4096 -
                     idx]; // note: toward
                           // higher values
      break;
    case 2:
      s0 = -ak_s_sin_lut[idx];
      s1 = -ak_s_sin_lut[idx + 1];
      break;
    case 3:
      s0 = -ak_s_sin_lut[4095 - idx];
      s1 = -ak_s_sin_lut[4096 - idx];
      break;
  }
  ak_log("t = %f", ak_fx32_to_f(t));
  return s0;
  //  return ak_fx32_lerp(s0, s1, t);
}
*/

ak_fx32
ak_cos(ak_angle a)
{
  return ak_sin(
    a + 16384); // quarter circle offset
}

ak_angle
ak_atan2(ak_fx32 y, ak_fx32 x)
{
  if (x == 0 && y == 0)
    return 0;
  if (x == 0)
    return y > 0 ? 16384 : 49152; // ±90°

  int32_t vx = x;
  int32_t vy = y;
  int32_t angle = 0;

  // CORDIC only converges for vx > 0
  // flip and offset by 180° for left
  // half-plane
  if (vx < 0) {
    angle = (vy >= 0) ? 32768 : -32768;
    vx = -vx;
    vy = -vy;
  }

  for (int i = 0; i < ak_s_atan2_iters;
       i++) {
    int32_t tx = vx;
    int32_t ty = vy;
    if (vy >= 0) {
      // vector above x-axis, rotate
      // clockwise
      vx = tx + (ty >> i);
      vy = ty - (tx >> i);
      angle += ak_s_atan2_lut[i];
    } else {
      // vector below x-axis, rotate
      // counter-clockwise
      vx = tx - (ty >> i);
      vy = ty + (tx >> i);
      angle -= ak_s_atan2_lut[i];
    }
  }

  return (ak_angle)
    angle; // uint16_t cast handles negative
           // wrap naturally
}

void
ak_iostream_print_angle(ak_iostream io,
                        ak_angle ag)
{
  double d = ((double)ag / UINT16_MAX) * 360;
  ak_iostream_print_d(io, d);
  ak_iostream_write(io, "Deg", 3);
}
