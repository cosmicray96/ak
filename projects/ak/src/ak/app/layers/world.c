#include "ak/app/layers/world.h"
#include "ak/app/app.h"
#include "ak/app/eq.h"
#include "ak/app/event.h"
#include "ak/game/stg/ettgen.h"
#include "ak/game/stg/world.h"
#include "ak/game/world/cb.h"
#include "ak/game/world/cb_itn.h"
#include "ak/game/world/cbflush.h"
#include "ak/game/world/view_itn.h"

//===== ak_lworld =====//
//--- private ---//
struct ak_lworld
{
  ak_alct alct;
  ak_app* app;
  ak_ettgen eg;
  ak_world w;
  ak_world_v wv;
  ak_world_cb wcb;
};

//--- public ---//
ak_lworld*
ak_lworld_make(ak_alct alct)
{
  ak_lworld* l =
    ak_alct_alloc(alct, sizeof(ak_lworld));
  l->alct = alct;
  return l;
}
void
ak_lworld_destroy(ak_lworld* l)
{
  ak_alct_free(l->alct, l);
}

//===== ak_applayer =====//
//--- private ---//

void
on_startup(void* ctx, ak_app* app)
{
  ak_lworld* l = ctx;
  l->app = app;
  l->eg = ak_ettgen_make(l->alct);
  l->w = ak_world_make(ak_ettgen_new(&l->eg),
                       l->alct);
  l->wv = ak_world_v_make(&l->w);
  l->wcb = ak_world_cb_make(&l->eg, l->alct);
}

void
on_shutdown(void* ctx)
{
  ak_lworld* l = ctx;
}

void
on_epusher(void* ctx, ak_app_eq* eq)
{
  ak_lworld* l = ctx;
}

bool
on_event(void* ctx, ak_evt e)
{
  ak_lworld* l = ctx;

  return false;
}

void
on_update(void* ctx, ak_dur delta)
{
  ak_lworld* l = ctx;
}

void
on_upost(void* ctx, ak_dur delta)
{
  ak_lworld* l = ctx;
  ak_world_cb_flush(&l->w, &l->wcb, &l->eg);
}

//--- public ---//
ak_applayer
ak_lworld_to_applayer(ak_lworld* l)
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
