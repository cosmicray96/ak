#include "ak/core/math/stm.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/math/trig.h"
#include "ak/core/math/vec2f.h"
#include "ak/core/math/vec3.h"
#include "ak/core/math/vec3f.h"
#include "ak/core/math/vec4.h"

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
ak_stm_write_vec4(ak_stm stm, ak_vec4 vec4)
{
  ak_stm_try(ak_stm_write_fx(stm, vec4.x));
  ak_stm_try(ak_stm_write_fx(stm, vec4.y));
  ak_stm_try(ak_stm_write_fx(stm, vec4.z));
  ak_stm_try(ak_stm_write_fx(stm, vec4.w));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_read_vec4(ak_stm stm, ak_vec4* o_vec4)
{
  ak_stm_try(
    ak_stm_read_fx(stm, &o_vec4->x));
  ak_stm_try(
    ak_stm_read_fx(stm, &o_vec4->y));
  ak_stm_try(
    ak_stm_read_fx(stm, &o_vec4->z));
  ak_stm_try(
    ak_stm_read_fx(stm, &o_vec4->w));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_write_vec2f(ak_stm stm,
                   ak_vec2f vec2f)
{
  ak_stm_try(ak_stm_write_f32(stm, vec2f.x));

  ak_stm_try(ak_stm_write_f32(stm, vec2f.y));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_read_vec2f(ak_stm stm,
                  ak_vec2f* o_vec2f)
{
  ak_stm_try(
    ak_stm_read_f32(stm, &o_vec2f->x));

  ak_stm_try(
    ak_stm_read_f32(stm, &o_vec2f->y));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_write_vec3f(ak_stm stm,
                   ak_vec3f vec3f)
{
  ak_stm_try(ak_stm_write_f32(stm, vec3f.x));
  ak_stm_try(ak_stm_write_f32(stm, vec3f.y));
  ak_stm_try(ak_stm_write_f32(stm, vec3f.z));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_read_vec3f(ak_stm stm,
                  ak_vec3f* o_vec3f)
{
  ak_stm_try(
    ak_stm_read_f32(stm, &o_vec3f->x));
  ak_stm_try(
    ak_stm_read_f32(stm, &o_vec3f->y));
  ak_stm_try(
    ak_stm_read_f32(stm, &o_vec3f->z));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_write_vec4f(ak_stm stm,
                   ak_vec4f vec4f)
{
  ak_stm_try(ak_stm_write_f32(stm, vec4f.x));
  ak_stm_try(ak_stm_write_f32(stm, vec4f.y));
  ak_stm_try(ak_stm_write_f32(stm, vec4f.z));
  ak_stm_try(ak_stm_write_f32(stm, vec4f.w));

  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_read_vec4f(ak_stm stm,
                  ak_vec4f* o_vec4f)
{
  ak_stm_try(
    ak_stm_read_f32(stm, &o_vec4f->x));
  ak_stm_try(
    ak_stm_read_f32(stm, &o_vec4f->y));
  ak_stm_try(
    ak_stm_read_f32(stm, &o_vec4f->z));
  ak_stm_try(
    ak_stm_read_f32(stm, &o_vec4f->w));

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
