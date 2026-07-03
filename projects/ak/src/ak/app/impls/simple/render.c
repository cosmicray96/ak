#include "ak/app/impls/simple/dir.h"
#include "ak/gfx/rctx.h"
#include "ak/platform/plat_base.h"

void
render_make(ak_lsimple* l)
{
  l->rr = ak_resreg_make(l->alct);
  l->pb = ak_plat_base_startup(
    &l->eq, &l->rr, l->alct);

  l->rctx = ak_plat_base_rctx(l->pb);

  l->gcb = ak_gcb_make(l->alct);
}
void
render_update(ak_lsimple* l)
{
  ak_rctx_render(l->rctx, &l->gcb);
}
void
render_destroy(ak_lsimple* l)
{
  ak_gcb_destroy(&l->gcb);
  ak_plat_base_shutdown(l->pb);
  ak_resreg_destroy(&l->rr);
}
