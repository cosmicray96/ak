#include "ak/gfx/gcb.h"
#include "ak/core/math/mat3x3.h"
#include "ak/debug.h"
#include "ak/gfx/gfx.h"
#include "ak/gfx/reses/shader.h"
#include "ak/res/reg.h"

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
ak_gcb_flush(ak_gcb* gcb,
             ak_gfx* gfx,
             ak_resreg* rr)
{

  ak_gfx_frame_begin(gfx);
  uint32_t count = ak_da_count(&gcb->cmds);
  ak_gfx_batchdata bd = { 0 };
  bool has_mtrlin = false;
  ak_shader* shader = 0;

  for (uint32_t i = 0; i < count; i++) {
    ak_gcbcmd* cmd = ak_da_at(&gcb->cmds, i);

    switch (cmd->type) {
      case ak_gcbcmdtype_batch: {
        bd = cmd->batch.bd;
        if (!has_mtrlin) {
          has_mtrlin = true;
        }
        break;
      }
      case ka_gcbcmdtype_call: {
        ak_assert(has_mtrlin);
        if (shader) {
          ak_shader_end(shader);
          shader = 0;
        }
        shader = ak_resreg_get(
          rr, cmd->call.cd.shaderid);
        ak_shader_begin(
          shader, rr, &bd, &cmd->call.cd);
        break;
      }
      case ak_gcbcmdtype_quad: {
        ak_assert(shader);
        ak_shader_pushquad(shader,
                           &cmd->quad.qd,
                           &cmd->quad.gmat);
        break;
      }
      case ak_gcbcmdtype_scissor: {
        ak_gfx_scissor_set(gfx,
                           cmd->scissor.x,
                           cmd->scissor.y,
                           cmd->scissor.w,
                           cmd->scissor.h);
        break;
      }
      case ak_gcbcmdtype_scissor_reset: {
        ak_gfx_scissor_reset(gfx);
        break;
      }
      case ak_gcbcmdtype_resize: {
        if (shader) {
          ak_shader_end(shader);
          shader = 0;
        }
        ak_gfx_resize(
          gfx, cmd->resize.w, cmd->resize.h);
        break;
      }
      default: {
        ak_assert(false);
        break;
      }
    }
  }
  if (shader) {
    ak_shader_end(shader);
    shader = 0;
  }

  ak_gfx_frame_end(gfx);
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
                   uint32_t w,
                   uint32_t h)
{
  ak_gcbcmd ci = {
    .type = ak_gcbcmdtype_resize,
    .resize = { .w = w, .h = h }
  };
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
