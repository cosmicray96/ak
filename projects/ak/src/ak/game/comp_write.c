#include "ak/core/math/fixed.h"
#include "ak/game/comp.h"
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
}
ak_comp_tu
ak_read_comp_tu(ak_iostream io)
{
  return (ak_comp_tu){ 0 };
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
               ak_gmat3_t gmat3);
ak_gmat3_t
ak_read_gmat3(ak_iostream io);

void
ak_write_camera(ak_iostream io,
                ak_camera_t camera);
ak_camera_t
ak_read_camera(ak_iostream io);

void
ak_write_screen(ak_iostream io,
                ak_screen_t screen);
ak_screen_t
ak_read_screen(ak_iostream io);

void
ak_write_mtrl(ak_iostream io,
              ak_mtrl_t mtrl);
ak_mtrl_t
ak_read_mtrl(ak_iostream io);

void
ak_write_mtrl_base(ak_iostream io,
                   ak_mtrl_base_t mtrl_base);
ak_mtrl_base_t
ak_read_mtrl_base(ak_iostream io);
