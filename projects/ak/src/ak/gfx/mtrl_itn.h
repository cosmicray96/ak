#ifndef ak_gfx_mtrl_itn_h
#define ak_gfx_mtrl_itn_h

#include "ak/core/math/mat3x3.h"
#include "ak/gfx/mtrl.h"

typedef struct
{
  void (*call_begin)(
    void* ctx,
    const ak_mtrl_basedata* bd,
    const ak_mtrl_indata* id);

  void (*call_end)(void* ctx);

  void (*push_quad)(
    void* ctx,
    const ak_mtrl_quaddata* qd,
    const ak_mat3_f* gmat);
  void* ctx;
} ak_mtrl;

#endif
