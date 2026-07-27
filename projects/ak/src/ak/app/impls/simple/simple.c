#include "ak/app/impls/simple.h"
#include "ak/app/eq.h"
#include "ak/app/impls/simple/dir.h"
#include "ak/core/mem/heap.h"

//--- private ---//
static void
crash_fn(void* ctx)
{
  ak_lsimple* l = ctx;
  ak_lsimple_destroy(l);
}

//--- export ---//
ak_lsimple*
ak_lsimple_make()
{
  ak_heap heap = ak_heap_make();
  ak_lsimple* l =
    ak_heap_alloc(&heap, sizeof(ak_lsimple));
  l->heap = heap;
  l->alct = ak_heap_to_alct(&l->heap);

  l->should_close = false;
  l->eq = ak_app_eq_make(l->alct);

  render_make(l);
  world_make(l);
  pg_make(l);

  ak_pgm_crashfn_reg(&crash_fn, l);

  return l;
}

void
ak_lsimple_destroy(ak_lsimple* l)
{
  // pg_destroy(l);
  world_destroy(l);
  render_destroy(l);

  ak_app_eq_destroy(&l->eq);

  ak_heap heap = l->heap;
  ak_heap_free(&l->heap, l);
  ak_heap_destroy(&heap);
}

uint32_t framecount = 10000;
bool
ak_lsimple_tick(void* ctx)
{
  if (framecount == 0) {
    return true;
  }
  framecount--;

  ak_lsimple* l = ctx;
  event_collect(l);
  event_drain(l);

  //  pg_update(l);
  world_update(l);
  render_update(l);

  return l->should_close;
}
