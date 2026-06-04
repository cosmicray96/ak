#include "ak/app/layers/world.h"
#include "ak/app/app.h"
#include "ak/app/eq.h"
#include "ak/app/event.h"
#include "ak/app/layers/core.h"
#include "ak/core/async/thpool.h"
#include "ak/core/async/thpool_itn.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/tf2d.h"
#include "ak/core/math/trig.h"
#include "ak/core/math/vec2.h"
#include "ak/debug.h"
#include "ak/game/comp_t.h"
#include "ak/game/core.h"
#include "ak/game/stg/world.h"
#include "ak/game/sys/ren.h"
#include "ak/game/sys/script.h"
#include "ak/game/sys/tf.h"
#include "ak/game/world/cb.h"
#include "ak/game/world/cb_itn.h"
#include "ak/game/world/cbflush.h"
#include "ak/game/world/view.h"
#include "ak/game/world/view_itn.h"
#include "ak/game/world/write.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gcb.h"
#include "ak/gfx/gresman.h"
#include "ak/os/time.h"
#include "ak/system/idgen.h"
#include "ak/system/render.h"
#include "ak/system/resman.h"
#include "ak/system/resman_itn.h"
#include "ak/ui/ui.h"
#include <stdbool.h>

//===== ak_lworld =====//
//--- private ---//

struct ak_lworld
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
  ak_resman rm;

  ak_gcb gcb;
  ak_renderer* renderer;

  ak_gresman* grm;

  ak_sys_tf sys_tf;
  ak_sys_ren sys_ren;
  ak_sys_script sys_script;

  ak_resid dog_rid;
  ak_gresid dog_gid;

  ak_ett e_mtrl_base;

  bool load;
  bool world_added;
  ak_resid wid;
  ak_world w_store;

  ak_ui ui;
  float time;
};

bool
on_resize(ak_lworld* l, ak_evt e)
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
push_child2(ak_wv* wv,
            ak_wcb* wcb,
            ak_ett mtrl_id,
            float x,
            float y)
{
  float size = 50;
  float space = size + 5;

  ak_ett root = ak_wv_ett_root(wv);
  ak_ett e = ak_wcb_ett_new(wcb, root);

  ak_tf2d_t tf = { 0 };
  tf = ak_tf2d_make(
    ak_vec2_make(ak_fx_f(x * space),
                 ak_fx_f(y * space)),
    ak_angle_deg(ak_fx_f(0)),
    ak_vec2_make(ak_fx_f(size),
                 ak_fx_f(size)));
  ak_wcb_comp_tf2d_add(wcb, e, tf);

  ak_mtrl_t mat = { 0 };
  mat.base_id = mtrl_id;
  mat.data.tex.uv_min =
    ak_vec2_make(ak_fx_f(0), ak_fx_f(0));
  mat.data.tex.uv_max = ak_vec2_make(
    ak_fx_f(1.0f), ak_fx_f(1.0f));
  ak_wcb_comp_mtrl_add(wcb, e, mat);
}

static void
store(ak_lworld* l)
{
  l->w_store = ak_world_make(l->alct);
  ak_idgen ig = ak_idgen_make(l->alct);
  ak_wv wv = ak_wv_make(&l->w_store);
  ak_wcb wcb = ak_wcb_make(&ig, l->alct);

  ak_wcb_ett_new(&wcb, 0);
  ak_world_cb_flush(&l->w_store, &wcb, &ig);

  for (int32_t y = -5; y < 6; y++) {
    for (int32_t x = -5; x < 6; x++) {
      push_child2(
        &wv, &wcb, l->e_mtrl_base, x, y);
    }
  }
  ak_world_cb_flush(&l->w_store, &wcb, &ig);

  ak_stm stm =
    ak_stm_open_file("./world.bin", "wb");
  ak_stmerr err = ak_stream_write_world(
    stm,
    &l->w_store,
    ak_world_ett_root(&l->w_store),
    l->alct);
  ak_assert(err == ak_stmerr_ok);
  ak_stm_close(stm);

  ak_wcb_destroy(&wcb);
  ak_wv_destroy(&wv);
  ak_idgen_destroy(&ig);
  ak_world_destroy(&l->w_store);

  ak_log("Done!");
  ak_app_close(l->app);
}

static void
set_root(ak_lworld* l)
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

  l->e_mtrl_base =
    ak_wcb_ett_new(&l->wcb, root);
  ak_mtrl_base_t base = { 0 };
  base.data.me = ak_mtrl_tex_e;
  base.data.tex.tex =
    (ak_tex){ .gid = l->dog_gid,
              .uv_type = ak_uv_repeat,
              .filter_type =
                ak_filter_linear };
  ak_wcb_comp_mtrl_base_add(
    &l->wcb, l->e_mtrl_base, base);

  ak_ett script_test =
    ak_wcb_ett_new(&l->wcb, root);
  ak_wcb_comp_script_add(
    &l->wcb,
    script_test,
    (ak_script_t){ .se = ak_script_test_e });
}

