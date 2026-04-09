#include "ak/program/program.h"
#include "ak/debug.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

//===== ak_program =====//
#define s_max_exit_fns 32
typedef struct
{
} exit_fn_item;

typedef struct
{

  ak_crash_fn crash_fn;
  void* crash_ctx;

  bool in_crash;
} program;

//--- private ---//
program p = { 0 };

//--- public ---//
void
ak_program_reg_crash_fn(ak_crash_fn fn,
                        void* ctx)
{
  p.crash_fn = fn;
  p.crash_ctx = ctx;
}

void
ak_program_crash()
{
  if (p.in_crash) {
    ak_log_cic();
    exit(1);
  }
  p.in_crash = true;

  if (p.crash_fn) {
    p.crash_fn(p.crash_ctx);
  }

  exit(1);
}
