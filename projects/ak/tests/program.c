#include "ak/program/program.h"
#include <ak/debug.h>

int
main()
{
  ak_program_startup();

  ak_log("Hello, %s", "World!");

  char* c = 0;
  *c = 'a';

  ak_program_shutdown();
  return 0;
}
