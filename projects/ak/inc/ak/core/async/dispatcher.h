#ifndef ak_gfx_dispatcher_h
#define ak_gfx_dispatcher_h

#include "ak/core/async/cond.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thread.h"
typedef struct
{
  ak_thread* th;
  ak_mutex m;
  ak_cond c;
  bool pending;
  void (*fn)(void* ctx);
  void* ctx;
} ak_dispatcher;

ak_dispatcher
ak_dispatcher_make(ak_thread* th);
void
ak_dispatcher_destroy(ak_dispatcher* d);

void
ak_dispatcher_flush(ak_dispatcher* d);

void
ak_dispatcher_run(ak_dispatcher* d,
                  void (*fn)(void* ctx),
                  void* ctx);

#endif
