#include "ak/app/layers/core.h"
#include "ak/app/app.h"
#include "ak/app/eq.h"
#include "ak/app/event.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/trig.h"
#include "ak/gfx/mtrl/vcol.h"
#include "ak/os/time.h"
#include "ak/platform/core.h"
#include "ak/platform/plat_ren.h"
#include "ak/program/core.h"
#include "ak/program/event.h"

#include "ak/gfx/core.h"
#include "ak/gfx/gfx.h"
#include "ak/platform/plat.h"

//===== ak_lcore =====//
//--- private ---//
struct ak_lcore
{
  ak_alct alct;
  ak_app* app;
  ak_app_eq* eq;
  ak_plat_ren* pr;
  ak_plat* p;
  ak_gfx* gf;
  ak_mtrl_vcol* mtrl;

  bool flip;
};

bool
on_resize(ak_lcore* l, ak_evt e)
{
  if (e.type != ak_evt_type_win) {
    return false;
  }
  if (e.win.type != ak_winevt_resize) {
    return false;
  }
  ak_gfx_resize(
    l->gf, e.win.resize.w, e.win.resize.h);
  return true;
}

bool
on_win_close(ak_lcore* l, ak_evt e)
{
  if (e.type != ak_evt_type_win) {
    return false;
  }
  if (e.win.type != ak_winevt_close) {
    return false;
  }
  ak_app_close(l->app);
  return true;
}

//--- public ---//
ak_lcore*
ak_lcore_make(ak_alct alct)
{
  ak_lcore* l =
    ak_alct_alloc(alct, sizeof(ak_lcore));
  l->alct = alct;
  return l;
}
void
ak_lcore_destroy(ak_lcore* l)
{
  ak_alct_free(l->alct, l);
}

//===== ak_applayer =====//
//--- private ---//

void
on_startup(void* ctx, ak_app* app)
{
  ak_lcore* l = ctx;
  l->app = app;
  l->eq = 0;
  l->pr = ak_plat_ren_startup(l->alct);
  l->p = ak_plat_startup(l->pr, l->alct);
  l->gf = ak_gfx_startup(l->pr, l->alct);

  l->flip = false;
}
void
on_shutdown(void* ctx)
{
  ak_lcore* l = ctx;
  ak_gfx_shutdown(l->gf);
  ak_plat_shutdown(l->p);
  ak_plat_ren_shutdown(l->pr);
}

void
on_epusher(void* ctx, ak_app_eq* eq)
{
  ak_lcore* l = ctx;
  if (!l->eq) {
    l->eq = eq;
  }

  ak_pgmevt pgmevt = ak_pgm_event_pop();
  while (pgmevt != ak_pgm_none) {
    ak_evt e = { 0 };
    e.type = ak_evt_type_pgm;
    e.pgm = pgmevt;
    ak_app_eq_push(eq, e);

    pgmevt = ak_pgm_event_pop();
  }

  ak_plat_eventflush(l->p, eq);
}

bool
on_event(void* ctx, ak_evt e)
{
  ak_lcore* l = ctx;

  if (on_win_close(l, e)) {
    return true;
  }

  if (on_resize(l, e)) {
    return true;
  }

  if (e.type != ak_evt_type_win) {
    return false;
  }
  if (e.win.type != ak_winevt_key) {
    return false;
  }
  if (e.win.key.code != ak_key_f) {
    return false;
  }
  if (e.win.key.action !=
      ak_keyaction_pressed) {
    return false;
  }
  {
    ak_evt e = { 0 };
    e.type = ak_evt_type_win;
    e.win.type = ak_winevt_resize;
    e.win.resize.w = l->flip ? 800 : 400;
    e.win.resize.h = l->flip ? 600 : 300;
    l->flip = !l->flip;

    ak_app_eq_push(l->eq, e);
  }

  /*
ak_log("key: %d", e.win.key.code);
ak_log("mode: %d", e.win.key.mode);
ak_log("action: %s",
   e.win.key.action ==
       ak_keyaction_pressed
     ? "pressed"
     : "released");
  */

  return false;
}

void
on_update(void* ctx, ak_dur delta)
{
  ak_lcore* l = ctx;

  float speed = 2;
  float t = ak_dur_as_secs_f(ak_dur_now());
  float col =
    ak_fx32_to_f(ak_fx32_abs(ak_sin(
      ak_angle_rad(ak_fx32_f(t * speed)))));
  ak_gfx_col_set(l->gf, col);

  ak_gfx_call_end(l->gf);
  ak_plat_ren_swapbuffer(l->pr);
}

void
on_upost(void* ctx, ak_dur delta)
{
  ak_lcore* l = ctx;
}

//--- public ---//
ak_applayer
ak_lcore_to_applayer(ak_lcore* l)
{
  ak_applayer appl = { 0 };
  appl.ctx = l;
  appl.on_startup = &on_startup;
  appl.on_shutdown = &on_shutdown;
  appl.on_epusher = &on_epusher;
  appl.on_event = &on_event;
  appl.on_update = &on_update;
  appl.on_upost = &on_upost;
  return appl;
}
