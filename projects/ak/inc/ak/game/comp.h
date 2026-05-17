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

#include "ak/gfx/mtrl.h"
static ak_comp_enum
ak_mtrl_to_comp_e(ak_mtrl_enum me)
{
  return me + (ak_comp_count_e -
               (2 * ak_mtrl_count_e));
}

#define ak_d_max_ett_ref 16
typedef struct
{
  uint32_t offsets[ak_d_max_ett_ref];
  uint32_t count;
} ak_comp_ett_ref;
static ak_comp_ett_ref
  ak_comp_ett_refs[ak_comp_count_e];

// --- offsetof expander per arg count ---
#define ak_off1(t, a)                       \
  offsetof(ak_as_comp_t(t), a)
#define ak_off2(t, a, ...)                  \
  offsetof(ak_as_comp_t(t), a),             \
    ak_off1(t, __VA_ARGS__)
#define ak_off3(t, a, ...)                  \
  offsetof(ak_as_comp_t(t), a),             \
    ak_off2(t, __VA_ARGS__)
#define ak_off4(t, a, ...)                  \
  offsetof(ak_as_comp_t(t), a),             \
    ak_off3(t, __VA_ARGS__)
#define ak_off5(t, a, ...)                  \
  offsetof(ak_as_comp_t(t), a),             \
    ak_off4(t, __VA_ARGS__)
#define ak_off6(t, a, ...)                  \
  offsetof(ak_as_comp_t(t), a),             \
    ak_off5(t, __VA_ARGS__)

#define ak_comp_offn_(n, t, ...)            \
  ak_off##n(t, __VA_ARGS__)
#define ak_comp_offn(n, t, ...)             \
  ak_comp_offn_(n, t, __VA_ARGS__)

#define ak_comp_set_ref(name, ...)          \
  do {                                      \
    uint32_t _offs[] = { ak_comp_offn(      \
      ak_nargs(__VA_ARGS__),                \
      name,                                 \
      __VA_ARGS__) };                       \
    uint32_t _n =                           \
      sizeof(_offs) / sizeof(_offs[0]);     \
    ak_log_assert(_n <= ak_d_max_ett_ref,   \
                  "too many ett refs");     \
    ak_comp_ett_refs[ak_as_comp_e(name)]    \
      .count = _n;                          \
    for (uint32_t _i = 0; _i < _n; _i++)    \
      ak_comp_ett_refs[ak_as_comp_e(name)]  \
        .offsets[_i] = _offs[_i];           \
  } while (0)

#endif
