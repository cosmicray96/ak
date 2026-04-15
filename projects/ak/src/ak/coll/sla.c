#include "ak/coll/sla.h"
#include "ak/coll/dbuff.h"
#include "ak/debug.h"
#include <stdint.h>
#include <string.h>

//===== ak_sla =====//
//--- private ---//
#define s_item_pos 8

static ak_sla_h
handle_make(uint32_t idx, uint32_t gen)
{
  return ((uint64_t)gen << 32) | idx;
}

static uint32_t
get_idx(ak_sla_h h)
{
  return (uint32_t)(h & 0xFFFFFFFFULL);
}
static uint32_t
get_gen(ak_sla_h h)
{
  return (uint32_t)(h >> 32);
}

static uint32_t*
get_gen_p(ak_sla* s, uint32_t idx)
{
  return ak_dbuff_at_t(
    uint32_t, &s->slots, idx);
}

static void
gen_inc(ak_sla* s, ak_sla_h h)
{
  (*get_gen_p(s, get_idx(h)))++;
}

static void*
get_item(ak_sla* s, uint32_t idx)
{
  void* ptr = ak_dbuff_at(&s->slots, idx);
  return (void*)((uintptr_t)ptr +
                 s_item_pos);
}
static void
set_item(ak_sla* s,
         uint32_t idx,
         const void* item)
{
  void* ptr = get_item(s, idx);
  memcpy(ptr, item, s->itemsize);
}

static uint32_t*
get_free_idx(ak_sla* s, uint32_t idx)
{
  return (uint32_t*)get_item(s, idx);
}

static uint32_t
slot_size(uint32_t itemsize)
{
  return (sizeof(ak_sla_h) + itemsize + 7) &
         ~7;
}

static void
init_slots(ak_sla* s, uint32_t start_idx)
{
  uint32_t cap = ak_dbuff_cap(&s->slots);
  ak_assert(start_idx < cap);
  for (uint32_t i = start_idx; i < cap;
       i++) {
    *get_gen_p(s, i) = 0;

    uint32_t* free_idx = get_free_idx(s, i);
    *free_idx = i + 1;
  }
  s->free_head_idx = start_idx;
}

static void
resize_ifneed(ak_sla* s)
{
  if (s->count < ak_dbuff_cap(&s->slots)) {
    return;
  }
  uint32_t old_cap = ak_dbuff_cap(&s->slots);
  ak_dbuff_grow_w_copy(&s->slots);
  init_slots(s, old_cap);
}

static uint32_t
free_idx_pop(ak_sla* s)
{
  ak_assert(s->count <
            ak_dbuff_cap(&s->slots));

  uint32_t free_idx = s->free_head_idx;

  s->free_head_idx =
    *get_free_idx(s, s->free_head_idx);

  return free_idx;
}

static void
free_idx_push(ak_sla* s, uint32_t idx)
{
  *get_free_idx(s, idx) = s->free_head_idx;
  s->free_head_idx = idx;
}

//--- export ---//
ak_sla
ak_sla_make(uint32_t itemsize, ak_alct alct)
{
  ak_sla s = { 0 };
  s.itemsize = itemsize;
  s.count = 0;
  s.slots = ak_dbuff_make(
    slot_size(itemsize), 1.5f, 10, alct);
  init_slots(&s, 0);
  return s;
}

void
ak_sla_destroy(ak_sla* s)
{
  ak_dbuff_destroy(&s->slots);
  s->free_head_idx = 0;
  s->count = 0;
  s->itemsize = 0;
}

bool
ak_sla_exist(ak_sla* s, ak_sla_h h)
{
  return (*get_gen_p(s, get_idx(h))) ==
         get_gen(h);
}

void*
ak_sla_at(ak_sla* s, ak_sla_h h)
{
  ak_assert(ak_sla_exist(s, h));
  return get_item(s, get_idx(h));
}

ak_sla_h
ak_sla_insert(ak_sla* s, const void* item)
{
  resize_ifneed(s);

  uint32_t idx = free_idx_pop(s);
  set_item(s, idx, item);

  s->count++;
  return handle_make(*get_gen_p(s, idx),
                     idx);
}

void
ak_sla_remove(ak_sla* s, ak_sla_h h)
{
  ak_assert(s->count > 0);
  ak_assert(ak_sla_exist(s, h));
  gen_inc(s, h);
  free_idx_push(s, get_idx(h));
  s->count--;
}
