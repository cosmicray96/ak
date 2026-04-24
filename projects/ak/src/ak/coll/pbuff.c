#include "ak/coll/pbuff.h"
#include "ak/core/mem/ptr.h"
#include "ak/debug.h"

//===== ak_pbuff =====//
//--- private ---//
#define s_page_size 512

static uint32_t
get_item_per_page(uint32_t itemsize)
{
  return s_page_size / itemsize;
}

void
ensure_page(ak_pbuff* pb, uint32_t page_idx)
{
  if (ak_hmn_exist_u64(&pb->pages,
                       page_idx)) {
    return;
  }

  void* page = ak_alct_alloc(
    ak_hmn_alct(&pb->pages), s_page_size);
  ak_hmn_insert_u64(
    &pb->pages, page_idx, &page);
}

//--- export ---//
ak_pbuff
ak_pbuff_make(uint32_t itemsize,
              ak_alct alct)
{
  ak_assert(itemsize <= s_page_size);

  ak_pbuff pb = { 0 };
  pb.itemsize = itemsize;
  pb.pages =
    ak_hmn_make(sizeof(void*), alct);

  return pb;
}

void
ak_pbuff_destroy(ak_pbuff* pb)
{
  ak_hmn_iter it =
    ak_hmn_iter_make(&pb->pages);
  ak_alct alct = ak_hmn_alct(&pb->pages);
  uint64_t key = 0;
  void* value = 0;
  while (ak_hmn_iter_next_u64(
    &it, &key, &value)) {
    ak_alct_free(alct, value);
  }
  ak_hmn_destroy(&pb->pages);
  pb->itemsize = 0;
}

void*
ak_pbuff_at(ak_pbuff* pb, uint32_t idx)
{
  uint32_t item_per_page =
    get_item_per_page(pb->itemsize);
  uint32_t page_idx = idx / item_per_page;
  uint32_t inpage_idx = idx % item_per_page;

  ensure_page(pb, page_idx);

  void* page =
    ak_hmn_at_u64(&pb->pages, page_idx);
  return ak_p_add(page,
                  inpage_idx * pb->itemsize);
}

void*
ak_pbuff_overwrite(ak_pbuff* pb,
                   uint32_t idx,
                   const void* item)
{
  void* ptr = ak_pbuff_at(pb, idx);
  ak_p_cpy(ptr, item, pb->itemsize);
  return ptr;
}
