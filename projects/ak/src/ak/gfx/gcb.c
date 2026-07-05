#include "ak/gfx/gcb.h"
#include "ak/core/math/mat3x3.h"

//===== ak_gcb =====//
//--- private ---//

//--- internal ---//
ak_gcb
ak_gcb_make(ak_alct alct)
{
  ak_gcb gcb = { 0 };
  gcb.cmds =
    ak_da_make(sizeof(ak_gcbcmd), alct);
  return gcb;
}

void
ak_gcb_destroy(ak_gcb* gcb)
{
  ak_da_destroy(&gcb->cmds);
}

void
ak_gcb_clear(ak_gcb* gcb)
{
  ak_da_clear(&gcb->cmds);
}

void
ak_gcb_joinback(ak_gcb* dest, ak_gcb* src)
{
  ak_da_bulk_pushback(
    &dest->cmds,
    &src->cmds,
    0,
    ak_da_count(&src->cmds));

  ak_da_clear(&src->cmds);
}

void
ak_gcb_push(ak_gcb* gcb,
            const ak_gcbcmd* cmd)
{
  ak_da_pushback(&gcb->cmds, &cmd);
}

void
ak_gcb_push_resize(ak_gcb* gcb,
                   uint32_t x,
                   uint32_t y,
                   uint32_t w,
                   uint32_t h)
{
  ak_gcbcmd ci = { .type =
                     ak_gcbcmdtype_resize,
                   .resize = { .x = x,
                               .y = y,
                               .w = w,
                               .h = h } };
  ak_da_pushback(&gcb->cmds, &ci);
}

void
ak_gcb_push_batch(ak_gcb* gcb,
                  const ak_gfx_batchdata* bd)
{
  ak_gcbcmd ci = { .type =
                     ak_gcbcmdtype_batch,
                   .batch = { .bd = *bd } };
  ak_da_pushback(&gcb->cmds, &ci);
}

void
ak_gcb_push_call(ak_gcb* gcb,
                 const ak_gfx_calldata* cd)
{

  ak_gcbcmd ci = { .type =
                     ka_gcbcmdtype_call,
                   .call = {
                     .cd = *cd,
                   } };
  ak_da_pushback(&gcb->cmds, &ci);
}

void
ak_gcb_push_quad(ak_gcb* gcb,
                 const ak_gfx_quaddata* qd,
                 const ak_mat3_f* gmat)
{
  ak_gcbcmd ci = {
    .type = ak_gcbcmdtype_quad,
    .quad = { .qd = *qd, .gmat = *gmat }
  };
  ak_da_pushback(&gcb->cmds, &ci);
}

void
ak_gcb_push_scissor_reset(ak_gcb* gcb)
{
  ak_gcbcmd ci = {
    .type = ak_gcbcmdtype_scissor_reset,
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
  ak_gcbcmd ci = { .type =
                     ak_gcbcmdtype_scissor,
                   .scissor = { .x = x,
                                .y = y,
                                .w = w,
                                .h = h } };
  ak_da_pushback(&gcb->cmds, &ci);
}
