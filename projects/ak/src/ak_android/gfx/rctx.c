#include "ak_android/gfx/rctx.h"

#include "ak/core/async/atomic.h"
#include "ak/gfx/gcb.h"
#include "ak/gfx/rctx.h"
#include "ak/platform/plat_base.h"
#include "ak/res/core.h"
#include "ak_android/platform/plat_base.h"
#include "ak_opengl/gfx/gcore.h"
#include "ak_opengl/platform/plat_base.h"

struct ak_rctx
{
  ak_alct alct;
  ak_thread* th;

  bool inited;

  ak_plat_base* pb;
  ak_resreg* rr;
  ak_gcore gcore;

  ak_gcb gcb;

  ak_atomicint shouldclose;

  ak_mutex m;
  ak_cond c;

  ak_dispatcher d_pre;
  ak_dispatcher d;
};

static void
thread_fn(void* ctx)
{
  ak_rctx* r = ctx;

  ak_opengl_plat_base_glctx_startup(r->pb);
  ak_mutex_lock(&r->m);
  r->gcore =
    ak_opengl_gcore_make(100, 100, r->alct);
  r->inited = true;
  ak_cond_broadcast(&r->c);
  ak_mutex_unlock(&r->m);

  while (
    !ak_atomicint_load(&r->shouldclose)) {

    ak_thread_sleep(r->th);

    ak_mutex_lock(&r->m);

    ak_dispatcher_flush(&r->d_pre);

    if (ak_android_plat_base_render_trylock(
          r->pb)) {

      ak_dispatcher_flush(&r->d);

      // ak_gcb_flush(&r->gcb, r->gfx,
      // r->rr);

      ak_android_plat_base_render_unlock(
        r->pb);
    } else {
      ak_gcb_clear(&r->gcb);
    }
    ak_mutex_unlock(&r->m);
  }
  ak_opengl_gcore_destroy(&r->gcore);
  ak_opengl_plat_base_glctx_shutdown(r->pb);
}

ak_rctx*
ak_android_rctx_startup_begin(
  ak_plat_base* pb,
  ak_resreg* rr,
  ak_alct alct)
{
  ak_rctx* r =
    ak_alct_alloc(alct, sizeof(ak_rctx));
  r->alct = alct;
  r->inited = false;
  r->pb = pb;
  r->rr = rr;

  r->gcb = ak_gcb_make(r->alct);
  r->m = ak_mutex_make();
  r->c = ak_cond_make();

  ak_atomicint_store(&r->shouldclose, 0);

  ak_mutex_lock(&r->m);
  r->th = ak_thread_make(&thread_fn, r);
  r->d = ak_dispatcher_make(r->th);
  r->d_pre = ak_dispatcher_make(r->th);
  return r;
}

void
ak_android_rctx_startup_end(ak_rctx* r)
{
  while (!r->inited)
    ak_cond_wait(&r->c, &r->m);
  ak_mutex_unlock(&r->m);
}

ak_dispatcher*
ak_android_rctx_dispatcher(ak_rctx* r)
{
  return &r->d_pre;
}

void
ak_android_rctx_shutdown(ak_rctx* r);
