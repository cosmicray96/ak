#ifndef ak_game_comp_h
#define ak_game_comp_h

#include "ak/debug.h"
#include "ak/def.h"
#include "ak/export.h"
#include "ak/game/comp_t.h"
#include "ak/system/stream.h"

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

ak_ex uint32_t
ak_comp_enum_offsets(ak_comp_enum ce,
                     uint32_t o_offsets[16]);

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
ak_ex const void*
ak_comp_tu_comp_const(const ak_comp_tu* ctu);

#define ak_d_comp_x(name)                   \
  ak_errcode ak_stm_write_##name(           \
    ak_stm stm, ak_as_comp_t(name) name);   \
  ak_errcode ak_stm_read_##name(            \
    ak_stm stm,                             \
    ak_as_comp_t(name) * o_##name);

#include "ak/game/comp.inc"
#undef ak_d_comp_x

ak_errcode
ak_stm_write_comp_tu(ak_stm stm,
                     ak_comp_tu ctu);
ak_errcode
ak_stm_read_comp_tu(ak_stm stm,
                    ak_comp_tu* o_ctu);

#endif
