#include "ak/ui/core.h"
#include "ak/ui/ui_dir.h"

//===== layout =====//
//--- private ---//
static void
set_layout(ak_uilstg* lstg,
           ak_fcnst* tree,
           ak_uiid id)
{
  ak_uielm* elm = ak_fcnst_at(tree, id);
  ak_uiaxistype axis_m =
    ak_uielm_axis_main(elm);
  ak_uiaxistype axis_c =
    ak_uielm_axis_cross(elm);

  float pt_pos_m =
    *ak_uipos_axis(elm->poss, axis_m);
  float pt_pos_c =
    *ak_uipos_axis(elm->poss, axis_c);
  float pt_size_m =
    *ak_uisize_axis(elm->sizes, axis_m);
  float pt_size_c =
    *ak_uisize_axis(elm->sizes, axis_c);

  float cur_main = pt_pos_m;

  ak_fcnst_itchild it =
    ak_fcnst_itchild_make(tree, id);
  ak_uiid cid = 0;
  while (
    (cid = ak_fcnst_itchild_next(&it))) {
    ak_uielm* celm = ak_fcnst_at(tree, cid);

    ak_cnst* c_cnst_m =
      ak_cnst_axis(celm->cnsts, axis_m);
    ak_cnst* c_cnst_c =
      ak_cnst_axis(celm->cnsts, axis_c);

    float* c_pos_m =
      ak_uipos_axis(celm->poss, axis_m);
    float* c_pos_c =
      ak_uipos_axis(celm->poss, axis_c);
    float* c_size_m =
      ak_uisize_axis(celm->sizes, axis_m);
    float* c_size_c =
      ak_uisize_axis(celm->sizes, axis_c);

    *c_size_m =
      ak_cnst_pt(*c_cnst_m, pt_size_m);
    *c_size_c =
      ak_cnst_pt(*c_cnst_c, pt_size_c);

    *c_pos_m = cur_main;
    *c_pos_c = pt_pos_c;

    cur_main += *c_size_m;

    set_layout(lstg, tree, cid);
  }
}

//--- dir ---//
ak_uilstg
ak_uilstg_make(ak_alct alct)
{
  return (ak_uilstg){ 0 };
}

void
ak_uilstg_destroy(ak_uilstg* lstg)
{
  // empty
}
void
ak_ui_layout(ak_ui* ui,
             float x,
             float y,
             float w,
             float h)
{
  ak_fcnst* tree = &ui->tree;
  ak_uiid root_id = ak_fcnst_root(tree);
  ak_uielm* elm = ak_fcnst_at(tree, root_id);

  float* elm_x =
    ak_uipos_axis(elm->poss, ak_uiaxis_h);
  float* elm_y =
    ak_uipos_axis(elm->poss, ak_uiaxis_v);
  float* elm_w =
    ak_uisize_axis(elm->sizes, ak_uiaxis_h);
  float* elm_h =
    ak_uisize_axis(elm->sizes, ak_uiaxis_v);

  *elm_x = x;
  *elm_y = y;
  *elm_w = w;
  *elm_h = h;

  set_layout(&ui->lstg, tree, root_id);
}
