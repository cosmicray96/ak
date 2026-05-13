#include "ak/app/app.h"
#include "ak/app/eq.h"
#include "ak/app/event.h"
#include "ak/coll/da.h"
#include "ak/debug.h"
#include "ak/os/time.h"
#include "ak/program/program.h"
#include <stdint.h>

ak_applayer_regs
ak_applayer_regs_make(ak_alct alct)
{
  ak_applayer_regs alr;
  alr.layers = ak_dq_make(
    sizeof(ak_applayer), 10, alct);
  alr.popping = false;
  return alr;
}
void
ak_applayer_regs_destroy(
  ak_applayer_regs* alr)
{
  ak_dq_destroy(&alr->layers);
  alr->popping = false;
}

void
ak_applayer_regs_push(ak_applayer_regs* alr,
                      ak_applayer l)
{
  ak_assert(!alr->popping);
  ak_dq_push(&alr->layers, &l);
}

bool
ak_applayer_regs_pop(ak_applayer_regs* alr,
                     ak_applayer* l)
{
  alr->popping = true;
  return ak_dq_pop(&alr->layers, l);
}

//===== ak_app  =====//
//--- private ---//
struct ak_app
{
  ak_alct alct;
  ak_da layers;
  uint32_t startuped_count;
  ak_app_eq eq;
  ak_dur frametime;
  ak_dur last_frametime;
  bool should_close;
};

void
run_startup(ak_app* a)
{
  ak_da_for_begin(
    ak_applayer, &a->layers, i, l);

  if (l->on_startup) {
    l->on_startup(l->ctx, a);
  }
  a->startuped_count++;

  ak_da_for_end();
}

void
run_shutdown(ak_app* a)
{
  for (uint32_t i = a->startuped_count;
       i-- > 0;) {
    ak_applayer* l =
      ak_da_at(ak_applayer, &a->layers, i);

    if (l->on_shutdown) {
      l->on_shutdown(l->ctx);
    }
  }
}

void
run_epusher(ak_app* a)
{
  ak_da_for_begin(
    ak_applayer, &a->layers, i, l);

  if (l->on_epusher) {
    l->on_epusher(l->ctx, &a->eq);
  }

  ak_da_for_end();
}

void
run_event(ak_app* a, ak_evt e)
{
  bool consumed = false;
  ak_da_for_begin(
    ak_applayer, &a->layers, i, l);

  if (l->on_event) {
    consumed = l->on_event(l->ctx, e);
  }
  if (consumed) {
    break;
  }

  ak_da_for_end();
}

void
run_update(ak_app* a, ak_dur delta)
{

  ak_da_for_begin(
    ak_applayer, &a->layers, i, l);

  if (l->on_update) {
    l->on_update(l->ctx, delta);
  }

  ak_da_for_end();
}

void
run_upost(ak_app* a, ak_dur delta)
{
  ak_da_for_rev_begin(
    ak_applayer, &a->layers, i, l);

  if (l->on_upost) {
    l->on_upost(l->ctx, delta);
  }

  ak_da_for_end();
}

static void
app_crash(void* ctx)
{
  ak_app* a = ctx;
  run_shutdown(a);
}

//--- public ---//
ak_app*
ak_app_make(ak_applayer_regs* regs,
            ak_alct alct)
{
  ak_app* app =
    ak_alct_alloc(alct, sizeof(ak_app));
  app->alct = alct;

  app->layers =
    ak_da_make(sizeof(ak_applayer), alct);
  app->startuped_count = 0;

  app->eq = ak_app_eq_make(alct);
  app->frametime = ak_dur_from_millis(1);
  app->last_frametime =
    ak_dur_from_millis(0);
  app->should_close = false;

  ak_applayer l = { 0 };
  while (ak_applayer_regs_pop(regs, &l)) {
    ak_da_pushback(&app->layers, &l);
  }

  ak_pgm_crashfn_reg(&app_crash, app);

  return app;
}

void
ak_app_destroy(ak_app* a)
{
  ak_da_destroy(&a->layers);
  ak_alct_free(a->alct, a);
}

void
ak_app_run(ak_app* a)
{
  run_startup(a);

  while (!a->should_close) {
    ak_dur frame_start = ak_dur_now();

    run_epusher(a);

    ak_evt e = ak_app_eq_pop(&a->eq);
    while (e.type != ak_evt_none) {
      // ak_log("event: %d", e.type);
      if (e.type == ak_evt_type_pgm &&
          e.pgm == ak_pgm_exit_req) {
        a->should_close = true;
      }

      run_event(a, e);
      e = ak_app_eq_pop(&a->eq);
    }

    run_update(a, a->last_frametime);
    run_upost(a, a->last_frametime);

    ak_dur frame_end = ak_dur_now();
    ak_dur diff = ak_dur_subtract(
      frame_end, frame_start);
    a->last_frametime = diff;
    ak_dur sleep_time =
      ak_dur_subtract(a->frametime, diff);
    ak_thread_sleep(sleep_time);
    // sleep;
  }

  run_shutdown(a);
}

void
ak_app_close(ak_app* a)
{
  a->should_close = true;
}
