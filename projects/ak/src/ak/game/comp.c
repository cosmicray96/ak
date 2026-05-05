#include "ak/game/comp.h"

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
