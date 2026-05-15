#ifndef ak_core_sync_thread_h
#define ak_core_sync_thread_h

#include "ak/export.h"
typedef struct ak_thread ak_thread;
typedef void (*ak_thread_fn)(void* ctx);

ak_ex ak_thread*
ak_thread_make(ak_thread_fn fn, void* ctx);
ak_ex void
ak_thread_join(ak_thread* t);

ak_ex void
ak_thread_sleep(ak_thread* t);
ak_ex void
ak_thread_wake(ak_thread* t);

#endif
