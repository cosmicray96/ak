#include "ak/system/render.h"
#include "ak/core/async/atomic.h"
#include "ak/core/async/cond.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thread.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gcb.h"
#include "ak/gfx/gfx.h"
#include "ak/gfx/gresman.h"
#include "ak/gfx/gresreg.h"
#include "ak/os/time.h"
#include "ak/platform/plat_base.h"
#include <stdbool.h>

//===== ak_renderer =====//
//--- private ---//
struct ak_renderer
{
  ak_alct alct;
  ak_thread* th;

  bool inited;

  ak_plat_base* pb;
  ak_gfx* gfx;

  ak_gresreg* grr;
  ak_gresman* grm;

  ak_gcb gcb;

  ak_dur time;
  ak_atomicint status;
  ak_atomicint shouldclose;

  ak_mutex m;
  ak_cond c;

  ak_mutex rfn_m;
  ak_renderer_fn rfn;
  void* rfn_ctx;
  ak_cond rfn_c;
  bool rfn_pending;
};
static ak_renderer* s_r;

static void
run_rfn(ak_renderer* r)
{
  ak_mutex_lock(&r->rfn_m);
  if (r->rfn_pending) {
    r->rfn(r->rfn_ctx);
    r->rfn_pending = false;
    ak_cond_broadcast(&r->rfn_c);
  }
  ak_mutex_unlock(&r->rfn_m);
}

static void
thread_fn(void* ctx)
{
  ak_renderer* r = ctx;

  ak_mutex_lock(&r->m);
  r->gfx = ak_gfx_startup(r->pb, r->alct);
  r->grr = ak_gresreg_make(r->gfx, r->alct);
  r->grm = ak_gresman_startup(
    r->grr, r->gfx, r->alct);
  r->inited = true;
  ak_cond_broadcast(&r->c);
  ak_mutex_unlock(&r->m);

  while (
    !ak_atomicint_load(&r->shouldclose)) {

    ak_atomicint_store(&r->status,
                       ak_renderer_idle);
    ak_thread_sleep(r->th);

    ak_mutex_lock(&r->m);

    run_rfn(r);

    if (ak_plat_base_render_trylock(r->pb)) {
      ak_atomicint_store(
        &r->status, ak_renderer_rendering);

      ak_gresman_update(r->grm);
      ak_gcb_flush(&r->gcb, r->gfx, r->grr);

      ak_plat_base_render_unlock(r->pb);
    } else {
      ak_gcb_clear(&r->gcb);
    }
    ak_mutex_unlock(&r->m);
  }
  ak_gresman_shutdown(r->grm);
  ak_gresreg_destroy(r->grr);
  ak_gfx_shutdown(r->gfx);
}

//--- internal ---//
ak_renderer*
ak_renderer_startup(ak_plat_base* pb,
                    ak_alct alct)
{
  ak_renderer* r =
    ak_alct_alloc(alct, sizeof(ak_renderer));
  s_r = r;
  r->alct = alct;
  r->inited = false;
  r->pb = pb;
  r->gcb = ak_gcb_make(r->alct);
  r->m = ak_mutex_make();
  r->c = ak_cond_make();

  r->rfn_pending = false;
  r->rfn = 0;
  r->rfn_ctx = 0;
  r->rfn_m = ak_mutex_make();
  r->rfn_c = ak_cond_make();

  ak_atomicint_store(&r->shouldclose, 0);
  ak_atomicint_store(&r->status,
                     ak_renderer_initing);

  r->th = ak_thread_make(&thread_fn, r);

  ak_mutex_lock(&r->m);
  while (!r->inited)
    ak_cond_wait(&r->c, &r->m);
  ak_mutex_unlock(&r->m);
  return r;
}

void
ak_renderer_shutdown(ak_renderer* r)
{
  ak_atomicint_store(&r->shouldclose, 1);
  ak_thread_wake(r->th);
  ak_thread_join(r->th);

  ak_gcb_destroy(&r->gcb);
  ak_cond_destroy(&r->rfn_c);

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

ak_gresman*
ak_renderer_gresman_get(ak_renderer* r)
{
  return r->grm;
}

ak_gresreg*
ak_renderer_gresreg_get(ak_renderer* r)
{
  return r->grr;
}

ak_renderer_status
ak_renderer_status_get(ak_renderer* r)
{
  return ak_atomicint_load(&r->status);
}

void
ak_renderer_run_fn(ak_renderer* r,
                   ak_renderer_fn fn,
                   void* ctx)
{
  ak_mutex_lock(&r->rfn_m);
  r->rfn = fn;
  r->rfn_ctx = ctx;
  r->rfn_pending = true;
  ak_thread_wake(r->th);

  while (r->rfn_pending) {
    ak_cond_wait(&r->rfn_c, &r->rfn_m);
  }

  ak_mutex_unlock(&r->rfn_m);
}
