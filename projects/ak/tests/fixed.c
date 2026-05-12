#include "ak/core/math/fixed.h"
#include "ak/core/io.h"
#include "ak/core/math/trig.h"
#include <ak/debug.h>
#include <ak/program/program.h>
#include <stdint.h>

int
main()
{
  ak_program_startup();

  ak_log("fixed start");

  ak_iostream sio = ak_iostream_sio();

  /*
{
ak_log("basic");
ak_fx32 fx = ak_fx32_f(10.5f);
ak_iostream_print_fx32(sio, fx);
ak_iostream_write(sio, "\n", 1);
ak_log("done.");
}
{
ak_log("basic2");

ak_iostream_print_u64(sio, 10);
ak_iostream_write(sio, "\n", 1);

ak_log("done.");
}
  */
  /*
{
ak_log("sin");
ak_iostream_write_str(sio, "Begin\n");

for (uint32_t i = 0; i < 10; i++) {
ak_angle ag = ak_angle_deg(
  ak_fx32_f(30.0f + (i * 5)));
ak_fx32 fx = ak_sin(ag);

ak_iostream_print_angle(sio, ag);
ak_iostream_write(sio, " -- ", 4);
ak_iostream_print_fx32(sio, fx);
ak_iostream_write(sio, "\n", 1);
}

ak_log("done.");
}
  */
  /*
{
ak_log("sqrt");
ak_iostream_write_str(sio, "sqrt\n");

ak_fx32 fx1 = ak_fx32_f(2.0f);
ak_fx32 fx2 = ak_fx32_sqrt(fx1);

ak_iostream_print_fx32(sio, fx1);
ak_iostream_write(sio, "\n", 1);
ak_iostream_print_fx32(sio, fx2);
ak_iostream_write(sio, "\n", 1);

ak_log("done.");
}
  */
  /*
{
ak_log("lerp");

ak_fx32 fx1 = ak_fx32_f(1.0f);
ak_fx32 fx2 = ak_fx32_f(10.f);

for (uint32_t i = 0; i < 10; i++) {
ak_fx32 t = ak_fx32_f(i / (float)10);
ak_fx32 l = ak_fx32_lerp(fx1, fx2, t);

ak_iostream_print_fx32(sio, fx1);
ak_iostream_write(sio, " , ", 3);
ak_iostream_print_fx32(sio, fx2);
ak_iostream_write(sio, " = ", 3);
ak_iostream_print_fx32(sio, l);
ak_iostream_write(sio, " ,t= ", 5);
ak_iostream_print_fx32(sio, t);
ak_iostream_write(sio, "\n", 1);
}

ak_log("done.");
}
  */

  {
    ak_log("atan2");
    ak_angle a1 =
      ak_atan2(ak_fx_i(0),
               ak_fx_i(1)); // → 0     (0°)
    ak_angle a2 =
      ak_atan2(ak_fx_i(1),
               ak_fx_i(1)); // → ~8192  (45°)
    ak_angle a3 =
      ak_atan2(ak_fx_i(1),
               ak_fx_i(0)); // → 16384  (90°)
    ak_angle a4 = ak_atan2(
      ak_fx_i(1),
      ak_fx_i(-1)); // → ~24576 (135°)

    ak_iostream_print_angle(sio, a1);
    ak_iostream_write(sio, ", ", 2);
    ak_iostream_print_angle(sio, a2);
    ak_iostream_write(sio, ", ", 2);
    ak_iostream_print_angle(sio, a3);
    ak_iostream_write(sio, ", ", 2);
    ak_iostream_print_angle(sio, a4);

    ak_log("done.");
  }
  ak_log("fixed done");
  ak_program_shutdown();
  return 0;
}
