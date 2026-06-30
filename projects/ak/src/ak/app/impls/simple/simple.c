#include "ak/app/impls/simple.h"
#include "ak/app/eq.h"
#include "ak/app/impls/simple/dir.h"
#include "ak/core/mem/heap.h"
#include "ak/platform/plat_base.h"
#include "ak/system/render.h"

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
  ak_alct alct = ak_heap_to_alct(&l->heap);

  l->should_close = false;
  l->eq = ak_app_eq_make(alct);

  l->pb = ak_plat_base_startup(alct);
  l->r = ak_renderer_startup(l->pb, alct);
  l->gcb = ak_gcb_make(alct);

  ak_pgm_crashfn_reg(&crash_fn, l);
  return l;
}

void
ak_lsimple_destroy(ak_lsimple* l)
{
  ak_gcb_destroy(&l->gcb);
  ak_renderer_shutdown(l->r);
  ak_plat_base_shutdown(l->pb);

  ak_app_eq_destroy(&l->eq);

  ak_heap heap = l->heap;
  ak_heap_free(&l->heap, l);
  ak_heap_destroy(&heap);
}

bool
ak_lsimple_tick(void* ctx)
{
  ak_lsimple* l = ctx;
  event_collect(l);
  event_drain(l);

  ak_renderer_render(l->r, &l->gcb);
  return l->should_close;
}
