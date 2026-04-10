#include "ak/coll/da.h"
#include "ak/coll/dbuff.h"

ak_da
ak_da_make(uint32_t is, ak_alct alct)
{
  ak_da da = { 0 };
  da.dbuff =
    ak_dbuff_make(is, 1.5f, 10, alct);
  da.count = 0;
  return da;
}

void
ak_da_destroy(ak_da* da)
{
  ak_dbuff_destroy(&da->dbuff);
  da->count = 0;
}

void
ak_da_clear(ak_da* da)
{
  da->count = 0;
}

void*
ak_da_at_impl(ak_da* da, uint32_t idx)
{
  return ak_dbuff_at(&da->dbuff, idx);
}
const void*
ak_da_at_const_impl(const ak_da* da,
                    uint32_t idx)
{
  return ak_dbuff_at_const(&da->dbuff, idx);
}

ak_ex void
ak_da_pushback(ak_da* da, const void* item);
ak_ex void
ak_da_pushfront(ak_da* da, const void* item);
ak_ex void
ak_da_insert(ak_da* da,
             uint32_t idx,
             const void* item);
ak_ex void
ak_da_overwrite(ak_da* da,
                uint32_t idx,
                const void* item);

ak_ex void
ak_da_remove(ak_da* da, uint32_t idx);
