#include "ak/game/comp.h"

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
