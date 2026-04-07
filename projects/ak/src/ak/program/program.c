#include "ak/program/program.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

//===== ak_program =====//
#define s_max_exit_fns 32
typedef struct
{
  ak_exit_fn fn;
  void* ctx;
} exit_fn_item;

typedef struct
{
  exit_fn_item exit_fns[s_max_exit_fns];
  uint32_t exit_fn_count;
  bool in_crash;
} program;

//--- private ---//
program p = { 0 };

//--- public ---//
void
ak_program_make()
{
  p.in_crash = false;
  p.exit_fn_count = 0;
}

void
ak_program_exit_fn_register(
  ak_exit_fn exit_fn,
  void* exit_ctx)
{
  exit_fn_item item = { 0 };
  item.ctx = exit_ctx;
  item.fn = exit_fn;
  p.exit_fns[p.exit_fn_count] = item;
  p.exit_fn_count++;
}

void
ak_program_exit()
{
  if (p.in_crash) {
    exit(1);
  }
  p.in_crash = true;

  for (uint32_t i = p.exit_fn_count; i > 0;
       i--) {
    uint32_t idx = i - 1;
    exit_fn_item item = p.exit_fns[idx];
    if (item.fn) {
      item.fn(item.ctx);
    }
  }

  exit(1);
}
