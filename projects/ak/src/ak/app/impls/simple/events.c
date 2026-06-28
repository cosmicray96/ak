#include "ak/app/event.h"
#include "ak/app/impls/simple/dir.h"
#include "ak/gfx/gcb.h"
#include "ak/platform/plat_base.h"

//--- private ---//
static void
event_on(ak_lsimple* l, const ak_evt* e)
{
  if (e->type == ak_evttype_pgm &&
      e->pgm == ak_evtpgm_exit) {
    l->should_close = true;
    return;
  }

  if (e->type == ak_evttype_win &&
      e->win.type == ak_evtwintype_resize) {
    ak_gcb_push_resize(&l->gcb,
                       e->win.resize.w,
                       e->win.resize.h);
    return;
  }
}

//--- internal ---//
void
event_collect(ak_lsimple* l)
{
  ak_plat_base_eventflush(l->pb, &l->eq);
}

void
event_drain(ak_lsimple* l)
{
  ak_evt e = { 0 };
  while (ak_app_eq_pop(&l->eq, &e)) {
    event_on(l, &e);
  }
}
