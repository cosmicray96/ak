#include "ak/app/layers/simple.h"
#include "ak/app/app.h"
#include "ak/app/eq.h"
#include "ak/app/event.h"
#include "ak/app/layers/core.h"
#include "ak/core/async/thpool.h"
#include "ak/core/async/thpool_itn.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/tf2d.h"
#include "ak/core/math/vec2.h"
#include "ak/game/comp_t.h"
#include "ak/game/core.h"
#include "ak/game/stg/world.h"
#include "ak/game/sys/ani.h"
#include "ak/game/sys/ren.h"
#include "ak/game/sys/script.h"
#include "ak/game/sys/tf.h"
#include "ak/game/world/cb.h"
#include "ak/game/world/cb_itn.h"
#include "ak/game/world/cbflush.h"
#include "ak/game/world/view.h"
#include "ak/game/world/view_itn.h"
#include "ak/gfx/gcb.h"
#include "ak/gfx/gresman.h"
#include "ak/gfx/gresreg.h"
#include "ak/os/time.h"
#include "ak/res/assetman.h"
#include "ak/res/reg.h"
#include "ak/res/resman.h"
#include "ak/res/resman_itn.h"
#include "ak/system/idgen.h"
#include "ak/system/render.h"

#include <stdbool.h>

//===== ak_lsimple =====//
//--- private ---//

struct ak_lsimple
{
  ak_alct alct;
  ak_app* app;

  ak_lcore* lcore;

  ak_idgen ig;
  ak_world w;
  ak_wv wv;
  ak_wcb wcb;
  ak_wcb wcb_script;

  ak_thpool* tp;
  ak_resreg* rr;
  ak_resman rm;
  ak_gresreg* grr;
  ak_gresman* grm;
  ak_assetman am;

  ak_gcb gcb;
  ak_renderer* renderer;

  ak_sys_tf sys_tf;
  ak_sys_ren sys_ren;
  ak_sys_script sys_script;
  ak_sys_ani sys_ani;
};

static bool
on_resize(ak_lsimple* l, ak_evt e)
{
  if (e.type == ak_evt_type_win &&
      e.win.type == ak_winevt_resize) {
    uint32_t w = e.win.resize.w;
    uint32_t h = e.win.resize.h;

    ak_ett root = ak_wv_ett_root(&l->wv);
    ak_wcb_comp_screen_add(
      &l->wcb,
      root,
      (ak_screen_t){ .w = w, .h = h });
    ak_world_cb_flush(
      &l->w, &l->wcb, &l->ig);

    ak_gcb_push_resize(&l->gcb, w, h);
  }
  return false;
}

