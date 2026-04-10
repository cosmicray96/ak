#include "ak/coll/dbuff.h"
#include <stdint.h>
#include <string.h>

//===== ak_dbuff =====//
//--- private ---//
static uint32_t
get_new_cap(uint32_t cur_cap,
            uint32_t cap_inc_by,
            float gr)
{
  uint32_t min_cap = cur_cap + cap_inc_by;
  uint32_t cap = cur_cap;
  while (cap < min_cap) {
    cap = cap * gr;
  }
  return cap;
}

//--- public ---//
ak_dbuff
ak_dbuff_make(uint32_t is,
              float gr,
              uint32_t sc,
              ak_alct alct)
{
  ak_dbuff d;
  d.alct = alct;
  d.is = is;
  d.sc = sc;
  d.gr = gr;
  d.cap = sc;
  d.buff = ak_alct_alloc(alct, d.cap * d.is);
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

ak_alct
ak_dbuff_alct(const ak_dbuff* d)
{
  return d->alct;
}
uint32_t
ak_dbuff_cap(const ak_dbuff* d)
{
  return d->cap;
}
uint32_t
ak_dbuff_itemsize(const ak_dbuff* d)
{
  return d->is;
}

void*
ak_dbuff_at(ak_dbuff* d, uint32_t idx)
{
  uint8_t* ptr = d->buff;
  ptr += (idx * d->is);
  return ptr;
}
const void*
ak_dbuff_at_const(const ak_dbuff* d,
                  uint32_t idx)
{

  const uint8_t* ptr = d->buff;
  ptr += (idx * d->is);
  return ptr;
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
  uint32_t new_cap =
    get_new_cap(d->cap, inc_by, d->gr);

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

  memcpy(new_buff, d->buff, d->cap);
  ak_alct_free(d->alct, d->buff);

  d->buff = new_buff;
  d->cap = new_cap;
}

ak_ex void
ak_dbuff_cap_inc_w_copy(ak_dbuff* d,
                        uint32_t inc_by)
{
  uint32_t new_cap =
    get_new_cap(d->cap, inc_by, d->gr);
  void* new_buff =
    ak_alct_alloc(d->alct, new_cap * d->is);

  memcpy(new_buff, d->buff, d->cap);
  ak_alct_free(d->alct, d->buff);

  d->buff = new_buff;
  d->cap = new_cap;
}
