#ifndef ak_program_program_h
#define ak_program_program_h

#include "ak/export.h"

typedef void (*ak_exit_fn)(void* ctx);

ak_ex void
ak_program_make();

ak_ex void
ak_program_exit_fn_register(
  ak_exit_fn exit_fn,
  void* exit_ctx);

ak_ex void
ak_program_exit();

#endif
