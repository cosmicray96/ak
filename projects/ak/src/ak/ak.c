#include "ak/ak.h"
#include "ak/app/core.h"
#include "ak/app/impls/simple.h"
#include "ak/app/runner.h"
#include "ak/program/program.h"

void
ak()
{
  ak_program_startup();
  ak_delta_set(ak_dur_from_millis(30));

  ak_lsimple* l = ak_lsimple_make();
  ak_runner(&ak_lsimple_tick, l, ak_delta());
  ak_lsimple_destroy(l);

  ak_program_shutdown();
}
