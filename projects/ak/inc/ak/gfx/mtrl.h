#ifndef ak_gfx_mtrl_mtrl_h
#define ak_gfx_mtrl_mtrl_h

#include "ak/core/math/mat3x3.h"
#include "ak/core/math/tf2d.h"
#include "ak/gfx/mtrl_t.h"

#define ak_as_mtrl_e(name) ak_mtrl_##name##_e
#define ak_as_mtrl_t(name)                  \
  ak_##name##_t // same as ak_as_comp_t(name)

typedef enum
{
#define X(name) ak_as_mtrl_e(name),
#include "ak/gfx/mtrl.inc"
#undef X
  ak_mtrl_count_e
} ak_mtrl_enum;

typedef struct
{
  void (*call_begin)(void* ctx,
                     const ak_mat3x3* vp);
  void (*call_end)(void* ctx);

  void (*push_quad)(void* ctx,
                    const ak_mat3x3* gmat3,
                    const void* comp);
  void* ctx;
} ak_mtrl;

#endif
