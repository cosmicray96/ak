#include "ak/core/async/thread.h"

#include <stdbool.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

//===== ak_thread =====//
//--- private ---//
struct ak_thread
{
  pthread_t pt;
  ak_thread_fn fn;
  void* ctx;

  sem_t sem;
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
  sem_init(&t->sem, 0, 0);
  pthread_create(
    &t->pt, NULL, &thread_fn, t);
  return t;
}

void
ak_thread_join(ak_thread* t)
{
  ak_thread_wake(t);
  pthread_join(t->pt, 0);
  sem_destroy(&t->sem);
  free(t);
}

void
ak_thread_sleep(ak_thread* t)
{
  sem_wait(&t->sem);
}

void
ak_thread_wake(ak_thread* t)
{
  sem_post(&t->sem);
}
