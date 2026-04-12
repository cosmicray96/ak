#ifndef ak_da_h
#define ak_da_h

#include "ak/coll/dbuff.h"
#include <stdint.h>

typedef struct ak_da ak_da;
struct ak_da
{
  ak_dbuff dbuff;
  uint32_t count;
};

ak_ex ak_da
ak_da_make(uint32_t is, ak_alct alct);
ak_ex void
ak_da_destroy(ak_da* da);

ak_ex void
ak_da_clear(ak_da* da);

static ak_alct
ak_da_alct(const ak_da* da)
{
  return ak_dbuff_alct(&da->dbuff);
}
static uint32_t
ak_da_cap(const ak_da* da)
{
  return ak_dbuff_cap(&da->dbuff);
}
static uint32_t
ak_da_count(const ak_da* da)
{
  return da->count;
}

ak_ex void
ak_da_at_copy(const ak_da* da,
              uint32_t idx,
              void* item);

ak_ex void*
ak_da_at_impl(ak_da* da, uint32_t idx);
#define ak_da_at(t, da, idx)                \
  ((t)*)ak_da_at_impl(da, idx)
ak_ex const void*
ak_da_at_const_impl(const ak_da* da,
                    uint32_t idx);
#define ak_da_at_const(t, da, idx)          \
  ((t)*)ak_da_at_const_impl(da, idx)

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

#endif
