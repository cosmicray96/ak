#ifndef ak_game_comp_h
#define ak_game_comp_h

#include "ak/def.h"
#include "ak/game/comp_t.h"

#include <stddef.h>

#define ak_as_comp_t(name) ak_##name##_t
#define ak_as_comp_e(name) ak_##name##_e

typedef enum
{
#define ak_d_comp_x(name) ak_as_comp_e(name),
#include "ak/game/comp.inc"
#undef ak_d_comp_x
  ak_comp_count_e
} ak_comp_enum;

static const uint32_t
  ak_comp_sizes[ak_comp_count_e] = {
#define ak_d_comp_x(name)                   \
  sizeof(ak_as_comp_t(name)),
#include "ak/game/comp.inc"
#undef ak_d_comp_x
  };

const char*
ak_comp_to_str(ak_comp_enum ce);

typedef struct
{
  ak_comp_enum ce;
  union
  {
    void* comp;
#define ak_d_comp_x(name)                   \
  ak_as_comp_t(name) name;
#include "ak/game/comp.inc"
#undef ak_d_comp_x
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

#define ak_d_comp_x(name)                   \
  void ak_write_##name(ak_iostream io,      \
                       ak_as_comp_t(name)   \
                         name);             \
  ak_as_comp_t(name)                        \
    ak_read_##name(ak_iostream io);

#include "ak/game/comp.inc"
#undef ak_d_comp_x

void
ak_write_comp_tu(ak_iostream io,
                 ak_comp_tu ctu);
ak_comp_tu
ak_read_comp_tu(ak_iostream io);

#ifdef ak_user_comp_path
#include ak_user_comp_path
#endif

#endif
