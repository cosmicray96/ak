#ifndef ak_program_program_h
#define ak_program_program_h

#include "ak/export.h"
#include "ak/os/time.h"

typedef void (*ak_pgm_crashfn)(void* ctx);

ak_ex void
ak_program_startup();
ak_ex void
ak_program_shutdown();

ak_ex void
ak_pgm_crashfn_reg(ak_pgm_crashfn fn,
                   void* ctx);

ak_ex void
ak_program_crash();

ak_ex void
ak_mainthread_sleep(ak_dur t);

#endif
