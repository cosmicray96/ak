#include "ak/ak.h"
#include "ak/app/impls/simple.h"
#include "ak/app/runner.h"
#include "ak/program/program.h"
#include "ak_android/android.h"

void
ak()
{
  ak_program_startup();

  ak_lsimple* l = ak_lsimple_make();

  ak_runner(&ak_lsimple_tick,
            l,
            ak_dur_from_millis(30));

  ak_lsimple_destroy(l);

  ak_program_shutdown();
}
