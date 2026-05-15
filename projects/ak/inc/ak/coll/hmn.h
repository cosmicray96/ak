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
ak_hmn_exist(ak_hmn* h, uint64_t key);
ak_ex void*
ak_hmn_at(ak_hmn* h, uint64_t key);
ak_ex void
ak_hmn_insert(ak_hmn* h,
              uint64_t key,
              const void* value);
ak_ex void
ak_hmn_remove(ak_hmn* h, uint64_t key);

typedef struct
{
  ak_hmn* h;
  uint32_t idx;
} ak_hmn_iter;

ak_ex ak_hmn_iter
ak_hmn_iter_make(ak_hmn* h);
ak_ex bool
ak_hmn_iter_next(ak_hmn_iter* it,
                 uint64_t* o_key,
                 void** o_value);

#endif
