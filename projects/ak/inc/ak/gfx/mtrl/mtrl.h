#ifndef ak_gfx_mtrl_mtrl_h
#define ak_gfx_mtrl_mtrl_h

#include "ak/core/math/vec4.h"

typedef struct ak_mtrl_vcol ak_mtrl_vcol;

typedef struct ak_mtrl_col ak_mtrl_col;
void
ak_mtrl_col_col_set(ak_mtrl_col* m,
                    ak_vec4 col);

typedef enum
{
#define X(name) ak_mtrl_##name##_e,
#include "ak/gfx/mtrl/mtrl.inc"
#undef X
  ak_mtrl_count_e
} ak_mtrl_enum;

#endif
