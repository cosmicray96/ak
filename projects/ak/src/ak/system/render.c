#include "ak/system/render.h"
#include "ak/core/async/atomic.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thread.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gcb.h"
#include "ak/gfx/gfx.h"
#include "ak/gfx/gresman.h"
#include "ak/gfx/gresreg.h"
#include "ak/gfx/mtrl/stg.h"
#include "ak/os/cpu.h"
#include "ak/os/time.h"
#include "ak/platform/plat_base.h"
#include <stdbool.h>

//===== ak_renderer =====//
//--- private ---//
struct ak_renderer
{
  ak_alct alct;
  ak_thread* th;

  ak_plat_base* pb;
  ak_gfx* gfx;

  ak_gresreg* grr;
  ak_gresman* grm;
  ak_mtrlstg* ms;

  ak_gcb gcb;

  ak_dur time;
  ak_atomicint status;
  ak_atomicint shouldclose;

  ak_mutex m;
};

void
thread_fn(void* ctx)
{
  ak_renderer* r = ctx;

  r->gfx = ak_gfx_startup(r->pb, r->alct);
  r->grr = ak_gresreg_make(r->gfx, r->alct);
  r->grm = ak_gresman_startup(
    r->grr, r->gfx, r->alct);
  r->ms = ak_mtrlstg_make(r->gfx, r->alct);

  while (
    !ak_atomicint_load(&r->shouldclose)) {

    ak_atomicint_store(&r->status,
                       ak_renderer_idle);
    ak_thread_sleep(r->th);

    ak_mutex_lock(&r->m);
    ak_atomicint_store(
      &r->status, ak_renderer_rendering);
    ak_gresman_update(r->grm);
    ak_gcb_flush(
      &r->gcb, r->gfx, r->grr, r->ms);
    ak_mutex_unlock(&r->m);
  }

  ak_mtrlstg_destroy(r->ms);
  ak_gfx_shutdown(r->gfx);
}

//--- internal ---//
ak_renderer*
ak_renderer_startup(ak_plat_base* pb,
                    ak_alct alct)
{
  ak_renderer* r =
    ak_alct_alloc(alct, sizeof(ak_renderer));
  r->alct = alct;
  r->pb = pb;
  r->gcb = ak_gcb_make(r->alct);
  r->m = ak_mutex_make();

  ak_atomicint_store(&r->shouldclose, 0);
  ak_atomicint_store(&r->status,
                     ak_renderer_initing);

  r->th = ak_thread_make(&thread_fn, r);
  return r;
}

void
ak_renderer_shutdown(ak_renderer* r)
{
  ak_atomicint_store(&r->shouldclose, 1);
  ak_thread_wake(r->th);
  ak_thread_join(r->th);

  ak_gcb_destroy(&r->gcb);
  ak_mutex_destroy(&r->m);
  ak_alct_free(r->alct, r);
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
  while (ak_atomicint_load(&r->status) ==
         ak_renderer_initing) {
    ak_cpu_yield();
  }
  return r->grm;
}

ak_gresreg*
ak_renderer_gresreg_get(ak_renderer* r)
{
  while (ak_atomicint_load(&r->status) ==
         ak_renderer_initing) {
    ak_cpu_yield();
  }
  return r->grr;
}

ak_renderer_status
ak_renderer_status_get(ak_renderer* r)
{
  return ak_atomicint_load(&r->status);
}
