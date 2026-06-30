#ifndef ak_app_impls_simple_itn_h
#define ak_app_impls_simple_itn_h

#include "ak/app/eq.h"
#include "ak/app/impls/simple.h"
#include "ak/core/mem/heap.h"
#include "ak/gfx/gcb.h"
#include "ak/platform/plat_base.h"
#include "ak/res/reg_itn.h"
#include "ak/system/render.h"

//--- private ---//
struct ak_lsimple
{
  ak_heap heap;
  ak_app_eq eq;
  bool should_close;

  ak_plat_base* pb;

  ak_resreg rr;
  ak_renderer* r;
  ak_gcb gcb;
};

void
event_drain(ak_lsimple* l);
void
event_collect(ak_lsimple* l);

#endif
