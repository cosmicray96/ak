#include "ak/core/async/dispatcher.h"
#include "ak/core/async/cond.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thread.h"

//===== ak_dispatcher =====//
//--- private ---//
static void
run(ak_dispatcher* d,
    void (*fn)(void* ctx),
    void* ctx,
    bool wake)
{
  ak_mutex_lock(&d->m_run);
  ak_mutex_lock(&d->m);
  d->fn = fn;
  d->ctx = ctx;
  d->pending = true;
  ak_cond_broadcast(&d->c);
  if (wake) {
    ak_thread_wake(d->th);
  }

  while (d->pending) {
    ak_cond_wait(&d->c, &d->m);
  }

  ak_mutex_unlock(&d->m);
  ak_mutex_unlock(&d->m_run);
}

//--- export ---//
ak_dispatcher
ak_dispatcher_make(ak_thread* th)
{
  return (
    ak_dispatcher){ .th = th,
                    .m = ak_mutex_make(),
                    .m_run = ak_mutex_make(),
                    .c = ak_cond_make(),
                    .pending = false };
}

void
ak_dispatcher_destroy(ak_dispatcher* d)
{
  ak_mutex_destroy(&d->m);
  ak_mutex_destroy(&d->m_run);
  ak_cond_destroy(&d->c);
}

void
ak_dispatcher_flush(ak_dispatcher* d)
{
  ak_mutex_lock(&d->m);
  if (d->pending) {
    d->fn(d->ctx);
    d->pending = false;
    ak_cond_broadcast(&d->c);
  }
  ak_mutex_unlock(&d->m);
}

void
ak_dispatcher_flush_wait(ak_dispatcher* d)
{
  ak_mutex_lock(&d->m);
  while (!d->pending) {
    ak_cond_wait(&d->c, &d->m);
  }
  d->fn(d->ctx);
  d->pending = false;
  ak_cond_broadcast(&d->c);
  ak_mutex_unlock(&d->m);
}

void
ak_dispatcher_run(ak_dispatcher* d,
                  void (*fn)(void* ctx),
                  void* ctx)
{
  run(d, fn, ctx, false);
}

void
ak_dispatcher_wake_run(ak_dispatcher* d,
                       void (*fn)(void* ctx),
                       void* ctx)
{
  run(d, fn, ctx, true);
}
