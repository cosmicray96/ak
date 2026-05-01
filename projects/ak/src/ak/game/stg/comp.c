#include "ak/game/stg/comp.h"
#include "ak/coll/hmn.h"
#include "ak/coll/spa.h"

//===== ak_compstg =====//

//--- public ---//
ak_compstg
ak_compstg_make(ak_alct alct)
{
  ak_compstg cs = { 0 };
  cs.comps =
    ak_hmn_make(sizeof(ak_spa), alct);
  {
    ak_spa spa =
      ak_spa_make(sizeof(ak_comp1), alct);
    ak_hmn_insert_u64(
      &cs.comps, ak_comptype_comp1, &spa);
  }
  {
    ak_spa spa =
      ak_spa_make(sizeof(ak_comp2), alct);
    ak_hmn_insert_u64(
      &cs.comps, ak_comptype_comp2, &spa);
  }

  return cs;
}

void
ak_compstg_destroy(ak_compstg* cs)
{
  ak_comptype key = { 0 };
  ak_spa* spa = { 0 };
  ak_hmn_iter it =
    ak_hmn_iter_make(&cs->comps);
  while (ak_hmn_iter_next_u64(
    &it, (uint64_t*)&key, (void**)&spa)) {
    ak_spa_destroy(spa);
  }
}

void
ak_compstg_add(ak_compstg* cs,
               ak_ett ett,
               ak_comptype ct,
               const void* comp)
{
  ak_spa* spa = ak_hmn_at_u64(
    &cs->comps, ak_comptype_comp1);
  ak_spa_insert(spa, ett, comp);
}

void
ak_compstg_remove(ak_compstg* cs,
                  ak_ett ett,
                  ak_comptype ct)
{
  ak_spa* spa = ak_hmn_at_u64(
    &cs->comps, ak_comptype_comp1);
  ak_spa_remove(spa, ett);
}

void*
ak_compstg_at(ak_compstg* cs,
              ak_ett ett,
              ak_comptype ct)
{
  ak_spa* spa =
    ak_hmn_at_u64(&cs->comps, ct);
  if (ak_spa_exist(spa, ett)) {
    return ak_spa_at(spa, ett);
  }
  return 0;
}

void*
ak_compstg_comp1(ak_compstg* cs, ak_ett ett)
{
  ak_spa* spa = ak_hmn_at_u64(
    &cs->comps, ak_comptype_comp1);
  if (ak_spa_exist(spa, ett)) {
    return ak_spa_at(spa, ett);
  }
  return 0;
}
void*
ak_compstg_comp2(ak_compstg* cs, ak_ett ett)
{
  ak_spa* spa = ak_hmn_at_u64(
    &cs->comps, ak_comptype_comp2);
  if (ak_spa_exist(spa, ett)) {
    return ak_spa_at(spa, ett);
  }
  return 0;
}
