#include "ak/system/render.h"
#include "ak/core/async/atomic.h"
#include "ak/core/async/thread.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/sys/ren.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gcb.h"
#include "ak/gfx/gfx.h"
#include "ak/gfx/gresman.h"
#include "ak/gfx/mtrl/stg.h"
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
  ak_gfx* gf;
  ak_wv* wv;
  ak_resman* rm;
  ak_gresman* grm;
  ak_mtrlstg* ms;

  ak_gcb gcb;

  ak_dur time;
  ak_atomicint shouldclose;
  ak_atomicint rendering;

  ak_atomicint inited;
};

void
thread_fn(void* ctx)
{
  ak_renderer* r = ctx;

  r->gf = ak_gfx_startup(r->pb, r->alct);
  r->grm = ak_gresman_startup(
    r->rm, r->gf, r->alct);
  r->ms =
    ak_mtrlstg_make(r->gf, r->grm, r->alct);
  r->gcb = ak_gcb_make(r->alct);

  ak_atomicint_store(&r->inited, 1);

  while (
    !ak_atomicint_load(&r->shouldclose)) {

    ak_thread_sleep(r->th);

    ak_atomicint_store(&r->rendering, 1);

    ak_gresman_update(r->grm);

    ak_gcb_flush(&r->gcb, r->gf, r->ms);

    ak_atomicint_store(&r->rendering, 0);
  }

  ak_gcb_destroy(&r->gcb);
  ak_mtrlstg_destroy(r->ms);
  ak_gfx_shutdown(r->gf);
}

//--- internal ---//
ak_renderer*
ak_renderer_startup(ak_plat_base* pb,
                    ak_wv* wv,
                    ak_resman* rm,
                    ak_alct alct)
{
  ak_renderer* r =
    ak_alct_alloc(alct, sizeof(ak_renderer));
  r->alct = alct;
  r->pb = pb;
  r->wv = wv;
  r->rm = rm;

  ak_atomicint_store(&r->shouldclose, 0);
  ak_atomicint_store(&r->rendering, 0);
  ak_atomicint_store(&r->inited, 0);

  r->th = ak_thread_make(&thread_fn, r);
  return r;
}

void
ak_renderer_shutdown(ak_renderer* r)
{
  ak_atomicint_store(&r->shouldclose, 1);
  ak_thread_join(r->th);
  ak_alct_free(r->alct, r);
}

void
ak_renderer_render(ak_renderer* r)
{
  ak_thread_wake(r->th);
}

void
ak_renderer_gcb(ak_renderer* r, ak_gcb* gcb)
{
  ak_renderer_stallwait(r);
  ak_gcb_joinback(&r->gcb, gcb);
}

void
ak_renderer_stallwait(ak_renderer* r)
{
  while (!ak_atomicint_load(&r->inited))
    ;
  while (ak_atomicint_load(&r->rendering))
    ;
}

ak_gresman*
ak_renderer_gresman(ak_renderer* r)
{

  ak_renderer_stallwait(r);
  return r->grm;
}
