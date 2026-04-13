#define _POSIX_C_SOURCE 200112L
#include "ak/os/time.h"

#include <time.h>

//===== ak_time =====//
//--- public ---//
ak_dur
ak_dur_now()
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  ak_dur d = { 0 };
  d.nanos =
    (uint64_t)ts.tv_sec * 1000000000ULL +
    (uint64_t)ts.tv_nsec;

  return d;
}
