#include "ak/game/comp.h"

#define ak_d_comp_x(name)                   \
  uint32_t ak_comp_ett_offsets_##name(      \
    uint32_t o_offsets[16]);
#include "ak/game/comp.inc"
#undef ak_d_comp_x

typedef uint32_t (*offset_fn)(
  uint32_t o_offsets[16]);

static offset_fn
  offset_fns[ak_comp_count_e] = {
#define ak_d_comp_x(name)                   \
  &ak_comp_ett_offsets_##name,
#include "ak/game/comp.inc"
#undef ak_d_comp_x

  };

uint32_t
ak_comp_enum_offsets(ak_comp_enum ce,
                     uint32_t o_offsets[16])
{
  return offset_fns[ce](o_offsets);
}

uint32_t
ak_comp_ett_offsets_tf2d(
  uint32_t o_offsets[16])
{
  return 0;
}
uint32_t
ak_comp_ett_offsets_gmat3(
  uint32_t o_offsets[16])
{
  return 0;
}
uint32_t
ak_comp_ett_offsets_camera(
  uint32_t o_offsets[16])
{
  return 0;
}
uint32_t
ak_comp_ett_offsets_screen(
  uint32_t o_offsets[16])
{
  return 0;
}
uint32_t
ak_comp_ett_offsets_mtrl(
  uint32_t o_offsets[16])
{
  return 0;
}

uint32_t
ak_comp_ett_offsets_mtrl_base(
  uint32_t o_offsets[16])
{
  o_offsets[0] =
    offsetof(ak_mtrl_t, base_id);
  return 1;
}

uint32_t
ak_comp_ett_offsets_script(
  uint32_t o_offsets[16])
{
  return 0;
}
