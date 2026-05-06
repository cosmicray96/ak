#ifndef ak_coll_dq_h
#define ak_coll_dq_h

#include "ak/coll/dbuff.h"
#include "ak/core/mem/allocator.h"
#include <stdbool.h>
typedef struct ak_dq ak_dq;
struct ak_dq
{
  ak_dbuff dbuff;
  uint32_t ridx;
  uint32_t widx;
  uint32_t count;
};

ak_ex ak_dq
ak_dq_make(uint32_t itemsize,
           uint32_t starting_cap,
           ak_alct alct);
ak_ex void
ak_dq_destroy(ak_dq* q);

ak_ex uint32_t
ak_dq_count(const ak_dq* q);
ak_ex void
ak_dq_push(ak_dq* q, const void* item);
ak_ex bool
ak_dq_pop(ak_dq* q, void* item);
ak_ex bool
ak_dq_peek(ak_dq* q,
           uint32_t idx,
           void* item);

#endif
