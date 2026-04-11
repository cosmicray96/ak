#include "ak/debug_itn.h"
#include "ak/program/program_itn.h"
#include <unistd.h>

void
ak_program_crash_fatal()
{
  ak_log_crash_fatal();
  _exit(1);
}
