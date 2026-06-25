#ifndef ak_core_async_cond_h
#define ak_core_async_cond_h

#include "ak/core/async/mutex.h"
#include "ak/export.h"

typedef struct
{
  void* c;
} ak_cond;

ak_ex ak_cond
ak_cond_make();

ak_ex void
ak_cond_destroy(ak_cond* c);

ak_ex void
ak_cond_wait(ak_cond* c, ak_mutex* m);

ak_ex void
ak_cond_signal(ak_cond* c);

ak_ex void
ak_cond_broadcast(ak_cond* c);

#endif
