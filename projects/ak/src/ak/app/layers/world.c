#include "ak/app/layers/world.h"
#include "ak/app/app.h"
#include "ak/app/eq.h"
#include "ak/app/event.h"
#include "ak/app/layers/core.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/tf2d.h"
#include "ak/core/math/trig.h"
#include "ak/core/math/vec2.h"
#include "ak/game/comp_t.h"
#include "ak/game/stg/core.h"
#include "ak/game/stg/ettgen.h"
#include "ak/game/stg/world.h"
#include "ak/game/sys/ren.h"
#include "ak/game/world/cb.h"
#include "ak/game/world/cb_itn.h"
#include "ak/game/world/cbflush.h"
#include "ak/game/world/view.h"
#include "ak/game/world/view_itn.h"

//===== ak_lworld =====//
//--- private ---//
struct ak_lworld
{
  ak_alct alct;
  ak_app* app;

  ak_lcore* lcore;

  ak_ettgen eg;
  ak_world w;
  ak_wv wv;
  ak_wcb wcb;

  ak_sys_ren sys_ren;
};

//--- public ---//
ak_lworld*
ak_lworld_make(ak_lcore* lcore, ak_alct alct)
{
  ak_lworld* l =
    ak_alct_alloc(alct, sizeof(ak_lworld));
  l->alct = alct;
  l->lcore = lcore;
  return l;
}
void
ak_lworld_destroy(ak_lworld* l)
{
  ak_alct_free(l->alct, l);
}

//===== ak_applayer =====//
//--- private ---//
static void
set_root(ak_lworld* l)
{

  ak_ett root = ak_wv_ett_root(&l->wv);
  ak_tf2d_t tf = ak_tf2d_identity();
  ak_wcb_comp_tf2d_add(&l->wcb, root, tf);
}

static void
push_child(ak_lworld* l, float x, float y)
{
  ak_ett root = ak_wv_ett_root(&l->wv);
  ak_ett e = ak_wcb_ett_new(&l->wcb, root);

  ak_tf2d_t tf = { 0 };
  tf = ak_tf2d_make(
    ak_vec2_make(ak_fx32_f(x), ak_fx32_f(y)),
    ak_angle_deg(ak_fx32_f(0)),
    ak_vec2_make(ak_fx32_f(0.1f),
                 ak_fx32_f(0.1f)));
  ak_wcb_comp_tf2d_add(&l->wcb, e, tf);

  ak_rect_t rect = { 0 };
  ak_wcb_comp_rect_add(&l->wcb, e, rect);
}

static void
on_startup(void* ctx, ak_app* app)
{
  ak_lworld* l = ctx;
  l->app = app;
  l->eg = ak_ettgen_make(l->alct);
  l->w = ak_world_make(ak_ettgen_new(&l->eg),
                       l->alct);
  l->wv = ak_wv_make(&l->w);
  l->wcb = ak_wcb_make(&l->eg, l->alct);

  l->sys_ren = ak_sys_ren_make(
    ak_lcore_gfx(l->lcore), &l->wv, l->alct);

  set_root(l);

  push_child(l, 0, 0);
  push_child(l, 0.5f, 0);
  push_child(l, 0.5f, 0.5f);
  push_child(l, 0, 0.5f);
  push_child(l, -0.5f, 0);
  push_child(l, -0.5f, -0.5f);
  push_child(l, 0, -0.5f);
  push_child(l, 0.5f, -0.5f);
  push_child(l, -0.5f, 0.5f);
}

static void
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

static void
on_update(void* ctx, ak_dur delta)
{
  ak_lworld* l = ctx;
  ak_sys_ren_render(&l->sys_ren);
}

static void
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
