#ifndef ak_gfx_mtrl_itn_h
#define ak_gfx_mtrl_itn_h

#include "ak/core/math/tf2d.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"

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

#define ak_as_mtrl_p(name) ak_##name

#define ak_d_mtrl_x(name)                   \
  typedef struct ak_as_mtrl_p(name)         \
    ak_as_mtrl_p(name);                     \
  ak_as_mtrl_p(name) *                      \
    ak_##name##_make(ak_gfx* g,             \
                     ak_alct alct);         \
  void ak_##name##_destroy(void* m);        \
  void ak_##name##_call_begin(              \
    void* m, const ak_mat3x3* vp);          \
  void ak_##name##_call_end(void* m);       \
  void ak_##name##_pushquad(                \
    void* m,                                \
    const ak_mat3x3* gmat3,                 \
    const void* comp);

#include "ak/gfx/mtrl.inc"
#undef ak_d_mtrl_x

#endif
