#include "ak/gfx/gcb.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/mat3x3.h"
#include "ak/gfx/mtrl/stg.h"
#include "ak/gfx/mtrl_itn.h"

//===== ak_gcb =====//
//--- private ---//
typedef struct
{
  ak_mtrl_basedata bd;
  uint32_t offset_begin;
  uint32_t offset_end;
} mtrl_item;

typedef struct
{
  ak_mtrl_quaddata qd;
  ak_mat3 gmat;
} quad_item;

//--- internal ---//
ak_gcb
ak_gcb_make(ak_gfx* gfx,
            ak_mtrlstg* ms,
            ak_alct alct)
{
  ak_gcb gcb = { 0 };
  gcb.gfx = gfx;
  gcb.ms = ms;
  gcb.quads =
    ak_da_make(sizeof(quad_item), alct);
  gcb.mtrls =
    ak_da_make(sizeof(mtrl_item), alct);

  return gcb;
}

void
ak_gcb_destroy(ak_gcb* gcb)
{
  ak_da_destroy(&gcb->mtrls);
  ak_da_destroy(&gcb->quads);
  gcb->gfx = 0;
  gcb->ms = 0;
}

void
ak_gcb_begin(ak_gcb* gcb,
             const ak_mat3* vp,
             ak_fx time)
{
  ak_da_clear(&gcb->mtrls);
  ak_da_clear(&gcb->quads);
  gcb->vp = *vp;
  gcb->time = time;
}

void
ak_gcb_flush(ak_gcb* gcb)
{
  uint32_t mcount = ak_da_count(&gcb->mtrls);
  {
    if (mcount > 0) {
      mtrl_item* mi = ak_da_at_impl(
        &gcb->mtrls, mcount - 1);
      mi->offset_end =
        ak_da_count(&gcb->quads);
    } else {
      return;
    }
  }

  for (uint32_t i = 0; i < mcount; i++) {
    mtrl_item* mi =
      ak_da_at_impl(&gcb->mtrls, i);
    ak_mtrl m =
      ak_mtrlstg_at(gcb->ms, mi->bd.me);
    m.call_begin(
      m.ctx, &mi->bd, &gcb->vp, gcb->time);

    uint32_t cur_quad = mi->offset_begin;
    while (cur_quad < mi->offset_end) {
      quad_item* qi =
        ak_da_at_impl(&gcb->quads, cur_quad);
      m.push_quad(m.ctx, &qi->qd, &qi->gmat);
      cur_quad++;
    }

    m.call_end(m.ctx);
  }
}

void
ak_gcb_push_mtrl(ak_gcb* gcb,
                 const ak_mtrl_basedata* bd)
{
  if (ak_da_count(&gcb->mtrls) > 0) {
    mtrl_item* mi = ak_da_at_impl(
      &gcb->mtrls,
      ak_da_count(&gcb->mtrls) - 1);
    mi->offset_end =
      ak_da_count(&gcb->quads);
  }
  mtrl_item item =
    (mtrl_item){ .bd = *bd,
                 .offset_begin =
                   ak_da_count(&gcb->quads),
                 .offset_end = 0 };
  ak_da_pushback(&gcb->mtrls, &item);
}

void
ak_gcb_push_quad(ak_gcb* gcb,
                 const ak_mtrl_quaddata* qd,
                 const ak_mat3* gmat)
{
  ak_assert(ak_da_count(&gcb->mtrls) > 0);
  quad_item qi = { .qd = *qd,
                   .gmat = *gmat };
  ak_da_pushback(&gcb->quads, &qi);
}
