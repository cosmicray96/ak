#include "ak/game/comp.h"
#include "ak/debug.h"

const char* strs[] = {
#define X(name) "ak_" #name,
#include "ak/game/comp.inc"
#undef X
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
#define X(name)                             \
  case ak_##name##_e: {                     \
    ctu->name = *(ak_##name##_t*)(comp);    \
    break;                                  \
  }
#include "ak/game/comp.inc"
#undef X
    default: {
      ak_assert(false);
    }
  }
}

void*
ak_comp_tu_comp(ak_comp_tu* ctu)
{
  switch (ctu->ce) {
#define X(name)                             \
  case ak_##name##_e: {                     \
    return &ctu->name;                      \
  }
#include "ak/game/comp.inc"
#undef X
    default: {
      ak_assert(false);
    }
  }
  return 0;
}
