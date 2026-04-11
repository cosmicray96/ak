#include "ak/program/program.h"
#include <ak/debug.h>

int
main()
{
  ak_program_startup();

  ak_log("Hello, %s", "World!");

  ak_program_shutdown();
  return 0;
}
