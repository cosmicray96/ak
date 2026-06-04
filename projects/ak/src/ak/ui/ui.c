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
  ui.lstg = ak_uilstg_make(alct);
  ui.rstg = ak_uirstg_make(alct);
  return ui;
}
void
ak_ui_destroy(ak_ui* ui)
{
  ak_uirstg_destroy(&ui->rstg);
  ak_uilstg_destroy(&ui->lstg);
  ak_fcnst_destroy(&ui->tree);
}

void
ak_ui_clear(ak_ui* ui)
{
  ak_fcnst_clear(&ui->tree);
  ui->setted = false;

  ak_uielm elm = {
    .axistype = ak_uiaxis_v,
    .cnst.x = { .rel = 1, .abs = 0 },
    .cnst.y = { .rel = 1, .abs = 0 },
    .visible = false,
    .clipping = false
  };
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
          ak_cnst x_cnst,
          ak_cnst y_cnst,
          bool visible)
{
  ak_assert(!ui->setted);
  ak_assert(pt != 0);

  ak_uielm elm = { .cnst.x = x_cnst,
                   .cnst.y = y_cnst,
                   .color =
                     (ak_vec4f){ .r = 0,
                                 .g = 0,
                                 .b = 1,
                                 .a = 1 },
                   .visible = visible,
                   .clipping = false };
  return ak_fcnst_add(&ui->tree, pt, &elm);
}

void
ak_ui_set(ak_ui* ui,
          float x,
          float y,
          float w,
          float h)
{
  ak_assert(!ui->setted);

  ak_ui_layout(ui, x, y, w, h);

  ui->setted = true;
}
