#include "ak/ui/ui_dir.h"

//--- private ---//

static void
bound_set(ak_fcnst* tree, ak_uiid id)
{
  ak_uielm* elm = ak_fcnst_at(tree, id);
  ak_fcnst_itchild it =
    ak_fcnst_itchild_make(tree, id);
  ak_uiid cid = 0;

  if (elm->axistype == ak_uiaxis_h) {
    while (
      (cid = ak_fcnst_itchild_next(&it))) {
      ak_uielm* celm = ak_fcnst_at(tree, id);
    }
  }
}

static void
bottom_up(ak_ui* ui)
{
  ak_fcnst* tree = ak_ui_tree(ui);
  ak_fcnst_itdfspost it =
    ak_fcnst_itdfspost_make(
      tree, ak_fcnst_root(tree));

  ak_uiid id = 0;
  while (
    (id = ak_fcnst_itdfspost_next(&it))) {
    ak_uielm* elm = ak_fcnst_at(tree, id);
  }
}

//--- dir ---//
void
ak_ui_layout(ak_ui* ui,
             uint32_t width,
             uint32_t height)
{
  ak_fcnst* tree = ak_ui_tree(ui);
}
