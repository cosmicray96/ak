#ifndef ak_gfx_mtrl_view_h
#define ak_gfx_mtrl_view_h

#include "ak/export.h"
#include "ak/gfx/mtrl/mtrl.h"
#include <stdint.h>
typedef struct ak_mtrlstg_v ak_mtrlstg_v;

ak_ex uint32_t
ak_mtrlstg_v_new(ak_mtrlstg_v* v,
                 ak_mtrl_enum me);
ak_ex void
ak_mtrlstg_v_remove(ak_mtrlstg_v* v,
                    ak_mtrl_enum me,
                    uint32_t idx);

ak_ex void*
ak_mtrlstg_v_at(ak_mtrlstg_v* v,
                ak_mtrl_enum me,
                uint32_t idx);

#define X(name)                             \
  ak_ex ak_mtrl_##name*                     \
    ak_mtrlstg_v_at_##name(ak_mtrlstg_v* v, \
                           ak_mtrl_enum me, \
                           uint32_t idx)    \
  {                                         \
    return (ak_mtrl_##name*)                \
      ak_mtrlstg_v_at(v, me, idx);          \
  }

#include "ak/gfx/mtrl/mtrl.inc"
#undef X

#endif
