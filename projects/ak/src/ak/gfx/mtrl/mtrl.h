#ifndef ak_gfx_mtrl_mtrl_h
#define ak_gfx_mtrl_mtrl_h

#include "ak/gfx/mtrl/col.h"
#include "ak/gfx/mtrl/vcol.h"

typedef enum
{
#define X(name) ak_mtrl_##name##_e,
#include "ak/gfx/mtrl/mtrl.inc"
#undef X
  ak_mtrl_count_e
} ak_mtrl_enum;

#endif