//--- public ---//
ak_lsimple*
ak_lsimple_make(ak_lcore* lcore,
                ak_alct alct)
{
  ak_lsimple* l =
    ak_alct_alloc(alct, sizeof(ak_lsimple));
  l->alct = alct;
  l->lcore = lcore;
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
set_root(ak_lsimple* l)
{
  ak_ett root = ak_wcb_ett_new(&l->wcb, 0);

  ak_screen_t screen = { .w = 800,
                         .h = 600 };
  ak_wcb_comp_screen_add(
    &l->wcb, root, screen);

  ak_ett e_cam =
    ak_wcb_ett_new(&l->wcb, root);
  ak_camera_t cam = { 0 };
  ak_wcb_comp_camera_add(
    &l->wcb, e_cam, cam);

  ak_ett script_test =
    ak_wcb_ett_new(&l->wcb, root);
  ak_wcb_comp_script_add(
    &l->wcb,
    script_test,
    (ak_script_t){ .se = ak_script_test_e });
}

static void
on_startup(void* ctx, ak_app* app)
{
  ak_lsimple* l = ctx;
  l->app = app;
  l->ig = ak_idgen_make(l->alct);
  l->w = ak_world_make(l->alct);
  l->wv = ak_wv_make(&l->w);
  l->wcb = ak_wcb_make(&l->ig, l->alct);

  l->wcb_script =
    ak_wcb_make(&l->ig, l->alct);

  l->rr = ak_resreg_make(l->alct);
  l->tp = ak_thpool_startup();
  l->rm =
    ak_resman_make(l->rr, l->tp, l->alct);

  l->gcb = ak_gcb_make(l->alct);

  l->renderer = ak_renderer_startup(
    ak_lcore_plat_base(l->lcore), l->alct);

  l->grm =
    ak_renderer_gresman_get(l->renderer);
  l->grr =
    ak_renderer_gresreg_get(l->renderer);

  l->am = ak_assetman_make(
    l->tp, &l->rm, l->grm, l->alct);

  l->sys_tf = ak_sys_tf_make(l->alct);
  l->sys_ren = ak_sys_ren_make(l->alct);
  l->sys_script =
    ak_sys_script_make(l->alct);
  l->sys_ani = ak_sys_ani_make(l->alct);

  set_root(l);

  ak_sys_script_set(
    &l->sys_script, &l->wv, &l->wcb);
  ak_sys_script_run_deinit(&l->sys_script,
                           &l->wcb_script);
  ak_world_cb_flush(&l->w, &l->wcb, &l->ig);
  ak_sys_script_run_init(&l->sys_script,
                         &l->wcb_script);
  ak_world_cb_flush(
    &l->w, &l->wcb_script, &l->ig);
}

static void
on_shutdown(void* ctx)
{
  ak_lsimple* l = ctx;

  ak_sys_script_run_shutdown(&l->sys_script,
                             &l->wcb_script);
  ak_world_cb_flush(
    &l->w, &l->wcb_script, &l->ig);

  ak_sys_ani_destroy(&l->sys_ani);
  ak_sys_script_destroy(&l->sys_script);
  ak_sys_ren_destroy(&l->sys_ren);
  ak_sys_tf_destroy(&l->sys_tf);

  ak_thpool_destroy_jobs(l->tp);

  ak_renderer_shutdown(l->renderer);

  ak_assetman_destroy(&l->am);
  ak_resman_destroy(&l->rm);
  ak_thpool_shutdown(l->tp);

  ak_resreg_destroy(l->rr);

  ak_wcb_destroy(&l->wcb_script);

  ak_wcb_destroy(&l->wcb);
  ak_wv_destroy(&l->wv);
  ak_world_destroy(&l->w);
  ak_idgen_destroy(&l->ig);
}

static void
on_epusher(void* ctx, ak_app_eq* eq)
{
  ak_lsimple* l = ctx;
}

static bool
on_event(void* ctx, ak_evt e)
{
  ak_lsimple* l = ctx;

  if (on_resize(l, e)) {
    return true;
  }

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

  ak_ett root = ak_wv_ett_root(&l->wv);
  ak_tf2 tf = ak_wv_comp_tf2d(&l->wv, root);
  tf.pos.x =
    ak_fxadd(tf.pos.x, ak_fx_f(0.2f));
  ak_wcb_comp_tf2d_add(&l->wcb, root, tf);

  return false;
}

static void
script_flush(ak_lsimple* l)
{
  ak_wcb* wcb = &l->wcb;
  ak_wcb* out_wcb = &l->wcb_script;

  ak_sys_script_set(
    &l->sys_script, &l->wv, wcb);

  ak_sys_script_run_deinit(&l->sys_script,
                           out_wcb);
  ak_world_cb_flush(&l->w, wcb, &l->ig);
  ak_sys_script_run_init(&l->sys_script,
                         out_wcb);

  ak_world_cb_flush(&l->w, out_wcb, &l->ig);
}

static void
on_update(void* ctx, ak_dur delta)
{
  ak_lsimple* l = ctx;

  ak_resman_update(&l->rm);

  ak_sys_tf_update(
    &l->sys_tf, &l->wv, &l->wcb);
  script_flush(l);

  ak_sys_script_run_update(
    &l->sys_script, delta, &l->wcb);
  script_flush(l);

  ak_sys_ani_update(&l->sys_ani,
                    &l->wv,
                    &l->wcb,
                    l->rr,
                    delta);
  script_flush(l);
  ak_sys_ren_render(
    &l->sys_ren, &l->wv, &l->gcb, delta);
  //  ui_render(l);

  //  ak_renderer_render(l->renderer,
  //  &l->gcb);
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
