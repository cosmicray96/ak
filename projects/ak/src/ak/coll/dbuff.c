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
ak_dbuff_make(float gr,
              uint32_t sc,
              ak_alct alct)
{
  ak_dbuff d;
  d.alct = alct;
  d.sc = sc;
  d.gr = gr;
  d.cap = sc;
  d.buff = ak_alct_alloc(alct, d.cap);
  return d;
}

void
ak_dbuff_destroy(ak_dbuff* d)
{
  ak_alct_free(d->alct, d->buff);
  ak_alct_invalidate(&d->alct);
  d->sc = 0;
  d->gr = 0;
  d->cap = 0;
  d->buff = 0;
}

void*
ak_dbuff_at(ak_dbuff* d, uint32_t idx)
{
  uint8_t* ptr = d->buff;
  ptr += idx;
  return ptr;
}

void
ak_dbuff_grow(ak_dbuff* d)
{
  uint32_t new_cap = d->cap * d->gr;

  ak_alct_free(d->alct, d->buff);

  d->buff = ak_alct_alloc(d->alct, new_cap);
  d->cap = new_cap;
}

void
ak_dbuff_cap_inc(ak_dbuff* d,
                 uint32_t inc_by)
{
  uint32_t new_cap =
    get_new_cap(d->cap, inc_by, d->gr);

  ak_alct_free(d->alct, d->buff);

  d->buff = ak_alct_alloc(d->alct, new_cap);
  d->cap = new_cap;
}

void
ak_dbuff_grow_w_copy(ak_dbuff* d)
{
  uint32_t new_cap = d->cap * d->gr;
  void* new_buff =
    ak_alct_alloc(d->alct, new_cap);

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
    ak_alct_alloc(d->alct, new_cap);

  memcpy(new_buff, d->buff, d->cap);
  ak_alct_free(d->alct, d->buff);

  d->buff = new_buff;
  d->cap = new_cap;
}