static void
ui_render(ak_lworld* l)
{
  ak_ui_clear(&l->ui);
  ak_ui_add(
    &l->ui,
    ak_ui_root(&l->ui),
    (ak_cnst){ .rel = 0.5f, .abs = 0 },
    (ak_cnst){ .rel = 0.5f, .abs = 0 },
    true);
  ak_ui_add(&l->ui,
            ak_ui_root(&l->ui),
            (ak_cnst){ .rel = 0, .abs = 0 },
            (ak_cnst){ .rel = 0,
                       .abs = l->time * 30 },
            true);
  ak_ui_set(&l->ui, 0, 0, 800, 600);
  ak_ui_render(&l->ui, &l->gcb);
}

static void
on_startup(void* ctx, ak_app* app)
{
  ak_lworld* l = ctx;
  l->app = app;
  l->ig = ak_idgen_make(l->alct);
  l->w = ak_world_make(l->alct);
  l->wv = ak_wv_make(&l->w);
  l->wcb = ak_wcb_make(&l->ig, l->alct);

  l->wcb_script =
    ak_wcb_make(&l->ig, l->alct);

  l->tp = ak_thpool_startup();
  l->rm = ak_resman_make(l->tp, l->alct);

  l->gcb = ak_gcb_make(l->alct);

  l->renderer = ak_renderer_startup(
    ak_lcore_plat_base(l->lcore),
    &l->rm,
    l->alct);

  l->grm =
    ak_renderer_gresman_get(l->renderer);

  l->load = true;
  l->world_added = false;

  l->wid = 5;
  ak_resman_register_world(
    &l->rm, l->wid, "./world.bin");

  l->dog_rid = 10;
  ak_resman_register_img(
    &l->rm, l->dog_rid, "./dog.png");
  l->dog_gid = 11;
  ak_gresman_register_tex_from_rid(
    l->grm, l->dog_gid, l->dog_rid);
  ak_gresman_load(l->grm, l->dog_gid);

  l->sys_tf = ak_sys_tf_make(l->alct);
  l->sys_ren = ak_sys_ren_make(l->alct);
  l->sys_script =
    ak_sys_script_make(l->alct);

  if (l->load) {
    ak_resman_load(&l->rm, l->wid);
  } else {
    store(l);
  }

  l->ui = ak_ui_make(l->alct);

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

  l->time = 0;
}

static void
on_shutdown(void* ctx)
{
  ak_lworld* l = ctx;

  ak_ui_destroy(&l->ui);

  ak_sys_script_run_shutdown(&l->sys_script,
                             &l->wcb_script);
  ak_world_cb_flush(
    &l->w, &l->wcb_script, &l->ig);

  ak_renderer_shutdown(l->renderer);

  ak_sys_script_destroy(&l->sys_script);
  ak_sys_ren_destroy(&l->sys_ren);
  ak_sys_tf_destroy(&l->sys_tf);

  ak_gresman_shutdown(l->grm);
  ak_resman_destroy(&l->rm);
  ak_thpool_shutdown(l->tp);

  ak_wcb_destroy(&l->wcb_script);

  ak_wcb_destroy(&l->wcb);
  ak_wv_destroy(&l->wv);
  ak_world_destroy(&l->w);
  ak_idgen_destroy(&l->ig);
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
on_update(void* ctx, ak_dur delta)
{
  ak_lworld* l = ctx;

  l->time += ak_dur_as_secs_f(delta);

  ak_resman_update(&l->rm);

  if (l->load && !l->world_added &&
      ak_resman_status(&l->rm, l->wid) ==
        ak_res_loaded) {
    ak_world w = { 0 };
    ak_stmerr err = { 0 };
    bool success = ak_resman_acquire_world(
      &l->rm, l->wid, &w, &err);
    ak_assert(success);
    ak_assert(err == ak_stmerr_ok);
    ak_world_graft(&l->w,
                   &w,
                   ak_world_ett_root(&l->w),
                   &l->ig,
                   l->alct);
    l->world_added = true;
    ak_resman_release(&l->rm, l->wid);
    ak_log("Loaded");
  }

  if (l->load && l->world_added) {
    ak_sys_tf_update(
      &l->sys_tf, &l->wv, &l->wcb);

    ak_sys_script_set(
      &l->sys_script, &l->wv, &l->wcb);
    ak_sys_script_run_deinit(&l->sys_script,
                             &l->wcb_script);
    ak_world_cb_flush(
      &l->w, &l->wcb, &l->ig);
    ak_sys_script_run_init(&l->sys_script,
                           &l->wcb_script);
    ak_world_cb_flush(
      &l->w, &l->wcb_script, &l->ig);

    ak_sys_script_run_update(
      &l->sys_script, delta, &l->wcb_script);

    ak_sys_script_set(&l->sys_script,
                      &l->wv,
                      &l->wcb_script);
    ak_sys_script_run_deinit(&l->sys_script,
                             &l->wcb);
    ak_world_cb_flush(
      &l->w, &l->wcb_script, &l->ig);
    ak_sys_script_run_init(&l->sys_script,
                           &l->wcb);
    ak_world_cb_flush(
      &l->w, &l->wcb, &l->ig);

    if (ak_gresman_status(l->grm,
                          l->dog_gid) ==
        ak_gres_loaded) {
      ak_sys_ren_render(
        &l->sys_ren, &l->wv, &l->gcb, delta);
    }
  }

  ui_render(l);
  ak_renderer_render(l->renderer, &l->gcb);
}

static void
on_upost(void* ctx, ak_dur delta)
{
  ak_lworld* l = ctx;
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
