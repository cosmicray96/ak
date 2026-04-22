#include "ak/coll/sibt.h"
#include "ak/coll/sla.h"
#include "ak/core/mem.h"
#include "ak/core/mem/ptr.h"
#include "ak/debug.h"

typedef struct
{
  ak_sibt_h p;
  ak_sibt_h c;
  ak_sibt_h s;
} link;

//===== ak_sibt =====//
//--- private ---//
static link*
get_l_p(ak_sibt* t, ak_sibt_h h)
{
  void* ptr = ak_sla_at(&t->slots, h);
  return ptr;
}

static link*
get_item_p(ak_sibt* t, ak_sibt_h h)
{
  void* ptr = ak_sla_at(&t->slots, h);
  return ak_p_add(ptr,
                  ak_align8(sizeof(link)));
}

static void
set_item(ak_sibt* t,
         ak_sibt_h h,
         const void* item)
{
  void* ptr = get_item_p(t, h);
  ak_p_cpy(ptr, item, t->itemsize);
}

static uint32_t
get_slotsize(uint32_t itemsize)
{
  uint32_t size = 0;
  size += ak_align8(sizeof(link));
  size += ak_align8(itemsize);
  return size;
}

static void
add_dummy(ak_sibt* t)
{
  ak_sibt_h h =
    ak_sla_insert_empty(&t->slots);

  ak_assert(h == 0);

  link l = { 0 };
  l.c = 0;
  l.p = 0;
  l.s = 0;

  *get_l_p(t, h) = l;
}

static void
add_root(ak_sibt* t)
{
  ak_sibt_h h =
    ak_sla_insert_empty(&t->slots);

  ak_assert(h);

  t->root = h;

  link l = { 0 };
  l.c = 0;
  l.p = 0;
  l.s = 0;

  *get_l_p(t, h) = l;
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

ak_sibt_h
ak_sibt_n_parent(ak_sibt* t, ak_sibt_h h)
{
  link* l = get_l_p(t, h);
  return l->p;
}

ak_sibt_h
ak_sibt_n_firstchild(ak_sibt* t, ak_sibt_h h)
{
  link* l = get_l_p(t, h);
  return l->c;
}

ak_sibt_h
ak_sibt_n_nextsib(ak_sibt* t, ak_sibt_h h)
{
  link* l = get_l_p(t, h);
  return l->s;
}

ak_sibt_h
ak_sibt_n_add(ak_sibt* t,
              ak_sibt_h p_h,
              const void* item)
{

  ak_sibt_h h =
    ak_sla_insert_empty(&t->slots);

  link* l = 0;
  {
    link l_itn = { 0 };
    l_itn.p = p_h;
    l_itn.s = 0;
    l_itn.c = 0;
    l = get_l_p(t, h);
    *l = l_itn;
  }
  set_item(t, h, item);

  ak_sibt_h c_h =
    ak_sibt_n_firstchild(t, p_h);
  if (c_h) {
    link* p_l = get_l_p(t, p_h);
    p_l->c = h;
    l->s = c_h;
  } else {
    link* p_l = get_l_p(t, p_h);
    p_l->c = h;
  }

  return h;
}

void
ak_sibt_n_remove(ak_sibt* t, ak_sibt_h h)
{
  ak_assert(h != t->root);

  ak_sibt_h c_h = ak_sibt_n_firstchild(t, h);
  while (c_h) {
    ak_sibt_n_remove(t, h);
    c_h = ak_sibt_n_nextsib(t, c_h);
  }

  ak_sla_remove(&t->slots, h);
}
