#include "ak/game/comp.h"
#include "ak/debug.h"

const char* strs[] = {
#define ak_d_comp_x(name) "ak_" #name,
#include "ak/game/comp.inc"
#undef ak_d_comp_x
};

const char*
ak_comp_to_str(ak_comp_enum ce)
{
  return strs[ce];
}

ak_comp_tu
ak_comp_tu_make(ak_comp_enum ce,
                const void* comp)
{
  ak_comp_tu ctu = { 0 };
  ak_comp_tu_make_ip(&ctu, ce, comp);
  return ctu;
}

void
ak_comp_tu_make_ip(ak_comp_tu* ctu,
                   ak_comp_enum ce,
                   const void* comp)
{
  ctu->ce = ce;
  switch (ce) {
#define ak_d_comp_x(name)                   \
  case ak_##name##_e: {                     \
    ctu->name = *(ak_##name##_t*)(comp);    \
    break;                                  \
  }
#include "ak/game/comp.inc"
#undef ak_d_comp_x
    default: {
      ak_assert(false);
    }
  }
}

void*
ak_comp_tu_comp(ak_comp_tu* ctu)
{
  switch (ctu->ce) {
#define ak_d_comp_x(name)                   \
  case ak_##name##_e: {                     \
    return &ctu->name;                      \
  }
#include "ak/game/comp.inc"
#undef ak_d_comp_x
    default: {
      ak_assert(false);
    }
  }
  return 0;
}

const void*
ak_comp_tu_comp_const(const ak_comp_tu* ctu)
{
  switch (ctu->ce) {
#define ak_d_comp_x(name)                   \
  case ak_##name##_e: {                     \
    return &ctu->name;                      \
  }
#include "ak/game/comp.inc"
#undef ak_d_comp_x
    default: {
      ak_assert(false);
    }
  }
  return 0;
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
