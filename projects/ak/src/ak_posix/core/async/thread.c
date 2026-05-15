#include "ak/core/async/thread.h"

#include <stdbool.h>
#include <stdlib.h>

#include <pthread.h>

//===== ak_thread =====//
//--- private ---//
struct ak_thread
{
  pthread_t pt;
  ak_thread_fn fn;
  void* ctx;
  pthread_mutex_t mtx;
  pthread_cond_t cond;
  bool sleeping;
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
  pthread_mutex_init(&t->mtx, NULL);
  pthread_cond_init(&t->cond, NULL);
  t->sleeping = 0;
  pthread_create(
    &t->pt, NULL, &thread_fn, t);
  return t;
}

void
ak_thread_join(ak_thread* t)
{
  ak_thread_wake(t);
  pthread_join(t->pt, 0);
  pthread_cond_destroy(&t->cond);
  pthread_mutex_destroy(&t->mtx);
  free(t);
}

void
ak_thread_sleep(ak_thread* t)
{
  pthread_mutex_lock(&t->mtx);
  t->sleeping = true;
  while (t->sleeping) {
    pthread_cond_wait(&t->cond, &t->mtx);
  }
  pthread_mutex_unlock(&t->mtx);
}

void
ak_thread_wake(ak_thread* t)
{
  pthread_mutex_lock(&t->mtx);
  t->sleeping = false;
  pthread_cond_signal(&t->cond);
  pthread_mutex_unlock(&t->mtx);
}
