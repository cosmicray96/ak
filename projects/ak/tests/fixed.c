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
  {
    ak_log("sin");
    ak_iostream_write_str(sio, "Begin\n");

    ak_angle ag =
      ak_angle_deg(ak_fx32_f(30.0f));
    ak_fx32 fx = ak_sin(ag);

    ak_iostream_print_angle(sio, ag);
    ak_iostream_write(sio, "\n", 1);
    ak_iostream_print_fx32(sio, fx);
    ak_iostream_write(sio, "\n", 1);

    ak_log("done.");
  }
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
  ak_log("fixed done");
  ak_program_shutdown();
  return 0;
}
