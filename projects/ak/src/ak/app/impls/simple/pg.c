#include "ak/app/impls/simple/dir.h"
#include "ak/game/core.h"
#include "ak/game/sys/script.h"
#include "ak/game/world/cb.h"
#include "ak/game/world/cbflush.h"
#include "ak/gfx/reses/shader.h"
#include "ak/gfx/reses/tex.h"
#include "ak/platform/plat_base.h"
#include "ak/res/reg.h"
#include "ak/res/reses/img.h"
#include "ak/res/reses/shaderstr.h"

void
pg_make(ak_lsimple* l)
{
  uint32_t width = ak_plat_base_width(l->pb);
  uint32_t height =
    ak_plat_base_height(l->pb);

  l->texid = 10;
  l->shaderid = 11;

  ak_img img = { 0 };
  ak_stm stm = { 0 };
  ak_errcode err = 0;

  err =
    ak_stm_open_ast("images/test.png", &stm);
  ak_assert(err == ak_ok);

  err = ak_stm_read_img(stm, &img, l->alct);
  ak_assert(err == ak_ok);
  ak_stm_close(stm);

  err = ak_tex_from_img(l->rctx,
                        &img,
                        ak_textype_rgba8,
                        &l->tex,
                        l->alct);
  ak_assert(err == ak_ok);
  ak_img_destroy(&img);

  ak_shaderstr ss = { 0 };
  err = ak_stm_open_ast("shaders/tex.gles",
                        &stm);
  ak_assert(err == ak_ok);
  err =
    ak_stm_read_shaderstr(stm, &ss, l->alct);
  ak_assert(err == ak_ok);
  ak_stm_close(stm);
  err = ak_shader_from_shaderstr(
    l->rctx, &ss, &l->shader, l->alct);
  ak_assert(err == ak_ok);

  ak_resreg_add(&l->rr, l->texid, l->tex);
  ak_resreg_add(
    &l->rr, l->shaderid, l->shader);

  ak_ett root = ak_wcb_ett_new(&l->wcb, 0);
  ak_wcb_comp_screen_add(
    &l->wcb,
    root,
    (ak_screen_t){
      .w = ak_plat_base_width(l->pb),
      .h = ak_plat_base_height(l->pb) });

  ak_ett cam_e =
    ak_wcb_ett_new(&l->wcb, root);
  ak_wcb_comp_camera_add(
    &l->wcb, cam_e, (ak_camera_t){ 0 });

  ak_ett mtrl_e =
    ak_wcb_ett_new(&l->wcb, root);
  ak_wcb_comp_mtrl_add(
    &l->wcb,
    mtrl_e,
    (ak_mtrl_t){
      .shaderid = l->shaderid,
      .tex = l->texid,
    });

  ak_ett sprite_e =
    ak_wcb_ett_new(&l->wcb, root);
  ak_wcb_comp_tf2d_add(
    &l->wcb,
    sprite_e,
    (ak_tf2d_t){
      .pos =
        ak_vec2_make(ak_fx_f(0), ak_fx_f(0)),
      .rot = ak_angle_rad(ak_fx_f(0)),
      .scale = ak_vec2_make(ak_fx_f(width),
                            ak_fx_f(height)),
    });
  ak_wcb_comp_quadsimple_add(
    &l->wcb,
    sprite_e,
    (ak_quadsimple_t){
      .mtrlid = mtrl_e,
      .data = {
        .uv_min = ak_vec2f_make(0, 0),
        .uv_max = ak_vec2f_make(1, 1),
      } });

  ak_ett s = ak_wcb_ett_new(&l->wcb, root);
  ak_wcb_comp_script_add(
    &l->wcb,
    s,
    (ak_script_t){ .se = ak_script_test_e });

  ak_sys_script_wcb_apply(
    &l->sys_script, &l->w, &l->wcb);
  //  ak_world_cb_flush(&l->w, &l->wcb);
}

void
pg_update(ak_lsimple* l)
{
}

void
pg_destroy(ak_lsimple* l)
{
}
