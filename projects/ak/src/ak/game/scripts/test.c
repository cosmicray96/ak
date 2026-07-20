#include "ak/debug.h"
#include "ak/game/script.h"
#include "ak/game/world/cb.h"

static void
on_init(ak_script_ctx ctx)
{
  ak_log("Inited, %d", ctx.e);
}

static void
on_deinit(ak_script_ctx ctx)
{
  ak_log("Deinited, %d", ctx.e);
}

static void
on_event(ak_script_ctx ctx)
{
  ak_log("Event, %d", ctx.e);
}

static void
on_update(ak_script_ctx ctx)
{
  ak_log("Update, %d", ctx.e);

  ak_ett e = ak_wcb_ett_new(ctx.wcb, ctx.e);
  ak_wcb_comp_script_add(
    ctx.wcb,
    e,
    (ak_script_t){ .se = ak_script_test_e });
}

ak_script
ak_test_to_script()
{
  return (ak_script){ .init = &on_init,
                      .deinit = &on_deinit,
                      .event = &on_event,
                      .update = &on_update };
}
