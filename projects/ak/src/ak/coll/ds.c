#include "ak/coll/ds.h"
#include "ak/coll/da.h"

ak_ds
ak_ds_make(uint32_t itemsize, ak_alct alct)
{
  ak_ds s = { 0 };
  s.a = ak_da_make(itemsize, alct);
  return s;
}

void
ak_ds_destroy(ak_ds* s)
{
  ak_da_destroy(&s->a);
}

void
ak_ds_clear(ak_ds* s)
{
  ak_da_clear(&s->a);
}

uint32_t
ak_ds_count(const ak_ds* s)
{
  return ak_da_count(&s->a);
}

void
ak_ds_push(ak_ds* s, const void* item)
{
  ak_da_pushback(&s->a, item);
}

bool
ak_ds_pop(ak_ds* s, void* o_item)
{
  if (ak_da_count(&s->a) == 0) {
    return false;
  }
  ak_da_at_copy(
    &s->a, ak_da_count(&s->a) - 1, o_item);
  ak_da_remove(&s->a,
               ak_da_count(&s->a) - 1);
  return true;
}

bool
ak_ds_peek(ak_ds* s,
           uint32_t idx,
           void* o_item)
{
  if (idx >= ak_da_count(&s->a)) {
    return false;
  }
  ak_da_at_copy(&s->a,
                ak_da_count(&s->a) - idx - 1,
                o_item);
  return true;
}
