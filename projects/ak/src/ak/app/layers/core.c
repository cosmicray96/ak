#include "ak/app/layers/core.h"
#include "ak/app/app.h"
#include "ak/app/eq.h"
#include "ak/app/event.h"
#include "ak/core/math/vec2.h"
#include "ak/gfx/mtrl/col_itn.h"
#include "ak/gfx/mtrl/mtrl.h"
#include "ak/gfx/mtrl/vcol_itn.h"
#include "ak/os/time.h"
#include "ak/platform/core.h"
#include "ak/platform/plat_ren.h"
#include "ak/program/core.h"
#include "ak/program/event.h"

#include "ak/gfx/core.h"
#include "ak/gfx/gfx.h"
#include "ak/platform/plat.h"

//===== ak_lcore =====//
//--- private ---//
struct ak_lcore
{
  ak_alct alct;
  ak_app* app;
  ak_app_eq* eq;
  ak_plat_ren* pr;
  ak_plat* p;
  ak_gfx* gf;
  ak_mtrl_vcol* m_vcol;
  ak_mtrl_col* m_col;

  bool flip;
};

static bool
on_resize(ak_lcore* l, ak_evt e)
{
  if (e.type != ak_evt_type_win) {
    return false;
  }
  if (e.win.type != ak_winevt_resize) {
    return false;
  }
  ak_gfx_resize(
    l->gf, e.win.resize.w, e.win.resize.h);
  return true;
}

static bool
on_win_close(ak_lcore* l, ak_evt e)
{
  if (e.type != ak_evt_type_win) {
    return false;
  }
  if (e.win.type != ak_winevt_close) {
    return false;
  }
  ak_app_close(l->app);
  return true;
}

//--- public ---//
ak_lcore*
ak_lcore_make(ak_alct alct)
{
  ak_lcore* l =
    ak_alct_alloc(alct, sizeof(ak_lcore));
  l->alct = alct;
  return l;
}
void
ak_lcore_destroy(ak_lcore* l)
{
  ak_alct_free(l->alct, l);
}

ak_gfx*
ak_lcore_gfx(ak_lcore* l)
{
  return l->gf;
}

//===== ak_applayer =====//
//--- private ---//

static void
on_startup(void* ctx, ak_app* app)
{
  ak_lcore* l = ctx;
  l->app = app;
  l->eq = 0;
  l->pr = ak_plat_ren_startup(l->alct);
  l->p = ak_plat_startup(l->pr, l->alct);
  l->gf = ak_gfx_startup(l->pr, l->alct);

  l->m_vcol =
    ak_mtrl_vcol_make(l->gf, l->alct);
  l->m_col =
    ak_mtrl_col_make(l->gf, l->alct);

  l->flip = false;
}

static void
on_shutdown(void* ctx)
{
  ak_lcore* l = ctx;

  ak_mtrl_vcol_destroy(l->m_vcol);

  ak_gfx_shutdown(l->gf);
  ak_plat_shutdown(l->p);
  ak_plat_ren_shutdown(l->pr);
}

static void
on_epusher(void* ctx, ak_app_eq* eq)
{
  ak_lcore* l = ctx;
  if (!l->eq) {
    l->eq = eq;
  }

  ak_pgmevt pgmevt = ak_pgm_event_pop();
  while (pgmevt != ak_pgm_none) {
    ak_evt e = { 0 };
    e.type = ak_evt_type_pgm;
    e.pgm = pgmevt;
    ak_app_eq_push(eq, e);

    pgmevt = ak_pgm_event_pop();
  }

  ak_plat_eventflush(l->p, eq);
}

static bool
on_event(void* ctx, ak_evt e)
{
  ak_lcore* l = ctx;

  if (on_win_close(l, e)) {
    return true;
  }

  if (on_resize(l, e)) {
    return true;
  }

  if (e.type != ak_evt_type_win) {
    return false;
  }
  if (e.win.type != ak_winevt_key) {
    return false;
  }
  if (e.win.key.code != ak_key_f) {
    return false;
  }
  if (e.win.key.action !=
      ak_keyaction_pressed) {
    return false;
  }
  {
    ak_evt e = { 0 };
    e.type = ak_evt_type_win;
    e.win.type = ak_winevt_resize;
    e.win.resize.w = l->flip ? 800 : 400;
    e.win.resize.h = l->flip ? 600 : 300;
    l->flip = !l->flip;

    ak_app_eq_push(l->eq, e);
  }

  /*
ak_log("key: %d", e.win.key.code);
ak_log("mode: %d", e.win.key.mode);
ak_log("action: %s",
   e.win.key.action ==
       ak_keyaction_pressed
     ? "pressed"
     : "released");
  */

  return false;
}

