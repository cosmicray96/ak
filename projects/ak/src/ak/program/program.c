#include "ak/program/program.h"
#include "ak/debug.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

//===== ak_program =====//
//--- private ---//
#define s_max_exit_fns 32
typedef struct
{
  ak_exit_fn fn;
  void* ctx;

} exit_item;

typedef struct
{
  exit_item exits[s_max_exit_fns];
  uint32_t exit_count;
  bool in_crash;
} program;

program p = { 0 };

//--- public ---//
void
ak_program_startup()
{
  p.exit_count = 0;
  p.in_crash = false;
}

void
ak_program_shutdown()
{
  for (uint32_t i = p.exit_count; i > 0;
       i--) {
    uint32_t idx = i - 1;
    exit_item e = p.exits[idx];
    e.fn(e.ctx);
  }
  p.exit_count = 0;
  p.in_crash = false;
}

void
ak_program_reg_exit_fn(ak_exit_fn fn,
                       void* ctx)
{
  exit_item e = { 0 };
  e.fn = fn;
  e.ctx = ctx;
  p.exits[p.exit_count] = e;
  p.exit_count++;
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
