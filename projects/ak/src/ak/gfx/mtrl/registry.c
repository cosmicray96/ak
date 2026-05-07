#include "ak/gfx/mtrl/registry.h"
#include "ak/coll/sla.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"
#include "ak/gfx/mtrl/mtrl_itn.h"

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
               ak_mtrl_enum me)
{
  switch (me) {
#define X(name)                             \
  case ak_mtrl_##name##_e: {                \
    return ak_sla_insert(                   \
      &mr->mtrls[me],                       \
      ak_mtrl_##name##_make(mr->gf,         \
                            mr->alct));     \
  }
#include "ak/gfx/mtrl/mtrl.inc"
#undef X
    default: {
      ak_assert(false);
    }
  }
  return 0;
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
ak_mtrlreg_destroy(ak_mtrlreg* mr)
{
  // todo
}

void*
ak_mtrlreg_at(ak_mtrlreg* mr,
              ak_mtrl_enum me,
              uint32_t idx)
{
  return ak_sla_at(&mr->mtrls[me], idx);
}

void
ak_mtrlreg_remove(ak_mtrlreg* mr,
                  ak_mtrl_enum me,
                  uint32_t idx)
{
  void* m = 0;
  switch (me) {
#define X(name)                             \
  case ak_mtrl_##name##_e: {                \
    m = *(void**)ak_sla_at(&mr->mtrls[me],  \
                           idx);            \
    ak_mtrl_##name##_destroy(               \
      (ak_mtrl_##name*)m);                  \
    ak_sla_remove(&mr->mtrls[me], idx);     \
  }
#include "ak/gfx/mtrl/mtrl.inc"
#undef X
    default: {
      ak_assert(false);
    }
  }
}
