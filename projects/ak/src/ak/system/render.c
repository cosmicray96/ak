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

  ak_sys_ren sys_ren;

  ak_dur time;
  ak_atomicint shouldclose;
  ak_atomicint rendering;

  ak_atomicint gfx_made;
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
  r->gcb = ak_gcb_make(r->ms, r->alct);

  ak_atomicint_store(&r->gfx_made, 1);

  r->sys_ren = ak_sys_ren_make(
    r->gf, &r->gcb, r->ms, r->wv, r->alct);

  ak_dur last = ak_dur_now();
  while (
    !ak_atomicint_load(&r->shouldclose)) {

    ak_thread_sleep(r->th);

    ak_atomicint_store(&r->rendering, 1);

    ak_dur now = ak_dur_now();
    ak_gresman_update(r->grm);

    ak_sys_ren_render(
      &r->sys_ren, ak_dur_diff(last, now));
    ak_gcb_flush(&r->gcb, r->gf);

    last = now;
    ak_atomicint_store(&r->rendering, 0);
  }

  ak_gcb_destroy(&r->gcb);
  ak_mtrlstg_destroy(r->ms);
  ak_sys_ren_destroy(&r->sys_ren);
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
  ak_atomicint_store(&r->gfx_made, 0);

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
ak_renderer_resize(ak_renderer* r,
                   uint32_t w,
                   uint32_t h)
{
  if (ak_atomicint_load(&r->gfx_made) == 0) {
    return;
  }
  ak_renderer_stallwait(r);
  ak_gcb_push_resize(&r->gcb, w, h);
}

void
ak_renderer_stallwait(ak_renderer* r)
{
  while (ak_atomicint_load(&r->rendering))
    ;
}

ak_gfx*
ak_renderer_gfx(ak_renderer* r)
{

  while (!ak_atomicint_load(&r->gfx_made))
    ;
  return r->gf;
}

ak_gresman*
ak_renderer_gresman(ak_renderer* r)
{
  while (!ak_atomicint_load(&r->gfx_made))
    ;
  return r->grm;
}
