#include "ak/ak.h"
#include "ak/app/app.h"
#include "ak/app/layers/core.h"
#include "ak/app/layers/simple.h"
#include "ak/app/layers/world.h"
#include "ak/core/mem/heap.h"
#include "ak/program/program.h"

void
ak()
{
  ak_program_startup();

  ak_heap heap = ak_heap_make();
  ak_alct alct = ak_heap_to_alct(&heap);

  ak_lcore* core = ak_lcore_make(alct);
  ak_applayer lcore =
    ak_lcore_to_applayer(core);

  ak_lsimple* simple =
    ak_lsimple_make(core, alct);
  ak_applayer lsimple =
    ak_lsimple_to_applayer(simple);

  ak_applayer_regs regs =
    ak_applayer_regs_make(alct);
  ak_applayer_regs_push(&regs, lcore);
  ak_applayer_regs_push(&regs, lsimple);

  ak_app* app = ak_app_make(&regs, alct);
  ak_app_run(app);
  ak_app_destroy(app);

  ak_applayer_regs_destroy(&regs);

  ak_lsimple_destroy(simple);
  ak_lcore_destroy(core);

  ak_heap_destroy(&heap);

  ak_program_shutdown();
}
