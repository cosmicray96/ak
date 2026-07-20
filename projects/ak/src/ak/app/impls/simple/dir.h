#ifndef ak_app_impls_simple_itn_h
#define ak_app_impls_simple_itn_h

#include "ak/app/eq.h"
#include "ak/app/impls/simple.h"
#include "ak/core/mem/heap.h"
#include "ak/game/core.h"
#include "ak/game/stg/world.h"
#include "ak/game/sys/ren.h"
#include "ak/game/sys/script.h"
#include "ak/game/sys/tf.h"
#include "ak/game/world/cb_itn.h"
#include "ak/game/world/view_itn.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gcb.h"
#include "ak/platform/plat_base.h"
#include "ak/res/core.h"
#include "ak/res/reg_itn.h"
#include "ak/res/reses/img.h"
#include "ak/system/idgen.h"

//--- private ---//
struct ak_lsimple
{
  ak_heap heap;
  ak_alct alct;
  ak_app_eq eq;
  bool should_close;

  ak_plat_base* pb;

  ak_resreg rr;
  ak_rctx* rctx;
  ak_gcb gcb;

  // world
  ak_idgen ig;
  ak_world w;
  ak_wv wv;
  ak_wcb wcb;
  ak_sys_script sys_script;
  ak_sys_tf sys_tf;
  ak_sys_ren sys_ren;

  // res
  ak_tex* tex;
  ak_resid texid;

  ak_shader* shader;
  ak_resid shaderid;
};

void
event_drain(ak_lsimple* l);
void
event_collect(ak_lsimple* l);

void
render_make(ak_lsimple* l);
void
render_update(ak_lsimple* l);
void
render_destroy(ak_lsimple* l);

void
world_make(ak_lsimple* l);
void
world_update(ak_lsimple* l);
void
world_destroy(ak_lsimple* l);

void
pg_make(ak_lsimple* l);
void
pg_update(ak_lsimple* l);
void
pg_destroy(ak_lsimple* l);

#endif
