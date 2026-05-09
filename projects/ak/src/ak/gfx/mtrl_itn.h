#ifndef ak_gfx_mtrl_itn_h
#define ak_gfx_mtrl_itn_h

#include "ak/core/math/tf2d.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"

#define ak_as_mtrl_p(name) ak_##name

#define X(name)                             \
  typedef struct ak_as_mtrl_p(name)         \
    ak_as_mtrl_p(name);                     \
  ak_as_mtrl_p(name) *                      \
    ak_##name##_make(ak_gfx* g,             \
                     ak_alct alct);         \
  void ak_##name##_destroy(void* m);        \
  void ak_##name##_call_begin(void* m);     \
  void ak_##name##_call_end(void* m);       \
  void ak_##name##_pushquad(                \
    void* m, ak_tf2d tf, const void* comp);

#include "ak/gfx/mtrl.inc"
#undef X

#endif
