#ifndef ak_coll_hmn_h
#define ak_coll_hmn_h

#include "ak/coll/dbuff.h"
#include "ak/export.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct ak_hmn ak_hmn;
struct ak_hmn
{
  ak_dbuff keystates;
  ak_dbuff values;

  uint32_t count;
  uint32_t dead_count;
};

ak_ex ak_hmn
ak_hmn_make(uint32_t valuesize,
            ak_alct alct);
ak_ex void
ak_hmn_destroy(ak_hmn* h);

static ak_alct
ak_hmn_alct(const ak_hmn* h)
{
  return ak_dbuff_alct(&h->values);
}
static uint32_t
ak_hmn_valuesize(const ak_hmn* h)
{
  return ak_dbuff_itemsize(&h->values);
}
static uint32_t
ak_hmn_count(const ak_hmn* h)
{
  return h->count;
}

ak_ex bool
ak_hmn_exist_u64(ak_hmn* h, uint64_t key);
ak_ex void*
ak_hmn_at_u64(ak_hmn* h, uint64_t key);
ak_ex void
ak_hmn_insert_u64(ak_hmn* h,
                  uint64_t key,
                  const void* value);
ak_ex void
ak_hmn_remove_u64(ak_hmn* h, uint64_t key);

static void*
ak_hmn_at_i64(ak_hmn* h, int64_t key)
{
  return ak_hmn_at_u64(h, (uint64_t)key);
}
static void
ak_hmn_insert_i64(ak_hmn* h,
                  int64_t key,
                  const void* value)
{
  ak_hmn_insert_u64(h, (uint64_t)key, value);
}
static void
ak_hmn_remove_i64(ak_hmn* h, int64_t key)
{
  ak_hmn_remove_u64(h, (uint64_t)key);
}

#define ak_hmn_at_u64_t(t, da, key)         \
  (t*)ak_hmn_at_u64(da, key)

#endif
