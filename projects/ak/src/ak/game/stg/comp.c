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

#define X(name)                             \
  cs->spas[ak_##name##_e] = ak_spa_make(    \
    sizeof(ak_##name##_t), alct);

#include "ak/game/comp.inc"
#undef X

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
                 ak_ett ett,
                 ak_comp_enum ce)
{
  return ak_spa_exist(&cs->spas[ce], ett);
}

void*
ak_compstg_at(ak_compstg* cs,
              ak_ett ett,
              ak_comp_enum ce)
{
  return ak_spa_at(&cs->spas[ce], ett);
}

void
ak_compstg_add(ak_compstg* cs,
               ak_ett ett,
               ak_comp_enum ce,
               const void* comp)
{
  ak_spa_insert(&cs->spas[ce], ett, comp);
}

void
ak_compstg_remove(ak_compstg* cs,
                  ak_ett ett,
                  ak_comp_enum ce)
{
  ak_spa_remove(&cs->spas[ce], ett);
}
