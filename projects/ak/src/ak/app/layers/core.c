#include "ak/app/layers/core.h"
#include "ak/app/app.h"
#include "ak/app/eq.h"
#include "ak/app/event.h"
#include "ak/os/time.h"

//===== ak_lcore =====//
//--- private ---//
struct ak_lcore
{
  ak_alct alct;
  ak_app* app;
};

//--- public ---//
ak_lcore*
ak_lcore_make(ak_alct alct)
{
  ak_lcore* l =
    ak_alct_alloc(alct, sizeof(ak_lcore));
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
}
void
on_shutdown(void* ctx)
{
  ak_lcore* l = ctx;
}
void
on_epusher(void* ctx, ak_app_eq* eq)
{
  ak_lcore* l = ctx;
}
bool
on_event(void* ctx, ak_evt e)
{
  ak_lcore* l = ctx;
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
