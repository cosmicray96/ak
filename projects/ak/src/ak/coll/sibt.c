#include "ak/coll/sibt.h"
#include "ak/coll/sla.h"
#include "ak/core/mem.h"
#include "ak/debug.h"
#include <X11/Xlib.h>
#include <string.h>

typedef struct
{
  ak_sibt_h p;
  ak_sibt_h c;
  ak_sibt_h s;
} hierarchy;

//===== ak_sibt =====//
//--- private ---//
static hierarchy*
get_hie_p(ak_sibt* t, ak_sibt_h h)
{
  void* ptr = ak_sla_at(&t->slots, h);
  return ptr;
}

static hierarchy*
get_item_p(ak_sibt* t, ak_sibt_h h)
{
  void* ptr = ak_sla_at(&t->slots, h);
  void* p =
    (void*)((uintptr_t)ptr +
            ak_align8(sizeof(hierarchy)));
  return p;
}

static void
set_item(ak_sibt* t,
         ak_sibt_h h,
         const void* item)
{
  void* ptr = get_item_p(t, h);
  memcpy(ptr, item, t->itemsize);
}

static uint32_t
get_slotsize(uint32_t itemsize)
{
  uint32_t size = 0;
  size += ak_align8(sizeof(hierarchy));
  size += ak_align8(itemsize);
  return size;
}

static void
add_dummy(ak_sibt* t)
{
  ak_sibt_h h =
    ak_sla_insert_empty(&t->slots);

  ak_assert(h == 0);

  hierarchy hie = { 0 };
  hie.c = 0;
  hie.p = 0;
  hie.s = 0;

  *get_hie_p(t, h) = hie;
}

static void
add_root(ak_sibt* t)
{
  ak_sibt_h h =
    ak_sla_insert_empty(&t->slots);
  t->root = h;

  hierarchy hie = { 0 };
  hie.c = 0;
  hie.p = 0;
  hie.s = 0;

  *get_hie_p(t, h) = hie;
}

//--- export ---//
ak_sibt
ak_sibt_make(uint32_t itemsize, ak_alct alct)
{
  ak_sibt t = { 0 };
  t.slots = ak_sla_make(
    get_slotsize(itemsize), alct);
  t.itemsize = itemsize;
  add_dummy(&t);
  add_root(&t);
  return t;
}

void
ak_sibt_destroy(ak_sibt* t)
{
  ak_sla_destroy(&t->slots);
  t->root = 0;
  t->itemsize = 0;
}

bool
ak_sibt_n_exist(ak_sibt* t, ak_sibt_h h)
{
  return ak_sla_exist(&t->slots, h);
}

ak_sibt_h
ak_sibt_n_parent(ak_sibt* t, ak_sibt_h h)
{
  ak_assert(ak_sibt_n_exist(t, h));

  hierarchy* hie = get_hie_p(t, h);
  return hie->p;
}

ak_sibt_h
ak_sibt_n_firstchild(ak_sibt* t, ak_sibt_h h)
{
  ak_assert(ak_sibt_n_exist(t, h));

  hierarchy* hie = get_hie_p(t, h);
  return hie->c;
}

ak_sibt_h
ak_sibt_n_nextsib(ak_sibt* t, ak_sibt_h h)
{
  ak_assert(ak_sibt_n_exist(t, h));

  hierarchy* hie = get_hie_p(t, h);
  return hie->s;
}

ak_sibt_h
ak_sibt_n_add(ak_sibt* t,
              ak_sibt_h p_h,
              const void* item)
{
  ak_assert(ak_sibt_n_exist(t, p_h));

  ak_sibt_h h =
    ak_sla_insert_empty(&t->slots);

  hierarchy* hie = 0;
  {
    hierarchy hie_itn = { 0 };
    hie_itn.p = p_h;
    hie_itn.s = 0;
    hie_itn.c = 0;
    hie = get_hie_p(t, h);
    *hie = hie_itn;
  }
  set_item(t, h, item);

  ak_sibt_h c_h =
    ak_sibt_n_firstchild(t, p_h);
  if (c_h) {
    hierarchy* p_hie = get_hie_p(t, p_h);
    p_hie->c = h;
    hie->s = c_h;
  } else {
    hierarchy* p_hie = get_hie_p(t, p_h);
    p_hie->c = h;
  }

  return h;
}

void
ak_sibt_n_remove(ak_sibt* t, ak_sibt_h h)
{
  ak_sibt_h c_h = ak_sibt_n_firstchild(t, h);
  while (c_h) {
    ak_sibt_n_remove(t, h);
    c_h = ak_sibt_n_nextsib(t, c_h);
  }

  ak_sla_remove(&t->slots, h);
}
