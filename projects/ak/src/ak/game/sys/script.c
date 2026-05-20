#include "ak/game/sys/script.h"
#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/async/batchjob.h"
#include "ak/core/async/thpool.h"
#include "ak/debug.h"
#include "ak/game/comp.h"
#include "ak/game/core.h"
#include "ak/game/script.h"
#include "ak/game/script/stg.h"
#include "ak/game/world/cb.h"
#include "ak/game/world/cb_itn.h"
#include "ak/game/world/view.h"

//===== depth_item =====//
//--- private ---//
typedef struct
{
  ak_script script;
  ak_script_ctx ctx;
  ak_ett e;
  uint32_t depth;
} script_item;

bool
depth_compare(const void* a, const void* b)
{
  return ((const script_item*)a)->depth >
         ((const script_item*)b)->depth;
}

//===== ak_sys_script =====//
//--- private ---//
static void
update_fn(void* ctx)
{
  script_item* si = ctx;
  if (si->script.update) {
    si->script.update(si->ctx);
  }
}

static void
run_update(ak_sys_script* s, ak_dur delta)
{
  ak_batchjob bj =
    ak_batchjob_make(s->tp, s->alct);
  uint32_t count = ak_da_count(&s->depths);
  for (uint32_t i = 0; i < count; i++) {
    script_item* si =
      ak_da_at_impl(&s->depths, i);

    si->ctx.e = si->e;
    si->ctx.wv = s->wv;
    si->ctx.wcb = ak_hmn_at(&s->map, si->e);
    si->ctx.delta = delta;

    ak_batchjob_submit(&bj, update_fn, &si);
  }
  ak_batchjob_begin(&bj);
}

//--- internal ---//
ak_sys_script
ak_sys_script_make(ak_wv* wv,
                   ak_wcb* wcb,
                   ak_idgen* ig,
                   ak_thpool* tp,
                   ak_alct alct)
{
  ak_sys_script s = { 0 };
  s.alct = alct;
  s.wv = wv;
  s.wcb = wcb;
  s.ig = ig;
  s.tp = tp;

  s.ss = ak_scriptstg_make(alct);

  s.map = ak_hmn_make(sizeof(ak_wcb), alct);
  s.depths =
    ak_da_make(sizeof(script_item), alct);
  return s;
}

void
ak_sys_script_destroy(ak_sys_script* s)
{
  ak_assert(false); // todo
}

void
ak_sys_script_update(ak_sys_script* s,
                     ak_dur delta)
{
  ak_da_clear(&s->depths);

  ak_wv_itcomp it =
    ak_wv_itcomp_make(s->wv, ak_screen_e);
  ak_ett e;
  ak_script_t script = { 0 };
  while (1) {
    e = ak_wv_itcomp_next(&it, &script);
    if (!e) {
      break;
    }
    if (!ak_hmn_exist(&s->map, e)) {
      ak_wcb wcb =
        ak_wcb_make(s->ig, s->alct);
      ak_hmn_insert(&s->map, e, &wcb);
    }
    script_item di = { 0 };
    di.e = e;
    di.depth = ak_wv_ett_depth(s->wv, e);
    di.script =
      ak_scriptstg_at(s->ss, script.se);
    ak_da_pushback(&s->depths, &di);
  }

  run_update(s, delta);
}
