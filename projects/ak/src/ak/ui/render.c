#include "ak/coll/fcnst.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/math/vec4f.h"
#include "ak/gfx/gfx.h"
#include "ak/ui/core.h"
#include "ak/ui/ui.h"
#include "ak/ui/ui_dir.h"

//===== render =====//
//--- private ---//
static void
scissor_push(ak_uirstg* rstg,
             ak_gcb* gcb,
             ak_vec4f rect)
{
  ak_vec4f r_last = { 0 };
  ak_vec4f r = { 0 };
  if (ak_dq_peek(
        &rstg->scissors, 0, &r_last)) {
    r = ak_ui_rectintersect(rect, r_last);
  } else {
    r = rect;
  }
  ak_dq_push(&rstg->scissors, &r);
  ak_gcb_push_scissor(
    gcb, r.rectx, r.recty, r.rectw, r.recth);
}

static void
scissor_pop(ak_uirstg* rstg, ak_gcb* gcb)
{
  ak_vec4f r = { 0 };
  ak_assert(ak_dq_pop(&rstg->scissors, &r));

  ak_vec4f r_last = { 0 };
  if (ak_dq_peek(
        &rstg->scissors, 0, &r_last)) {
    ak_gcb_push_scissor(gcb,
                        r_last.rectx,
                        r_last.recty,
                        r_last.rectw,
                        r_last.recth);
  } else {
    ak_gcb_push_scissor_reset(gcb);
  }
}

static void
elm_render_bg(ak_uirstg* rstg,
              ak_gcb* gcb,
              ak_fcnst* tree,
              ak_uiid id)
{
  ak_uielm* elm = ak_fcnst_at(tree, id);

  if (elm->clipping) {
    scissor_push(rstg,
                 gcb,
                 (ak_vec4f){
                   .rectx = elm->pos.x,
                   .recty = elm->pos.y,
                   .rectw = elm->size.x,
                   .recth = elm->size.y,
                 });
  }

  if (elm->visible) {

    ak_mat3_f m =
      ak_mat3_from_rect(elm->pos.x,
                        elm->pos.y,
                        elm->size.x,
                        elm->size.y);

    ak_gcb_push_quad(gcb, &elm->qd, &m);
  }

  ak_fcnst_itchild it =
    ak_fcnst_itchild_make(tree, id);
  ak_uiid cid = 0;
  while (
    (cid = ak_fcnst_itchild_next(&it))) {
    elm_render_bg(rstg, gcb, tree, cid);
  }

  if (elm->clipping) {
    scissor_pop(rstg, gcb);
  }
}

//--- dir ---//
ak_uirstg
ak_uirstg_make(ak_alct alct)
{
  ak_uirstg rstg = { 0 };
  rstg.scissors =
    ak_dq_make(sizeof(ak_vec4f), alct);
  rstg.mtrl_b =
    (ak_mtrl_basedata){ .shaderid = 16 };
  return rstg;
}
void
ak_uirstg_destroy(ak_uirstg* rstg)
{
  ak_dq_destroy(&rstg->scissors);
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

  elm_render_bg(&ui->rstg,
                gcb,
                &ui->tree,
                ak_fcnst_root(&ui->tree));
}
