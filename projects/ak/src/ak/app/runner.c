#include "ak/app/runner.h"
#include "ak/os/time.h"
#include "ak/program/program.h"

void
ak_runner(bool (*fn)(void* ctx),
          void* ctx,
          ak_dur frametime)
{
  ak_dur deadline = ak_dur_now();

  while (true) {
    deadline =
      ak_dur_add(deadline, frametime);

    bool exit = fn(ctx);
    if (exit) {
      break;
    }

    ak_dur now = ak_dur_now();
    ak_dur sleeptime =
      ak_dur_subtract(deadline, now);
    ak_this_thread_sleep(sleeptime);
  }
}
