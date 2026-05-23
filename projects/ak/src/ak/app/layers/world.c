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
#include "ak/game/comp_t.h"
#include "ak/game/core.h"
#include "ak/game/stg/world.h"
#include "ak/game/sys/tf.h"
#include "ak/game/world/cb.h"
#include "ak/game/world/cb_itn.h"
#include "ak/game/world/cbflush.h"
#include "ak/game/world/view.h"
#include "ak/game/world/view_itn.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gfx.h"
#include "ak/gfx/gresman.h"
#include "ak/system/render.h"
#include "ak/system/resman.h"
#include "ak/system/resman_itn.h"

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

  ak_thpool* tp;
  ak_resman rm;

  ak_renderer* renderer;

  ak_gfx* gf;
  ak_gresman* grm;

  ak_sys_tf sys_tf;

  ak_resid dog_rid;
  ak_gresid dog_gid;

  ak_ett e_mtrl_base;
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

    ak_renderer_resize(l->renderer, w, h);
    ak_world_cb_flush(
      &l->w, &l->wcb, &l->ig);
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
set_root(ak_lworld* l)
{

  ak_ett root = ak_wv_ett_root(&l->wv);
  ak_tf2d_t tf = ak_tf2d_identity();
  ak_wcb_comp_tf2d_add(&l->wcb, root, tf);
  ak_mat3 mat = ak_mat3_identity();
  ak_wcb_comp_gmat3_add(&l->wcb, root, mat);

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
  base.data.tex =
    (ak_tex){ .gid = l->dog_gid,
              .uv_type = ak_uv_repeat,
              .filter_type =
                ak_filter_linear };
  ak_wcb_comp_mtrl_base_add(
    &l->wcb, l->e_mtrl_base, base);
}

static void
push_child(ak_lworld* l, float x, float y)
{
  ak_ett root = ak_wv_ett_root(&l->wv);
  ak_ett e = ak_wcb_ett_new(&l->wcb, root);

  ak_fx scale_x = ak_fx_i(800);
  ak_fx scale_y = ak_fx_i(600);
  ak_tf2d_t tf = { 0 };
  tf = ak_tf2d_make(
    ak_vec2_make(ak_fx_f(x), ak_fx_f(y)),
    ak_angle_deg(ak_fx_f(0)),
    ak_vec2_make(scale_x, scale_y));
  ak_wcb_comp_tf2d_add(&l->wcb, e, tf);

  ak_mtrl_t mat = { 0 };
  mat.base_id = l->e_mtrl_base;
  mat.data.uv_min =
    ak_vec2_make(ak_fx_f(0), ak_fx_f(0));
  mat.data.uv_max = ak_vec2_make(
    ak_fx_f(1.0f), ak_fx_f(1.0f));
  ak_wcb_comp_mtrl_add(&l->wcb, e, mat);
}

static void
on_startup(void* ctx, ak_app* app)
{
  ak_lworld* l = ctx;
  l->app = app;
  l->ig = ak_idgen_make(l->alct);
  l->w = ak_world_make(ak_idgen_new(&l->ig),
                       l->alct);
  l->wv = ak_wv_make(&l->w);
  l->wcb = ak_wcb_make(&l->ig, l->alct);

  l->tp = ak_thpool_startup();
  l->rm = ak_resman_make(l->tp, l->alct);

  l->renderer = ak_renderer_startup(
    ak_lcore_plat_base(l->lcore),
    &l->wv,
    &l->rm,
    l->alct);

  l->gf = ak_renderer_gfx(l->renderer);
  l->grm = ak_renderer_gresman(l->renderer);

  l->dog_rid = ak_resman_register_img(
    &l->rm, "./dog.png");
  l->dog_gid = ak_gresman_register_img(
    l->grm, l->dog_rid);
  ak_gresman_load(l->grm, l->dog_gid);

  l->sys_tf =
    ak_sys_tf_make(&l->wv, &l->wcb, l->alct);

  set_root(l);
  push_child(l, 0, 0);
  ak_world_cb_flush(&l->w, &l->wcb, &l->ig);
}

static void
on_shutdown(void* ctx)
{
  ak_lworld* l = ctx;

  ak_renderer_shutdown(l->renderer);

  ak_sys_tf_destroy(&l->sys_tf);

  ak_gresman_shutdown(l->grm);
  ak_resman_destroy(&l->rm);
  ak_thpool_shutdown(l->tp);

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
  ak_resman_update(&l->rm);

  ak_renderer_stallwait(l->renderer);

  ak_world_cb_flush(&l->w, &l->wcb, &l->ig);

  ak_sys_tf_update(&l->sys_tf);
  // other world wide system
  // script system
  // physic system
  // ai/path system
}

static void
on_upost(void* ctx, ak_dur delta)
{
  ak_lworld* l = ctx;
  ak_renderer_stallwait(l->renderer);
  ak_world_cb_flush(&l->w, &l->wcb, &l->ig);
  ak_renderer_render(l->renderer);
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
