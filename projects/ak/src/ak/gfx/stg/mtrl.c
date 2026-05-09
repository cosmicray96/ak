#include "ak/gfx/stg/mtrl.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/mtrl.h"
#include "ak/gfx/mtrl_itn.h"

//===== ak_mtrlstg =====//
//--- private ---//
struct ak_mtrlstg
{
  ak_alct alct;
  void* mtrls[ak_mtrl_count_e];
};

//--- public ---//
ak_mtrlstg*
ak_mtrlstg_make(ak_gfx* g, ak_alct alct)
{
  ak_mtrlstg* ms =
    ak_alct_alloc(alct, sizeof(ak_mtrlstg));
  ms->alct = alct;

#define X(name)                             \
  ms->mtrls[ak_as_mtrl_e(name)] =           \
    ak_##name##_make(g, alct);
#include "ak/gfx/mtrl.inc"
#undef X

  return ms;
}
void
ak_mtrlstg_destroy(ak_mtrlstg* ms)
{

#define X(name)                             \
  ak_##name##_destroy(                      \
    ms->mtrls[ak_as_mtrl_e(name)]);
#include "ak/gfx/mtrl.inc"
#undef X

  ak_alct_free(ms->alct, ms);
}

ak_mtrl
ak_mtrlstg_at(ak_mtrlstg* ms,
              ak_mtrl_enum me)
{
  ak_mtrl mtrl = { 0 };
  mtrl.ctx = ms->mtrls[me];
  switch (me) {
#define X(name)                             \
  case ak_as_mtrl_e(name): {                \
    mtrl.call_begin =                       \
      &ak_##name##_call_begin;              \
    mtrl.call_end = &ak_##name##_call_end;  \
    mtrl.push_quad = &ak_##name##_pushquad; \
    break;                                  \
  }
#include "ak/gfx/mtrl.inc"
#undef X
    default: {
      ak_assert(false);
    }
  }

  return mtrl;
}
