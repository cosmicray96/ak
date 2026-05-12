#ifndef ak_core_async_mutex_h
#define ak_core_async_mutex_h

#include "ak/core/mem/allocator.h"
#include "ak/export.h"
typedef struct
{
  void* m;
} ak_mutex;

ak_ex ak_mutex
ak_mutex_make();
ak_ex void
ak_mutex_destroy(ak_mutex* m);

ak_ex void
ak_mutex_lock(ak_mutex* m);
ak_ex void
ak_mutex_unlock(ak_mutex* m);

#endif
