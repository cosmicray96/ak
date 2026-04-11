#include "ak/program/program.h"
#include "ak/debug.h"
#include "ak/debug_itn.h"

#include "ak/program/event_itn.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

//===== ak_program =====//
//--- private ---//
typedef struct
{
  bool in_crash;
} program;
static program p = { 0 };

//--- export ---//
void
ak_program_startup()
{
  p.in_crash = false;

  ak_debug_startup();
  ak_pgm_event_startup();
}

void
ak_program_shutdown()
{
  ak_pgm_event_shutdown();
  ak_debug_shutdown();
}

void
ak_program_crash()
{
  ak_log_crash();
  if (p.in_crash) { // crash in crash
    ak_log_cic();
    exit(1);
  }
  p.in_crash = true;
  ak_program_shutdown();
  exit(1);
}
