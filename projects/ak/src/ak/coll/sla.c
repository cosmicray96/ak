#include "ak/coll/sla.h"
#include "ak/coll/dbuff.h"
#include "ak/debug.h"
#include <stdint.h>

//===== ak_sla =====//
//--- private ---//

static uint32_t*
get_free_idx(ak_sla* s, uint32_t idx)
{
  return ak_dbuff_at(&s->slots, idx);
}

static void
init_slots(ak_sla* s, uint32_t start_idx)
{
  uint32_t cap = ak_dbuff_cap(&s->slots);
  ak_assert(start_idx < cap);
  for (uint32_t i = start_idx; i < cap;
       i++) {
    *get_free_idx(s, i) = i + 1;
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
  s.count = 0;
  s.slots =
    ak_dbuff_make(itemsize, 1.5f, 10, alct);
  init_slots(&s, 0);
  return s;
}

void
ak_sla_destroy(ak_sla* s)
{
  ak_dbuff_destroy(&s->slots);
  s->free_head_idx = 0;
  s->count = 0;
}

void*
ak_sla_at(ak_sla* s, uint32_t idx)
{
  return ak_dbuff_at(&s->slots, idx);
}

uint32_t
ak_sla_insert_empty(ak_sla* s)
{
  resize_ifneed(s);

  uint32_t idx = free_idx_pop(s);
  // set_item(s, idx, item);

  s->count++;
  return idx;
}

uint32_t
ak_sla_insert(ak_sla* s, const void* item)
{
  resize_ifneed(s);

  uint32_t idx = free_idx_pop(s);
  ak_dbuff_overwrite(&s->slots, idx, item);

  s->count++;
  return idx;
}

void
ak_sla_remove(ak_sla* s, uint32_t idx)
{
  free_idx_push(s, idx);
  s->count--;
}
