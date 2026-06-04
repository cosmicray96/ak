#include "ak/coll/sla.h"
#include "ak/coll/dbuff.h"
#include "ak/core/mem.h"
#include "ak/core/mem/ptr.h"
#include "ak/debug.h"
#include <stdint.h>

//===== metadata =====//
//--- private ---//
typedef struct
{
  uint32_t nextfree;
  bool alive;
} metadata;

//===== ak_sla =====//
//--- private ---//

static metadata*
metadata_get(ak_sla* s, uint32_t idx)
{
  return ak_dbuff_at(&s->slots, idx);
}

static void
metadata_set(ak_sla* s,
             uint32_t idx,
             metadata md)
{
  *metadata_get(s, idx) = md;
}

static uint32_t
get_slotsize(uint32_t itemsize)
{
  uint32_t size = 0;
  size += ak_align8(sizeof(metadata));
  size += ak_align8(itemsize);
  return size;
}

static uint32_t
get_itemsize(ak_sla* s)
{
  return ak_dbuff_itemsize(&s->slots) -
         ak_align8(sizeof(metadata));
}

static void*
item_get(ak_sla* s, uint32_t idx)
{
  void* p = ak_dbuff_at(&s->slots, idx);
  p =
    ak_p_add(p, ak_align8(sizeof(metadata)));
  return p;
}

static void
item_set(ak_sla* s,
         uint32_t idx,
         const void* item)
{
  void* p = item_get(s, idx);
  ak_p_cpy(p, item, get_itemsize(s));
}

static void
init_slots(ak_sla* s, uint32_t start_idx)
{
  uint32_t cap = ak_dbuff_cap(&s->slots);
  ak_assert(start_idx < cap);
  for (uint32_t i = start_idx; i < cap;
       i++) {
    metadata* md = metadata_get(s, i);
    md->nextfree = i + 1;
    md->alive = false;
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

//--- export ---//
ak_sla
ak_sla_make(uint32_t itemsize, ak_alct alct)
{
  ak_sla s = { 0 };
  s.count = 0;
  s.slots = ak_dbuff_make(
    get_slotsize(itemsize), 1.5f, 10, alct);
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

void
ak_sla_clear(ak_sla* s)
{
  init_slots(s, 0);
  s->count = 0;
}

void*
ak_sla_at(ak_sla* s, uint32_t idx)
{
  return item_get(s, idx);
}

bool
ak_sla_exist(ak_sla* s, uint32_t idx)
{
  return metadata_get(s, idx)->alive;
}

uint32_t
ak_sla_insert_empty(ak_sla* s)
{
  resize_ifneed(s);

  uint32_t idx = s->free_head_idx;
  metadata* md = metadata_get(s, idx);
  md->alive = true;
  s->free_head_idx = md->nextfree;

  s->count++;
  return idx;
}

uint32_t
ak_sla_insert(ak_sla* s, const void* item)
{
  resize_ifneed(s);

  uint32_t idx = s->free_head_idx;
  metadata* md = metadata_get(s, idx);
  md->alive = true;
  s->free_head_idx = md->nextfree;

  item_set(s, idx, item);

  s->count++;
  return idx;
}

void
ak_sla_remove(ak_sla* s, uint32_t idx)
{
  metadata* md = metadata_get(s, idx);
  md->alive = false;
  md->nextfree = s->free_head_idx;
  s->free_head_idx = idx;
  s->count--;
}
