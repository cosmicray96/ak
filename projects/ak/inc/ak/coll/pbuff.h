#ifndef ak_coll_pbuff_h
#define ak_coll_pbuff_h

#include "ak/coll/hmn.h"
#include "ak/core/mem/allocator.h"
#include "ak/export.h"
typedef struct ak_pbuff ak_pbuff;
struct ak_pbuff
{
  ak_hmn pages;
  uint32_t itemsize;
};

ak_ex ak_pbuff
ak_pbuff_make(uint32_t itemsize,
              ak_alct alct);
ak_ex void
ak_pbuff_destroy(ak_pbuff* pb);

static ak_alct
ak_pbuff_alct(const ak_pbuff* pb)
{
  return ak_hmn_alct(&pb->pages);
}
static uint32_t
ak_pbuff_item(const ak_pbuff* pb)
{
  return pb->itemsize;
}

ak_ex void*
ak_pbuff_at(ak_pbuff* pb, uint32_t idx);
ak_ex void*
ak_pbuff_overwrite(ak_pbuff* pb,
                   uint32_t idx,
                   const void* item);

#endif
