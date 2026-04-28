#include "ak/coll/spa.h"
#include "ak/coll/da.h"
#include "ak/coll/pbuff.h"
#include "ak/core/mem.h"
#include "ak/core/mem/ptr.h"
#include "ak/debug.h"
#include <stdint.h>

//===== ak_spa =====//
//--- private ---//

static uint32_t*
get_s_from_d_idx_p(ak_spa* s, uint32_t d_idx)
{
  uint32_t* p =
    ak_da_at(uint32_t, &s->denses, d_idx);
  return p;
}
static uint32_t*
get_d_from_s_idx_p(ak_spa* s, uint32_t s_idx)
{
  uint32_t* p =
    ak_pbuff_at(&s->s_to_d_idxs, s_idx);
  return p;
}

static void*
get_item_p(ak_spa* s, uint32_t d_idx)
{
  void* p =
    ak_da_at(void, &s->denses, d_idx);
  ak_p_add(p, ak_align8(sizeof(uint32_t)));
  return p;
}

static void
set_item(ak_spa* s,
         uint32_t d_idx,
         const void* item)
{
  void* p = get_item_p(s, d_idx);
  ak_p_cpy(p, item, s->itemsize);
}

static uint32_t*
get_d_idx_last(ak_spa* s)
{
  uint32_t d_idx_l = ak_da_count(&s->denses);
  uint32_t* s_idx_l =
    get_s_from_d_idx_p(s, d_idx_l);
  return get_d_from_s_idx_p(s, *s_idx_l);
}

static uint32_t*
get_s_idx_last(ak_spa* s)
{
  uint32_t d_idx_l = ak_da_count(&s->denses);
  uint32_t* s_idx_l =
    get_s_from_d_idx_p(s, d_idx_l);
  return s_idx_l;
}

static uint32_t
get_slotsize(uint32_t itemsize)
{
  uint32_t size = 0;
  size += ak_align8(sizeof(uint32_t));
  size += itemsize;
  return size;
}
static void
add_dummy(ak_spa* s)
{
  ak_assert(ak_da_count(&s->denses) == 0);
  ak_da_pushback_zero(&s->denses);
}

//--- export ---//
ak_spa
ak_spa_make(uint32_t itemsize, ak_alct alct)
{
  ak_spa s = { 0 };
  s.s_to_d_idxs =
    ak_pbuff_make(sizeof(uint32_t), alct);
  s.denses =
    ak_da_make(get_slotsize(itemsize), alct);
  s.itemsize = itemsize;
  add_dummy(&s);
  return s;
}

void
ak_spa_destroy(ak_spa* s)
{
  ak_pbuff_destroy(&s->s_to_d_idxs);
  ak_da_destroy(&s->denses);
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
  return get_item_p(s, d_idx);
}

void
ak_spa_insert(ak_spa* s,
              uint32_t s_idx,
              const void* item)
{
  ak_assert(!ak_spa_exist(s, s_idx));

  uint32_t d_idx = ak_da_count(&s->denses);
  ak_da_pushback_zero(&s->denses);

  *get_d_from_s_idx_p(s, s_idx) = d_idx;
  *get_s_from_d_idx_p(s, d_idx) = s_idx;
  set_item(s, d_idx, item);
}

void
ak_spa_overwrite(ak_spa* s,
                 uint32_t s_idx,
                 const void* item)
{
  ak_assert(ak_spa_exist(s, s_idx));

  uint32_t* d_idx_p =
    get_d_from_s_idx_p(s, s_idx);
  set_item(s, *d_idx_p, item);
}

void
ak_spa_remove(ak_spa* s, uint32_t s_idx)
{
  ak_assert(ak_spa_exist(s, s_idx));

  uint32_t* d_idx =
    get_d_from_s_idx_p(s, s_idx);
  uint32_t* s_idx_p =
    get_s_from_d_idx_p(s, *d_idx);

  *s_idx_p = 0;

  uint32_t* d_idx_l = get_d_idx_last(s);
  *d_idx_l = *d_idx;

  ak_da_remove_swaplast(&s->denses, *d_idx);
}
