#include "ak/ui/core.h"
#include "ak/ui/ui_dir.h"

//--- private ---//

static void
set_layout(ak_uilstg* lstg,
           ak_fcnst* tree,
           ak_uiid id)
{
  ak_da_clear(&lstg->bounds);

  ak_uielm* elm = ak_fcnst_at(tree, id);
  ak_uiaxistype a_main =
    ak_uielm_axis_main(elm);
  ak_uiaxistype a_cross =
    ak_uielm_axis_cross(elm);

  float p_main =
    *ak_uipos_axis(elm->poss, a_main);
  float p_cross =
    *ak_uipos_axis(elm->poss, a_cross);
  float s_main =
    *ak_uisize_axis(elm->sizes, a_main);
  float s_cross =
    *ak_uisize_axis(elm->sizes, a_cross);

  ak_fcnst_itchild it =
    ak_fcnst_itchild_make(tree, id);
  ak_uiid cid = 0;
  while (
    (cid = ak_fcnst_itchild_next(&it))) {
    ak_uielm* celm = ak_fcnst_at(tree, cid);

    float* c_p_cross =
      ak_uipos_axis(celm->poss, a_cross);
    *c_p_cross = p_cross;

    float* c_p_main =
      ak_uipos_axis(celm->poss, a_main);
  }
}

//--- dir ---//
void
ak_ui_layout(ak_ui* ui,
             ak_uilstg* lstg,
             uint32_t width,
             uint32_t height)
{
  ak_fcnst* tree = ak_ui_tree(ui);

  ak_fcnst_itbfs_reset(
    &lstg->it, tree, ak_fcnst_root(tree));
}

/*


static void
bound_set(ak_fcnst* tree, ak_uiid id)
{
  ak_uielm* elm = ak_fcnst_at(tree, id);
  ak_uiaxistype axis_main =
    ak_uielm_axis_main(elm);
  ak_uiaxistype axis_cross =
    ak_uielm_axis_cross(elm);

  ak_uibound b_main = ak_uibound_from_cnst(
    *ak_uielm_cnst_axis(elm, axis_main));
  ak_uibound b_cross = ak_uibound_from_cnst(
    *ak_uielm_cnst_axis(elm, axis_cross));

  ak_fcnst_itchild it =
    ak_fcnst_itchild_make(tree, id);
  ak_uiid cid = 0;
  while (
    (cid = ak_fcnst_itchild_next(&it))) {
    ak_uielm* celm = ak_fcnst_at(tree, cid);

    ak_uibound c_b_main =
      ak_uibound_from_cnst(
        *ak_uielm_cnst_axis(celm,
                            axis_main));
    ak_uibound c_b_cross =
      ak_uibound_from_cnst(
        *ak_uielm_cnst_axis(celm,
                            axis_cross));

    b_main.min += c_b_main.min;
    b_main.max += c_b_main.max;

    b_cross.min =
      ak_max(b_cross.min, c_b_cross.min);
    b_cross.max =
      ak_max(b_cross.max, c_b_cross.max);
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
*/
