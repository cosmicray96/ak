#ifndef ak_coll_ds_h
#define ak_coll_ds_h

#include "ak/coll/da.h"
#include "ak/core/mem/allocator.h"
#include <stdbool.h>
typedef struct ak_ds ak_ds;
struct ak_ds
{
  ak_da a;
};

ak_ex ak_ds
ak_ds_make(uint32_t itemsize, ak_alct alct);
ak_ex void
ak_ds_destroy(ak_ds* s);
ak_ex void
ak_ds_clear(ak_ds* s);

ak_ex uint32_t
ak_ds_count(const ak_ds* s);
ak_ex void
ak_ds_push(ak_ds* s, const void* item);
ak_ex bool
ak_ds_pop(ak_ds* s, void* o_item);
ak_ex bool
ak_ds_peek(ak_ds* s,
           uint32_t idx,
           void* o_item);

#endif
