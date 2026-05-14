#include "ak/game/stg/comp.h"
#include "ak/coll/spa.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/comp.h"
#include <stdint.h>

//===== ak_compstg =====//
//--- private ---//
struct ak_compstg
{
  ak_alct alct;
  ak_spa spas[ak_comp_count_e];
};

//--- public ---//
ak_compstg*
ak_compstg_make(ak_alct alct)
{
  ak_compstg* cs =
    ak_alct_alloc(alct, sizeof(ak_compstg));
  cs->alct = alct;

#define ak_d_comp_x(name)                   \
  cs->spas[ak_##name##_e] = ak_spa_make(    \
    sizeof(ak_##name##_t), alct);

#include "ak/game/comp.inc"
#undef ak_d_comp_x

  return cs;
}

void
ak_compstg_destroy(ak_compstg* cs)
{
  for (uint32_t i = 0; i < ak_comp_count_e;
       i++) {
    ak_spa_destroy(&cs->spas[i]);
  }
  ak_alct_free(cs->alct, cs);
}

bool
ak_compstg_exist(ak_compstg* cs,
                 ak_ett e,
                 ak_comp_enum ce)
{
  return ak_spa_exist(&cs->spas[ce], e);
}

void*
ak_compstg_at(ak_compstg* cs,
              ak_ett e,
              ak_comp_enum ce)
{
  return ak_spa_at(&cs->spas[ce], e);
}

void
ak_compstg_overwrite(ak_compstg* cs,
                     ak_ett e,
                     ak_comp_enum ce,
                     const void* comp)
{
  ak_spa_overwrite(&cs->spas[ce], e, comp);
}

void
ak_compstg_add(ak_compstg* cs,
               ak_ett e,
               ak_comp_enum ce,
               const void* comp)
{
  ak_spa_insert(&cs->spas[ce], e, comp);
}

void
ak_compstg_remove(ak_compstg* cs,
                  ak_ett e,
                  ak_comp_enum ce)
{
  ak_spa_remove(&cs->spas[ce], e);
}

void
ak_compstg_remove_all(ak_compstg* cs,
                      ak_ett e)
{
  uint32_t count = ak_comp_count_e;
  for (uint32_t i = 0; i < count; i++) {
    ak_spa* spa = &cs->spas[i];
    if (ak_spa_exist(spa, e)) {
      ak_spa_remove(spa, e);
    }
  }
}

uint32_t
ak_compstg_count(ak_compstg* cs,
                 ak_comp_enum ce)
{
  return ak_spa_count(&cs->spas[ce]);
}

void*
ak_compstg_at_idx(ak_compstg* cs,
                  ak_comp_enum ce,
                  uint32_t idx,
                  ak_ett* o_e)
{
  return ak_spa_at_didx(
    &cs->spas[ce], idx, o_e);
}
