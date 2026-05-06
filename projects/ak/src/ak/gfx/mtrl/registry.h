#ifndef ak_gfx_mtrl_register_h
#define ak_gfx_mtrl_register_h

#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/mtrl/mtrl.h"
#include <stdint.h>

typedef struct ak_mtrlreg ak_mtrlreg;

ak_mtrlreg*
ak_mtrlreg_make(ak_gfx* gf, ak_alct alct);
void
ak_mtrlreg_destroy(ak_mtrlreg* mr);

void*
ak_mtrlreg_at(ak_mtrlreg* mr,
              ak_mtrl_enum me,
              uint32_t idx);

void
ak_mtrlreg_remove(ak_mtrlreg* mr,
                  ak_mtrl_enum me,
                  uint32_t idx);

#define X(name)                             \
  uint32_t ak_mtrlreg_##name##_new(         \
    ak_mtrlreg* mr, ak_alct alct);

#include "ak/gfx/mtrl/mtrl.inc"
#undef X

#endif
