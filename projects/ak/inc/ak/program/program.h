#ifndef ak_program_program_h
#define ak_program_program_h

#include "ak/export.h"

typedef void (*ak_crash_fn)(void* ctx);

ak_ex void
ak_program_reg_crash_fn(ak_crash_fn crash_fn,
                        void* ctx);

ak_ex void
ak_program_crash();

#endif
