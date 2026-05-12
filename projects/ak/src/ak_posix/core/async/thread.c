#include "ak/core/async/thread.h"

#include <stdlib.h>

#include <pthread.h>

//===== ak_thread =====//
//--- private ---//
struct ak_thread
{
  pthread_t pt;
  ak_thread_fn fn;
  void* ctx;
};

static void*
thread_fn(void* thread)
{
  ak_thread* t = thread;
  t->fn(t->ctx);
  return 0;
}

//--- export ---//
ak_thread*
ak_thread_make(ak_thread_fn fn, void* ctx)
{
  ak_thread* t = malloc(sizeof(ak_thread));
  t->fn = fn;
  t->ctx = ctx;
  pthread_create(
    &t->pt, NULL, &thread_fn, t);
  return t;
}

void
ak_thread_join(ak_thread* t)
{
  pthread_join(t->pt, 0);
  free(t);
}
