#define _POSIX_C_SOURCE 200112L
#include "ak/core/async/mutex.h"

#include <stdlib.h>

#include <pthread.h>

//===== ak_mutex =====//
//--- private ---//
typedef struct
{
  pthread_mutex_t pm;
} m_impl;

//--- export ---//
ak_mutex
ak_mutex_make()
{
  ak_mutex m = { 0 };
  m.m = malloc(sizeof(m_impl));

  m_impl* impl = m.m;
  pthread_mutex_init(&impl->pm, NULL);

  return m;
}

void
ak_mutex_destroy(ak_mutex* m)
{
  m_impl* impl = (m_impl*)m->m;
  pthread_mutex_destroy(&impl->pm);

  free(m->m);
  m->m = 0;
}
void
ak_mutex_lock(ak_mutex* m)
{
  m_impl* impl = (m_impl*)m->m;
  pthread_mutex_lock(&impl->pm);
}

void
ak_mutex_unlock(ak_mutex* m)
{
  m_impl* impl = (m_impl*)m->m;
  pthread_mutex_unlock(&impl->pm);
}
