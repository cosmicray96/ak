#ifndef ak_dbuff_h
#define ak_dbuff_h

#include "ak/core/mem/allocator.h"
#include "ak/export.h"

#include <stdint.h>

#define ak_dbuff_fields

typedef struct
{
  ak_alct alct;
  void* buff;
  uint32_t cap; // capacity
  float gr;     // growth rate
  uint32_t sc;  // starting capacity
  uint32_t is;  // itemsize
} ak_dbuff;

ak_ex ak_dbuff
ak_dbuff_make(uint32_t is,
              float gr,
              uint32_t sc,
              ak_alct alct);
ak_ex void
ak_dbuff_destroy(ak_dbuff* d);

static void*
ak_dbuff_ptr(ak_dbuff* d)
{
  return d->buff;
}
static ak_alct
ak_dbuff_alct(const ak_dbuff* d)
{
  return d->alct;
}
static uint32_t
ak_dbuff_cap(const ak_dbuff* d)
{
  return d->cap;
}
static uint32_t
ak_dbuff_itemsize(const ak_dbuff* d)
{
  return d->is;
}
static uint32_t
ak_dbuff_growthrate(const ak_dbuff* d)
{
  return d->gr;
}

ak_ex void
ak_dbuff_at_copy(const ak_dbuff* d,
                 uint32_t idx,
                 void* item);
ak_ex void*
ak_dbuff_at(ak_dbuff* d, uint32_t idx);
ak_ex const void*
ak_dbuff_at_const(const ak_dbuff* d,
                  uint32_t idx);

ak_ex void
ak_dbuff_overwrite(ak_dbuff* d,
                   uint32_t idx,
                   const void* item);

ak_ex void
ak_dbuff_bulk_overwrite(ak_dbuff* dest,
                        const ak_dbuff* src,
                        uint32_t dest_idx,
                        uint32_t src_idx,
                        uint32_t src_count);

ak_ex void
ak_dbuff_ensure_cap(ak_dbuff* d,
                    uint32_t min_cap);
ak_ex void
ak_dbuff_grow(ak_dbuff* d);
ak_ex void
ak_dbuff_cap_inc(ak_dbuff* d,
                 uint32_t inc_by);
ak_ex void
ak_dbuff_grow_w_copy(ak_dbuff* d);
ak_ex void
ak_dbuff_cap_inc_w_copy(ak_dbuff* d,
                        uint32_t inc_by);

#endif
