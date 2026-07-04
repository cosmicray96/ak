#include "ak_android/gfx/rctx.h"

#include "ak/core/async/atomic.h"
#include "ak/core/async/cond.h"
#include "ak/core/async/dispatcher.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thread.h"
#include "ak/gfx/gcb.h"
#include "ak/gfx/rctx.h"
#include "ak/platform/plat_base.h"
#include "ak/res/core.h"
#include "ak_android/platform/plat_base.h"
#include "ak_opengl/gfx/gcb.h"
#include "ak_opengl/gfx/gcore.h"
#include "ak_opengl/platform/plat_base.h"

struct ak_rctx
{
  ak_alct alct;

  ak_plat_base* pb;

  ak_resreg* rr;
  ak_gcore gcore;
  ak_gcb gcb;

  bool inited;
  ak_cond c;
  ak_mutex m;
  ak_thread* th;
  ak_dispatcher d;

  ak_atomicint close;
};

static void
thread_fn(void* ctx)
{
  ak_rctx* r = ctx;
  ak_mutex_lock(&r->m);
  ak_android_plat_base_rctx_loader_startup(
    r->pb);
  r->inited = true;
  ak_cond_broadcast(&r->c);
  ak_mutex_unlock(&r->m);
  while (!ak_atomicint_load(&r->close)) {

    ak_thread_sleep(r->th);
    ak_dispatcher_flush(&r->d);
  }
  ak_android_plat_base_rctx_loader_shutdown(
    r->pb);
}

ak_rctx*
ak_android_rctx_startup(ak_plat_base* pb,
                        ak_resreg* rr,
                        ak_alct alct)
{
  ak_rctx* r =
    ak_alct_alloc(alct, sizeof(ak_rctx));
  r->alct = alct;
  r->pb = pb;
  r->rr = rr;
  r->gcore = ak_opengl_gcore_make(
    ak_plat_base_width(pb),
    ak_plat_base_height(pb),
    alct);
  r->gcb = ak_gcb_make(r->alct);

  r->inited = false;
  r->c = ak_cond_make();
  r->m = ak_mutex_make();
  r->close = ak_atomicint_make(0);

  ak_mutex_lock(&r->m);
  r->th = ak_thread_make(&thread_fn, r);
  r->d = ak_dispatcher_make(r->th);
  while (!r->inited) {
    ak_cond_wait(&r->c, &r->m);
  }
  ak_mutex_unlock(&r->m);
  return r;
}

void
ak_android_rctx_shutdown(ak_rctx* r)
{
  ak_atomicint_store(&r->close, 1);
  ak_thread_wake(r->th);
  ak_thread_join(r->th);
  ak_dispatcher_destroy(&r->d);
  ak_mutex_destroy(&r->m);
  ak_cond_destroy(&r->c);

  ak_gcb_destroy(&r->gcb);
  ak_opengl_gcore_destroy(&r->gcore);

  ak_alct_free(r->alct, r);
}

void
ak_rctx_render(ak_rctx* r, ak_gcb* gcb)
{
  if (ak_android_plat_base_surface_ready(
        r->pb)) {
    ak_gcb_flush(gcb, &r->gcore, r->rr);
    ak_opengl_plat_base_swapbuffer(r->pb);
  }
}

ak_dispatcher*
ak_opengl_rctx_dispatcher(ak_rctx* r)
{
  return &r->d;
}

void
ak_opengl_rctx_call_begin(ak_rctx* r)
{
  ak_opengl_gcore_call_begin(&r->gcore);
}
void
ak_opengl_rctx_call_end(ak_rctx* r)
{

  ak_opengl_gcore_call_end(&r->gcore);
}
void
ak_opengl_rctx_pushquad(
  ak_rctx* r,
  const ak_gfx_quaddata* qd,
  const ak_mat3_f* gmat3)
{
  ak_opengl_gcore_pushquad(
    &r->gcore, qd, gmat3);
}
