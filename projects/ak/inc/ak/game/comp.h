#ifndef ak_game_comp_h
#define ak_game_comp_h

#include "ak/game/comp_t.h"

typedef enum
{
#define X(name) ak_##name##_e,
#include "ak/game/comp.inc"
#undef X
  ak_comp_count_e
} ak_comp_enum;

const char*
ak_comp_to_str(ak_comp_enum ce);

typedef struct
{
  ak_comp_enum ce;
  union
  {
#define X(name) ak_##name##_t name;
#include "ak/game/comp.inc"
#undef X
  };
} ak_comp_tu;

ak_ex ak_comp_tu
ak_comp_tu_make(ak_comp_enum ce,
                const void* comp);
ak_ex void
ak_comp_tu_make_ip(ak_comp_tu* ctu,
                   ak_comp_enum ce,
                   const void* comp);
ak_ex void*
ak_comp_tu_comp(ak_comp_tu* tu);

// mane enum, name array, etc
#endif
