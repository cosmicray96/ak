#include "ak/core/io.h"
#include "ak/core/math/fixed.h"
#include "ak/debug.h"
#include "ak/game/comp.h"
#include "ak/game/comp_t.h"
#include <ak/core/math/vec3.h>

//--- private ---//
void
ak_write_angle(ak_iostream io,
               ak_angle angle)
{
  ak_write_u16(io, angle);
}
ak_angle
ak_read_angle(ak_iostream io)
{
  return ak_read_u16(io);
}

void
ak_write_vec2(ak_iostream io, ak_vec2 vec2)
{
  ak_write_fx(io, vec2.x);
  ak_write_fx(io, vec2.y);
}
ak_vec2
ak_read_vec2(ak_iostream io)
{
  ak_vec2 v = { 0 };
  v.x = ak_read_fx(io);
  v.y = ak_read_fx(io);
  return v;
}

void
ak_write_vec3(ak_iostream io, ak_vec3 vec3)
{
  ak_write_fx(io, vec3.x);
  ak_write_fx(io, vec3.y);
  ak_write_fx(io, vec3.z);
}
ak_vec3
ak_read_vec3(ak_iostream io)
{
  ak_vec3 v = { 0 };
  v.x = ak_read_fx(io);
  v.y = ak_read_fx(io);
  v.z = ak_read_fx(io);
  return v;
}

void
ak_write_mat3(ak_iostream io, ak_mat3 mat3)
{
  for (uint32_t i = 0; i < 9; i++) {
    ak_write_fx(io, mat3.v[i]);
  }
}

ak_mat3
ak_read_mat3(ak_iostream io)
{
  ak_mat3 mat3 = { 0 };
  for (uint32_t i = 0; i < 9; i++) {
    mat3.v[i] = ak_read_fx(io);
  }
  return mat3;
}

//--- internal ---//

void
ak_write_comp_tu(ak_iostream io,
                 ak_comp_tu ctu)
{
  ak_write_i32(io, ctu.ce);
  switch (ctu.ce) {
#define ak_d_comp_x(name)                   \
  case ak_as_comp_e(name): {                \
    ak_as_comp_t(name) name =               \
      *(ak_as_comp_t(                       \
        name)*)ak_comp_tu_comp(&ctu);       \
    ak_write_##name(io, name);              \
    return;                                 \
  }
#include "ak/game/comp.inc"
#undef ak_d_comp_x
    default: {
      ak_assert(false);
    }
  }
}

ak_comp_tu
ak_read_comp_tu(ak_iostream io)
{
  ak_comp_tu ctu = { 0 };
  ctu.ce = ak_read_i32(io);

  switch (ctu.ce) {
#define ak_d_comp_x(name)                   \
  case ak_as_comp_e(name): {                \
    ak_as_comp_t(name) name =               \
      *(ak_as_comp_t(                       \
        name)*)ak_comp_tu_comp(&ctu);       \
    ctu.name = ak_read_##name(io);          \
    break;                                  \
  }
#include "ak/game/comp.inc"
#undef ak_d_comp_x
    default: {
      ak_assert(false);
    }
  }

  return ctu;
}

void
ak_write_tf2d(ak_iostream io, ak_tf2d_t tf2d)
{
  ak_write_vec2(io, tf2d.pos);
  ak_write_fx(io, tf2d.rot);
  ak_write_vec2(io, tf2d.scale);
}
ak_tf2d_t
ak_read_tf2d(ak_iostream io)
{
  ak_tf2d_t tf2d = { 0 };
  tf2d.pos = ak_read_vec2(io);
  tf2d.rot = ak_read_fx(io);
  tf2d.scale = ak_read_vec2(io);
  return tf2d;
}

void
ak_write_gmat3(ak_iostream io,
               ak_gmat3_t gmat3)
{
  ak_write_mat3(io, gmat3);
}

ak_gmat3_t
ak_read_gmat3(ak_iostream io)
{
  return ak_read_mat3(io);
}

void
ak_write_camera(ak_iostream io,
                ak_camera_t camera)
{
  ak_write_bool(io, camera.ph);
}

ak_camera_t
ak_read_camera(ak_iostream io)
{
  ak_camera_t cam = { 0 };
  cam.ph = ak_read_bool(io);
  return cam;
}

void
ak_write_screen(ak_iostream io,
                ak_screen_t screen)
{
  ak_write_u32(io, screen.w);
  ak_write_u32(io, screen.h);
}

ak_screen_t
ak_read_screen(ak_iostream io)
{
  ak_screen_t s = { 0 };
  s.w = ak_read_u16(io);
  s.h = ak_read_u16(io);
  return s;
}

void
ak_write_mtrl(ak_iostream io, ak_mtrl_t mtrl)
{
  ak_assert(false);
  ak_write_u32(io, mtrl.base_id);
}

ak_mtrl_t
ak_read_mtrl(ak_iostream io)
{
  ak_assert(false);
  ak_mtrl_t m = { 0 };
  m.base_id = ak_read_u32(io);
  return m;
}

void
ak_write_mtrl_base(ak_iostream io,
                   ak_mtrl_base_t mtrl_base)
{
  ak_assert(false);
  ak_write_i32(io, mtrl_base.me);
}

ak_mtrl_base_t
ak_read_mtrl_base(ak_iostream io)
{
  ak_assert(false);
  ak_mtrl_base_t mb = { 0 };
  mb.me = ak_read_i32(io);
  return mb;
}

void
ak_write_script(ak_iostream io,
                ak_script_t script)
{
  ak_write_i32(io, script.se);
}
ak_script_t
ak_read_script(ak_iostream io)
{
  ak_script_t script = { 0 };
  script.se = ak_read_i32(io);
  return script;
}
