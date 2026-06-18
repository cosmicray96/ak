#include "ak/coll/dbuff.h"
#include "ak/core/mem/ptr.h"
#include "ak/debug.h"
#include <stdint.h>
#include <string.h>

//===== ak_dbuff =====//
//--- private ---//
static uint32_t
get_new_cap(uint32_t cur_cap,
            uint32_t cap_inc_by,
            float gr,
            uint32_t sc)
{
  if (cur_cap == 0) {
    return sc;
  }
  uint32_t min_cap = cur_cap + cap_inc_by;
  uint32_t cap = cur_cap;
  while (cap < min_cap) {
    cap = cap * gr;
  }
  return cap;
}

//--- export ---//
ak_dbuff
ak_dbuff_make(uint32_t is,
              float gr,
              uint32_t sc,
              ak_alct alct)
{
  ak_assert(gr > 1.0f);
  ak_assert(is > 0);
  ak_assert(sc > 0);

  ak_dbuff d;
  d.alct = alct;
  d.is = is;
  d.sc = sc;
  d.gr = gr;
  d.cap = sc;
  d.buff = ak_alct_alloc(alct, d.cap * d.is);
  return d;
}

ak_dbuff
ak_dbuff_from_ptr(void* data,
                  uint32_t is,
                  uint32_t count,
                  float gr,
                  ak_alct alct)
{
  ak_assert(gr > 1.0f);
  ak_assert(is > 0);
  ak_assert(count > 0);
  ak_assert(data);

  ak_dbuff d = { 0 };
  d.alct = alct;
  d.is = is;
  d.sc = count;
  d.gr = gr;
  d.cap = count;
  return d;
}

void
ak_dbuff_destroy(ak_dbuff* d)
{
  ak_alct_free(d->alct, d->buff);
  ak_alct_invalidate(&d->alct);
  d->is = 0;
  d->sc = 0;
  d->gr = 0;
  d->cap = 0;
  d->buff = 0;
}

void
ak_dbuff_at_copy(const ak_dbuff* d,
                 uint32_t idx,
                 void* o_item)
{
  ak_assert(idx < d->cap);

  const void* ptr =
    ak_dbuff_at_const(d, idx);
  memcpy(o_item, ptr, d->is);
}
void*
ak_dbuff_at(ak_dbuff* d, uint32_t idx)
{
  ak_assert(idx < d->cap);

  uint8_t* ptr = d->buff;
  ptr += (idx * d->is);
  return ptr;
}
const void*
ak_dbuff_at_const(const ak_dbuff* d,
                  uint32_t idx)
{
  ak_assert(idx < d->cap);

  const uint8_t* ptr = d->buff;
  ptr += (idx * d->is);
  return ptr;
}

void
ak_dbuff_overwrite_zero(ak_dbuff* d,
                        uint32_t idx)
{
  void* dest = ak_dbuff_at(d, idx);
  memset(dest, 0, d->is);
}

void
ak_dbuff_overwrite(ak_dbuff* d,
                   uint32_t idx,
                   const void* item)
{
  memcpy(ak_dbuff_at(d, idx), item, d->is);
}

void
ak_dbuff_swap(ak_dbuff* d,
              uint32_t idx1,
              uint32_t idx2)
{
  ak_assert(idx1 < d->cap);
  ak_assert(idx2 < d->cap);
  if (idx1 == idx2) {
    return;
  }
  ak_p_swap(ak_dbuff_at(d, idx1),
            ak_dbuff_at(d, idx2),
            d->is);
}

void
ak_dbuff_bulk_overwrite(ak_dbuff* dest,
                        const ak_dbuff* src,
                        uint32_t dest_idx,
                        uint32_t src_idx,
                        uint32_t src_count)
{
  ak_assert(dest_idx <= dest->cap);
  ak_assert(src_idx < src->cap);
  ak_assert(src_idx + src_count <= src->cap);
  ak_assert(dest->is == src->is);
  ak_assert(dest_idx + src_count <=
            dest->cap);

  if (src_count == 0) {
    return;
  }
  void* dest_ptr =
    ak_dbuff_at(dest, dest_idx);
  const void* src_ptr =
    ak_dbuff_at_const(src, src_idx);
  uint32_t size = dest->is * src_count;
  memmove(dest_ptr, src_ptr, size);
}

void
ak_dbuff_ensure_cap(ak_dbuff* d,
                    uint32_t min_cap)
{
  if (d->cap >= min_cap) {
    return;
  }
  ak_dbuff_cap_inc_w_copy(d,
                          min_cap - d->cap);
}

void
ak_dbuff_grow(ak_dbuff* d)
{
  uint32_t new_cap = d->cap * d->gr;

  ak_alct_free(d->alct, d->buff);

  d->buff =
    ak_alct_alloc(d->alct, new_cap * d->is);
  d->cap = new_cap;
}

void
ak_dbuff_cap_inc(ak_dbuff* d,
                 uint32_t inc_by)
{
  if (inc_by == 0) {
    return;
  }
  uint32_t new_cap = get_new_cap(
    d->cap, inc_by, d->gr, d->sc);

  ak_alct_free(d->alct, d->buff);

  d->buff =
    ak_alct_alloc(d->alct, new_cap * d->is);
  d->cap = new_cap;
}

void
ak_dbuff_grow_w_copy(ak_dbuff* d)
{
  uint32_t new_cap = d->cap * d->gr;
  void* new_buff =
    ak_alct_alloc(d->alct, new_cap * d->is);

  memcpy(new_buff, d->buff, d->cap * d->is);
  ak_alct_free(d->alct, d->buff);

  d->buff = new_buff;
  d->cap = new_cap;
}

ak_ex void
ak_dbuff_cap_inc_w_copy(ak_dbuff* d,
                        uint32_t inc_by)
{
  if (inc_by == 0) {
    return;
  }

  uint32_t new_cap = get_new_cap(
    d->cap, inc_by, d->gr, d->sc);
  void* new_buff =
    ak_alct_alloc(d->alct, new_cap * d->is);

  memcpy(new_buff, d->buff, d->cap * d->is);
  ak_alct_free(d->alct, d->buff);

  d->buff = new_buff;
  d->cap = new_cap;
}
