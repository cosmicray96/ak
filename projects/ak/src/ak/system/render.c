#include "ak/system/render.h"
#include "ak/core/async/atomic.h"
#include "ak/core/async/cond.h"
#include "ak/core/async/dispatcher.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thread.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gcb.h"
#include "ak/gfx/gfx.h"
#include "ak/os/time.h"
#include "ak/platform/plat_base.h"
#include "ak/res/core.h"
#include <stdbool.h>

//===== ak_renderer =====//
//--- private ---//
struct ak_renderer
{
  ak_alct alct;
  ak_thread* th;

  bool inited;

  ak_plat_base* pb;
  ak_resreg* rr;
  ak_gfx* gfx;

  ak_gcb gcb;

  ak_dur time;
  ak_atomicint status;
  ak_atomicint shouldclose;

  ak_mutex m;
  ak_cond c;

  ak_dispatcher d_pre;
  ak_dispatcher d;
};
static ak_renderer* s_r;

static void
thread_fn(void* ctx)
{
  ak_renderer* r = ctx;

  ak_mutex_lock(&r->m);
  r->gfx =
    ak_gfx_startup(r->pb, &r->d, r->alct);
  r->inited = true;
  ak_cond_broadcast(&r->c);
  ak_mutex_unlock(&r->m);

  while (
    !ak_atomicint_load(&r->shouldclose)) {

    ak_atomicint_store(&r->status,
                       ak_renderer_idle);
    ak_thread_sleep(r->th);

    ak_mutex_lock(&r->m);

    ak_dispatcher_flush(&r->d_pre);

    if (ak_plat_base_render_trylock(r->pb)) {
      ak_atomicint_store(
        &r->status, ak_renderer_rendering);

      ak_dispatcher_flush(&r->d);

      ak_gcb_flush(&r->gcb, r->gfx, r->rr);

      ak_plat_base_render_unlock(r->pb);
    } else {
      ak_gcb_clear(&r->gcb);
    }
    ak_mutex_unlock(&r->m);
  }
  ak_gfx_shutdown(r->gfx);
}

//--- internal ---//
ak_renderer*
ak_renderer_startup(ak_plat_base* pb,
                    ak_resreg* rr,
                    ak_alct alct)
{
  ak_renderer* r =
    ak_alct_alloc(alct, sizeof(ak_renderer));
  s_r = r;
  r->alct = alct;
  r->inited = false;
  r->pb = pb;
  r->rr = rr;

  r->gcb = ak_gcb_make(r->alct);
  r->m = ak_mutex_make();
  r->c = ak_cond_make();

  ak_atomicint_store(&r->shouldclose, 0);
  ak_atomicint_store(&r->status,
                     ak_renderer_initing);

  ak_mutex_lock(&r->m);
  r->th = ak_thread_make(&thread_fn, r);
  while (!r->inited)
    ak_cond_wait(&r->c, &r->m);

  r->d = ak_dispatcher_make(r->th);
  r->d_pre = ak_dispatcher_make(r->th);

  ak_mutex_unlock(&r->m);
  return r;
}

void
ak_renderer_shutdown(ak_renderer* r)
{
  ak_atomicint_store(&r->shouldclose, 1);
  ak_thread_wake(r->th);
  ak_thread_join(r->th);

  ak_dispatcher_destroy(&r->d);
  ak_dispatcher_destroy(&r->d_pre);

  ak_gcb_destroy(&r->gcb);

  ak_cond_destroy(&r->c);
  ak_mutex_destroy(&r->m);
  ak_alct_free(r->alct, r);
}

ak_renderer*
ak_renderer_get()
{
  return s_r;
}

void
ak_renderer_render(ak_renderer* r,
                   ak_gcb* gcb)
{
  ak_mutex_lock(&r->m);
  ak_gcb_joinback(&r->gcb, gcb);
  ak_mutex_unlock(&r->m);
  ak_thread_wake(r->th);
}

ak_renderer_status
ak_renderer_status_get(ak_renderer* r)
{
  return ak_atomicint_load(&r->status);
}

ak_dispatcher*
ak_renderer_dispatcher_pre(ak_renderer* r)
{
  return &r->d_pre;
}

ak_dispatcher*
ak_renderer_dispatcher(ak_renderer* r)
{
  return &r->d;
}

ak_gfx*
ak_renderer_gfx(ak_renderer* r)
{
  return r->gfx;
}
