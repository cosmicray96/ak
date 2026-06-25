#define _POSIX_C_SOURCE 200112L
#include "ak/core/async/mutex.h"

#include <stdlib.h>

#include <pthread.h>

//===== ak_mutex =====//

//--- export ---//
ak_mutex
ak_mutex_make()
{
  ak_mutex m = { 0 };
  m.m = malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(m.m, NULL);

  return m;
}

void
ak_mutex_destroy(ak_mutex* m)
{
  pthread_mutex_destroy(m->m);
  free(m->m);
}

void
ak_mutex_lock(ak_mutex* m)
{
  pthread_mutex_lock(m->m);
}

void
ak_mutex_unlock(ak_mutex* m)
{
  pthread_mutex_unlock(m->m);
}
