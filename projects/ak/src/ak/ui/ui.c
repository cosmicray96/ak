#include "ak/ui/ui.h"
#include "ak/ui/core.h"
#include "ak/ui/ui_dir.h"

ak_ui
ak_ui_make(ak_alct alct)
{
  ak_ui ui = { 0 };
  ui.tree =
    ak_fcnst_make(sizeof(ak_uielm), alct);
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
}

ak_fcnst*
ak_ui_tree(ak_ui* ui)
{
  return &ui->tree;
}

ak_uiid
ak_ui_add(ak_ui* ui,
          ak_uiid pt,
          ak_cnst x_cnst,
          ak_cnst y_cnst)
{

  ak_uielm elm = {
    .cnst.x = x_cnst,
    .cnst.y = y_cnst,
  };
  return ak_fcnst_add(&ui->tree, pt, &elm);
}
