#include "ak/app/layers/core.h"
#include "ak/app/app.h"
#include "ak/app/eq.h"
#include "ak/app/event.h"
#include "ak/debug.h"
#include "ak/os/time.h"
#include "ak/platform/core.h"
#include "ak/program/core.h"
#include "ak/program/event.h"

#include "ak/platform/plat.h"

//===== ak_lcore =====//
//--- private ---//
struct ak_lcore
{
  ak_alct alct;
  ak_app* app;
  ak_plat* p;
  ak_window w;
  ak_window w2;
};

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
  l->p = ak_plat_startup(l->alct);
  l->w = ak_plat_win_make(l->p);
  l->w2 = ak_plat_win_make(l->p);
}
void
on_shutdown(void* ctx)
{
  ak_lcore* l = ctx;
  ak_plat_win_destroy(l->p, l->w);
  ak_plat_win_destroy(l->p, l->w2);
  ak_plat_shutdown(l->p);
}

void
on_epusher(void* ctx, ak_app_eq* eq)
{

  ak_lcore* l = ctx;
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

  if (e.type != ak_evt_type_win) {
    return false;
  }
  if (e.win.type != ak_winevt_key) {
    return false;
  }
  ak_log("key: %d", e.win.key.code);
  ak_log("mode: %d", e.win.key.mode);
  ak_log("action: %s",
         e.win.key.action ==
             ak_keyaction_pressed
           ? "pressed"
           : "released");
  ak_log("window: %d", e.win.w);

  return false;
}

void
on_update(void* ctx, ak_dur delta)
{
  ak_lcore* l = ctx;
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
