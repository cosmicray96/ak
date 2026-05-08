#ifndef ak_core_math_trig_h
#define ak_core_math_trig_h

#include "ak/core/io.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/vec2.h"
#include "ak/export.h"
#include <stdint.h>
typedef uint16_t ak_angle;

#define ak_pi_f 3.14159265f
#define ak_pi_d 3.14159265358979323846

#define ak_tau_f 6.28318530f
#define ak_tau_d 6.28318530717958647692

static ak_angle
ak_angle_deg(ak_fx32 fx)
{
  float deg = ak_fx32_to_f(fx);
  return (ak_angle)(deg *
                    (65536.0f / 360.0f));
}

static ak_angle
ak_angle_rad(ak_fx32 fx)
{
  float rad = ak_fx32_to_f(fx);
  return (ak_angle)(rad *
                    (65536.0f / (ak_tau_f)));
}

ak_ex ak_fx32
ak_sin(ak_angle a);

ak_ex ak_fx32
ak_cos(ak_angle a);

ak_ex ak_angle
ak_atan2(ak_fx32 y, ak_fx32 x);

static ak_angle
ak_atan2_vec2(ak_vec2 v)
{
  return ak_atan2(v.y, v.x);
}

ak_ex void
ak_iostream_print_angle(ak_iostream io,
                        ak_angle ag);
#endif
