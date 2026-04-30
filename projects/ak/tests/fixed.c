#include "ak/core/math/fixed.h"
#include "ak/core/io.h"
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
  ak_log("fixed done");
  ak_program_shutdown();
  return 0;
}
