#include "ak/gfx/gcb.h"
#include "ak/gfx/reses/shader.h"
#include "ak/res/reg.h"
#include "ak_opengl/gfx/gcb.h"
#include "ak_opengl/gfx/gcore.h"

//--- impl ---//
void
ak_gcb_flush(ak_gcb* gcb,
             ak_gcore* gcore,
             ak_resreg* rr)
{
  ak_opengl_gcore_frame_begin(gcore);
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
        ak_opengl_gcore_scissor_set(
          gcore,
          cmd->scissor.x,
          cmd->scissor.y,
          cmd->scissor.w,
          cmd->scissor.h);
        break;
      }
      case ak_gcbcmdtype_scissor_reset: {
        ak_opengl_gcore_scissor_reset(gcore);
        break;
      }
      case ak_gcbcmdtype_resize: {
        if (shader) {
          ak_shader_end(shader);
          shader = 0;
        }
        ak_opengl_gcore_resize(
          gcore,
          cmd->resize.w,
          cmd->resize.h);
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

  ak_opengl_gcore_frame_end(gcore);
  ak_da_clear(&gcb->cmds);
}
