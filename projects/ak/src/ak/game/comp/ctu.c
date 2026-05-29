#include "ak/game/comp.h"

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