typedef struct
{
  ak_vec2 pos;
  ak_vec4 col;
} vert;

static void
on_update(void* ctx, ak_dur delta)
{
  ak_lcore* l = ctx;
  return;
  vert verts[6] = {
    // Triangle 1
    {
      ak_vec2_make(ak_fx32_f(-0.5f),
                   ak_fx32_f(-0.5f)),
      ak_vec4_make(ak_fx32_f(1.0f),
                   ak_fx32_f(0.0f),
                   ak_fx32_f(0.0f),
                   ak_fx32_f(1.0f)) // red
    },
    {
      ak_vec2_make(ak_fx32_f(0.5f),
                   ak_fx32_f(-0.5f)),
      ak_vec4_make(ak_fx32_f(0.0f),
                   ak_fx32_f(1.0f),
                   ak_fx32_f(0.0f),
                   ak_fx32_f(1.0f)) // green
    },
    {
      ak_vec2_make(ak_fx32_f(0.5f),
                   ak_fx32_f(0.5f)),
      ak_vec4_make(ak_fx32_f(0.0f),
                   ak_fx32_f(0.0f),
                   ak_fx32_f(1.0f),
                   ak_fx32_f(1.0f)) // blue
    },

    // Triangle 2
    {
      ak_vec2_make(ak_fx32_f(-0.5f),
                   ak_fx32_f(-0.5f)),
      ak_vec4_make(
        ak_fx32_f(1.0f),
        ak_fx32_f(0.0f),
        ak_fx32_f(1.0f),
        ak_fx32_f(1.0f)) // magenta
    },
    {
      ak_vec2_make(ak_fx32_f(0.5f),
                   ak_fx32_f(0.5f)),
      ak_vec4_make(ak_fx32_f(0.0f),
                   ak_fx32_f(1.0f),
                   ak_fx32_f(1.0f),
                   ak_fx32_f(1.0f)) // cyan
    },
    {
      ak_vec2_make(ak_fx32_f(-0.5f),
                   ak_fx32_f(0.5f)),
      ak_vec4_make(ak_fx32_f(1.0f),
                   ak_fx32_f(1.0f),
                   ak_fx32_f(0.0f),
                   ak_fx32_f(1.0f)) // yellow
    }
  };

  ak_gfx_frame_begin(l->gf);

  ak_mtrl_col_col_set(l->m_col,
                      verts[0].col);
  ak_mtrl_col_call_begin(l->m_col);
  for (uint32_t i = 0; i < 3; i++) {
    ak_mtrl_col_pushvert(l->m_col,
                         verts[i].pos);
  }
  ak_mtrl_col_call_end(l->m_col);

  ak_mtrl_vcol_call_begin(l->m_vcol);
  for (uint32_t i = 3; i < 6; i++) {
    ak_mtrl_vcol_pushvert(
      l->m_vcol, verts[i].pos, verts[i].col);
  }
  ak_mtrl_vcol_call_end(l->m_vcol);

  ak_gfx_frame_end(l->gf);
}

static void
on_upost(void* ctx, ak_dur delta)
{
  ak_lcore* l = ctx;
  ak_plat_ren_swapbuffer(l->pr);
}

//--- public ---//
ak_applayer
ak_lcore_to_applayer(ak_lcore* l)
{
  ak_applayer appl = { 0 };
  appl.ctx = l;
  appl.on_startup = &on_startup;
  appl.on_shutdown = &on_shutdown;
  appl.on_epusher = &on_epusher;
  appl.on_event = &on_event;
  appl.on_update = &on_update;
  appl.on_upost = &on_upost;
  return appl;
}
