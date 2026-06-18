#include "ak/core/math/stm.h"
#include "ak/game/comp.h"
#include "ak/game/comp_t.h"
#include <ak/core/math/vec3.h>

//--- private ---//

ak_stmerr
ak_stm_write_qd(ak_stm stm,
                ak_gfx_quaddata qd)
{
  ak_stm_try(
    ak_stm_write_vec4f(stm, qd.col));
  ak_stm_try(
    ak_stm_write_vec2f(stm, qd.uv_min));
  ak_stm_try(
    ak_stm_write_vec2f(stm, qd.uv_max));
  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_read_qd(ak_stm stm,
               ak_gfx_quaddata* o_qd)
{

  ak_stm_try(
    ak_stm_read_vec4f(stm, &o_qd->col));
  ak_stm_try(
    ak_stm_read_vec2f(stm, &o_qd->uv_min));
  ak_stm_try(
    ak_stm_read_vec2f(stm, &o_qd->uv_max));
  return ak_stmerr_ok;
}

//--- internal ---//

ak_stmerr
ak_stm_write_comp_tu(ak_stm stm,
                     ak_comp_tu ctu)
{
  ak_stm_try(ak_stm_write_u32(stm, ctu.ce));
  switch (ctu.ce) {
#define ak_d_comp_x(name)                   \
  case ak_as_comp_e(name): {                \
    ak_as_comp_t(name) name =               \
      *(ak_as_comp_t(                       \
        name)*)ak_comp_tu_comp(&ctu);       \
    return ak_stm_write_##name(stm, name);  \
  }
#include "ak/game/comp.inc"
#undef ak_d_comp_x
    default: {
      return ak_stmerr_err;
    }
  }
}

ak_stmerr
ak_stm_read_comp_tu(ak_stm stm,
                    ak_comp_tu* o_ctu)
{
  ak_stm_try(
    ak_stm_read_u32(stm, &o_ctu->ce));
  switch (o_ctu->ce) {
#define ak_d_comp_x(name)                   \
  case ak_as_comp_e(name): {                \
    ak_as_comp_t(name)* name =              \
      ak_comp_tu_comp(o_ctu);               \
    return ak_stm_read_##name(stm, name);   \
  }
#include "ak/game/comp.inc"
#undef ak_d_comp_x
    default: {
      return ak_stmerr_err;
    }
  }
}

ak_stmerr
ak_stm_write_tf2d(ak_stm stm, ak_tf2d_t tf2d)
{
  ak_stm_try(
    ak_stm_write_vec2(stm, tf2d.pos));

  ak_stm_try(
    ak_stm_write_angle(stm, tf2d.rot));

  ak_stm_try(
    ak_stm_write_vec2(stm, tf2d.scale));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_read_tf2d(ak_stm stm,
                 ak_tf2d_t* o_tf2d)
{
  ak_stm_try(
    ak_stm_read_vec2(stm, &o_tf2d->pos));

  ak_stm_try(
    ak_stm_read_angle(stm, &o_tf2d->rot));

  ak_stm_try(
    ak_stm_read_vec2(stm, &o_tf2d->scale));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_write_gmat3(ak_stm stm,
                   ak_gmat3_t gmat3)
{
  return ak_stm_write_mat3(stm, gmat3);
}

ak_stmerr
ak_stm_read_gmat3(ak_stm stm,
                  ak_gmat3_t* o_gmat3)
{
  return ak_stm_read_mat3(stm, o_gmat3);
}

ak_stmerr
ak_stm_write_camera(ak_stm stm,
                    ak_camera_t camera)
{
  return ak_stm_write_bool(stm, camera.ph);
}

ak_stmerr
ak_stm_read_camera(ak_stm stm,
                   ak_camera_t* o_camera)
{
  return ak_stm_read_bool(stm,
                          &o_camera->ph);
}

ak_stmerr
ak_stm_write_screen(ak_stm stm,
                    ak_screen_t screen)
{
  ak_stm_try(
    ak_stm_write_u32(stm, screen.w));

  ak_stm_try(
    ak_stm_write_u32(stm, screen.h));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_read_screen(ak_stm stm,
                   ak_screen_t* o_screen)
{
  ak_stm_try(
    ak_stm_read_u32(stm, &o_screen->w));

  ak_stm_try(
    ak_stm_read_u32(stm, &o_screen->h));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_write_quadsimple(
  ak_stm stm,
  ak_quadsimple_t quadsimple)
{
  ak_stm_try(ak_stm_write_u32(
    stm, quadsimple.mtrlid));
  ak_stm_try(
    ak_stm_write_qd(stm, quadsimple.data));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_read_quadsimple(
  ak_stm stm,
  ak_quadsimple_t* o_quadsimple)
{
  ak_stm_try(ak_stm_read_u32(
    stm, &o_quadsimple->mtrlid));
  ak_stm_try(ak_stm_read_qd(
    stm, &o_quadsimple->data));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_write_mtrl(ak_stm stm, ak_mtrl_t mtrl)
{
  ak_stm_try(
    ak_stm_write_u32(stm, mtrl.shaderid));
  ak_stm_try(
    ak_stm_write_u32(stm, mtrl.tex));
  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_read_mtrl(ak_stm stm,
                 ak_mtrl_t* o_mtrl)
{

  ak_stm_try(
    ak_stm_read_u32(stm, &o_mtrl->shaderid));
  ak_stm_try(
    ak_stm_read_u32(stm, &o_mtrl->tex));
  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_write_script(ak_stm stm,
                    ak_script_t script)
{
  return ak_stm_write_u32(stm, script.se);
}

ak_stmerr
ak_stm_read_script(ak_stm stm,
                   ak_script_t* o_script)
{
  return ak_stm_read_u32(stm, &o_script->se);
}

ak_stmerr
ak_stm_write_anistate(ak_stm stm,
                      ak_anistate_t anistate)
{
  return ak_stmerr_err;
}

ak_stmerr
ak_stm_read_anistate(
  ak_stm stm,
  ak_anistate_t* o_anistate)
{
  return ak_stmerr_err;
}
