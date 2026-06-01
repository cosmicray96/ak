#include "ak/core/math/mat3x3.h"
#include "ak/gfx/gfx.h"
#include "ak/gfx/mtrl.h"
#include "ak/ui/ui.h"
#include "ak/ui/ui_dir.h"

static void
elm_render_bg(ak_uirstg* rstg,
              ak_gcb* gcb,
              ak_fcnst* tree,
              ak_uiid id)
{
  ak_uielm* elm =
    ak_fcnst_at(tree, ak_fcnst_root(tree));

  ak_mtrl_quaddata qd = {
    .me = ak_mtrl_ui_e,
    .ui = { .col = elm->color }
  };

  ak_mat3_f m =
    ak_mat3_from_rect(elm->pos.x,
                      elm->pos.y,
                      elm->size.x,
                      elm->size.y);

  ak_gcb_push_quad(gcb, &qd, &m);

  // recurse down the tree
}

void
ak_ui_render(ak_ui* ui, ak_gcb* gcb)
{
  ak_uielm* elm = ak_fcnst_at(
    &ui->tree, ak_fcnst_root(&ui->tree));

  ak_mat3_f vp = ak_gfx_vp_ui_make(
    elm->size.x, elm->size.y);
  ak_mtrl_indata mid = { .vp = vp };
  ak_gcb_push_mtrlin(gcb, &mid);

  ak_gcb_push_mtrl(gcb, &ui->rstg.mtrl_b);
}
