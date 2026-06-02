#include "ak/game/sys/script.h"
#include "ak/game/world/cb_itn.h"
#include "ak/game/world/view.h"

//===== ak_sys_script =====//
//--- internal ---//
ak_sys_script
ak_sys_script_make(ak_alct alct)
{
  ak_sys_script s = { 0 };
  s.alct = alct;
  s.wv = 0;
  s.ss = ak_scriptstg_make(alct);
  s.inits = ak_da_make(sizeof(ak_ett), alct);
  s.deinits =
    ak_da_make(sizeof(ak_ett), alct);

  return s;
}

void
ak_sys_script_destroy(ak_sys_script* s)
{
  ak_da_destroy(&s->deinits);
  ak_da_destroy(&s->inits);
  ak_scriptstg_destroy(s->ss);
  s->wv = 0;
}

void
ak_sys_script_set(ak_sys_script* s,
                  ak_wv* wv,
                  ak_wcb* wcb)
{
  ak_da_clear(&s->inits);
  ak_da_clear(&s->deinits);

  s->wv = wv;
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
      }
    }
  }
}

void
ak_sys_script_run_init(ak_sys_script* s,
                       ak_wcb* output_wcb)
{
  ak_script_ctx ctx = { .wv = s->wv,
                        .wcb = output_wcb };

  uint32_t count = ak_da_count(&s->inits);
  for (uint32_t i = 0; i < count; i++) {
    ak_ett e =
      *(ak_ett*)ak_da_at(&s->inits, i);
    ak_assert(
      ak_wv_comp_script_exist(s->wv, e));
    ak_script_t script_t =
      ak_wv_comp_script(s->wv, e);
    ak_script script =
      ak_scriptstg_at(s->ss, script_t.se);
    ctx.e = e;
    if (script.init) {
      script.init(ctx);
    }
  }
}

void
ak_sys_script_run_deinit(ak_sys_script* s,
                         ak_wcb* output_wcb)
{
  ak_script_ctx ctx = { .wv = s->wv,
                        .wcb = output_wcb };

  uint32_t count = ak_da_count(&s->deinits);
  for (uint32_t i = 0; i < count; i++) {
    ak_ett e =
      *(ak_ett*)ak_da_at(&s->deinits, i);
    ak_assert(
      ak_wv_comp_script_exist(s->wv, e));
    ak_script_t script_t =
      ak_wv_comp_script(s->wv, e);
    ak_script script =
      ak_scriptstg_at(s->ss, script_t.se);
    ctx.e = e;
    if (script.deinit) {
      script.deinit(ctx);
    }
  }
}

void
ak_sys_script_run_event(ak_sys_script* s,
                        ak_evt evt,
                        ak_wcb* output_wcb)
{
  ak_script_ctx ctx = { .wv = s->wv,
                        .wcb = output_wcb,
                        .evt = evt };
  ak_wv_itdfspost it = ak_wv_itdfspost_make(
    s->wv, ak_wv_ett_root(s->wv));
  ak_ett ett = 0;
  while ((ett = ak_wv_itdfspost_next(&it))) {
    if (!ak_wv_comp_script_exist(s->wv,
                                 ett)) {
      continue;
    }
    ak_script_t script_t =
      ak_wv_comp_script(s->wv, ett);
    ak_script script =
      ak_scriptstg_at(s->ss, script_t.se);
    ctx.e = ett;
    if (script.event) {
      script.event(ctx);
    }
  }
}
void
ak_sys_script_run_update(ak_sys_script* s,
                         ak_dur delta,
                         ak_wcb* output_wcb)
{
  ak_script_ctx ctx = { .wv = s->wv,
                        .wcb = output_wcb,
                        .delta = delta };
  ak_wv_itdfspost it = ak_wv_itdfspost_make(
    s->wv, ak_wv_ett_root(s->wv));
  ak_ett ett = 0;
  while ((ett = ak_wv_itdfspost_next(&it))) {
    if (!ak_wv_comp_script_exist(s->wv,
                                 ett)) {
      continue;
    }
    ak_script_t script_t =
      ak_wv_comp_script(s->wv, ett);
    ak_script script =
      ak_scriptstg_at(s->ss, script_t.se);
    ctx.e = ett;
    if (script.update) {
      script.update(ctx);
    }
  }
}

void
ak_sys_script_run_shutdown(
  ak_sys_script* s,
  ak_wcb* output_wcb)
{
  ak_script_ctx ctx = { .wv = s->wv,
                        .wcb = output_wcb };
  ak_wv_itdfspost it = ak_wv_itdfspost_make(
    s->wv, ak_wv_ett_root(s->wv));
  ak_ett ett = 0;
  while ((ett = ak_wv_itdfspost_next(&it))) {
    if (!ak_wv_comp_script_exist(s->wv,
                                 ett)) {
      continue;
    }
    ak_script_t script_t =
      ak_wv_comp_script(s->wv, ett);
    ak_script script =
      ak_scriptstg_at(s->ss, script_t.se);
    ctx.e = ett;
    if (script.deinit) {
      script.deinit(ctx);
    }
  }
}
