#ifndef ak_coll_sla_h
#define ak_coll_sla_h

#include "ak/coll/dbuff.h"
#include "ak/core/mem/allocator.h"
#include "ak/export.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct ak_sla ak_sla;
struct ak_sla
{
  ak_dbuff slots;
  uint32_t free_head_idx;
  uint32_t count;
};

ak_ex ak_sla
ak_sla_make(uint32_t itemsize, ak_alct alct);
ak_ex void
ak_sla_destroy(ak_sla* s);

ak_ex void
ak_sla_clear(ak_sla* s);

static uint32_t
ak_sla_itemsize(const ak_sla* s)
{
  return ak_dbuff_itemsize(&s->slots);
}

ak_ex void*
ak_sla_at(ak_sla* s, uint32_t idx);
ak_ex bool
ak_sla_exist(ak_sla* s, uint32_t idx);

ak_ex uint32_t
ak_sla_insert_empty(ak_sla* s);
ak_ex uint32_t
ak_sla_insert(ak_sla* s, const void* item);
ak_ex void
ak_sla_remove(ak_sla* s, uint32_t h);

#endif
