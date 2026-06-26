#include "ak/app/layers/simple.h"
#include "ak/app/app.h"
#include "ak/app/eq.h"
#include "ak/app/event.h"
#include "ak/os/time.h"
#include "ak/platform/plat_base.h"

#include <stdbool.h>

//===== ak_lsimple =====//
//--- private ---//

struct ak_lsimple
{
  ak_alct alct;
  ak_app* app;

  ak_plat_base* pb;
};

//--- public ---//
ak_lsimple*
ak_lsimple_make(ak_alct alct)
{
  ak_lsimple* l =
    ak_alct_alloc(alct, sizeof(ak_lsimple));
  l->alct = alct;
  return l;
}
void
ak_lsimple_destroy(ak_lsimple* l)
{
  ak_alct_free(l->alct, l);
}

//===== ak_applayer =====//
//--- private ---//

static void
on_startup(void* ctx, ak_app* app)
{
  ak_lsimple* l = ctx;
  l->app = app;

  l->pb = ak_plat_base_startup(l->alct);
}

static void
on_shutdown(void* ctx)
{
  ak_lsimple* l = ctx;
  ak_plat_base_shutdown(l->pb);
}

static void
on_epusher(void* ctx, ak_app_eq* eq)
{
  ak_lsimple* l = ctx;
  ak_plat_base_eventflush(l->pb, eq);
}

static bool
on_event(void* ctx, ak_evt e)
{
  ak_lsimple* l = ctx;

  if (e.type != ak_evt_type_win) {
    return false;
  }
  if (e.win.type != ak_winevt_key) {
    return false;
  }
  if (e.win.key.code != ak_key_a) {
    return false;
  }
  if (e.win.key.action !=
      ak_keyaction_pressed) {
    return false;
  }
  ak_app_close(l->app);

  return false;
}

static void
on_update(void* ctx, ak_dur delta)
{
  ak_lsimple* l = ctx;
}

static void
on_upost(void* ctx, ak_dur delta)
{
  ak_lsimple* l = ctx;
}

//--- public ---//
ak_applayer
ak_lsimple_to_applayer(ak_lsimple* l)
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
