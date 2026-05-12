#define _POSIX_C_SOURCE 200112L

#include "ak/program/program.h"
#include "ak/debug.h"
#include "ak/debug_itn.h"
#include "ak/program/program_itn.h"

#include <stdint.h>
#include <string.h>

#include <errno.h>
#include <time.h>
#include <unistd.h>

void
ak_program_crash_fatal()
{
  ak_log_crash_fatal();
  _exit(1);
}

void
ak_thread_sleep(ak_dur t)
{
  if (ak_dur_zero(t)) {
    return;
  }

  uint64_t ns = t.nanos;

  struct timespec ts = { .tv_sec =
                           ns / 1000000000,
                         .tv_nsec =
                           ns % 1000000000 };
  struct timespec rem;
  while (nanosleep(&ts, &rem) == -1) {
    if (errno == EINTR) {
      ts = rem;
    } else {
      ak_log("%s", strerror(errno));
      ak_ec(ak_err_sleep);
    }
  }
}
