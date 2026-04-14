#include "ak/coll/hmn.h"
#include "ak/coll/dbuff.h"
#include "ak/core/errcode.h"
#include "ak/debug.h"
#include <stdint.h>

#define s_gr 2
#define s_sc 16
#define s_resizeper 0.7f

//===== item =====//
//--- private ---//
typedef struct
{
  uint64_t key;
  enum
  {
    empty,
    alive,
    dead
  } state;
} item;

//===== ak_hmn =====//
//--- private ---//

static uint32_t
get_cap(const ak_hmn* h)
{
  return ak_dbuff_cap(&h->values);
}

static uint64_t
get_hash(uint64_t key)
{
  uint64_t x = key;
  x ^= x >> 30;
  x *= 0xbf58476d1ce4e5b9ULL;
  x ^= x >> 27;
  x *= 0x94d049bb133111ebULL;
  x ^= x >> 31;
  return x;
}
static uint32_t
get_idx(ak_hmn* h, uint64_t hash)
{
  uint32_t cap = get_cap(h);
  return (uint32_t)(hash % (uint64_t)cap);
}

static void
keystates_empty(ak_hmn* h)
{
  uint32_t cap = get_cap(h);
  for (uint32_t i = 0; i < cap; i++) {
    item* itm =
      ak_dbuff_at(&h->keystates, i);
    itm->state = empty;
  }
}

static ak_hmn
ak_hmn_make_startingcount(
  uint32_t valuesize,
  uint32_t startingcount,
  ak_alct alct)
{
  ak_hmn h = { 0 };
  h.keystates = ak_dbuff_make(
    sizeof(item), s_gr, startingcount, alct);
  h.values = ak_dbuff_make(
    valuesize, s_gr, startingcount, alct);
  h.count = 0;
  h.dead_count = 0;

  keystates_empty(&h);

  return h;
}

static void
resize_ifneed(ak_hmn* h)
{
  uint32_t cap = get_cap(h);
  if ((h->count + h->dead_count) <
      s_resizeper * cap) {
    return;
  }

  uint32_t new_cap = 0;

  if (h->dead_count > h->count) {
    new_cap = cap;
  } else {
    new_cap = cap * s_gr;
  }

  ak_hmn new_h = ak_hmn_make_startingcount(
    ak_hmn_valuesize(h),
    new_cap,
    ak_hmn_alct(h));

  for (uint32_t i = 0; i < cap; i++) {
    const item* itm =
      ak_dbuff_at_const(&h->keystates, i);
    if (itm->state != alive) {
      continue;
    }

    const void* value =
      ak_dbuff_at_const(&h->values, i);
    ak_hmn_insert_u64(
      &new_h, itm->key, value);
  }

  ak_hmn_destroy(h);
  *h = new_h;
}

//--- export ---//

ak_hmn
ak_hmn_make(uint32_t valuesize, ak_alct alct)
{
  return ak_hmn_make_startingcount(
    valuesize, s_sc, alct);
}

void
ak_hmn_destroy(ak_hmn* h)
{
  ak_dbuff_destroy(&h->values);
  ak_dbuff_destroy(&h->keystates);
  h->count = 0;
  h->dead_count = 0;
}

void*
ak_hmn_at_itn_u64(ak_hmn* h, uint64_t key)
{
  uint64_t hash = get_hash(key);
  uint32_t idx = get_idx(h, hash);
  uint32_t cap = get_cap(h);

  for (uint32_t i = 0; i < cap;
       i++, idx = (idx + 1) % cap) {
    item* itm =
      ak_dbuff_at(&h->keystates, idx);
    if (itm->state == alive) {
      if (itm->key == key) {
        return ak_dbuff_at(&h->values, idx);
      }
      continue;
    } else if (itm->state == dead) {
      continue;
    }
    return 0;
  }
  return 0;
}

void*
ak_hmn_at_u64(ak_hmn* h, uint64_t key)
{
  void* ptr = ak_hmn_at_itn_u64(h, key);
  ak_log("ptr: %ull", ptr);
  ak_assert(ptr);
  return ptr;
}

ak_ex bool
ak_hmn_exist_u64(ak_hmn* h, uint64_t key)
{
  void* ptr = ak_hmn_at_itn_u64(h, key);
  return ptr != 0;
}

void
ak_hmn_insert_u64(ak_hmn* h,
                  uint64_t key,
                  const void* value)
{
  resize_ifneed(h);
  uint64_t hash = get_hash(key);
  uint32_t idx = get_idx(h, hash);
  uint32_t cap = ak_dbuff_cap(&h->values);
  int64_t dead_idx = -1;

  for (uint32_t i = 0; i < cap;
       i++, idx = (idx + 1) % cap) {
    item* itm =
      ak_dbuff_at(&h->keystates, idx);

    if (itm->state == alive) {
      if (itm->key == key) {
        ak_dbuff_overwrite(
          &h->values, idx, value);
        return;
      }
      continue;

    } else if (itm->state == dead) {
      if (dead_idx == -1)
        dead_idx = idx;
      continue;
    }

    if (dead_idx != -1) {
      idx = dead_idx;
    }
    item new_itm = { .key = key,
                     .state = alive };

    ak_dbuff_overwrite(
      &h->keystates, idx, &new_itm);
    ak_dbuff_overwrite(
      &h->values, idx, value);
    h->count++;
    return;
  }
}

void
ak_hmn_remove_u64(ak_hmn* h, uint64_t key)
{
  uint64_t hash = get_hash(key);
  uint32_t idx = get_idx(h, hash);
  uint32_t cap = ak_dbuff_cap(&h->values);

  for (uint32_t i = 0; i < cap;
       i++, idx = (idx + 1) % cap) {
    item* itm =
      ak_dbuff_at(&h->keystates, idx);
    if (itm->state == alive) {
      if (itm->key == key) {
        itm->state = dead;
        h->count--;
        h->dead_count++;
        return;
      }
      continue;
    } else if (itm->state == dead) {
      continue;
    }
    // empty
    ak_ec(ak_err_removed_remove);
  }
}
