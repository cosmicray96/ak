#include "ak/program/program.h"
#include "ak/core/math/math.h"
#include "ak/debug.h"
#include "ak/debug_itn.h"
#include "ak/program/program_itn.h"

#include "ak/os/time_itn.h"
#include "ak/program/event_itn.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

//===== ak_program =====//
//--- private ---//
#define s_max_crashfn 32
typedef struct
{
  ak_pgm_crashfn fn;
  void* ctx;
} crash_item;

typedef struct
{
  crash_item crashitems[s_max_crashfn];
  uint32_t crashitem_count;
  bool in_crash;
} program;
static program p = { 0 };

void
run_crashfns()
{
  for (uint32_t i = p.crashitem_count; i > 0;
       i--) {
    uint32_t idx = i - 1;
    crash_item item = p.crashitems[idx];
    item.fn(item.ctx);
  }
  p.crashitem_count = 0;
}

//--- export ---//
void
ak_program_startup()
{
  p.in_crash = false;
  p.crashitem_count = 0;

  ak_debug_startup();
  ak_pgm_crashfn_reg(&ak_debug_shutdown, 0);

  ak_math_startup();
  ak_pgm_crashfn_reg(&ak_math_shutdown, 0);

  ak_time_startup();
  ak_pgm_crashfn_reg(&ak_time_shutdown, 0);

  /*
ak_pgm_event_startup(
&ak_program_crash_fatal);
ak_pgm_crashfn_reg(&ak_pgm_event_shutdown,
               0);
  */
}

void
ak_program_shutdown()
{
  ak_pgm_event_shutdown(0);
  ak_time_shutdown(0);
  ak_math_shutdown(0);
  ak_debug_shutdown(0);
}

void
ak_pgm_crashfn_reg(ak_pgm_crashfn fn,
                   void* ctx)
{
  ak_assert(!p.in_crash);
  ak_assert(p.crashitem_count <
            s_max_crashfn);

  crash_item item = { 0 };
  item.fn = fn;
  item.ctx = ctx;
  p.crashitems[p.crashitem_count] = item;

  p.crashitem_count++;
}

void
ak_program_crash()
{
  ak_log_crash_itn();
  if (p.in_crash) { // crash in crash
    ak_log_cic_itn();
    exit(1);
  }
  p.in_crash = true;
  run_crashfns();
  exit(1);
}
