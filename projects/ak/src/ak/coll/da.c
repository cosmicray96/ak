#include "ak/coll/da.h"
#include "ak/coll/dbuff.h"
#include "ak/debug.h"
#include <string.h>

//===== ak_da =====//
//--- private ---//
static void
resize_ifneeded(ak_da* da)
{
  if (da->count < ak_dbuff_cap(&da->dbuff)) {
    return;
  }
  ak_dbuff_grow_w_copy(&da->dbuff);
}

//--- export ---//
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

void
ak_da_overwrite_zero(ak_da* da, uint32_t idx)
{
  ak_assert(idx < da->count);
  ak_dbuff_overwrite_zero(&da->dbuff, idx);
}

ak_ex void*
ak_da_pushback_zero(ak_da* da)
{
  resize_ifneeded(da);

  da->count++;
  ak_da_overwrite_zero(da, da->count - 1);

  return ak_da_at_impl(da, da->count - 1);
}

void
ak_da_at_copy(const ak_da* da,
              uint32_t idx,
              void* o_item)
{
  ak_assert(idx < da->count);
  ak_dbuff_at_copy(&da->dbuff, idx, o_item);
}

void*
ak_da_at_impl(ak_da* da, uint32_t idx)
{
  ak_assert(idx < da->count);
  return ak_dbuff_at(&da->dbuff, idx);
}
const void*
ak_da_at_const_impl(const ak_da* da,
                    uint32_t idx)
{
  ak_assert(idx < da->count);
  return ak_dbuff_at_const(&da->dbuff, idx);
}

void
ak_da_pushback(ak_da* da, const void* item)
{
  resize_ifneeded(da);
  ak_dbuff_overwrite(
    &da->dbuff, da->count, item);
  da->count++;
}

void
ak_da_pushfront(ak_da* da, const void* item)
{
  ak_da_insert(da, 0, item);
}

void
ak_da_insert(ak_da* da,
             uint32_t idx,
             const void* item)
{
  ak_assert(idx < da->count);
  resize_ifneeded(da);
  ak_dbuff_bulk_overwrite(
    &da->dbuff,
    &da->dbuff,
    idx + 1,
    idx,
    ak_dbuff_cap(&da->dbuff) - idx);
  ak_dbuff_overwrite(&da->dbuff, idx, item);
  da->count++;
}

void
ak_da_overwrite(ak_da* da,
                uint32_t idx,
                const void* item)
{
  ak_assert(idx < da->count);
  ak_dbuff_overwrite(&da->dbuff, idx, item);
}

void
ak_da_remove(ak_da* da, uint32_t idx)
{
  ak_assert(idx < da->count);
  ak_assert(da->count > 0);
  if (idx == da->count - 1) {
    da->count--;
    return;
  }

  ak_dbuff_bulk_overwrite(&da->dbuff,
                          &da->dbuff,
                          idx,
                          idx + 1,
                          da->count - idx -
                            1);
  da->count--;
}

void
ak_da_remove_swaplast(ak_da* da,
                      uint32_t idx)
{
  ak_assert(idx < da->count);
  ak_assert(da->count > 0);
  if (idx == da->count - 1) {
    da->count--;
    return;
  }

  ak_dbuff_swap(
    &da->dbuff, idx, da->count - 1);

  da->count--;
}

bool
ak_da_findfirst(ak_da* da,
                const void* item,
                uint32_t* idx,
                ak_equal_fn fn)
{
  for (uint32_t i = 0; i < ak_da_count(da);
       i++) {
    void* cddt = ak_da_at_impl(da, i);
    if (fn(item, cddt)) {
      return true;
    }
  }

  return false;
}
