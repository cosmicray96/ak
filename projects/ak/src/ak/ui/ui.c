#include "ak/ui/ui.h"
#include "ak/debug.h"
#include "ak/ui/core.h"
#include "ak/ui/ui_dir.h"

ak_ui
ak_ui_make(ak_alct alct)
{
  ak_ui ui = { 0 };
  ui.tree =
    ak_fcnst_make(sizeof(ak_uielm), alct);

  ui.scissors =
    ak_dq_make(sizeof(ak_vec4f), alct);
  ui.mtrl =
    (ak_gfx_calldata){ .shaderid = 18 };

  return ui;
}
void
ak_ui_destroy(ak_ui* ui)
{
  ak_fcnst_destroy(&ui->tree);
}

void
ak_ui_clear(ak_ui* ui)
{
  ak_fcnst_clear(&ui->tree);

  ak_uielm elm = { .args = {
                     .axistype = ak_uiaxis_v,
                     .cnst.x = { .rel = 1,
                                 .abs = 0 },
                     .cnst.y = { .rel = 1,
                                 .abs = 0 },
                     .visible = false,
                     .clipping = false } };
  ak_fcnst_add(&ui->tree, 0, &elm);
}

ak_uiid
ak_ui_root(ak_ui* ui)
{
  return ak_fcnst_root(&ui->tree);
}

ak_uiid
ak_ui_add(ak_ui* ui,
          ak_uiid pt,
          const ak_uielm_args* args,
          const ak_gfx_quaddata* qd)
{
  ak_assert(pt != 0);

  ak_uielm elm = { .args = *args,
                   .qd = *qd };
  return ak_fcnst_add(&ui->tree, pt, &elm);
}
