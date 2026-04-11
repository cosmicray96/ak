#ifndef ak_program_program_h
#define ak_program_program_h

#include "ak/export.h"

typedef void (*ak_exit_fn)(void* ctx);

ak_ex void
ak_program_startup();
ak_ex void
ak_program_shutdown();

ak_ex void
ak_program_crash();

#endif
