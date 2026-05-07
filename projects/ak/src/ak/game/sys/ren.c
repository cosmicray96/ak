#include "ak/game/sys/ren.h"
#include "ak/core/mem/allocator.h"

ak_sys_ren
ak_sys_ren_make(ak_gfx* gf,
                ak_world_v* wv,
                ak_alct alct)
{
  ak_sys_ren r = { 0 };
  r.alct = alct;
  r.wv = wv;
  r.gf = gf;
  return r;
}

void
ak_sys_ren_destroy(ak_sys_ren* r)
{
  ak_alct_invalidate(&r->alct);
  r->gf = 0;
  r->wv = 0;
}

void
ak_sys_ren_render(ak_sys_ren* r)
{
}
