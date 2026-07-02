#include "ak/gfx/rctx.h"
#include "ak/core/async/dispatcher.h"
#include "ak_opengl_x11/gfx/rctx.h"

#include "ak/core/async/atomic.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gcb.h"
#include "ak/platform/plat_base.h"
#include "ak_opengl/gfx/gcb.h"
#include "ak_opengl/gfx/gcore.h"

//===== ak_rctx =====//
//--- private ---//
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

  ak_dispatcher d;

  ak_mutex m;
  ak_cond c;
};

static void
thread_fn(void* ctx)
{
  ak_rctx* r = ctx;

  ak_mutex_lock(&r->m);
  r->gcore = ak_opengl_gcore_make(
    ak_plat_base_width(r->pb),
    ak_plat_base_height(r->pb),
    r->alct);
  r->inited = true;
  ak_cond_broadcast(&r->c);
  ak_mutex_unlock(&r->m);

  while (
    !ak_atomicint_load(&r->shouldclose)) {

    ak_thread_sleep(r->th);

    ak_mutex_lock(&r->m);
    ak_dispatcher_flush(&r->d);

    ak_gcb_flush(&r->gcb, &r->gcore, r->rr);

    ak_mutex_unlock(&r->m);
  }
  ak_opengl_gcore_destroy(&r->gcore);
}

//--- internal ---//
ak_rctx*
ak_opengl_x11_rctx_startup(ak_plat_base* pb,
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
  while (!r->inited)
    ak_cond_wait(&r->c, &r->m);
  ak_mutex_unlock(&r->m);
  return r;
}

void
ak_opengl_x11_rctx_shutdown(ak_rctx* r)
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
