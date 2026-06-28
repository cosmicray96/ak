#ifndef ak_app_runner_h
#define ak_app_runner_h

#include "ak/os/time.h"

void
ak_runner(bool (*fn)(void* ctx),
          void* ctx,
          ak_dur frametime);

#endif
