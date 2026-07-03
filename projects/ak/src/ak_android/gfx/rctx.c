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
  ak_thread* th;

  ak_plat_base* pb;
  ak_resreg* rr;
  ak_gcore gcore;

  ak_gcb gcb;

  ak_atomicint shouldclose;

  bool inited;

  ak_mutex m;
  ak_cond c;

  ak_dispatcher d_pre;
  ak_dispatcher d;
};

static void
thread_fn(void* ctx)
{
  ak_rctx* r = ctx;

  while (
    !ak_atomicint_load(&r->shouldclose)) {

    ak_thread_sleep(r->th);

    ak_mutex_lock(&r->m);

    ak_dispatcher_flush(&r->d_pre);

    if (ak_android_plat_base_render_trylock(
          r->pb)) {

      if (!r->inited) {
        r->gcore = ak_opengl_gcore_make(
          ak_plat_base_width(r->pb),
          ak_plat_base_height(r->pb),
          r->alct);
        r->inited = true;
        ak_cond_broadcast(&r->c);
      }

      ak_dispatcher_flush(&r->d);

      ak_gcb_flush(
        &r->gcb, &r->gcore, r->rr);
      ak_opengl_plat_base_swapbuffer(r->pb);

      ak_android_plat_base_render_unlock(
        r->pb);
    } else {
      ak_gcb_clear(&r->gcb);
    }
    ak_mutex_unlock(&r->m);
  }
  ak_opengl_gcore_destroy(&r->gcore);
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
  r->inited = false;

  r->gcb = ak_gcb_make(r->alct);
  r->m = ak_mutex_make();
  r->c = ak_cond_make();

  ak_atomicint_store(&r->shouldclose, 0);

  ak_mutex_lock(&r->m);
  r->th = ak_thread_make(&thread_fn, r);
  r->d = ak_dispatcher_make(r->th);
  r->d_pre = ak_dispatcher_make(r->th);
  ak_mutex_unlock(&r->m);
  return r;
}

ak_dispatcher*
ak_android_rctx_dispatcher_pre(ak_rctx* r)
{
  return &r->d_pre;
}

void
ak_android_rctx_wait_inited(ak_rctx* r)
{
  ak_mutex_lock(&r->m);
  while (!r->inited) {
    ak_cond_wait(&r->c, &r->m);
  }
  ak_mutex_unlock(&r->m);
}

void
ak_android_rctx_shutdown(ak_rctx* r)
{
  ak_atomicint_store(&r->shouldclose, 1);
  ak_thread_wake(r->th);
  ak_thread_join(r->th);

  ak_dispatcher_destroy(&r->d);

  ak_gcb_destroy(&r->gcb);

  ak_cond_destroy(&r->c);
  ak_mutex_destroy(&r->m);
  ak_alct_free(r->alct, r);
}

void
ak_rctx_render(ak_rctx* r, ak_gcb* gcb)
{
  ak_mutex_lock(&r->m);
  ak_gcb_joinback(&r->gcb, gcb);
  ak_mutex_unlock(&r->m);
  ak_thread_wake(r->th);
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
