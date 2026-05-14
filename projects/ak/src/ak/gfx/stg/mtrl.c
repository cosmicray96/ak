#include "ak/gfx/stg/mtrl.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/mtrl.h"
#include "ak/gfx/mtrl_itn.h"

#define ak_d_mtrl_x(name)                   \
  typedef struct ak_mtrl_##name             \
    ak_mtrl_##name;                         \
  ak_mtrl_##name* ak_mtrl_##name##_make(    \
    ak_gfx* gf, ak_alct alct);              \
  void ak_mtrl_##name##_destroy(            \
    ak_mtrl_##name* m);                     \
  void ak_mtrl_##name##_call_begin(         \
    void* m,                                \
    const ak_mtrl_basedata* bd,             \
    const ak_mat3x3* vp);                   \
  void ak_mtrl_##name##_call_end(void* m);  \
  void ak_mtrl_##name##_pushquad(           \
    void* m,                                \
    const ak_mtrl_quaddata* dq,             \
    const ak_mat3x3* gmat3);

#include "ak/gfx/mtrl.inc"
#undef ak_d_mtrl_x

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

#define ak_d_mtrl_x(name)                   \
  ms->mtrls[ak_as_mtrl_e(name)] =           \
    ak_mtrl_##name##_make(g, alct);
#include "ak/gfx/mtrl.inc"
#undef ak_d_mtrl_x

  return ms;
}
void
ak_mtrlstg_destroy(ak_mtrlstg* ms)
{

#define ak_d_mtrl_x(name)                   \
  ak_mtrl_##name##_destroy(                 \
    ms->mtrls[ak_as_mtrl_e(name)]);
#include "ak/gfx/mtrl.inc"
#undef ak_d_mtrl_x

  ak_alct_free(ms->alct, ms);
}

ak_mtrl
ak_mtrlstg_at(ak_mtrlstg* ms,
              ak_mtrl_enum me)
{
  ak_mtrl mtrl = { 0 };
  mtrl.ctx = ms->mtrls[me];
  switch (me) {
#define ak_d_mtrl_x(name)                   \
  case ak_as_mtrl_e(name): {                \
    mtrl.call_begin =                       \
      &ak_mtrl_##name##_call_begin;         \
    mtrl.call_end =                         \
      &ak_mtrl_##name##_call_end;           \
    mtrl.push_quad =                        \
      &ak_mtrl_##name##_pushquad;           \
    break;                                  \
  }
#include "ak/gfx/mtrl.inc"
#undef ak_d_mtrl_x
    default: {
      ak_assert(false);
    }
  }

  return mtrl;
}
