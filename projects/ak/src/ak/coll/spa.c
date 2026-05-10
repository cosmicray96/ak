#include "ak/coll/spa.h"
#include "ak/coll/da.h"
#include "ak/coll/pbuff.h"
#include "ak/debug.h"
#include <stdint.h>

//===== ak_spa =====//
//--- private ---//

static uint32_t*
get_s_from_d_idx_p(ak_spa* s, uint32_t d_idx)
{
  uint32_t* p =
    ak_da_at_impl(&s->d_to_s_idxs, d_idx);
  return p;
}
static uint32_t*
get_d_from_s_idx_p(ak_spa* s, uint32_t s_idx)
{
  uint32_t* p =
    ak_pbuff_at(&s->s_to_d_idxs, s_idx);
  return p;
}

static void
add_dummy(ak_spa* s)
{
  ak_assert(ak_da_count(&s->denses) == 0);
  ak_assert(ak_da_count(&s->d_to_s_idxs) ==
            0);
  ak_da_pushback_zero(&s->denses);
  ak_da_pushback_zero(&s->d_to_s_idxs);
}

//--- export ---//
ak_spa
ak_spa_make(uint32_t itemsize, ak_alct alct)
{
  ak_spa s = { 0 };
  s.s_to_d_idxs =
    ak_pbuff_make(sizeof(uint32_t), alct);
  s.denses = ak_da_make(itemsize, alct);
  s.d_to_s_idxs =
    ak_da_make(sizeof(uint32_t), alct);
  add_dummy(&s);
  return s;
}

void
ak_spa_destroy(ak_spa* s)
{
  ak_pbuff_destroy(&s->s_to_d_idxs);
  ak_da_destroy(&s->d_to_s_idxs);
  ak_da_destroy(&s->denses);
}

void
ak_spa_clear(ak_spa* s)
{
  uint32_t count = ak_da_count(&s->denses);
  for (uint32_t i = 1; i < count; i++) {
    uint32_t* sidx =
      get_s_from_d_idx_p(s, i);
    *get_d_from_s_idx_p(s, *sidx) = 0;
  }
  ak_da_clear(&s->denses);
  ak_da_clear(&s->d_to_s_idxs);
  add_dummy(s);
}

uint32_t
ak_spa_count(const ak_spa* s)
{
  return ak_da_count(&s->denses) - 1;
}

bool
ak_spa_exist(ak_spa* s, uint32_t s_idx)
{
  ak_assert(s_idx);
  uint32_t* d_idx_p =
    get_d_from_s_idx_p(s, s_idx);
  return *d_idx_p != 0;
}

void*
ak_spa_at(ak_spa* s, uint32_t s_idx)
{
  ak_assert(ak_spa_exist(s, s_idx));

  uint32_t d_idx =
    *get_d_from_s_idx_p(s, s_idx);
  return ak_da_at_impl(&s->denses, d_idx);
}

void
ak_spa_insert(ak_spa* s,
              uint32_t s_idx,
              const void* item)
{
  ak_assert(!ak_spa_exist(s, s_idx));

  uint32_t d_idx = ak_da_count(&s->denses);

  ak_da_pushback(&s->denses, item);
  ak_da_pushback(&s->d_to_s_idxs, &s_idx);

  *(uint32_t*)ak_pbuff_at(&s->s_to_d_idxs,
                          s_idx) = d_idx;
}

void
ak_spa_overwrite(ak_spa* s,
                 uint32_t s_idx,
                 const void* item)
{
  ak_assert(ak_spa_exist(s, s_idx));

  uint32_t* d_idx_p =
    get_d_from_s_idx_p(s, s_idx);
  ak_da_overwrite(
    &s->denses, *d_idx_p, item);
}

void
ak_spa_remove(ak_spa* s, uint32_t s_idx)
{
  ak_assert(ak_spa_exist(s, s_idx));

  uint32_t d_idx =
    *get_d_from_s_idx_p(s, s_idx);

  uint32_t last_d_idx =
    ak_da_count(&s->denses) - 1;

  uint32_t last_s_idx =
    *get_s_from_d_idx_p(s, last_d_idx);

  *get_d_from_s_idx_p(s, s_idx) = 0;

  if (d_idx != last_d_idx) {
    *get_d_from_s_idx_p(s, last_s_idx) =
      d_idx;
  }

  ak_da_remove_swaplast(&s->denses, d_idx);
  ak_da_remove_swaplast(&s->d_to_s_idxs,
                        d_idx);
}

ak_ex void*
ak_spa_at_didx(ak_spa* s,
               uint32_t idx,
               uint32_t* o_s_idx)
{
  ak_assert(idx < ak_spa_count(s));
  uint32_t d_idx = idx + 1;
  *o_s_idx = *get_s_from_d_idx_p(s, d_idx);
  ak_assert(ak_spa_exist(s, *o_s_idx));

  return ak_da_at_impl(&s->denses, d_idx);
}
