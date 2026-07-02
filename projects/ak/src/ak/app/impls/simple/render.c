#include "ak/system/render.h"
#include "ak/app/impls/simple/dir.h"

void
render_make(ak_lsimple* l)
{
  l->rr = ak_resreg_make(l->alct);
  l->pb =
    ak_plat_base_startup(&l->eq, l->alct);
  l->r = ak_renderer_startup(
    l->pb, &l->rr, l->alct);

  ak_plat_base_render_dispatch_set(
    l->pb, ak_renderer_dispatcher_pre(l->r));

  l->gcb = ak_gcb_make(l->alct);
}
void
render_update(ak_lsimple* l)
{
  ak_renderer_render(l->r, &l->gcb);
}
void
render_destroy(ak_lsimple* l)
{
  ak_renderer_shutdown(l->r);
  ak_gcb_destroy(&l->gcb);
  ak_resreg_destroy(&l->rr);
  ak_plat_base_shutdown(l->pb);
}
