#ifndef ak_da_h
#define ak_da_h

#include "ak/coll/dbuff.h"
#include "ak/export.h"
#include <stdbool.h>
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
ak_ex void
ak_da_reserve(ak_da* da, uint32_t count);

static ak_alct
ak_da_alct(const ak_da* da)
{
  return ak_dbuff_alct(&da->dbuff);
}

static uint32_t
ak_da_itemsize(const ak_da* da)
{
  return ak_dbuff_itemsize(&da->dbuff);
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

static void*
ak_da_ptr(ak_da* da)
{
  return ak_dbuff_ptr(&da->dbuff);
}

ak_ex void
ak_da_overwrite_zero(ak_da* da,
                     uint32_t idx);
ak_ex void*
ak_da_pushback_zero(ak_da* da);

ak_ex void
ak_da_at_copy(const ak_da* da,
              uint32_t idx,
              void* o_item);

ak_ex void*
ak_da_at(ak_da* da, uint32_t idx);
ak_ex const void*
ak_da_at_const(const ak_da* da,
               uint32_t idx);

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

ak_ex void
ak_da_remove_swaplast(ak_da* da,
                      uint32_t idx);

ak_ex void
ak_da_remove_range(ak_da* da,
                   uint32_t idx,
                   uint32_t count);

typedef bool (*ak_equal_fn)(const void* a,
                            const void* b);
ak_ex bool
ak_da_findfirst(ak_da* da,
                const void* item,
                uint32_t* idx,
                ak_equal_fn fn);

ak_ex void
ak_da_sort(ak_da* da,
           bool (*sort_fn)(const void* a,
                           const void* b));

ak_ex void
ak_da_bulk_pushback(ak_da* dest,
                    const ak_da* src,
                    uint32_t src_idx,
                    uint32_t src_count);

#endif
