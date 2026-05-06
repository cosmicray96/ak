#include "ak/gfx/mtrl/registry.h"
#include "ak/coll/sla.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include <stdint.h>

//===== ak_mtrlreg =====//
//--- private ---//
struct ak_mtrlreg
{
  ak_alct alct;
  ak_gfx* gf;
  ak_sla mtrls[ak_mtrl_count_e];
};

uint32_t
ak_mtrlreg_new(ak_mtrlreg* mr,
               ak_mtrl_enum me,
               void* m)
{
  return ak_sla_insert(&mr->mtrls[me], &m);
}

//--- public ---//
ak_mtrlreg*
ak_mtrlreg_make(ak_gfx* gf, ak_alct alct)
{
  ak_mtrlreg* mr =
    ak_alct_alloc(alct, sizeof(ak_mtrlreg));
  mr->alct = alct;
  mr->gf = gf;

  return mr;
}

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
    ak_mtrlreg* mr, ak_alct alct)           \
  {                                         \
    ak_mtrl_##name* m =                     \
      ak_mtrl_##name##_make(mr->gf,         \
                            mr->alct);      \
    ak_mtrlreg_new(                         \
      mr, ak_mtrl_##name##_e, m);           \
  }

#include "ak/gfx/mtrl/mtrl.inc"
#undef X
