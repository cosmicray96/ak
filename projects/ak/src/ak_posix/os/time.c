#define _POSIX_C_SOURCE 200112L
#include "ak/os/time.h"

#include <time.h>

//===== ak_time =====//
//--- private ---//
typedef struct
{
  ak_dur start;
} timeitn;
timeitn s_timeitn = { 0 };

static ak_dur
ak_dur_now_itn()
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  ak_dur d = { 0 };
  d.nanos =
    (uint64_t)ts.tv_sec * 1000000000ULL +
    (uint64_t)ts.tv_nsec;

  return d;
}

//--- public ---//
void
ak_time_startup()
{
  s_timeitn.start = ak_dur_now_itn();
}
void
ak_time_shutdown()
{
  // empty
}

ak_dur
ak_dur_now()
{
  ak_dur d = ak_dur_now_itn();
  return ak_dur_diff(d, s_timeitn.start);
}
