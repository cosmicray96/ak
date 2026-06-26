#include "ak/core/async/cond.h"

#include <pthread.h>
#include <stdlib.h>

ak_cond
ak_cond_make()
{
  ak_cond c;
  c.c = malloc(sizeof(pthread_cond_t));
  pthread_cond_init(c.c, NULL);
  return c;
}

void
ak_cond_destroy(ak_cond* c)
{
  pthread_cond_destroy(c->c);
  free(c->c);
}

void
ak_cond_wait(ak_cond* c, ak_mutex* m)
{
  pthread_cond_wait(c->c, m->m);
}

void
ak_cond_signal(ak_cond* c)
{
  pthread_cond_signal(c->c);
}

void
ak_cond_broadcast(ak_cond* c)
{
  pthread_cond_broadcast(c->c);
}
