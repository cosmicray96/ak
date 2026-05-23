#include "ak/gfx/gcb.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/mat3x3.h"
#include "ak/gfx/gfx.h"
#include "ak/gfx/mtrl/stg.h"
#include "ak/gfx/mtrl_itn.h"

//===== ak_gcb =====//
//--- private ---//
typedef struct
{
  ak_mtrl_basedata bd;
  ak_mat3_f vp;
  ak_fx time;
} mtrl_item;

typedef struct
{
  ak_mtrl_quaddata qd;
  ak_mat3 gmat;
} quad_item;
typedef struct
{
  int32_t x;
  int32_t y;
  uint32_t w;
  uint32_t h;
} scissor_item;
typedef enum
{
  cmdtype_quad,
  cmdtype_mtrl,
  cmdtype_scissor,
  cmdtype_scissor_reset,
  cmdtype_resize,
} cmdtype;
typedef struct
{
  cmdtype type;
  union
  {
    mtrl_item mi;
    quad_item qi;
    scissor_item si;
    struct resize_item
    {
      uint32_t w;
      uint32_t h;
    } ri;
  };
} cmd_item;

//--- internal ---//
ak_gcb
ak_gcb_make(ak_mtrlstg* ms, ak_alct alct)
{
  ak_gcb gcb = { 0 };
  gcb.ms = ms;
  gcb.cmds =
    ak_da_make(sizeof(cmd_item), alct);
  return gcb;
}

void
ak_gcb_destroy(ak_gcb* gcb)
{
  ak_da_destroy(&gcb->cmds);
  gcb->ms = 0;
}

void
ak_gcb_flush(ak_gcb* gcb, ak_gfx* gfx)
{

  ak_gfx_frame_begin(gfx);
  uint32_t count = ak_da_count(&gcb->cmds);
  ak_mtrl m = { 0 };
  bool has_mtrl = false;
  for (uint32_t i = 0; i < count; i++) {
    cmd_item* ci =
      ak_da_at_impl(&gcb->cmds, i);

    switch (ci->type) {
      case cmdtype_mtrl: {
        if (has_mtrl) {
          m.call_end(m.ctx);
        }
        m = ak_mtrlstg_at(gcb->ms,
                          ci->mi.bd.me);
        m.call_begin(m.ctx,
                     &ci->mi.bd,
                     &(ci->mi.vp),
                     ci->mi.time);
        has_mtrl = true;
        break;
      }
      case cmdtype_quad: {
        ak_assert(has_mtrl);
        m.push_quad(
          m.ctx, &ci->qi.qd, &ci->qi.gmat);
        break;
      }
      case cmdtype_scissor: {
        ak_gfx_scissor_set(gfx,
                           ci->si.x,
                           ci->si.y,
                           ci->si.w,
                           ci->si.h);
        break;
      }
      case cmdtype_scissor_reset: {
        ak_gfx_scissor_reset(gfx);
        break;
      }
      case cmdtype_resize: {
        ak_gfx_resize(
          gfx, ci->ri.w, ci->ri.h);
        break;
      }
      default: {
        ak_assert(false);
        break;
      }
    }
  }
  if (has_mtrl) {
    m.call_end(m.ctx);
  }

  ak_gfx_frame_end(gfx);
  ak_da_clear(&gcb->cmds);
}

void
ak_gcb_push_resize(ak_gcb* gcb,
                   uint32_t w,
                   uint32_t h)
{
  cmd_item ci = { .type = cmdtype_resize,
                  .ri = { .w = w, .h = h } };
  ak_da_pushback(&gcb->cmds, &ci);
}

void
ak_gcb_push_mtrl(ak_gcb* gcb,
                 const ak_mtrl_basedata* bd,
                 const ak_mat3_f* vp,
                 ak_fx time)
{

  cmd_item ci = { .type = cmdtype_mtrl,
                  .mi = { .bd = *bd,
                          .vp = *vp,
                          .time = time } };
  ak_da_pushback(&gcb->cmds, &ci);
}

void
ak_gcb_push_quad(ak_gcb* gcb,
                 const ak_mtrl_quaddata* qd,
                 const ak_mat3* gmat)
{
  cmd_item ci = { .type = cmdtype_quad,
                  .qi = { .qd = *qd,
                          .gmat = *gmat } };
  ak_da_pushback(&gcb->cmds, &ci);
}

void
ak_gcb_push_scissor_reset(ak_gcb* gcb)
{
  cmd_item ci = {
    .type = cmdtype_scissor_reset,
  };
  ak_da_pushback(&gcb->cmds, &ci);
}
void
ak_gcb_push_scissor(ak_gcb* gcb,
                    int32_t x,
                    int32_t y,
                    uint32_t w,
                    uint32_t h)
{
  cmd_item ci = {
    .type = cmdtype_scissor,
    .si = { .x = x, .y = y, .w = w, .h = h }
  };
  ak_da_pushback(&gcb->cmds, &ci);
}
