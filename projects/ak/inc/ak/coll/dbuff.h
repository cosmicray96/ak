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

ak_ex ak_alct
ak_dbuff_alct(const ak_dbuff* d);
ak_ex uint32_t
ak_dbuff_cap(const ak_dbuff* d);
ak_ex uint32_t
ak_dbuff_itemsize(const ak_dbuff* d);

ak_ex void*
ak_dbuff_at(ak_dbuff* d, uint32_t idx);
ak_ex const void*
ak_dbuff_at_const(const ak_dbuff* d,
                  uint32_t idx);

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
