#ifndef ak_game_comp_h
#define ak_game_comp_h

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
#define _ak_off1(t, a)                      \
  offsetof(ak_comp_##t, a)
#define _ak_off2(t, a, ...)                 \
  offsetof(ak_comp_##t, a),                 \
    _ak_off1(t, __VA_ARGS__)
#define _ak_off3(t, a, ...)                 \
  offsetof(ak_comp_##t, a),                 \
    _ak_off2(t, __VA_ARGS__)
#define _ak_off4(t, a, ...)                 \
  offsetof(ak_comp_##t, a),                 \
    _ak_off3(t, __VA_ARGS__)
#define _ak_off5(t, a, ...)                 \
  offsetof(ak_comp_##t, a),                 \
    _ak_off4(t, __VA_ARGS__)
// ... up to ak_d_max_ett_ref (16)

// --- count args ---
#define _ak_nargs(...)                      \
  _ak_nargs_(__VA_ARGS__,                   \
             16,                            \
             15,                            \
             14,                            \
             13,                            \
             12,                            \
             11,                            \
             10,                            \
             9,                             \
             8,                             \
             7,                             \
             6,                             \
             5,                             \
             4,                             \
             3,                             \
             2,                             \
             1)
#define _ak_nargs_(_1,                      \
                   _2,                      \
                   _3,                      \
                   _4,                      \
                   _5,                      \
                   _6,                      \
                   _7,                      \
                   _8,                      \
                   _9,                      \
                   _10,                     \
                   _11,                     \
                   _12,                     \
                   _13,                     \
                   _14,                     \
                   _15,                     \
                   _16,                     \
                   N,                       \
                   ...)                     \
  N

// --- dispatch to correct expander ---
#define _ak_offn(n, t, ...)                 \
  _ak_off##n(t, __VA_ARGS__)
#define _ak_offn_(n, t, ...)                \
  _ak_offn(                                 \
    n, t, __VA_ARGS__) // extra pass for
                       // expansion

// --- the actual macro ---
#define ak_d_set_ett_ref(name, ...)         \
  do {                                      \
    uint32_t _offs[] = { _ak_offn_(         \
      _ak_nargs(__VA_ARGS__),               \
      name,                                 \
      __VA_ARGS__) };                       \
    uint32_t _n =                           \
      sizeof(_offs) / sizeof(_offs[0]);     \
    _Static_assert(_n <= ak_d_max_ett_ref,  \
                   "too many ett refs");    \
    ak_comp_ett_refs[ak_comp_##name##_e]    \
      .count = _n;                          \
    for (uint32_t _i = 0; _i < _n; _i++)    \
      ak_comp_ett_refs[ak_comp_##name##_e]  \
        .offsets[_i] = _offs[_i];           \
  } while (0)

static void
a()
{
  // ak_d_set_ett_ref(compA, a, b, c);
}

#endif
