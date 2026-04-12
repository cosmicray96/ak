#include "ak/app/app.h"
#include "ak/debug.h"

ak_applayer_regs
ak_applayer_regs_make(ak_alct alct)
{
  ak_applayer_regs alr;
  alr.layers = ak_dq_make(
    sizeof(ak_applayer), 10, alct);
  alr.popping = false;
  return alr;
}
void
ak_applayer_regs_destroy(
  ak_applayer_regs* alr)
{
  ak_dq_destroy(&alr->layers);
  alr->popping = false;
}

void
ak_applayer_regs_push(ak_applayer_regs* alr,
                      const ak_applayer* l)
{
  ak_assert(!alr->popping);
  ak_dq_push(&alr->layers, l);
}

bool
ak_applayer_regs_pop(ak_applayer_regs* alr,
                     ak_applayer* l)
{
  alr->popping = true;
  return ak_dq_pop(&alr->layers, l);
}

//===== ak_app  =====//
ak_app*
ak_app_make(ak_applayer_regs* regs);
void
ak_app_destroy(ak_app* a);

void
ak_app_run(ak_app* a);

void
ak_app_event_push(ak_app* a, ak_event e);
