#include "ak/game/stg/world.h"
#include "ak/app/core.h"
#include "ak/app/impls/simple/dir.h"
#include "ak/game/sys/ren.h"
#include "ak/game/sys/tf.h"
#include "ak/game/world/cb_itn.h"
#include "ak/game/world/cbflush.h"
#include "ak/game/world/view_itn.h"
#include "ak/system/idgen.h"

void
world_make(ak_lsimple* l)
{
  l->ig = ak_idgen_make(l->alct);
  l->w = ak_world_make(l->alct);
  l->wv = ak_wv_make(&l->w);
  l->wcb = ak_wcb_make(&l->ig, l->alct);

  l->sys_tf = ak_sys_tf_make(l->alct);
  l->sys_ren = ak_sys_ren_make(l->alct);
}

void
world_update(ak_lsimple* l)
{
  ak_sys_tf_update(
    &l->sys_tf, &l->wv, &l->wcb);
  ak_world_cb_flush(&l->w, &l->wcb);

  ak_sys_ren_render(&l->sys_ren,
                    &l->wv,
                    &l->gcb,
                    ak_delta());
}

void
world_destroy(ak_lsimple* l)
{
  ak_idgen_destroy(&l->ig);
  ak_world_destroy(&l->w);
  ak_wcb_destroy(&l->wcb);

  ak_sys_tf_destroy(&l->sys_tf);
  ak_sys_ren_destroy(&l->sys_ren);
}
