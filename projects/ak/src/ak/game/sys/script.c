#include "ak/game/sys/script.h"
#include "ak/app/core.h"
#include "ak/game/world/cb_itn.h"
#include "ak/game/world/cbflush.h"
#include "ak/game/world/view.h"
#include "ak/game/world/view_itn.h"
#include "ak/system/idgen.h"

//===== ak_sys_script =====//
//--- private ---//
static void
parse_wcb(ak_sys_script* s,
          ak_wv* wv,
          ak_wcb* wcb)
{
  ak_da_clear(&s->inits);
  ak_da_clear(&s->deinits);

  uint32_t count = ak_wcb_count(wcb);
  for (uint32_t i = 0; i < count; i++) {
    ak_wcbitem cmd = { 0 };
    ak_assert(ak_wcb_peek(wcb, i, &cmd));

    switch (cmd.cmd) {
      case ak_wcbtype_comp_add: {
        if (cmd.ctu.ce == ak_script_e) {
          ak_da_pushback(&s->inits, &cmd.e);
        }
        break;
      }
      case ak_wcbtype_comp_remove: {
        if (cmd.ctu.ce == ak_script_e) {
          ak_da_pushback(&s->deinits,
                         &cmd.e);
        }
        break;
      }
      case ak_wcbtype_ett_remove: {
        ak_wv_itdfspost it =
          ak_wv_itdfspost_make(wv, cmd.e);
        ak_ett c = 0;
        while (
          (c = ak_wv_itdfspost_next(&it))) {
          if (ak_wv_comp_script_exist(wv,
                                      c)) {
            ak_da_pushback(&s->deinits, &c);
          }
        }
        break;
      }
      default: {
        // empty
      }
    }
  }
}

static void
run_init(ak_sys_script* s, ak_wv* wv)
{
  ak_script_ctx ctx = { .wv = wv,
                        .wcb =
                          &s->wcb_output };

  uint32_t count = ak_da_count(&s->inits);
  for (uint32_t i = 0; i < count; i++) {
    ak_ett e =
      *(ak_ett*)ak_da_at(&s->inits, i);
    ak_assert(
      ak_wv_comp_script_exist(wv, e));
    ak_script_t script_t =
      ak_wv_comp_script(wv, e);
    ak_script script =
      ak_scriptstg_at(s->ss, script_t.se);
    ctx.e = e;
    if (script.init) {
      script.init(ctx);
    }
  }
}

static void
run_deinit(ak_sys_script* s, ak_wv* wv)
{
  ak_script_ctx ctx = { .wv = wv,
                        .wcb =
                          &s->wcb_output };

  uint32_t count = ak_da_count(&s->deinits);
  for (uint32_t i = 0; i < count; i++) {
    ak_ett e =
      *(ak_ett*)ak_da_at(&s->deinits, i);
    ak_assert(
      ak_wv_comp_script_exist(wv, e));
    ak_script_t script_t =
      ak_wv_comp_script(wv, e);
    ak_script script =
      ak_scriptstg_at(s->ss, script_t.se);
    ctx.e = e;
    if (script.deinit) {
      script.deinit(ctx);
    }
  }
}

static void
run_update(ak_sys_script* s, ak_wv* wv)
{
  ak_script_ctx ctx = { .wv = wv,
                        .wcb = &s->wcb,
                        .delta =
                          ak_delta() };
  ak_wv_itdfspost it = ak_wv_itdfspost_make(
    wv, ak_wv_ett_root(wv));
  ak_ett ett = 0;
  while ((ett = ak_wv_itdfspost_next(&it))) {
    if (!ak_wv_comp_script_exist(wv, ett)) {
      continue;
    }
    ak_script_t script_t =
      ak_wv_comp_script(wv, ett);
    ak_script script =
      ak_scriptstg_at(s->ss, script_t.se);
    ctx.e = ett;
    if (script.update) {
      script.update(ctx);
    }
  }
}

static void
apply_cmd(ak_sys_script* s,
          ak_world* w,
          ak_wcb* wcb)
{
  ak_wv wv = ak_wv_make(w);

  parse_wcb(s, &wv, wcb);
  run_deinit(s, &wv);

  ak_world_cb_flush(w, wcb);

  run_init(s, &wv);
  ak_world_cb_flush(w, &s->wcb_output);
}

//--- internal ---//
ak_sys_script
ak_sys_script_make(ak_idgen* ig,
                   ak_alct alct)
{
  ak_sys_script s = { 0 };
  s.alct = alct;
  s.wcb = ak_wcb_make(ig, alct);
  s.wcb_output = ak_wcb_make(ig, alct);
  s.ss = ak_scriptstg_make(alct);
  s.inits = ak_da_make(sizeof(ak_ett), alct);
  s.deinits =
    ak_da_make(sizeof(ak_ett), alct);

  return s;
}

void
ak_sys_script_destroy(ak_sys_script* s)
{
  ak_wcb_destroy(&s->wcb);
  ak_wcb_destroy(&s->wcb_output);

  ak_da_destroy(&s->deinits);
  ak_da_destroy(&s->inits);
  ak_scriptstg_destroy(s->ss);
}

void
ak_sys_script_on_event(ak_sys_script* s,
                       ak_world* w,
                       ak_ett ett,
                       const ak_evt* evt)
{
  ak_wv wv = ak_wv_make(w);
  ak_script_ctx ctx = { .wv = &wv,
                        .wcb = &s->wcb,
                        .evt = *evt };
  if (!ak_wv_comp_script_exist(&wv, ett)) {
    return;
  }
  ak_script_t script_t =
    ak_wv_comp_script(&wv, ett);
  ak_script script =
    ak_scriptstg_at(s->ss, script_t.se);
  ctx.e = ett;
  if (script.event) {
    script.event(ctx);
  }

  apply_cmd(s, w, &s->wcb);
}

void
ak_sys_script_update(ak_sys_script* s,
                     ak_world* w)
{
  ak_wv wv = ak_wv_make(w);

  run_update(s, &wv);

  apply_cmd(s, w, &s->wcb);
}

void
ak_sys_script_wcb_apply(ak_sys_script* s,
                        ak_world* w,
                        ak_wcb* wcb)
{

  apply_cmd(s, w, wcb);
}
