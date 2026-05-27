#include "ak/core/math/fixed.h"
#include "ak/debug.h"
#include "ak/game/comp.h"
#include "ak/game/comp_t.h"
#include <ak/core/math/vec3.h>

//--- private ---//

ak_stmerr
ak_stm_write_angle(ak_stm stm,
                   ak_angle angle)
{
  return ak_stm_write_u16(stm,
                          (uint16_t)angle);
}

ak_stmerr
ak_stm_read_angle(ak_stm stm,
                  ak_angle* o_angle)
{
  uint16_t value;

  ak_stm_try(ak_stm_read_u16(stm, &value));

  *o_angle = (ak_angle)value;

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_write_vec2(ak_stm stm, ak_vec2 vec2)
{
  ak_stm_try(ak_stm_write_fx(stm, vec2.x));

  ak_stm_try(ak_stm_write_fx(stm, vec2.y));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_read_vec2(ak_stm stm, ak_vec2* o_vec2)
{
  ak_stm_try(
    ak_stm_read_fx(stm, &o_vec2->x));

  ak_stm_try(
    ak_stm_read_fx(stm, &o_vec2->y));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_write_vec3(ak_stm stm, ak_vec3 vec3)
{
  ak_stm_try(ak_stm_write_fx(stm, vec3.x));

  ak_stm_try(ak_stm_write_fx(stm, vec3.y));

  ak_stm_try(ak_stm_write_fx(stm, vec3.z));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_read_vec3(ak_stm stm, ak_vec3* o_vec3)
{
  ak_stm_try(
    ak_stm_read_fx(stm, &o_vec3->x));

  ak_stm_try(
    ak_stm_read_fx(stm, &o_vec3->y));

  ak_stm_try(
    ak_stm_read_fx(stm, &o_vec3->z));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_write_mat3(ak_stm stm, ak_mat3 mat3)
{
  for (uint32_t i = 0; i < 9; ++i) {
    ak_stm_try(
      ak_stm_write_fx(stm, mat3.v[i]));
  }

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_read_mat3(ak_stm stm, ak_mat3* o_mat3)
{
  for (uint32_t i = 0; i < 9; ++i) {
    ak_stm_try(
      ak_stm_read_fx(stm, &o_mat3->v[i]));
  }

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
ak_stm_write_mtrl(ak_stm stm, ak_mtrl_t mtrl)
{
  ak_assert(false);

  return ak_stm_write_u32(stm, mtrl.base_id);
}

ak_stmerr
ak_stm_read_mtrl(ak_stm stm,
                 ak_mtrl_t* o_mtrl)
{
  ak_assert(false);

  return ak_stm_read_u32(stm,
                         &o_mtrl->base_id);
}

ak_stmerr
ak_stm_write_mtrl_base(
  ak_stm stm,
  ak_mtrl_base_t mtrl_base)
{
  ak_assert(false);

  return ak_stm_write_u32(stm,
                          mtrl_base.data.me);
}

ak_stmerr
ak_stm_read_mtrl_base(
  ak_stm stm,
  ak_mtrl_base_t* o_mtrl_base)
{
  ak_assert(false);

  return ak_stm_read_u32(
    stm, &o_mtrl_base->data.me);
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
