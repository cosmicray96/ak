#include "ak/game/stg/comp.h"
#include "ak/coll/hmn.h"
#include "ak/coll/spa.h"

//===== ak_compstg =====//
//--- private ---//
typedef enum
{
  compkey_none = 0,
  key_comp1,
  key_comp2
} compkey;

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
      &cs.comps, key_comp1, &spa);
  }
  {
    ak_spa spa =
      ak_spa_make(sizeof(ak_comp2), alct);
    ak_hmn_insert_u64(
      &cs.comps, key_comp2, &spa);
  }

  return cs;
}

void
ak_compstg_destroy(ak_compstg* cs)
{
  compkey key = { 0 };
  ak_spa* spa = { 0 };
  ak_hmn_iter it =
    ak_hmn_iter_make(&cs->comps);
  while (ak_hmn_iter_next_u64(
    &it, (uint64_t*)&key, (void**)&spa)) {
    ak_spa_destroy(spa);
  }
}

void*
ak_compstg_comp1(ak_compstg* cs, ak_ett ett)
{
  ak_spa* spa =
    ak_hmn_at_u64(&cs->comps, key_comp1);
  if (ak_spa_exist(spa, ett)) {
    return ak_spa_at(spa, ett);
  }
  return 0;
}
void*
ak_compstg_comp2(ak_compstg* cs, ak_ett ett)
{
  ak_spa* spa =
    ak_hmn_at_u64(&cs->comps, key_comp2);
  if (ak_spa_exist(spa, ett)) {
    return ak_spa_at(spa, ett);
  }
  return 0;
}
